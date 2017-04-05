| vi:ft=gas68k:
	.include "defs.inc"
	.text
	.global start
start:
	| copy .data section from ROM to RAM
	move.l #_data_lma, %a0
	move.l #_data_start, %a1
	move.l #_data_end, %a2
	cmp.l %a1, %a2
	beq.s 1f	| skip data copy if the section is empty
0:	move.l (%a0)+, (%a1)+
	cmp.l %a1, %a2
	bne.s 0b
1:
	| zero the .bss section
	move.l #_bss_start, %a0
	move.l #_bss_end, %a1
	cmp.l %a0, %a1
	beq.s 1f	| skip bss clearing if the section is empty
0:	clr.l (%a0)+
	cmp.l %a0, %a1
	bne.s 0b
1:
	jsr enable_intr
	jsr main
endloop:
	stop #0x2700
	bra.s endloop
