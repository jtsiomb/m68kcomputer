#ifndef INTR_H_
#define INTR_H_

#include <stdint.h>

enum {
	INTR_AUTOVEC1 = 25,
	INTR_AUTOVEC2,
	INTR_AUTOVEC3,
	INTR_AUTOVEC4,
	INTR_AUTOVEC5,
	INTR_AUTOVEC6,
	INTR_AUTOVEC7
};

#define INTR_UART	INTR_AUTOVEC2
/* TODO: change the timer interrupt to a maskable one */
#define INTR_TIMER	INTR_AUTOVEC7

#define enable_intr()	set_intr_level(0)
#define disable_intr()	set_intr_level(7)

void init_intr(void);
/* defined in intr-asm.s */
void set_intr_vector_base(void *vptr);
void *get_intr_vector_base(void);
int set_intr_level(short lvl);
int get_intr_level(void);
void set_intr_vector(short inum, uint32_t addr);

uintptr_t intr_uart;

#endif	/* INTR_H_ */
