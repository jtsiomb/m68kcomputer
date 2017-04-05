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

| print_word expects a word in d0
	.global print_word
print_word:
	bsr print_hex_digit
	lsr.w #4, %d0
	bsr print_hex_digit
	lsr.w #4, %d0
	bsr print_hex_digit
	lsr.w #4, %d0
	bsr print_hex_digit
	rts

| print_hex_digit expects a nibble in d0
	.global print_hex_digit
print_hex_digit:
	move.b %d0, %d1
	andi.b #0xf, %d1
	cmpi.b #9, %d1
	bhi.s .Lpdig_over9
	| it's between 0 and 9
	addi.b #'0', %d1
	move.b %d1, IOADDR_UART
	rts
.Lpdig_over9:
	| it's between a and f
	addi.b #87, %d1
	move.b %d1, IOADDR_UART
	rts


| parse_word expects a pointer to a hex string in a0
| returns the hex word value in d0
	.global parse_word
parse_word:
	move.l %d4, -(%sp)
	moveq.l #0, %d4

	move.b (%a0)+, %d0
	bsr parse_hex_digit
	cmpi.w #0, %d0
	blt.s .Lret
	move.b %d0, %d4
	lsl.w #4, %d4

	move.b (%a0)+, %d0
	bsr parse_hex_digit
	cmpi.w #0, %d0
	blt.s .Lret
	or.b %d0, %d4
	lsl.w #4, %d4

	move.b (%a0)+, %d0
	bsr parse_hex_digit
	cmpi.w #0, %d0
	blt.s .Lret
	or.b %d0, %d4
	lsl.w #4, %d4

	move.b (%a0)+, %d0
	bsr parse_hex_digit
	cmpi.l #0, %d0
	blt.s .Lret
	or.b %d0, %d4

	move.l %d4, %d0
.Lret:	move.l (%sp)+, %d4
	rts

| parse_hex_digit expects an ascii char in d0
	.global parse_hex_digit
parse_hex_digit:
	cmpi.b #'0', %d0
	blo.s .Linvhex
	cmpi.b #'9', %d0
	bhi.s .Lover9
	| it's between 0 and 9
	subi.b #'0', %d0
	rts
.Lover9:
	cmpi.b #'a', %d0
	blo.s .Linvhex
	cmpi.b #'f', %d0
	bhi.s .Linvhex
	| it's between a and f
	subi.b #87, %d0
	rts
.Linvhex:
	move.w #-1, %d0
	rts
