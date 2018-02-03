#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "serial.h"
#include "ds1302rtc.h"

void init(void);
void do_read(unsigned char addr);
void do_write(unsigned char addr);
void pulse_dtack(void);
unsigned char rdata(void);
void wdata(unsigned char data);
const char *day_of_week(int x);

unsigned char last_cmd;
unsigned char rtc_seq_idx = 0;

struct rtc_time tm;
struct rtc_date date;


int main(void)
{
	init();

	rtc_get_time(&tm);
	rtc_get_date(&date);

	/*
	printf("\r\nI/O board lite debug\r\n");
	printf("%s %02d/%02d/%d - %02d:%02d.%02ds\r\n", day_of_week(date.dow), date.day,
			date.month, date.year, tm.hour, tm.min, tm.sec);
	*/

	for(;;) {
		if((PIND & PD_ENABLE_BIT) == 0) {
			int addr = PINB & 3;
			if(PIND & PD_RDWR_BIT) {
				do_read(addr);
			} else {
				do_write(addr);
			}
		}
	}
	return 0;
}

void init(void)
{
	/* IRQ bits outputs, the rest inputs */
	DDRB = PB_IRQ_UART_BIT | PB_IRQ_TIMER_BIT | PB_RTC_CLK_BIT;
	PORTB = PB_IRQ_UART_BIT | PB_IRQ_TIMER_BIT; /* IRQ outputs high, inputs without pullups */
	DDRD = PD_DTACK_BIT | PD_RTC_CE_BIT; /* DTACK and RTC_CE are outputs, the rest are inputs */
	PORTD = 0;	/* all outputs low, the rest inputs without pullups */
	/* port C is the data bus: default to inputs without pullups */
	DDRC = 0;
	PORTC = 0;

	/* initialize the UART and enable interrupts */
	init_serial(115200);
	sei();

	rtc_init();
}

void do_read(unsigned char addr)
{
	unsigned char data;

	switch(addr) {
	case REG_UART_BUFFER:
		data = getchar();
		/*printf("DBG: %d", data);
		if(isprint(data)) {
			printf(" (%c)", data);
		}
		printf("\r\n");*/
		/* also acknowledge the interrupt if it's pending and
		 * there are no more bytes to read
		 */
		cli();
		if(!have_input()) {
			PORTB |= PB_IRQ_UART_BIT;
		}
		sei();
		break;

	case REG_CTL_DATA:
		data = rdata();
		break;

	default:
		data = 0xff;
	}

	/* drive data bus */
	DDRC = PC_DATA_MASK;
	DDRD |= PD_DATA_MASK;

	/* write the data to the data bus */
	PORTC = data;
	PORTD = (PORTD & ~PD_DATA_MASK) | (data & PD_DATA_MASK);

	pulse_dtack();
	while((PIND & PD_ENABLE_BIT) == 0);

	/* read done, tri-state data bus */
	DDRC = 0;
	DDRD &= ~PD_DATA_MASK;
}

void do_write(unsigned char addr)
{
	/* read the value off the data bus */
	unsigned char data = (PINC & PC_DATA_MASK) | (PIND & PD_DATA_MASK);

	switch(addr) {
	case REG_UART_BUFFER:
		putchar(data);
		fflush(stdout);
		/*printf("DBG: write(%x)->'%c' (%x)\n", (unsigned int)addr, data, (unsigned int)data);*/
		break;

	case REG_CTL_CMD:
		last_cmd = data;
		switch(data) {
		case CMD_TIME:
			rtc_get_time(&tm);
			rtc_seq_idx = 0;
			break;
		case CMD_DATE:
			rtc_get_date(&date);
			rtc_seq_idx = 0;
			break;
		default:
			break;
		}
		break;

	case REG_CTL_DATA:
		wdata(data);
		break;

	case REG_INT_ACK:	/* interrupt acknowledge */
		switch(data) {
		case 2:
			PORTB |= PB_IRQ_UART_BIT;
			break;
		case 7:
			PORTB |= PB_IRQ_TIMER_BIT;
			break;
		default:
			break;
		}
		break;
	}

	pulse_dtack();
	while((PIND & PD_ENABLE_BIT) == 0);
}

void pulse_dtack(void)
{
	PORTD |= PD_DTACK_BIT;
	asm volatile ("nop\n\t");
	PORTD &= ~PD_DTACK_BIT;
}

/* processor is reading from the data register */
unsigned char rdata(void)
{
	switch(last_cmd) {
	case CMD_TIME:
		return ((int*)&tm)[rtc_seq_idx++];

	case CMD_DATE:
		if(rtc_seq_idx == 0) {
			++rtc_seq_idx;
			return date.year - 2000;
		} else {
			return ((int*)&date)[rtc_seq_idx++];
		}

	case CMD_INVALID:
	default:
		break;
	}
	return 0xff;
}

/* processor is writing to the data register */
void wdata(unsigned char data)
{
	switch(last_cmd) {
	case CMD_TIME:
		((int*)&tm)[rtc_seq_idx] = data;
		if(++rtc_seq_idx >= 3) {
			rtc_set_time(tm.hour, tm.min, tm.sec);
			last_cmd = CMD_INVALID;
		}
		break;

	case CMD_DATE:
		((int*)&date)[rtc_seq_idx] = data;
		if(++rtc_seq_idx == 3) {
			date.year = 2000 + date.year;
			rtc_set_date(date.year, date.month, date.day);
		} else if(rtc_seq_idx >= 4) {
			rtc_set_weekday(date.dow);
			last_cmd = CMD_INVALID;
		}
		break;

	case CMD_INVALID:
	default:
		break;
	}
}

/* this is called by the UART interrupt to signal that we got some data
 * and we should interrupt the processor
 */
void recv_data_intr(void)
{
	/* raise a 68k interrupt */
	PORTB &= ~PB_IRQ_UART_BIT;
}

const char *day_of_week(int x)
{
	static const char *daystr[] = { "Monday", "Tuesday", "Wednesday",
		"Thursday", "Friday", "Saturday", "Sunday" };
	return daystr[x - 1];
}
