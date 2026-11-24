/**
 ******************************************************************************
 *
 * @file        MG82F6D64_UART3_MID.h
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

#ifndef MG82F6D64_UART3_MID_H
#define MG82F6D64_UART3_MID_H





///@cond __DRV_URT3_Easy_Wizard_Init
#define UART3_8BIT_CONFIG0 (UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_4800|UART3_PIN_CONFIG_RX_P36_TX_P37)
#define UART3_8BIT_CONFIG1 (UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_4800|UART3_PIN_CONFIG_RX_P36_TX_P37)
#define UART3_8BIT_CONFIG2 (UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_9600|UART3_PIN_CONFIG_RX_P36_TX_P37)
#define UART3_8BIT_CONFIG3 (UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_9600|UART3_PIN_CONFIG_RX_P36_TX_P37)
#define UART3_8BIT_CONFIG4 (UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_19200|UART3_PIN_CONFIG_RX_P36_TX_P37)
#define UART3_8BIT_CONFIG5 (UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_19200|UART3_PIN_CONFIG_RX_P36_TX_P37)
#define UART3_8BIT_CONFIG6 (UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_1X_LSB_TX_RX|UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_38400|UART3_PIN_CONFIG_RX_P36_TX_P37)
#define UART3_8BIT_CONFIG7 (UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_1X_MSB_TX_RX|UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_38400|UART3_PIN_CONFIG_RX_P36_TX_P37)
#define UART3_8BIT_CONFIG8 (UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_38400|UART3_PIN_CONFIG_RX_P36_TX_P37)
#define UART3_8BIT_CONFIG9 (UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_38400|UART3_PIN_CONFIG_RX_P36_TX_P37)
#define UART3_8BIT_CONFIG10 (UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_1X_LSB_TX_RX|UART3_8BIT_S3BRG_IHRCO_110592MHZ_BAUD_RATE_57600|UART3_PIN_CONFIG_RX_P36_TX_P37)
#define UART3_8BIT_CONFIG11 (UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_1X_MSB_TX_RX|UART3_8BIT_S3BRG_IHRCO_110592MHZ_BAUD_RATE_57600|UART3_PIN_CONFIG_RX_P36_TX_P37)
#define UART3_8BIT_CONFIG12 (UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_57600|UART3_PIN_CONFIG_RX_P36_TX_P37)
#define UART3_8BIT_CONFIG13 (UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_57600|UART3_PIN_CONFIG_RX_P36_TX_P37)
#define UART3_8BIT_CONFIG14 (UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_1X_LSB_TX_RX|UART3_8BIT_S3BRG_IHRCO_110592MHZ_BAUD_RATE_115200|UART3_PIN_CONFIG_RX_P36_TX_P37)
#define UART3_8BIT_CONFIG15 (UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_1X_MSB_TX_RX|UART3_8BIT_S3BRG_IHRCO_110592MHZ_BAUD_RATE_115200|UART3_PIN_CONFIG_RX_P36_TX_P37)
#define UART3_8BIT_CONFIG16 (UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART3_8BIT_S3BRG_IHRCO_110592MHZ_BAUD_RATE_115200|UART3_PIN_CONFIG_RX_P36_TX_P37)
#define UART3_8BIT_CONFIG17 (UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART3_8BIT_S3BRG_IHRCO_110592MHZ_BAUD_RATE_115200|UART3_PIN_CONFIG_RX_P36_TX_P37)
#define UART3_8BIT_CONFIG18 (UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_230400|UART3_PIN_CONFIG_RX_P36_TX_P37)
#define UART3_8BIT_CONFIG19 (UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_230400|UART3_PIN_CONFIG_RX_P36_TX_P37)
#define UART3_8BIT_CONFIG20 (UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_250000|UART3_PIN_CONFIG_RX_P36_TX_P37)
#define UART3_8BIT_CONFIG21 (UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_250000|UART3_PIN_CONFIG_RX_P36_TX_P37)
#define UART3_8BIT_CONFIG22 (UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX|UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_750000|UART3_PIN_CONFIG_RX_P36_TX_P37)
#define UART3_8BIT_CONFIG23 (UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX|UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_750000|UART3_PIN_CONFIG_RX_P36_TX_P37)
//constant URT3 mode (0~255) option
#define UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_2X_LSB_TX_RX    (0x00000000)
#define UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_2X_MSB_TX_RX    (0x01000000)
#define UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_1X_LSB_TX_RX    (0x02000000)
#define UART3_8BIT_S3BRG_SYSCLK_DIV_1_BAUD_RATE_1X_MSB_TX_RX    (0x03000000)
//constant URT3 baud rate(0~255) option
#define UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_4800    (0x00000000)
#define UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_9600    (0x00010000)
#define UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_19200    (0x00020000)
#define UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_38400    (0x00030000)
#define UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_57600    (0x00040000)
#define UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_115200    (0x00050000)
#define UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_230400    (0x00060000)
#define UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_250000    (0x00070000)
#define UART3_8BIT_S3BRG_IHRCO_12MHZ_BAUD_RATE_750000    (0x00080000)
#define UART3_8BIT_S3BRG_IHRCO_110592MHZ_BAUD_RATE_57600    (0x00090000)
#define UART3_8BIT_S3BRG_IHRCO_110592MHZ_BAUD_RATE_115200    (0x000a0000)
//constant URT3 pin config (0~15) option
#define UART3_PIN_CONFIG_RX_P36_TX_P37    (0x00000000)
#define UART3_PIN_CONFIG_RX_P65_TX_P66    (0x00000100)


///@endcond




/**
*******************************************************************************
* @brief       UART3 Easy Wizard
* @details     Set SM33 SM03 SM13 S3DOR S3MOD1
* @param[in]    __SELECT__ :
*  @arg\b       UART3_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p36 Tx p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_Mode_Easy_Select(URT3_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/

#define __DRV_URT3_Mode_Easy_Select(__SELECT__)\
    MWT(\
        __DRV_URT3_EasySetSM33(__SELECT__);\
        __DRV_URT3_EasySetSM03(__SELECT__);\
        __DRV_URT3_EasySetSM13(__SELECT__);\
        __DRV_URT3_EasySetS3DOR(__SELECT__);\
        __DRV_URT3_EasySetS3MOD1(__SELECT__);\
    ;)

/**
*******************************************************************************
* @brief       UART3 Easy Wizard
* @details     Set S3TR S3BRT S3BRC
* @param[in]    __SELECT__ :
*  @arg\b       UART3_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p36 Tx p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_BaudRate_Easy_Select(URT3_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT3_BaudRate_Easy_Select(__SELECT__)\
    MWT(\
        __DRV_URT3_EasySetS3TR_Stop(__SELECT__);\
        __DRV_URT3_EasySetS3TX12(__SELECT__);\
        __DRV_URT3_EasySetS3BRT(__SELECT__);\
        __DRV_URT3_EasySetS3BRC(__SELECT__);\
        __DRV_URT3_EasySetS3TR_Start(__SELECT__);\
     ;)

/**
*******************************************************************************
* @brief       UART3 PinMux Easy Wizard
* @details     Set S3PS0
* @param[in]    __SELECT__ :
*  @arg\b       UART3_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p36 Tx p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_PinMux_Easy_Select(URT3_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT3_PinMux_Easy_Select(__SELECT__)\
    MWT(\
        __DRV_URT3_EasySetS3PS0(__SELECT__);\
    ;)




/**
*******************************************************************************
* @brief       UART3 Serial Reception
* @details     Set REN3
* @param[in]   __SELECT__ :
*  @arg\b       UART3_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p36 Tx p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_EasyEnableSerialReception(UART3_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/

#define __DRV_URT3_EasyEnableSerialReception(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CON_Page);\
        ((__SELECT__^UART3_SELECT0)==OPTION_MATCH?(REN3=1):\
        (__SELECT__^UART3_SELECT1)==OPTION_MATCH?(REN3=1):\
        (__SELECT__^UART3_SELECT2)==OPTION_MATCH?(REN3=1):\
        (__SELECT__^UART3_SELECT3)==OPTION_MATCH?(REN3=1):\
        (__SELECT__^UART3_SELECT4)==OPTION_MATCH?(REN3=1):\
        (__SELECT__^UART3_SELECT5)==OPTION_MATCH?(REN3=1):\
        (__SELECT__^UART3_SELECT6)==OPTION_MATCH?(REN3=1):\
        (__SELECT__^UART3_SELECT7)==OPTION_MATCH?(REN3=1):\
        (__SELECT__^UART3_SELECT8)==OPTION_MATCH?(REN3=1):\
        (__SELECT__^UART3_SELECT9)==OPTION_MATCH?(REN3=1):\
        (__SELECT__^UART3_SELECT10)==OPTION_MATCH?(REN3=1):\
        (__SELECT__^UART3_SELECT11)==OPTION_MATCH?(REN3=1):\
        (__SELECT__^UART3_SELECT12)==OPTION_MATCH?(REN3=1):\
        (__SELECT__^UART3_SELECT13)==OPTION_MATCH?(REN3=1):\
        (__SELECT__^UART3_SELECT14)==OPTION_MATCH?(REN3=1):\
        (__SELECT__^UART3_SELECT15)==OPTION_MATCH?(REN3=1):\
        (__SELECT__^UART3_SELECT16)==OPTION_MATCH?(REN3=1):\
        (__SELECT__^UART3_SELECT17)==OPTION_MATCH?(REN3=1):\
        (__SELECT__^UART3_SELECT18)==OPTION_MATCH?(REN3=1):\
        (__SELECT__^UART3_SELECT19)==OPTION_MATCH?(REN3=1):\
        (__SELECT__^UART3_SELECT20)==OPTION_MATCH?(REN3=1):\
        (__SELECT__^UART3_SELECT21)==OPTION_MATCH?(REN3=1):\
        (__SELECT__^UART3_SELECT22)==OPTION_MATCH?(REN3=1):\
        (__SELECT__^UART3_SELECT23)==OPTION_MATCH?(REN3=1):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)


/**
*******************************************************************************
* @brief       UART3 Serial Reception
* @details     Clear REN3
* @param[in]    __SELECT__ :
*  @arg\b       UART3_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p36 Tx p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_EasyDisableSerialReception(UART3_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT3_EasyDisableSerialReception(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CON_Page);\
        ((__SELECT__^UART3_SELECT0)==OPTION_MATCH?(REN3=0):\
        (__SELECT__^UART3_SELECT1)==OPTION_MATCH?(REN3=0):\
        (__SELECT__^UART3_SELECT2)==OPTION_MATCH?(REN3=0):\
        (__SELECT__^UART3_SELECT3)==OPTION_MATCH?(REN3=0):\
        (__SELECT__^UART3_SELECT4)==OPTION_MATCH?(REN3=0):\
        (__SELECT__^UART3_SELECT5)==OPTION_MATCH?(REN3=0):\
        (__SELECT__^UART3_SELECT6)==OPTION_MATCH?(REN3=0):\
        (__SELECT__^UART3_SELECT7)==OPTION_MATCH?(REN3=0):\
        (__SELECT__^UART3_SELECT8)==OPTION_MATCH?(REN3=0):\
        (__SELECT__^UART3_SELECT9)==OPTION_MATCH?(REN3=0):\
        (__SELECT__^UART3_SELECT10)==OPTION_MATCH?(REN3=0):\
        (__SELECT__^UART3_SELECT11)==OPTION_MATCH?(REN3=0):\
        (__SELECT__^UART3_SELECT12)==OPTION_MATCH?(REN3=0):\
        (__SELECT__^UART3_SELECT13)==OPTION_MATCH?(REN3=0):\
        (__SELECT__^UART3_SELECT14)==OPTION_MATCH?(REN3=0):\
        (__SELECT__^UART3_SELECT15)==OPTION_MATCH?(REN3=0):\
        (__SELECT__^UART3_SELECT16)==OPTION_MATCH?(REN3=0):\
        (__SELECT__^UART3_SELECT17)==OPTION_MATCH?(REN3=0):\
        (__SELECT__^UART3_SELECT18)==OPTION_MATCH?(REN3=0):\
        (__SELECT__^UART3_SELECT19)==OPTION_MATCH?(REN3=0):\
        (__SELECT__^UART3_SELECT20)==OPTION_MATCH?(REN3=0):\
        (__SELECT__^UART3_SELECT21)==OPTION_MATCH?(REN3=0):\
        (__SELECT__^UART3_SELECT22)==OPTION_MATCH?(REN3=0):\
        (__SELECT__^UART3_SELECT23)==OPTION_MATCH?(REN3=0):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)


/**
*******************************************************************************
* @brief       UART3 Mode Select
* @details     Set SM33
* @param[in]    __SELECT__ :
*  @arg\b       UART3_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p36 Tx p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_EasySetSM33(UART3_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT3_EasySetSM33(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CON_Page);\
        ((__SELECT__^UART3_SELECT0)==OPTION_MATCH?(S3CFG=S3CFG&(~SM33)):\
        (__SELECT__^UART3_SELECT1)==OPTION_MATCH?(S3CFG=S3CFG&(~SM33)):\
        (__SELECT__^UART3_SELECT2)==OPTION_MATCH?(S3CFG=S3CFG&(~SM33)):\
        (__SELECT__^UART3_SELECT3)==OPTION_MATCH?(S3CFG=S3CFG&(~SM33)):\
        (__SELECT__^UART3_SELECT4)==OPTION_MATCH?(S3CFG=S3CFG&(~SM33)):\
        (__SELECT__^UART3_SELECT5)==OPTION_MATCH?(S3CFG=S3CFG&(~SM33)):\
        (__SELECT__^UART3_SELECT6)==OPTION_MATCH?(S3CFG=S3CFG&(~SM33)):\
        (__SELECT__^UART3_SELECT7)==OPTION_MATCH?(S3CFG=S3CFG&(~SM33)):\
        (__SELECT__^UART3_SELECT8)==OPTION_MATCH?(S3CFG=S3CFG&(~SM33)):\
        (__SELECT__^UART3_SELECT9)==OPTION_MATCH?(S3CFG=S3CFG&(~SM33)):\
        (__SELECT__^UART3_SELECT10)==OPTION_MATCH?(S3CFG=S3CFG&(~SM33)):\
        (__SELECT__^UART3_SELECT11)==OPTION_MATCH?(S3CFG=S3CFG&(~SM33)):\
        (__SELECT__^UART3_SELECT12)==OPTION_MATCH?(S3CFG=S3CFG&(~SM33)):\
        (__SELECT__^UART3_SELECT13)==OPTION_MATCH?(S3CFG=S3CFG&(~SM33)):\
        (__SELECT__^UART3_SELECT14)==OPTION_MATCH?(S3CFG=S3CFG&(~SM33)):\
        (__SELECT__^UART3_SELECT15)==OPTION_MATCH?(S3CFG=S3CFG&(~SM33)):\
        (__SELECT__^UART3_SELECT16)==OPTION_MATCH?(S3CFG=S3CFG&(~SM33)):\
        (__SELECT__^UART3_SELECT17)==OPTION_MATCH?(S3CFG=S3CFG&(~SM33)):\
        (__SELECT__^UART3_SELECT18)==OPTION_MATCH?(S3CFG=S3CFG&(~SM33)):\
        (__SELECT__^UART3_SELECT19)==OPTION_MATCH?(S3CFG=S3CFG&(~SM33)):\
        (__SELECT__^UART3_SELECT20)==OPTION_MATCH?(S3CFG=S3CFG&(~SM33)):\
        (__SELECT__^UART3_SELECT21)==OPTION_MATCH?(S3CFG=S3CFG&(~SM33)):\
        (__SELECT__^UART3_SELECT22)==OPTION_MATCH?(S3CFG=S3CFG&(~SM33)):\
        (__SELECT__^UART3_SELECT23)==OPTION_MATCH?(S3CFG=S3CFG&(~SM33)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)


/**
*******************************************************************************
* @brief       UART3 Mode Select
* @details     Set SM03
* @param[in]    __SELECT__ :
*  @arg\b       UART3_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p36 Tx p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_EasySetSM03(UART0_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT3_EasySetSM03(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CON_Page);\
        ((__SELECT__^UART3_SELECT0)==OPTION_MATCH?(SM03=0):\
        (__SELECT__^UART3_SELECT1)==OPTION_MATCH?(SM03=0):\
        (__SELECT__^UART3_SELECT2)==OPTION_MATCH?(SM03=0):\
        (__SELECT__^UART3_SELECT3)==OPTION_MATCH?(SM03=0):\
        (__SELECT__^UART3_SELECT4)==OPTION_MATCH?(SM03=0):\
        (__SELECT__^UART3_SELECT5)==OPTION_MATCH?(SM03=0):\
        (__SELECT__^UART3_SELECT6)==OPTION_MATCH?(SM03=0):\
        (__SELECT__^UART3_SELECT7)==OPTION_MATCH?(SM03=0):\
        (__SELECT__^UART3_SELECT8)==OPTION_MATCH?(SM03=0):\
        (__SELECT__^UART3_SELECT9)==OPTION_MATCH?(SM03=0):\
        (__SELECT__^UART3_SELECT10)==OPTION_MATCH?(SM03=0):\
        (__SELECT__^UART3_SELECT11)==OPTION_MATCH?(SM03=0):\
        (__SELECT__^UART3_SELECT12)==OPTION_MATCH?(SM03=0):\
        (__SELECT__^UART3_SELECT13)==OPTION_MATCH?(SM03=0):\
        (__SELECT__^UART3_SELECT14)==OPTION_MATCH?(SM03=0):\
        (__SELECT__^UART3_SELECT15)==OPTION_MATCH?(SM03=0):\
        (__SELECT__^UART3_SELECT16)==OPTION_MATCH?(SM03=0):\
        (__SELECT__^UART3_SELECT17)==OPTION_MATCH?(SM03=0):\
        (__SELECT__^UART3_SELECT18)==OPTION_MATCH?(SM03=0):\
        (__SELECT__^UART3_SELECT19)==OPTION_MATCH?(SM03=0):\
        (__SELECT__^UART3_SELECT20)==OPTION_MATCH?(SM03=0):\
        (__SELECT__^UART3_SELECT21)==OPTION_MATCH?(SM03=0):\
        (__SELECT__^UART3_SELECT22)==OPTION_MATCH?(SM03=0):\
        (__SELECT__^UART3_SELECT23)==OPTION_MATCH?(SM03=0):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)


/**
*******************************************************************************
* @brief       UART3 Mode Select
* @details     Set SM13
* @param[in]    __SELECT__ :
*  @arg\b       UART3_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p36 Tx p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_EasySetSM13(UART3_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/

#define __DRV_URT3_EasySetSM13(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CON_Page);\
        ((__SELECT__^UART3_SELECT0)==OPTION_MATCH?(SM13=1):\
        (__SELECT__^UART3_SELECT1)==OPTION_MATCH?(SM13=1):\
        (__SELECT__^UART3_SELECT2)==OPTION_MATCH?(SM13=1):\
        (__SELECT__^UART3_SELECT3)==OPTION_MATCH?(SM13=1):\
        (__SELECT__^UART3_SELECT4)==OPTION_MATCH?(SM13=1):\
        (__SELECT__^UART3_SELECT5)==OPTION_MATCH?(SM13=1):\
        (__SELECT__^UART3_SELECT6)==OPTION_MATCH?(SM13=1):\
        (__SELECT__^UART3_SELECT7)==OPTION_MATCH?(SM13=1):\
        (__SELECT__^UART3_SELECT8)==OPTION_MATCH?(SM13=1):\
        (__SELECT__^UART3_SELECT9)==OPTION_MATCH?(SM13=1):\
        (__SELECT__^UART3_SELECT10)==OPTION_MATCH?(SM13=1):\
        (__SELECT__^UART3_SELECT11)==OPTION_MATCH?(SM13=1):\
        (__SELECT__^UART3_SELECT12)==OPTION_MATCH?(SM13=1):\
        (__SELECT__^UART3_SELECT13)==OPTION_MATCH?(SM13=1):\
        (__SELECT__^UART3_SELECT14)==OPTION_MATCH?(SM13=1):\
        (__SELECT__^UART3_SELECT15)==OPTION_MATCH?(SM13=1):\
        (__SELECT__^UART3_SELECT16)==OPTION_MATCH?(SM13=1):\
        (__SELECT__^UART3_SELECT17)==OPTION_MATCH?(SM13=1):\
        (__SELECT__^UART3_SELECT18)==OPTION_MATCH?(SM13=1):\
        (__SELECT__^UART3_SELECT19)==OPTION_MATCH?(SM13=1):\
        (__SELECT__^UART3_SELECT20)==OPTION_MATCH?(SM13=1):\
        (__SELECT__^UART3_SELECT21)==OPTION_MATCH?(SM13=1):\
        (__SELECT__^UART3_SELECT22)==OPTION_MATCH?(SM13=1):\
        (__SELECT__^UART3_SELECT23)==OPTION_MATCH?(SM13=1):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief       UART3 S3BRG Clock Source
* @details     Set S3TX12
* @param[in]    __SELECT__ :
*  @arg\b       UART3_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p36 Tx p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_EasySetS3TX12(UART3_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT3_EasySetS3TX12(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CFG_Page);\
        ((__SELECT__^UART3_SELECT0)==OPTION_MATCH?(S3CFG=S3CFG|(S3TX12)):\
        (__SELECT__^UART3_SELECT1)==OPTION_MATCH?(S3CFG=S3CFG|(S3TX12)):\
        (__SELECT__^UART3_SELECT2)==OPTION_MATCH?(S3CFG=S3CFG|(S3TX12)):\
        (__SELECT__^UART3_SELECT3)==OPTION_MATCH?(S3CFG=S3CFG|(S3TX12)):\
        (__SELECT__^UART3_SELECT4)==OPTION_MATCH?(S3CFG=S3CFG|(S3TX12)):\
        (__SELECT__^UART3_SELECT5)==OPTION_MATCH?(S3CFG=S3CFG|(S3TX12)):\
        (__SELECT__^UART3_SELECT6)==OPTION_MATCH?(S3CFG=S3CFG|(S3TX12)):\
        (__SELECT__^UART3_SELECT7)==OPTION_MATCH?(S3CFG=S3CFG|(S3TX12)):\
        (__SELECT__^UART3_SELECT8)==OPTION_MATCH?(S3CFG=S3CFG|(S3TX12)):\
        (__SELECT__^UART3_SELECT9)==OPTION_MATCH?(S3CFG=S3CFG|(S3TX12)):\
        (__SELECT__^UART3_SELECT10)==OPTION_MATCH?(S3CFG=S3CFG|(S3TX12)):\
        (__SELECT__^UART3_SELECT11)==OPTION_MATCH?(S3CFG=S3CFG|(S3TX12)):\
        (__SELECT__^UART3_SELECT12)==OPTION_MATCH?(S3CFG=S3CFG|(S3TX12)):\
        (__SELECT__^UART3_SELECT13)==OPTION_MATCH?(S3CFG=S3CFG|(S3TX12)):\
        (__SELECT__^UART3_SELECT14)==OPTION_MATCH?(S3CFG=S3CFG|(S3TX12)):\
        (__SELECT__^UART3_SELECT15)==OPTION_MATCH?(S3CFG=S3CFG|(S3TX12)):\
        (__SELECT__^UART3_SELECT16)==OPTION_MATCH?(S3CFG=S3CFG|(S3TX12)):\
        (__SELECT__^UART3_SELECT17)==OPTION_MATCH?(S3CFG=S3CFG|(S3TX12)):\
        (__SELECT__^UART3_SELECT18)==OPTION_MATCH?(S3CFG=S3CFG|(S3TX12)):\
        (__SELECT__^UART3_SELECT19)==OPTION_MATCH?(S3CFG=S3CFG|(S3TX12)):\
        (__SELECT__^UART3_SELECT20)==OPTION_MATCH?(S3CFG=S3CFG|(S3TX12)):\
        (__SELECT__^UART3_SELECT21)==OPTION_MATCH?(S3CFG=S3CFG|(S3TX12)):\
        (__SELECT__^UART3_SELECT22)==OPTION_MATCH?(S3CFG=S3CFG|(S3TX12)):\
        (__SELECT__^UART3_SELECT23)==OPTION_MATCH?(S3CFG=S3CFG|(S3TX12)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief       UART3 Data Order
* @details     Set S3DOR
* @param[in]    __SELECT__ :
*  @arg\b       UART3_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p36 Tx p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_EasySetS3DOR(UART3_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/

#define __DRV_URT3_EasySetS3DOR(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CFG_Page);\
        ((__SELECT__^UART3_SELECT0)==OPTION_MATCH?(S3CFG=S3CFG|(S3DOR)):\
        (__SELECT__^UART3_SELECT1)==OPTION_MATCH?(S3CFG=S3CFG&(~S3DOR)):\
        (__SELECT__^UART3_SELECT2)==OPTION_MATCH?(S3CFG=S3CFG|(S3DOR)):\
        (__SELECT__^UART3_SELECT3)==OPTION_MATCH?(S3CFG=S3CFG&(~S3DOR)):\
        (__SELECT__^UART3_SELECT4)==OPTION_MATCH?(S3CFG=S3CFG|(S3DOR)):\
        (__SELECT__^UART3_SELECT5)==OPTION_MATCH?(S3CFG=S3CFG&(~S3DOR)):\
        (__SELECT__^UART3_SELECT6)==OPTION_MATCH?(S3CFG=S3CFG|(S3DOR)):\
        (__SELECT__^UART3_SELECT7)==OPTION_MATCH?(S3CFG=S3CFG&(~S3DOR)):\
        (__SELECT__^UART3_SELECT8)==OPTION_MATCH?(S3CFG=S3CFG|(S3DOR)):\
        (__SELECT__^UART3_SELECT9)==OPTION_MATCH?(S3CFG=S3CFG&(~S3DOR)):\
        (__SELECT__^UART3_SELECT10)==OPTION_MATCH?(S3CFG=S3CFG|(S3DOR)):\
        (__SELECT__^UART3_SELECT11)==OPTION_MATCH?(S3CFG=S3CFG&(~S3DOR)):\
        (__SELECT__^UART3_SELECT12)==OPTION_MATCH?(S3CFG=S3CFG|(S3DOR)):\
        (__SELECT__^UART3_SELECT13)==OPTION_MATCH?(S3CFG=S3CFG&(~S3DOR)):\
        (__SELECT__^UART3_SELECT14)==OPTION_MATCH?(S3CFG=S3CFG|(S3DOR)):\
        (__SELECT__^UART3_SELECT15)==OPTION_MATCH?(S3CFG=S3CFG&(~S3DOR)):\
        (__SELECT__^UART3_SELECT16)==OPTION_MATCH?(S3CFG=S3CFG|(S3DOR)):\
        (__SELECT__^UART3_SELECT17)==OPTION_MATCH?(S3CFG=S3CFG&(~S3DOR)):\
        (__SELECT__^UART3_SELECT18)==OPTION_MATCH?(S3CFG=S3CFG|(S3DOR)):\
        (__SELECT__^UART3_SELECT19)==OPTION_MATCH?(S3CFG=S3CFG&(~S3DOR)):\
        (__SELECT__^UART3_SELECT20)==OPTION_MATCH?(S3CFG=S3CFG|(S3DOR)):\
        (__SELECT__^UART3_SELECT21)==OPTION_MATCH?(S3CFG=S3CFG&(~S3DOR)):\
        (__SELECT__^UART3_SELECT22)==OPTION_MATCH?(S3CFG=S3CFG|(S3DOR)):\
        (__SELECT__^UART3_SELECT23)==OPTION_MATCH?(S3CFG=S3CFG&(~S3DOR)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief       UART3 Enhance Baud Rate
* @details     Set S3MOD1
* @param[in]    __SELECT__ :
*  @arg\b       UART3_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p36 Tx p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_EasySetS3MOD1(UART3_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT3_EasySetS3MOD1(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CFG_Page);\
        ((__SELECT__^UART3_SELECT0)==OPTION_MATCH?(S3CFG=S3CFG|(S3MOD1)):\
        (__SELECT__^UART3_SELECT1)==OPTION_MATCH?(S3CFG=S3CFG|(S3MOD1)):\
        (__SELECT__^UART3_SELECT2)==OPTION_MATCH?(S3CFG=S3CFG|(S3MOD1)):\
        (__SELECT__^UART3_SELECT3)==OPTION_MATCH?(S3CFG=S3CFG|(S3MOD1)):\
        (__SELECT__^UART3_SELECT4)==OPTION_MATCH?(S3CFG=S3CFG|(S3MOD1)):\
        (__SELECT__^UART3_SELECT5)==OPTION_MATCH?(S3CFG=S3CFG|(S3MOD1)):\
        (__SELECT__^UART3_SELECT6)==OPTION_MATCH?(S3CFG=S3CFG&(~S3MOD1)):\
        (__SELECT__^UART3_SELECT7)==OPTION_MATCH?(S3CFG=S3CFG&(~S3MOD1)):\
        (__SELECT__^UART3_SELECT8)==OPTION_MATCH?(S3CFG=S3CFG|(S3MOD1)):\
        (__SELECT__^UART3_SELECT9)==OPTION_MATCH?(S3CFG=S3CFG|(S3MOD1)):\
        (__SELECT__^UART3_SELECT10)==OPTION_MATCH?(S3CFG=S3CFG&(~S3MOD1)):\
        (__SELECT__^UART3_SELECT11)==OPTION_MATCH?(S3CFG=S3CFG&(~S3MOD1)):\
        (__SELECT__^UART3_SELECT12)==OPTION_MATCH?(S3CFG=S3CFG|(S3MOD1)):\
        (__SELECT__^UART3_SELECT13)==OPTION_MATCH?(S3CFG=S3CFG|(S3MOD1)):\
        (__SELECT__^UART3_SELECT14)==OPTION_MATCH?(S3CFG=S3CFG&(~S3MOD1)):\
        (__SELECT__^UART3_SELECT15)==OPTION_MATCH?(S3CFG=S3CFG&(~S3MOD1)):\
        (__SELECT__^UART3_SELECT16)==OPTION_MATCH?(S3CFG=S3CFG|(S3MOD1)):\
        (__SELECT__^UART3_SELECT17)==OPTION_MATCH?(S3CFG=S3CFG|(S3MOD1)):\
        (__SELECT__^UART3_SELECT18)==OPTION_MATCH?(S3CFG=S3CFG|(S3MOD1)):\
        (__SELECT__^UART3_SELECT19)==OPTION_MATCH?(S3CFG=S3CFG|(S3MOD1)):\
        (__SELECT__^UART3_SELECT20)==OPTION_MATCH?(S3CFG=S3CFG|(S3MOD1)):\
        (__SELECT__^UART3_SELECT21)==OPTION_MATCH?(S3CFG=S3CFG|(S3MOD1)):\
        (__SELECT__^UART3_SELECT22)==OPTION_MATCH?(S3CFG=S3CFG|(S3MOD1)):\
        (__SELECT__^UART3_SELECT23)==OPTION_MATCH?(S3CFG=S3CFG|(S3MOD1)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)



/**
*******************************************************************************
* @brief      UART3  Baud Rate Generator Reload Register
* @details    Write S3BRT
* @param[in]    __SELECT__ :
*  @arg\b       UART3_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p36 Tx p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_EasySetS3BRT(UART3_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT3_EasySetS3BRT
#define S3BRG_BRGRL_2400_1X_12000000_12T            0xF3        // 0.16%
#define S3BRG_BRGRL_2400_1X_12000000_1T             0x64        // 0.16%
#define S3BRG_BRGRL_4800_1X_12000000_1T             0xB2        // 0.16%
#define S3BRG_BRGRL_9600_1X_12000000_1T             0xD9        // 0.16%
#define S3BRG_BRGRL_19200_1X_12000000_1T            0xEC        // -2.34%
#define S3BRG_BRGRL_38400_1X_12000000_1T            0xF6        // -2.34%
#define S3BRG_BRGRL_2400_2X_12000000_12T            0xE6        // 0.16%
#define S3BRG_BRGRL_4800_2X_12000000_12T            0xF3        // 0.16%
#define S3BRG_BRGRL_4800_2X_12000000_1T             0x64        // 0.16%
#define S3BRG_BRGRL_9600_2X_12000000_1T             0xB2        // 0.16%
#define S3BRG_BRGRL_19200_2X_12000000_1T            0xD9        // 0.16%
#define S3BRG_BRGRL_38400_2X_12000000_1T            0xEC        // -2.34%
#define S3BRG_BRGRL_57600_2X_12000000_1T            0xF3        // 0.16%
#define S3BRG_BRGRL_115200_2X_11059200_1T           0xFA        // 0%
#define S3BRG_BRGRL_115200_1X_11059200_1T           0xFD        // 0%
#define S3BRG_BRGRL_57600_1X_11059200_1T            0xFA        // 0%
#define S3BRG_BRGRL_230400_2X_11059200_1T           0xFD        // 0%
#define S3BRG_BRGRL_250000_2X_12000000_1T           0xFD        // 0%
#define S3BRG_BRGRL_750000_2X_12000000_1T           0xFF        // 0%
///@endcond

#define __DRV_URT3_EasySetS3BRT(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S3BRT_Page);\
        ((__SELECT__^UART3_SELECT0)==OPTION_MATCH?(S3BRT=S3BRG_BRGRL_4800_2X_12000000_1T):\
        (__SELECT__^UART3_SELECT1)==OPTION_MATCH?(S3BRT=S3BRG_BRGRL_4800_2X_12000000_1T):\
        (__SELECT__^UART3_SELECT2)==OPTION_MATCH?(S3BRT=S3BRG_BRGRL_9600_2X_12000000_1T):\
        (__SELECT__^UART3_SELECT3)==OPTION_MATCH?(S3BRT=S3BRG_BRGRL_9600_2X_12000000_1T):\
        (__SELECT__^UART3_SELECT4)==OPTION_MATCH?(S3BRT=S3BRG_BRGRL_19200_2X_12000000_1T):\
        (__SELECT__^UART3_SELECT5)==OPTION_MATCH?(S3BRT=S3BRG_BRGRL_19200_2X_12000000_1T):\
        (__SELECT__^UART3_SELECT6)==OPTION_MATCH?(S3BRT=S3BRG_BRGRL_38400_1X_12000000_1T):\
        (__SELECT__^UART3_SELECT7)==OPTION_MATCH?(S3BRT=S3BRG_BRGRL_38400_1X_12000000_1T):\
        (__SELECT__^UART3_SELECT8)==OPTION_MATCH?(S3BRT=S3BRG_BRGRL_38400_2X_12000000_1T):\
        (__SELECT__^UART3_SELECT9)==OPTION_MATCH?(S3BRT=S3BRG_BRGRL_38400_2X_12000000_1T):\
        (__SELECT__^UART3_SELECT10)==OPTION_MATCH?(S3BRT=S3BRG_BRGRL_57600_1X_11059200_1T):\
        (__SELECT__^UART3_SELECT11)==OPTION_MATCH?(S3BRT=S3BRG_BRGRL_57600_1X_11059200_1T):\
        (__SELECT__^UART3_SELECT12)==OPTION_MATCH?(S3BRT=S3BRG_BRGRL_57600_2X_12000000_1T):\
        (__SELECT__^UART3_SELECT13)==OPTION_MATCH?(S3BRT=S3BRG_BRGRL_57600_2X_12000000_1T):\
        (__SELECT__^UART3_SELECT14)==OPTION_MATCH?(S3BRT=S3BRG_BRGRL_115200_1X_11059200_1T):\
        (__SELECT__^UART3_SELECT15)==OPTION_MATCH?(S3BRT=S3BRG_BRGRL_115200_1X_11059200_1T):\
        (__SELECT__^UART3_SELECT16)==OPTION_MATCH?(S3BRT=S3BRG_BRGRL_115200_2X_11059200_1T):\
        (__SELECT__^UART3_SELECT17)==OPTION_MATCH?(S3BRT=S3BRG_BRGRL_115200_2X_11059200_1T):\
        (__SELECT__^UART3_SELECT18)==OPTION_MATCH?(S3BRT=S3BRG_BRGRL_230400_2X_11059200_1T):\
        (__SELECT__^UART3_SELECT19)==OPTION_MATCH?(S3BRT=S3BRG_BRGRL_230400_2X_11059200_1T):\
        (__SELECT__^UART3_SELECT20)==OPTION_MATCH?(S3BRT=S3BRG_BRGRL_250000_2X_12000000_1T):\
        (__SELECT__^UART3_SELECT21)==OPTION_MATCH?(S3BRT=S3BRG_BRGRL_250000_2X_12000000_1T):\
        (__SELECT__^UART3_SELECT22)==OPTION_MATCH?(S3BRT=S3BRG_BRGRL_750000_2X_12000000_1T):\
        (__SELECT__^UART3_SELECT23)==OPTION_MATCH?(S3BRT=S3BRG_BRGRL_750000_2X_12000000_1T):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)


/**
*******************************************************************************
* @brief      UART3  Baud Rate Generator Reload Register
* @details    Write S3BRC
* @param[in]    __SELECT__ :
*  @arg\b       UART3_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p36 Tx p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_EasySetS3BRC(UART3_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT3_EasySetS3BRC(__SELECT__)\
    MWT(\
         __DRV_SFR_PageIndex(S3BRC_Page);\
         ((__SELECT__^UART3_SELECT0)==OPTION_MATCH?(S3BRC=S3BRG_BRGRL_4800_2X_12000000_1T):\
         (__SELECT__^UART3_SELECT1)==OPTION_MATCH?(S3BRC=S3BRG_BRGRL_4800_2X_12000000_1T):\
         (__SELECT__^UART3_SELECT2)==OPTION_MATCH?(S3BRC=S3BRG_BRGRL_9600_2X_12000000_1T):\
         (__SELECT__^UART3_SELECT3)==OPTION_MATCH?(S3BRC=S3BRG_BRGRL_9600_2X_12000000_1T):\
         (__SELECT__^UART3_SELECT4)==OPTION_MATCH?(S3BRC=S3BRG_BRGRL_19200_2X_12000000_1T):\
         (__SELECT__^UART3_SELECT5)==OPTION_MATCH?(S3BRC=S3BRG_BRGRL_19200_2X_12000000_1T):\
         (__SELECT__^UART3_SELECT6)==OPTION_MATCH?(S3BRC=S3BRG_BRGRL_38400_1X_12000000_1T):\
         (__SELECT__^UART3_SELECT7)==OPTION_MATCH?(S3BRC=S3BRG_BRGRL_38400_1X_12000000_1T):\
         (__SELECT__^UART3_SELECT8)==OPTION_MATCH?(S3BRC=S3BRG_BRGRL_38400_2X_12000000_1T):\
         (__SELECT__^UART3_SELECT9)==OPTION_MATCH?(S3BRC=S3BRG_BRGRL_38400_2X_12000000_1T):\
         (__SELECT__^UART3_SELECT10)==OPTION_MATCH?(S3BRC=S3BRG_BRGRL_57600_1X_11059200_1T):\
         (__SELECT__^UART3_SELECT11)==OPTION_MATCH?(S3BRC=S3BRG_BRGRL_57600_1X_11059200_1T):\
         (__SELECT__^UART3_SELECT12)==OPTION_MATCH?(S3BRC=S3BRG_BRGRL_57600_2X_12000000_1T):\
         (__SELECT__^UART3_SELECT13)==OPTION_MATCH?(S3BRC=S3BRG_BRGRL_57600_2X_12000000_1T):\
         (__SELECT__^UART3_SELECT14)==OPTION_MATCH?(S3BRC=S3BRG_BRGRL_115200_1X_11059200_1T):\
         (__SELECT__^UART3_SELECT15)==OPTION_MATCH?(S3BRC=S3BRG_BRGRL_115200_1X_11059200_1T):\
         (__SELECT__^UART3_SELECT16)==OPTION_MATCH?(S3BRC=S3BRG_BRGRL_115200_2X_11059200_1T):\
         (__SELECT__^UART3_SELECT17)==OPTION_MATCH?(S3BRC=S3BRG_BRGRL_115200_2X_11059200_1T):\
         (__SELECT__^UART3_SELECT18)==OPTION_MATCH?(S3BRC=S3BRG_BRGRL_230400_2X_11059200_1T):\
         (__SELECT__^UART3_SELECT19)==OPTION_MATCH?(S3BRC=S3BRG_BRGRL_230400_2X_11059200_1T):\
         (__SELECT__^UART3_SELECT20)==OPTION_MATCH?(S3BRC=S3BRG_BRGRL_250000_2X_12000000_1T):\
         (__SELECT__^UART3_SELECT21)==OPTION_MATCH?(S3BRC=S3BRG_BRGRL_250000_2X_12000000_1T):\
         (__SELECT__^UART3_SELECT22)==OPTION_MATCH?(S3BRC=S3BRG_BRGRL_750000_2X_12000000_1T):\
         (__SELECT__^UART3_SELECT23)==OPTION_MATCH?(S3BRC=S3BRG_BRGRL_750000_2X_12000000_1T):_nop_());\
         __DRV_SFR_PageIndex(0);\
     ;)



/**
*******************************************************************************
* @brief       UART3 S3BRG Operation Enable
* @details     Set S3TR
* @param[in]    __SELECT__ :
*  @arg\b       UART3_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p36 Tx p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_EasySetS3TR_Start(UART3_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT3_EasySetS3TR_Start(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CFG_Page);\
        ((__SELECT__^UART3_SELECT0)==OPTION_MATCH?(S3CFG=S3CFG|(S3TR)):\
        (__SELECT__^UART3_SELECT1)==OPTION_MATCH?(S3CFG=S3CFG|(S3TR)):\
        (__SELECT__^UART3_SELECT2)==OPTION_MATCH?(S3CFG=S3CFG|(S3TR)):\
        (__SELECT__^UART3_SELECT3)==OPTION_MATCH?(S3CFG=S3CFG|(S3TR)):\
        (__SELECT__^UART3_SELECT4)==OPTION_MATCH?(S3CFG=S3CFG|(S3TR)):\
        (__SELECT__^UART3_SELECT5)==OPTION_MATCH?(S3CFG=S3CFG|(S3TR)):\
        (__SELECT__^UART3_SELECT6)==OPTION_MATCH?(S3CFG=S3CFG|(S3TR)):\
        (__SELECT__^UART3_SELECT7)==OPTION_MATCH?(S3CFG=S3CFG|(S3TR)):\
        (__SELECT__^UART3_SELECT8)==OPTION_MATCH?(S3CFG=S3CFG|(S3TR)):\
        (__SELECT__^UART3_SELECT9)==OPTION_MATCH?(S3CFG=S3CFG|(S3TR)):\
        (__SELECT__^UART3_SELECT10)==OPTION_MATCH?(S3CFG=S3CFG|(S3TR)):\
        (__SELECT__^UART3_SELECT11)==OPTION_MATCH?(S3CFG=S3CFG|(S3TR)):\
        (__SELECT__^UART3_SELECT12)==OPTION_MATCH?(S3CFG=S3CFG|(S3TR)):\
        (__SELECT__^UART3_SELECT13)==OPTION_MATCH?(S3CFG=S3CFG|(S3TR)):\
        (__SELECT__^UART3_SELECT14)==OPTION_MATCH?(S3CFG=S3CFG|(S3TR)):\
        (__SELECT__^UART3_SELECT15)==OPTION_MATCH?(S3CFG=S3CFG|(S3TR)):\
        (__SELECT__^UART3_SELECT16)==OPTION_MATCH?(S3CFG=S3CFG|(S3TR)):\
        (__SELECT__^UART3_SELECT17)==OPTION_MATCH?(S3CFG=S3CFG|(S3TR)):\
        (__SELECT__^UART3_SELECT18)==OPTION_MATCH?(S3CFG=S3CFG|(S3TR)):\
        (__SELECT__^UART3_SELECT19)==OPTION_MATCH?(S3CFG=S3CFG|(S3TR)):\
        (__SELECT__^UART3_SELECT20)==OPTION_MATCH?(S3CFG=S3CFG|(S3TR)):\
        (__SELECT__^UART3_SELECT21)==OPTION_MATCH?(S3CFG=S3CFG|(S3TR)):\
        (__SELECT__^UART3_SELECT22)==OPTION_MATCH?(S3CFG=S3CFG|(S3TR)):\
        (__SELECT__^UART3_SELECT23)==OPTION_MATCH?(S3CFG=S3CFG|(S3TR)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief       UART3 S3BRG Operation Disable
* @details     Set S3TR
* @param[in]    __SELECT__ :
*  @arg\b       UART3_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p36 Tx p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_EasySetS3TR_Stop(UART3_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/


#define __DRV_URT3_EasySetS3TR_Stop(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CFG_Page);\
        ((__SELECT__^UART3_SELECT0)==OPTION_MATCH?(S3CFG=S3CFG&(~S3TR)):\
        (__SELECT__^UART3_SELECT1)==OPTION_MATCH?(S3CFG=S3CFG&(~S3TR)):\
        (__SELECT__^UART3_SELECT2)==OPTION_MATCH?(S3CFG=S3CFG&(~S3TR)):\
        (__SELECT__^UART3_SELECT3)==OPTION_MATCH?(S3CFG=S3CFG&(~S3TR)):\
        (__SELECT__^UART3_SELECT4)==OPTION_MATCH?(S3CFG=S3CFG&(~S3TR)):\
        (__SELECT__^UART3_SELECT5)==OPTION_MATCH?(S3CFG=S3CFG&(~S3TR)):\
        (__SELECT__^UART3_SELECT6)==OPTION_MATCH?(S3CFG=S3CFG&(~S3TR)):\
        (__SELECT__^UART3_SELECT7)==OPTION_MATCH?(S3CFG=S3CFG&(~S3TR)):\
        (__SELECT__^UART3_SELECT8)==OPTION_MATCH?(S3CFG=S3CFG&(~S3TR)):\
        (__SELECT__^UART3_SELECT9)==OPTION_MATCH?(S3CFG=S3CFG&(~S3TR)):\
        (__SELECT__^UART3_SELECT10)==OPTION_MATCH?(S3CFG=S3CFG&(~S3TR)):\
        (__SELECT__^UART3_SELECT11)==OPTION_MATCH?(S3CFG=S3CFG&(~S3TR)):\
        (__SELECT__^UART3_SELECT12)==OPTION_MATCH?(S3CFG=S3CFG&(~S3TR)):\
        (__SELECT__^UART3_SELECT13)==OPTION_MATCH?(S3CFG=S3CFG&(~S3TR)):\
        (__SELECT__^UART3_SELECT14)==OPTION_MATCH?(S3CFG=S3CFG&(~S3TR)):\
        (__SELECT__^UART3_SELECT15)==OPTION_MATCH?(S3CFG=S3CFG&(~S3TR)):\
        (__SELECT__^UART3_SELECT16)==OPTION_MATCH?(S3CFG=S3CFG&(~S3TR)):\
        (__SELECT__^UART3_SELECT17)==OPTION_MATCH?(S3CFG=S3CFG&(~S3TR)):\
        (__SELECT__^UART3_SELECT18)==OPTION_MATCH?(S3CFG=S3CFG&(~S3TR)):\
        (__SELECT__^UART3_SELECT19)==OPTION_MATCH?(S3CFG=S3CFG&(~S3TR)):\
        (__SELECT__^UART3_SELECT20)==OPTION_MATCH?(S3CFG=S3CFG&(~S3TR)):\
        (__SELECT__^UART3_SELECT21)==OPTION_MATCH?(S3CFG=S3CFG&(~S3TR)):\
        (__SELECT__^UART3_SELECT22)==OPTION_MATCH?(S3CFG=S3CFG&(~S3TR)):\
        (__SELECT__^UART3_SELECT23)==OPTION_MATCH?(S3CFG=S3CFG&(~S3TR)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)


/**
*******************************************************************************
* @brief        UART3 Pin Config
* @details     Set S3PS0
* @param[in]    __SELECT__ :
*  @arg\b       UART3_8BIT_CONFIG0 : 4800 12MHz 8bit LSB Rx p36 Tx p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG1 : 4800 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG2 : 9600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG3 : 9600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG4 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG5 : 19200 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG6 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG7 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG8 : 38400 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG9 : 38400 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG10 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG11 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG12 : 57600 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG13 : 57600 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG14 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG15 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 default baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG16 : 115200 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG17 : 115200 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG18 : 230400 11.0592MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG19 : 230400 11.0592MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG20 : 250000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG21 : 250000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG22 : 750000 12MHz 8bit LSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
*  @arg\b       UART3_8BIT_CONFIG23 : 750000 12MHz 8bit MSB Rx p36 TX p37 Tx Rx both SYSCLK/1 double baud rate S3BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_EasySetS3PS0(UART3_8BIT_CONFIG0)
* @endcode
*******************************************************************************
*/

#define __DRV_URT3_EasySetS3PS0(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR13_Page);\
        ((__SELECT__^UART3_SELECT0)==OPTION_MATCH?(AUXR13=AUXR13&(~S3PS0)):\
        (__SELECT__^UART3_SELECT1)==OPTION_MATCH?(AUXR13=AUXR13&(~S3PS0)):\
        (__SELECT__^UART3_SELECT2)==OPTION_MATCH?(AUXR13=AUXR13&(~S3PS0)):\
        (__SELECT__^UART3_SELECT3)==OPTION_MATCH?(AUXR13=AUXR13&(~S3PS0)):\
        (__SELECT__^UART3_SELECT4)==OPTION_MATCH?(AUXR13=AUXR13&(~S3PS0)):\
        (__SELECT__^UART3_SELECT5)==OPTION_MATCH?(AUXR13=AUXR13&(~S3PS0)):\
        (__SELECT__^UART3_SELECT6)==OPTION_MATCH?(AUXR13=AUXR13&(~S3PS0)):\
        (__SELECT__^UART3_SELECT7)==OPTION_MATCH?(AUXR13=AUXR13&(~S3PS0)):\
        (__SELECT__^UART3_SELECT8)==OPTION_MATCH?(AUXR13=AUXR13&(~S3PS0)):\
        (__SELECT__^UART3_SELECT9)==OPTION_MATCH?(AUXR13=AUXR13&(~S3PS0)):\
        (__SELECT__^UART3_SELECT10)==OPTION_MATCH?(AUXR13=AUXR13&(~S3PS0)):\
        (__SELECT__^UART3_SELECT11)==OPTION_MATCH?(AUXR13=AUXR13&(~S3PS0)):\
        (__SELECT__^UART3_SELECT12)==OPTION_MATCH?(AUXR13=AUXR13&(~S3PS0)):\
        (__SELECT__^UART3_SELECT13)==OPTION_MATCH?(AUXR13=AUXR13&(~S3PS0)):\
        (__SELECT__^UART3_SELECT14)==OPTION_MATCH?(AUXR13=AUXR13&(~S3PS0)):\
        (__SELECT__^UART3_SELECT15)==OPTION_MATCH?(AUXR13=AUXR13&(~S3PS0)):\
        (__SELECT__^UART3_SELECT16)==OPTION_MATCH?(AUXR13=AUXR13&(~S3PS0)):\
        (__SELECT__^UART3_SELECT17)==OPTION_MATCH?(AUXR13=AUXR13&(~S3PS0)):\
        (__SELECT__^UART3_SELECT18)==OPTION_MATCH?(AUXR13=AUXR13&(~S3PS0)):\
        (__SELECT__^UART3_SELECT19)==OPTION_MATCH?(AUXR13=AUXR13&(~S3PS0)):\
        (__SELECT__^UART3_SELECT20)==OPTION_MATCH?(AUXR13=AUXR13&(~S3PS0)):\
        (__SELECT__^UART3_SELECT21)==OPTION_MATCH?(AUXR13=AUXR13&(~S3PS0)):\
        (__SELECT__^UART3_SELECT22)==OPTION_MATCH?(AUXR13=AUXR13&(~S3PS0)):\
        (__SELECT__^UART3_SELECT23)==OPTION_MATCH?(AUXR13=AUXR13&(~S3PS0)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)











#endif  //MG82F6D64_UART3_MID_H



