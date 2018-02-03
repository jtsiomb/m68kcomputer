#include "rtc.h"
#include "ioreg.h"

void rtc_set_time(int hour, int min, int sec)
{
	IOREG_CMD = CMD_TIME;
	IOREG_DATA = hour;
	IOREG_DATA = min;
	IOREG_DATA = sec;
}

void rtc_get_time(struct rtc_time *tm)
{
	IOREG_CMD = CMD_TIME;
	tm->hour = IOREG_DATA;
	tm->min = IOREG_DATA;
	tm->sec = IOREG_DATA;
}

void rtc_set_date(int year, int month, int day, int dow)
{
	IOREG_CMD = CMD_DATE;
	IOREG_DATA = year - 2000;
	IOREG_DATA = month;
	IOREG_DATA = day;
	IOREG_DATA = dow;
}

void rtc_get_date(struct rtc_date *date)
{
	IOREG_CMD = CMD_DATE;
	date->year = IOREG_DATA + 2000;
	date->month = IOREG_DATA;
	date->day = IOREG_DATA;
	date->dow = IOREG_DATA;
}
