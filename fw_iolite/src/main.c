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

enum {
	REG_UART_BUFFER,
	REG_CTL_CMD,
	REG_CTL_DATA
};

void init(void);
void do_read(unsigned char addr);
void do_write(unsigned char addr);
void pulse_dtack(void);
unsigned char rdata(void);
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

	printf("\nI/O board lite debug\n");
	printf("%s %02d/%02d/%d - %02d:%02d.%02ds\n", day_of_week(date.dow), date.day,
			date.month, date.year, tm.hour, tm.min, tm.sec);

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
	init_serial(38400);
	sei();

	rtc_init();
}

void do_read(unsigned char addr)
{
	unsigned char data;

	switch(addr) {
	case 0: /* UART */
		data = getchar();
		/*printf("DBG: read(%x)->'%c'\n", (unsigned int)addr, data);*/
		/* also acknowledge the interrupt if it's pending and
		 * there are no more bytes to read
		 */
		cli();
		if(!have_input()) {
			PORTB |= PB_IRQ_UART_BIT;
		}
		sei();
		break;

	case 2:
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
	case 0:	/* UART */
		putchar(data);
		fflush(stdout);
		/*printf("DBG: write(%x)->'%c' (%x)\n", (unsigned int)addr, data, (unsigned int)data);*/
		break;

	case 1: /* cmd */
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

	case 2: /* data */
		break;

	case 3:	/* interrupt acknowledge */
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
		return ((int*)&date)[rtc_seq_idx++];

	case CMD_INVALID:
	default:
		break;
	}
	return 0xff;
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
