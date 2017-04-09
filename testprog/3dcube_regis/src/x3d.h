#ifndef X3D_H_
#define X3D_H_

#include <inttypes.h>

enum {
	X3D_POINTS = 1,
	X3D_LINES = 2,
	X3D_TRIANGLES = 3,
	X3D_QUADS = 4
};

void x3d_projection(int fov, int32_t aspect, int32_t nearz, int32_t farz);

int x3d_push_matrix(void);
int x3d_pop_matrix(void);
void x3d_load_matrix(int32_t *m);
void x3d_mult_matrix(int32_t *m);
void x3d_load_identity(void);
void x3d_translate(int32_t x, int32_t y, int32_t z);
void x3d_rotate(int32_t angle, int32_t x, int32_t y, int32_t z);
void x3d_scale(int32_t x, int32_t y, int32_t z);

void x3d_vertex_array(int count, const int32_t *ptr);

int x3d_draw(int prim, int vnum);
int x3d_draw_indexed(int prim, int count, uint16_t *ptr);

void x3d_begin(int mode);
void x3d_end(void);

void x3d_vertex(int32_t x, int32_t y, int32_t z);
void x3d_color_index(int cidx);

#endif	/* X3D_H_ */
