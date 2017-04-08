#include "util.h"
#include "ioreg.h"

int putchar(int c)
{
	if(c == '\n') {
		IOREG_UART = '\r';
	}
	IOREG_UART = c;
	return c;
}
