#include <stdio.h>
#include "regis.h"
#include "sincos.h"
#include "x3d.h"

static void draw(void);

int tm = 0;

int main(int argc, char **argv)
{
	int pg = 1;

	x3d_projection(45, (WIDTH << 16) / HEIGHT, 65536 / 2, 65536 * 500);

	for(;;) {
		regis_enter();
		regis_draw_page(pg);
		regis_clear();

		draw();

		regis_show_page(pg);
		pg = (pg + 1) & 1;
		regis_leave();

		++tm;
	}

	regis_enter();
	regis_clear();
	regis_leave();
	return 0;
}

static void draw(void)
{
	x3d_load_identity();
	x3d_translate(0, 0, 8 << 16);
	x3d_rotate(25 << 16, 65536, 0, 0);
	x3d_rotate(tm << 13, 0, 65536, 0);

	x3d_color_index(3);

	x3d_begin(X3D_QUADS);
	x3d_vertex(-65536, -65536, -65536);
	x3d_vertex(65536, -65536, -65536);
	x3d_vertex(65536, 65536, -65536);
	x3d_vertex(-65536, 65536, -65536);

	x3d_vertex(65536, -65536, -65536);
	x3d_vertex(65536, -65536, 65536);
	x3d_vertex(65536, 65536, 65536);
	x3d_vertex(65536, 65536, -65536);

	x3d_vertex(65536, -65536, 65536);
	x3d_vertex(-65536, -65536, 65536);
	x3d_vertex(-65536, 65536, 65536);
	x3d_vertex(65536, 65536, 65536);

	x3d_vertex(-65536, -65536, 65536);
	x3d_vertex(-65536, -65536, -65536);
	x3d_vertex(-65536, 65536, -65536);
	x3d_vertex(-65536, 65536, 65536);
	x3d_end();
}
