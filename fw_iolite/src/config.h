#ifndef CONFIG_H_
#define CONFIG_H_

#define F_CPU	XTAL

/* pin assignments:
 * B[0,1]	reg address (see enum in main.c)
 * B2		~IRQ2 (uart)
 * B3		~IRQ7 (timer)
 * B4		RTC data
 * B5		RTC clock
 * C[0,5]	D0 - D5
 * D2		~ENABLE (input)
 * D3		RD/~WR (input)
 * D4		DTACK (active high)
 * D5		RTC enable
 * D6		D6
 * D7		D7
 */

#define PB_REG_ADDR_MASK	0x03
#define PB_IRQ_UART_BIT		0x04
#define PB_IRQ_TIMER_BIT	0x08
#define PB_RTC_DATA_BIT		0x10
#define PB_RTC_CLK_BIT		0x20
#define PC_DATA_MASK		0x3f
#define PD_DATA_MASK		0xc0
#define PD_ENABLE_BIT		0x04
#define PD_RDWR_BIT			0x08
#define PD_DTACK_BIT		0x10
#define PD_RTC_CE_BIT		0x20


#endif	/* CONFIG_H_ */
