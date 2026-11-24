/**
 ******************************************************************************
 *
 * @file        MG82F6D64_UART1_DRV.h
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

#ifndef MG82F6D64_UART1_DRV_H
#define MG82F6D64_UART1_DRV_H

/**
*******************************************************************************
* @brief        UART1 Enable Interrupt
* @details      UART1 Enable Interrupt
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_Interrupt_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT1_IT_Cmd(__STATE__)\
    __STATE__==0?(EIE1=EIE1&(~ES1)):(EIE1=EIE1|(ES1))
/**
*******************************************************************************
* @brief       UART1 Mode Select
* @details     Set SM31 SM01 SM11
* @param[in]   __MODE__ :
*  @arg\b      URT1_MODE0_SHIFT_REG
*  @arg\b      URT1_MODE1_8BIT
*  @arg\b      URT1_MODE2_9BIT
*  @arg\b      URT1_MODE3_9BIT
*  @arg\b      URT1_MODE4_SPI_MASTER
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_Mode_Select(URT1_Mode0_SHIFT_REG)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT1_Mode_Select
#define UART1_MODE0_SHIFT_REG 0x00
#define UART1_MODE1_8BIT 0x01
#define UART1_MODE2_9BIT 0x02
#define UART1_MODE3_9BIT 0x03
#define UART1_MODE4_SPI_MASTER 0x04
///@endcond
#define __DRV_URT1_Mode_Select(__MODE__)\
    MWT(\
        __DRV_URT1_SetSM31(__MODE__);\
        __DRV_URT1_SetSM01(__MODE__);\
        __DRV_URT1_SetSM11(__MODE__);\
    ;)
/**
*******************************************************************************
* @brief       UART1 PinMUX Select
* @details     Set S1PS1 S1PS0
* @param[in]   __RXTX__ :
*  @arg\b      UART1_RX_P10_TX_P11
*  @arg\b      UART1_RX_P60_TX_P61
*  @arg\b      UART1_RX_P44_TX_P45
*  @arg\b      UART1_RX_P34_TX_P35
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_PinMux_Select(UART1_RX_P10_TX_P11)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT1_PinMux_Select
#define UART1_RX_P10_TX_P11 0x00
#define UART1_RX_P60_TX_P61 0x01
#define UART1_RX_P44_TX_P45 0x02
#define UART1_RX_P34_TX_P35 0x03
///@endcond
#define __DRV_URT1_PinMux_Select(__RXTX__)\
    MWT(\
        __DRV_URT1_SetS1PS1(__RXTX__);\
        __DRV_URT1_SetS1PS0(__RXTX__);\
    ;)

/**
*******************************************************************************
* @brief       UART1 Mode Select
* @details     Set SM31
* @param[in]   __MODE__ :
*  @arg\b      URT1_MODE0_SHIFT_REG
*  @arg\b      URT1_MODE1_8BIT
*  @arg\b      URT1_MODE2_9BIT
*  @arg\b      URT1_MODE3_9BIT
*  @arg\b      URT1_MODE4_SPI_MASTER
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_SetSM31(URT1_MODE1_8BIT)
* @endcode
*******************************************************************************
*/
#define __DRV_URT1_SetSM31(__MODE__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CFG_Page);\
        ((__MODE__^0x00)==0?(S1CFG=S1CFG&(~SM31)):\
        (__MODE__^0x01)==0?(S1CFG=S1CFG&(~SM31)):\
        (__MODE__^0x02)==0?(S1CFG=S1CFG&(~SM31)):\
        (__MODE__^0x03)==0?(S1CFG=S1CFG&(~SM31)):\
        (__MODE__^0x04)==0?(S1CFG=S1CFG|(SM31)):\
        (__MODE__^0x05)==0?(S1CFG=S1CFG|(SM31)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART1 Mode Select
* @details     Set SM01
* @param[in]   __MODE__ :
*  @arg\b      URT1_MODE0_SHIFT_REG
*  @arg\b      URT1_MODE1_8BIT
*  @arg\b      URT1_MODE2_9BIT
*  @arg\b      URT1_MODE3_9BIT
*  @arg\b      URT1_MODE4_SPI_MASTER
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_SetSM01(URT1_MODE2_9BIT)
* @endcode
*******************************************************************************
*/
#define __DRV_URT1_SetSM01(__MODE__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CON_Page);\
        ((__MODE__^0x00)==0?(SM01=0):\
        (__MODE__^0x01)==0?(SM01=0):\
        (__MODE__^0x02)==0?(SM01=1):\
        (__MODE__^0x03)==0?(SM01=1):\
        (__MODE__^0x04)==0?(SM01=0):\
        (__MODE__^0x05)==0?(SM01=0):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART1 Mode Select
* @details     Set SM11
* @param[in]   __MODE__ :
*  @arg\b      URT1_MODE0_SHIFT_REG
*  @arg\b      URT1_MODE1_8BIT
*  @arg\b      URT1_MODE2_9BIT
*  @arg\b      URT1_MODE3_9BIT
*  @arg\b      URT1_MODE4_SPI_MASTER
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_SetSM11(URT1_MODE3_9BIT)
* @endcode
*******************************************************************************
*/
#define __DRV_URT1_SetSM11(__MODE__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CON_Page);\
        ((__MODE__^0x00)==0?(SM11=0):\
        (__MODE__^0x01)==0?(SM11=1):\
        (__MODE__^0x02)==0?(SM11=0):\
        (__MODE__^0x03)==0?(SM11=1):\
        (__MODE__^0x04)==0?(SM11=0):\
        (__MODE__^0x05)==0?(SM11=1):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART1 PinMUX Select
* @details     Set S1PS1
* @param[in]   __RXTX__ :
*  @arg\b      UART1_RX_P10_TX_P11
*  @arg\b      UART1_RX_P60_TX_P61
*  @arg\b      UART1_RX_P44_TX_P45
*  @arg\b      UART1_RX_P34_TX_P35
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_SetS1PS1(UART1_RX_P10_TX_P11)
* @endcode
*******************************************************************************
*/
#define __DRV_URT1_SetS1PS1(__RXTX__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR9_Page);\
        ((__RXTX__^0x00)==0?AUXR9=AUXR9&(~S1PS1):\
        (__RXTX__^0x01)==0?AUXR9=AUXR9&(~S1PS1):\
        (__RXTX__^0x02)==0?AUXR9=AUXR9|(S1PS1):\
        (__RXTX__^0x03)==0?AUXR9=AUXR9|(S1PS1):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART1 PinMUX Select
* @details     Set S1PS0
* @param[in]   __RXTX__ :
*  @arg\b      UART1_RX_P10_TX_P11
*  @arg\b      UART1_RX_P60_TX_P61
*  @arg\b      UART1_RX_P44_TX_P45
*  @arg\b      UART1_RX_P34_TX_P35
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_SetS1PS0(UART1_RX_P10_TX_P11)
* @endcode
*******************************************************************************
*/
#define __DRV_URT1_SetS1PS0(__RXTX__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR9_Page);\
        ((__RXTX__^0x00)==0?AUXR9=AUXR9&(~S1PS0):\
        (__RXTX__^0x01)==0?AUXR9=AUXR9|(S1PS0):\
        (__RXTX__^0x02)==0?AUXR9=AUXR9&(~S1PS0):\
        (__RXTX__^0x03)==0?AUXR9=AUXR9|(S1PS0):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART1 Data Order Select
* @details     Set S1DOR
* @param[in]   __ORDER__ :
*  @arg\b      UART1_DATA_ORDER_LSB
*  @arg\b      UART1_DATA_ORDER_MSB
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_DataOrder_Select(UART1_DATA_ORDER_LSB)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT1_DataOrder_Select
#define UART1_DATA_ORDER_LSB 0x08
#define UART1_DATA_ORDER_MSB 0x00
///@endcond
#define __DRV_URT1_DataOrder_Select(__ORDER__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CFG_Page);\
        (__ORDER__==0x08)?(S1CFG=S1CFG|(S1DOR)):(S1CFG=S1CFG&(~S1DOR));\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART1 Timer Mode Select
* @details     Set S1TME
* @param[in]   __MODE__ :
*  @arg\b      UART1_TIMER_MODE
*  @arg\b      UART1_UART_MODE
* @return      None
* @note        S1TME=1 UART1 timer mode
* @par         Example
* @code
__DRV_URT1_TimerMode_Select(UART1_TIMER_MODE)
* @endcode
*******************************************************************************
*/

///@cond __DRV_URT1_TimerMode_Select
#define UART1_TIMER_MODE 1
#define UART1_UART_MODE 0
///@endcond
#define __DRV_URT1_TimerMode_Select(__MODE__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CFG_Page);\
        (__MODE__==1)?(S1CFG=S1CFG|(S1TME)):(S1CFG=S1CFG&(~S1TME));\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART1 Pure Timer Select
* @details     Set S1DOR
* @param[in]   __MODE__ :
*  @arg\b      UART1_8BIT_PURE_TIMER
*  @arg\b      UART1_16BIT_PURE_TIMER
* @return      None
* @note        S1DOR=1 S1TME=1 8bit pure timer
* @par         Example
* @code
__DRV_URT1_PureTimer_Select(UART1_8BIT_PURE_TIMER)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT1_PureTimer_Select
#define UART1_8BIT_PURE_TIMER 1
#define UART1_16BIT_PURE_TIMER 0
///@endcond

#define __DRV_URT1_PureTimer_Select(__MODE__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CFG_Page);\
        (__MODE__==1)?(S1CFG=S1CFG|(S1DOR)):(S1CFG=S1CFG&(~S1DOR));\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief       UART1 Enhance Baud Rate Select
* @details     Set S1MOD1
* @param[in]   __TIME__ :
*  @arg\b      UART1_DEFAULT_BAUD_RATE
*  @arg\b      UART1_DOUBLE_BAUD_RATE_X1
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_BaudRateX2_Select(Default_Baud_Rate)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT1_BaudRateX2_Select
#define UART1_DEFAULT_BAUD_RATE 0
#define UART1_DOUBLE_BAUD_RATE_X1 16
///@endcond

#define __DRV_URT1_BaudRateX2_Select(__TIME__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CFG_Page);\
        ((__TIME__^16)==0?(S1CFG=S1CFG|(S1MOD1)):(S1CFG=S1CFG&(~S1MOD1)));\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART1 S1BRG Clock Source Select
* @details     Set S1TX12
* @param[in]   __SOURCE__ :
*  @arg\b      S1BRG_CLOCK_SOURCE_SYSCLK_DIV_1
*  @arg\b      S1BRG_CLOCK_SOURCE_SYSCLK_DIV_12
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_BaudRateGeneratorClock_Select(S1BRG_CLOCK_SOURCE_SYSCLK_DIV_1)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT1_BaudRateGeneratorClock_Select
#define S1BRG_CLOCK_SOURCE_SYSCLK_DIV_1 64
#define S1BRG_CLOCK_SOURCE_SYSCLK_DIV_12 0
///@endcond
#define __DRV_URT1_BaudRateGeneratorClock_Select(__SOURCE__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CFG_Page);\
        (__SOURCE__)==0?(S1CFG=S1CFG&(~S1TX12)):(S1CFG=S1CFG|(S1TX12));\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief        UART1 Mode2 Pure Timer Clock Source Select
* @details      Set S1TX12 SM21
* @param[in]    __SOURCE__ :
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_T1OF
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_1
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_T0OF
* @return       None
* @note         None
* @par          Example
* @code
__DRV_URT1_Pure_Timer_Clock_Source_Select(PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_URT1_Pure_Timer_Clock_Source_Select(__SOURCE__)\
    MWT(\
        __DRV_URT1_SetS1TX12(__SOURCE__);\
        __DRV_URT1_SetSM21(__SOURCE__);\
    ;)
/**
*******************************************************************************
* @brief       UART1 Mode2 Pure Timer Clock Source
* @details     Set S1TX12
* @param[in]   __SOURCE__ :
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_T1OF
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_1
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_T0OF
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_SetS1TX12(PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_URT1_SetS1TX12(__SOURCE__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CFG_Page);\
        (__SOURCE__^0)==0?S1CFG=S1CFG&(~S1TX12):\
        (__SOURCE__^1)==0?S1CFG=S1CFG&(~S1TX12):\
        (__SOURCE__^2)==0?S1CFG=S1CFG|(S1TX12):\
        (__SOURCE__^3)==0?S1CFG=S1CFG|(S1TX12):_nop_();\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART1 Mode2 Pure Timer Clock Source
* @details     Set SM21
* @param[in]   __SOURCE__ :
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_T1OF
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_1
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_T0OF
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_SetSM21(PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_URT1_SetSM21(__SOURCE__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CON_Page);\
        (__SOURCE__^0)==0?(SM21=0):\
        (__SOURCE__^1)==0?(SM21=1):\
        (__SOURCE__^2)==0?(SM21=0):\
        (__SOURCE__^3)==0?(SM21=1):_nop_();\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART1 Enable S1BRG clock output control
* @details     Set S1CKOE
* @param[in]   __STATE__ :
*  @arg\b      MW_DISABLE
*  @arg\b      MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_Baud_Rate_Generator_clock_output_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/

#define __DRV_URT1_Baud_Rate_Generator_Clock_Output_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CFG_Page);\
        (__STATE__==1)?(S1CFG=S1CFG|(S1CKOE)):(S1CFG=S1CFG&(~S1CKOE));\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief       UART1 Enable S1BRG Operation
* @details     Set S1TR
* @param[in]   __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_BaudRateGenerator_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT1_BaudRateGenerator_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CFG_Page);\
        (__STATE__==1)?(S1CFG=S1CFG|(S1TR)):(S1CFG=S1CFG&(~S1TR));\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief      UART1 Baud Rate Generator Reload Register
* @details    Write S1BRT S1BRC
* @param[in] __RELOAD__ :
*  @arg\b    S1BRG_BRGRL_2400_1X_12000000_12T
*  @arg\b    S1BRG_BRGRL_2400_1X_12000000_1T
*  @arg\b    S1BRG_BRGRL_4800_1X_12000000_1T
*  @arg\b    S1BRG_BRGRL_9600_1X_12000000_1T
*  @arg\b    S1BRG_BRGRL_19200_1X_12000000_1T
*  @arg\b    S1BRG_BRGRL_38400_1X_12000000_1T
*  @arg\b    S1BRG_BRGRL_2400_2X_12000000_12T
*  @arg\b    S1BRG_BRGRL_4800_2X_12000000_12T
*  @arg\b    S1BRG_BRGRL_4800_2X_12000000_1T
*  @arg\b    S1BRG_BRGRL_9600_2X_12000000_1T
*  @arg\b    S1BRG_BRGRL_19200_2X_12000000_1T
*  @arg\b    S1BRG_BRGRL_38400_2X_12000000_1T
*  @arg\b    S1BRG_BRGRL_57600_2X_12000000_1T
*  @arg\b    S1BRG_BRGRL_115200_2X_11059200_1T
*  @arg\b    S1BRG_BRGRL_115200_1X_11059200_1T
*  @arg\b    S1BRG_BRGRL_57600_1X_11059200_1T
*  @arg\b    S1BRG_BRGRL_230400_2X_11059200_1T
*  @arg\b    S1BRG_BRGRL_250000_2X_12000000_1T
*  @arg\b    S1BRG_BRGRL_750000_2X_12000000_1T
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_BaudRateReloadReg_Write(S1BRG_BRGRL_9600_2X_12000000_1T)
* @endcode
*******************************************************************************
*/
#define __DRV_URT1_BaudRateReloadReg_Write(__RELOAD__)\
    MWT(\
        __DRV_SFR_PageIndex(S1BRT_Page);\
        S1BRT=S1BRC=__RELOAD__;\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief        UART1 In Timer Mode Pure Timer 16 Bit Reload
* @details      Set S1BRC S1BRT
* @param[in]    __RELOAD__ :
*  @arg\b 0~65535
* @return       None
* @note         None
* @par          Example
* @code
__DRV_URT1_BaudRateReloadReg16Bit_Write(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_URT1_BaudRateReloadReg16Bit_Write(__RELOAD__)\
    MWT(\
        __DRV_SFR_PageIndex(S1BRT_Page);\
        S1BRT=HIBYTE(__RELOAD__);\
        S1BRC=LOBYTE(__RELOAD__)&0xff;\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief       UART1 Enable Reception
* @details     Set REN1
* @param[in]   __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_SerialReception_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT1_SerialReception_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CON_Page);\
        (__STATE__==1)?(REN1=1):(REN1=0);\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief       UART1 In Mode 2 Enable SYSCLK/32 SYSCLK/64 SYSCLK/96 SYSCLK/192
* @details     Set S1M0X3
* @param[in] __STATE__ :
*  @arg\b    MW_DISABLE
*  @arg\b    MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_BaudRateDiv3_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT1_BaudRateDiv3_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CFG_Page);\
        __STATE__==64?(S1CFG=S1CFG|(S1M0X3)):(S1CFG=S1CFG&(~S1M0X3));\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART1 In Mode 0 Mode 4 Enable SYSCLK/4 SYSCLK/12
* @details     Set S1M0X3
* @param[in] __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_BaudRateSysclkDiv4_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/

#define __DRV_URT1_BaudRateSysclkDiv4_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CFG_Page);\
        __STATE__==64?(S1CFG=S1CFG|(S1M0X3)):(S1CFG=S1CFG&(~S1M0X3));\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief     UART1 In Timer Mode Enable Pure Timer Baudrate Generator Overflow to S1 Interrupt
* @details   Set TB81
* @param[in] __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_Pure_Timer_Baudrate_Generator_Overflow_to_S1_Interrupt_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT1_Pure_Timer_Baudrate_Generator_Overflow_to_S1_Interrupt_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CON_Page);\
        ((__STATE__)==1?(TB81=1):(TB81=0));\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief     UART1 In Timer Mode Enable Pure Timer External Pin Trigger to S1 Interrupt
* @details   Set REN1
* @param[in] __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_Pure_Timer_External_Pin_Trigger_to_S1_Interrupt_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT1_Pure_Timer_External_Pin_Trigger_to_S1_Interrupt_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CON_Page);\
        ((__STATE__)==1?(REN1=1):(REN1=0));\
        __DRV_SFR_PageIndex(0);\
    ;)


/**
*******************************************************************************
* @brief     UART1 In Timer Mode Enable Pure Timer External Pin Control Baudrate Generator
* @details   Set S1MOD1
* @param[in] __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_Pure_Timer_External_Pin_Control_Baudrate_Generator_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT1_Pure_Timer_External_Pin_Control_Baudrate_Generator_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CFG_Page);\
        ((__STATE__)==1?(S1CFG=S1CFG|(S1MOD1)):(S1CFG=S1CFG&(~S1MOD1)));\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief     UART1 In Timer Mode Enable Pure Timer External Pin Trigger Level
* @details   Set RB81
* @param[in] __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT1_Pure_Timer_External_Pin_Trigger_Level_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT1_Pure_Timer_External_Pin_Trigger_Level_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S1CON_Page);\
        ((__STATE__)==1?(RB81=1):(RB81=0));\
        __DRV_SFR_PageIndex(0);\
    ;)
///@cond
bool DRV_URT1_GetTI1(void);
bool DRV_URT1_GetRI1(void);
void DRV_URT1_ClearTI1(void);
void DRV_URT1_ClearRI1(void);
void DRV_URT1_ClearSM21(void);
void DRV_URT1_SetSM21(void);
void DRV_URT1_SetTXData9th(void);
void DRV_URT1_ClearTXData9th(void);
bool DRV_URT1_GetRXData9th(void);
void DRV_URT1_SendTXData(uint8_t TXData);
uint8_t DRV_URT1_ReceiveRXData(void);
///@endcond



#endif

