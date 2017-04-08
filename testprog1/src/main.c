#include <stdio.h>
#include "util.h"
#include "regis.h"

void koch(int x0, int y0, int x1, int y1, int iter);

int main(void)
{
	regis_enter();
	regis_clear();
	koch(10, 300, 400, 300, 4);
	regis_leave();
	return 0;
}

void koch(int x0, int y0, int x1, int y1, int iter)
{
	int xmid, ymid;
	int dx, dy;
	int x = x0, y = y0, nx, ny;

	if(!iter) {
		regis_abspos(x0, y0);
		regis_begin_vector(REGIS_UNBOUNDED);
		regis_absv(x1, y1);
		regis_end_vector();
		return;
	}

	dx = (x1 - x0) / 3;
	dy = (y1 - y0) / 3;

	xmid = (x0 + x1) / 2;
	ymid = (y0 + y1) / 2;

	--iter;
	nx = x0 + dx;
	ny = y0 + dy;
	koch(x, y, nx, ny, iter);
	x = nx;
	y = ny;
	nx = xmid + dy;
	ny = ymid - dx;
	koch(x, y, nx, ny, iter);
	x = nx;
	y = ny;
	nx = x0 + 2 * dx;
	ny = y0 + 2 * dy;
	koch(x, y, nx, ny, iter);
	koch(nx, ny, x1, y1, iter);
}
