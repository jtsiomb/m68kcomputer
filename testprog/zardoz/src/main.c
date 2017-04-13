#include <stdio.h>
#include "intr.h"

int main(void)
{
	printf("initializing zardoz v" VERSION "\n");
	init_intr();
	printf("vbr: %x\n", (unsigned int)get_intr_vector_base());
	set_intr_vector_base(0);

	return 0;
}
