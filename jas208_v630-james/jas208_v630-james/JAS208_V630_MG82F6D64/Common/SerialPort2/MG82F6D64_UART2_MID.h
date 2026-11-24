/**
 ******************************************************************************
 *
 * @file        MG82F6D64_UART2_MID.h
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

#ifndef MG82F6D64_UART2_MID_H
#define MG82F6D64_UART2_MID_H




///@cond __DRV_URT2_Easy_Wizard_Init
#define UART2_8BIT_CONFIG0 (UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_4800|UART2_PIN_CONFIG_RX_P32_TX_P33)
#define UART2_8BIT_CONFIG1 (UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_4800|UART2_PIN_CONFIG_RX_P32_TX_P33)
#define UART2_8BIT_CONFIG2 (UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_9600|UART2_PIN_CONFIG_RX_P32_TX_P33)
#define UART2_8BIT_CONFIG3 (UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_9600|UART2_PIN_CONFIG_RX_P32_TX_P33)
#define UART2_8BIT_CONFIG4 (UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_19200|UART2_PIN_CONFIG_RX_P32_TX_P33)
#define UART2_8BIT_CONFIG5 (UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_19200|UART2_PIN_CONFIG_RX_P32_TX_P33)
#define UART2_8BIT_CONFIG6 (UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_1X_LSB_TX_RX|UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_38400|UART2_PIN_CONFIG_RX_P32_TX_P33)
#define UART2_8BIT_CONFIG7 (UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_1X_MSB_TX_RX|UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_38400|UART2_PIN_CONFIG_RX_P32_TX_P33)
#define UART2_8BIT_CONFIG8 (UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_38400|UART2_PIN_CONFIG_RX_P32_TX_P33)
#define UART2_8BIT_CONFIG9 (UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_38400|UART2_PIN_CONFIG_RX_P32_TX_P33)
#define UART2_8BIT_CONFIG10 (UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_1X_LSB_TX_RX|UART2_8BIT_S2BRG_IHRCO_110592MHZ_BAUD_RATE_57600|UART2_PIN_CONFIG_RX_P32_TX_P33)
#define UART2_8BIT_CONFIG11 (UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_1X_MSB_TX_RX|UART2_8BIT_S2BRG_IHRCO_110592MHZ_BAUD_RATE_57600|UART2_PIN_CONFIG_RX_P32_TX_P33)
#define UART2_8BIT_CONFIG12 (UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_57600|UART2_PIN_CONFIG_RX_P32_TX_P33)
#define UART2_8BIT_CONFIG13 (UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_57600|UART2_PIN_CONFIG_RX_P32_TX_P33)
#define UART2_8BIT_CONFIG14 (UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_1X_LSB_TX_RX|UART2_8BIT_S2BRG_IHRCO_110592MHZ_BAUD_RATE_115200|UART2_PIN_CONFIG_RX_P32_TX_P33)
#define UART2_8BIT_CONFIG15 (UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_1X_MSB_TX_RX|UART2_8BIT_S2BRG_IHRCO_110592MHZ_BAUD_RATE_115200|UART2_PIN_CONFIG_RX_P32_TX_P33)
#define UART2_8BIT_CONFIG16 (UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART2_8BIT_S2BRG_IHRCO_110592MHZ_BAUD_RATE_115200|UART2_PIN_CONFIG_RX_P32_TX_P33)
#define UART2_8BIT_CONFIG17 (UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART2_8BIT_S2BRG_IHRCO_110592MHZ_BAUD_RATE_115200|UART2_PIN_CONFIG_RX_P32_TX_P33)
#define UART2_8BIT_CONFIG18 (UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_230400|UART2_PIN_CONFIG_RX_P32_TX_P33)
#define UART2_8BIT_CONFIG19 (UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_230400|UART2_PIN_CONFIG_RX_P32_TX_P33)
#define UART2_8BIT_CONFIG20 (UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_250000|UART2_PIN_CONFIG_RX_P32_TX_P33)
#define UART2_8BIT_CONFIG21 (UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_250000|UART2_PIN_CONFIG_RX_P32_TX_P33)
#define UART2_8BIT_CONFIG22 (UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_750000|UART2_PIN_CONFIG_RX_P32_TX_P33)
#define UART2_8BIT_CONFIG23 (UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_750000|UART2_PIN_CONFIG_RX_P32_TX_P33)
//constant URT2 mode (0~255) option
#define UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX    (0x00000000)
#define UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX    (0x01000000)
#define UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_1X_LSB_TX_RX    (0x02000000)
#define UART2_8BIT_S2BRG_SYSCLK_DIV_1_BAUD_RATE_1X_MSB_TX_RX    (0x03000000)
//constant URT2 baud rate(0~255) option
#define UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_4800    (0x00000000)
#define UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_9600    (0x00010000)
#define UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_19200    (0x00020000)
#define UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_38400    (0x00030000)
#define UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_57600    (0x00040000)
#define UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_115200    (0x00050000)
#define UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_230400    (0x00060000)
#define UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_250000    (0x00070000)
#define UART2_8BIT_S2BRG_IHRCO_12MHZ_BAUD_RATE_750000    (0x00080000)
#define UART2_8BIT_S2BRG_IHRCO_110592MHZ_BAUD_RATE_57600    (0x00090000)
#define UART2_8BIT_S2BRG_IHRCO_110592MHZ_BAUD_RATE_115200    (0x000a0000)
//constant URT2 pin config (0~15) option
#define UART2_PIN_CONFIG_RX_P32_TX_P33    (0x00000000)
#define UART2_PIN_CONFIG_RX_P67_TX_P57    (0x00000100)


///@endcond




/**
*******************************************************************************
* @brief       UART2 Easy Wizard
* @details     Set SM32 SM02 SM12 S2DOR S2MOD1
* @param[in]    __SELECT__ :
*  @arg\b       UART2_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p32 Tx p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_Mode_Easy_Select(URT2_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/

#define __DRV_URT2_Mode_Easy_Select(__SELECT__)\
    MWT(\
        __DRV_URT2_EasySetSM32(__SELECT__);\
        __DRV_URT2_EasySetSM02(__SELECT__);\
        __DRV_URT2_EasySetSM12(__SELECT__);\
        __DRV_URT2_EasySetS2DOR(__SELECT__);\
        __DRV_URT2_EasySetS2MOD1(__SELECT__);\
    ;)

/**
*******************************************************************************
* @brief       UART2 Easy Wizard
* @details     Set S2TR S2BRT S2BRC
* @param[in]    __SELECT__ :
*  @arg\b       UART2_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p32 Tx p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_BaudRate_Easy_Select(URT2_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT2_BaudRate_Easy_Select(__SELECT__)\
    MWT(\
        __DRV_URT2_EasySetS2TR_Stop(__SELECT__);\
        __DRV_URT2_EasySetS2TX12(__SELECT__);\
        __DRV_URT2_EasySetS2BRT(__SELECT__);\
        __DRV_URT2_EasySetS2BRC(__SELECT__);\
        __DRV_URT2_EasySetS2TR_Start(__SELECT__);\
     ;)

/**
*******************************************************************************
* @brief       UART2 PinMux Easy Wizard
* @details     Set S2PS0
* @param[in]    __SELECT__ :
*  @arg\b       UART2_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p32 Tx p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_PinMux_Easy_Select(URT2_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT2_PinMux_Easy_Select(__SELECT__)\
    MWT(\
        __DRV_URT2_EasySetS2PS0(__SELECT__);\
    ;)




/**
*******************************************************************************
* @brief       UART2 Serial Reception
* @details     Set REN2
* @param[in]   __SELECT__ :
*  @arg\b       UART2_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p32 Tx p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_EasyEnableSerialReception(UART2_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/

#define __DRV_URT2_EasyEnableSerialReception(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CON_Page);\
        ((__SELECT__^UART2_SELECT0)==OPTION_MATCH?(REN2=1):\
        (__SELECT__^UART2_SELECT1)==OPTION_MATCH?(REN2=1):\
        (__SELECT__^UART2_SELECT2)==OPTION_MATCH?(REN2=1):\
        (__SELECT__^UART2_SELECT3)==OPTION_MATCH?(REN2=1):\
        (__SELECT__^UART2_SELECT4)==OPTION_MATCH?(REN2=1):\
        (__SELECT__^UART2_SELECT5)==OPTION_MATCH?(REN2=1):\
        (__SELECT__^UART2_SELECT6)==OPTION_MATCH?(REN2=1):\
        (__SELECT__^UART2_SELECT7)==OPTION_MATCH?(REN2=1):\
        (__SELECT__^UART2_SELECT8)==OPTION_MATCH?(REN2=1):\
        (__SELECT__^UART2_SELECT9)==OPTION_MATCH?(REN2=1):\
        (__SELECT__^UART2_SELECT10)==OPTION_MATCH?(REN2=1):\
        (__SELECT__^UART2_SELECT11)==OPTION_MATCH?(REN2=1):\
        (__SELECT__^UART2_SELECT12)==OPTION_MATCH?(REN2=1):\
        (__SELECT__^UART2_SELECT13)==OPTION_MATCH?(REN2=1):\
        (__SELECT__^UART2_SELECT14)==OPTION_MATCH?(REN2=1):\
        (__SELECT__^UART2_SELECT15)==OPTION_MATCH?(REN2=1):\
        (__SELECT__^UART2_SELECT16)==OPTION_MATCH?(REN2=1):\
        (__SELECT__^UART2_SELECT17)==OPTION_MATCH?(REN2=1):\
        (__SELECT__^UART2_SELECT18)==OPTION_MATCH?(REN2=1):\
        (__SELECT__^UART2_SELECT19)==OPTION_MATCH?(REN2=1):\
        (__SELECT__^UART2_SELECT20)==OPTION_MATCH?(REN2=1):\
        (__SELECT__^UART2_SELECT21)==OPTION_MATCH?(REN2=1):\
        (__SELECT__^UART2_SELECT22)==OPTION_MATCH?(REN2=1):\
        (__SELECT__^UART2_SELECT23)==OPTION_MATCH?(REN2=1):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)


/**
*******************************************************************************
* @brief       UART2 Serial Reception
* @details     Clear REN2
* @param[in]    __SELECT__ :
*  @arg\b       UART2_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p32 Tx p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_EasyDisableSerialReception(UART2_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT2_EasyDisableSerialReception(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CON_Page);\
        ((__SELECT__^UART2_SELECT0)==OPTION_MATCH?(REN2=0):\
        (__SELECT__^UART2_SELECT1)==OPTION_MATCH?(REN2=0):\
        (__SELECT__^UART2_SELECT2)==OPTION_MATCH?(REN2=0):\
        (__SELECT__^UART2_SELECT3)==OPTION_MATCH?(REN2=0):\
        (__SELECT__^UART2_SELECT4)==OPTION_MATCH?(REN2=0):\
        (__SELECT__^UART2_SELECT5)==OPTION_MATCH?(REN2=0):\
        (__SELECT__^UART2_SELECT6)==OPTION_MATCH?(REN2=0):\
        (__SELECT__^UART2_SELECT7)==OPTION_MATCH?(REN2=0):\
        (__SELECT__^UART2_SELECT8)==OPTION_MATCH?(REN2=0):\
        (__SELECT__^UART2_SELECT9)==OPTION_MATCH?(REN2=0):\
        (__SELECT__^UART2_SELECT10)==OPTION_MATCH?(REN2=0):\
        (__SELECT__^UART2_SELECT11)==OPTION_MATCH?(REN2=0):\
        (__SELECT__^UART2_SELECT12)==OPTION_MATCH?(REN2=0):\
        (__SELECT__^UART2_SELECT13)==OPTION_MATCH?(REN2=0):\
        (__SELECT__^UART2_SELECT14)==OPTION_MATCH?(REN2=0):\
        (__SELECT__^UART2_SELECT15)==OPTION_MATCH?(REN2=0):\
        (__SELECT__^UART2_SELECT16)==OPTION_MATCH?(REN2=0):\
        (__SELECT__^UART2_SELECT17)==OPTION_MATCH?(REN2=0):\
        (__SELECT__^UART2_SELECT18)==OPTION_MATCH?(REN2=0):\
        (__SELECT__^UART2_SELECT19)==OPTION_MATCH?(REN2=0):\
        (__SELECT__^UART2_SELECT20)==OPTION_MATCH?(REN2=0):\
        (__SELECT__^UART2_SELECT21)==OPTION_MATCH?(REN2=0):\
        (__SELECT__^UART2_SELECT22)==OPTION_MATCH?(REN2=0):\
        (__SELECT__^UART2_SELECT23)==OPTION_MATCH?(REN2=0):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)


/**
*******************************************************************************
* @brief       UART2 Mode Select
* @details     Set SM32
* @param[in]    __SELECT__ :
*  @arg\b       UART2_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p32 Tx p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_EasySetSM32(UART2_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT2_EasySetSM32(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CFG_Page);\
        ((__SELECT__^UART2_SELECT0)==OPTION_MATCH?(S2CFG=S2CFG&(~SM32)):\
        (__SELECT__^UART2_SELECT1)==OPTION_MATCH?(S2CFG=S2CFG&(~SM32)):\
        (__SELECT__^UART2_SELECT2)==OPTION_MATCH?(S2CFG=S2CFG&(~SM32)):\
        (__SELECT__^UART2_SELECT3)==OPTION_MATCH?(S2CFG=S2CFG&(~SM32)):\
        (__SELECT__^UART2_SELECT4)==OPTION_MATCH?(S2CFG=S2CFG&(~SM32)):\
        (__SELECT__^UART2_SELECT5)==OPTION_MATCH?(S2CFG=S2CFG&(~SM32)):\
        (__SELECT__^UART2_SELECT6)==OPTION_MATCH?(S2CFG=S2CFG&(~SM32)):\
        (__SELECT__^UART2_SELECT7)==OPTION_MATCH?(S2CFG=S2CFG&(~SM32)):\
        (__SELECT__^UART2_SELECT8)==OPTION_MATCH?(S2CFG=S2CFG&(~SM32)):\
        (__SELECT__^UART2_SELECT9)==OPTION_MATCH?(S2CFG=S2CFG&(~SM32)):\
        (__SELECT__^UART2_SELECT10)==OPTION_MATCH?(S2CFG=S2CFG&(~SM32)):\
        (__SELECT__^UART2_SELECT11)==OPTION_MATCH?(S2CFG=S2CFG&(~SM32)):\
        (__SELECT__^UART2_SELECT12)==OPTION_MATCH?(S2CFG=S2CFG&(~SM32)):\
        (__SELECT__^UART2_SELECT13)==OPTION_MATCH?(S2CFG=S2CFG&(~SM32)):\
        (__SELECT__^UART2_SELECT14)==OPTION_MATCH?(S2CFG=S2CFG&(~SM32)):\
        (__SELECT__^UART2_SELECT15)==OPTION_MATCH?(S2CFG=S2CFG&(~SM32)):\
        (__SELECT__^UART2_SELECT16)==OPTION_MATCH?(S2CFG=S2CFG&(~SM32)):\
        (__SELECT__^UART2_SELECT17)==OPTION_MATCH?(S2CFG=S2CFG&(~SM32)):\
        (__SELECT__^UART2_SELECT18)==OPTION_MATCH?(S2CFG=S2CFG&(~SM32)):\
        (__SELECT__^UART2_SELECT19)==OPTION_MATCH?(S2CFG=S2CFG&(~SM32)):\
        (__SELECT__^UART2_SELECT20)==OPTION_MATCH?(S2CFG=S2CFG&(~SM32)):\
        (__SELECT__^UART2_SELECT21)==OPTION_MATCH?(S2CFG=S2CFG&(~SM32)):\
        (__SELECT__^UART2_SELECT22)==OPTION_MATCH?(S2CFG=S2CFG&(~SM32)):\
        (__SELECT__^UART2_SELECT23)==OPTION_MATCH?(S2CFG=S2CFG&(~SM32)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)


/**
*******************************************************************************
* @brief       UART2 Mode Select
* @details     Set SM02
* @param[in]    __SELECT__ :
*  @arg\b       UART2_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p32 Tx p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_EasySetSM02(UART0_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT2_EasySetSM02(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CON_Page);\
        ((__SELECT__^UART2_SELECT0)==OPTION_MATCH?(SM02=0):\
        (__SELECT__^UART2_SELECT1)==OPTION_MATCH?(SM02=0):\
        (__SELECT__^UART2_SELECT2)==OPTION_MATCH?(SM02=0):\
        (__SELECT__^UART2_SELECT3)==OPTION_MATCH?(SM02=0):\
        (__SELECT__^UART2_SELECT4)==OPTION_MATCH?(SM02=0):\
        (__SELECT__^UART2_SELECT5)==OPTION_MATCH?(SM02=0):\
        (__SELECT__^UART2_SELECT6)==OPTION_MATCH?(SM02=0):\
        (__SELECT__^UART2_SELECT7)==OPTION_MATCH?(SM02=0):\
        (__SELECT__^UART2_SELECT8)==OPTION_MATCH?(SM02=0):\
        (__SELECT__^UART2_SELECT9)==OPTION_MATCH?(SM02=0):\
        (__SELECT__^UART2_SELECT10)==OPTION_MATCH?(SM02=0):\
        (__SELECT__^UART2_SELECT11)==OPTION_MATCH?(SM02=0):\
        (__SELECT__^UART2_SELECT12)==OPTION_MATCH?(SM02=0):\
        (__SELECT__^UART2_SELECT13)==OPTION_MATCH?(SM02=0):\
        (__SELECT__^UART2_SELECT14)==OPTION_MATCH?(SM02=0):\
        (__SELECT__^UART2_SELECT15)==OPTION_MATCH?(SM02=0):\
        (__SELECT__^UART2_SELECT16)==OPTION_MATCH?(SM02=0):\
        (__SELECT__^UART2_SELECT17)==OPTION_MATCH?(SM02=0):\
        (__SELECT__^UART2_SELECT18)==OPTION_MATCH?(SM02=0):\
        (__SELECT__^UART2_SELECT19)==OPTION_MATCH?(SM02=0):\
        (__SELECT__^UART2_SELECT20)==OPTION_MATCH?(SM02=0):\
        (__SELECT__^UART2_SELECT21)==OPTION_MATCH?(SM02=0):\
        (__SELECT__^UART2_SELECT22)==OPTION_MATCH?(SM02=0):\
        (__SELECT__^UART2_SELECT23)==OPTION_MATCH?(SM02=0):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)


/**
*******************************************************************************
* @brief       UART2 Mode Select
* @details     Set SM12
* @param[in]    __SELECT__ :
*  @arg\b       UART2_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p32 Tx p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_EasySetSM12(UART2_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/

#define __DRV_URT2_EasySetSM12(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CON_Page);\
        ((__SELECT__^UART2_SELECT0)==OPTION_MATCH?(SM12=1):\
        (__SELECT__^UART2_SELECT1)==OPTION_MATCH?(SM12=1):\
        (__SELECT__^UART2_SELECT2)==OPTION_MATCH?(SM12=1):\
        (__SELECT__^UART2_SELECT3)==OPTION_MATCH?(SM12=1):\
        (__SELECT__^UART2_SELECT4)==OPTION_MATCH?(SM12=1):\
        (__SELECT__^UART2_SELECT5)==OPTION_MATCH?(SM12=1):\
        (__SELECT__^UART2_SELECT6)==OPTION_MATCH?(SM12=1):\
        (__SELECT__^UART2_SELECT7)==OPTION_MATCH?(SM12=1):\
        (__SELECT__^UART2_SELECT8)==OPTION_MATCH?(SM12=1):\
        (__SELECT__^UART2_SELECT9)==OPTION_MATCH?(SM12=1):\
        (__SELECT__^UART2_SELECT10)==OPTION_MATCH?(SM12=1):\
        (__SELECT__^UART2_SELECT11)==OPTION_MATCH?(SM12=1):\
        (__SELECT__^UART2_SELECT12)==OPTION_MATCH?(SM12=1):\
        (__SELECT__^UART2_SELECT13)==OPTION_MATCH?(SM12=1):\
        (__SELECT__^UART2_SELECT14)==OPTION_MATCH?(SM12=1):\
        (__SELECT__^UART2_SELECT15)==OPTION_MATCH?(SM12=1):\
        (__SELECT__^UART2_SELECT16)==OPTION_MATCH?(SM12=1):\
        (__SELECT__^UART2_SELECT17)==OPTION_MATCH?(SM12=1):\
        (__SELECT__^UART2_SELECT18)==OPTION_MATCH?(SM12=1):\
        (__SELECT__^UART2_SELECT19)==OPTION_MATCH?(SM12=1):\
        (__SELECT__^UART2_SELECT20)==OPTION_MATCH?(SM12=1):\
        (__SELECT__^UART2_SELECT21)==OPTION_MATCH?(SM12=1):\
        (__SELECT__^UART2_SELECT22)==OPTION_MATCH?(SM12=1):\
        (__SELECT__^UART2_SELECT23)==OPTION_MATCH?(SM12=1):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief       UART2 S2BRG Clock Source
* @details     Set S2TX12
* @param[in]    __SELECT__ :
*  @arg\b       UART2_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p32 Tx p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_EasySetS2TX12(UART2_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT2_EasySetS2TX12(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CFG_Page);\
        ((__SELECT__^UART2_SELECT0)==OPTION_MATCH?(S2CFG=S2CFG|(S2TX12)):\
        (__SELECT__^UART2_SELECT1)==OPTION_MATCH?(S2CFG=S2CFG|(S2TX12)):\
        (__SELECT__^UART2_SELECT2)==OPTION_MATCH?(S2CFG=S2CFG|(S2TX12)):\
        (__SELECT__^UART2_SELECT3)==OPTION_MATCH?(S2CFG=S2CFG|(S2TX12)):\
        (__SELECT__^UART2_SELECT4)==OPTION_MATCH?(S2CFG=S2CFG|(S2TX12)):\
        (__SELECT__^UART2_SELECT5)==OPTION_MATCH?(S2CFG=S2CFG|(S2TX12)):\
        (__SELECT__^UART2_SELECT6)==OPTION_MATCH?(S2CFG=S2CFG|(S2TX12)):\
        (__SELECT__^UART2_SELECT7)==OPTION_MATCH?(S2CFG=S2CFG|(S2TX12)):\
        (__SELECT__^UART2_SELECT8)==OPTION_MATCH?(S2CFG=S2CFG|(S2TX12)):\
        (__SELECT__^UART2_SELECT9)==OPTION_MATCH?(S2CFG=S2CFG|(S2TX12)):\
        (__SELECT__^UART2_SELECT10)==OPTION_MATCH?(S2CFG=S2CFG|(S2TX12)):\
        (__SELECT__^UART2_SELECT11)==OPTION_MATCH?(S2CFG=S2CFG|(S2TX12)):\
        (__SELECT__^UART2_SELECT12)==OPTION_MATCH?(S2CFG=S2CFG|(S2TX12)):\
        (__SELECT__^UART2_SELECT13)==OPTION_MATCH?(S2CFG=S2CFG|(S2TX12)):\
        (__SELECT__^UART2_SELECT14)==OPTION_MATCH?(S2CFG=S2CFG|(S2TX12)):\
        (__SELECT__^UART2_SELECT15)==OPTION_MATCH?(S2CFG=S2CFG|(S2TX12)):\
        (__SELECT__^UART2_SELECT16)==OPTION_MATCH?(S2CFG=S2CFG|(S2TX12)):\
        (__SELECT__^UART2_SELECT17)==OPTION_MATCH?(S2CFG=S2CFG|(S2TX12)):\
        (__SELECT__^UART2_SELECT18)==OPTION_MATCH?(S2CFG=S2CFG|(S2TX12)):\
        (__SELECT__^UART2_SELECT19)==OPTION_MATCH?(S2CFG=S2CFG|(S2TX12)):\
        (__SELECT__^UART2_SELECT20)==OPTION_MATCH?(S2CFG=S2CFG|(S2TX12)):\
        (__SELECT__^UART2_SELECT21)==OPTION_MATCH?(S2CFG=S2CFG|(S2TX12)):\
        (__SELECT__^UART2_SELECT22)==OPTION_MATCH?(S2CFG=S2CFG|(S2TX12)):\
        (__SELECT__^UART2_SELECT23)==OPTION_MATCH?(S2CFG=S2CFG|(S2TX12)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief       UART2 Data Order
* @details     Set S2DOR
* @param[in]    __SELECT__ :
*  @arg\b       UART2_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p32 Tx p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_EasySetS2DOR(UART2_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/

#define __DRV_URT2_EasySetS2DOR(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CFG_Page);\
        ((__SELECT__^UART2_SELECT0)==OPTION_MATCH?(S2CFG=S2CFG|(S2DOR)):\
        (__SELECT__^UART2_SELECT1)==OPTION_MATCH?(S2CFG=S2CFG&(~S2DOR)):\
        (__SELECT__^UART2_SELECT2)==OPTION_MATCH?(S2CFG=S2CFG|(S2DOR)):\
        (__SELECT__^UART2_SELECT3)==OPTION_MATCH?(S2CFG=S2CFG&(~S2DOR)):\
        (__SELECT__^UART2_SELECT4)==OPTION_MATCH?(S2CFG=S2CFG|(S2DOR)):\
        (__SELECT__^UART2_SELECT5)==OPTION_MATCH?(S2CFG=S2CFG&(~S2DOR)):\
        (__SELECT__^UART2_SELECT6)==OPTION_MATCH?(S2CFG=S2CFG|(S2DOR)):\
        (__SELECT__^UART2_SELECT7)==OPTION_MATCH?(S2CFG=S2CFG&(~S2DOR)):\
        (__SELECT__^UART2_SELECT8)==OPTION_MATCH?(S2CFG=S2CFG|(S2DOR)):\
        (__SELECT__^UART2_SELECT9)==OPTION_MATCH?(S2CFG=S2CFG&(~S2DOR)):\
        (__SELECT__^UART2_SELECT10)==OPTION_MATCH?(S2CFG=S2CFG|(S2DOR)):\
        (__SELECT__^UART2_SELECT11)==OPTION_MATCH?(S2CFG=S2CFG&(~S2DOR)):\
        (__SELECT__^UART2_SELECT12)==OPTION_MATCH?(S2CFG=S2CFG|(S2DOR)):\
        (__SELECT__^UART2_SELECT13)==OPTION_MATCH?(S2CFG=S2CFG&(~S2DOR)):\
        (__SELECT__^UART2_SELECT14)==OPTION_MATCH?(S2CFG=S2CFG|(S2DOR)):\
        (__SELECT__^UART2_SELECT15)==OPTION_MATCH?(S2CFG=S2CFG&(~S2DOR)):\
        (__SELECT__^UART2_SELECT16)==OPTION_MATCH?(S2CFG=S2CFG|(S2DOR)):\
        (__SELECT__^UART2_SELECT17)==OPTION_MATCH?(S2CFG=S2CFG&(~S2DOR)):\
        (__SELECT__^UART2_SELECT18)==OPTION_MATCH?(S2CFG=S2CFG|(S2DOR)):\
        (__SELECT__^UART2_SELECT19)==OPTION_MATCH?(S2CFG=S2CFG&(~S2DOR)):\
        (__SELECT__^UART2_SELECT20)==OPTION_MATCH?(S2CFG=S2CFG|(S2DOR)):\
        (__SELECT__^UART2_SELECT21)==OPTION_MATCH?(S2CFG=S2CFG&(~S2DOR)):\
        (__SELECT__^UART2_SELECT22)==OPTION_MATCH?(S2CFG=S2CFG|(S2DOR)):\
        (__SELECT__^UART2_SELECT23)==OPTION_MATCH?(S2CFG=S2CFG&(~S2DOR)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief       UART2 Enhance Baud Rate
* @details     Set S2MOD1
* @param[in]    __SELECT__ :
*  @arg\b       UART2_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p32 Tx p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_EasySetS2MOD1(UART2_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT2_EasySetS2MOD1(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CFG_Page);\
        ((__SELECT__^UART2_SELECT0)==OPTION_MATCH?(S2CFG=S2CFG|(S2MOD1)):\
        (__SELECT__^UART2_SELECT1)==OPTION_MATCH?(S2CFG=S2CFG|(S2MOD1)):\
        (__SELECT__^UART2_SELECT2)==OPTION_MATCH?(S2CFG=S2CFG|(S2MOD1)):\
        (__SELECT__^UART2_SELECT3)==OPTION_MATCH?(S2CFG=S2CFG|(S2MOD1)):\
        (__SELECT__^UART2_SELECT4)==OPTION_MATCH?(S2CFG=S2CFG|(S2MOD1)):\
        (__SELECT__^UART2_SELECT5)==OPTION_MATCH?(S2CFG=S2CFG|(S2MOD1)):\
        (__SELECT__^UART2_SELECT6)==OPTION_MATCH?(S2CFG=S2CFG&(~S2MOD1)):\
        (__SELECT__^UART2_SELECT7)==OPTION_MATCH?(S2CFG=S2CFG&(~S2MOD1)):\
        (__SELECT__^UART2_SELECT8)==OPTION_MATCH?(S2CFG=S2CFG|(S2MOD1)):\
        (__SELECT__^UART2_SELECT9)==OPTION_MATCH?(S2CFG=S2CFG|(S2MOD1)):\
        (__SELECT__^UART2_SELECT10)==OPTION_MATCH?(S2CFG=S2CFG&(~S2MOD1)):\
        (__SELECT__^UART2_SELECT11)==OPTION_MATCH?(S2CFG=S2CFG&(~S2MOD1)):\
        (__SELECT__^UART2_SELECT12)==OPTION_MATCH?(S2CFG=S2CFG|(S2MOD1)):\
        (__SELECT__^UART2_SELECT13)==OPTION_MATCH?(S2CFG=S2CFG|(S2MOD1)):\
        (__SELECT__^UART2_SELECT14)==OPTION_MATCH?(S2CFG=S2CFG&(~S2MOD1)):\
        (__SELECT__^UART2_SELECT15)==OPTION_MATCH?(S2CFG=S2CFG&(~S2MOD1)):\
        (__SELECT__^UART2_SELECT16)==OPTION_MATCH?(S2CFG=S2CFG|(S2MOD1)):\
        (__SELECT__^UART2_SELECT17)==OPTION_MATCH?(S2CFG=S2CFG|(S2MOD1)):\
        (__SELECT__^UART2_SELECT18)==OPTION_MATCH?(S2CFG=S2CFG|(S2MOD1)):\
        (__SELECT__^UART2_SELECT19)==OPTION_MATCH?(S2CFG=S2CFG|(S2MOD1)):\
        (__SELECT__^UART2_SELECT20)==OPTION_MATCH?(S2CFG=S2CFG|(S2MOD1)):\
        (__SELECT__^UART2_SELECT21)==OPTION_MATCH?(S2CFG=S2CFG|(S2MOD1)):\
        (__SELECT__^UART2_SELECT22)==OPTION_MATCH?(S2CFG=S2CFG|(S2MOD1)):\
        (__SELECT__^UART2_SELECT23)==OPTION_MATCH?(S2CFG=S2CFG|(S2MOD1)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)



/**
*******************************************************************************
* @brief      UART2  Baud Rate Generator Reload Register
* @details    Write S2BRT
* @param[in]    __SELECT__ :
*  @arg\b       UART2_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p32 Tx p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_EasySetS2BRT(UART2_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT2_EasySetS2BRT
#define S2BRG_BRGRL_2400_1X_12000000_12T            0xF3        // 0.16%
#define S2BRG_BRGRL_2400_1X_12000000_1T             0x64        // 0.16%
#define S2BRG_BRGRL_4800_1X_12000000_1T             0xB2        // 0.16%
#define S2BRG_BRGRL_9600_1X_12000000_1T             0xD9        // 0.16%
#define S2BRG_BRGRL_19200_1X_12000000_1T            0xEC        // -2.34%
#define S2BRG_BRGRL_38400_1X_12000000_1T            0xF6        // -2.34%
#define S2BRG_BRGRL_2400_2X_12000000_12T            0xE6        // 0.16%
#define S2BRG_BRGRL_4800_2X_12000000_12T            0xF3        // 0.16%
#define S2BRG_BRGRL_4800_2X_12000000_1T             0x64        // 0.16%
#define S2BRG_BRGRL_9600_2X_12000000_1T             0xB2        // 0.16%
#define S2BRG_BRGRL_19200_2X_12000000_1T            0xD9        // 0.16%
#define S2BRG_BRGRL_38400_2X_12000000_1T            0xEC        // -2.34%
#define S2BRG_BRGRL_57600_2X_12000000_1T            0xF3        // 0.16%
#define S2BRG_BRGRL_115200_2X_11059200_1T           0xFA        // 0%
#define S2BRG_BRGRL_115200_1X_11059200_1T           0xFD        // 0%
#define S2BRG_BRGRL_57600_1X_11059200_1T            0xFA        // 0%
#define S2BRG_BRGRL_230400_2X_11059200_1T           0xFD        // 0%
#define S2BRG_BRGRL_250000_2X_12000000_1T           0xFD        // 0%
#define S2BRG_BRGRL_750000_2X_12000000_1T           0xFF        // 0%
///@endcond

#define __DRV_URT2_EasySetS2BRT(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S2BRT_Page);\
        ((__SELECT__^UART2_SELECT0)==OPTION_MATCH?(S2BRT=S2BRG_BRGRL_4800_2X_12000000_1T):\
        (__SELECT__^UART2_SELECT1)==OPTION_MATCH?(S2BRT=S2BRG_BRGRL_4800_2X_12000000_1T):\
        (__SELECT__^UART2_SELECT2)==OPTION_MATCH?(S2BRT=S2BRG_BRGRL_9600_2X_12000000_1T):\
        (__SELECT__^UART2_SELECT3)==OPTION_MATCH?(S2BRT=S2BRG_BRGRL_9600_2X_12000000_1T):\
        (__SELECT__^UART2_SELECT4)==OPTION_MATCH?(S2BRT=S2BRG_BRGRL_19200_2X_12000000_1T):\
        (__SELECT__^UART2_SELECT5)==OPTION_MATCH?(S2BRT=S2BRG_BRGRL_19200_2X_12000000_1T):\
        (__SELECT__^UART2_SELECT6)==OPTION_MATCH?(S2BRT=S2BRG_BRGRL_38400_1X_12000000_1T):\
        (__SELECT__^UART2_SELECT7)==OPTION_MATCH?(S2BRT=S2BRG_BRGRL_38400_1X_12000000_1T):\
        (__SELECT__^UART2_SELECT8)==OPTION_MATCH?(S2BRT=S2BRG_BRGRL_38400_2X_12000000_1T):\
        (__SELECT__^UART2_SELECT9)==OPTION_MATCH?(S2BRT=S2BRG_BRGRL_38400_2X_12000000_1T):\
        (__SELECT__^UART2_SELECT10)==OPTION_MATCH?(S2BRT=S2BRG_BRGRL_57600_1X_11059200_1T):\
        (__SELECT__^UART2_SELECT11)==OPTION_MATCH?(S2BRT=S2BRG_BRGRL_57600_1X_11059200_1T):\
        (__SELECT__^UART2_SELECT12)==OPTION_MATCH?(S2BRT=S2BRG_BRGRL_57600_2X_12000000_1T):\
        (__SELECT__^UART2_SELECT13)==OPTION_MATCH?(S2BRT=S2BRG_BRGRL_57600_2X_12000000_1T):\
        (__SELECT__^UART2_SELECT14)==OPTION_MATCH?(S2BRT=S2BRG_BRGRL_115200_1X_11059200_1T):\
        (__SELECT__^UART2_SELECT15)==OPTION_MATCH?(S2BRT=S2BRG_BRGRL_115200_1X_11059200_1T):\
        (__SELECT__^UART2_SELECT16)==OPTION_MATCH?(S2BRT=S2BRG_BRGRL_115200_2X_11059200_1T):\
        (__SELECT__^UART2_SELECT17)==OPTION_MATCH?(S2BRT=S2BRG_BRGRL_115200_2X_11059200_1T):\
        (__SELECT__^UART2_SELECT18)==OPTION_MATCH?(S2BRT=S2BRG_BRGRL_230400_2X_11059200_1T):\
        (__SELECT__^UART2_SELECT19)==OPTION_MATCH?(S2BRT=S2BRG_BRGRL_230400_2X_11059200_1T):\
        (__SELECT__^UART2_SELECT20)==OPTION_MATCH?(S2BRT=S2BRG_BRGRL_250000_2X_12000000_1T):\
        (__SELECT__^UART2_SELECT21)==OPTION_MATCH?(S2BRT=S2BRG_BRGRL_250000_2X_12000000_1T):\
        (__SELECT__^UART2_SELECT22)==OPTION_MATCH?(S2BRT=S2BRG_BRGRL_750000_2X_12000000_1T):\
        (__SELECT__^UART2_SELECT23)==OPTION_MATCH?(S2BRT=S2BRG_BRGRL_750000_2X_12000000_1T):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)


/**
*******************************************************************************
* @brief      UART2  Baud Rate Generator Reload Register
* @details    Write S2BRC
* @param[in]    __SELECT__ :
*  @arg\b       UART2_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p32 Tx p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_EasySetS2BRC(UART2_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT2_EasySetS2BRC(__SELECT__)\
    MWT(\
         __DRV_SFR_PageIndex(S2BRC_Page);\
         ((__SELECT__^UART2_SELECT0)==OPTION_MATCH?(S2BRC=S2BRG_BRGRL_4800_2X_12000000_1T):\
         (__SELECT__^UART2_SELECT1)==OPTION_MATCH?(S2BRC=S2BRG_BRGRL_4800_2X_12000000_1T):\
         (__SELECT__^UART2_SELECT2)==OPTION_MATCH?(S2BRC=S2BRG_BRGRL_9600_2X_12000000_1T):\
         (__SELECT__^UART2_SELECT3)==OPTION_MATCH?(S2BRC=S2BRG_BRGRL_9600_2X_12000000_1T):\
         (__SELECT__^UART2_SELECT4)==OPTION_MATCH?(S2BRC=S2BRG_BRGRL_19200_2X_12000000_1T):\
         (__SELECT__^UART2_SELECT5)==OPTION_MATCH?(S2BRC=S2BRG_BRGRL_19200_2X_12000000_1T):\
         (__SELECT__^UART2_SELECT6)==OPTION_MATCH?(S2BRC=S2BRG_BRGRL_38400_1X_12000000_1T):\
         (__SELECT__^UART2_SELECT7)==OPTION_MATCH?(S2BRC=S2BRG_BRGRL_38400_1X_12000000_1T):\
         (__SELECT__^UART2_SELECT8)==OPTION_MATCH?(S2BRC=S2BRG_BRGRL_38400_2X_12000000_1T):\
         (__SELECT__^UART2_SELECT9)==OPTION_MATCH?(S2BRC=S2BRG_BRGRL_38400_2X_12000000_1T):\
         (__SELECT__^UART2_SELECT10)==OPTION_MATCH?(S2BRC=S2BRG_BRGRL_57600_1X_11059200_1T):\
         (__SELECT__^UART2_SELECT11)==OPTION_MATCH?(S2BRC=S2BRG_BRGRL_57600_1X_11059200_1T):\
         (__SELECT__^UART2_SELECT12)==OPTION_MATCH?(S2BRC=S2BRG_BRGRL_57600_2X_12000000_1T):\
         (__SELECT__^UART2_SELECT13)==OPTION_MATCH?(S2BRC=S2BRG_BRGRL_57600_2X_12000000_1T):\
         (__SELECT__^UART2_SELECT14)==OPTION_MATCH?(S2BRC=S2BRG_BRGRL_115200_1X_11059200_1T):\
         (__SELECT__^UART2_SELECT15)==OPTION_MATCH?(S2BRC=S2BRG_BRGRL_115200_1X_11059200_1T):\
         (__SELECT__^UART2_SELECT16)==OPTION_MATCH?(S2BRC=S2BRG_BRGRL_115200_2X_11059200_1T):\
         (__SELECT__^UART2_SELECT17)==OPTION_MATCH?(S2BRC=S2BRG_BRGRL_115200_2X_11059200_1T):\
         (__SELECT__^UART2_SELECT18)==OPTION_MATCH?(S2BRC=S2BRG_BRGRL_230400_2X_11059200_1T):\
         (__SELECT__^UART2_SELECT19)==OPTION_MATCH?(S2BRC=S2BRG_BRGRL_230400_2X_11059200_1T):\
         (__SELECT__^UART2_SELECT20)==OPTION_MATCH?(S2BRC=S2BRG_BRGRL_250000_2X_12000000_1T):\
         (__SELECT__^UART2_SELECT21)==OPTION_MATCH?(S2BRC=S2BRG_BRGRL_250000_2X_12000000_1T):\
         (__SELECT__^UART2_SELECT22)==OPTION_MATCH?(S2BRC=S2BRG_BRGRL_750000_2X_12000000_1T):\
         (__SELECT__^UART2_SELECT23)==OPTION_MATCH?(S2BRC=S2BRG_BRGRL_750000_2X_12000000_1T):_nop_());\
         __DRV_SFR_PageIndex(0);\
     ;)



/**
*******************************************************************************
* @brief       UART2 S2BRG Operation Enable
* @details     Set S2TR
* @param[in]    __SELECT__ :
*  @arg\b       UART2_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p32 Tx p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_EasySetS2TR_Start(UART2_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT2_EasySetS2TR_Start(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CFG_Page);\
        ((__SELECT__^UART2_SELECT0)==OPTION_MATCH?(S2CFG=S2CFG|(S2TR)):\
        (__SELECT__^UART2_SELECT1)==OPTION_MATCH?(S2CFG=S2CFG|(S2TR)):\
        (__SELECT__^UART2_SELECT2)==OPTION_MATCH?(S2CFG=S2CFG|(S2TR)):\
        (__SELECT__^UART2_SELECT3)==OPTION_MATCH?(S2CFG=S2CFG|(S2TR)):\
        (__SELECT__^UART2_SELECT4)==OPTION_MATCH?(S2CFG=S2CFG|(S2TR)):\
        (__SELECT__^UART2_SELECT5)==OPTION_MATCH?(S2CFG=S2CFG|(S2TR)):\
        (__SELECT__^UART2_SELECT6)==OPTION_MATCH?(S2CFG=S2CFG|(S2TR)):\
        (__SELECT__^UART2_SELECT7)==OPTION_MATCH?(S2CFG=S2CFG|(S2TR)):\
        (__SELECT__^UART2_SELECT8)==OPTION_MATCH?(S2CFG=S2CFG|(S2TR)):\
        (__SELECT__^UART2_SELECT9)==OPTION_MATCH?(S2CFG=S2CFG|(S2TR)):\
        (__SELECT__^UART2_SELECT10)==OPTION_MATCH?(S2CFG=S2CFG|(S2TR)):\
        (__SELECT__^UART2_SELECT11)==OPTION_MATCH?(S2CFG=S2CFG|(S2TR)):\
        (__SELECT__^UART2_SELECT12)==OPTION_MATCH?(S2CFG=S2CFG|(S2TR)):\
        (__SELECT__^UART2_SELECT13)==OPTION_MATCH?(S2CFG=S2CFG|(S2TR)):\
        (__SELECT__^UART2_SELECT14)==OPTION_MATCH?(S2CFG=S2CFG|(S2TR)):\
        (__SELECT__^UART2_SELECT15)==OPTION_MATCH?(S2CFG=S2CFG|(S2TR)):\
        (__SELECT__^UART2_SELECT16)==OPTION_MATCH?(S2CFG=S2CFG|(S2TR)):\
        (__SELECT__^UART2_SELECT17)==OPTION_MATCH?(S2CFG=S2CFG|(S2TR)):\
        (__SELECT__^UART2_SELECT18)==OPTION_MATCH?(S2CFG=S2CFG|(S2TR)):\
        (__SELECT__^UART2_SELECT19)==OPTION_MATCH?(S2CFG=S2CFG|(S2TR)):\
        (__SELECT__^UART2_SELECT20)==OPTION_MATCH?(S2CFG=S2CFG|(S2TR)):\
        (__SELECT__^UART2_SELECT21)==OPTION_MATCH?(S2CFG=S2CFG|(S2TR)):\
        (__SELECT__^UART2_SELECT22)==OPTION_MATCH?(S2CFG=S2CFG|(S2TR)):\
        (__SELECT__^UART2_SELECT23)==OPTION_MATCH?(S2CFG=S2CFG|(S2TR)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief       UART2 S2BRG Operation Disable
* @details     Set S2TR
* @param[in]    __SELECT__ :
*  @arg\b       UART2_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p32 Tx p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_EasySetS2TR_Stop(UART2_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT2_EasySetS2TR_Stop(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CFG_Page);\
        ((__SELECT__^UART2_SELECT0)==OPTION_MATCH?(S2CFG=S2CFG&(~S2TR)):\
        (__SELECT__^UART2_SELECT1)==OPTION_MATCH?(S2CFG=S2CFG&(~S2TR)):\
        (__SELECT__^UART2_SELECT2)==OPTION_MATCH?(S2CFG=S2CFG&(~S2TR)):\
        (__SELECT__^UART2_SELECT3)==OPTION_MATCH?(S2CFG=S2CFG&(~S2TR)):\
        (__SELECT__^UART2_SELECT4)==OPTION_MATCH?(S2CFG=S2CFG&(~S2TR)):\
        (__SELECT__^UART2_SELECT5)==OPTION_MATCH?(S2CFG=S2CFG&(~S2TR)):\
        (__SELECT__^UART2_SELECT6)==OPTION_MATCH?(S2CFG=S2CFG&(~S2TR)):\
        (__SELECT__^UART2_SELECT7)==OPTION_MATCH?(S2CFG=S2CFG&(~S2TR)):\
        (__SELECT__^UART2_SELECT8)==OPTION_MATCH?(S2CFG=S2CFG&(~S2TR)):\
        (__SELECT__^UART2_SELECT9)==OPTION_MATCH?(S2CFG=S2CFG&(~S2TR)):\
        (__SELECT__^UART2_SELECT10)==OPTION_MATCH?(S2CFG=S2CFG&(~S2TR)):\
        (__SELECT__^UART2_SELECT11)==OPTION_MATCH?(S2CFG=S2CFG&(~S2TR)):\
        (__SELECT__^UART2_SELECT12)==OPTION_MATCH?(S2CFG=S2CFG&(~S2TR)):\
        (__SELECT__^UART2_SELECT13)==OPTION_MATCH?(S2CFG=S2CFG&(~S2TR)):\
        (__SELECT__^UART2_SELECT14)==OPTION_MATCH?(S2CFG=S2CFG&(~S2TR)):\
        (__SELECT__^UART2_SELECT15)==OPTION_MATCH?(S2CFG=S2CFG&(~S2TR)):\
        (__SELECT__^UART2_SELECT16)==OPTION_MATCH?(S2CFG=S2CFG&(~S2TR)):\
        (__SELECT__^UART2_SELECT17)==OPTION_MATCH?(S2CFG=S2CFG&(~S2TR)):\
        (__SELECT__^UART2_SELECT18)==OPTION_MATCH?(S2CFG=S2CFG&(~S2TR)):\
        (__SELECT__^UART2_SELECT19)==OPTION_MATCH?(S2CFG=S2CFG&(~S2TR)):\
        (__SELECT__^UART2_SELECT20)==OPTION_MATCH?(S2CFG=S2CFG&(~S2TR)):\
        (__SELECT__^UART2_SELECT21)==OPTION_MATCH?(S2CFG=S2CFG&(~S2TR)):\
        (__SELECT__^UART2_SELECT22)==OPTION_MATCH?(S2CFG=S2CFG&(~S2TR)):\
        (__SELECT__^UART2_SELECT23)==OPTION_MATCH?(S2CFG=S2CFG&(~S2TR)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)


/**
*******************************************************************************
* @brief        UART2 Pin Config
* @details     Set S2PS0
* @param[in]    __SELECT__ :
*  @arg\b       UART2_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p32 Tx p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 default baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
*  @arg\b       UART2_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p32 TX p33 Tx Rx both SYSCLK/1 double baud rate S2BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_EasySetS2PS0(UART2_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/

#define __DRV_URT2_EasySetS2PS0(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR13_Page);\
        ((__SELECT__^UART2_SELECT0)==OPTION_MATCH?(AUXR13=AUXR13&(~S2PS0)):\
        (__SELECT__^UART2_SELECT1)==OPTION_MATCH?(AUXR13=AUXR13&(~S2PS0)):\
        (__SELECT__^UART2_SELECT2)==OPTION_MATCH?(AUXR13=AUXR13&(~S2PS0)):\
        (__SELECT__^UART2_SELECT3)==OPTION_MATCH?(AUXR13=AUXR13&(~S2PS0)):\
        (__SELECT__^UART2_SELECT4)==OPTION_MATCH?(AUXR13=AUXR13&(~S2PS0)):\
        (__SELECT__^UART2_SELECT5)==OPTION_MATCH?(AUXR13=AUXR13&(~S2PS0)):\
        (__SELECT__^UART2_SELECT6)==OPTION_MATCH?(AUXR13=AUXR13&(~S2PS0)):\
        (__SELECT__^UART2_SELECT7)==OPTION_MATCH?(AUXR13=AUXR13&(~S2PS0)):\
        (__SELECT__^UART2_SELECT8)==OPTION_MATCH?(AUXR13=AUXR13&(~S2PS0)):\
        (__SELECT__^UART2_SELECT9)==OPTION_MATCH?(AUXR13=AUXR13&(~S2PS0)):\
        (__SELECT__^UART2_SELECT10)==OPTION_MATCH?(AUXR13=AUXR13&(~S2PS0)):\
        (__SELECT__^UART2_SELECT11)==OPTION_MATCH?(AUXR13=AUXR13&(~S2PS0)):\
        (__SELECT__^UART2_SELECT12)==OPTION_MATCH?(AUXR13=AUXR13&(~S2PS0)):\
        (__SELECT__^UART2_SELECT13)==OPTION_MATCH?(AUXR13=AUXR13&(~S2PS0)):\
        (__SELECT__^UART2_SELECT14)==OPTION_MATCH?(AUXR13=AUXR13&(~S2PS0)):\
        (__SELECT__^UART2_SELECT15)==OPTION_MATCH?(AUXR13=AUXR13&(~S2PS0)):\
        (__SELECT__^UART2_SELECT16)==OPTION_MATCH?(AUXR13=AUXR13&(~S2PS0)):\
        (__SELECT__^UART2_SELECT17)==OPTION_MATCH?(AUXR13=AUXR13&(~S2PS0)):\
        (__SELECT__^UART2_SELECT18)==OPTION_MATCH?(AUXR13=AUXR13&(~S2PS0)):\
        (__SELECT__^UART2_SELECT19)==OPTION_MATCH?(AUXR13=AUXR13&(~S2PS0)):\
        (__SELECT__^UART2_SELECT20)==OPTION_MATCH?(AUXR13=AUXR13&(~S2PS0)):\
        (__SELECT__^UART2_SELECT21)==OPTION_MATCH?(AUXR13=AUXR13&(~S2PS0)):\
        (__SELECT__^UART2_SELECT22)==OPTION_MATCH?(AUXR13=AUXR13&(~S2PS0)):\
        (__SELECT__^UART2_SELECT23)==OPTION_MATCH?(AUXR13=AUXR13&(~S2PS0)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)











#endif  //MG82F6D64_UART2_MID_H



