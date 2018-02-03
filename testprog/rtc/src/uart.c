#include <stdio.h>
#include <ctype.h>
#include "uart.h"
#include "ioreg.h"

#define INBUF_SIZE	32
#define INBUF_MASK	(INBUF_SIZE - 1)
#define NEXTIDX(x)	(((x) + 1) & INBUF_MASK)
static volatile char inbuf[INBUF_SIZE];
static volatile short inbuf_rd, inbuf_wr;

int putchar(int c)
{
	if(c == '\n') {
		IOREG_UART = '\r';
	}
	IOREG_UART = c;
	return c;
}

int getchar(void)
{
	int c;

	while(inbuf_wr == inbuf_rd);

	c = inbuf[inbuf_rd];
	inbuf_rd = NEXTIDX(inbuf_rd);
	return c;
}

int have_input(void)
{
	return inbuf_wr != inbuf_rd;
}

/* this function is called from the uart interrupt vector */
void uart_handler(void)
{
	static char prev_c;
	char c = IOREG_UART;

	if(c == '\n' && prev_c == '\r') {
		prev_c = c;
		return;
	}
	prev_c = c;

	if(c == '\r') {
		IOREG_UART = c;
		c = '\n';
	}
	IOREG_UART = c;		/* echo */

	inbuf[inbuf_wr] = c;
	inbuf_wr = NEXTIDX(inbuf_wr);

	if(inbuf_rd == inbuf_wr) {
		/* if the write end caught up with the read end, discard the
		 * least-recent input char.
		 */
		inbuf_rd = NEXTIDX(inbuf_rd);
	}
}
