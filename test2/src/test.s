| vi:ft=gas68k:
	.text
	.global start
start:
	jsr enable_intr
	move.l #msg, %a0
	jsr printstr
mainloop:
	bra.s mainloop
	
| printstr expects pointer to zero-terminated string in a0
printstr:
	move.b (%a0)+, %d0
	tst.b %d0
	beq.s 0f
	move.b %d0, 0x8001
	bra.s printstr
0:	rts

	.global intr_uart
intr_uart:
	| just echo back
	move.l %d0, -(%sp)
	move.b 0x8001, %d0
	move.b %d0, 0x8001
	move.l (%sp)+, %d0
	rte	

msg:	.asciz "Hello world from the 68k\n"
