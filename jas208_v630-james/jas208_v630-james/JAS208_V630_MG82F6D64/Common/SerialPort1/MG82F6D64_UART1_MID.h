/**
 ******************************************************************************
 *
 * @file        MG82F6D64_UART1_MID.h
 *
 * @brief       This is the C code format driver file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     V1.02
 * @date        2022/06/09
 * @author      Megawin Software Center
 * @copyright   Copyright (c) 2020 MegaWin Technology Co., Ltd.
 *              All rights reserved.
 *
 ******************************************************************************
 * @par 		Disclaimer
 *		The Demo software is provided "AS IS"  without any warranty, either
 *		expressed or implied, including, but not limited to, the implied warranties
 *		of merchantability and fitness for a particular purpose.  The author will
 *		not be liable for any special, incidental, consequential or indirect
 *		damages due to loss of data or any other reason.
 *		These statements agree with the world wide and local dictated laws about
 *		authorship and violence against these laws.
 ******************************************************************************
 ******************************************************************************
 */

#ifndef MG82F6D64_UART1_MID_H
#define MG82F6D64_UART1_MID_H




///@cond __DRV_URT1_Easy_Wizard_Init
#define UART1_8BIT_CONFIG0 (UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_4800|UART1_PIN_CONFIG_RX_P34_TX_P35)
#define UART1_8BIT_CONFIG1 (UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_4800|UART1_PIN_CONFIG_RX_P34_TX_P35)
#define UART1_8BIT_CONFIG2 (UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_9600|UART1_PIN_CONFIG_RX_P34_TX_P35)
#define UART1_8BIT_CONFIG3 (UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_9600|UART1_PIN_CONFIG_RX_P34_TX_P35)
#define UART1_8BIT_CONFIG4 (UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_19200|UART1_PIN_CONFIG_RX_P34_TX_P35)
#define UART1_8BIT_CONFIG5 (UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_19200|UART1_PIN_CONFIG_RX_P34_TX_P35)
#define UART1_8BIT_CONFIG6 (UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_1X_LSB_TX_RX|UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_38400|UART1_PIN_CONFIG_RX_P34_TX_P35)
#define UART1_8BIT_CONFIG7 (UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_1X_MSB_TX_RX|UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_38400|UART1_PIN_CONFIG_RX_P34_TX_P35)
#define UART1_8BIT_CONFIG8 (UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_38400|UART1_PIN_CONFIG_RX_P34_TX_P35)
#define UART1_8BIT_CONFIG9 (UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_38400|UART1_PIN_CONFIG_RX_P34_TX_P35)
#define UART1_8BIT_CONFIG10 (UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_1X_LSB_TX_RX|UART1_8BIT_S1BRG_IHRCO_110592MHZ_BAUD_RATE_57600|UART1_PIN_CONFIG_RX_P34_TX_P35)
#define UART1_8BIT_CONFIG11 (UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_1X_MSB_TX_RX|UART1_8BIT_S1BRG_IHRCO_110592MHZ_BAUD_RATE_57600|UART1_PIN_CONFIG_RX_P34_TX_P35)
#define UART1_8BIT_CONFIG12 (UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_57600|UART1_PIN_CONFIG_RX_P34_TX_P35)
#define UART1_8BIT_CONFIG13 (UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_57600|UART1_PIN_CONFIG_RX_P34_TX_P35)
#define UART1_8BIT_CONFIG14 (UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_1X_LSB_TX_RX|UART1_8BIT_S1BRG_IHRCO_110592MHZ_BAUD_RATE_115200|UART1_PIN_CONFIG_RX_P34_TX_P35)
#define UART1_8BIT_CONFIG15 (UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_1X_MSB_TX_RX|UART1_8BIT_S1BRG_IHRCO_110592MHZ_BAUD_RATE_115200|UART1_PIN_CONFIG_RX_P34_TX_P35)
#define UART1_8BIT_CONFIG16 (UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART1_8BIT_S1BRG_IHRCO_110592MHZ_BAUD_RATE_115200|UART1_PIN_CONFIG_RX_P34_TX_P35)
#define UART1_8BIT_CONFIG17 (UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART1_8BIT_S1BRG_IHRCO_110592MHZ_BAUD_RATE_115200|UART1_PIN_CONFIG_RX_P34_TX_P35)
#define UART1_8BIT_CONFIG18 (UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_230400|UART1_PIN_CONFIG_RX_P34_TX_P35)
#define UART1_8BIT_CONFIG19 (UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_230400|UART1_PIN_CONFIG_RX_P34_TX_P35)
#define UART1_8BIT_CONFIG20 (UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_250000|UART1_PIN_CONFIG_RX_P34_TX_P35)
#define UART1_8BIT_CONFIG21 (UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_250000|UART1_PIN_CONFIG_RX_P34_TX_P35)
#define UART1_8BIT_CONFIG22 (UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_750000|UART1_PIN_CONFIG_RX_P34_TX_P35)
#define UART1_8BIT_CONFIG23 (UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_750000|UART1_PIN_CONFIG_RX_P34_TX_P35)
//constant URT1 mode (0~255) option
#define UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX    (0x00000000)
#define UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX    (0x01000000)
#define UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_1X_LSB_TX_RX    (0x02000000)
#define UART1_8BIT_S1BRG_SYSCLK_DIV_1_BAUD_RATE_1X_MSB_TX_RX    (0x03000000)
//constant URT1 baud rate(0~255) option
#define UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_4800    (0x00000000)
#define UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_9600    (0x00010000)
#define UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_19200    (0x00020000)
#define UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_38400    (0x00030000)
#define UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_57600    (0x00040000)
#define UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_115200    (0x00050000)
#define UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_230400    (0x00060000)
#define UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_250000    (0x00070000)
#define UART1_8BIT_S1BRG_IHRCO_12MHZ_BAUD_RATE_750000    (0x00080000)
#define UART1_8BIT_S1BRG_IHRCO_110592MHZ_BAUD_RATE_57600    (0x00090000)
#define UART1_8BIT_S1BRG_IHRCO_110592MHZ_BAUD_RATE_115200    (0x000a0000)
//constant URT1 pin config (0~15) option
#define UART1_PIN_CONFIG_RX_P10_TX_P11    (0x00000000)
#define UART1_PIN_CONFIG_RX_P60_TX_P61    (0x00000100)
#define UART1_PIN_CONFIG_RX_P44_TX_P45    (0x00000200)
#define UART1_PIN_CONFIG_RX_P34_TX_P35    (0x00000300)
///@endcond




/**
*******************************************************************************
* @brief       UART1 Easy Wizard
* @details     Set SM31 SM01 SM11 S1DOR S1MOD1
* @param[in]    __SELECT__ :
*  @arg\b       UART1_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_Mode_Easy_Select(URT1_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/

#define __DRV_URT1_Mode_Easy_Select(__SELECT__)\
    MWT(\
        __DRV_URT1_EasySetSM31(__SELECT__);\
        __DRV_URT1_EasySetSM01(__SELECT__);\
        __DRV_URT1_EasySetSM11(__SELECT__);\
        __DRV_URT1_EasySetS1DOR(__SELECT__);\
        __DRV_URT1_EasySetS1MOD1(__SELECT__);\
    ;)

/**
*******************************************************************************
* @brief       UART1 Easy Wizard
* @details     Set S1TR S1BRT S1BRC
* @param[in]    __SELECT__ :
*  @arg\b       UART1_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_BaudRate_Easy_Select(URT1_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT1_BaudRate_Easy_Select(__SELECT__)\
    MWT(\
        __DRV_URT1_EasySetS1TR_Stop(__SELECT__);\
        __DRV_URT1_EasySetS1TX12(__SELECT__);\
        __DRV_URT1_EasySetS1BRT(__SELECT__);\
        __DRV_URT1_EasySetS1BRC(__SELECT__);\
        __DRV_URT1_EasySetS1TR_Start(__SELECT__);\
     ;)

/**
*******************************************************************************
* @brief       UART1 PinMux Easy Wizard
* @details     Set S1PS0 S1PS1
* @param[in]    __SELECT__ :
*  @arg\b       UART1_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_PinMux_Easy_Select(URT1_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT1_PinMux_Easy_Select(__SELECT__)\
    MWT(\
        __DRV_URT1_EasySetS1PS1(__SELECT__);\
        __DRV_URT1_EasySetS1PS0(__SELECT__);\
    ;)




/**
*******************************************************************************
* @brief       UART1 Serial Reception
* @details     Set REN1
* @param[in]   __SELECT__ :
*  @arg\b       UART1_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_EasyEnableSerialReception(UART1_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/

#define __DRV_URT1_EasyEnableSerialReception(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CON_Page);\
        ((__SELECT__^UART1_SELECT0)==OPTION_MATCH?(REN1=1):\
        (__SELECT__^UART1_SELECT1)==OPTION_MATCH?(REN1=1):\
        (__SELECT__^UART1_SELECT2)==OPTION_MATCH?(REN1=1):\
        (__SELECT__^UART1_SELECT3)==OPTION_MATCH?(REN1=1):\
        (__SELECT__^UART1_SELECT4)==OPTION_MATCH?(REN1=1):\
        (__SELECT__^UART1_SELECT5)==OPTION_MATCH?(REN1=1):\
        (__SELECT__^UART1_SELECT6)==OPTION_MATCH?(REN1=1):\
        (__SELECT__^UART1_SELECT7)==OPTION_MATCH?(REN1=1):\
        (__SELECT__^UART1_SELECT8)==OPTION_MATCH?(REN1=1):\
        (__SELECT__^UART1_SELECT9)==OPTION_MATCH?(REN1=1):\
        (__SELECT__^UART1_SELECT10)==OPTION_MATCH?(REN1=1):\
        (__SELECT__^UART1_SELECT11)==OPTION_MATCH?(REN1=1):\
        (__SELECT__^UART1_SELECT12)==OPTION_MATCH?(REN1=1):\
        (__SELECT__^UART1_SELECT13)==OPTION_MATCH?(REN1=1):\
        (__SELECT__^UART1_SELECT14)==OPTION_MATCH?(REN1=1):\
        (__SELECT__^UART1_SELECT15)==OPTION_MATCH?(REN1=1):\
        (__SELECT__^UART1_SELECT16)==OPTION_MATCH?(REN1=1):\
        (__SELECT__^UART1_SELECT17)==OPTION_MATCH?(REN1=1):\
        (__SELECT__^UART1_SELECT18)==OPTION_MATCH?(REN1=1):\
        (__SELECT__^UART1_SELECT19)==OPTION_MATCH?(REN1=1):\
        (__SELECT__^UART1_SELECT20)==OPTION_MATCH?(REN1=1):\
        (__SELECT__^UART1_SELECT21)==OPTION_MATCH?(REN1=1):\
        (__SELECT__^UART1_SELECT22)==OPTION_MATCH?(REN1=1):\
        (__SELECT__^UART1_SELECT23)==OPTION_MATCH?(REN1=1):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)


/**
*******************************************************************************
* @brief       UART1 Serial Reception
* @details     Clear REN1
* @param[in]    __SELECT__ :
*  @arg\b       UART1_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_EasyDisableSerialReception(UART1_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT1_EasyDisableSerialReception(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CON_Page);\
        ((__SELECT__^UART1_SELECT0)==OPTION_MATCH?(REN1=0):\
        (__SELECT__^UART1_SELECT1)==OPTION_MATCH?(REN1=0):\
        (__SELECT__^UART1_SELECT2)==OPTION_MATCH?(REN1=0):\
        (__SELECT__^UART1_SELECT3)==OPTION_MATCH?(REN1=0):\
        (__SELECT__^UART1_SELECT4)==OPTION_MATCH?(REN1=0):\
        (__SELECT__^UART1_SELECT5)==OPTION_MATCH?(REN1=0):\
        (__SELECT__^UART1_SELECT6)==OPTION_MATCH?(REN1=0):\
        (__SELECT__^UART1_SELECT7)==OPTION_MATCH?(REN1=0):\
        (__SELECT__^UART1_SELECT8)==OPTION_MATCH?(REN1=0):\
        (__SELECT__^UART1_SELECT9)==OPTION_MATCH?(REN1=0):\
        (__SELECT__^UART1_SELECT10)==OPTION_MATCH?(REN1=0):\
        (__SELECT__^UART1_SELECT11)==OPTION_MATCH?(REN1=0):\
        (__SELECT__^UART1_SELECT12)==OPTION_MATCH?(REN1=0):\
        (__SELECT__^UART1_SELECT13)==OPTION_MATCH?(REN1=0):\
        (__SELECT__^UART1_SELECT14)==OPTION_MATCH?(REN1=0):\
        (__SELECT__^UART1_SELECT15)==OPTION_MATCH?(REN1=0):\
        (__SELECT__^UART1_SELECT16)==OPTION_MATCH?(REN1=0):\
        (__SELECT__^UART1_SELECT17)==OPTION_MATCH?(REN1=0):\
        (__SELECT__^UART1_SELECT18)==OPTION_MATCH?(REN1=0):\
        (__SELECT__^UART1_SELECT19)==OPTION_MATCH?(REN1=0):\
        (__SELECT__^UART1_SELECT20)==OPTION_MATCH?(REN1=0):\
        (__SELECT__^UART1_SELECT21)==OPTION_MATCH?(REN1=0):\
        (__SELECT__^UART1_SELECT22)==OPTION_MATCH?(REN1=0):\
        (__SELECT__^UART1_SELECT23)==OPTION_MATCH?(REN1=0):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)


/**
*******************************************************************************
* @brief       UART1 Mode Select
* @details     Set SM31
* @param[in]    __SELECT__ :
*  @arg\b       UART1_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_EasySetSM31(UART1_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT1_EasySetSM31(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CFG_Page);\
        ((__SELECT__^UART1_SELECT0)==OPTION_MATCH?(S1CFG=S1CFG&(~SM31)):\
        (__SELECT__^UART1_SELECT1)==OPTION_MATCH?(S1CFG=S1CFG&(~SM31)):\
        (__SELECT__^UART1_SELECT2)==OPTION_MATCH?(S1CFG=S1CFG&(~SM31)):\
        (__SELECT__^UART1_SELECT3)==OPTION_MATCH?(S1CFG=S1CFG&(~SM31)):\
        (__SELECT__^UART1_SELECT4)==OPTION_MATCH?(S1CFG=S1CFG&(~SM31)):\
        (__SELECT__^UART1_SELECT5)==OPTION_MATCH?(S1CFG=S1CFG&(~SM31)):\
        (__SELECT__^UART1_SELECT6)==OPTION_MATCH?(S1CFG=S1CFG&(~SM31)):\
        (__SELECT__^UART1_SELECT7)==OPTION_MATCH?(S1CFG=S1CFG&(~SM31)):\
        (__SELECT__^UART1_SELECT8)==OPTION_MATCH?(S1CFG=S1CFG&(~SM31)):\
        (__SELECT__^UART1_SELECT9)==OPTION_MATCH?(S1CFG=S1CFG&(~SM31)):\
        (__SELECT__^UART1_SELECT10)==OPTION_MATCH?(S1CFG=S1CFG&(~SM31)):\
        (__SELECT__^UART1_SELECT11)==OPTION_MATCH?(S1CFG=S1CFG&(~SM31)):\
        (__SELECT__^UART1_SELECT12)==OPTION_MATCH?(S1CFG=S1CFG&(~SM31)):\
        (__SELECT__^UART1_SELECT13)==OPTION_MATCH?(S1CFG=S1CFG&(~SM31)):\
        (__SELECT__^UART1_SELECT14)==OPTION_MATCH?(S1CFG=S1CFG&(~SM31)):\
        (__SELECT__^UART1_SELECT15)==OPTION_MATCH?(S1CFG=S1CFG&(~SM31)):\
        (__SELECT__^UART1_SELECT16)==OPTION_MATCH?(S1CFG=S1CFG&(~SM31)):\
        (__SELECT__^UART1_SELECT17)==OPTION_MATCH?(S1CFG=S1CFG&(~SM31)):\
        (__SELECT__^UART1_SELECT18)==OPTION_MATCH?(S1CFG=S1CFG&(~SM31)):\
        (__SELECT__^UART1_SELECT19)==OPTION_MATCH?(S1CFG=S1CFG&(~SM31)):\
        (__SELECT__^UART1_SELECT20)==OPTION_MATCH?(S1CFG=S1CFG&(~SM31)):\
        (__SELECT__^UART1_SELECT21)==OPTION_MATCH?(S1CFG=S1CFG&(~SM31)):\
        (__SELECT__^UART1_SELECT22)==OPTION_MATCH?(S1CFG=S1CFG&(~SM31)):\
        (__SELECT__^UART1_SELECT23)==OPTION_MATCH?(S1CFG=S1CFG&(~SM31)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)


/**
*******************************************************************************
* @brief       UART1 Mode Select
* @details     Set SM01
* @param[in]    __SELECT__ :
*  @arg\b       UART1_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_EasySetSM01(UART0_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT1_EasySetSM01(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CON_Page);\
        ((__SELECT__^UART1_SELECT0)==OPTION_MATCH?(SM01=0):\
        (__SELECT__^UART1_SELECT1)==OPTION_MATCH?(SM01=0):\
        (__SELECT__^UART1_SELECT2)==OPTION_MATCH?(SM01=0):\
        (__SELECT__^UART1_SELECT3)==OPTION_MATCH?(SM01=0):\
        (__SELECT__^UART1_SELECT4)==OPTION_MATCH?(SM01=0):\
        (__SELECT__^UART1_SELECT5)==OPTION_MATCH?(SM01=0):\
        (__SELECT__^UART1_SELECT6)==OPTION_MATCH?(SM01=0):\
        (__SELECT__^UART1_SELECT7)==OPTION_MATCH?(SM01=0):\
        (__SELECT__^UART1_SELECT8)==OPTION_MATCH?(SM01=0):\
        (__SELECT__^UART1_SELECT9)==OPTION_MATCH?(SM01=0):\
        (__SELECT__^UART1_SELECT10)==OPTION_MATCH?(SM01=0):\
        (__SELECT__^UART1_SELECT11)==OPTION_MATCH?(SM01=0):\
        (__SELECT__^UART1_SELECT12)==OPTION_MATCH?(SM01=0):\
        (__SELECT__^UART1_SELECT13)==OPTION_MATCH?(SM01=0):\
        (__SELECT__^UART1_SELECT14)==OPTION_MATCH?(SM01=0):\
        (__SELECT__^UART1_SELECT15)==OPTION_MATCH?(SM01=0):\
        (__SELECT__^UART1_SELECT16)==OPTION_MATCH?(SM01=0):\
        (__SELECT__^UART1_SELECT17)==OPTION_MATCH?(SM01=0):\
        (__SELECT__^UART1_SELECT18)==OPTION_MATCH?(SM01=0):\
        (__SELECT__^UART1_SELECT19)==OPTION_MATCH?(SM01=0):\
        (__SELECT__^UART1_SELECT20)==OPTION_MATCH?(SM01=0):\
        (__SELECT__^UART1_SELECT21)==OPTION_MATCH?(SM01=0):\
        (__SELECT__^UART1_SELECT22)==OPTION_MATCH?(SM01=0):\
        (__SELECT__^UART1_SELECT23)==OPTION_MATCH?(SM01=0):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)


/**
*******************************************************************************
* @brief       UART1 Mode Select
* @details     Set SM11
* @param[in]    __SELECT__ :
*  @arg\b       UART1_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_EasySetSM11(UART1_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/

#define __DRV_URT1_EasySetSM11(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CON_Page);\
        ((__SELECT__^UART1_SELECT0)==OPTION_MATCH?(SM11=1):\
        (__SELECT__^UART1_SELECT1)==OPTION_MATCH?(SM11=1):\
        (__SELECT__^UART1_SELECT2)==OPTION_MATCH?(SM11=1):\
        (__SELECT__^UART1_SELECT3)==OPTION_MATCH?(SM11=1):\
        (__SELECT__^UART1_SELECT4)==OPTION_MATCH?(SM11=1):\
        (__SELECT__^UART1_SELECT5)==OPTION_MATCH?(SM11=1):\
        (__SELECT__^UART1_SELECT6)==OPTION_MATCH?(SM11=1):\
        (__SELECT__^UART1_SELECT7)==OPTION_MATCH?(SM11=1):\
        (__SELECT__^UART1_SELECT8)==OPTION_MATCH?(SM11=1):\
        (__SELECT__^UART1_SELECT9)==OPTION_MATCH?(SM11=1):\
        (__SELECT__^UART1_SELECT10)==OPTION_MATCH?(SM11=1):\
        (__SELECT__^UART1_SELECT11)==OPTION_MATCH?(SM11=1):\
        (__SELECT__^UART1_SELECT12)==OPTION_MATCH?(SM11=1):\
        (__SELECT__^UART1_SELECT13)==OPTION_MATCH?(SM11=1):\
        (__SELECT__^UART1_SELECT14)==OPTION_MATCH?(SM11=1):\
        (__SELECT__^UART1_SELECT15)==OPTION_MATCH?(SM11=1):\
        (__SELECT__^UART1_SELECT16)==OPTION_MATCH?(SM11=1):\
        (__SELECT__^UART1_SELECT17)==OPTION_MATCH?(SM11=1):\
        (__SELECT__^UART1_SELECT18)==OPTION_MATCH?(SM11=1):\
        (__SELECT__^UART1_SELECT19)==OPTION_MATCH?(SM11=1):\
        (__SELECT__^UART1_SELECT20)==OPTION_MATCH?(SM11=1):\
        (__SELECT__^UART1_SELECT21)==OPTION_MATCH?(SM11=1):\
        (__SELECT__^UART1_SELECT22)==OPTION_MATCH?(SM11=1):\
        (__SELECT__^UART1_SELECT23)==OPTION_MATCH?(SM11=1):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief       UART1 S1BRG Clock Source
* @details     Set S1TX12
* @param[in]    __SELECT__ :
*  @arg\b       UART1_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_EasySetS1TX12(UART1_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT1_EasySetS1TX12(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CFG_Page);\
        ((__SELECT__^UART1_SELECT0)==OPTION_MATCH?(S1CFG=S1CFG|(S1TX12)):\
        (__SELECT__^UART1_SELECT1)==OPTION_MATCH?(S1CFG=S1CFG|(S1TX12)):\
        (__SELECT__^UART1_SELECT2)==OPTION_MATCH?(S1CFG=S1CFG|(S1TX12)):\
        (__SELECT__^UART1_SELECT3)==OPTION_MATCH?(S1CFG=S1CFG|(S1TX12)):\
        (__SELECT__^UART1_SELECT4)==OPTION_MATCH?(S1CFG=S1CFG|(S1TX12)):\
        (__SELECT__^UART1_SELECT5)==OPTION_MATCH?(S1CFG=S1CFG|(S1TX12)):\
        (__SELECT__^UART1_SELECT6)==OPTION_MATCH?(S1CFG=S1CFG|(S1TX12)):\
        (__SELECT__^UART1_SELECT7)==OPTION_MATCH?(S1CFG=S1CFG|(S1TX12)):\
        (__SELECT__^UART1_SELECT8)==OPTION_MATCH?(S1CFG=S1CFG|(S1TX12)):\
        (__SELECT__^UART1_SELECT9)==OPTION_MATCH?(S1CFG=S1CFG|(S1TX12)):\
        (__SELECT__^UART1_SELECT10)==OPTION_MATCH?(S1CFG=S1CFG|(S1TX12)):\
        (__SELECT__^UART1_SELECT11)==OPTION_MATCH?(S1CFG=S1CFG|(S1TX12)):\
        (__SELECT__^UART1_SELECT12)==OPTION_MATCH?(S1CFG=S1CFG|(S1TX12)):\
        (__SELECT__^UART1_SELECT13)==OPTION_MATCH?(S1CFG=S1CFG|(S1TX12)):\
        (__SELECT__^UART1_SELECT14)==OPTION_MATCH?(S1CFG=S1CFG|(S1TX12)):\
        (__SELECT__^UART1_SELECT15)==OPTION_MATCH?(S1CFG=S1CFG|(S1TX12)):\
        (__SELECT__^UART1_SELECT16)==OPTION_MATCH?(S1CFG=S1CFG|(S1TX12)):\
        (__SELECT__^UART1_SELECT17)==OPTION_MATCH?(S1CFG=S1CFG|(S1TX12)):\
        (__SELECT__^UART1_SELECT18)==OPTION_MATCH?(S1CFG=S1CFG|(S1TX12)):\
        (__SELECT__^UART1_SELECT19)==OPTION_MATCH?(S1CFG=S1CFG|(S1TX12)):\
        (__SELECT__^UART1_SELECT20)==OPTION_MATCH?(S1CFG=S1CFG|(S1TX12)):\
        (__SELECT__^UART1_SELECT21)==OPTION_MATCH?(S1CFG=S1CFG|(S1TX12)):\
        (__SELECT__^UART1_SELECT22)==OPTION_MATCH?(S1CFG=S1CFG|(S1TX12)):\
        (__SELECT__^UART1_SELECT23)==OPTION_MATCH?(S1CFG=S1CFG|(S1TX12)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief       UART1 Data Order
* @details     Set S1DOR
* @param[in]    __SELECT__ :
*  @arg\b       UART1_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_EasySetS1DOR(UART1_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/

#define __DRV_URT1_EasySetS1DOR(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CFG_Page);\
        ((__SELECT__^UART1_SELECT0)==OPTION_MATCH?(S1CFG=S1CFG|(S1DOR)):\
        (__SELECT__^UART1_SELECT1)==OPTION_MATCH?(S1CFG=S1CFG&(~S1DOR)):\
        (__SELECT__^UART1_SELECT2)==OPTION_MATCH?(S1CFG=S1CFG|(S1DOR)):\
        (__SELECT__^UART1_SELECT3)==OPTION_MATCH?(S1CFG=S1CFG&(~S1DOR)):\
        (__SELECT__^UART1_SELECT4)==OPTION_MATCH?(S1CFG=S1CFG|(S1DOR)):\
        (__SELECT__^UART1_SELECT5)==OPTION_MATCH?(S1CFG=S1CFG&(~S1DOR)):\
        (__SELECT__^UART1_SELECT6)==OPTION_MATCH?(S1CFG=S1CFG|(S1DOR)):\
        (__SELECT__^UART1_SELECT7)==OPTION_MATCH?(S1CFG=S1CFG&(~S1DOR)):\
        (__SELECT__^UART1_SELECT8)==OPTION_MATCH?(S1CFG=S1CFG|(S1DOR)):\
        (__SELECT__^UART1_SELECT9)==OPTION_MATCH?(S1CFG=S1CFG&(~S1DOR)):\
        (__SELECT__^UART1_SELECT10)==OPTION_MATCH?(S1CFG=S1CFG|(S1DOR)):\
        (__SELECT__^UART1_SELECT11)==OPTION_MATCH?(S1CFG=S1CFG&(~S1DOR)):\
        (__SELECT__^UART1_SELECT12)==OPTION_MATCH?(S1CFG=S1CFG|(S1DOR)):\
        (__SELECT__^UART1_SELECT13)==OPTION_MATCH?(S1CFG=S1CFG&(~S1DOR)):\
        (__SELECT__^UART1_SELECT14)==OPTION_MATCH?(S1CFG=S1CFG|(S1DOR)):\
        (__SELECT__^UART1_SELECT15)==OPTION_MATCH?(S1CFG=S1CFG&(~S1DOR)):\
        (__SELECT__^UART1_SELECT16)==OPTION_MATCH?(S1CFG=S1CFG|(S1DOR)):\
        (__SELECT__^UART1_SELECT17)==OPTION_MATCH?(S1CFG=S1CFG&(~S1DOR)):\
        (__SELECT__^UART1_SELECT18)==OPTION_MATCH?(S1CFG=S1CFG|(S1DOR)):\
        (__SELECT__^UART1_SELECT19)==OPTION_MATCH?(S1CFG=S1CFG&(~S1DOR)):\
        (__SELECT__^UART1_SELECT20)==OPTION_MATCH?(S1CFG=S1CFG|(S1DOR)):\
        (__SELECT__^UART1_SELECT21)==OPTION_MATCH?(S1CFG=S1CFG&(~S1DOR)):\
        (__SELECT__^UART1_SELECT22)==OPTION_MATCH?(S1CFG=S1CFG|(S1DOR)):\
        (__SELECT__^UART1_SELECT23)==OPTION_MATCH?(S1CFG=S1CFG&(~S1DOR)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief       UART1 Enhance Baud Rate
* @details     Set S1MOD1
* @param[in]    __SELECT__ :
*  @arg\b       UART1_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_EasySetS1MOD1(UART1_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT1_EasySetS1MOD1(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CFG_Page);\
        ((__SELECT__^UART1_SELECT0)==OPTION_MATCH?(S1CFG=S1CFG|(S1MOD1)):\
        (__SELECT__^UART1_SELECT1)==OPTION_MATCH?(S1CFG=S1CFG|(S1MOD1)):\
        (__SELECT__^UART1_SELECT2)==OPTION_MATCH?(S1CFG=S1CFG|(S1MOD1)):\
        (__SELECT__^UART1_SELECT3)==OPTION_MATCH?(S1CFG=S1CFG|(S1MOD1)):\
        (__SELECT__^UART1_SELECT4)==OPTION_MATCH?(S1CFG=S1CFG|(S1MOD1)):\
        (__SELECT__^UART1_SELECT5)==OPTION_MATCH?(S1CFG=S1CFG|(S1MOD1)):\
        (__SELECT__^UART1_SELECT6)==OPTION_MATCH?(S1CFG=S1CFG&(~S1MOD1)):\
        (__SELECT__^UART1_SELECT7)==OPTION_MATCH?(S1CFG=S1CFG&(~S1MOD1)):\
        (__SELECT__^UART1_SELECT8)==OPTION_MATCH?(S1CFG=S1CFG|(S1MOD1)):\
        (__SELECT__^UART1_SELECT9)==OPTION_MATCH?(S1CFG=S1CFG|(S1MOD1)):\
        (__SELECT__^UART1_SELECT10)==OPTION_MATCH?(S1CFG=S1CFG&(~S1MOD1)):\
        (__SELECT__^UART1_SELECT11)==OPTION_MATCH?(S1CFG=S1CFG&(~S1MOD1)):\
        (__SELECT__^UART1_SELECT12)==OPTION_MATCH?(S1CFG=S1CFG|(S1MOD1)):\
        (__SELECT__^UART1_SELECT13)==OPTION_MATCH?(S1CFG=S1CFG|(S1MOD1)):\
        (__SELECT__^UART1_SELECT14)==OPTION_MATCH?(S1CFG=S1CFG&(~S1MOD1)):\
        (__SELECT__^UART1_SELECT15)==OPTION_MATCH?(S1CFG=S1CFG&(~S1MOD1)):\
        (__SELECT__^UART1_SELECT16)==OPTION_MATCH?(S1CFG=S1CFG|(S1MOD1)):\
        (__SELECT__^UART1_SELECT17)==OPTION_MATCH?(S1CFG=S1CFG|(S1MOD1)):\
        (__SELECT__^UART1_SELECT18)==OPTION_MATCH?(S1CFG=S1CFG|(S1MOD1)):\
        (__SELECT__^UART1_SELECT19)==OPTION_MATCH?(S1CFG=S1CFG|(S1MOD1)):\
        (__SELECT__^UART1_SELECT20)==OPTION_MATCH?(S1CFG=S1CFG|(S1MOD1)):\
        (__SELECT__^UART1_SELECT21)==OPTION_MATCH?(S1CFG=S1CFG|(S1MOD1)):\
        (__SELECT__^UART1_SELECT22)==OPTION_MATCH?(S1CFG=S1CFG|(S1MOD1)):\
        (__SELECT__^UART1_SELECT23)==OPTION_MATCH?(S1CFG=S1CFG|(S1MOD1)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)



/**
*******************************************************************************
* @brief      UART1  Baud Rate Generator Reload Register
* @details    Write S1BRT
* @param[in]    __SELECT__ :
*  @arg\b       UART1_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_EasySetS1BRT(UART1_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT1_EasySetS1BRT
#define S1BRG_BRGRL_2400_1X_12000000_12T            0xF3        // 0.16%
#define S1BRG_BRGRL_2400_1X_12000000_1T             0x64        // 0.16%
#define S1BRG_BRGRL_4800_1X_12000000_1T             0xB2        // 0.16%
#define S1BRG_BRGRL_9600_1X_12000000_1T             0xD9        // 0.16%
#define S1BRG_BRGRL_19200_1X_12000000_1T            0xEC        // -2.34%
#define S1BRG_BRGRL_38400_1X_12000000_1T            0xF6        // -2.34%
#define S1BRG_BRGRL_2400_2X_12000000_12T            0xE6        // 0.16%
#define S1BRG_BRGRL_4800_2X_12000000_12T            0xF3        // 0.16%
#define S1BRG_BRGRL_4800_2X_12000000_1T             0x64        // 0.16%
#define S1BRG_BRGRL_9600_2X_12000000_1T             0xB2        // 0.16%
#define S1BRG_BRGRL_19200_2X_12000000_1T            0xD9        // 0.16%
#define S1BRG_BRGRL_38400_2X_12000000_1T            0xEC        // -2.34%
#define S1BRG_BRGRL_57600_2X_12000000_1T            0xF3        // 0.16%
#define S1BRG_BRGRL_115200_2X_11059200_1T           0xFA        // 0%
#define S1BRG_BRGRL_115200_1X_11059200_1T           0xFD        // 0%
#define S1BRG_BRGRL_57600_1X_11059200_1T            0xFA        // 0%
#define S1BRG_BRGRL_230400_2X_11059200_1T           0xFD        // 0%
#define S1BRG_BRGRL_250000_2X_12000000_1T           0xFD        // 0%
#define S1BRG_BRGRL_750000_2X_12000000_1T           0xFF        // 0%
///@endcond

#define __DRV_URT1_EasySetS1BRT(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S1BRT_Page);\
        ((__SELECT__^UART1_SELECT0)==OPTION_MATCH?(S1BRT=S1BRG_BRGRL_4800_2X_12000000_1T):\
        (__SELECT__^UART1_SELECT1)==OPTION_MATCH?(S1BRT=S1BRG_BRGRL_4800_2X_12000000_1T):\
        (__SELECT__^UART1_SELECT2)==OPTION_MATCH?(S1BRT=S1BRG_BRGRL_9600_2X_12000000_1T):\
        (__SELECT__^UART1_SELECT3)==OPTION_MATCH?(S1BRT=S1BRG_BRGRL_9600_2X_12000000_1T):\
        (__SELECT__^UART1_SELECT4)==OPTION_MATCH?(S1BRT=S1BRG_BRGRL_19200_2X_12000000_1T):\
        (__SELECT__^UART1_SELECT5)==OPTION_MATCH?(S1BRT=S1BRG_BRGRL_19200_2X_12000000_1T):\
        (__SELECT__^UART1_SELECT6)==OPTION_MATCH?(S1BRT=S1BRG_BRGRL_38400_1X_12000000_1T):\
        (__SELECT__^UART1_SELECT7)==OPTION_MATCH?(S1BRT=S1BRG_BRGRL_38400_1X_12000000_1T):\
        (__SELECT__^UART1_SELECT8)==OPTION_MATCH?(S1BRT=S1BRG_BRGRL_38400_2X_12000000_1T):\
        (__SELECT__^UART1_SELECT9)==OPTION_MATCH?(S1BRT=S1BRG_BRGRL_38400_2X_12000000_1T):\
        (__SELECT__^UART1_SELECT10)==OPTION_MATCH?(S1BRT=S1BRG_BRGRL_57600_1X_11059200_1T):\
        (__SELECT__^UART1_SELECT11)==OPTION_MATCH?(S1BRT=S1BRG_BRGRL_57600_1X_11059200_1T):\
        (__SELECT__^UART1_SELECT12)==OPTION_MATCH?(S1BRT=S1BRG_BRGRL_57600_2X_12000000_1T):\
        (__SELECT__^UART1_SELECT13)==OPTION_MATCH?(S1BRT=S1BRG_BRGRL_57600_2X_12000000_1T):\
        (__SELECT__^UART1_SELECT14)==OPTION_MATCH?(S1BRT=S1BRG_BRGRL_115200_1X_11059200_1T):\
        (__SELECT__^UART1_SELECT15)==OPTION_MATCH?(S1BRT=S1BRG_BRGRL_115200_1X_11059200_1T):\
        (__SELECT__^UART1_SELECT16)==OPTION_MATCH?(S1BRT=S1BRG_BRGRL_115200_2X_11059200_1T):\
        (__SELECT__^UART1_SELECT17)==OPTION_MATCH?(S1BRT=S1BRG_BRGRL_115200_2X_11059200_1T):\
        (__SELECT__^UART1_SELECT18)==OPTION_MATCH?(S1BRT=S1BRG_BRGRL_230400_2X_11059200_1T):\
        (__SELECT__^UART1_SELECT19)==OPTION_MATCH?(S1BRT=S1BRG_BRGRL_230400_2X_11059200_1T):\
        (__SELECT__^UART1_SELECT20)==OPTION_MATCH?(S1BRT=S1BRG_BRGRL_250000_2X_12000000_1T):\
        (__SELECT__^UART1_SELECT21)==OPTION_MATCH?(S1BRT=S1BRG_BRGRL_250000_2X_12000000_1T):\
        (__SELECT__^UART1_SELECT22)==OPTION_MATCH?(S1BRT=S1BRG_BRGRL_750000_2X_12000000_1T):\
        (__SELECT__^UART1_SELECT23)==OPTION_MATCH?(S1BRT=S1BRG_BRGRL_750000_2X_12000000_1T):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)


/**
*******************************************************************************
* @brief      UART1  Baud Rate Generator Reload Register
* @details    Write S1BRC
* @param[in]    __SELECT__ :
*  @arg\b       UART1_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_EasySetS1BRC(UART1_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT1_EasySetS1BRC(__SELECT__)\
    MWT(\
         __DRV_SFR_PageIndex(S1BRC_Page);\
         ((__SELECT__^UART1_SELECT0)==OPTION_MATCH?(S1BRC=S1BRG_BRGRL_4800_2X_12000000_1T):\
         (__SELECT__^UART1_SELECT1)==OPTION_MATCH?(S1BRC=S1BRG_BRGRL_4800_2X_12000000_1T):\
         (__SELECT__^UART1_SELECT2)==OPTION_MATCH?(S1BRC=S1BRG_BRGRL_9600_2X_12000000_1T):\
         (__SELECT__^UART1_SELECT3)==OPTION_MATCH?(S1BRC=S1BRG_BRGRL_9600_2X_12000000_1T):\
         (__SELECT__^UART1_SELECT4)==OPTION_MATCH?(S1BRC=S1BRG_BRGRL_19200_2X_12000000_1T):\
         (__SELECT__^UART1_SELECT5)==OPTION_MATCH?(S1BRC=S1BRG_BRGRL_19200_2X_12000000_1T):\
         (__SELECT__^UART1_SELECT6)==OPTION_MATCH?(S1BRC=S1BRG_BRGRL_38400_1X_12000000_1T):\
         (__SELECT__^UART1_SELECT7)==OPTION_MATCH?(S1BRC=S1BRG_BRGRL_38400_1X_12000000_1T):\
         (__SELECT__^UART1_SELECT8)==OPTION_MATCH?(S1BRC=S1BRG_BRGRL_38400_2X_12000000_1T):\
         (__SELECT__^UART1_SELECT9)==OPTION_MATCH?(S1BRC=S1BRG_BRGRL_38400_2X_12000000_1T):\
         (__SELECT__^UART1_SELECT10)==OPTION_MATCH?(S1BRC=S1BRG_BRGRL_57600_1X_11059200_1T):\
         (__SELECT__^UART1_SELECT11)==OPTION_MATCH?(S1BRC=S1BRG_BRGRL_57600_1X_11059200_1T):\
         (__SELECT__^UART1_SELECT12)==OPTION_MATCH?(S1BRC=S1BRG_BRGRL_57600_2X_12000000_1T):\
         (__SELECT__^UART1_SELECT13)==OPTION_MATCH?(S1BRC=S1BRG_BRGRL_57600_2X_12000000_1T):\
         (__SELECT__^UART1_SELECT14)==OPTION_MATCH?(S1BRC=S1BRG_BRGRL_115200_1X_11059200_1T):\
         (__SELECT__^UART1_SELECT15)==OPTION_MATCH?(S1BRC=S1BRG_BRGRL_115200_1X_11059200_1T):\
         (__SELECT__^UART1_SELECT16)==OPTION_MATCH?(S1BRC=S1BRG_BRGRL_115200_2X_11059200_1T):\
         (__SELECT__^UART1_SELECT17)==OPTION_MATCH?(S1BRC=S1BRG_BRGRL_115200_2X_11059200_1T):\
         (__SELECT__^UART1_SELECT18)==OPTION_MATCH?(S1BRC=S1BRG_BRGRL_230400_2X_11059200_1T):\
         (__SELECT__^UART1_SELECT19)==OPTION_MATCH?(S1BRC=S1BRG_BRGRL_230400_2X_11059200_1T):\
         (__SELECT__^UART1_SELECT20)==OPTION_MATCH?(S1BRC=S1BRG_BRGRL_250000_2X_12000000_1T):\
         (__SELECT__^UART1_SELECT21)==OPTION_MATCH?(S1BRC=S1BRG_BRGRL_250000_2X_12000000_1T):\
         (__SELECT__^UART1_SELECT22)==OPTION_MATCH?(S1BRC=S1BRG_BRGRL_750000_2X_12000000_1T):\
         (__SELECT__^UART1_SELECT23)==OPTION_MATCH?(S1BRC=S1BRG_BRGRL_750000_2X_12000000_1T):_nop_());\
         __DRV_SFR_PageIndex(0);\
     ;)



/**
*******************************************************************************
* @brief       UART1 S1BRG Operation Enable
* @details     Set S1TR
* @param[in]    __SELECT__ :
*  @arg\b       UART1_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_EasySetS1TR_Start(UART1_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT1_EasySetS1TR_Start(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CFG_Page);\
        ((__SELECT__^UART1_SELECT0)==OPTION_MATCH?(S1CFG=S1CFG|(S1TR)):\
        (__SELECT__^UART1_SELECT1)==OPTION_MATCH?(S1CFG=S1CFG|(S1TR)):\
        (__SELECT__^UART1_SELECT2)==OPTION_MATCH?(S1CFG=S1CFG|(S1TR)):\
        (__SELECT__^UART1_SELECT3)==OPTION_MATCH?(S1CFG=S1CFG|(S1TR)):\
        (__SELECT__^UART1_SELECT4)==OPTION_MATCH?(S1CFG=S1CFG|(S1TR)):\
        (__SELECT__^UART1_SELECT5)==OPTION_MATCH?(S1CFG=S1CFG|(S1TR)):\
        (__SELECT__^UART1_SELECT6)==OPTION_MATCH?(S1CFG=S1CFG|(S1TR)):\
        (__SELECT__^UART1_SELECT7)==OPTION_MATCH?(S1CFG=S1CFG|(S1TR)):\
        (__SELECT__^UART1_SELECT8)==OPTION_MATCH?(S1CFG=S1CFG|(S1TR)):\
        (__SELECT__^UART1_SELECT9)==OPTION_MATCH?(S1CFG=S1CFG|(S1TR)):\
        (__SELECT__^UART1_SELECT10)==OPTION_MATCH?(S1CFG=S1CFG|(S1TR)):\
        (__SELECT__^UART1_SELECT11)==OPTION_MATCH?(S1CFG=S1CFG|(S1TR)):\
        (__SELECT__^UART1_SELECT12)==OPTION_MATCH?(S1CFG=S1CFG|(S1TR)):\
        (__SELECT__^UART1_SELECT13)==OPTION_MATCH?(S1CFG=S1CFG|(S1TR)):\
        (__SELECT__^UART1_SELECT14)==OPTION_MATCH?(S1CFG=S1CFG|(S1TR)):\
        (__SELECT__^UART1_SELECT15)==OPTION_MATCH?(S1CFG=S1CFG|(S1TR)):\
        (__SELECT__^UART1_SELECT16)==OPTION_MATCH?(S1CFG=S1CFG|(S1TR)):\
        (__SELECT__^UART1_SELECT17)==OPTION_MATCH?(S1CFG=S1CFG|(S1TR)):\
        (__SELECT__^UART1_SELECT18)==OPTION_MATCH?(S1CFG=S1CFG|(S1TR)):\
        (__SELECT__^UART1_SELECT19)==OPTION_MATCH?(S1CFG=S1CFG|(S1TR)):\
        (__SELECT__^UART1_SELECT20)==OPTION_MATCH?(S1CFG=S1CFG|(S1TR)):\
        (__SELECT__^UART1_SELECT21)==OPTION_MATCH?(S1CFG=S1CFG|(S1TR)):\
        (__SELECT__^UART1_SELECT22)==OPTION_MATCH?(S1CFG=S1CFG|(S1TR)):\
        (__SELECT__^UART1_SELECT23)==OPTION_MATCH?(S1CFG=S1CFG|(S1TR)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief       UART1 S1BRG Operation Disable
* @details     Set S1TR
* @param[in]    __SELECT__ :
*  @arg\b       UART1_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_EasySetS1TR_Stop(UART1_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT1_EasySetS1TR_Stop(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CFG_Page);\
        ((__SELECT__^UART1_SELECT0)==OPTION_MATCH?(S1CFG=S1CFG&(~S1TR)):\
        (__SELECT__^UART1_SELECT1)==OPTION_MATCH?(S1CFG=S1CFG&(~S1TR)):\
        (__SELECT__^UART1_SELECT2)==OPTION_MATCH?(S1CFG=S1CFG&(~S1TR)):\
        (__SELECT__^UART1_SELECT3)==OPTION_MATCH?(S1CFG=S1CFG&(~S1TR)):\
        (__SELECT__^UART1_SELECT4)==OPTION_MATCH?(S1CFG=S1CFG&(~S1TR)):\
        (__SELECT__^UART1_SELECT5)==OPTION_MATCH?(S1CFG=S1CFG&(~S1TR)):\
        (__SELECT__^UART1_SELECT6)==OPTION_MATCH?(S1CFG=S1CFG&(~S1TR)):\
        (__SELECT__^UART1_SELECT7)==OPTION_MATCH?(S1CFG=S1CFG&(~S1TR)):\
        (__SELECT__^UART1_SELECT8)==OPTION_MATCH?(S1CFG=S1CFG&(~S1TR)):\
        (__SELECT__^UART1_SELECT9)==OPTION_MATCH?(S1CFG=S1CFG&(~S1TR)):\
        (__SELECT__^UART1_SELECT10)==OPTION_MATCH?(S1CFG=S1CFG&(~S1TR)):\
        (__SELECT__^UART1_SELECT11)==OPTION_MATCH?(S1CFG=S1CFG&(~S1TR)):\
        (__SELECT__^UART1_SELECT12)==OPTION_MATCH?(S1CFG=S1CFG&(~S1TR)):\
        (__SELECT__^UART1_SELECT13)==OPTION_MATCH?(S1CFG=S1CFG&(~S1TR)):\
        (__SELECT__^UART1_SELECT14)==OPTION_MATCH?(S1CFG=S1CFG&(~S1TR)):\
        (__SELECT__^UART1_SELECT15)==OPTION_MATCH?(S1CFG=S1CFG&(~S1TR)):\
        (__SELECT__^UART1_SELECT16)==OPTION_MATCH?(S1CFG=S1CFG&(~S1TR)):\
        (__SELECT__^UART1_SELECT17)==OPTION_MATCH?(S1CFG=S1CFG&(~S1TR)):\
        (__SELECT__^UART1_SELECT18)==OPTION_MATCH?(S1CFG=S1CFG&(~S1TR)):\
        (__SELECT__^UART1_SELECT19)==OPTION_MATCH?(S1CFG=S1CFG&(~S1TR)):\
        (__SELECT__^UART1_SELECT20)==OPTION_MATCH?(S1CFG=S1CFG&(~S1TR)):\
        (__SELECT__^UART1_SELECT21)==OPTION_MATCH?(S1CFG=S1CFG&(~S1TR)):\
        (__SELECT__^UART1_SELECT22)==OPTION_MATCH?(S1CFG=S1CFG&(~S1TR)):\
        (__SELECT__^UART1_SELECT23)==OPTION_MATCH?(S1CFG=S1CFG&(~S1TR)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)


/**
*******************************************************************************
* @brief        UART1 Pin Config
* @details     Set S1PS1
* @param[in]    __SELECT__ :
*  @arg\b       UART1_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_EasySetS1PS0(UART1_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/

#define __DRV_URT1_EasySetS1PS0(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR9_Page);\
        ((__SELECT__^UART1_SELECT0)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS0)):\
        (__SELECT__^UART1_SELECT1)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS0)):\
        (__SELECT__^UART1_SELECT2)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS0)):\
        (__SELECT__^UART1_SELECT3)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS0)):\
        (__SELECT__^UART1_SELECT4)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS0)):\
        (__SELECT__^UART1_SELECT5)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS0)):\
        (__SELECT__^UART1_SELECT6)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS0)):\
        (__SELECT__^UART1_SELECT7)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS0)):\
        (__SELECT__^UART1_SELECT8)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS0)):\
        (__SELECT__^UART1_SELECT9)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS0)):\
        (__SELECT__^UART1_SELECT10)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS0)):\
        (__SELECT__^UART1_SELECT11)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS0)):\
        (__SELECT__^UART1_SELECT12)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS0)):\
        (__SELECT__^UART1_SELECT13)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS0)):\
        (__SELECT__^UART1_SELECT14)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS0)):\
        (__SELECT__^UART1_SELECT15)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS0)):\
        (__SELECT__^UART1_SELECT16)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS0)):\
        (__SELECT__^UART1_SELECT17)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS0)):\
        (__SELECT__^UART1_SELECT18)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS0)):\
        (__SELECT__^UART1_SELECT19)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS0)):\
        (__SELECT__^UART1_SELECT20)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS0)):\
        (__SELECT__^UART1_SELECT21)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS0)):\
        (__SELECT__^UART1_SELECT22)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS0)):\
        (__SELECT__^UART1_SELECT23)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS0)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief        UART1 Pin Config
* @details     Set S1PS1
* @param[in]    __SELECT__ :
*  @arg\b       UART1_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 default baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
*  @arg\b       UART1_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p34 Tx p35 Tx Rx both SYSCLK/1 double baud rate S1BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_EasySetS1PS1(UART1_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT1_EasySetS1PS1(__SELECT__)\
    MWT(\
         __DRV_SFR_PageIndex(AUXR9_Page);\
         ((__SELECT__^UART1_SELECT0)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS1)):\
         (__SELECT__^UART1_SELECT1)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS1)):\
         (__SELECT__^UART1_SELECT2)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS1)):\
         (__SELECT__^UART1_SELECT3)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS1)):\
         (__SELECT__^UART1_SELECT4)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS1)):\
         (__SELECT__^UART1_SELECT5)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS1)):\
         (__SELECT__^UART1_SELECT6)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS1)):\
         (__SELECT__^UART1_SELECT7)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS1)):\
         (__SELECT__^UART1_SELECT8)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS1)):\
         (__SELECT__^UART1_SELECT9)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS1)):\
         (__SELECT__^UART1_SELECT10)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS1)):\
         (__SELECT__^UART1_SELECT11)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS1)):\
         (__SELECT__^UART1_SELECT12)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS1)):\
         (__SELECT__^UART1_SELECT13)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS1)):\
         (__SELECT__^UART1_SELECT14)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS1)):\
         (__SELECT__^UART1_SELECT15)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS1)):\
         (__SELECT__^UART1_SELECT16)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS1)):\
         (__SELECT__^UART1_SELECT17)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS1)):\
         (__SELECT__^UART1_SELECT18)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS1)):\
         (__SELECT__^UART1_SELECT19)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS1)):\
         (__SELECT__^UART1_SELECT20)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS1)):\
         (__SELECT__^UART1_SELECT21)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS1)):\
         (__SELECT__^UART1_SELECT22)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS1)):\
         (__SELECT__^UART1_SELECT23)==OPTION_MATCH?(AUXR9=AUXR9|(S1PS1)):_nop_());\
         __DRV_SFR_PageIndex(0);\
     ;)









#endif  //MG82F6D64_UART1_MID_H



