| vi:ft=gas68k:
	.include "defs.inc"
	.text

	.global main
main:
	movea.l #greet, %a0
	jsr printstr

mainloop:
	DISABLE_INTR
	move.w inbuf_rd, %d1
	cmp.w inbuf_wr, %d1
	bne.s .Lhaveinput
	ENABLE_INTR
	bra.s mainloop
.Lhaveinput:
	movea.l #inbuf, %a0
	move.b (%a0,%d1.w), %d0
	addi.w #1, %d1
	andi.w #INBUF_MASK, %d1
	move.w %d1, inbuf_rd
	ENABLE_INTR
	jsr proc_input
	bra.s mainloop

| proc_input expects input character in d0
proc_input:
	cmpi.b #13, %d0		| if newline, goto runcmd
	bne.s .Lappend
	| zero-terminate, reset idx, and run
	move.w cmdbuf_idx, %d0
	tst.w %d0	| on empty buffer, return
	bne.s 0f
	rts
0:	movea.l #cmdbuf, %a0
	move.b #0, (%a0,%d0.w)
	move.w #0, cmdbuf_idx
	bra.s .Lruncmd
.Lappend:
	move.w cmdbuf_idx, %d1
	cmpi.w #CMDBUF_SZ, %d1	| if overflow, return
	bne.s 0f
	rts
0:	movea.l #cmdbuf, %a0
	move.b %d0, (%a0,%d1.w)	| append character
	addi.w #1, %d1
	move.w %d1, cmdbuf_idx
	rts
.Lruncmd:
	move.w #0, %d0
	move.b cmdbuf, %d0
	cmpi.b #LAST_BRA, %d0
	bhi cmd_invalid
	| calculate offset in the branch table
	subi.b #FIRST_BRA, %d0
	blo cmd_invalid
	lsl.w #2, %d0
	jmp .Lbratab(%pc, %d0.w)

.Lbratab:
	.equ FIRST_BRA, 'a'
	.equ LAST_BRA, 'x'
	bra.w cmd_addr	| 'a' - address
	bra.w cmd_invalid
	bra.w cmd_invalid
	bra.w cmd_invalid
	bra.w cmd_echo	| 'e' - echo
	bra.w cmd_invalid
	bra.w cmd_invalid
	bra.w cmd_help	| 'h' - help
	bra.w cmd_invalid
	bra.w cmd_invalid
	bra.w cmd_invalid
	bra.w cmd_invalid
	bra.w cmd_invalid
	bra.w cmd_invalid
	bra.w cmd_invalid
	bra.w cmd_invalid
	bra.w cmd_invalid
	bra.w cmd_read	| 'r' - read
	bra.w cmd_invalid
	bra.w cmd_invalid
	bra.w cmd_invalid
	bra.w cmd_invalid
	bra.w cmd_write	| 'w' - write
	bra.w cmd_exec	| 'x' - execute
	bra.w cmd_invalid

cmd_addr: | set address command handler
	movea.l #cmdbuf, %a0
	addq.l #1, %a0	| skip the command char (should be 'a')
	jsr parse_addr
	| DBG
	move.l %d0, %d4
	movea.l #str_dbg, %a0
	jsr printstr
	move.l %d4, %d0
	swap %d0
	jsr print_word
	move.b #' ', IOADDR_UART
	move.w %d4, %d0
	jsr print_word
	move.b #13, IOADDR_UART
	move.b #10, IOADDR_UART
	move.l %d4, %d0

	tst.l %d0
	bge.s 0f
	movea.l #str_invval, %a0
	jsr printstr
	rts
0:	move.l %d0, cur_addr
	rts

cmd_echo: | set echo on/off command handler
	movea.l #cmdbuf, %a0
	addq.l #1, %a0	| skip the command char (should be 'e')
	move.w #0, %d0
	cmpi.b #'0', (%a0)
	beq.s 0f
	move.w #1, %d0
0:	move.w %d0, echo 
	rts

cmd_read: | read word command handler
	movea.l #cmdbuf, %a0
	addq.l #1, %a0	| skip the command char (should be 'r')
	movea.l cur_addr, %a1
	move.w (%a1)+, %d0
	move.l %a1, cur_addr
	jsr print_word
	move.b #13, IOADDR_UART
	move.b #10, IOADDR_UART
	rts

cmd_write: | write word command handler
	movea.l #cmdbuf, %a0
	addq.l #1, %a0	| skip the command char (should be 'w')
	jsr parse_word
	cmp.l #0, %d0
	bge.s 0f
	movea.l #str_invval, %a0
	jsr printstr
	rts
0:	movea.l cur_addr, %a1
	move.l %d0, (%a1)+
	move.l %a1, cur_addr
	rts

cmd_exec: | execute code from current address command handler
	movea.l cur_addr, %a0
	jsr (%a0)
	rts

cmd_help: | help command handler
	movea.l #str_help, %a0
	jsr printstr
	rts

cmd_invalid:
	movea.l #str_unkcmd, %a0
	jsr printstr
	move.b cmdbuf, IOADDR_UART
	move.b #13, IOADDR_UART
	move.b #10, IOADDR_UART
	rts

| UART interrupt handler echoes the character and appends it to the
| input buffer for further processing in the main loop
	.global intr_uart
intr_uart:
	movem.l %d0-%d2/%a0, -(%sp)
	move.b IOADDR_UART, %d0
	cmpi.w #0, echo
	beq.s .Lskipecho
	move.b %d0, IOADDR_UART | echo
	cmpi.w #13, %d0
	bne.s .Lskipecho
	move.b #10, IOADDR_UART
.Lskipecho:
	movea.l #inbuf, %a0
	move.w inbuf_wr, %d1	| index -> d1
	move.b %d0, (%a0,%d1.w)	| *(inbuf + index) = d0
	| advance write index
	addq.w #1, %d1		
	andi.w #INBUF_MASK, %d1
	move.w %d1, inbuf_wr
	| if write index caught up to the read index, advance the read
	| index (dropping the oldest byte from the fifo)
	cmp.w inbuf_rd, %d1
	bne.s 0f
	addq.w #1, inbuf_rd
0:	movem.l (%sp)+, %d0-%d2/%a0
	rte


	.section .rodata,"a"
greet:	.asciz "68k monitor by John Tsiombikas <nuclear@member.fsf.org>\r\n"
str_unkcmd: .asciz "Unknown command: "
str_help:
	.ascii "Commands:\r\n"
	.ascii "  a<addr> - set address for read/write operations\r\n"
	.ascii "  r - read word and increment address\r\n"
	.ascii "  w<data> - write word and increment address\r\n"
	.ascii "  e<0|1> - echo on/off\r\n"
	.ascii "  h - print command help\r\n"
	.byte 0
str_invval: .asciz "Invalid hex value\r\n"
str_dbg: .asciz "DBG: "

	.data
	.align 2
echo:	.word 1

	.bss
| input buffer
inbuf:	.space 32
	.equ INBUF_MASK, 0x1f
	.align 2
inbuf_wr: .word 0
inbuf_rd: .word 0
| command buffer used in proc_input
	.equ CMDBUF_SZ, 64
cmdbuf: .space CMDBUF_SZ
	.align 2
cmdbuf_idx: .word 0
cur_addr: .long 0
