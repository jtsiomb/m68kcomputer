#ifndef REGIS_H_
#define REGIS_H_

#include <stdio.h>

enum {
	REGIS_BOUNDED = 1,
	REGIS_UNBOUNDED = 0,
	REGIS_FILL = 2,
	REGIS_WIRE = 0
};

void regis_enter(void);
void regis_leave(void);
void regis_draw_page(int x);
void regis_show_page(int x);
void regis_clear(void);
void regis_abspos(int x, int y);
void regis_relpos(int x, int y);
void regis_absv(int x, int y);
void regis_relv(int x, int y);
void regis_nullv(void);

void regis_begin_vector(unsigned int mode);
void regis_end_vector(void);

#endif	/* REGIS_H_ */
