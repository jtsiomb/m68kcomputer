#include <stdio.h>
#include <string.h>
#include "x3d.h"
#include "fixed.h"
#include "sincos.h"
#include "regis.h"

typedef struct pvec3 {
	int32_t x, y, z;
} pvec3;

typedef struct pvec2 {
	int32_t x, y;
} pvec2;


#define MAT_STACK_SIZE	4

struct matrix {
	int32_t m[12];
};

static void proc_vertex(const int32_t *vin, pvec3 *vout);

void draw_poly(int num, const pvec3 *verts, int color);
void draw_point(const pvec3 *v, int color);


static int32_t proj_fov = M_PI_X16;
static int32_t proj_aspect = 65536;
static int32_t inv_proj_aspect = 65536;
static int32_t proj_near = ftox16(0.5);
static int32_t proj_far = 500 << 16;
static int32_t inv_tan_half_xfov, inv_tan_half_yfov;

#define ID_INIT {65536, 0, 0, 0, 0, 65536, 0, 0, 0, 0, 65536, 0}

static struct matrix identity = { ID_INIT };

static short mtop;
static struct matrix mstack[MAT_STACK_SIZE] = { {ID_INIT}, {ID_INIT} };

static const int32_t *vertex_array;
static unsigned short vertex_count;

static uint8_t im_color_index;
static int32_t im_vertex[4 * 3];
static short im_vcount;
static short im_mode;


void x3d_projection(int fov, int32_t aspect, int32_t nearz, int32_t farz)
{
	proj_fov = (M_PI_X16 * fov) / 180;
	proj_aspect = aspect;
	inv_proj_aspect = x16div(65536, proj_aspect);
	proj_near = nearz;
	proj_far = farz;

	inv_tan_half_yfov = (int32_t)(2.41421356 * 65536.0);
	inv_tan_half_xfov = x16mul(inv_tan_half_yfov, aspect);
}

int x3d_push_matrix(void)
{
	short newtop = mtop + 1;
	if(newtop >= MAT_STACK_SIZE) {
		return -1;
	}
	memcpy(mstack + newtop, mstack + mtop, sizeof *mstack);
	mtop = newtop;
	return 0;
}

int x3d_pop_matrix(void)
{
	if(mtop <= 0) {
		return -1;
	}
	--mtop;
	return 0;
}

void x3d_load_matrix(int32_t *m)
{
	memcpy(mstack[mtop].m, m, sizeof *mstack);
}


#define M(i,j)	(((i) << 2) + (j))
void x3d_mult_matrix(int32_t *m)
{
	int i, j;
	struct matrix tmp;

	memcpy(tmp.m, mstack[mtop].m, sizeof tmp);

	for(i=0; i<3; i++) {
		for(j=0; j<4; j++) {
			mstack[mtop].m[M(i, j)] =
				x16mul(m[M(0, j)], tmp.m[M(i, 0)]) +
				x16mul(m[M(1, j)], tmp.m[M(i, 1)]) +
				x16mul(m[M(2, j)], tmp.m[M(i, 2)]);
		}
		mstack[mtop].m[M(i, 3)] += tmp.m[M(i, 3)];
	}
}

void x3d_load_identity(void)
{
	memcpy(mstack[mtop].m, identity.m, sizeof identity);
}

void x3d_translate(int32_t x, int32_t y, int32_t z)
{
	int32_t m[] = ID_INIT;
	m[3] = x;
	m[7] = y;
	m[11] = z;

	x3d_mult_matrix(m);
}

void x3d_rotate(int32_t deg, int32_t x, int32_t y, int32_t z)
{
	int32_t xform[] = ID_INIT;

	int32_t angle = x16mul(M_PI_X16, deg) / 180;
	int32_t sina = sin_x16(angle);
	int32_t cosa = cos_x16(angle);
	int32_t one_minus_cosa = 65536 - cosa;
	int32_t nxsq = x16sq(x);
	int32_t nysq = x16sq(y);
	int32_t nzsq = x16sq(z);

	xform[0] = nxsq + x16mul(65536 - nxsq, cosa);
	xform[4] = x16mul(x16mul(x, y), one_minus_cosa) - x16mul(z, sina);
	xform[8] = x16mul(x16mul(x, z), one_minus_cosa) + x16mul(y, sina);
	xform[1] = x16mul(x16mul(x, y), one_minus_cosa) + x16mul(z, sina);
	xform[5] = nysq + x16mul(65536 - nysq, cosa);
	xform[9] = x16mul(x16mul(y, z), one_minus_cosa) - x16mul(x, sina);
	xform[2] = x16mul(x16mul(x, z), one_minus_cosa) - x16mul(y, sina);
	xform[6] = x16mul(x16mul(y, z), one_minus_cosa) + x16mul(x, sina);
	xform[10] = nzsq + x16mul(65536 - nzsq, cosa);

	x3d_mult_matrix(xform);
}

void x3d_scale(int32_t x, int32_t y, int32_t z)
{
	int32_t m[] = ID_INIT;

	m[0] = x;
	m[5] = y;
	m[10] = z;

	x3d_mult_matrix(m);
}

void x3d_vertex_array(int count, const int32_t *ptr)
{
	vertex_array = ptr;
	vertex_count = count;
}

int x3d_draw(int prim, int vnum)
{
	int i, j, pverts = prim;
	const int32_t *vptr = vertex_array;
	uint16_t color;

	if(!vertex_array) return -1;

	if(vnum > vertex_count) {
		vnum = vertex_count;
	}

	for(i=0; i<vnum; i+=pverts) {
		/* process vertices */
		pvec3 vpos[4];

		for(j=0; j<pverts; j++) {
			proc_vertex(vptr, vpos + j);

			if(vpos[j].z <= proj_near) {
				goto skip_prim;
			}

			vptr += 3;
		}

		color = im_color_index;

		/* project & viewport */
		for(j=0; j<pverts; j++) {
			int32_t x, y;

			x = x16mul(vpos[j].x, inv_tan_half_xfov);
			x = x16div(x, vpos[j].z);
			vpos[j].x = (x16mul(x, inv_proj_aspect) + 65536) * (WIDTH / 2);

			y = x16mul(vpos[j].y, inv_tan_half_yfov);
			y = x16div(y, vpos[j].z);
			vpos[j].y = (65536 - y) * (HEIGHT / 2);
		}

		switch(pverts) {
		case X3D_POINTS:
			draw_point(vpos, color);
			break;

		case X3D_LINES:
			break;

		case X3D_TRIANGLES:
		case X3D_QUADS:
			draw_poly(pverts, vpos, im_color_index);
			break;
		}
skip_prim: ;
	}

	return 0;
}

static void proc_vertex(const int32_t *vin, pvec3 *vout)
{
	int i;
	int32_t tvert[3];
	int32_t *mvmat = mstack[mtop].m;

	/* transform vertex with current matrix */
	for(i=0; i<3; i++) {
		tvert[i] = x16mul(mvmat[0], vin[0]) +
			x16mul(mvmat[1], vin[1]) +
			x16mul(mvmat[2], vin[2]) +
			mvmat[3];
		mvmat += 4;
	}

	vout->x = tvert[0];
	vout->y = tvert[1];
	vout->z = tvert[2];
}

void x3d_begin(int mode)
{
	im_mode = mode;
	im_vcount = 0;
}

void x3d_end(void)
{
}

void x3d_vertex(int32_t x, int32_t y, int32_t z)
{
	int32_t *vptr = im_vertex + im_vcount * 3;
	vptr[0] = x;
	vptr[1] = y;
	vptr[2] = z;

	im_vcount = (im_vcount + 1) % im_mode;
	if(!im_vcount) {
		x3d_vertex_array(im_mode, im_vertex);
		x3d_draw(im_mode, im_mode);
	}
}

void x3d_color_index(int cidx)
{
	im_color_index = cidx;
}


void draw_poly(int num, const pvec3 *verts, int color)
{
	int i;
	regis_abspos(verts[0].x >> 16, verts[0].y >> 16);
	regis_begin_vector(REGIS_BOUNDED);

	for(i=0; i<num-1; i++) {
		++verts;
		regis_absv(verts->x >> 16, verts->y >> 16);
	}
	regis_end_vector();
}

void draw_point(const pvec3 *v, int color)
{
}
