/**
 ******************************************************************************
 *
 * @file        MG82F6D64_CONFIG.h
 *
 * @brief       This is the C code format driver head file.
 *
 * @par         Project
 *              MA82F6D64
 * @version     V1.01
 * @date        2021/10/21
 * @author      Megawin Software Center
 * @copyright   Copyright (c) 2019 MegaWin Technology Co., Ltd.
 *              All rights reserved.
 *
 ******************************************************************************
 * @par         Disclaimer
 *      The Demo software is provided "AS IS" without any warranty, either
 *      expressed or implied, including, but not limited to, the implied warranties
 *      of merchantability and fitness for a particular purpose.  The author will
 *      not be liable for any special, incidental, consequential or indirect
 *      damages due to loss of data or any other reason.
 *      These statements agree with the worldwide and local dictated laws about
 *      authorship and violence against these laws.
 ******************************************************************************
 ******************************************************************************
 */
 
#ifndef _MG82F6D64_CONFIG_H
#define _MG82F6D64_CONFIG_H


#include <intrins.h>
#include "REG_MG82F6D64.h"
#include "TYPEDEF.h"
#include "stdio.h"

/**
 ******************************************************************************
 * do-while template
 ******************************************************************************
 */
#define MWT( __stuff__ )  do { __stuff__ } while (0)


/**
 ******************************************************************************
 * Headers
 ******************************************************************************
 */

//COMMON
#include "MG82F6D64_COMMON_DRV.h"
//#include "MG82F6D64_COMMON_MID.h"
//#include "MG82F6D64_WIZARD.h"

//Ch6. EMB
#include "MG82F6D64_EMB_DRV.h"

//Ch8. DMA
#include "MG82F6D64_DMA_DRV.h"
//#include "MG82F6D64_DMA_MID.h"

//Ch9. System Clock
#include "MG82F6D64_CLK_DRV.h"
#include "MG82F6D64_CLK_MID.h"

//Ch10. Watch Dog Timer (WDT)
#include "MG82F6D64_WDT_DRV.h"
//#include "MG82F6D64_WDT_MID.h"

//Ch11. Real-Time-Clock (RTC)/System-Timer
#include "MG82F6D64_RTC_DRV.h"
//#include "MG82F6D64_RTC_MID.h"

//Ch12. System Reset
#include "MG82F6D64_RST_DRV.h"
//#include "MG82F6D64_xxxxx_MID.h"

//Ch13. Power Management
#include "MG82F6D64_PW_DRV.h"
//#include "MG82F6D64_PW_MID.h"

//Ch14. Configurable I/O Ports (GPIO)
#include "MG82F6D64_GPIO_DRV.h"
//#include "MG82F6D64_GPIO_MID.h"

//Ch15. Interrupt
#include "MG82F6D64_INT_DRV.h"
//#include "MG82F6D64_INT_MID.h"
#include "MG82F6D64_INT_VECTOR.h"

//Ch16. Timers/Counters
#include "MG82F6D64_TIMER_DRV.h"
//#include "MG82F6D64_TIMER_MID.h"

//Ch17. Programmable Counter Array (PCA0)
#include "MG82F6D64_PCA_DRV.h"
//#include "MG82F6D64_PCA_MID.h"

//Ch18. SerialPort0 (UART0)
#include "MG82F6D64_UART0_DRV.h"
#include "MG82F6D64_UART0_MID.h"

//Ch19. SerialPort1 (UART1)
#include "MG82F6D64_UART1_DRV.h"
#include "MG82F6D64_UART1_MID.h"

//Ch20. SerialPort2 (UART2)
#include "MG82F6D64_UART2_DRV.h"
#include "MG82F6D64_UART2_MID.h"

//Ch21. SerialPort3 (UART3)
#include "MG82F6D64_UART3_DRV.h"
#include "MG82F6D64_UART3_MID.h"

//Ch22. Serial Peripheral Interface (SPI)
#include "MG82F6D64_SPI_DRV.h"
#include "MG82F6D64_SPI_MID.h"

//Ch23. Two Wire Serial Interface (TWI0/I2C0 & TWI1/I2C1)
#include "MG82F6D64_I2C0_DRV.h"
//#include "MG82F6D64_I2C0_MID.h"
#include "MG82F6D64_I2C1_DRV.h"
//#include "MG82F6D64_I2C1_MID.h"

//Ch24. Serial Interface Detection (STWI/SI2C)
#include "MG82F6D64_SI2C_DRV.h"
//#include "MG82F6D64_SI2C_MID.h"

//Ch25. Beeper
#include "MG82F6D64_BEEPER_DRV.h"
//#include "MG82F6D64_xxxxx_MID.h"

//Ch26. Keypad Interrupt (KBI)
#include "MG82F6D64_KBI_DRV.h"
//#include "MG82F6D64_KBI_MID.h"

//Ch27. General Purpose Logic (GPL-CRC)
//#include "MG82F6D64_GPL_DRV.h"
//#include "MG82F6D64_GPL_MID.h"

//Ch28. Output Signal Break & Modulator 0/1 (OBM0/1)
//#include "MG82F6D64_OBM0_DRV.h"
//#include "MG82F6D64_OBM0_MID.h"
//#include "MG82F6D64_OBM1_DRV.h"
//#include "MG82F6D64_OBM1_MID.h"

//Ch29. 12-Bit ADC
#include "MG82F6D64_ADC_DRV.h"
//#include "MG82F6D64_ADC_MID.h"

//Ch30. Analog Comparator 0/1/2 (AC0/AC1/AC2)
#include "MG82F6D64_AC0_DRV.h"
//#include "MG82F6D64_AC0_MID.h"
#include "MG82F6D64_AC1_DRV.h"
//#include "MG82F6D64_AC1_MID.h"
#include "MG82F6D64_AC2_DRV.h"
//#include "MG82F6D64_AC2_MID.h"

//Ch31. Internal Voltage Reference (IVR, 1.4V)
//#include "MG82F6D64_IVR_DRV.h"
//#include "MG82F6D64_IVR_MID.h"

//Ch32. ISP and IAP
#include "MG82F6D64_IAP_DRV.h"
//#include "MG82F6D64_IAP_MID.h"

#endif


