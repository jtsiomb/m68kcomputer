#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "intr.h"
#include "tctl.h"
#include "rtc.h"

void proc_input(char c);
int tokenize_input(void);
void runcmd(void);
const char *day_of_week(int x);

static char input[128];
static short cur;

static char *argv[64];
static short argc;

int main(void)
{
	int c;

	printf("RTC test program\n");
	init_intr();

	while((c = getchar()) != 4) {
		proc_input(c);
	}

	cleanup_intr();
	return 0;
}

void proc_input(char c)
{
	int i;

	switch(c) {
	case '\b':
	case 127:
		if(cur > 0) {
			--cur;
			if(c != '\b') {
				putchar('\b');
			}
			printf(" \b");
		}
		break;

	case 21:	/* ctrl-U */
		if(cur > 0) {
			printf(TCTL_KILL_HOME);
			cur = 0;
		}
		break;

	case '\n':
		input[cur] = 0;
		cur = 0;

		if(tokenize_input()) {
			runcmd();
		}
		break;

	default:
		if(cur < sizeof input - 1) {
			input[cur++] = c;
		}
	}
}

static char *skip_space(char *s)
{
	while(*s && isspace(*s)) ++s;
	return s;
}

int tokenize_input(void)
{
	char *ptr = skip_space(input);
	if(!*ptr) {
		argc = 0;
		argv[0] = 0;
		return 0;
	}

	argc = 1;
	argv[0] = ptr;

	while(*ptr) {
		if(isspace(*ptr)) {
			*ptr = 0;
			ptr = skip_space(ptr + 1);
			if(*ptr) {
				argv[argc++] = ptr;
			}
		} else {
			++ptr;
		}
	}

	argv[argc] = 0;
	return argc;
}

void runcmd(void)
{
	struct rtc_time tm;
	struct rtc_date date;
	char *endp;

	if(!argc) return;

	if(strcasecmp(argv[0], "time") == 0) {
		if(!argv[1]) {
			rtc_get_time(&tm);
			printf("current time: %02d:%02d.%02ds\n", tm.hour, tm.min, tm.sec);
		} else {
			if(argc < 3) {
				goto time_usage;
			} else if(argc < 4) {
				tm.sec = 0;
			} else {
				tm.sec = strtol(argv[3], &endp, 10);
				if(*endp) goto time_usage;
			}

			tm.hour = strtol(argv[1], &endp, 10);
			if(*endp) goto time_usage;
			tm.min = strtol(argv[2], &endp, 10);
			if(*endp) goto time_usage;

			rtc_set_time(tm.hour, tm.min, tm.sec);
		}

	} else if(strcasecmp(argv[0], "date") == 0) {
		if(!argv[1]) {
			rtc_get_date(&date);
			printf("current date: %s %02d/%02d/%d\n", day_of_week(date.dow),
					date.day, date.month, date.year);
		} else {
			if(argc < 4) {
				goto date_usage;
			} else if(argc < 5) {
				date.dow = 0;
			} else {
				date.dow = strtol(argv[4], &endp, 10);
				if(*endp) goto date_usage;
			}

			date.year = strtol(argv[1], &endp, 10);
			if(*endp) goto date_usage;
			date.month = strtol(argv[2], &endp, 10);
			if(*endp) goto date_usage;
			date.day = strtol(argv[3], &endp, 10);
			if(*endp) goto date_usage;

			rtc_set_date(date.year, date.month, date.day, date.dow);
		}
	}

	return;

time_usage:
	printf("usage: time <hour> <min> [sec]\n");
	return;

date_usage:
	printf("usage: date <year> <month> <day> [day of week (0 = monday)]\n");
}

const char *day_of_week(int x)
{
	static const char *daystr[] = { "Monday", "Tuesday", "Wednesday",
		"Thursday", "Friday", "Saturday", "Sunday" };
	return daystr[x - 1];
}
