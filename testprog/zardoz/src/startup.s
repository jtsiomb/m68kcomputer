| vi:ft=gas68k:
	.section .text.startup
	.global start
start:	| zero the .bss section
	move.l #_bss_start, %a0
	move.l #_bss_end, %a1
	cmp.l %a0, %a1
	beq.s 1f	| skip bss clearing if the section is empty
0:	clr.l (%a0)+
	cmp.l %a0, %a1
	bne.s 0b
1:
	jsr main
	rts
