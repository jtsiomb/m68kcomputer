| vi:ft=gas68k:
| the following will go into the .vect section which will be placed at the very
| begining of the binary at address 0 by the linker (see lnkscript).
	.section .vect,"a"
	.extern start
| exception vectors
	.long _stacktop		| 00 reset - initial SSP
	.long start		| 01 reset - initial PC
	.long intr_fatal	| 02 bus error
	.long intr_fatal	| 03 address error
	.long intr_fatal	| 04 illegal instruction
	.long intr_fatal	| 05 zero divide
	.long intr_fatal	| 06 chk instruction
	.long intr_fatal	| 07 trapv instruction
	.long intr_fatal	| 08 privilege violation
	.long intr_fatal	| 09 trace
	.long intr_fatal	| 0a line 1010 emulator
	.long intr_fatal	| 0b line 1111 emulator
	.long intr_fatal	| 0c reserved
	.long intr_fatal	| 0d reserved
	.long intr_fatal	| 0e format error (mc68010 only)
	.long intr_fatal	| 0f uninitialized interrupt vector
	.long intr_fatal	| 10 \
	.long intr_fatal	| 11 |
	.long intr_fatal	| 12 |
	.long intr_fatal	| 13  > reserved
	.long intr_fatal	| 14 |
	.long intr_fatal	| 15 |
	.long intr_fatal	| 16 |
	.long intr_fatal	| 17 /
	.long intr_fatal	| 18 spurious interrupt 
	.long intr_slot1	| 19 level 1 interrupt (slot 1 expansion)
	.long intr_uart		| 1a level 2 interrupt (UART)
	.long intr_fatal	| 1b level 3 interrupt
	.long intr_slot2	| 1c level 4 interrupt (slot 2 expansion)
	.long intr_fatal	| 1d level 5 interrupt
	.long intr_fatal	| 1e level 6 interrupt
	.long intr_timer	| 1f level 7 interrupt (timer)
	.long intr_fatal	| 20 trap 0
	.long intr_fatal	| 21 trap 1
	.long intr_fatal	| 22 trap 2
	.long intr_fatal	| 23 trap 3
	.long intr_fatal	| 24 trap 4
	.long intr_fatal	| 25 trap 5
	.long intr_fatal	| 26 trap 6
	.long intr_fatal	| 27 trap 7
	.long intr_fatal	| 28 trap 8
	.long intr_fatal	| 29 trap 9
	.long intr_fatal	| 2a trap a
	.long intr_fatal	| 2b trap b
	.long intr_fatal	| 2c trap c
	.long intr_fatal	| 2d trap d
	.long intr_fatal	| 2e trap e
	.long intr_fatal	| 2f trap f
	.long intr_fatal	| 30 \
	.long intr_fatal	| 31 |
	.long intr_fatal	| 32 |
	.long intr_fatal	| 33 |
	.long intr_fatal	| 34 |
	.long intr_fatal	| 35 |
	.long intr_fatal	| 36 |
	.long intr_fatal	| 37 |
	.long intr_fatal	| 38  > reserved
	.long intr_fatal	| 39 |
	.long intr_fatal	| 3a |
	.long intr_fatal	| 3b |
	.long intr_fatal	| 3c |
	.long intr_fatal	| 3d |
	.long intr_fatal	| 3e |
	.long intr_fatal	| 3f /

| from here on we continue in the regular .text section since we don't care
| where this code ends up.
	.text

.global enable_intr
enable_intr:
	andi.w #0xf8ff, %sr
	rts

.global disable_intr
disable_intr:
	ori.w #0x0300, %sr
	rts

| interrupt handlers
intr_fatal:
intr_slot1:
intr_slot2:
intr_timer:
	move.l #fatal_msg, %a0
0:	move.b (%a0)+, %d0
	tst.b %d0
	beq.s 0f
	move.b %d0, 0x8000	| 0x8000 is the address of the UART
	bra.s 0b
0:	stop #0x2700
	| sanity check
	move.l #dot, %a0
	move.b (%a0), 0x8000
	bra.s 0b

fatal_msg: .asciz "Unexpected interrupt, system halted!\n"
dot: .ascii "."
