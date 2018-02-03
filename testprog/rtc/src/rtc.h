#ifndef RTC_H_
#define RTC_H_

struct rtc_time {
	int hour, min, sec;
};
struct rtc_date {
	int year, month, day, dow;
};

void rtc_set_time(int hour, int min, int sec);
void rtc_get_time(struct rtc_time *tm);
void rtc_set_date(int year, int month, int day, int dow);
void rtc_get_date(struct rtc_date *date);

#endif	/* RTC_H_ */
