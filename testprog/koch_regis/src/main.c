#include <stdio.h>
#include "regis.h"

#define KOCH_ITER	4

void koch(int x0, int y0, int x1, int y1, int iter);

static int verts[][2] = {
	{70, 140}, {420, 140},
	{420, 140}, {240, 440},
	{240, 440}, {70, 140}
};

int main(void)
{
	int i;

	regis_enter();
	regis_clear();
	regis_leave();

	for(i=0; i<3; i++) {
		int vidx0 = i * 2;
		int vidx1 = i * 2 + 1;
		int x0 = verts[vidx0][0];
		int y0 = verts[vidx0][1];
		int x1 = verts[vidx1][0];
		int y1 = verts[vidx1][1];
		koch(x0, y0, x1, y1, KOCH_ITER);
	}

	return 0;
}

void koch(int x0, int y0, int x1, int y1, int iter)
{
	int xmid, ymid;
	int dx, dy;
	int x = x0, y = y0, nx, ny;

	if(!iter) {
		regis_enter();
		regis_abspos(x0, y0);
		regis_begin_vector(REGIS_UNBOUNDED);
		regis_absv(x1, y1);
		regis_end_vector();
		regis_leave();
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
