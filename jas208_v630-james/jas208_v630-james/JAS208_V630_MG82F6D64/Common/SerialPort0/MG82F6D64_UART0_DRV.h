/**
 ******************************************************************************
 *
 * @file        MG82F6D64_UART0_DRV.h
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

#ifndef MG82F6D64_UART0_DRV_H
#define MG82F6D64_UART0_DRV_H

/**
*****************************************************************************
* @brief        UART0 Enable Interrupt
* @details      UART0 Enable Interrupt
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_URT0_IT_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_IT_Cmd(__STATE__)\
    ((__STATE__==0)?(ES0=0):(ES0=1))

/**
*****************************************************************************
* @brief        UART0 Enable Block TI0
* @details      UART0 Enable Block Interrupt Flag TI0
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_URT0_Block_S0IntFlag_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_Block_S0IntFlag_Cmd(__STATE__)\
    ((__STATE__==0)?(S0CFG=S0CFG&(~BTI)):(S0CFG=S0CFG|BTI))
/**
*****************************************************************************
* @brief        UART0 Enable TI0 To System Flag Interrupt
* @details      UART0 Enable Interrupt Flag TI0 to System Flag Interrupt
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         The System Flag interrupt flags include: WDTF, BOF0, BOF1, RTCF, SPWF and MCDF in PCON1, TI0 in S0CON, BM0F and BM1F in AUXR0, STAF and STOF in AUXR2.
* @par          Example
* @code
                __DRV_URT0_TI0toSYSINT_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_TI0toSYSINT_Cmd(__STATE__)\
        ((__STATE__==0)?(S0CFG=S0CFG&(~UTIE)):(S0CFG=S0CFG|UTIE))
/**
*******************************************************************************
* @brief        UART0 Mode Select
* @details      Set SM30 SM00 SM10
* @param[in]    __MODE__ :
*  @arg\b       UART0_MODE0_SHIFT_REG
*  @arg\b       UART0_MODE1_8BIT
*  @arg\b       UART0_MODE2_9BIT
*  @arg\b       UART0_MODE3_9BIT
*  @arg\b       UART0_MODE4_SPI_MASTER
*  @arg\b       UART0_MODE5_LIN
* @return       None
* @note         None
* @par         Example
* @code
               __DRV_URT0_Mode_Select(UART0_Mode0_SHIFT_REG)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT0_Mode_Select
#define UART0_MODE0_SHIFT_REG 0x00
#define UART0_MODE1_8BIT 0x01
#define UART0_MODE2_9BIT 0x02
#define UART0_MODE3_9BIT 0x03
#define UART0_MODE4_SPI_MASTER 0x04
#define UART0_MODE5_LIN 0x05
///@endcond
#define __DRV_URT0_Mode_Select(__MODE__)\
    MWT(__DRV_URT0_SetSM30(__MODE__);\
        __DRV_URT0_SetSM00(__MODE__);\
        __DRV_URT0_SetSM10(__MODE__);\
    ;)
/**
*******************************************************************************
* @brief        UART0 Pin MUX Select
* @details      Set S0PS1 S0PS0
* @param[in]    __RXTX__ :
*  @arg\b       UART0_RX_P30_TX_P31
*  @arg\b       UART0_RX_P44_TX_P45
*  @arg\b       UART0_RX_P31_TX_P30
*  @arg\b       UART0_RX_P17_TX_P22
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_URT0_PinMux_Select(UART0_RX_P30_TX_P31)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT0_PinMux_Select
#define UART0_RX_P30_TX_P31 0x00
#define UART0_RX_P44_TX_P45 0x01
#define UART0_RX_P31_TX_P30 0x02
#define UART0_RX_P17_TX_P22 0x03
///@endcond
#define __DRV_URT0_PinMux_Select(__RXTX__)\
    MWT(\
        __DRV_URT0_SetS0PS1(__RXTX__);\
        __DRV_URT0_SetS0PS0(__RXTX__);\
    ;)
/**
*******************************************************************************
* @brief        UART01 S01MI Pin MUX Select
* @details      Set SnMIPS (UART0/1 Spi Master Input Pin Select )
* @param[in]    __RECEPTION__ :
*  @arg\b       UART0_S0MI_P62_UART1_S1MI_P63
*  @arg\b       UART0_S0MI_P57_UART1_S1MI_P67
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_URT01_S01MI_PinMux_Select(UART0_S0MI_P62_UART1_S1MI_P63)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT01_S01MI_PinMux_Select
#define UART0_S0MI_P62_UART1_S1MI_P63 0
#define UART0_S0MI_P57_UART1_S1MI_P67 1
///@endcond
#define __DRV_URT01_S01MI_PinMux_Select(__RECEPTION__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR6_Page);\
        __RECEPTION__==0?(AUXR6=AUXR6&(~SnMIPS)):(AUXR6=AUXR6|(SnMIPS));\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief        UART0 Lin TxRx Select
* @details      Set TXRX0 (UART0 Lin Bus Tx/Rx Select )
* @param[in]    __RXTX__ :
*  @arg\b       UART0_LIN_BUS_RX
*  @arg\b       UART0_LIN_BUS_TX
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_URT0_Lin_Bus_TxRx_Select(UART0_LIN_BUS_RX)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT0_Lin_Bus_TxRx_Select
#define UART0_LIN_BUS_RX 0
#define UART0_LIN_BUS_TX 1
///@endcond
#define __DRV_URT0_Lin_Bus_TxRx_Select(__RXTX__)\
    ((__RXTX__)==0?(S0CFG1=S0CFG1&(~TXRX0)):(S0CFG1=S0CFG1|(TXRX0)))
/**
*******************************************************************************
* @brief        UART0 Lin break length Select
* @details      Set S0SB16 (UART0 Lin Bus Break Length Select )
* @param[in]    __LENGTH__ :
*  @arg\b       UART0_LIN_BUS_BREAK13
*  @arg\b       UART0_LIN_BUS_BREAK16
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_URT0_Lin_Bus_BreakLength_Select(UART0_LIN_BUS_BREAK13)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT0_Break_Length_Select
#define UART0_LIN_BUS_BREAK13 0
#define UART0_LIN_BUS_BREAK16 1
///@endcond
#define __DRV_URT0_Lin_Bus_BreakLength_Select(__LENGTH__)\
    ((__LENGTH__)==0?(S0CFG1=S0CFG1&(~S0SB16)):(S0CFG1=S0CFG1|(S0SB16)))
/**
*******************************************************************************
* @brief        UART0 Baud Rate Select
* @details      Set SMOD2 SMOD1
* @param[in]    __TIME__ :
*  @arg\b       UART0_DEFAULT_BAUD_RATE
*  @arg\b       UART0_DOUBLE_BAUD_RATE_X1
*  @arg\b       UART0_DOUBLE_BAUD_RATE_X2
*  @arg\b       UART0_DOUBLE_BAUD_RATE_X4
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_URT0_BaudRateX2X4_Select(UART0_Default_Baud_Rate)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT0_BaudRateX2X4_Select
#define UART0_DEFAULT_BAUD_RATE 0
#define UART0_DOUBLE_BAUD_RATE_X1 16
#define UART0_DOUBLE_BAUD_RATE_X2 32
#define UART0_DOUBLE_BAUD_RATE_X4 48
///@endcond
#define __DRV_URT0_BaudRateX2X4_Select(__TIME__)\
    MWT(\
        __DRV_URT0_SetSMOD2(__TIME__);\
        __DRV_URT0_SetSMOD1(__TIME__);\
    ;)
/**
*******************************************************************************
* @brief        UART0 Mode Select
* @details      Set SM30
* @param[in]    __MODE__ :
*  @arg\b       UART0_MODE0_SHIFT_REG
*  @arg\b       UART0_MODE1_8BIT
*  @arg\b       UART0_MODE2_9BIT
*  @arg\b       UART0_MODE3_9BIT
*  @arg\b       UART0_MODE4_SPI_MASTER
*  @arg\b       UART0_MODE5_LIN
* @return       None
* @note         None
* @par         Example
* @code
               __DRV_URT0_SetSM30(UART0_MODE1_8BIT)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_SetSM30(__MODE__)\
    MWT(\
        (__MODE__^0x00)==0?(S0CFG=S0CFG&(~SM30)):\
        (__MODE__^0x01)==0?(S0CFG=S0CFG&(~SM30)):\
        (__MODE__^0x02)==0?(S0CFG=S0CFG&(~SM30)):\
        (__MODE__^0x03)==0?(S0CFG=S0CFG&(~SM30)):\
        (__MODE__^0x04)==0?(S0CFG=S0CFG|(SM30)):\
        (__MODE__^0x05)==0?(S0CFG=S0CFG|(SM30)):_nop_();\
    ;)
/**
*******************************************************************************
* @brief        UART0 Mode Select
* @details      Set SM00
* @param[in]    __MODE__ :
*  @arg\b       UART0_MODE0_SHIFT_REG
*  @arg\b       UART0_MODE1_8BIT
*  @arg\b       UART0_MODE2_9BIT
*  @arg\b       UART0_MODE3_9BIT
*  @arg\b       UART0_MODE4_SPI_MASTER
*  @arg\b       UART0_MODE5_LIN :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_URT0_SetSM00(UART0_MODE2_9BIT)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_SetSM00(__MODE__)\
    MWT(\
        (__MODE__^0x00)==0?(SM00_FE=0):\
        (__MODE__^0x01)==0?(SM00_FE=0):\
        (__MODE__^0x02)==0?(SM00_FE=1):\
        (__MODE__^0x03)==0?(SM00_FE=1):\
        (__MODE__^0x04)==0?(SM00_FE=0):\
        (__MODE__^0x05)==0?(SM00_FE=0):_nop_();\
    ;)
/**
*******************************************************************************
* @brief        UART0 Mode Select
* @details      Set SM10
* @param[in]    __MODE__ :
*  @arg\b       UART0_MODE0_SHIFT_REG
*  @arg\b       UART0_MODE1_8BIT
*  @arg\b       UART0_MODE2_9BIT
*  @arg\b       UART0_MODE3_9BIT
*  @arg\b       UART0_MODE4_SPI_MASTER
*  @arg\b       UART0_MODE5_LIN
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_URT0_SetSM10(UART0_MODE3_9BIT)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_SetSM10(__MODE__)\
    MWT(\
        (__MODE__^0x00)==0?(SM10=0):\
        (__MODE__^0x01)==0?(SM10=1):\
        (__MODE__^0x02)==0?(SM10=0):\
        (__MODE__^0x03)==0?(SM10=1):\
        (__MODE__^0x04)==0?(SM10=0):\
        (__MODE__^0x05)==0?(SM10=1):_nop_();\
    ;)
/**
*******************************************************************************
* @brief        UART0 Pin MUX Select
* @details      Set S0PS1
* @param[in]    __RXTX__ :
*  @arg\b       UART0_RX_P30_TX_P31
*  @arg\b       UART0_RX_P44_TX_P45
*  @arg\b       UART0_RX_P31_TX_P30
*  @arg\b       UART0_RX_P17_TX_P22
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_URT0_SetS0PS1(UART0_RX_P30_TX_P31)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_SetS0PS1(__RXTX__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR10_Page);\
        (__RXTX__^0x00)==0?AUXR10=AUXR10&(~S0PS1):\
        (__RXTX__^0x01)==0?AUXR10=AUXR10&(~S0PS1):\
        (__RXTX__^0x02)==0?AUXR10=AUXR10|(S0PS1):\
        (__RXTX__^0x03)==0?AUXR10=AUXR10|(S0PS1):_nop_();\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief        UART0 Pin MUX Select
* @details      Set S0PS0
* @param[in]    __RXTX__ :
*  @arg\b       UART0_RX_P30_TX_P31
*  @arg\b       UART0_RX_P44_TX_P45
*  @arg\b       UART0_RX_P31_TX_P30
*  @arg\b       UART0_RX_P17_TX_P22
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_URT0_SetS0PS0(UART0_RX_P30_TX_P31)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_SetS0PS0(__RXTX__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR3_Page);\
        (__RXTX__^0x00)==0?AUXR3=AUXR3&(~S0PS0):\
        (__RXTX__^0x01)==0?AUXR3=AUXR3|(S0PS0):\
        (__RXTX__^0x02)==0?AUXR3=AUXR3&(~S0PS0):\
        (__RXTX__^0x03)==0?AUXR3=AUXR3|(S0PS0):_nop_();\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief        UART0 Data Order Select
* @details      Set S0DOR
* @param[in]    __ORDER__ :
*  @arg\b       UART0_DATA_ORDER_LSB
*  @arg\b       UART0_DATA_ORDER_MSB
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_URT0_DataOrder_Select(UART0_DATA_ORDER_LSB)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT0_DataOrder_Select
#define UART0_DATA_ORDER_LSB 0x08
#define UART0_DATA_ORDER_MSB 0x00
///@endcond
#define __DRV_URT0_DataOrder_Select(__ORDER__)\
    ((__ORDER__==0x08)?(S0CFG=S0CFG|(S0DOR)):(S0CFG=S0CFG&(~S0DOR)))
/**
*******************************************************************************
* @brief        UART0 S0BRG Timer Mode Select
* @details      Set S0DOR
* @param[in]    __MODE__ :
*  @arg\b       UART0_8BIT_TIMER_MODE
*  @arg\b       UART0_16BIT_TIMER_MODE
* @return       None
* @note         S0DOR=1 S0TCK=1 S0RCK=1 SM30=0 SM00=1 SM00=0 8BIT TIMER MODE
* @par          Example
* @code
                __DRV_URT0_TimerMode_Select(S0BRG_8BIT_TIMER_MODE)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT0_TimerMode_Select
#define UART0_8BIT_TIMER_MODE 1
#define UART0_16BIT_TIMER_MODE 0
///@endcond
#define __DRV_URT0_TimerMode_Select(__MODE__)\
    ((__MODE__==1)?(S0CFG=S0CFG|(S0DOR)):(S0CFG=S0CFG&(~S0DOR)))
/**
*******************************************************************************
* @brief        UART0 Baud Rate Select
* @details      Set SMOD2
* @param[in]   __TIME__ :
*  @arg\b       UART0_DEFAULT_BAUD_RATE
*  @arg\b       UART0_DOUBLE_BAUD_RATE_X1
*  @arg\b       UART0_DOUBLE_BAUD_RATE_X2
*  @arg\b       UART0_DOUBLE_BAUD_RATE_X4
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_URT0_SetSMOD2(UART0_Double_Baud_Rate_X1)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_SetSMOD2(__TIME__)\
    MWT(\
        (__TIME__^0)==0?S0CFG=S0CFG&(~SMOD2):\
        (__TIME__^16)==0?S0CFG=S0CFG&(~SMOD2):\
        (__TIME__^32)==0?S0CFG=S0CFG|(SMOD2):\
        (__TIME__^48)==0?S0CFG=S0CFG|(SMOD2):_nop_();\
    ;)
/**
*******************************************************************************
* @brief       UART0 Baud Rate Select
* @details     Set SMOD1
* @param[in]   __TIME__ :
*  @arg\b      UART0_DEFAULT_BAUD_RATE
*  @arg\b      UART0_DOUBLE_BAUD_RATE_X1
*  @arg\b      UART0_DOUBLE_BAUD_RATE_X2
*  @arg\b      UART0_DOUBLE_BAUD_RATE_X4
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_SetSMOD1(UART0_DOUBLE_BAUD_RATE_X2)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_SetSMOD1(__TIME__)\
    MWT(\
        (__TIME__^0)==0?PCON0=PCON0&(~SMOD1):\
        (__TIME__^16)==0?PCON0=PCON0|(SMOD1):\
        (__TIME__^32)==0?PCON0=PCON0&(~SMOD1):\
        (__TIME__^48)==0?PCON0=PCON0|(SMOD1):_nop_();\
    ;)
/**
*******************************************************************************
* @brief       UART0 Baud Rate Select
* @details     Set SMOD2
* @param[in]   __TIME__ :
*  @arg\b      UART0_DEFAULT_BAUD_RATE
*  @arg\b      UART0_DOUBLE_BAUD_RATE_X1
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_BaudRateX2_Select(UART0_DEFAULT_BAUD_RATE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_BaudRateX2_Select(__TIME__)\
    ((__TIME__^16)==0?(S0CFG=S0CFG|(SMOD2)):(S0CFG=S0CFG&(~SMOD2)))
/**
*******************************************************************************
* @brief       UART0 S0BRG Clock Source Select
* @details     Set S0TX12
* @param[in]   __SOURCE__ :
*  @arg\b      S0BRG_CLOCK_SOURCE_SYSCLK_DIV_1
*  @arg\b      S0BRG_CLOCK_SOURCE_SYSCLK_DIV_12
* @return      None
* @note        Before set S0TX12  Set SMOD3=1 first
* @par         Example
* @code
__DRV_URT0_BaudRateGeneratorClock_Select(S0BRG_CLOCK_SOURCE_SYSCLK_DIV_1)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT0_BaudRateGeneratorClock_Select
#define S0BRG_CLOCK_SOURCE_SYSCLK_DIV_1 64
#define S0BRG_CLOCK_SOURCE_SYSCLK_DIV_12 0
///@endcond
#define __DRV_URT0_BaudRateGeneratorClock_Select(__SOURCE__)\
    ((__SOURCE__==0)?(S0CR1=S0CR1&(~S0TX12)):(S0CR1=S0CR1|(S0TX12)))
/**
*******************************************************************************
* @brief       UART0 Access S0CR1
* @details     Set SMOD3
* @param[in]   __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_S0Control_Select(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_S0Control_Cmd(__STATE__)\
    ((__STATE__==1)?(S0CFG=S0CFG|(SMOD3)):(S0CFG=S0CFG&(~SMOD3)))
/**
*******************************************************************************
* @brief       UART0 Receive Clock Select
* @details     Set S0RCK
* @param[in]   __SOURCE__ :
*  @arg\b      RECEIVE_CLOCK_S0BRG
*  @arg\b      RECEIVE_CLOCK_TIMER_S1BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_RXClockSource_Select(RECEIVE_CLOCK_S0BRG)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT0_RXClockSource_Select
#define RECEIVE_CLOCK_S0BRG 0x01
#define RECEIVE_CLOCK_SYSCLK_TIMER_S1BRG 0x00
///@endcond
#define __DRV_URT0_RXClockSource_Select(__SOURCE__)\
    ((__SOURCE__==0x01)?(S0CR1=S0CR1|(S0RCK)):(S0CR1=S0CR1&(~S0RCK)))
/**
*******************************************************************************
* @brief       UART0 Transmit Clock Select
* @details     Set S0TCK
* @param[in]   __SOURCE__ :
*  @arg\b      TRANSMIT_CLOCK_S0BRG
*  @arg\b      TRANSMIT_CLOCK_TIMER_S1BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_TXClockSource_Select(TRANSMIT_CLOCK_S0BRG)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT0_TXClockSource_Select
#define TRANSMIT_CLOCK_S0BRG 0x01
#define TRANSMIT_CLOCK_SYSCLK_TIMER_S1BRG 0x00
///@endcond
#define __DRV_URT0_TXClockSource_Select(__SOURCE__)\
    ((__SOURCE__)==0x01?(S0CR1=S0CR1|(S0TCK)):(S0CR1=S0CR1&(~S0TCK)))
/**
*******************************************************************************
* @brief        UART0 Clock Source S0BRG Select
* @details      Set S0TCK S0RCK
* @param[in]    __SOURCE__ :
*  @arg\b       TRANSMIT_CLOCK_SOURCE_TIMER_S1BRG_RECEIVE_CLOCK_SOURCE_TIMER_S1BRG
*  @arg\b       TRANSMIT_CLOCK_SOURCE_TIMER_S1BRG_RECEIVE_CLOCK_SOURCE_S0BRG
*  @arg\b       TRANSMIT_CLOCK_SOURCE_S0BRG_RECEIVE_CLOCK_SOURCE_TIMER_S1BRG
*  @arg\b       TRANSMIT_CLOCK_SOURCE_S0BRG_RECEIVE_CLOCK_SOURCE_S0BRG
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_URT0_Clock_Source_S0BRG_Select(TRANSMIT_CLOCK_SOURCE_S0BRG_RECEIVE_CLOCK_SOURCE_S0BRG)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT0_Clock_Source_S0BRG_Select
#define TRANSMIT_CLOCK_SOURCE_TIMER_S1BRG_RECEIVE_CLOCK_SOURCE_TIMER_S1BRG 0
#define TRANSMIT_CLOCK_SOURCE_TIMER_S1BRG_RECEIVE_CLOCK_SOURCE_S0BRG 1
#define TRANSMIT_CLOCK_SOURCE_S0BRG_RECEIVE_CLOCK_SOURCE_TIMER_S1BRG 2
#define TRANSMIT_CLOCK_SOURCE_S0BRG_RECEIVE_CLOCK_SOURCE_S0BRG 3
///@endcond
#define __DRV_URT0_Clock_Source_S0BRG_Select(__SOURCE__)\
    MWT(\
        __DRV_URT0_S0Control_Cmd(MW_ENABLE);\
        __DRV_URT0_SetS0TCK(__SOURCE__);\
        __DRV_URT0_SetS0RCK(__SOURCE__);\
    ;)
/**
*******************************************************************************
* @brief       UART0 Clock Source S0BRG Select
* @details     Set S0TCK
* @param[in]   __SOURCE__ :
*  @arg\b      TRANSMIT_CLOCK_SOURCE_TIMER_S1BRG_RECEIVE_CLOCK_SOURCE_TIMER_S1BRG
*  @arg\b      TRANSMIT_CLOCK_SOURCE_TIMER_S1BRG_RECEIVE_CLOCK_SOURCE_S0BRG
*  @arg\b      TRANSMIT_CLOCK_SOURCE_S0BRG_RECEIVE_CLOCK_SOURCE_TIMER_S1BRG
*  @arg\b      TRANSMIT_CLOCK_SOURCE_S0BRG_RECEIVE_CLOCK_SOURCE_S0BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_SetS0TCK(TRANSMIT_CLOCK_SOURCE_TIMER_S1BRG_RECEIVE_CLOCK_SOURCE_TIMER_S1BRG)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_SetS0TCK(__SOURCE__)\
    MWT(\
        (__SOURCE__^0)==0?S0CR1=S0CR1&(~S0TCK):\
        (__SOURCE__^1)==0?S0CR1=S0CR1&(~S0TCK):\
        (__SOURCE__^2)==0?S0CR1=S0CR1|(S0TCK):\
        (__SOURCE__^3)==0?S0CR1=S0CR1|(S0TCK):_nop_();\
    ;)
/**
*******************************************************************************
* @brief       UART0 Clock Source S0BRG Select
* @details     Set S0RCK
* @param[in]   __SOURCE__ :
*  @arg\b      TRANSMIT_CLOCK_SOURCE_TIMER_S1BRG_RECEIVE_CLOCK_SOURCE_TIMER_S1BRG
*  @arg\b      TRANSMIT_CLOCK_SOURCE_TIMER_S1BRG_RECEIVE_CLOCK_SOURCE_S0BRG
*  @arg\b      TRANSMIT_CLOCK_SOURCE_S0BRG_RECEIVE_CLOCK_SOURCE_TIMER_S1BRG
*  @arg\b      TRANSMIT_CLOCK_SOURCE_S0BRG_RECEIVE_CLOCK_SOURCE_S0BRG
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_SetS0RCK(TRANSMIT_CLOCK_SOURCE_TIMER_S1BRG_RECEIVE_CLOCK_SOURCE_TIMER_S1BRG)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_SetS0RCK(__SOURCE__)\
    MWT(\
        (__SOURCE__^0)==0?S0CR1=S0CR1&(~S0RCK):\
        (__SOURCE__^1)==0?S0CR1=S0CR1|(S0RCK):\
        (__SOURCE__^2)==0?S0CR1=S0CR1&(~S0RCK):\
        (__SOURCE__^3)==0?S0CR1=S0CR1|(S0RCK):_nop_();\
    ;)
/**
*******************************************************************************
* @brief        UART0 Clock Source Timer2 Select
* @details      Set S0TCK S0RCK
* @param[in]    __SOURCE__ :
*  @arg\b       TRANSMIT_CLOCK_SOURCE_TIMER1_S1BRG_RECEIVE_CLOCK_SOURCE_TIMER1_S1BRG
*  @arg\b       TRANSMIT_CLOCK_SOURCE_TIMER1_S1BRG_RECEIVE_CLOCK_SOURCE_TIMER2
*  @arg\b       TRANSMIT_CLOCK_SOURCE_TIMER2_RECEIVE_CLOCK_SOURCE_TIMER1_S1BRG
*  @arg\b       TRANSMIT_CLOCK_SOURCE_TIMER2_RECEIVE_CLOCK_SOURCE_TIMER2
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_URT0_Clock_Source_Timer2_Select(TRANSMIT_CLOCK_SOURCE_TIMER1_S1BRG_RECEIVE_CLOCK_SOURCE_TIMER1_S1BRG)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT0_Clock_Source_Timer2_Select
#define TRANSMIT_CLOCK_SOURCE_TIMER1_S1BRG_RECEIVE_CLOCK_SOURCE_TIMER1_S1BRG 0
#define TRANSMIT_CLOCK_SOURCE_TIMER1_S1BRG_RECEIVE_CLOCK_SOURCE_TIMER2 1
#define TRANSMIT_CLOCK_SOURCE_TIMER2_RECEIVE_CLOCK_SOURCE_TIMER1_S1BRG 2
#define TRANSMIT_CLOCK_SOURCE_TIMER2_RECEIVE_CLOCK_SOURCE_TIMER2 3
///@endcond
#define __DRV_URT0_Clock_Source_Timer2_Select(__SOURCE__)\
    MWT(\
        __DRV_TIMER2_Function_Access_Control_Cmd(MW_DISABLE);\
        __DRV_URT0_SetTCLK(__SOURCE__);\
        __DRV_URT0_SetRCLK(__SOURCE__);\
    ;)
/**
*******************************************************************************
* @brief       UART0 Clock Source Timer2 Select
* @details     Set TCLK
* @param[in]   __SOURCE__ :
*  @arg\b      TRANSMIT_CLOCK_SOURCE_TIMER1_S1BRG_RECEIVE_CLOCK_SOURCE_TIMER1_S1BRG
*  @arg\b      TRANSMIT_CLOCK_SOURCE_TIMER_S1BRG_RECEIVE_CLOCK_SOURCE_TIMER2
*  @arg\b      TRANSMIT_CLOCK_SOURCE_TIMER2_RECEIVE_CLOCK_SOURCE_TIMER1_S1BRG
*  @arg\b      TRANSMIT_CLOCK_SOURCE_TIMER2_RECEIVE_CLOCK_SOURCE_TIMER2
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_SetTCLK(TRANSMIT_CLOCK_SOURCE_TIMER1_S1BRG_RECEIVE_CLOCK_SOURCE_TIMER1_S1BRG)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_SetTCLK(__SOURCE__)\
    MWT(\
        (__SOURCE__^0)==0?(TCLK_TL2IE=0):\
        (__SOURCE__^1)==0?(TCLK_TL2IE=0):\
        (__SOURCE__^2)==0?(TCLK_TL2IE=1):\
        (__SOURCE__^3)==0?(TCLK_TL2IE=1):_nop_();\
    ;)
/**
*******************************************************************************
* @brief       UART0 Clock Source Timer2 Select
* @details     Set RCLK
* @param[in]   __SOURCE__ :
*  @arg\b      TRANSMIT_CLOCK_SOURCE_TIMER1_S1BRG_RECEIVE_CLOCK_SOURCE_TIMER1_S1BRG
*  @arg\b      TRANSMIT_CLOCK_SOURCE_TIMER1_S1BRG_RECEIVE_CLOCK_SOURCE_TIMER2
*  @arg\b      TRANSMIT_CLOCK_SOURCE_TIMER2_RECEIVE_CLOCK_SOURCE_TIMER1_S1BRG
*  @arg\b      TRANSMIT_CLOCK_SOURCE_TIMER2_RECEIVE_CLOCK_SOURCE_TIMER2
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_SetRCLK(TRANSMIT_CLOCK_SOURCE_TIMER1_S1BRG_RECEIVE_CLOCK_SOURCE_TIMER1_S1BRG)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_SetRCLK(__SOURCE__)\
    MWT(\
        (__SOURCE__^0)==0?(RCLK_TF2L=0):\
        (__SOURCE__^1)==0?(RCLK_TF2L=1):\
        (__SOURCE__^2)==0?(RCLK_TF2L=0):\
        (__SOURCE__^3)==0?(RCLK_TF2L=1):_nop_();\
    ;)
/**
*******************************************************************************
* @brief        UART0 Mode2 Pure Timer Clock Source Select
* @details      Set S0TX12 SM20
* @param[in]    __SOURCE__ :
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_T1OF
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_1
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_T0OF
* @return       None
* @note         None
* @par          Example
* @code
__DRV_URT0_Pure_Timer_Clock_Source_Select(PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT0_Pure_Timer_Clock_Source_Select
#define PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12 0
#define PURE_TIMER_CLOCK_SOURCE_T1OF 1
#define PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_1 2
#define PURE_TIMER_CLOCK_SOURCE_T0OF 3
///@endcond
#define __DRV_URT0_Pure_Timer_Clock_Source_Select(__SOURCE__)\
    MWT(\
        __DRV_URT0_SetS0TX12(__SOURCE__);\
        __DRV_URT0_SetSM20(__SOURCE__);\
    ;)
/**
*******************************************************************************
* @brief       UART0 Mode2 Pure Timer Clock Source Select
* @details     Set S0TX12
* @param[in]   __SOURCE__ :
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_T1OF
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_1
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_T0OF
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_SetS0TX12(PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_SetS0TX12(__SOURCE__)\
    MWT(\
        (__SOURCE__^0)==0?S0CR1=S0CR1&(~S0TX12):\
        (__SOURCE__^1)==0?S0CR1=S0CR1&(~S0TX12):\
        (__SOURCE__^2)==0?S0CR1=S0CR1|(S0TX12):\
        (__SOURCE__^3)==0?S0CR1=S0CR1|(S0TX12):_nop_();\
    ;)
/**
*******************************************************************************
* @brief       UART0 Mode2 Pure Timer Clock Source Select
* @details     Set SM20
* @param[in]   __SOURCE__ :
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_T1OF
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_1
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_T0OF
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_SetSM20(PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_SetSM20(__SOURCE__)\
    MWT(\
        (__SOURCE__^0)==0?(SM20=0):\
        (__SOURCE__^1)==0?(SM20=1):\
        (__SOURCE__^2)==0?(SM20=0):\
        (__SOURCE__^3)==0?(SM20=1):_nop_();\
    ;)
/**
*******************************************************************************
* @brief        UART0 Clock Source S1BRG Select
* @details      Set URTS
* @param[in]    __SOURCE__ :
*  @arg\b       TRANSMIT_CLOCK_SOURCE_TIMER1_RECEIVE_CLOCK_SOURCE_TIMER1
*  @arg\b       TRANSMIT_CLOCK_SOURCE_S1BRG_RECEIVE_CLOCK_SOURCE_S1BRG
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_URT0_Clock_Source_S1BRG_Select(TRANSMIT_CLOCK_SOURCE_TIMER1_RECEIVE_CLOCK_SOURCE_TIMER1)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT0_Clock_Source_S1BRG_Select
#define TRANSMIT_CLOCK_SOURCE_TIMER1_RECEIVE_CLOCK_SOURCE_TIMER1 0
#define TRANSMIT_CLOCK_SOURCE_S1BRG_RECEIVE_CLOCK_SOURCE_S1BRG 1
///@endcond
#define __DRV_URT0_Clock_Source_S1BRG_Select(__SOURCE__)\
    ((__SOURCE__)==0?(S0CFG=S0CFG&(~URTS)):(S0CFG=S0CFG|(URTS)))
/**
*******************************************************************************
* @brief       UART0 Enable S0BRG clock output control
* @details     Set S0CKOE
* @param[in]   __STATE__ :
*  @arg\b      MW_DISABLE
*  @arg\b      MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_Baud_Rate_Generator_clock_output_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/

#define __DRV_URT0_Baud_Rate_Generator_Clock_Output_Cmd(__STATE__)\
    ((__STATE__)==1?(S0CR1=S0CR1|(S0CKOE)):(S0CR1=S0CR1&(~S0CKOE)))

/**
*******************************************************************************
* @brief       UART0 Enable S0BRG Operation
* @details     Set S0TR
* @param[in]   __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        Before write S0BRT and S0BRC,S0BRG operation stop first
* @par         Example
* @code
__DRV_URT0_BaudRateGenerator_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/

#define __DRV_URT0_BaudRateGenerator_Cmd(__STATE__)\
    ((__STATE__)==1?(S0CR1=S0CR1|(S0TR)):(S0CR1=S0CR1&(~S0TR)))
/**
*******************************************************************************
* @brief        UART0 Baud Rate Generator Reload Register
* @details      Write S0BRT S0BRC
* @param[in]    __RELOAD__ :
*  @arg\b S0BRG_BRGRL_2400_1X_12000000_12T
*  @arg\b S0BRG_BRGRL_4800_1X_11059200_12T
*  @arg\b S0BRG_BRGRL_9600_1X_11059200_12T
*  @arg\b S0BRG_BRGRL_2400_1X_12000000_1T
*  @arg\b S0BRG_BRGRL_4800_1X_12000000_1T
*  @arg\b S0BRG_BRGRL_9600_1X_12000000_1T
*  @arg\b S0BRG_BRGRL_19200_1X_12000000_1T
*  @arg\b S0BRG_BRGRL_38400_1X_12000000_1T
*  @arg\b S0BRG_BRGRL_57600_1X_11059200_1T
*  @arg\b S0BRG_BRGRL_115200_1X_11059200_1T
*  @arg\b S0BRG_BRGRL_2400_2X_12000000_12T
*  @arg\b S0BRG_BRGRL_4800_2X_12000000_12T
*  @arg\b S0BRG_BRGRL_9600_2X_11059200_12T
*  @arg\b S0BRG_BRGRL_19200_2X_11059200_12T
*  @arg\b S0BRG_BRGRL_57600_2X_11059200_12T
*  @arg\b S0BRG_BRGRL_4800_2X_12000000_1T
*  @arg\b S0BRG_BRGRL_9600_2X_12000000_1T
*  @arg\b S0BRG_BRGRL_19200_2X_12000000_1T
*  @arg\b S0BRG_BRGRL_38400_2X_12000000_1T
*  @arg\b S0BRG_BRGRL_57600_2X_12000000_1T
*  @arg\b S0BRG_BRGRL_115200_2X_11059200_1T
*  @arg\b S0BRG_BRGRL_230400_2X_11059200_1T
*  @arg\b S0BRG_BRGRL_250000_2X_12000000_1T
*  @arg\b S0BRG_BRGRL_750000_2X_12000000_1T
* @return       None
* @note         Before write S0BRT and S0BRC,S0BRG operation stop first
* @par          Example
* @code
__DRV_URT0_BaudRateReloadReg_Write(S0BRG_BRGRL_9600_2X_12000000_1T)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_BaudRateReloadReg_Write(__RELOAD__)\
    (S0BRT=S0BRC=__RELOAD__)
/**
*******************************************************************************
* @brief        UART0 Mode2 Pure Timer 16 Bit Reload
* @details      Set S0BRC S0BRT
* @param[in]    __RELOAD__ :
*  @arg\b 0~65535
* @return       None
* @note         None
* @par          Example
* @code
__DRV_URT0_BaudRateReloadReg16Bit_Write(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_BaudRateReloadReg16Bit_Write(__RELOAD__)\
    MWT(\
        S0BRT=HIBYTE(__RELOAD__);\
        S0BRC=LOBYTE(__RELOAD__)&0xff;\
    ;)
/**
*******************************************************************************
* @brief       UART0 Enable Reception
* @details     Set REN0
* @param[in]   __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_SerialReception_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_SerialReception_Cmd(__STATE__)\
    ((__STATE__)==1?(REN0=1):(REN0=0))
/**
*******************************************************************************
* @brief     UART0 In Mode 2 Enable SYSCLK/32 SYSCLK/64 SYSCLK/96 SYSCLK/192
* @details   Disable URM0X3
* @param[in] __STATE__ :
*  @arg\b       MW_DISABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_BaudRateDiv3_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_BaudRateDiv3_Cmd(__STATE__)\
    ((__STATE__)==64?(S0CFG=S0CFG|(URM0X3)):(S0CFG=S0CFG&(~URM0X3)))
/**
*******************************************************************************
* @brief     UART0 In Mode 0 Mode 4 Enable SYSCLK/4,SYSCLK/12
* @details   Set URM0X3
* @param[in] __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_BaudRateSysclkDiv4_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_BaudRateSysclkDiv4_Cmd(__STATE__)\
    ((__STATE__)==1?(S0CFG=S0CFG|(URM0X3)):(S0CFG=S0CFG&(~URM0X3)))
/**
*******************************************************************************
* @brief     UART0 In Mode 2 Enable Pure Timer Baudrate Generator Overflow to S0 Interrupt
* @details   Set TB80
* @param[in] __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_Pure_Timer_Baudrate_Generator_Overflow_to_S0_Interrupt_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_Pure_Timer_Baudrate_Generator_Overflow_to_S0_Interrupt_Cmd(__STATE__)\
    ((__STATE__)==1?(TB80=1):(TB80=0))
/**
*******************************************************************************
* @brief     UART0 In Mode 2 Enable Pure Timer External Pin Trigger to S0 Interrupt
* @details   Set REN0
* @param[in] __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_Pure_Timer_External_Pin_Trigger_to_S0_Interrupt_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_Pure_Timer_External_Pin_Trigger_to_S0_Interrupt_Cmd(__STATE__)\
    ((__STATE__)==1?(REN0=1):(REN0=0))
/**
*******************************************************************************
* @brief     UART0 In Mode 2 Enable Pure Timer External Pin Control Baudrate Generator
* @details   Set SMOD1
* @param[in] __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_Pure_Timer_External_Pin_Control_Baudrate_Generator_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_Pure_Timer_External_Pin_Control_Baudrate_Generator_Cmd(__STATE__)\
    ((__STATE__)==1?(PCON0=PCON0|(SMOD1)):(PCON0=PCON0&(~SMOD1)))
/**
*******************************************************************************
* @brief     UART0 In Mode 2 Enable Pure Timer External Pin Trigger Level
* @details   Set RB80
* @param[in] __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT0_Pure_Timer_External_Pin_Trigger_Level_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT0_Pure_Timer_External_Pin_Trigger_Level_Cmd(__STATE__)\
    ((__STATE__)==1?(RB80=1):(RB80=0))


///@cond
bool DRV_URT0_GetTI0(void);
bool DRV_URT0_GetRI0(void);
void DRV_URT0_ClearTI0(void);
void DRV_URT0_ClearRI0(void);
void DRV_URT0_ClearSM20(void);
void DRV_URT0_SetSM20(void);
void DRV_URT0_SetTXData9th(void);
void DRV_URT0_ClearTXData9th(void);
bool DRV_URT0_GetRXData9th(void);
void DRV_URT0_SendTXData(uint8_t TXData);
uint8_t DRV_URT0_ReceiveRXData(void);
void DRV_URT0_SetSADDR(uint8_t Address );
void DRV_URT0_SetSADEN(uint8_t Address );

void DRV_URT0_SetSyncBreakFlag(void);
bool DRV_URT0_GetSyncBreakFlag(void);
void DRV_URT0_SetSyncBreak(void);
bool DRV_URT0_GetSyncBreak(void);
void DRV_URT0_SetAutoBaudRate(void);
uint8_t DRV_URT0_GetBaudRate(void);
void DRV_URT0_SetTransmitErrorFlag(void);
bool DRV_URT0_GetTransmitErrorFlag(void);
///@endcond
#endif

