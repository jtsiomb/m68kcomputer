#include "util.h"
#include "ioreg.h"

void printstr(const char *str)
{
	while(*str) {
		char c = *str++;
		if(c == '\n') {
			IOREG_UART = '\r';
		}
		IOREG_UART = c;
	}
}
