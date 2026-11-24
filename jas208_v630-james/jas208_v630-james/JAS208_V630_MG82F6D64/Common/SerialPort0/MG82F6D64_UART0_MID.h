/**
 ******************************************************************************
 *
 * @file        MG82F6D64_UART0_MID.h
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

#ifndef MG82F6D64_UART0_MID_H
#define MG82F6D64_UART0_MID_H




///@cond __DRV_URT0_Easy_Wizard_Init


#define OPTION_MATCH 0

#define UART0_8BIT_CONFIG0 (UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX|UART0_8BIT_S0BRG_IHRCO_12MHZ_BAUD_RATE_9600|UART0_PIN_CONFIG_RX_P30_TX_P31)
#define UART0_8BIT_CONFIG1 (UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_RX|UART0_8BIT_S0BRG_IHRCO_12MHZ_BAUD_RATE_9600|UART0_PIN_CONFIG_RX_P30_TX_P31)
#define UART0_8BIT_CONFIG2 (UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART0_8BIT_S0BRG_IHRCO_12MHZ_BAUD_RATE_9600|UART0_PIN_CONFIG_RX_P30_TX_P31)
#define UART0_8BIT_CONFIG3 (UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX|UART0_8BIT_S0BRG_IHRCO_12MHZ_BAUD_RATE_9600|UART0_PIN_CONFIG_RX_P30_TX_P31)
#define UART0_8BIT_CONFIG4 (UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_RX|UART0_8BIT_S0BRG_IHRCO_12MHZ_BAUD_RATE_9600|UART0_PIN_CONFIG_RX_P30_TX_P31)
#define UART0_8BIT_CONFIG5 (UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART0_8BIT_S0BRG_IHRCO_12MHZ_BAUD_RATE_9600|UART0_PIN_CONFIG_RX_P30_TX_P31)
#define UART0_8BIT_CONFIG6 (UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX|UART0_8BIT_S0BRG_IHRCO_12MHZ_BAUD_RATE_19200|UART0_PIN_CONFIG_RX_P30_TX_P31)
#define UART0_8BIT_CONFIG7 (UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_RX|UART0_8BIT_S0BRG_IHRCO_12MHZ_BAUD_RATE_19200|UART0_PIN_CONFIG_RX_P30_TX_P31)
#define UART0_8BIT_CONFIG8 (UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART0_8BIT_S0BRG_IHRCO_12MHZ_BAUD_RATE_19200|UART0_PIN_CONFIG_RX_P30_TX_P31)
#define UART0_8BIT_CONFIG9 (UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX|UART0_8BIT_S0BRG_IHRCO_12MHZ_BAUD_RATE_19200|UART0_PIN_CONFIG_RX_P30_TX_P31)
#define UART0_8BIT_CONFIG10 (UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_RX|UART0_8BIT_S0BRG_IHRCO_12MHZ_BAUD_RATE_19200|UART0_PIN_CONFIG_RX_P30_TX_P31)
#define UART0_8BIT_CONFIG11 (UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART0_8BIT_S0BRG_IHRCO_12MHZ_BAUD_RATE_19200|UART0_PIN_CONFIG_RX_P30_TX_P31)
#define UART0_8BIT_CONFIG12 (UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX|UART0_8BIT_S0BRG_IHRCO_12MHZ_BAUD_RATE_57600|UART0_PIN_CONFIG_RX_P30_TX_P31)
#define UART0_8BIT_CONFIG13 (UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_RX|UART0_8BIT_S0BRG_IHRCO_12MHZ_BAUD_RATE_57600|UART0_PIN_CONFIG_RX_P30_TX_P31)
#define UART0_8BIT_CONFIG14 (UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART0_8BIT_S0BRG_IHRCO_12MHZ_BAUD_RATE_57600|UART0_PIN_CONFIG_RX_P30_TX_P31)
#define UART0_8BIT_CONFIG15 (UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX|UART0_8BIT_S0BRG_IHRCO_12MHZ_BAUD_RATE_57600|UART0_PIN_CONFIG_RX_P30_TX_P31)
#define UART0_8BIT_CONFIG16 (UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_RX|UART0_8BIT_S0BRG_IHRCO_12MHZ_BAUD_RATE_57600|UART0_PIN_CONFIG_RX_P30_TX_P31)
#define UART0_8BIT_CONFIG17 (UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART0_8BIT_S0BRG_IHRCO_12MHZ_BAUD_RATE_57600|UART0_PIN_CONFIG_RX_P30_TX_P31)
#define UART0_8BIT_CONFIG18 (UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX|UART0_8BIT_S0BRG_IHRCO_110592MHZ_BAUD_RATE_115200|UART0_PIN_CONFIG_RX_P30_TX_P31)
#define UART0_8BIT_CONFIG19 (UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_RX|UART0_8BIT_S0BRG_IHRCO_110592MHZ_BAUD_RATE_115200|UART0_PIN_CONFIG_RX_P30_TX_P31)
#define UART0_8BIT_CONFIG20 (UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART0_8BIT_S0BRG_IHRCO_110592MHZ_BAUD_RATE_115200|UART0_PIN_CONFIG_RX_P30_TX_P31)
#define UART0_8BIT_CONFIG21 (UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX|UART0_8BIT_S0BRG_IHRCO_110592MHZ_BAUD_RATE_115200|UART0_PIN_CONFIG_RX_P30_TX_P31)
#define UART0_8BIT_CONFIG22 (UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_RX|UART0_8BIT_S0BRG_IHRCO_110592MHZ_BAUD_RATE_115200|UART0_PIN_CONFIG_RX_P30_TX_P31)
#define UART0_8BIT_CONFIG23 (UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART0_8BIT_S0BRG_IHRCO_110592MHZ_BAUD_RATE_115200|UART0_PIN_CONFIG_RX_P30_TX_P31)
//constant URT0 mode (0~255) option
#define UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX    (0x00000000)
#define UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_RX    (0x01000000)
#define UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX (0x02000000)
#define UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX    (0x03000000)
#define UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_RX    (0x04000000)
#define UART0_8BIT_S0BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX (0x05000000)
//constant URT0 baud rate(0~255) option
#define UART0_8BIT_S0BRG_IHRCO_12MHZ_BAUD_RATE_9600   (0x00000000)
#define UART0_8BIT_S0BRG_IHRCO_12MHZ_BAUD_RATE_19200  (0x00010000)
#define UART0_8BIT_S0BRG_IHRCO_12MHZ_BAUD_RATE_57600  (0x00020000)
#define UART0_8BIT_S0BRG_IHRCO_12MHZ_BAUD_RATE_115200 (0x00030000)
#define UART0_8BIT_S0BRG_IHRCO_110592MHZ_BAUD_RATE_115200 (0x00040000)
//constant URT0 pin config (0~15) option
#define UART0_PIN_CONFIG_RX_P30_TX_P31 (0x00000000)
#define UART0_PIN_CONFIG_RX_P44_TX_P45 (0x00000100)
#define UART0_PIN_CONFIG_RX_P31_TX_P30 (0x00000200)
#define UART0_PIN_CONFIG_RX_P17_TX_P22 (0x00000300)
///@endcond



/**
*******************************************************************************
* @brief        UART0 Easy Wizard
* @details      Set SM30 SM00 SM10 S0DOR SMOD2 SMOD1 SMOD3 S0TX12 S0RCK S0TCK
* @param[in]    __SELECT__ :
*  @arg\b       UART0_8BIT_CONFIG0 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG1 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG4 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG5 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG6 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG7 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG8 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG9 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG10 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG11 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG13 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG14 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG15 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG16 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG17 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG18 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG19 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG20 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG21 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG22 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG23 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_URT0_Mode_Easy_Select(UART0_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT0_Mode_Easy_Select(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(0);\
        __DRV_URT0_EasySetSM30(__SELECT__);\
        __DRV_URT0_EasySetSM00(__SELECT__);\
        __DRV_URT0_EasySetSM10(__SELECT__);\
        __DRV_URT0_EasySetS0DOR(__SELECT__);\
        __DRV_URT0_EasySetSMOD2(__SELECT__);\
        __DRV_URT0_EasySetSMOD1(__SELECT__);\
        __DRV_URT0_EasySetSMOD3(__SELECT__);\
        __DRV_URT0_EasySetS0TX12(__SELECT__);\
        __DRV_URT0_EasySetS0RCK(__SELECT__);\
        __DRV_URT0_EasySetS0TCK(__SELECT__);\
    ;)


/**
*******************************************************************************
* @brief        UART0 Easy Wizard
* @details      Set S0TR S0BRT S0BRC
* @param[in]    __SELECT__ :
*  @arg\b       UART0_8BIT_CONFIG0 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG1 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG4 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG5 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG6 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG7 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG8 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG9 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG10 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG11 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG13 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG14 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG15 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG16 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG17 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG18 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG19 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG20 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG21 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG22 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG23 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_URT0_BaudRate_Easy_Select(UART0_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/

#define __DRV_URT0_BaudRate_Easy_Select(__SELECT__)\
    MWT(\
        __DRV_URT0_EasySetS0TR_Stop(__SELECT__);\
        __DRV_URT0_EasySetS0BRT(__SELECT__);\
        __DRV_URT0_EasySetS0BRC(__SELECT__);\
        __DRV_URT0_EasySetS0TR_Start(__SELECT__);\
    ;)

/**
*******************************************************************************
* @brief        UART0 PinMux Easy Wizard
* @details      Set S0PS0 S0PS1
* @param[in]    __SELECT__ :
*  @arg\b       UART0_8BIT_CONFIG0 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG1 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG4 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG5 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG6 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG7 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG8 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG9 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG10 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG11 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG13 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG14 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG15 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG16 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG17 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG18 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG19 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG20 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG21 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG22 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG23 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_URT0_PinMux_Easy_Select(UART0_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/

#define __DRV_URT0_PinMux_Easy_Select(__SELECT__)\
    MWT(\
        __DRV_URT0_EasySetS0PS0(__SELECT__);\
        __DRV_URT0_EasySetS0PS1(__SELECT__);\
    ;)



/**
*******************************************************************************
* @brief       UART0 Serial Reception
* @details     Set REN0
* @param[in]  __SELECT__ :
*  @arg\b       UART0_8BIT_CONFIG0 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG1 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG4 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG5 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG6 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG7 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG8 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG9 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG10 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG11 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG13 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG14 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG15 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG16 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG17 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG18 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG19 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG20 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG21 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG22 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG23 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_EasyEnableSerialReception(UART0_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/

#define __DRV_URT0_EasyEnableSerialReception(__SELECT__)\
    MWT(\
        ((__SELECT__^UART0_SELECT0)==OPTION_MATCH?(REN0=1):\
        (__SELECT__^UART0_SELECT1)==OPTION_MATCH?(REN0=1):\
        (__SELECT__^UART0_SELECT2)==OPTION_MATCH?(REN0=1):\
        (__SELECT__^UART0_SELECT3)==OPTION_MATCH?(REN0=1):\
        (__SELECT__^UART0_SELECT4)==OPTION_MATCH?(REN0=1):\
        (__SELECT__^UART0_SELECT5)==OPTION_MATCH?(REN0=1):\
        (__SELECT__^UART0_SELECT6)==OPTION_MATCH?(REN0=1):\
        (__SELECT__^UART0_SELECT7)==OPTION_MATCH?(REN0=1):\
        (__SELECT__^UART0_SELECT8)==OPTION_MATCH?(REN0=1):\
        (__SELECT__^UART0_SELECT9)==OPTION_MATCH?(REN0=1):\
        (__SELECT__^UART0_SELECT10)==OPTION_MATCH?(REN0=1):\
        (__SELECT__^UART0_SELECT11)==OPTION_MATCH?(REN0=1):\
        (__SELECT__^UART0_SELECT12)==OPTION_MATCH?(REN0=1):\
        (__SELECT__^UART0_SELECT13)==OPTION_MATCH?(REN0=1):\
        (__SELECT__^UART0_SELECT14)==OPTION_MATCH?(REN0=1):\
        (__SELECT__^UART0_SELECT15)==OPTION_MATCH?(REN0=1):\
        (__SELECT__^UART0_SELECT16)==OPTION_MATCH?(REN0=1):\
        (__SELECT__^UART0_SELECT17)==OPTION_MATCH?(REN0=1):\
        (__SELECT__^UART0_SELECT18)==OPTION_MATCH?(REN0=1):\
        (__SELECT__^UART0_SELECT19)==OPTION_MATCH?(REN0=1):\
        (__SELECT__^UART0_SELECT20)==OPTION_MATCH?(REN0=1):\
        (__SELECT__^UART0_SELECT21)==OPTION_MATCH?(REN0=1):\
        (__SELECT__^UART0_SELECT22)==OPTION_MATCH?(REN0=1):\
        (__SELECT__^UART0_SELECT23)==OPTION_MATCH?(REN0=1):_nop_());\
    ;)



/**
*******************************************************************************
* @brief       UART0 Serial Reception
* @details     Clear REN0
* @param[in]   __SELECT__ :
*  @arg\b       UART0_8BIT_CONFIG0 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG1 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG4 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG5 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG6 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG7 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG8 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG9 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG10 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG11 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG13 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG14 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG15 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG16 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG17 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG18 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG19 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG20 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG21 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG22 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG23 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_EasyDisableSerialReception(UART0_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/

#define __DRV_URT0_EasyDisableSerialReception(__SELECT__)\
    MWT(\
        ((__SELECT__^UART0_SELECT0)==OPTION_MATCH?(REN0=0):\
        (__SELECT__^UART0_SELECT1)==OPTION_MATCH?(REN0=0):\
        (__SELECT__^UART0_SELECT2)==OPTION_MATCH?(REN0=0):\
        (__SELECT__^UART0_SELECT3)==OPTION_MATCH?(REN0=0):\
        (__SELECT__^UART0_SELECT4)==OPTION_MATCH?(REN0=0):\
        (__SELECT__^UART0_SELECT5)==OPTION_MATCH?(REN0=0):\
        (__SELECT__^UART0_SELECT6)==OPTION_MATCH?(REN0=0):\
        (__SELECT__^UART0_SELECT7)==OPTION_MATCH?(REN0=0):\
        (__SELECT__^UART0_SELECT8)==OPTION_MATCH?(REN0=0):\
        (__SELECT__^UART0_SELECT9)==OPTION_MATCH?(REN0=0):\
        (__SELECT__^UART0_SELECT10)==OPTION_MATCH?(REN0=0):\
        (__SELECT__^UART0_SELECT11)==OPTION_MATCH?(REN0=0):\
        (__SELECT__^UART0_SELECT12)==OPTION_MATCH?(REN0=0):\
        (__SELECT__^UART0_SELECT13)==OPTION_MATCH?(REN0=0):\
        (__SELECT__^UART0_SELECT14)==OPTION_MATCH?(REN0=0):\
        (__SELECT__^UART0_SELECT15)==OPTION_MATCH?(REN0=0):\
        (__SELECT__^UART0_SELECT16)==OPTION_MATCH?(REN0=0):\
        (__SELECT__^UART0_SELECT17)==OPTION_MATCH?(REN0=0):\
        (__SELECT__^UART0_SELECT18)==OPTION_MATCH?(REN0=0):\
        (__SELECT__^UART0_SELECT19)==OPTION_MATCH?(REN0=0):\
        (__SELECT__^UART0_SELECT20)==OPTION_MATCH?(REN0=0):\
        (__SELECT__^UART0_SELECT21)==OPTION_MATCH?(REN0=0):\
        (__SELECT__^UART0_SELECT22)==OPTION_MATCH?(REN0=0):\
        (__SELECT__^UART0_SELECT23)==OPTION_MATCH?(REN0=0):_nop_());\
    ;)


/**
*******************************************************************************
* @brief       UART0 Mode Select
* @details     Set SM30
* @param[in]    __SELECT__ :
*  @arg\b       UART0_8BIT_CONFIG0 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG1 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG4 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG5 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG6 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG7 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG8 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG9 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG10 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG11 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG13 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG14 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG15 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG16 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG17 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG18 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG19 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG20 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG21 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG22 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG23 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
* @return       None
* @note         None
* @par         Example
* @code
__DRV_URT0_EasySetSM30(UART0_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT0_EasySetSM30(__SELECT__)\
    MWT(\
        ((__SELECT__^UART0_SELECT0)==OPTION_MATCH?(S0CFG=S0CFG&(~SM30)):\
        (__SELECT__^UART0_SELECT1)==OPTION_MATCH?(S0CFG=S0CFG&(~SM30)):\
        (__SELECT__^UART0_SELECT2)==OPTION_MATCH?(S0CFG=S0CFG&(~SM30)):\
        (__SELECT__^UART0_SELECT3)==OPTION_MATCH?(S0CFG=S0CFG&(~SM30)):\
        (__SELECT__^UART0_SELECT4)==OPTION_MATCH?(S0CFG=S0CFG&(~SM30)):\
        (__SELECT__^UART0_SELECT5)==OPTION_MATCH?(S0CFG=S0CFG&(~SM30)):\
        (__SELECT__^UART0_SELECT6)==OPTION_MATCH?(S0CFG=S0CFG&(~SM30)):\
        (__SELECT__^UART0_SELECT7)==OPTION_MATCH?(S0CFG=S0CFG&(~SM30)):\
        (__SELECT__^UART0_SELECT8)==OPTION_MATCH?(S0CFG=S0CFG&(~SM30)):\
        (__SELECT__^UART0_SELECT9)==OPTION_MATCH?(S0CFG=S0CFG&(~SM30)):\
        (__SELECT__^UART0_SELECT10)==OPTION_MATCH?(S0CFG=S0CFG&(~SM30)):\
        (__SELECT__^UART0_SELECT11)==OPTION_MATCH?(S0CFG=S0CFG&(~SM30)):\
        (__SELECT__^UART0_SELECT12)==OPTION_MATCH?(S0CFG=S0CFG&(~SM30)):\
        (__SELECT__^UART0_SELECT13)==OPTION_MATCH?(S0CFG=S0CFG&(~SM30)):\
        (__SELECT__^UART0_SELECT14)==OPTION_MATCH?(S0CFG=S0CFG&(~SM30)):\
        (__SELECT__^UART0_SELECT15)==OPTION_MATCH?(S0CFG=S0CFG&(~SM30)):\
        (__SELECT__^UART0_SELECT16)==OPTION_MATCH?(S0CFG=S0CFG&(~SM30)):\
        (__SELECT__^UART0_SELECT17)==OPTION_MATCH?(S0CFG=S0CFG&(~SM30)):\
        (__SELECT__^UART0_SELECT18)==OPTION_MATCH?(S0CFG=S0CFG&(~SM30)):\
        (__SELECT__^UART0_SELECT19)==OPTION_MATCH?(S0CFG=S0CFG&(~SM30)):\
        (__SELECT__^UART0_SELECT20)==OPTION_MATCH?(S0CFG=S0CFG&(~SM30)):\
        (__SELECT__^UART0_SELECT21)==OPTION_MATCH?(S0CFG=S0CFG&(~SM30)):\
        (__SELECT__^UART0_SELECT22)==OPTION_MATCH?(S0CFG=S0CFG&(~SM30)):\
        (__SELECT__^UART0_SELECT23)==OPTION_MATCH?(S0CFG=S0CFG&(~SM30)):_nop_());\
    ;)


/**
*******************************************************************************
* @brief       UART0 Mode Select
* @details     Set SM00
* @param[in]    __SELECT__ :
*  @arg\b       UART0_8BIT_CONFIG0 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG1 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG4 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG5 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG6 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG7 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG8 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG9 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG10 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG11 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG13 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG14 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG15 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG16 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG17 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG18 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG19 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG20 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG21 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG22 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG23 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_EasySetSM00(UART0_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/

#define __DRV_URT0_EasySetSM00(__SELECT__)\
    MWT(\
        ((__SELECT__^UART0_SELECT0)==OPTION_MATCH?(SM00_FE=0):\
        (__SELECT__^UART0_SELECT1)==OPTION_MATCH?(SM00_FE=0):\
        (__SELECT__^UART0_SELECT2)==OPTION_MATCH?(SM00_FE=0):\
        (__SELECT__^UART0_SELECT3)==OPTION_MATCH?(SM00_FE=0):\
        (__SELECT__^UART0_SELECT4)==OPTION_MATCH?(SM00_FE=0):\
        (__SELECT__^UART0_SELECT5)==OPTION_MATCH?(SM00_FE=0):\
        (__SELECT__^UART0_SELECT6)==OPTION_MATCH?(SM00_FE=0):\
        (__SELECT__^UART0_SELECT7)==OPTION_MATCH?(SM00_FE=0):\
        (__SELECT__^UART0_SELECT8)==OPTION_MATCH?(SM00_FE=0):\
        (__SELECT__^UART0_SELECT9)==OPTION_MATCH?(SM00_FE=0):\
        (__SELECT__^UART0_SELECT10)==OPTION_MATCH?(SM00_FE=0):\
        (__SELECT__^UART0_SELECT11)==OPTION_MATCH?(SM00_FE=0):\
        (__SELECT__^UART0_SELECT12)==OPTION_MATCH?(SM00_FE=0):\
        (__SELECT__^UART0_SELECT13)==OPTION_MATCH?(SM00_FE=0):\
        (__SELECT__^UART0_SELECT14)==OPTION_MATCH?(SM00_FE=0):\
        (__SELECT__^UART0_SELECT15)==OPTION_MATCH?(SM00_FE=0):\
        (__SELECT__^UART0_SELECT16)==OPTION_MATCH?(SM00_FE=0):\
        (__SELECT__^UART0_SELECT17)==OPTION_MATCH?(SM00_FE=0):\
        (__SELECT__^UART0_SELECT18)==OPTION_MATCH?(SM00_FE=0):\
        (__SELECT__^UART0_SELECT19)==OPTION_MATCH?(SM00_FE=0):\
        (__SELECT__^UART0_SELECT20)==OPTION_MATCH?(SM00_FE=0):\
        (__SELECT__^UART0_SELECT21)==OPTION_MATCH?(SM00_FE=0):\
        (__SELECT__^UART0_SELECT22)==OPTION_MATCH?(SM00_FE=0):\
        (__SELECT__^UART0_SELECT23)==OPTION_MATCH?(SM00_FE=0):_nop_());\
    ;)


/**
*******************************************************************************
* @brief       UART0 Mode Select
* @details     Set SM10
* @param[in]    __SELECT__ :
*  @arg\b       UART0_8BIT_CONFIG0 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG1 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG4 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG5 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG6 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG7 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG8 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG9 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG10 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG11 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG13 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG14 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG15 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG16 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG17 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG18 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG19 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG20 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG21 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG22 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG23 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_EasySetSM10(UART0_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/

#define __DRV_URT0_EasySetSM10(__SELECT__)\
    MWT(\
        ((__SELECT__^UART0_SELECT0)==OPTION_MATCH?(SM10=1):\
        (__SELECT__^UART0_SELECT1)==OPTION_MATCH?(SM10=1):\
        (__SELECT__^UART0_SELECT2)==OPTION_MATCH?(SM10=1):\
        (__SELECT__^UART0_SELECT3)==OPTION_MATCH?(SM10=1):\
        (__SELECT__^UART0_SELECT4)==OPTION_MATCH?(SM10=1):\
        (__SELECT__^UART0_SELECT5)==OPTION_MATCH?(SM10=1):\
        (__SELECT__^UART0_SELECT6)==OPTION_MATCH?(SM10=1):\
        (__SELECT__^UART0_SELECT7)==OPTION_MATCH?(SM10=1):\
        (__SELECT__^UART0_SELECT8)==OPTION_MATCH?(SM10=1):\
        (__SELECT__^UART0_SELECT9)==OPTION_MATCH?(SM10=1):\
        (__SELECT__^UART0_SELECT10)==OPTION_MATCH?(SM10=1):\
        (__SELECT__^UART0_SELECT11)==OPTION_MATCH?(SM10=1):\
        (__SELECT__^UART0_SELECT12)==OPTION_MATCH?(SM10=1):\
        (__SELECT__^UART0_SELECT13)==OPTION_MATCH?(SM10=1):\
        (__SELECT__^UART0_SELECT14)==OPTION_MATCH?(SM10=1):\
        (__SELECT__^UART0_SELECT15)==OPTION_MATCH?(SM10=1):\
        (__SELECT__^UART0_SELECT16)==OPTION_MATCH?(SM10=1):\
        (__SELECT__^UART0_SELECT17)==OPTION_MATCH?(SM10=1):\
        (__SELECT__^UART0_SELECT18)==OPTION_MATCH?(SM10=1):\
        (__SELECT__^UART0_SELECT19)==OPTION_MATCH?(SM10=1):\
        (__SELECT__^UART0_SELECT20)==OPTION_MATCH?(SM10=1):\
        (__SELECT__^UART0_SELECT21)==OPTION_MATCH?(SM10=1):\
        (__SELECT__^UART0_SELECT22)==OPTION_MATCH?(SM10=1):\
        (__SELECT__^UART0_SELECT23)==OPTION_MATCH?(SM10=1):_nop_());\
    ;)


/**
*******************************************************************************
* @brief       UART0 S0CR1 Access Enable.
* @details     Set SMOD3
* @param[in]   __SELECT__ :
*  @arg\b       UART0_8BIT_CONFIG0 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG1 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG4 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG5 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG6 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG7 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG8 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG9 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG10 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG11 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG13 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG14 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG15 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG16 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG17 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG18 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG19 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG20 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG21 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG22 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG23 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_EasySetSMOD3(UART0_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/

#define __DRV_URT0_EasySetSMOD3(__SELECT__)\
    MWT(\
        ((__SELECT__^UART0_SELECT0)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD3)):\
        (__SELECT__^UART0_SELECT1)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD3)):\
        (__SELECT__^UART0_SELECT2)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD3)):\
        (__SELECT__^UART0_SELECT3)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD3)):\
        (__SELECT__^UART0_SELECT4)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD3)):\
        (__SELECT__^UART0_SELECT5)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD3)):\
        (__SELECT__^UART0_SELECT6)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD3)):\
        (__SELECT__^UART0_SELECT7)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD3)):\
        (__SELECT__^UART0_SELECT8)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD3)):\
        (__SELECT__^UART0_SELECT9)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD3)):\
        (__SELECT__^UART0_SELECT10)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD3)):\
        (__SELECT__^UART0_SELECT11)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD3)):\
        (__SELECT__^UART0_SELECT12)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD3)):\
        (__SELECT__^UART0_SELECT13)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD3)):\
        (__SELECT__^UART0_SELECT14)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD3)):\
        (__SELECT__^UART0_SELECT15)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD3)):\
        (__SELECT__^UART0_SELECT16)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD3)):\
        (__SELECT__^UART0_SELECT17)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD3)):\
        (__SELECT__^UART0_SELECT18)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD3)):\
        (__SELECT__^UART0_SELECT19)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD3)):\
        (__SELECT__^UART0_SELECT20)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD3)):\
        (__SELECT__^UART0_SELECT21)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD3)):\
        (__SELECT__^UART0_SELECT22)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD3)):\
        (__SELECT__^UART0_SELECT23)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD3)):_nop_());\
    ;)

/**
*******************************************************************************
* @brief       UART0 S0BRG Clock Source
* @details     Set S0TX12
* @param[in]   __SELECT__ :
*  @arg\b       UART0_8BIT_CONFIG0 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG1 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG4 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG5 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG6 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG7 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG8 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG9 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG10 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG11 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG13 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG14 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG15 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG16 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG17 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG18 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG19 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG20 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG21 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG22 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG23 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
* @return      None
* @note        Before set S0TX12 , set SMOD3=1 first
* @par         Example
* @code
__DRV_URT0_EasySetS0TX12(UART0_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT0_EasySetS0TX12(__SELECT__)\
    MWT(\
        ((__SELECT__^UART0_SELECT0)==OPTION_MATCH?(S0CR1=S0CR1|(S0TX12)):\
        (__SELECT__^UART0_SELECT1)==OPTION_MATCH?(S0CR1=S0CR1|(S0TX12)):\
        (__SELECT__^UART0_SELECT2)==OPTION_MATCH?(S0CR1=S0CR1|(S0TX12)):\
        (__SELECT__^UART0_SELECT3)==OPTION_MATCH?(S0CR1=S0CR1|(S0TX12)):\
        (__SELECT__^UART0_SELECT4)==OPTION_MATCH?(S0CR1=S0CR1|(S0TX12)):\
        (__SELECT__^UART0_SELECT5)==OPTION_MATCH?(S0CR1=S0CR1|(S0TX12)):\
        (__SELECT__^UART0_SELECT6)==OPTION_MATCH?(S0CR1=S0CR1|(S0TX12)):\
        (__SELECT__^UART0_SELECT7)==OPTION_MATCH?(S0CR1=S0CR1|(S0TX12)):\
        (__SELECT__^UART0_SELECT8)==OPTION_MATCH?(S0CR1=S0CR1|(S0TX12)):\
        (__SELECT__^UART0_SELECT9)==OPTION_MATCH?(S0CR1=S0CR1|(S0TX12)):\
        (__SELECT__^UART0_SELECT10)==OPTION_MATCH?(S0CR1=S0CR1|(S0TX12)):\
        (__SELECT__^UART0_SELECT11)==OPTION_MATCH?(S0CR1=S0CR1|(S0TX12)):\
        (__SELECT__^UART0_SELECT12)==OPTION_MATCH?(S0CR1=S0CR1|(S0TX12)):\
        (__SELECT__^UART0_SELECT13)==OPTION_MATCH?(S0CR1=S0CR1|(S0TX12)):\
        (__SELECT__^UART0_SELECT14)==OPTION_MATCH?(S0CR1=S0CR1|(S0TX12)):\
        (__SELECT__^UART0_SELECT15)==OPTION_MATCH?(S0CR1=S0CR1|(S0TX12)):\
        (__SELECT__^UART0_SELECT16)==OPTION_MATCH?(S0CR1=S0CR1|(S0TX12)):\
        (__SELECT__^UART0_SELECT17)==OPTION_MATCH?(S0CR1=S0CR1|(S0TX12)):\
        (__SELECT__^UART0_SELECT18)==OPTION_MATCH?(S0CR1=S0CR1|(S0TX12)):\
        (__SELECT__^UART0_SELECT19)==OPTION_MATCH?(S0CR1=S0CR1|(S0TX12)):\
        (__SELECT__^UART0_SELECT20)==OPTION_MATCH?(S0CR1=S0CR1|(S0TX12)):\
        (__SELECT__^UART0_SELECT21)==OPTION_MATCH?(S0CR1=S0CR1|(S0TX12)):\
        (__SELECT__^UART0_SELECT22)==OPTION_MATCH?(S0CR1=S0CR1|(S0TX12)):\
        (__SELECT__^UART0_SELECT23)==OPTION_MATCH?(S0CR1=S0CR1|(S0TX12)):_nop_());\
    ;)


/**
*******************************************************************************
* @brief       UART0 Receive Clock
* @details     Set S0RCK
* @param[in]    __SELECT__ :
*  @arg\b       UART0_8BIT_CONFIG0 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG1 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG4 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG5 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG6 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG7 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG8 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG9 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG10 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG11 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG13 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG14 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG15 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG16 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG17 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG18 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG19 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG20 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG21 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG22 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG23 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_EasySetS0RCK(URT0_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT0_EasySetS0RCK(__SELECT__)\
    MWT(\
        ((__SELECT__^UART0_SELECT0)==OPTION_MATCH?(S0CR1=S0CR1&(~S0RCK)):\
        (__SELECT__^UART0_SELECT1)==OPTION_MATCH?(S0CR1=S0CR1|(S0RCK)):\
        (__SELECT__^UART0_SELECT2)==OPTION_MATCH?(S0CR1=S0CR1|(S0RCK)):\
        (__SELECT__^UART0_SELECT3)==OPTION_MATCH?(S0CR1=S0CR1&(~S0RCK)):\
        (__SELECT__^UART0_SELECT4)==OPTION_MATCH?(S0CR1=S0CR1|(S0RCK)):\
        (__SELECT__^UART0_SELECT5)==OPTION_MATCH?(S0CR1=S0CR1|(S0RCK)):\
        (__SELECT__^UART0_SELECT6)==OPTION_MATCH?(S0CR1=S0CR1&(~S0RCK)):\
        (__SELECT__^UART0_SELECT7)==OPTION_MATCH?(S0CR1=S0CR1|(S0RCK)):\
        (__SELECT__^UART0_SELECT8)==OPTION_MATCH?(S0CR1=S0CR1|(S0RCK)):\
        (__SELECT__^UART0_SELECT9)==OPTION_MATCH?(S0CR1=S0CR1&(~S0RCK)):\
        (__SELECT__^UART0_SELECT10)==OPTION_MATCH?(S0CR1=S0CR1|(S0RCK)):\
        (__SELECT__^UART0_SELECT11)==OPTION_MATCH?(S0CR1=S0CR1|(S0RCK)):\
        (__SELECT__^UART0_SELECT12)==OPTION_MATCH?(S0CR1=S0CR1&(~S0RCK)):\
        (__SELECT__^UART0_SELECT13)==OPTION_MATCH?(S0CR1=S0CR1|(S0RCK)):\
        (__SELECT__^UART0_SELECT14)==OPTION_MATCH?(S0CR1=S0CR1|(S0RCK)):\
        (__SELECT__^UART0_SELECT15)==OPTION_MATCH?(S0CR1=S0CR1&(~S0RCK)):\
        (__SELECT__^UART0_SELECT16)==OPTION_MATCH?(S0CR1=S0CR1|(S0RCK)):\
        (__SELECT__^UART0_SELECT17)==OPTION_MATCH?(S0CR1=S0CR1|(S0RCK)):\
        (__SELECT__^UART0_SELECT18)==OPTION_MATCH?(S0CR1=S0CR1&(~S0RCK)):\
        (__SELECT__^UART0_SELECT19)==OPTION_MATCH?(S0CR1=S0CR1|(S0RCK)):\
        (__SELECT__^UART0_SELECT20)==OPTION_MATCH?(S0CR1=S0CR1|(S0RCK)):\
        (__SELECT__^UART0_SELECT21)==OPTION_MATCH?(S0CR1=S0CR1&(~S0RCK)):\
        (__SELECT__^UART0_SELECT22)==OPTION_MATCH?(S0CR1=S0CR1|(S0RCK)):\
        (__SELECT__^UART0_SELECT23)==OPTION_MATCH?(S0CR1=S0CR1|(S0RCK)):_nop_());\
    ;)


/**
*******************************************************************************
* @brief       UART0 Transmit Clock
* @details     Set S0TCK
* @param[in]   __SELECT__ :
*  @arg\b       UART0_8BIT_CONFIG0 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG1 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG4 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG5 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG6 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG7 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG8 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG9 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG10 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG11 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG13 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG14 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG15 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG16 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG17 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG18 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG19 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG20 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG21 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG22 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG23 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
* @return      None
* @note        None
* @par         Example
* @code
___DRV_URT0_EasySetS0TCK(UART0_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT0_EasySetS0TCK(__SELECT__)\
    MWT(\
        ((__SELECT__^UART0_SELECT0)==OPTION_MATCH?(S0CR1=S0CR1|(S0TCK)):\
        (__SELECT__^UART0_SELECT1)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TCK)):\
        (__SELECT__^UART0_SELECT2)==OPTION_MATCH?(S0CR1=S0CR1|(S0TCK)):\
        (__SELECT__^UART0_SELECT3)==OPTION_MATCH?(S0CR1=S0CR1|(S0TCK)):\
        (__SELECT__^UART0_SELECT4)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TCK)):\
        (__SELECT__^UART0_SELECT5)==OPTION_MATCH?(S0CR1=S0CR1|(S0TCK)):\
        (__SELECT__^UART0_SELECT6)==OPTION_MATCH?(S0CR1=S0CR1|(S0TCK)):\
        (__SELECT__^UART0_SELECT7)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TCK)):\
        (__SELECT__^UART0_SELECT8)==OPTION_MATCH?(S0CR1=S0CR1|(S0TCK)):\
        (__SELECT__^UART0_SELECT9)==OPTION_MATCH?(S0CR1=S0CR1|(S0TCK)):\
        (__SELECT__^UART0_SELECT10)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TCK)):\
        (__SELECT__^UART0_SELECT11)==OPTION_MATCH?(S0CR1=S0CR1|(S0TCK)):\
        (__SELECT__^UART0_SELECT12)==OPTION_MATCH?(S0CR1=S0CR1|(S0TCK)):\
        (__SELECT__^UART0_SELECT13)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TCK)):\
        (__SELECT__^UART0_SELECT14)==OPTION_MATCH?(S0CR1=S0CR1|(S0TCK)):\
        (__SELECT__^UART0_SELECT15)==OPTION_MATCH?(S0CR1=S0CR1|(S0TCK)):\
        (__SELECT__^UART0_SELECT16)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TCK)):\
        (__SELECT__^UART0_SELECT17)==OPTION_MATCH?(S0CR1=S0CR1|(S0TCK)):\
        (__SELECT__^UART0_SELECT18)==OPTION_MATCH?(S0CR1=S0CR1|(S0TCK)):\
        (__SELECT__^UART0_SELECT19)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TCK)):\
        (__SELECT__^UART0_SELECT20)==OPTION_MATCH?(S0CR1=S0CR1|(S0TCK)):\
        (__SELECT__^UART0_SELECT21)==OPTION_MATCH?(S0CR1=S0CR1|(S0TCK)):\
        (__SELECT__^UART0_SELECT22)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TCK)):\
        (__SELECT__^UART0_SELECT23)==OPTION_MATCH?(S0CR1=S0CR1|(S0TCK)):_nop_());\
    ;)


/**
*******************************************************************************
* @brief       UART0 Data Order
* @details     Set S0DOR
* @param[in]   __SELECT__ :
*  @arg\b       UART0_8BIT_CONFIG0 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG1 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG4 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG5 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG6 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG7 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG8 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG9 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG10 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG11 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG13 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG14 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG15 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG16 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG17 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG18 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG19 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG20 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG21 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG22 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG23 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_EasySetS0DOR(UART0_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT0_EasySetS0DOR(__SELECT__)\
    MWT(\
        ((__SELECT__^UART0_SELECT0)==OPTION_MATCH?(S0CFG=S0CFG|(S0DOR)):\
        (__SELECT__^UART0_SELECT1)==OPTION_MATCH?(S0CFG=S0CFG|(S0DOR)):\
        (__SELECT__^UART0_SELECT2)==OPTION_MATCH?(S0CFG=S0CFG|(S0DOR)):\
        (__SELECT__^UART0_SELECT3)==OPTION_MATCH?(S0CFG=S0CFG&(~S0DOR)):\
        (__SELECT__^UART0_SELECT4)==OPTION_MATCH?(S0CFG=S0CFG&(~S0DOR)):\
        (__SELECT__^UART0_SELECT5)==OPTION_MATCH?(S0CFG=S0CFG&(~S0DOR)):\
        (__SELECT__^UART0_SELECT6)==OPTION_MATCH?(S0CFG=S0CFG|(S0DOR)):\
        (__SELECT__^UART0_SELECT7)==OPTION_MATCH?(S0CFG=S0CFG|(S0DOR)):\
        (__SELECT__^UART0_SELECT8)==OPTION_MATCH?(S0CFG=S0CFG|(S0DOR)):\
        (__SELECT__^UART0_SELECT9)==OPTION_MATCH?(S0CFG=S0CFG&(~S0DOR)):\
        (__SELECT__^UART0_SELECT10)==OPTION_MATCH?(S0CFG=S0CFG&(~S0DOR)):\
        (__SELECT__^UART0_SELECT11)==OPTION_MATCH?(S0CFG=S0CFG&(~S0DOR)):\
        (__SELECT__^UART0_SELECT12)==OPTION_MATCH?(S0CFG=S0CFG|(S0DOR)):\
        (__SELECT__^UART0_SELECT13)==OPTION_MATCH?(S0CFG=S0CFG|(S0DOR)):\
        (__SELECT__^UART0_SELECT14)==OPTION_MATCH?(S0CFG=S0CFG|(S0DOR)):\
        (__SELECT__^UART0_SELECT15)==OPTION_MATCH?(S0CFG=S0CFG&(~S0DOR)):\
        (__SELECT__^UART0_SELECT16)==OPTION_MATCH?(S0CFG=S0CFG&(~S0DOR)):\
        (__SELECT__^UART0_SELECT17)==OPTION_MATCH?(S0CFG=S0CFG&(~S0DOR)):\
        (__SELECT__^UART0_SELECT18)==OPTION_MATCH?(S0CFG=S0CFG|(S0DOR)):\
        (__SELECT__^UART0_SELECT19)==OPTION_MATCH?(S0CFG=S0CFG|(S0DOR)):\
        (__SELECT__^UART0_SELECT20)==OPTION_MATCH?(S0CFG=S0CFG|(S0DOR)):\
        (__SELECT__^UART0_SELECT21)==OPTION_MATCH?(S0CFG=S0CFG&(~S0DOR)):\
        (__SELECT__^UART0_SELECT22)==OPTION_MATCH?(S0CFG=S0CFG&(~S0DOR)):\
        (__SELECT__^UART0_SELECT23)==OPTION_MATCH?(S0CFG=S0CFG&(~S0DOR)):_nop_());\
    ;)


/**
*******************************************************************************
* @brief       UART0 Enhance Baud Rate
* @details     Set SMOD1
* @param[in]    __SELECT__ :
*  @arg\b       UART0_8BIT_CONFIG0 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG1 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG4 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG5 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG6 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG7 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG8 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG9 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG10 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG11 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG13 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG14 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG15 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG16 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG17 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG18 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG19 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG20 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG21 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG22 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG23 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_EasySetSMOD1(UART0_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT0_EasySetSMOD1(__SELECT__)\
    MWT(\
        ((__SELECT__^UART0_SELECT0)==OPTION_MATCH?(PCON0=PCON0&(~SMOD1)):\
        (__SELECT__^UART0_SELECT1)==OPTION_MATCH?(PCON0=PCON0&(~SMOD1)):\
        (__SELECT__^UART0_SELECT2)==OPTION_MATCH?(PCON0=PCON0&(~SMOD1)):\
        (__SELECT__^UART0_SELECT3)==OPTION_MATCH?(PCON0=PCON0&(~SMOD1)):\
        (__SELECT__^UART0_SELECT4)==OPTION_MATCH?(PCON0=PCON0&(~SMOD1)):\
        (__SELECT__^UART0_SELECT5)==OPTION_MATCH?(PCON0=PCON0&(~SMOD1)):\
        (__SELECT__^UART0_SELECT6)==OPTION_MATCH?(PCON0=PCON0&(~SMOD1)):\
        (__SELECT__^UART0_SELECT7)==OPTION_MATCH?(PCON0=PCON0&(~SMOD1)):\
        (__SELECT__^UART0_SELECT8)==OPTION_MATCH?(PCON0=PCON0&(~SMOD1)):\
        (__SELECT__^UART0_SELECT9)==OPTION_MATCH?(PCON0=PCON0&(~SMOD1)):\
        (__SELECT__^UART0_SELECT10)==OPTION_MATCH?(PCON0=PCON0&(~SMOD1)):\
        (__SELECT__^UART0_SELECT11)==OPTION_MATCH?(PCON0=PCON0&(~SMOD1)):\
        (__SELECT__^UART0_SELECT12)==OPTION_MATCH?(PCON0=PCON0&(~SMOD1)):\
        (__SELECT__^UART0_SELECT13)==OPTION_MATCH?(PCON0=PCON0&(~SMOD1)):\
        (__SELECT__^UART0_SELECT14)==OPTION_MATCH?(PCON0=PCON0&(~SMOD1)):\
        (__SELECT__^UART0_SELECT15)==OPTION_MATCH?(PCON0=PCON0&(~SMOD1)):\
        (__SELECT__^UART0_SELECT16)==OPTION_MATCH?(PCON0=PCON0&(~SMOD1)):\
        (__SELECT__^UART0_SELECT17)==OPTION_MATCH?(PCON0=PCON0&(~SMOD1)):\
        (__SELECT__^UART0_SELECT18)==OPTION_MATCH?(PCON0=PCON0&(~SMOD1)):\
        (__SELECT__^UART0_SELECT19)==OPTION_MATCH?(PCON0=PCON0&(~SMOD1)):\
        (__SELECT__^UART0_SELECT20)==OPTION_MATCH?(PCON0=PCON0&(~SMOD1)):\
        (__SELECT__^UART0_SELECT21)==OPTION_MATCH?(PCON0=PCON0&(~SMOD1)):\
        (__SELECT__^UART0_SELECT22)==OPTION_MATCH?(PCON0=PCON0&(~SMOD1)):\
        (__SELECT__^UART0_SELECT23)==OPTION_MATCH?(PCON0=PCON0&(~SMOD1)):_nop_());\
    ;)


/**
*******************************************************************************
* @brief       UART0 Enhance Baud Rate.
* @details     Set SMOD2
* @param[in]   __SELECT__ :
*  @arg\b       UART0_8BIT_CONFIG0 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG1 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG4 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG5 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG6 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG7 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG8 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG9 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG10 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG11 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG13 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG14 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG15 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG16 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG17 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG18 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG19 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG20 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG21 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG22 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG23 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_EasySetSMOD2(UART0_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT0_EasySetSMOD2(__SELECT__)\
    MWT(\
        ((__SELECT__^UART0_SELECT0)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD2)):\
        (__SELECT__^UART0_SELECT1)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD2)):\
        (__SELECT__^UART0_SELECT2)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD2)):\
        (__SELECT__^UART0_SELECT3)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD2)):\
        (__SELECT__^UART0_SELECT4)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD2)):\
        (__SELECT__^UART0_SELECT5)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD2)):\
        (__SELECT__^UART0_SELECT6)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD2)):\
        (__SELECT__^UART0_SELECT7)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD2)):\
        (__SELECT__^UART0_SELECT8)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD2)):\
        (__SELECT__^UART0_SELECT9)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD2)):\
        (__SELECT__^UART0_SELECT10)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD2)):\
        (__SELECT__^UART0_SELECT11)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD2)):\
        (__SELECT__^UART0_SELECT12)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD2)):\
        (__SELECT__^UART0_SELECT13)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD2)):\
        (__SELECT__^UART0_SELECT14)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD2)):\
        (__SELECT__^UART0_SELECT15)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD2)):\
        (__SELECT__^UART0_SELECT16)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD2)):\
        (__SELECT__^UART0_SELECT17)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD2)):\
        (__SELECT__^UART0_SELECT18)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD2)):\
        (__SELECT__^UART0_SELECT19)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD2)):\
        (__SELECT__^UART0_SELECT20)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD2)):\
        (__SELECT__^UART0_SELECT21)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD2)):\
        (__SELECT__^UART0_SELECT22)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD2)):\
        (__SELECT__^UART0_SELECT23)==OPTION_MATCH?(S0CFG=S0CFG|(SMOD2)):_nop_());\
    ;)


/**
*******************************************************************************
* @brief      UART0  Baud Rate Generator Reload Register
* @details    Write S0BRT
* @param[in]    __SELECT__ :
*  @arg\b       UART0_8BIT_CONFIG0 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG1 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG4 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG5 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG6 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG7 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG8 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG9 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG10 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG11 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG13 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG14 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG15 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG16 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG17 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG18 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG19 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG20 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG21 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG22 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG23 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
* @return      None
* @note        Before write S0BRT and S0BRC,S0BRG operation stop first
* @par         Example
* @code
__DRV_URT0_EasySetS0BRT(UART0_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/

///@cond __DRV_URT0_EasySetS0BRT
//                                                              //error
#define S0BRG_BRGRL_2400_1X_12000000_12T            0xF3        // 0.16%
#define S0BRG_BRGRL_4800_1X_11059200_12T            0xFA        // 0%
#define S0BRG_BRGRL_9600_1X_11059200_12T            0xFD        // 0%
#define S0BRG_BRGRL_2400_1X_12000000_1T             0x64        // 0.16%
#define S0BRG_BRGRL_4800_1X_12000000_1T             0xB2        // 0.16%
#define S0BRG_BRGRL_9600_1X_12000000_1T             0xD9        // 0.16%
#define S0BRG_BRGRL_19200_1X_12000000_1T            0xEC        // -2.34%
#define S0BRG_BRGRL_38400_1X_12000000_1T            0xF6        // -2.34%
#define S0BRG_BRGRL_57600_1X_11059200_1T            0xFA        // 0%
#define S0BRG_BRGRL_115200_1X_11059200_1T           0xFD        // 0%
#define S0BRG_BRGRL_2400_2X_12000000_12T            0xE6        // 0.16%
#define S0BRG_BRGRL_4800_2X_12000000_12T            0xF3        // 0.16%
#define S0BRG_BRGRL_9600_2X_11059200_12T            0xFA        // 0%
#define S0BRG_BRGRL_19200_2X_11059200_12T           0xFD        // 0%
#define S0BRG_BRGRL_57600_2X_11059200_12T           0xFF        // 0%
#define S0BRG_BRGRL_4800_2X_12000000_1T             0x64        // 0.16%
#define S0BRG_BRGRL_9600_2X_12000000_1T             0xB2        // 0.16%
#define S0BRG_BRGRL_19200_2X_12000000_1T            0xD9        // 0.16%
#define S0BRG_BRGRL_38400_2X_12000000_1T            0xEC        // -2.34%
#define S0BRG_BRGRL_57600_2X_12000000_1T            0xF3        // 0.16%
#define S0BRG_BRGRL_115200_2X_11059200_1T           0xFA        // 0%
#define S0BRG_BRGRL_230400_2X_11059200_1T           0xFD        // 0%
#define S0BRG_BRGRL_250000_2X_12000000_1T           0xFD        // 0%
#define S0BRG_BRGRL_750000_2X_12000000_1T           0xFF        // 0%
///@endcond

#define __DRV_URT0_EasySetS0BRT(__SELECT__)\
    MWT(\
        ((__SELECT__^UART0_SELECT0)==OPTION_MATCH?(S0BRT=S0BRG_BRGRL_9600_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT1)==OPTION_MATCH?(S0BRT=S0BRG_BRGRL_9600_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT2)==OPTION_MATCH?(S0BRT=S0BRG_BRGRL_9600_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT3)==OPTION_MATCH?(S0BRT=S0BRG_BRGRL_9600_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT4)==OPTION_MATCH?(S0BRT=S0BRG_BRGRL_9600_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT5)==OPTION_MATCH?(S0BRT=S0BRG_BRGRL_9600_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT6)==OPTION_MATCH?(S0BRT=S0BRG_BRGRL_19200_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT7)==OPTION_MATCH?(S0BRT=S0BRG_BRGRL_19200_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT8)==OPTION_MATCH?(S0BRT=S0BRG_BRGRL_19200_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT9)==OPTION_MATCH?(S0BRT=S0BRG_BRGRL_19200_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT10)==OPTION_MATCH?(S0BRT=S0BRG_BRGRL_19200_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT11)==OPTION_MATCH?(S0BRT=S0BRG_BRGRL_19200_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT12)==OPTION_MATCH?(S0BRT=S0BRG_BRGRL_57600_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT13)==OPTION_MATCH?(S0BRT=S0BRG_BRGRL_57600_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT14)==OPTION_MATCH?(S0BRT=S0BRG_BRGRL_57600_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT15)==OPTION_MATCH?(S0BRT=S0BRG_BRGRL_57600_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT16)==OPTION_MATCH?(S0BRT=S0BRG_BRGRL_57600_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT17)==OPTION_MATCH?(S0BRT=S0BRG_BRGRL_57600_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT18)==OPTION_MATCH?(S0BRT=S0BRG_BRGRL_115200_2X_11059200_1T):\
        (__SELECT__^UART0_SELECT19)==OPTION_MATCH?(S0BRT=S0BRG_BRGRL_115200_2X_11059200_1T):\
        (__SELECT__^UART0_SELECT20)==OPTION_MATCH?(S0BRT=S0BRG_BRGRL_115200_2X_11059200_1T):\
        (__SELECT__^UART0_SELECT21)==OPTION_MATCH?(S0BRT=S0BRG_BRGRL_115200_2X_11059200_1T):\
        (__SELECT__^UART0_SELECT22)==OPTION_MATCH?(S0BRT=S0BRG_BRGRL_115200_2X_11059200_1T):\
        (__SELECT__^UART0_SELECT23)==OPTION_MATCH?(S0BRT=S0BRG_BRGRL_115200_2X_11059200_1T):_nop_());\
    ;)


/**
*******************************************************************************
* @brief      UART0  Baud Rate Generator Reload Register
* @details    Write S0BRC
* @param[in]  __SELECT__ :
*  @arg\b       UART0_8BIT_CONFIG0 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG1 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG4 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG5 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG6 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG7 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG8 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG9 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG10 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG11 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG13 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG14 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG15 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG16 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG17 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG18 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG19 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG20 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG21 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG22 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG23 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
* @return      None
* @note        Before write S0BRT and S0BRC,S0BRG operation stop first
* @par         Example
* @code
__DRV_URT0_EasySetS0BRC(UART0_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT0_EasySetS0BRC(__SELECT__)\
    MWT(\
        ((__SELECT__^UART0_SELECT0)==OPTION_MATCH?(S0BRC=S0BRG_BRGRL_9600_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT1)==OPTION_MATCH?(S0BRC=S0BRG_BRGRL_9600_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT2)==OPTION_MATCH?(S0BRC=S0BRG_BRGRL_9600_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT3)==OPTION_MATCH?(S0BRC=S0BRG_BRGRL_9600_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT4)==OPTION_MATCH?(S0BRC=S0BRG_BRGRL_9600_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT5)==OPTION_MATCH?(S0BRC=S0BRG_BRGRL_9600_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT6)==OPTION_MATCH?(S0BRC=S0BRG_BRGRL_19200_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT7)==OPTION_MATCH?(S0BRC=S0BRG_BRGRL_19200_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT8)==OPTION_MATCH?(S0BRC=S0BRG_BRGRL_19200_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT9)==OPTION_MATCH?(S0BRC=S0BRG_BRGRL_19200_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT10)==OPTION_MATCH?(S0BRC=S0BRG_BRGRL_19200_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT11)==OPTION_MATCH?(S0BRC=S0BRG_BRGRL_19200_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT12)==OPTION_MATCH?(S0BRC=S0BRG_BRGRL_57600_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT13)==OPTION_MATCH?(S0BRC=S0BRG_BRGRL_57600_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT14)==OPTION_MATCH?(S0BRC=S0BRG_BRGRL_57600_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT15)==OPTION_MATCH?(S0BRC=S0BRG_BRGRL_57600_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT16)==OPTION_MATCH?(S0BRC=S0BRG_BRGRL_57600_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT17)==OPTION_MATCH?(S0BRC=S0BRG_BRGRL_57600_2X_12000000_1T):\
        (__SELECT__^UART0_SELECT18)==OPTION_MATCH?(S0BRC=S0BRG_BRGRL_115200_2X_11059200_1T):\
        (__SELECT__^UART0_SELECT19)==OPTION_MATCH?(S0BRC=S0BRG_BRGRL_115200_2X_11059200_1T):\
        (__SELECT__^UART0_SELECT20)==OPTION_MATCH?(S0BRC=S0BRG_BRGRL_115200_2X_11059200_1T):\
        (__SELECT__^UART0_SELECT21)==OPTION_MATCH?(S0BRC=S0BRG_BRGRL_115200_2X_11059200_1T):\
        (__SELECT__^UART0_SELECT22)==OPTION_MATCH?(S0BRC=S0BRG_BRGRL_115200_2X_11059200_1T):\
        (__SELECT__^UART0_SELECT23)==OPTION_MATCH?(S0BRC=S0BRG_BRGRL_115200_2X_11059200_1T):_nop_());\
    ;)


/**
*******************************************************************************
* @brief       UART0 S0BRG Operation Enable
* @details     Set S0TR
* @param[in]   __SELECT__ :
*  @arg\b       UART0_8BIT_CONFIG0 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG1 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG4 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG5 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG6 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG7 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG8 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG9 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG10 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG11 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG13 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG14 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG15 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG16 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG17 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG18 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG19 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG20 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG21 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG22 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG23 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
* @return      None
* @note        Before write S0BRT and S0BRC,S0BRG operation stop first
* @par         Example
* @code
__DRV_URT0_EasySetS0TR_Start(UART0_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT0_EasySetS0TR_Start(__SELECT__)\
    MWT(\
        ((__SELECT__^UART0_SELECT0)==OPTION_MATCH?(S0CR1=S0CR1|(S0TR)):\
        (__SELECT__^UART0_SELECT1)==OPTION_MATCH?(S0CR1=S0CR1|(S0TR)):\
        (__SELECT__^UART0_SELECT2)==OPTION_MATCH?(S0CR1=S0CR1|(S0TR)):\
        (__SELECT__^UART0_SELECT3)==OPTION_MATCH?(S0CR1=S0CR1|(S0TR)):\
        (__SELECT__^UART0_SELECT4)==OPTION_MATCH?(S0CR1=S0CR1|(S0TR)):\
        (__SELECT__^UART0_SELECT5)==OPTION_MATCH?(S0CR1=S0CR1|(S0TR)):\
        (__SELECT__^UART0_SELECT6)==OPTION_MATCH?(S0CR1=S0CR1|(S0TR)):\
        (__SELECT__^UART0_SELECT7)==OPTION_MATCH?(S0CR1=S0CR1|(S0TR)):\
        (__SELECT__^UART0_SELECT8)==OPTION_MATCH?(S0CR1=S0CR1|(S0TR)):\
        (__SELECT__^UART0_SELECT9)==OPTION_MATCH?(S0CR1=S0CR1|(S0TR)):\
        (__SELECT__^UART0_SELECT10)==OPTION_MATCH?(S0CR1=S0CR1|(S0TR)):\
        (__SELECT__^UART0_SELECT11)==OPTION_MATCH?(S0CR1=S0CR1|(S0TR)):\
        (__SELECT__^UART0_SELECT12)==OPTION_MATCH?(S0CR1=S0CR1|(S0TR)):\
        (__SELECT__^UART0_SELECT13)==OPTION_MATCH?(S0CR1=S0CR1|(S0TR)):\
        (__SELECT__^UART0_SELECT14)==OPTION_MATCH?(S0CR1=S0CR1|(S0TR)):\
        (__SELECT__^UART0_SELECT15)==OPTION_MATCH?(S0CR1=S0CR1|(S0TR)):\
        (__SELECT__^UART0_SELECT16)==OPTION_MATCH?(S0CR1=S0CR1|(S0TR)):\
        (__SELECT__^UART0_SELECT17)==OPTION_MATCH?(S0CR1=S0CR1|(S0TR)):\
        (__SELECT__^UART0_SELECT18)==OPTION_MATCH?(S0CR1=S0CR1|(S0TR)):\
        (__SELECT__^UART0_SELECT19)==OPTION_MATCH?(S0CR1=S0CR1|(S0TR)):\
        (__SELECT__^UART0_SELECT20)==OPTION_MATCH?(S0CR1=S0CR1|(S0TR)):\
        (__SELECT__^UART0_SELECT21)==OPTION_MATCH?(S0CR1=S0CR1|(S0TR)):\
        (__SELECT__^UART0_SELECT22)==OPTION_MATCH?(S0CR1=S0CR1|(S0TR)):\
        (__SELECT__^UART0_SELECT23)==OPTION_MATCH?(S0CR1=S0CR1|(S0TR)):_nop_());\
    ;)


/**
*******************************************************************************
* @brief       UART0 S0BRG Operation Disable
* @details     Set S0TR
* @param[in]    __SELECT__ :
*  @arg\b       UART0_8BIT_CONFIG0 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG1 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG4 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG5 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG6 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG7 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG8 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG9 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG10 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG11 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG13 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG14 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG15 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG16 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG17 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG18 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG19 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG20 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG21 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG22 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG23 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
* @return      None
* @note        Before write S0BRT and S0BRC,S0BRG operation stop first
* @par         Example
* @code
__DRV_URT0_EasySetS0TR_Stop(UART0_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT0_EasySetS0TR_Stop(__SELECT__)\
    MWT(\
        ((__SELECT__^UART0_SELECT0)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TR)):\
        (__SELECT__^UART0_SELECT1)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TR)):\
        (__SELECT__^UART0_SELECT2)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TR)):\
        (__SELECT__^UART0_SELECT3)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TR)):\
        (__SELECT__^UART0_SELECT4)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TR)):\
        (__SELECT__^UART0_SELECT5)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TR)):\
        (__SELECT__^UART0_SELECT6)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TR)):\
        (__SELECT__^UART0_SELECT7)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TR)):\
        (__SELECT__^UART0_SELECT8)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TR)):\
        (__SELECT__^UART0_SELECT9)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TR)):\
        (__SELECT__^UART0_SELECT10)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TR)):\
        (__SELECT__^UART0_SELECT11)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TR)):\
        (__SELECT__^UART0_SELECT12)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TR)):\
        (__SELECT__^UART0_SELECT13)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TR)):\
        (__SELECT__^UART0_SELECT14)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TR)):\
        (__SELECT__^UART0_SELECT15)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TR)):\
        (__SELECT__^UART0_SELECT16)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TR)):\
        (__SELECT__^UART0_SELECT17)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TR)):\
        (__SELECT__^UART0_SELECT18)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TR)):\
        (__SELECT__^UART0_SELECT19)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TR)):\
        (__SELECT__^UART0_SELECT20)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TR)):\
        (__SELECT__^UART0_SELECT21)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TR)):\
        (__SELECT__^UART0_SELECT22)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TR)):\
        (__SELECT__^UART0_SELECT23)==OPTION_MATCH?(S0CR1=S0CR1&(~S0TR)):_nop_());\
    ;)


/**
*******************************************************************************
* @brief       UART0 Pin Config
* @details     Set S0PS0
* @param[in]    __SELECT__ :
*  @arg\b       UART0_8BIT_CONFIG0 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG1 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG4 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG5 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG6 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG7 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG8 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG9 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG10 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG11 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG13 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG14 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG15 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG16 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG17 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG18 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG19 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG20 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG21 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG22 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG23 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_EasySetS0PS0(UART0_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT0_EasySetS0PS0(__SELECT__)\
    MWT(\
        ((__SELECT__^UART0_SELECT0)==OPTION_MATCH?(AUXR3=AUXR3&(~S0PS0)):\
        (__SELECT__^UART0_SELECT1)==OPTION_MATCH?(AUXR3=AUXR3&(~S0PS0)):\
        (__SELECT__^UART0_SELECT2)==OPTION_MATCH?(AUXR3=AUXR3&(~S0PS0)):\
        (__SELECT__^UART0_SELECT3)==OPTION_MATCH?(AUXR3=AUXR3&(~S0PS0)):\
        (__SELECT__^UART0_SELECT4)==OPTION_MATCH?(AUXR3=AUXR3&(~S0PS0)):\
        (__SELECT__^UART0_SELECT5)==OPTION_MATCH?(AUXR3=AUXR3&(~S0PS0)):\
        (__SELECT__^UART0_SELECT6)==OPTION_MATCH?(AUXR3=AUXR3&(~S0PS0)):\
        (__SELECT__^UART0_SELECT7)==OPTION_MATCH?(AUXR3=AUXR3&(~S0PS0)):\
        (__SELECT__^UART0_SELECT8)==OPTION_MATCH?(AUXR3=AUXR3&(~S0PS0)):\
        (__SELECT__^UART0_SELECT9)==OPTION_MATCH?(AUXR3=AUXR3&(~S0PS0)):\
        (__SELECT__^UART0_SELECT10)==OPTION_MATCH?(AUXR3=AUXR3&(~S0PS0)):\
        (__SELECT__^UART0_SELECT11)==OPTION_MATCH?(AUXR3=AUXR3&(~S0PS0)):\
        (__SELECT__^UART0_SELECT12)==OPTION_MATCH?(AUXR3=AUXR3&(~S0PS0)):\
        (__SELECT__^UART0_SELECT13)==OPTION_MATCH?(AUXR3=AUXR3&(~S0PS0)):\
        (__SELECT__^UART0_SELECT14)==OPTION_MATCH?(AUXR3=AUXR3&(~S0PS0)):\
        (__SELECT__^UART0_SELECT15)==OPTION_MATCH?(AUXR3=AUXR3&(~S0PS0)):\
        (__SELECT__^UART0_SELECT16)==OPTION_MATCH?(AUXR3=AUXR3&(~S0PS0)):\
        (__SELECT__^UART0_SELECT17)==OPTION_MATCH?(AUXR3=AUXR3&(~S0PS0)):\
        (__SELECT__^UART0_SELECT18)==OPTION_MATCH?(AUXR3=AUXR3&(~S0PS0)):\
        (__SELECT__^UART0_SELECT19)==OPTION_MATCH?(AUXR3=AUXR3&(~S0PS0)):\
        (__SELECT__^UART0_SELECT20)==OPTION_MATCH?(AUXR3=AUXR3&(~S0PS0)):\
        (__SELECT__^UART0_SELECT21)==OPTION_MATCH?(AUXR3=AUXR3&(~S0PS0)):\
        (__SELECT__^UART0_SELECT22)==OPTION_MATCH?(AUXR3=AUXR3&(~S0PS0)):\
        (__SELECT__^UART0_SELECT23)==OPTION_MATCH?(AUXR3=AUXR3&(~S0PS0)):_nop_());\
    ;)


/**
*******************************************************************************
* @brief       UART0 Pin Config
* @details     Set S0PS1
* @param[in]   __SELECT__ :
*  @arg\b       UART0_8BIT_CONFIG0 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG1 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG4 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG5 : 9600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG6 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG7 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG8 : 19200 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG9 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG10 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG11 : 19200 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG13 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG14 : 57600 12MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG15 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG16 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG17 : 57600 12MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG18 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG19 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG20 : 115200 11.0592MHz 8bit LSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG21 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG22 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Rx only SYSCLK/1 double baud rate S0BRG
*  @arg\b       UART0_8BIT_CONFIG23 : 115200 11.0592MHz 8bit MSB Rx p30 Tx p31 Tx/Rx both SYSCLK/1 double baud rate S0BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_EasySetS0PS1(UART0_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/

#define __DRV_URT0_EasySetS0PS1(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR10_Page);\
        ((__SELECT__^UART0_SELECT0)==OPTION_MATCH?(AUXR10=AUXR10&(~S0PS1)):\
        (__SELECT__^UART0_SELECT1)==OPTION_MATCH?(AUXR10=AUXR10&(~S0PS1)):\
        (__SELECT__^UART0_SELECT2)==OPTION_MATCH?(AUXR10=AUXR10&(~S0PS1)):\
        (__SELECT__^UART0_SELECT3)==OPTION_MATCH?(AUXR10=AUXR10&(~S0PS1)):\
        (__SELECT__^UART0_SELECT4)==OPTION_MATCH?(AUXR10=AUXR10&(~S0PS1)):\
        (__SELECT__^UART0_SELECT5)==OPTION_MATCH?(AUXR10=AUXR10&(~S0PS1)):\
        (__SELECT__^UART0_SELECT6)==OPTION_MATCH?(AUXR10=AUXR10&(~S0PS1)):\
        (__SELECT__^UART0_SELECT7)==OPTION_MATCH?(AUXR10=AUXR10&(~S0PS1)):\
        (__SELECT__^UART0_SELECT8)==OPTION_MATCH?(AUXR10=AUXR10&(~S0PS1)):\
        (__SELECT__^UART0_SELECT9)==OPTION_MATCH?(AUXR10=AUXR10&(~S0PS1)):\
        (__SELECT__^UART0_SELECT10)==OPTION_MATCH?(AUXR10=AUXR10&(~S0PS1)):\
        (__SELECT__^UART0_SELECT11)==OPTION_MATCH?(AUXR10=AUXR10&(~S0PS1)):\
        (__SELECT__^UART0_SELECT12)==OPTION_MATCH?(AUXR10=AUXR10&(~S0PS1)):\
        (__SELECT__^UART0_SELECT13)==OPTION_MATCH?(AUXR10=AUXR10&(~S0PS1)):\
        (__SELECT__^UART0_SELECT14)==OPTION_MATCH?(AUXR10=AUXR10&(~S0PS1)):\
        (__SELECT__^UART0_SELECT15)==OPTION_MATCH?(AUXR10=AUXR10&(~S0PS1)):\
        (__SELECT__^UART0_SELECT16)==OPTION_MATCH?(AUXR10=AUXR10&(~S0PS1)):\
        (__SELECT__^UART0_SELECT17)==OPTION_MATCH?(AUXR10=AUXR10&(~S0PS1)):\
        (__SELECT__^UART0_SELECT18)==OPTION_MATCH?(AUXR10=AUXR10&(~S0PS1)):\
        (__SELECT__^UART0_SELECT19)==OPTION_MATCH?(AUXR10=AUXR10&(~S0PS1)):\
        (__SELECT__^UART0_SELECT20)==OPTION_MATCH?(AUXR10=AUXR10&(~S0PS1)):\
        (__SELECT__^UART0_SELECT21)==OPTION_MATCH?(AUXR10=AUXR10&(~S0PS1)):\
        (__SELECT__^UART0_SELECT22)==OPTION_MATCH?(AUXR10=AUXR10&(~S0PS1)):\
        (__SELECT__^UART0_SELECT23)==OPTION_MATCH?(AUXR10=AUXR10&(~S0PS1)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)









#endif  //MG82F6D64_UART0_MID_H



