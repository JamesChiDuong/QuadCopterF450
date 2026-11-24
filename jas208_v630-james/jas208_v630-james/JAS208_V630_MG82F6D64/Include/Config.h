#ifndef _CONFIG_H
#define _CONFIG_H

//============================================================================//
// Define
//============================================================================//
#define ENABLE_UART1 0
#define ENABLE_UART2 1
#define ENABLE_UART3 0
#define ENABLE_INT0 1
#define ENABLE_INT1 1
#define ENABLE_INT2 1
#define ENABLE_INT3 0
#define ENABLE_TWI1 0
#define ENABLE_ADC0 1
#define ENABLE_DELAYUS 1
#define ENABLE_DELAYMS 1
#define EN_110592 0
#define EN_221184 0
#define EN_294912 1
#define EN_442368 0
#define ENABLE_RTC 1
#define EN_RTC_SRC_32000 1
#define EN_RTC_WKUP 1

/*************************************************
Set SysClk (MAX.50MHz) (MAX.50MHz)
Selection:
	11059200,12000000,
	22118400,24000000,
	29491200,32000000,
	44236800,48000000
*************************************************/
#if EN_110592 == 1
#define MCU_SYSCLK 11059200
#elif EN_221184 == 1
#define MCU_SYSCLK 22118400
#elif EN_294912 == 1
#define MCU_SYSCLK 29491200
#elif EN_442368 == 1
#define MCU_SYSCLK 44236800
#else
#define MCU_SYSCLK 12000000
#endif
/*************************************************
Set  CpuClk (MAX.36MHz)
	1) CpuCLK=SysCLK
	2) CpuClk=SysClk/2
*************************************************/
#if EN_442368 == 1
#define MCU_CPUCLK (MCU_SYSCLK / 2)
#else
#define MCU_CPUCLK (MCU_SYSCLK)
#endif
#define FREQ_CPU_M 12

#define OUTPUT_PORT P2
#define INPUT_PORT P0

// MCU WKUP
#define MCU_WKUP_PIN 0
// nRESET_MCU
#define N_RESET_MCU_PIN 1

#define UART_NUM_MAX 3

//============================================================================//
// Typedef
//============================================================================//
typedef enum
{
	WKUP_SRC_NONE = 0U, // 0
	WKUP_SRC_INT0,		// 1
	WKUP_SRC_INT1,		// 2
	WKUP_SRC_INT2,		// 3
	WKUP_SRC_INT3,		// 4
	WKUP_SRC_RTC,		// 5
	WKUP_SRC_ALM,		// 6
	WKUP_SRC_TIMER,		// 7
	WKUP_SRC_HW_RST		// 8

} WkupSourceTypeDef;

#endif // _CONFIG_H
