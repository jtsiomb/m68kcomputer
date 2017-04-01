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
const char *day_of_week(int x);

int main(void)
{
	struct rtc_time tm;
	struct rtc_date date;

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
				printf("got read request for address %d\n", addr);
				PIND &= ~PD_DTACK_BIT;
				for(;;);
			} else {
				unsigned char data = (PINC & PC_DATA_MASK) | (PIND & PD_DATA_MASK);
				printf("got write for address %d: %d\n", addr, data);
			}
			_delay_ms(100);
			PORTD |= PD_DTACK_BIT;
			asm volatile ("nop\n\r");
			PORTD &= ~PD_DTACK_BIT;

			while((PIND & PD_ENABLE_BIT) == 0);
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

const char *day_of_week(int x)
{
	static const char *daystr[] = { "Monday", "Tuesday", "Wednesday",
		"Thursday", "Friday", "Saturday", "Sunday" };
	return daystr[x - 1];
}
