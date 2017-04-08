#include "regis.h"

void regis_enter(void)
{
	printf("\033P0p\n");
}

void regis_leave(void)
{
	printf("\033\\");
	/*fflush(stdout);*/
}

void regis_draw_page(int x)
{
	printf("P(P%d)\n", x ? 1 : 0);
}

void regis_show_page(int x)
{
	printf("S(P%d)\n", x ? 1 : 0);
}

void regis_clear(void)
{
	printf("S(E)\n");
}

void regis_abspos(int x, int y)
{
	putchar('P');
	regis_absv(x, y);
}

void regis_relpos(int x, int y)
{
	putchar('P');
	regis_relv(x, y);
}

void regis_absv(int x, int y)
{
	printf("[%d,%d]\n", x, y);
}

void regis_relv(int x, int y)
{
	printf("[%+d,%+d]\n", x, y);
}

void regis_nullv(void)
{
	printf("[]\n");
}

static unsigned int cur_mode;

void regis_begin_vector(unsigned int mode)
{
	if(mode & REGIS_FILL) {
		printf("F(");
	}
	putchar('V');
	if(mode & REGIS_BOUNDED) {
		printf("(B)");
	}
	putchar('\n');
	cur_mode = mode;
}

void regis_end_vector(void)
{
	printf("(E)");
	if(cur_mode & REGIS_FILL) {
		putchar(')');
	}
	putchar('\n');
}
