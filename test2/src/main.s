| vi:ft=gas68k:
	.include "defs.inc"
	.text

	.global main
main:
	movea.l #greet, %a0
	jsr printstr

mainloop:
	move.b inbuf_rd, %d1
	cmp.b inbuf_wr, %d1
	beq.s .Linbuf_empty	| skip if the input queue is empty
	lea inbuf, %a0
	DISABLE_INTR
	move.b inbuf_rd, %d1	| re-read again in the critical section
	move.b (%a0,%d1), %d0
	addi.b #1, %d1
	andi.b #INBUF_MASK, %d1
	move.b %d1, inbuf_rd
	ENABLE_INTR
	jsr proc_input
.Linbuf_empty:
	bra.s mainloop

| proc_input expects input character in d0
proc_input:
	cmpi.b #10, %d0		| if newline, goto runcmd
	beq.s .Lruncmd
	move.b cmdbuf_idx, %d1
	cmpi.b #CMDBUF_SZ, %d1	| if overflow, return
	bne.s 0f
	rts
0:	lea cmdbuf, %a0
	move.b %d0, (%a0,%d1)	| append character
	addi.b #1, %d1
	move.b %d1, cmdbuf_idx
	rts

.Lruncmd:
	move.b cmdbuf, %d0
	cmpi.b #LAST_BRA, %d0
	bhi.s .Linvalid
	| calculate offset in the branch table
	subi.b #FIRST_BRA, %d0
	bcs.s .Linvalid
	lsl.b #1, %d0
	jmp .Lbratab(%pc, %d0.w)
	
.Lbratab:
	.equ FIRST_BRA, 97	| 'a'
	.equ LAST_BRA, 120	| 'x'
	bra.s .Laddr	| 'a' - address
	bra.s .Linvalid
	bra.s .Linvalid
	bra.s .Linvalid
	bra.s .Lecho	| 'e' - echo
	bra.s .Linvalid
	bra.s .Linvalid
	bra.s .Lhelp	| 'h' - help
	bra.s .Linvalid
	bra.s .Linvalid
	bra.s .Linvalid
	bra.s .Linvalid
	bra.s .Linvalid
	bra.s .Linvalid
	bra.s .Linvalid
	bra.s .Linvalid
	bra.s .Linvalid
	bra.s .Lread	| 'r' - read
	bra.s .Linvalid
	bra.s .Linvalid
	bra.s .Linvalid
	bra.s .Linvalid
	bra.s .Lwrite	| 'w' - write
	bra.s .Lexec	| 'x' - execute
	nop

.Laddr:
	lea cmdbuf + 1, %a0
.Lecho:
.Lread:
.Lwrite:
.Lexec:
.Lhelp:
	movea.l #str_help, %a0
	jsr printstr
	rts
.Linvalid:
	movea.l #str_unkcmd, %a0
	jsr printstr
	move.l %d0, IOADDR_UART
	move.l #13, IOADDR_UART
	move.l #10, IOADDR_UART
	rts
	

| UART interrupt handler echoes the character and appends it to the
| input buffer for further processing in the main loop
	.global intr_uart
intr_uart:
	movem.l %d0-%d2/%a0, -(%sp)
	move.b IOADDR_UART, %d0
	move.b %d0, IOADDR_UART | echo
	lea inbuf, %a0
	move.b inbuf_wr, %d1	| index -> d1
	move.b %d0, (%a0,%d1)	| *(inbuf + index) = d0
	| advance write index
	addi.b #1, %d1		
	andi.b #INBUF_MASK, %d1
	move.b %d1, inbuf_wr
	| if write index caught up to the read index, advance the read
	| index (dropping the oldest byte from the fifo)
	cmp.b inbuf_rd, %d1
	bne.s 0f
	addi.b #1, inbuf_rd
0:	movem.l (%sp)+, %d0-%d2/%a0
	rte

	.data
greet:	.asciz "68k monitor by John Tsiombikas <nuclear@member.fsf.org>\r\n"
str_unkcmd: .asciz "Unknown command: "
str_help:
	.asciz "Commands:\r\n"
	.asciz "a<addr> - set address for read/write operations\r\n"
	.asciz "r - read word and increment address\r\n"
	.asciz "w<data> - write word and increment address\r\n"
	.asciz "e<0|1> - echo on/off\r\n"
	.asciz "h - print command help\r\n"

	.bss
| input buffer
inbuf:	.space 32
	.equ INBUF_MASK, 0x1f
inbuf_wr: .byte
inbuf_rd: .byte
| command buffer used in proc_input
	.equ CMDBUF_SZ, 64
cmdbuf: .space CMDBUF_SZ
cmdbuf_idx: .byte
