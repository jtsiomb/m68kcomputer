#ifndef IOREG_H_
#define IOREG_H_

#define IOREG_UART *((volatile unsigned char*)0x8001)
#define IOREG_CMD *((volatile unsigned char*)0x8003)
#define IOREG_DATA *((volatile unsigned char*)0x8005)

enum {
	CMD_TIME	= 1,
	CMD_DATE	= 2,
};

#endif	/* IOREG_H_ */
