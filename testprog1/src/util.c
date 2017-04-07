#include "util.h"
#include "ioreg.h"

void printstr(const char *str)
{
	while(*str) {
		IOREG_UART = *str++;
	}
}
