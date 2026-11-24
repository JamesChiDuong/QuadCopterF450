/**
 ******************************************************************************
 *
 * @file        MG82F6D64_UART2_DRV.h
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

#ifndef MG82F6D64_UART2_DRV_H
#define MG82F6D64_UART2_DRV_H

/**
*******************************************************************************
* @brief        UART2 Enable Interrupt
* @details      UART2 Enable Interrupt
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_Interrupt_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT2_IT_Cmd(__STATE__)\
    __STATE__==0?(EIE2=EIE2&(~ES2)):(EIE2=EIE2|(ES2))
/**
*******************************************************************************
* @brief       UART2 Mode Select
* @details     Set SM32 SM02 SM12
* @param[in]   __MODE__ :
*  @arg\b      URT2_MODE0_SHIFT_REG
*  @arg\b      URT2_MODE1_8BIT
*  @arg\b      URT2_MODE2_9BIT
*  @arg\b      URT2_MODE3_9BIT
*  @arg\b      URT2_MODE4_SPI_MASTER
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_Mode_Select(URT2_Mode0_SHIFT_REG)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT2_Mode_Select
#define UART2_MODE0_SHIFT_REG 0x00
#define UART2_MODE1_8BIT 0x01
#define UART2_MODE2_9BIT 0x02
#define UART2_MODE3_9BIT 0x03
#define UART2_MODE4_SPI_MASTER 0x04
///@endcond
#define __DRV_URT2_Mode_Select(__MODE__)\
    MWT(\
        __DRV_URT2_SetSM32(__MODE__);\
        __DRV_URT2_SetSM02(__MODE__);\
        __DRV_URT2_SetSM12(__MODE__);\
    ;)
/**
*******************************************************************************
* @brief       UART2 PinMUX Select
* @details     Set S2PS0
* @param[in]   __RXTX__ :
*  @arg\b      UART2_RX_P32_TX_P33
*  @arg\b      UART2_RX_P67_TX_P57
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_PinMux_Select(UART2_RX_P32_TX_P33)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT2_PinMux_Select
#define UART2_RX_P32_TX_P33 0x00
#define UART2_RX_P67_TX_P57 0x01
///@endcond
#define __DRV_URT2_PinMux_Select(__RXTX__)\
    MWT(\
        __DRV_URT2_SetS2PS0(__RXTX__);\
    ;)

/**
*******************************************************************************
* @brief       UART2 Mode Select
* @details     Set SM32
* @param[in]   __MODE__ :
*  @arg\b      URT2_MODE0_SHIFT_REG
*  @arg\b      URT2_MODE1_8BIT
*  @arg\b      URT2_MODE2_9BIT
*  @arg\b      URT2_MODE3_9BIT
*  @arg\b      URT2_MODE4_SPI_MASTER
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_SetSM31(URT2_MODE1_8BIT)
* @endcode
*******************************************************************************
*/
#define __DRV_URT2_SetSM32(__MODE__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CFG_Page);\
        ((__MODE__^0x00)==0?(S2CFG=S2CFG&(~SM32)):\
        (__MODE__^0x01)==0?(S2CFG=S2CFG&(~SM32)):\
        (__MODE__^0x02)==0?(S2CFG=S2CFG&(~SM32)):\
        (__MODE__^0x03)==0?(S2CFG=S2CFG&(~SM32)):\
        (__MODE__^0x04)==0?(S2CFG=S2CFG|(SM32)):\
        (__MODE__^0x05)==0?(S2CFG=S2CFG|(SM32)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART2 Mode Select
* @details     Set SM02
* @param[in]   __MODE__ :
*  @arg\b      URT2_MODE0_SHIFT_REG
*  @arg\b      URT2_MODE1_8BIT
*  @arg\b      URT2_MODE2_9BIT
*  @arg\b      URT2_MODE3_9BIT
*  @arg\b      URT2_MODE4_SPI_MASTER
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_SetSM02(URT2_MODE2_9BIT)
* @endcode
*******************************************************************************
*/
#define __DRV_URT2_SetSM02(__MODE__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CON_Page);\
        ((__MODE__^0x00)==0?(SM02=0):\
        (__MODE__^0x01)==0?(SM02=0):\
        (__MODE__^0x02)==0?(SM02=1):\
        (__MODE__^0x03)==0?(SM02=1):\
        (__MODE__^0x04)==0?(SM02=0):\
        (__MODE__^0x05)==0?(SM02=0):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART2 Mode Select
* @details     Set SM12
* @param[in]   __MODE__ :
*  @arg\b      URT2_MODE0_SHIFT_REG
*  @arg\b      URT2_MODE1_8BIT
*  @arg\b      URT2_MODE2_9BIT
*  @arg\b      URT2_MODE3_9BIT
*  @arg\b      URT2_MODE4_SPI_MASTER
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_SetSM12(URT2_MODE3_9BIT)
* @endcode
*******************************************************************************
*/
#define __DRV_URT2_SetSM12(__MODE__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CON_Page);\
        ((__MODE__^0x00)==0?(SM12=0):\
        (__MODE__^0x01)==0?(SM12=1):\
        (__MODE__^0x02)==0?(SM12=0):\
        (__MODE__^0x03)==0?(SM12=1):\
        (__MODE__^0x04)==0?(SM12=0):\
        (__MODE__^0x05)==0?(SM12=1):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART2 PinMUX Select
* @details     Set S2PS0
* @param[in]   __RXTX__ :
*  @arg\b      UART2_RX_P32_TX_P33
*  @arg\b      UART2_RX_P67_TX_P57
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_SetS2PS0(UART2_RX_P32_TX_P33)
* @endcode
*******************************************************************************
*/
#define __DRV_URT2_SetS2PS0(__RXTX__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR13_Page);\
        ((__RXTX__^0x00)==0?AUXR13=AUXR13&(~S2PS0):\
        (__RXTX__^0x01)==0?AUXR13=AUXR13|(S2PS0):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART2 Data Order Select
* @details     Set S2DOR
* @param[in]   __ORDER__ :
*  @arg\b      UART2_DATA_ORDER_LSB
*  @arg\b      UART2_DATA_ORDER_MSB
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_DataOrder_Select(UART2_DATA_ORDER_LSB)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT2_DataOrder_Select
#define UART2_DATA_ORDER_LSB 0x08
#define UART2_DATA_ORDER_MSB 0x00
///@endcond
#define __DRV_URT2_DataOrder_Select(__ORDER__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CFG_Page);\
        (__ORDER__==0x08)?(S2CFG=S2CFG|(S2DOR)):(S2CFG=S2CFG&(~S2DOR));\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART2 Timer Mode Select
* @details     Set S2TME
* @param[in]   __MODE__ :
*  @arg\b      UART2_TIMER_MODE
*  @arg\b      UART2_UART_MODE
* @return      None
* @note        S2TME=1 UART2 timer mode
* @par         Example
* @code
__DRV_URT2_TimerMode_Select(UART2_TIMER_MODE)
* @endcode
*******************************************************************************
*/

///@cond __DRV_URT2_TimerMode_Select
#define UART2_TIMER_MODE 1
#define UART2_UART_MODE 0
///@endcond
#define __DRV_URT2_TimerMode_Select(__MODE__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CFG_Page);\
        (__MODE__==1)?(S2CFG=S2CFG|(S2TME)):(S2CFG=S2CFG&(~S2TME));\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART2 Pure Timer Select
* @details     Set S2DOR
* @param[in]   __MODE__ :
*  @arg\b      UART2_8BIT_PURE_TIMER
*  @arg\b      UART2_16BIT_PURE_TIMER
* @return      None
* @note        S2DOR=1 S2TME=1 8bit pure timer
* @par         Example
* @code
__DRV_URT2_PureTimer_Select(UART2_8BIT_PURE_TIMER)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT2_PureTimer_Select
#define UART2_8BIT_PURE_TIMER 1
#define UART2_16BIT_PURE_TIMER 0
///@endcond

#define __DRV_URT2_PureTimer_Select(__MODE__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CFG_Page);\
        (__MODE__==1)?(S2CFG=S2CFG|(S2DOR)):(S2CFG=S2CFG&(~S2DOR));\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief       UART2 Enhance Baud Rate Select
* @details     Set S1MOD1
* @param[in]   __TIME__ :
*  @arg\b      UART2_DEFAULT_BAUD_RATE
*  @arg\b      UART2_DOUBLE_BAUD_RATE_X1
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_BaudRateX2_Select(Default_Baud_Rate)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT2_BaudRateX2_Select
#define UART2_DEFAULT_BAUD_RATE 0
#define UART2_DOUBLE_BAUD_RATE_X1 16
///@endcond

#define __DRV_URT2_BaudRateX2_Select(__TIME__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CFG_Page);\
        ((__TIME__^16)==0?(S2CFG=S2CFG|(S2MOD1)):(S2CFG=S2CFG&(~S2MOD1)));\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART2 S2BRG Clock Source Select
* @details     Set S2TX12
* @param[in]   __SOURCE__ :
*  @arg\b      S2BRG_CLOCK_SOURCE_SYSCLK_DIV_1
*  @arg\b      S2BRG_CLOCK_SOURCE_SYSCLK_DIV_12
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_BaudRateGeneratorClock_Select(S2BRG_CLOCK_SOURCE_SYSCLK_DIV_1)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT2_BaudRateGeneratorClock_Select
#define S2BRG_CLOCK_SOURCE_SYSCLK_DIV_1 64
#define S2BRG_CLOCK_SOURCE_SYSCLK_DIV_12 0
///@endcond
#define __DRV_URT2_BaudRateGeneratorClock_Select(__SOURCE__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CFG_Page);\
        (__SOURCE__)==0?(S2CFG=S2CFG&(~S2TX12)):(S2CFG=S2CFG|(S2TX12));\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief        UART2 Mode2 Pure Timer Clock Source Select
* @details      Set S2TX12 SM22
* @param[in]    __SOURCE__ :
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_T1OF
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_1
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_T0OF
* @return       None
* @note         None
* @par          Example
* @code
__DRV_URT2_Pure_Timer_Clock_Source_Select(PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_URT2_Pure_Timer_Clock_Source_Select(__SOURCE__)\
    MWT(\
        __DRV_URT2_SetS2TX12(__SOURCE__);\
        __DRV_URT2_SetSM22(__SOURCE__);\
    ;)
/**
*******************************************************************************
* @brief       UART2 Mode2 Pure Timer Clock Source Select
* @details     Set S2TX12
* @param[in]   __SOURCE__ :
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_T1OF
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_1
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_T0OF
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_SetS2TX12(PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_URT2_SetS2TX12(__SOURCE__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CFG_Page);\
        (__SOURCE__^0)==0?S2CFG=S2CFG&(~S2TX12):\
        (__SOURCE__^1)==0?S2CFG=S2CFG&(~S2TX12):\
        (__SOURCE__^2)==0?S2CFG=S2CFG|(S2TX12):\
        (__SOURCE__^3)==0?S2CFG=S2CFG|(S2TX12):_nop_();\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART2 Mode2 Pure Timer Clock Source Select
* @details     Set SM22
* @param[in]   __SOURCE__ :
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_T1OF
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_1
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_T0OF
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_SetSM22(PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_URT2_SetSM22(__SOURCE__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CON_Page);\
        (__SOURCE__^0)==0?(SM22=0):\
        (__SOURCE__^1)==0?(SM22=1):\
        (__SOURCE__^2)==0?(SM22=0):\
        (__SOURCE__^3)==0?(SM22=1):_nop_();\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART2 Enable S2BRG clock output control
* @details     Set S2CKOE
* @param[in]   __STATE__ :
*  @arg\b      MW_DISABLE
*  @arg\b      MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_Baud_Rate_Generator_clock_output_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/

#define __DRV_URT2_Baud_Rate_Generator_Clock_Output_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CFG_Page);\
        (__STATE__==1)?(S2CFG=S2CFG|(S2CKOE)):(S2CFG=S2CFG&(~S2CKOE));\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief       UART2 Enable S2BRG Operation
* @details     Set S2TR
* @param[in]   __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_BaudRateGenerator_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT2_BaudRateGenerator_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CFG_Page);\
        (__STATE__==1)?(S2CFG=S2CFG|(S2TR)):(S2CFG=S2CFG&(~S2TR));\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief      UART2 Baud Rate Generator Reload Register
* @details    Write S2BRT S2BRC
* @param[in] __RELOAD__ :
*  @arg\b    S2BRG_BRGRL_2400_1X_12000000_12T
*  @arg\b    S2BRG_BRGRL_2400_1X_12000000_1T
*  @arg\b    S2BRG_BRGRL_4800_1X_12000000_1T
*  @arg\b    S2BRG_BRGRL_9600_1X_12000000_1T
*  @arg\b    S2BRG_BRGRL_19200_1X_12000000_1T
*  @arg\b    S2BRG_BRGRL_38400_1X_12000000_1T
*  @arg\b    S2BRG_BRGRL_2400_2X_12000000_12T
*  @arg\b    S2BRG_BRGRL_4800_2X_12000000_12T
*  @arg\b    S2BRG_BRGRL_4800_2X_12000000_1T
*  @arg\b    S2BRG_BRGRL_9600_2X_12000000_1T
*  @arg\b    S2BRG_BRGRL_19200_2X_12000000_1T
*  @arg\b    S2BRG_BRGRL_38400_2X_12000000_1T
*  @arg\b    S2BRG_BRGRL_57600_2X_12000000_1T
*  @arg\b    S2BRG_BRGRL_115200_2X_11059200_1T
*  @arg\b    S2BRG_BRGRL_115200_1X_11059200_1T
*  @arg\b    S2BRG_BRGRL_57600_1X_11059200_1T
*  @arg\b    S2BRG_BRGRL_230400_2X_11059200_1T
*  @arg\b    S2BRG_BRGRL_250000_2X_12000000_1T
*  @arg\b    S2BRG_BRGRL_750000_2X_12000000_1T
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_BaudRateReloadReg_Write(S2BRG_BRGRL_9600_2X_12000000_1T)
* @endcode
*******************************************************************************
*/
#define __DRV_URT2_BaudRateReloadReg_Write(__RELOAD__)\
    MWT(\
        __DRV_SFR_PageIndex(S2BRT_Page);\
        (S2BRT=S2BRC=__RELOAD__);\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief        UART2 In Timer Mode Pure Timer 16 Bit Reload
* @details      Set S2BRC S2BRT
* @param[in]    __RELOAD__ :
*  @arg\b 0~65535
* @return       None
* @note         None
* @par          Example
* @code
__DRV_URT2_BaudRateReloadReg16Bit_Write(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_URT2_BaudRateReloadReg16Bit_Write(__RELOAD__)\
    MWT(\
        __DRV_SFR_PageIndex(S2BRT_Page);\
        S2BRT=HIBYTE(__RELOAD__);\
        S2BRC=LOBYTE(__RELOAD__)&0xff;\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief       UART2 Enable Reception
* @details     Set REN2
* @param[in]   __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_SerialReception_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT2_SerialReception_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CON_Page);\
        (__STATE__==1)?(REN2=1):(REN2=0);\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief       UART2 In Mode 2 Enable SYSCLK/32 SYSCLK/64 SYSCLK/96 SYSCLK/192
* @details     Set S2M0X3
* @param[in] __STATE__ :
*  @arg\b    MW_DISABLE
*  @arg\b    MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_BaudRateDiv3_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT2_BaudRateDiv3_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CFG_Page);\
        __STATE__==64?(S2CFG=S2CFG|(S2M0X3)):(S2CFG=S2CFG&(~S2M0X3));\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART2 In Mode 0 Mode 4 Enable SYSCLK/4 SYSCLK/12
* @details     Enable S2M0X3
* @param[in] __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_BaudRateSysclkDiv4_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/

#define __DRV_URT2_BaudRateSysclkDiv4_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CFG_Page);\
        __STATE__==64?(S2CFG=S2CFG|(S2M0X3)):(S2CFG=S2CFG&(~S2M0X3));\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief     UART2 In Timer Mode Enable Pure Timer Baudrate Generator Overflow to S2 Interrupt
* @details   Set TB82
* @param[in] __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_Pure_Timer_Baudrate_Generator_Overflow_to_S2_Interrupt_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT2_Pure_Timer_Baudrate_Generator_Overflow_to_S2_Interrupt_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CON_Page);\
        ((__STATE__)==1?(TB82=1):(TB82=0));\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief     UART2 In Timer Mode Enable Pure Timer External Pin Trigger to S2 Interrupt
* @details   Set REN2
* @param[in] __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_Pure_Timer_External_Pin_Trigger_to_S2_Interrupt_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT2_Pure_Timer_External_Pin_Trigger_to_S2_Interrupt_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CON_Page);\
        ((__STATE__)==1?(REN2=1):(REN2=0));\
        __DRV_SFR_PageIndex(0);\
    ;)


/**
*******************************************************************************
* @brief     UART2 In Timer Mode Enable Pure Timer External Pin Control Baudrate Generator
* @details   Set S2MOD1
* @param[in] __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_Pure_Timer_External_Pin_Control_Baudrate_Generator_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT2_Pure_Timer_External_Pin_Control_Baudrate_Generator_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CFG_Page);\
        ((__STATE__)==1?(S2CFG=S2CFG|(S2MOD1)):(S2CFG=S2CFG&(~S2MOD1)));\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief     UART2 In Timer Mode Enable Pure Timer External Pin Trigger Level
* @details   Set RB82
* @param[in] __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT2_Pure_Timer_External_Pin_Trigger_Level_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT2_Pure_Timer_External_Pin_Trigger_Level_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S2CON_Page);\
        ((__STATE__)==1?(RB82=1):(RB82=0));\
        __DRV_SFR_PageIndex(0);\
    ;)
///@cond
bool DRV_URT2_GetTI2(void);
bool DRV_URT2_GetRI2(void);
void DRV_URT2_ClearTI2(void);
void DRV_URT2_ClearRI2(void);
void DRV_URT2_ClearSM22(void);
void DRV_URT2_SetSM22(void);
void DRV_URT2_SetTXData9th(void);
void DRV_URT2_ClearTXData9th(void);
bool DRV_URT2_GetRXData9th(void);
void DRV_URT2_SendTXData(uint8_t TXData);
uint8_t DRV_URT2_ReceiveRXData(void);
///@endcond



#endif

