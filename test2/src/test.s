| vi:ft=gas68k:
	.text
	.global start
start:	move.b #0, %d0
top:	move.b %d0, 0x8000
	addi.b #1, %d0

|	move.l #0x80000, %d1
|delay1: subi.l #1, %d1
|	bne.s delay1

	bra.s top
