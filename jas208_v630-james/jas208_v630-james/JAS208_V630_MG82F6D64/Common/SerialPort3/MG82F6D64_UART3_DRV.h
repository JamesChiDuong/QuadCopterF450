/**
 ******************************************************************************
 *
 * @file        MG82F6D64_UART3_DRV.h
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

#ifndef MG82F6D64_UART3_DRV_H
#define MG82F6D64_UART3_DRV_H

/**
*******************************************************************************
* @brief        UART3 Enable Interrupt
* @details      UART3 Enable Interrupt
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_Interrupt_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT3_IT_Cmd(__STATE__)\
    __STATE__==0?(EIE2=EIE2&(~ES3)):(EIE2=EIE2|(ES3))
/**
*******************************************************************************
* @brief       UART3 Mode Select
* @details     Set SM33 SM03 SM13
* @param[in]   __MODE__ :
*  @arg\b      URT3_MODE0_SHIFT_REG
*  @arg\b      URT3_MODE1_8BIT
*  @arg\b      URT3_MODE2_9BIT
*  @arg\b      URT3_MODE3_9BIT
*  @arg\b      URT3_MODE4_SPI_MASTER
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_Mode_Select(URT3_Mode0_SHIFT_REG)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT3_Mode_Select
#define UART3_MODE0_SHIFT_REG 0x00
#define UART3_MODE1_8BIT 0x01
#define UART3_MODE2_9BIT 0x02
#define UART3_MODE3_9BIT 0x03
#define UART3_MODE4_SPI_MASTER 0x04
///@endcond
#define __DRV_URT3_Mode_Select(__MODE__)\
    MWT(\
        __DRV_URT3_SetSM33(__MODE__);\
        __DRV_URT3_SetSM03(__MODE__);\
        __DRV_URT3_SetSM13(__MODE__);\
    ;)
/**
*******************************************************************************
* @brief       UART3 PinMUX Select
* @details     Set S3PS0
* @param[in]   __RXTX__ :
*  @arg\b      UART3_RX_P36_TX_P37
*  @arg\b      UART3_RX_P65_TX_P66
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_PinMux_Select(UART3_RX_P32_TX_P33)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT3_PinMux_Select
#define UART3_RX_P36_TX_P37 0x00
#define UART3_RX_P65_TX_P66 0x01
///@endcond
#define __DRV_URT3_PinMux_Select(__RXTX__)\
    MWT(\
        __DRV_URT3_SetS3PS0(__RXTX__);\
    ;)

/**
*******************************************************************************
* @brief       UART3 Mode Select
* @details     Set SM33
* @param[in]   __MODE__ :
*  @arg\b      URT3_MODE0_SHIFT_REG
*  @arg\b      URT3_MODE1_8BIT
*  @arg\b      URT3_MODE2_9BIT
*  @arg\b      URT3_MODE3_9BIT
*  @arg\b      URT3_MODE4_SPI_MASTER
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_SetSM31(URT3_MODE1_8BIT)
* @endcode
*******************************************************************************
*/
#define __DRV_URT3_SetSM33(__MODE__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CFG_Page);\
        ((__MODE__^0x00)==0?(S3CFG=S3CFG&(~SM33)):\
        (__MODE__^0x01)==0?(S3CFG=S3CFG&(~SM33)):\
        (__MODE__^0x02)==0?(S3CFG=S3CFG&(~SM33)):\
        (__MODE__^0x03)==0?(S3CFG=S3CFG&(~SM33)):\
        (__MODE__^0x04)==0?(S3CFG=S3CFG|(SM33)):\
        (__MODE__^0x05)==0?(S3CFG=S3CFG|(SM33)):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART3 Mode Select
* @details     Set SM03
* @param[in]   __MODE__ :
*  @arg\b      URT3_MODE0_SHIFT_REG
*  @arg\b      URT3_MODE1_8BIT
*  @arg\b      URT3_MODE2_9BIT
*  @arg\b      URT3_MODE3_9BIT
*  @arg\b      URT3_MODE4_SPI_MASTER
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_SetSM03(URT3_MODE2_9BIT)
* @endcode
*******************************************************************************
*/
#define __DRV_URT3_SetSM03(__MODE__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CON_Page);\
        ((__MODE__^0x00)==0?(SM03=0):\
        (__MODE__^0x01)==0?(SM03=0):\
        (__MODE__^0x02)==0?(SM03=1):\
        (__MODE__^0x03)==0?(SM03=1):\
        (__MODE__^0x04)==0?(SM03=0):\
        (__MODE__^0x05)==0?(SM03=0):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART3 Mode Select
* @details     Set SM13
* @param[in]   __MODE__ :
*  @arg\b      URT3_MODE0_SHIFT_REG
*  @arg\b      URT3_MODE1_8BIT
*  @arg\b      URT3_MODE2_9BIT
*  @arg\b      URT3_MODE3_9BIT
*  @arg\b      URT3_MODE4_SPI_MASTER
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_SetSM13(URT3_MODE3_9BIT)
* @endcode
*******************************************************************************
*/
#define __DRV_URT3_SetSM13(__MODE__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CON_Page);\
        ((__MODE__^0x00)==0?(SM13=0):\
        (__MODE__^0x01)==0?(SM13=1):\
        (__MODE__^0x02)==0?(SM13=0):\
        (__MODE__^0x03)==0?(SM13=1):\
        (__MODE__^0x04)==0?(SM13=0):\
        (__MODE__^0x05)==0?(SM13=1):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART3 PinMUX Select
* @details     Set S3PS0
* @param[in]   __RXTX__ :
*  @arg\b      UART3_RX_P36_TX_P37
*  @arg\b      UART3_RX_P65_TX_P66
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_SetS3PS0(UART3_RX_P36_TX_P37)
* @endcode
*******************************************************************************
*/
#define __DRV_URT3_SetS3PS0(__RXTX__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR13_Page);\
        ((__RXTX__^0x00)==0?AUXR13=AUXR13&(~S3PS0):\
        (__RXTX__^0x01)==0?AUXR13=AUXR13|(S3PS0):_nop_());\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART3 Data Order Select
* @details     Set S3DOR
* @param[in]   __ORDER__ :
*  @arg\b      UART3_DATA_ORDER_LSB
*  @arg\b      UART3_DATA_ORDER_MSB
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_DataOrder_Select(UART3_DATA_ORDER_LSB)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT3_DataOrder_Select
#define UART3_DATA_ORDER_LSB 0x08
#define UART3_DATA_ORDER_MSB 0x00
///@endcond
#define __DRV_URT3_DataOrder_Select(__ORDER__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CFG_Page);\
        (__ORDER__==0x08)?(S3CFG=S3CFG|(S3DOR)):(S3CFG=S3CFG&(~S3DOR));\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART3 Timer Mode Select
* @details     Set S3TME
* @param[in]   __MODE__ :
*  @arg\b      UART3_TIMER_MODE
*  @arg\b      UART3_UART_MODE
* @return      None
* @note        S3TME=1 UART3 timer mode
* @par         Example
* @code
__DRV_URT3_TimerMode_Select(UART3_TIMER_MODE)
* @endcode
*******************************************************************************
*/

///@cond __DRV_URT3_TimerMode_Select
#define UART3_TIMER_MODE 1
#define UART3_UART_MODE 0
///@endcond
#define __DRV_URT3_TimerMode_Select(__MODE__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CFG_Page);\
        (__MODE__==1)?(S3CFG=S3CFG|(S3TME)):(S3CFG=S3CFG&(~S3TME));\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART3 Pure Timer Select
* @details     Set S3DOR
* @param[in]   __MODE__ :
*  @arg\b      UART3_8BIT_PURE_TIMER
*  @arg\b      UART3_16BIT_PURE_TIMER
* @return      None
* @note        S3DOR=1 S3TME=1 8bit pure timer
* @par         Example
* @code
__DRV_URT3_PureTimer_Select(UART3_8BIT_PURE_TIMER)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT3_PureTimer_Select
#define UART3_8BIT_PURE_TIMER 1
#define UART3_16BIT_PURE_TIMER 0
///@endcond

#define __DRV_URT3_PureTimer_Select(__MODE__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CFG_Page);\
        (__MODE__==1)?(S3CFG=S3CFG|(S3DOR)):(S3CFG=S3CFG&(~S3DOR));\
        __DRV_SFR_PageIndex(0);\
    ;)


/**
*******************************************************************************
* @brief       UART3 Enhance Baud Rate Select
* @details     Set S1MOD1
* @param[in]   __TIME__ :
*  @arg\b      UART3_DEFAULT_BAUD_RATE
*  @arg\b      UART3_DOUBLE_BAUD_RATE_X1
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_BaudRateX2_Select(Default_Baud_Rate)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT3_BaudRateX2_Select
#define UART3_DEFAULT_BAUD_RATE 0
#define UART3_DOUBLE_BAUD_RATE_X1 16
///@endcond

#define __DRV_URT3_BaudRateX2_Select(__TIME__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CFG_Page);\
        ((__TIME__^16)==0?(S3CFG=S3CFG|(S3MOD1)):(S3CFG=S3CFG&(~S3MOD1)));\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART3 S3BRG Clock Source Select
* @details     Set S3TX12
* @param[in]   __SOURCE__ :
*  @arg\b      S3BRG_CLOCK_SOURCE_SYSCLK_DIV_1
*  @arg\b      S3BRG_CLOCK_SOURCE_SYSCLK_DIV_12
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_BaudRateGeneratorClock_Select(S3BRG_CLOCK_SOURCE_SYSCLK_DIV_1)
* @endcode
*******************************************************************************
*/
///@cond __DRV_URT3_BaudRateGeneratorClock_Select
#define S3BRG_CLOCK_SOURCE_SYSCLK_DIV_1 64
#define S3BRG_CLOCK_SOURCE_SYSCLK_DIV_12 0
///@endcond
#define __DRV_URT3_BaudRateGeneratorClock_Select(__SOURCE__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CFG_Page);\
        (__SOURCE__)==0?(S3CFG=S3CFG&(~S3TX12)):(S3CFG=S3CFG|(S3TX12));\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief        UART3 Mode2 Pure Timer Clock Source Select
* @details      Set S3TX12 SM23
* @param[in]    __SOURCE__ :
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_T1OF
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_1
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_T0OF
* @return       None
* @note         None
* @par          Example
* @code
__DRV_URT3_Pure_Timer_Clock_Source_Select(PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_URT3_Pure_Timer_Clock_Source_Select(__SOURCE__)\
    MWT(\
        __DRV_URT3_SetS3TX12(__SOURCE__);\
        __DRV_URT3_SetSM23(__SOURCE__);\
    ;)
/**
*******************************************************************************
* @brief       UART3 Mode2 Pure Timer Clock Source Select
* @details     Set S3TX12
* @param[in]   __SOURCE__ :
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_T1OF
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_1
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_T0OF
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_SetS3TX12(PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_URT3_SetS3TX12(__SOURCE__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CFG_Page);\
        (__SOURCE__^0)==0?S3CFG=S3CFG&(~S3TX12):\
        (__SOURCE__^1)==0?S3CFG=S3CFG&(~S3TX12):\
        (__SOURCE__^2)==0?S3CFG=S3CFG|(S3TX12):\
        (__SOURCE__^3)==0?S3CFG=S3CFG|(S3TX12):_nop_();\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART3 Mode2 Pure Timer Clock Source Select
* @details     Set SM23
* @param[in]   __SOURCE__ :
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_T1OF
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_1
*  @arg\b       PURE_TIMER_CLOCK_SOURCE_T0OF
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_SetSM23(PURE_TIMER_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_URT3_SetSM23(__SOURCE__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CON_Page);\
        (__SOURCE__^0)==0?(SM23=0):\
        (__SOURCE__^1)==0?(SM23=1):\
        (__SOURCE__^2)==0?(SM23=0):\
        (__SOURCE__^3)==0?(SM23=1):_nop_();\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART3 Enable S3BRG clock output control
* @details     Set S3CKOE
* @param[in]   __STATE__ :
*  @arg\b      MW_DISABLE
*  @arg\b      MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_Baud_Rate_Generator_clock_output_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/

#define __DRV_URT3_Baud_Rate_Generator_Clock_Output_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CFG_Page);\
        (__STATE__==1)?(S3CFG=S3CFG|(S3CKOE)):(S3CFG=S3CFG&(~S3CKOE));\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief       UART3 Enable S3BRG Operation
* @details     Set S3TR
* @param[in]   __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_BaudRateGenerator_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT3_BaudRateGenerator_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CFG_Page);\
        (__STATE__==1)?(S3CFG=S3CFG|(S3TR)):(S3CFG=S3CFG&(~S3TR));\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief      UART3 Baud Rate Generator Reload Register
* @details    Write S3BRT S3BRC
* @param[in] __RELOAD__ :
*  @arg\b    S3BRG_BRGRL_2400_1X_12000000_12T
*  @arg\b    S3BRG_BRGRL_2400_1X_12000000_1T
*  @arg\b    S3BRG_BRGRL_4800_1X_12000000_1T
*  @arg\b    S3BRG_BRGRL_9600_1X_12000000_1T
*  @arg\b    S3BRG_BRGRL_19200_1X_12000000_1T
*  @arg\b    S3BRG_BRGRL_38400_1X_12000000_1T
*  @arg\b    S3BRG_BRGRL_2400_2X_12000000_12T
*  @arg\b    S3BRG_BRGRL_4800_2X_12000000_12T
*  @arg\b    S3BRG_BRGRL_4800_2X_12000000_1T
*  @arg\b    S3BRG_BRGRL_9600_2X_12000000_1T
*  @arg\b    S3BRG_BRGRL_19200_2X_12000000_1T
*  @arg\b    S3BRG_BRGRL_38400_2X_12000000_1T
*  @arg\b    S3BRG_BRGRL_57600_2X_12000000_1T
*  @arg\b    S3BRG_BRGRL_115200_2X_11059200_1T
*  @arg\b    S3BRG_BRGRL_115200_1X_11059200_1T
*  @arg\b    S3BRG_BRGRL_57600_1X_11059200_1T
*  @arg\b    S3BRG_BRGRL_230400_2X_11059200_1T
*  @arg\b    S3BRG_BRGRL_250000_2X_12000000_1T
*  @arg\b    S3BRG_BRGRL_750000_2X_12000000_1T
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_BaudRateReloadReg_Write(S3BRG_BRGRL_9600_2X_12000000_1T)
* @endcode
*******************************************************************************
*/
#define __DRV_URT3_BaudRateReloadReg_Write(__RELOAD__)\
    MWT(\
        __DRV_SFR_PageIndex(S3BRT_Page);\
        (S3BRT=S3BRC=__RELOAD__);\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief        UART3 In Timer Mode Pure Timer 16 Bit Reload
* @details      Set S3BRC S3BRT
* @param[in]    __RELOAD__ :
*  @arg\b 0~65535
* @return       None
* @note         None
* @par          Example
* @code
__DRV_URT3_BaudRateReloadReg16Bit_Write(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_URT3_BaudRateReloadReg16Bit_Write(__RELOAD__)\
    MWT(\
        __DRV_SFR_PageIndex(S3BRT_Page);\
        S3BRT=HIBYTE(__RELOAD__);\
        S3BRC=LOBYTE(__RELOAD__)&0xff;\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief       UART3 Enable Reception
* @details     Set REN3
* @param[in]   __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_SerialReception_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT3_SerialReception_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CON_Page);\
        (__STATE__==1)?(REN3=1):(REN3=0);\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief       UART3 In Mode 2 Enable SYSCLK/32 SYSCLK/64 SYSCLK/96 SYSCLK/192
* @details     Set S3M0X3
* @param[in] __STATE__ :
*  @arg\b    MW_DISABLE
*  @arg\b    MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_BaudRateDiv3_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT3_BaudRateDiv3_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CFG_Page);\
        __STATE__==64?(S3CFG=S3CFG|(S3M0X3)):(S3CFG=S3CFG&(~S3M0X3));\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief       UART3 In Mode 0 Mode 4 Enable SYSCLK/4 SYSCLK/12
* @details     Enable S3M0X3
* @param[in] __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_BaudRateSysclkDiv4_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/

#define __DRV_URT3_BaudRateSysclkDiv4_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CFG_Page);\
        __STATE__==64?(S3CFG=S3CFG|(S3M0X3)):(S3CFG=S3CFG&(~S3M0X3));\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief     UART3 In Timer Mode Enable Pure Timer Baudrate Generator Overflow to S3 Interrupt
* @details   Set TB83
* @param[in] __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_Pure_Timer_Baudrate_Generator_Overflow_to_S3_Interrupt_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT3_Pure_Timer_Baudrate_Generator_Overflow_to_S3_Interrupt_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CON_Page);\
        ((__STATE__)==1?(TB83=1):(TB83=0));\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief     UART3 In Timer Mode Enable Pure Timer External Pin Trigger to S3 Interrupt
* @details   Set REN3
* @param[in] __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_Pure_Timer_External_Pin_Trigger_to_S3_Interrupt_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT3_Pure_Timer_External_Pin_Trigger_to_S3_Interrupt_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CON_Page);\
        ((__STATE__)==1?(REN3=1):(REN3=0));\
        __DRV_SFR_PageIndex(0);\
    ;)


/**
*******************************************************************************
* @brief     UART3 In Timer Mode Enable Pure Timer External Pin Control Baudrate Generator
* @details   Set S3MOD1
* @param[in] __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_Pure_Timer_External_Pin_Control_Baudrate_Generator_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT3_Pure_Timer_External_Pin_Control_Baudrate_Generator_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CFG_Page);\
        ((__STATE__)==1?(S3CFG=S3CFG|(S3MOD1)):(S3CFG=S3CFG&(~S3MOD1)));\
        __DRV_SFR_PageIndex(0);\
    ;)

/**
*******************************************************************************
* @brief     UART3 In Timer Mode Enable Pure Timer External Pin Trigger Level
* @details   Set RB82
* @param[in] __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return      None
* @note        None
* @par         Example
* @code
__DRV_URT3_Pure_Timer_External_Pin_Trigger_Level_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_URT3_Pure_Timer_External_Pin_Trigger_Level_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(S3CON_Page);\
        ((__STATE__)==1?(RB83=1):(RB83=0));\
        __DRV_SFR_PageIndex(0);\
    ;)
///@cond
bool DRV_URT3_GetTI3(void);
bool DRV_URT3_GetRI3(void);
void DRV_URT3_ClearTI3(void);
void DRV_URT3_ClearRI3(void);
void DRV_URT3_ClearSM23(void);
void DRV_URT3_SetSM23(void);
void DRV_URT3_SetTXData9th(void);
void DRV_URT3_ClearTXData9th(void);
bool DRV_URT3_GetRXData9th(void);
void DRV_URT3_SendTXData(uint8_t TXData);
uint8_t DRV_URT3_ReceiveRXData(void);
///@endcond



#endif

