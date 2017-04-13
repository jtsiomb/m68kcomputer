#include <stdio.h>
#include <string.h>
#include "intr.h"

static uintptr_t intr_vec_table[64];

void init_intr(void)
{
	int i;

	disable_intr();
	memcpy(intr_vec_table, 0, sizeof intr_vec_table);
	set_intr_vector_base(intr_vec_table);
	set_intr_vector(INTR_UART, intr_uart);
	enable_intr();

	for(i=0; i<16; i++) {
		printf("%d: %x    %d: %x    %d: %x    %d: %x\n",
				i * 4, (unsigned int)intr_vec_table[i * 4],
				i * 4 + 1, (unsigned int)intr_vec_table[i * 4 + 1],
				i * 4 + 2, (unsigned int)intr_vec_table[i * 4 + 2],
				i * 4 + 3, (unsigned int)intr_vec_table[i * 4 + 3]);
	}
}
