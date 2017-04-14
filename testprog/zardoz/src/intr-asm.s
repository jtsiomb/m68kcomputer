| vi:ft=gas68k:
	.text

	.global set_intr_vector_base
set_intr_vector_base:
	move.l 4(%sp), %a0
	movec %a0, %vbr
	rts

	.global get_intr_vector_base
get_intr_vector_base:
	movec %vbr, %d0
	rts

	.global set_intr_level
set_intr_level:
	move.l 4(%sp), %d0
	lsl.w #8, %d0
	move.w %sr, %d1
	andi.w #0xf8ff, %d1
	or.w %d0, %d1
	move.w %sr, %d0
	move.w %d1, %sr
	lsr.w #8, %d0
	ori.w #7, %d0
	rts

	.global get_intr_level
get_intr_level:
	move.w %sr, %d0
	lsr.w #8, %d0
	ori.w #7, %d0
	rts

	.global set_intr_vector
set_intr_vector:
	move.l 4(%sp), %d0
	lsl.w #2, %d0
	movea.l 8(%sp), %a0
	movec %vbr, %a1
	move.l %a0, (%a1,%d0.w)
	rts

| the serial interrupt just saves state and calls uart_handler in uart.c
	.extern uart_handler
	.global intr_uart
intr_uart:
	movem.l %d0-%d1/%a0-%a1, -(%sp)
	jsr uart_handler
	movem.l (%sp)+, %d0-%d1/%a0-%a1
	rte
