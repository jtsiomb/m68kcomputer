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
	rol.w #4, %d0
	bsr print_hex_digit
	rol.w #4, %d0
	bsr print_hex_digit
	rol.w #4, %d0
	bsr print_hex_digit
	rol.w #4, %d0
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

| parse_addr expects a pointer ot a 6-digit hex string in a0
| returns the longword value in d0 or (longword)-1
| (NOTE: -1 or ffffffff is not a valid 24bit address)
	.global parse_addr
parse_addr:
	move.l %d4, -(%sp)
	moveq.l #0, %d4

	bsr parse_byte
	tst.w %d0
	blt.s .Laret
	move.w %d0, %d4
	swap %d4

	bsr parse_word
	tst.l %d0
	blt.s .Laret

	or.l %d4, %d0
.Laret:	move.l (%sp)+, %d4
	rts

| parse_word expects a pointer to a hex string in a0
| returns the word value in d0 or (longword)-1
	.global parse_word
parse_word:
	move.w %d4, -(%sp)

	moveq.l #0, %d0
	bsr parse_byte
	tst.w %d0
	blt.s .Lwerr
	move.b %d0, %d4
	lsl.w #8, %d4

	bsr parse_byte
	tst.w %d0
	blt.s .Lwerr
	or.w %d4, %d0

	bra.s .Lwret
.Lwerr: ext.l %d0
.Lwret:	move.w (%sp)+, %d4
	rts

| parse_byte expects a pointer to a hex string in a0
| returns the byte value in d0 or (word)-1
	.global parse_byte
parse_byte:
	move.w %d4, -(%sp)

	moveq.l #0, %d0
	move.b (%a0)+, %d0	| read first char for the MSNibble
	bsr parse_hex_digit
	tst.b %d0
	blt.s .Lberr
	move.b %d0, %d4
	lsl.b #4, %d4

	move.b (%a0)+, %d0	| read the next char for the LSNibble
	bsr parse_hex_digit
	tst.b %d0
	blt.s .Lberr
	or.b %d4, %d0

	bra.s .Lbret
.Lberr:	ext.w %d0
.Lbret:	move.w (%sp)+, %d4
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
	move.b #-1, %d0
	rts
