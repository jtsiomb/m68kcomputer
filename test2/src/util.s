| vi:ft=gas68k:
	.include "defs.inc"
	.text
| printstr expects pointer to zero-terminated string in a0
	.global printstr
printstr:
	move.b (%a0)+, %d0
	tst.b %d0
	beq.s 0f
	move.b %d0, IOADDR_UART
	bra.s printstr
0:	rts

	.global printchar
printchar:
	move.b %d0, IOADDR_UART
	rts
