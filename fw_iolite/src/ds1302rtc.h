#ifndef DS1302RTC_H_
#define DS1302RTC_H_

struct rtc_time {
	int hour, min, sec;
};
struct rtc_date {
	int year, month, day, dow;
};

void rtc_init(void);
void rtc_set_time(int hour, int min, int sec);
void rtc_get_time(struct rtc_time *tm);
void rtc_set_date(int year, int month, int day);
void rtc_set_weekday(int dow);
void rtc_get_date(struct rtc_date *date);
void rtc_store(int addr, unsigned char byte);
unsigned char rtc_load(int addr);

#endif	/* DS1302RTC_H_ */
