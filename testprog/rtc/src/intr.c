#include <stdio.h>
#include <string.h>
#include "intr.h"

static uintptr_t intr_vec_table[64] __attribute__((aligned(1024)));

void init_intr(void)
{
	disable_intr();
	memcpy(intr_vec_table, 0, sizeof intr_vec_table);
	set_intr_vector_base(intr_vec_table);
	set_intr_vector(INTR_UART, intr_uart);
	enable_intr();
}

void cleanup_intr(void)
{
	disable_intr();
	set_intr_vector_base(0);
	enable_intr();
}
