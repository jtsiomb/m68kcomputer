#include "config.h"

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "ds1302rtc.h"

#define CLOCK(x) \
	do { if(x) PORTB |= PB_RTC_CLK_BIT; else PORTB &= ~PB_RTC_CLK_BIT; } while(0)
#define CE(x) \
	do { if(x) PORTD |= PD_RTC_CE_BIT; else PORTD &= ~PD_RTC_CE_BIT; } while(0)
#define SET_IO(x) \
	do { if(x) PORTB |= PB_RTC_DATA_BIT; else PORTB &= ~PB_RTC_DATA_BIT; } while(0)
#define GET_IO ((PINB & PB_RTC_DATA_BIT) ? 1 : 0)
#define DDR_IO(x) \
	do { if(x) DDRB |= PB_RTC_DATA_BIT; else DDRB &= ~PB_RTC_DATA_BIT; } while(0)

#define IN	0
#define OUT	1

#define CMD_RAM_BIT		0x40
#define CMD_RD_BIT		0x01

#define REG_SEC		0x80
#define REG_MIN		0x82
#define REG_HOUR	0x84
#define REG_DOM		0x86
#define REG_MONTH	0x88
#define REG_DOW		0x8a
#define REG_YEAR	0x8c
#define REG_WPROT	0x8e
#define REG_CHARGER	0x90

#define SEC_CLOCK_HALT	0x80
#define HOUR_12MODE_BIT	0x80
#define WPROT_BIT		0x80

static void write_reg(int reg, unsigned char byte);
static unsigned char read_reg(int reg);
static void send_byte(unsigned char byte);
static unsigned char recv_byte(void);

void rtc_init(void)
{
	unsigned char tmp;

	write_reg(REG_WPROT, 0);
	write_reg(REG_CHARGER, 0);

	tmp = read_reg(REG_SEC);
	write_reg(REG_SEC, tmp & ~SEC_CLOCK_HALT);

	tmp = read_reg(REG_HOUR);
	write_reg(REG_HOUR, tmp & ~HOUR_12MODE_BIT);
}

void rtc_set_time(int hour, int min, int sec)
{
	unsigned char hval, mval, sval;

	hval = (hour % 10) | ((hour / 10) << 4);
	mval = (min % 10) | ((min / 10) << 4);
	sval = (sec % 10) | ((sec / 10) << 4);

	write_reg(REG_WPROT, 0);

	write_reg(REG_SEC, sval);
	write_reg(REG_MIN, mval);
	write_reg(REG_HOUR, hval);

	write_reg(REG_WPROT, WPROT_BIT);
}

void rtc_get_time(struct rtc_time *tm)
{
	unsigned char hval, mval, sval;

	sval = read_reg(REG_SEC);
	mval = read_reg(REG_MIN);
	hval = read_reg(REG_HOUR);

	tm->hour = (hval & 0x0f) + (((hval >> 4) & 3) * 10);
	tm->min = (mval & 0x0f) + (((mval >> 4) & 7) * 10);
	tm->sec = (sval & 0x0f) + (((sval >> 4) & 7) * 10);
}

void rtc_set_date(int year, int month, int day)
{
	unsigned char yval, mval, dval;

	year -= 2000;
	yval = (year % 10) | ((year / 10) << 4);
	mval = (month % 10) | ((month / 10) << 4);
	dval = (day % 10) | ((day / 10) << 4);

	write_reg(REG_WPROT, 0);

	write_reg(REG_YEAR, yval);
	write_reg(REG_MONTH, mval);
	write_reg(REG_DOM, dval);

	write_reg(REG_WPROT, WPROT_BIT);
}

void rtc_set_weekday(int dow)
{
	write_reg(REG_WPROT, 0);
	write_reg(REG_DOW, dow);
	write_reg(REG_WPROT, 1);
}

void rtc_get_date(struct rtc_date *date)
{
	unsigned char yval, mval, dval, wval;

	yval = read_reg(REG_YEAR);
	mval = read_reg(REG_MONTH);
	dval = read_reg(REG_DOM);
	wval = read_reg(REG_DOW);

	date->year = (yval & 0x0f) + ((yval >> 4) * 10) + 2000;
	date->month = (mval & 0x0f) + (((mval >> 4) & 1) * 10);
	date->day = (dval & 0x0f) + (((dval >> 4) & 3) * 10);
	date->dow = wval & 7;
}

void rtc_store(int addr, unsigned char byte)
{
	write_reg(addr | 0x80 | CMD_RAM_BIT, byte);
}

unsigned char rtc_load(int addr)
{
	return read_reg(addr | 0x80 | CMD_RAM_BIT);
}

#define DATA_CLK_DELAY asm volatile("nop\n\tnop\n\t")

static void write_reg(int reg, unsigned char byte)
{
	CLOCK(0);
	CE(0);

	CE(1);
	send_byte(reg);
	send_byte(byte);
	CE(0);
}

static unsigned char read_reg(int reg)
{
	unsigned char res = 0;
	reg |= CMD_RD_BIT;

	CLOCK(0);
	CE(0);

	CE(1);
	send_byte(reg);
	res = recv_byte();
	CE(0);
	return res;
}

static void send_byte(unsigned char byte)
{
	int i;

	DDR_IO(OUT);

	for(i=0; i<8; i++) {
		CLOCK(0);
		_delay_us(0.25);
		SET_IO(byte & 1);
		DATA_CLK_DELAY;
		CLOCK(1);
		_delay_us(0.25);
		byte >>= 1;
	}
}

static unsigned char recv_byte(void)
{
	int i;
	unsigned char val = 0;

	DDR_IO(IN);

	for(i=0; i<8; i++) {
		CLOCK(0);
		_delay_us(0.25);
		val = (val >> 1) | (GET_IO ? 0x80 : 0);
		CLOCK(1);
		_delay_us(0.25);
	}
	return val;
}

