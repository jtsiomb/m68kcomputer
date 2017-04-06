| vi:ft=gas68k:

| I/O device addresses
	.equ IOADDR_UART, 0x8001
	.equ IOADDR_CMD, 0x8003
	.equ IOADDR_DATA, 0x8005

	.macro DISABLE_INTR
	ori.w #0x0700, %sr
	.endm

	.macro ENABLE_INTR
	andi.w #0xf8ff, %sr
	.endm
