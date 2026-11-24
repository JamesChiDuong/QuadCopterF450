/**
 ******************************************************************************
 *
 * @file        MG82F6D64_INT_DRV.H
 *
 * @brief       This is the C code format driver head file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     v1.02
 * @date        2022/06/09
 * @copyright   Copyright (c) 2019 MegaWin Technology Co., Ltd.
 *              All rights reserved.
 *
 ******************************************************************************
 * @par         Disclaimer
 *      The Demo software is provided "AS IF"  without any warranty, either
 *      expressed or implied, including, but not limited to, the implied warranties
 *      of merchantability and fitness for a particular purpose.  The author will
 *      not be liable for any special, incidental, consequential or indirect
 *      damages due to loss of data or any other reason.
 *      These statements agree with the world wide and local dictated laws about
 *      authorship and violence against these laws.
 ******************************************************************************
 ******************************************************************************
 */

 
#ifndef _MG82F6D64_INT_DRV_H
#define _MG82F6D64_INT_DRV_H


/**
 *******************************************************************************
 * @brief       Global Enables all interrupts
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_ITEA_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT_ITEA_Enable()        EA = MW_ENABLE


/**
 *******************************************************************************
 * @brief       Global Disables all interrupts
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_ITEA_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT_ITEA_Disable()        EA = MW_DISABLE


/**
 *******************************************************************************
 * @brief       Global Enable/Disable all interrupts
 * @details
 * @param[in]   __STATE__ : config EA control bit
 *  @arg\b      MW_DISABLE : Set global interrupts disable (Default)
 *  @arg\b      MW_ENABLE : Set global interrupts enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_ITEA_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT_ITEA_Cmd(__STATE__)        EA = __STATE__


/**
 *******************************************************************************
 * @brief       Enable nINT0 interrupt
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT0_IT_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT0_IT_Enable()        EX0 = MW_ENABLE


/**
 *******************************************************************************
 * @brief       Disable nINT0 interrupt
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT0_IT_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT0_IT_Disable()        EX0 = MW_DISABLE


/**
 *******************************************************************************
 * @brief       Enable/Disable nINT0 interrupt
 * @details
 * @param[in]   __STATE__ : config EX0 control bit
 *  @arg\b      MW_DISABLE : Set external interrupt 0 disable (Default)
 *  @arg\b      MW_ENABLE : Set external interrupt 0 enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT0_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT0_IT_Cmd(__STATE__)        EX0 = __STATE__


/**
 *******************************************************************************
 * @brief       Set nINT0 interrupt flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT0_SetFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT0_SetFlag()        IE0 = MW_ENABLE


/**
 *******************************************************************************
 * @brief       Get nINT0 interrupt flag
 * @details
 * @return      IE0 : return IE0 bit status
 * @note
 * @par         Example
 * @code
 *    __DRV_INT0_GetFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT0_GetFlag()        IE0


/**
 *******************************************************************************
 * @brief       Clear nINT0 interrupt flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT0_ClearFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT0_ClearFlag()        IE0 = MW_DISABLE


/**
 *******************************************************************************
 * @brief       Set nINT0 input pin source
 * @details
 * @param[in]   __SELECT__ : Set external interrupt 0 input pin source
 *  @arg\b      INT0_INPUT_PIN_SELECT_P32 : Set external interrupt 0 input pin source to P32 (Default)
 *  @arg\b      INT0_INPUT_PIN_SELECT_P30 : Set external interrupt 0 input pin source to P30
 *  @arg\b      INT0_INPUT_PIN_SELECT_P34 : Set external interrupt 0 input pin source to P34
 *  @arg\b      INT0_INPUT_PIN_SELECT_P40 : Set external interrupt 0 input pin source to P40
 *  @arg\b      INT0_INPUT_PIN_SELECT_P10 : Set external interrupt 0 input pin source to P10
 *  @arg\b      INT0_INPUT_PIN_SELECT_P14 : Set external interrupt 0 input pin source to P14
 *  @arg\b      INT0_INPUT_PIN_SELECT_P36 : Set external interrupt 0 input pin source to P36
 *  @arg\b      INT0_INPUT_PIN_SELECT_P05 : Set external interrupt 0 input pin source to P05
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT0_PinMux_Select(INT0_INPUT_PIN_SELECT_P40);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT0_PinMux_Select
#define INT0_INPUT_PIN_SELECT_P32     0x0000
#define INT0_INPUT_PIN_SELECT_P30     0x0010
#define INT0_INPUT_PIN_SELECT_P34     0x0020
#define INT0_INPUT_PIN_SELECT_P40     0x0030
#define INT0_INPUT_PIN_SELECT_P10     0x4000
#define INT0_INPUT_PIN_SELECT_P14     0x4010
#define INT0_INPUT_PIN_SELECT_P36     0x4020
#define INT0_INPUT_PIN_SELECT_P05     0x4030
/// @endcond
#define __DRV_INT0_PinMux_Select(__SELECT__)\
    MWT(\
        XICFG = (XICFG & ~(INT0IS1 | INT0IS0)) | LOBYTE(__SELECT__);\
        __DRV_SFR_PageIndex(XICFG1_Page);\
        XICFG1 = (XICFG1 & ~INT0IS2) | HIBYTE(__SELECT__);\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Set nINT0 event trigger type
 * @details
 * @param[in]   __SELECT__ : Set external interrupt 0 event trigger type
 *  @arg\b      INT0_TRIGGER_TYPE_LOW_LEVEL : Set external interrupt 0 event trigger type to low level (Default)
 *  @arg\b      INT0_TRIGGER_TYPE_HIGH_LEVEL : Set external interrupt 0 event trigger type to high level
 *  @arg\b      INT0_TRIGGER_TYPE_FALLING_EDGE : Set external interrupt 0 event trigger type to falling edge
 *  @arg\b      INT0_TRIGGER_TYPE_RISING_EDGE : Set external interrupt 0 event trigger type to rising edge
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT0_TriggerType_Select(INT0_TRIGGER_TYPE_FALLING_EDGE);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT0_TriggerType_Select
#define INT0_TRIGGER_TYPE_LOW_LEVEL       0x0000
#define INT0_TRIGGER_TYPE_HIGH_LEVEL      0x0001
#define INT0_TRIGGER_TYPE_FALLING_EDGE    0x0100
#define INT0_TRIGGER_TYPE_RISING_EDGE     0x0101
/// @endcond
#define __DRV_INT0_TriggerType_Select(__SELECT__)\
    MWT(\
        AUXR0 = (AUXR0 & ~INT0H) | LOBYTE(__SELECT__);\
        TCON = (TCON & ~IT0) | HIBYTE(__SELECT__);\
    )


/**
 *******************************************************************************
 * @brief       Set nINT0 interrupt priority level
 * @details
 * @param[in]   __SELECT__ : Set external interrupt 0 priority level
 *  @arg\b      INT0_PRIORITY_LOWEST : Set external interrupt 0 priority to lowest level (Default)
 *  @arg\b      INT0_PRIORITY_MIDDLE_LOW : Set external interrupt 0 priority to middle low level
 *  @arg\b      INT0_PRIORITY_MIDDLE_HIGH : Set external interrupt 0 priority to middle high level
 *  @arg\b      INT0_PRIORITY_HIGHEST : Set external interrupt 0 priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT0_Priority_Select(INT0_PRIORITY_HIGHEST);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT0_Priority_Select
#define INT0_PRIORITY_LOWEST          0x0000
#define INT0_PRIORITY_MIDDLE_LOW      0x0001
#define INT0_PRIORITY_MIDDLE_HIGH     0x0100
#define INT0_PRIORITY_HIGHEST         0x0101
/// @endcond
#define __DRV_INT0_Priority_Select(__SELECT__)\
    MWT(\
        IP0L = (IP0L & ~PX0L) | LOBYTE(__SELECT__);\
        IP0H = (IP0H & ~PX0H) | HIBYTE(__SELECT__);\
    )


/**
 *******************************************************************************
 * @brief       Set nINT0 filter mode type
 * @details
 * @param[in]   __SELECT__ : Set external interrupt 0 filter mode type
 *  @arg\b      INT0_FILTER_DISABLE : Disable external interrupt 0 filter mode type (Default)
 *  @arg\b      INT0_FILTER_SYSCLK_X3 : Set external interrupt 0 filter mode type to SYSCLKx3
 *  @arg\b      INT0_FILTER_SYSCLKDIV6X3 : Set external interrupt 0 filter mode type to SYSCLK/6x3
 *  @arg\b      INT0_FILTER_S0TOF : Set external interrupt 0 filter mode type to S0TOF
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT0_FilterMode_Select(INT0_FILTER_SYSCLKx3);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT0_FilterMode_Select
#define INT0_FILTER_DISABLE       0x0000
#define INT0_FILTER_SYSCLK_X3     0x0001
#define INT0_FILTER_SYSCLK_DIV6X3 0x0100
#define INT0_FILTER_S0TOF         0x0101
/// @endcond
#define __DRV_INT0_FilterMode_Select(__SELECT__)\
    MWT(\
        XICFG = (XICFG & ~X0FLT) | LOBYTE(__SELECT__);\
        __DRV_SFR_PageIndex(XICFG1_Page);\
        XICFG1 = (XICFG1 & ~X0FLT1) | HIBYTE(__SELECT__);\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Enable nINT1 interrupt
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT1_IT_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT1_IT_Enable()        EX1 = MW_ENABLE


/**
 *******************************************************************************
 * @brief       Disable nINT1 interrupt
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT1_IT_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT1_IT_Disable()        EX1 = MW_DISABLE


/**
 *******************************************************************************
 * @brief       Enable/Disable nINT1 interrupt
 * @details
 * @param[in]   __STATE__ : config EX1 control bit
 *  @arg\b      MW_DISABLE : Set external interrupt 1 disable (Default)
 *  @arg\b      MW_ENABLE : Set external interrupt 1 enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT1_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT1_IT_Cmd(__STATE__)        EX1 = __STATE__


/**
 *******************************************************************************
 * @brief       Set nINT1 interrupt flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT1_SetFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT1_SetFlag()        IE1 = MW_ENABLE


/**
 *******************************************************************************
 * @brief       Get nINT1 interrupt flag
 * @details
 * @return      IE1 : return IE1 bit status
 * @note
 * @par         Example
 * @code
 *    __DRV_INT1_GetFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT1_GetFlag()        IE1


/**
 *******************************************************************************
 * @brief       Clear nINT1 interrupt flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    DRV_INT1_ClearFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT1_ClearFlag()        IE1 = MW_DISABLE


/**
 *******************************************************************************
 * @brief       Set nINT1 input pin source
 * @details
 * @param[in]   __SELECT__ : Set external interrupt 1 input pin source
 *  @arg\b      INT1_INPUT_PIN_SELECT_P33 : Set external interrupt 1 input pin source to P33 (Default)
 *  @arg\b      INT1_INPUT_PIN_SELECT_P31 : Set external interrupt 1 input pin source to P31
 *  @arg\b      INT1_INPUT_PIN_SELECT_P35 : Set external interrupt 1 input pin source to P35
 *  @arg\b      INT1_INPUT_PIN_SELECT_P41 : Set external interrupt 1 input pin source to P41
 *  @arg\b      INT1_INPUT_PIN_SELECT_P45 : Set external interrupt 1 input pin source to P45
 *  @arg\b      INT1_INPUT_PIN_SELECT_P15 : Set external interrupt 1 input pin source to P15
 *  @arg\b      INT1_INPUT_PIN_SELECT_P37 : Set external interrupt 1 input pin source to P37
 *  @arg\b      INT1_INPUT_PIN_SELECT_P07 : Set external interrupt 1 input pin source to P07
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT1_PinMux_Select(INT1_INPUT_PIN_SELECT_P31);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT1_PinMux_Select
#define INT1_INPUT_PIN_SELECT_P33     0x0000
#define INT1_INPUT_PIN_SELECT_P31     0x0040
#define INT1_INPUT_PIN_SELECT_P35     0x0080
#define INT1_INPUT_PIN_SELECT_P41     0x00C0
#define INT1_INPUT_PIN_SELECT_P45     0x8000
#define INT1_INPUT_PIN_SELECT_P15     0x8040
#define INT1_INPUT_PIN_SELECT_P37     0x8080
#define INT1_INPUT_PIN_SELECT_P07     0x80C0
/// @endcond
#define __DRV_INT1_PinMux_Select(__SELECT__)\
    MWT(\
        XICFG = (XICFG & ~(INT1IS1 | INT1IS0)) | LOBYTE(__SELECT__);\
        __DRV_SFR_PageIndex(XICFG1_Page);\
        XICFG1 = (XICFG1 & ~INT1IS2) | HIBYTE(__SELECT__);\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Set nINT1 event trigger type
 * @details
 * @param[in]   __SELECT__ : Set external interrupt 1 event trigger type
 *  @arg\b      INT1_TRIGGER_TYPE_LOW_LEVEL : Set external interrupt 1 event trigger type to low level (Default)
 *  @arg\b      INT1_TRIGGER_TYPE_HIGH_LEVEL : Set external interrupt 1 event trigger type to high level
 *  @arg\b      INT1_TRIGGER_TYPE_FALLING_EDGE : Set external interrupt 1 event trigger type to falling edge
 *  @arg\b      INT1_TRIGGER_TYPE_RISING_EDGE : Set external interrupt 1 event trigger type to rising edge
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT1_TriggerType_Select(INT1_TRIGGER_TYPE_RISING_EDGE);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT1_TriggerType_Select
#define INT1_TRIGGER_TYPE_LOW_LEVEL       0x0000
#define INT1_TRIGGER_TYPE_HIGH_LEVEL      0x0002
#define INT1_TRIGGER_TYPE_FALLING_EDGE    0x0400
#define INT1_TRIGGER_TYPE_RISING_EDGE     0x0402
/// @endcond
#define __DRV_INT1_TriggerType_Select(__SELECT__)\
    MWT(\
        AUXR0 = (AUXR0 & ~INT1H) | LOBYTE(__SELECT__);\
        TCON = (TCON & ~IT1) | HIBYTE(__SELECT__);\
    )


/**
 *******************************************************************************
 * @brief       Set nINT1 interrupt priority level
 * @details
 * @param[in]   __SELECT__ : Set external interrupt 1 priority level
 *  @arg\b      INT1_PRIORITY_LOWEST : Set external interrupt 1 priority to lowest level (Default)
 *  @arg\b      INT1_PRIORITY_MIDDLE_LOW : Set external interrupt 1 priority to middle low level
 *  @arg\b      INT1_PRIORITY_MIDDLE_HIGH : Set external interrupt 1 priority to middle high level
 *  @arg\b      INT1_PRIORITY_HIGHEST : Set external interrupt 1 priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT1_Priority_Select(INT1_PRIORITY_MIDDLE_LOW);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT1_Priority_Select
#define INT1_PRIORITY_LOWEST          0x0000
#define INT1_PRIORITY_MIDDLE_LOW      0x0004
#define INT1_PRIORITY_MIDDLE_HIGH     0x0400
#define INT1_PRIORITY_HIGHEST         0x0404
/// @endcond
#define __DRV_INT1_Priority_Select(__SELECT__)\
    MWT(\
        IP0L = (IP0L & ~PX1L) | LOBYTE(__SELECT__);\
        IP0H = (IP0H & ~PX1H) | HIBYTE(__SELECT__);\
    )


/**
 *******************************************************************************
 * @brief       Set nINT1 filter mode type
 * @details
 * @param[in]   __SELECT__ : Set external interrupt 1 filter mode type
 *  @arg\b      INT1_FILTER_DISABLE : Disable external interrupt 1 filter mode type (Default)
 *  @arg\b      INT1_FILTER_SYSCLK_X3 : Set external interrupt 1 filter mode type to SYSCLKx3
 *  @arg\b      INT1_FILTER_SYSCLKDIV6X3 : Set external interrupt 1 filter mode type to SYSCLK/6x3
 *  @arg\b      INT1_FILTER_S0TOF : Set external interrupt 1 filter mode type to S0TOF
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT1_FilterMode_Select(INT1_FILTER_DISABLE);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT1_FilterMode_Select
#define INT1_FILTER_DISABLE       0x0000
#define INT1_FILTER_SYSCLK_X3     0x0002
#define INT1_FILTER_SYSCLK_DIV6X3 0x0200
#define INT1_FILTER_S0TOF         0x0202
/// @endcond
#define __DRV_INT1_FilterMode_Select(__SELECT__)\
    MWT(\
        XICFG = (XICFG & ~X1FLT) | LOBYTE(__SELECT__);\
        __DRV_SFR_PageIndex(XICFG1_Page);\
        XICFG1 = (XICFG1 & ~X1FLT1) | HIBYTE(__SELECT__);\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Enable nINT2 interrupt
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT2_IT_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT2_IT_Enable()        EX2 = MW_ENABLE


/**
 *******************************************************************************
 * @brief       Disable nINT2 interrupt
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT2_IT_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT2_IT_Disable()        EX2 = MW_DISABLE


/**
 *******************************************************************************
 * @brief       Enable/Disable nINT2 interrupt
 * @details
 * @param[in]   __STATE__ : config EX2 control bit
 *  @arg\b      MW_DISABLE : Set external interrupt 2 disable (Default)
 *  @arg\b      MW_ENABLE : Set external interrupt 2 enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT2_IT_Cmd(MW_DISABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT2_IT_Cmd(__STATE__)        EX2 = __STATE__


/**
 *******************************************************************************
 * @brief       Set nINT2 interrupt flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT2_SetFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT2_SetFlag()        IE2 = MW_ENABLE


/**
 *******************************************************************************
 * @brief       Get nINT2 interrupt flag
 * @details 
 * @return      IE2 : return IE2 bit status
 * @note
 * @par         Example
 * @code
 *    __DRV_INT2_GetFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT2_GetFlag()        IE2


/**
 *******************************************************************************
 * @brief       Clear nINT2 interrupt flag
 * @details 
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT2_ClearFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT2_ClearFlag()        IE2 = MW_DISABLE


/**
 *******************************************************************************
 * @brief       Set nINT2 input pin source
 * @details
 * @param[in]   __SELECT__ : Set external interrupt 2 input pin source
 *  @arg\b      INT2_INPUT_PIN_SELECT_P43 : Set external interrupt 2 input pin source to P43 (Default)
 *  @arg\b      INT2_INPUT_PIN_SELECT_P20 : Set external interrupt 2 input pin source to P20
 *  @arg\b      INT2_INPUT_PIN_SELECT_P11 : Set external interrupt 2 input pin source to P11
 *  @arg\b      INT2_INPUT_PIN_SELECT_P25 : Set external interrupt 2 input pin source to P25
 *  @arg\b      INT2_INPUT_PIN_SELECT_P44 : Set external interrupt 2 input pin source to P44
 *  @arg\b      INT2_INPUT_PIN_SELECT_P60 : Set external interrupt 2 input pin source to P60
 *  @arg\b      INT2_INPUT_PIN_SELECT_P01 : Set external interrupt 2 input pin source to P01
 *  @arg\b      INT2_INPUT_PIN_SELECT_P62 : Set external interrupt 2 input pin source to P62
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT2_PinMux_Select(INT2_INPUT_PIN_SELECT_P20);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT2_PinMux_Select
#define INT2_INPUT_PIN_SELECT_P43     0x0000
#define INT2_INPUT_PIN_SELECT_P20     0x0010
#define INT2_INPUT_PIN_SELECT_P11     0x0020
#define INT2_INPUT_PIN_SELECT_P25     0x0030
#define INT2_INPUT_PIN_SELECT_P44     0x0100
#define INT2_INPUT_PIN_SELECT_P60     0x0110
#define INT2_INPUT_PIN_SELECT_P01     0x0120
#define INT2_INPUT_PIN_SELECT_P62     0x0130

/// @endcond
#define __DRV_INT2_PinMux_Select(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(XICFG1_Page);\
        XICFG1 = (XICFG1 & ~(INT2IS1 | INT2IS0)) | LOBYTE(__SELECT__);\
        __DRV_SFR_PageIndex(XICFG2_Page);\
        XICFG2 = XICFG2 & (~INT2IS2) | HIBYTE(__SELECT__);\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Set nINT2 event trigger type
 * @details
 * @param[in]   __SELECT__ : Set external interrupt 2 event trigger type
 *  @arg\b      INT2_TRIGGER_TYPE_LOW_LEVEL : Set external interrupt 2 event trigger type to low level (Default)
 *  @arg\b      INT2_TRIGGER_TYPE_HIGH_LEVEL : Set external interrupt 2 event trigger type to high level
 *  @arg\b      INT2_TRIGGER_TYPE_FALLING_EDGE : Set external interrupt 2 event trigger type to falling edge
 *  @arg\b      INT2_TRIGGER_TYPE_RISING_EDGE : Set external interrupt 2 event trigger type to rising edge
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT2_TriggerType_Select(INT2_TRIGGER_TYPE_HIGH_LEVEL);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT2_TriggerType_Select
#define INT2_TRIGGER_TYPE_LOW_LEVEL       0x00
#define INT2_TRIGGER_TYPE_HIGH_LEVEL      0x08
#define INT2_TRIGGER_TYPE_FALLING_EDGE    0x01
#define INT2_TRIGGER_TYPE_RISING_EDGE     0x09
/// @endcond
#define __DRV_INT2_TriggerType_Select(__SELECT__)\
    MWT(\
        XICON = (XICON & ~INT2_TRIGGER_TYPE_RISING_EDGE) | __SELECT__;\
    )


/**
 *******************************************************************************
 * @brief       Set nINT2 interrupt priority level
 * @details
 * @param[in]   __SELECT__ : Set external interrupt 2 priority level
 *  @arg\b      INT2_PRIORITY_LOWEST : Set external interrupt 2 priority to lowest level (Default)
 *  @arg\b      INT2_PRIORITY_MIDDLE_LOW : Set external interrupt 2 priority to middle low level
 *  @arg\b      INT2_PRIORITY_MIDDLE_HIGH : Set external interrupt 2 priority to middle high level
 *  @arg\b      INT2_PRIORITY_HIGHEST : Set external interrupt 2 priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT2_Priority_Select(INT2_PRIORITY_LOWEST);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT2_Priority_Select
#define INT2_PRIORITY_LOWEST          0x0000
#define INT2_PRIORITY_MIDDLE_LOW      0x0040
#define INT2_PRIORITY_MIDDLE_HIGH     0x4000
#define INT2_PRIORITY_HIGHEST         0x4040
/// @endcond
#define __DRV_INT2_Priority_Select(__SELECT__)\
    MWT(\
        IP0L = (IP0L & ~PX2L) | LOBYTE(__SELECT__);\
        IP0H = (IP0H & ~PX2H) | HIBYTE(__SELECT__);\
    )


/**
 *******************************************************************************
 * @brief       Set nINT2 filter mode type
 * @details
 * @param[in]   __SELECT__ : Set external interrupt 2 filter mode type
 *  @arg\b      INT2_FILTER_DISABLE : Disable external interrupt 2 filter mode type (Default)
 *  @arg\b      INT2_FILTER_SYSCLK_X3 : Set external interrupt 2 filter mode type to SYSCLKx3
 *  @arg\b      INT2_FILTER_SYSCLKDIV6X3 : Set external interrupt 2 filter mode type to SYSCLK/6x3
 *  @arg\b      INT2_FILTER_S0TOF : Set external interrupt 2 filter mode type to S0TOF
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT2_FilterMode_Select(INT2_FILTER_SYSCLKDIV6x3);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT2_FilterMode_Select
#define INT2_FILTER_DISABLE       0x0000
#define INT2_FILTER_SYSCLK_X3     0x0004
#define INT2_FILTER_SYSCLK_DIV6X3 0x0400
#define INT2_FILTER_S0TOF         0x0404
/// @endcond
#define __DRV_INT2_FilterMode_Select(__SELECT__)\
    MWT(\
        XICFG = (XICFG & ~X2FLT) | LOBYTE(__SELECT__);\
        __DRV_SFR_PageIndex(XICFG1_Page);\
        XICFG1 = (XICFG1 & ~X2FLT1) | HIBYTE(__SELECT__);\
        __DRV_SFR_PageIndex(0);\
    )




/**
 *******************************************************************************
 * @brief       Enable nINT3 interrupt
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT3_IT_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT3_IT_Enable()        EX3 = MW_ENABLE


/**
 *******************************************************************************
 * @brief       Disable nINT3 interrupt
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT3_IT_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT3_IT_Disable()        EX3 = MW_DISABLE


/**
 *******************************************************************************
 * @brief       Enable/Disable nINT3 interrupt
 * @details
 * @param[in]   __STATE__ : config EX3 control bit
 *  @arg\b      MW_DISABLE : Set external interrupt 3 disable (Default)
 *  @arg\b      MW_ENABLE : Set external interrupt 3 enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT3_IT_Cmd(MW_DISABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT3_IT_Cmd(__STATE__)        EX3 = __STATE__


/**
 *******************************************************************************
 * @brief       Set nINT3 interrupt flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT3_SetFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT3_SetFlag()        IE3 = MW_ENABLE


/**
 *******************************************************************************
 * @brief       Get nINT3 interrupt flag
 * @details 
 * @return      IE3 : return IE3 bit status
 * @note
 * @par         Example
 * @code
 *    __DRV_INT3_GetFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT3_GetFlag()        IE3


/**
 *******************************************************************************
 * @brief       Clear nINT3 interrupt flag
 * @details 
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT3_ClearFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT3_ClearFlag()        IE3 = MW_DISABLE


/**
 *******************************************************************************
 * @brief       Set nINT3 input pin source
 * @details
 * @param[in]   __SELECT__ : Set external interrupt 3 input pin source
 *  @arg\b      INT3_INPUT_PIN_SELECT_P42 : Set external interrupt 3 input pin source to P42 (Default)
 *  @arg\b      INT3_INPUT_PIN_SELECT_P21 : Set external interrupt 3 input pin source to P21
 *  @arg\b      INT3_INPUT_PIN_SELECT_P00 : Set external interrupt 3 input pin source to P00
 *  @arg\b      INT3_INPUT_PIN_SELECT_P16 : Set external interrupt 3 input pin source to P16
 *  @arg\b      INT3_INPUT_PIN_SELECT_P02 : Set external interrupt 3 input pin source to P02
 *  @arg\b      INT3_INPUT_PIN_SELECT_P61 : Set external interrupt 3 input pin source to P61
 *  @arg\b      INT3_INPUT_PIN_SELECT_P06 : Set external interrupt 3 input pin source to P06
 *  @arg\b      INT3_INPUT_PIN_SELECT_P63 : Set external interrupt 3 input pin source to P63
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT3_PinMux_Select(INT3_INPUT_PIN_SELECT_P16);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT3_PinMux_Select
#define INT3_INPUT_PIN_SELECT_P42     0x00
#define INT3_INPUT_PIN_SELECT_P21     0x02
#define INT3_INPUT_PIN_SELECT_P00     0x04
#define INT3_INPUT_PIN_SELECT_P16     0x06
#define INT3_INPUT_PIN_SELECT_P02     0x08
#define INT3_INPUT_PIN_SELECT_P61     0x0A
#define INT3_INPUT_PIN_SELECT_P06     0x0C
#define INT3_INPUT_PIN_SELECT_P63     0x0E

/// @endcond
#define __DRV_INT3_PinMux_Select(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(XICFG2_Page);\
        XICFG2 = (XICFG2 & ~(INT3IS2 | INT3IS1 | INT3IS0)) | __SELECT__;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Set nINT3 event trigger type
 * @details
 * @param[in]   __SELECT__ : Set external interrupt 3 event trigger type
 *  @arg\b      INT3_TRIGGER_TYPE_LOW_LEVEL : Set external interrupt 3 event trigger type to low level (Default)
 *  @arg\b      INT3_TRIGGER_TYPE_HIGH_LEVEL : Set external interrupt 3 event trigger type to high level
 *  @arg\b      INT3_TRIGGER_TYPE_FALLING_EDGE : Set external interrupt 3 event trigger type to falling edge
 *  @arg\b      INT3_TRIGGER_TYPE_RISING_EDGE : Set external interrupt 3 event trigger type to rising edge
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT3_TriggerType_Select(INT3_TRIGGER_TYPE_HIGH_LEVEL);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT3_TriggerType_Select
#define INT3_TRIGGER_TYPE_LOW_LEVEL       0x00
#define INT3_TRIGGER_TYPE_HIGH_LEVEL      0x80
#define INT3_TRIGGER_TYPE_FALLING_EDGE    0x10
#define INT3_TRIGGER_TYPE_RISING_EDGE     0x90
/// @endcond
#define __DRV_INT3_TriggerType_Select(__SELECT__)\
    MWT(\
        XICON = (XICON & ~INT3_TRIGGER_TYPE_RISING_EDGE) | __SELECT__;\
    )


/**
 *******************************************************************************
 * @brief       Set nINT3 interrupt priority level
 * @details
 * @param[in]   __SELECT__ : Set external interrupt 3 priority level
 *  @arg\b      INT3_PRIORITY_LOWEST : Set external interrupt 3 priority to lowest level (Default)
 *  @arg\b      INT3_PRIORITY_MIDDLE_LOW : Set external interrupt 3 priority to middle low level
 *  @arg\b      INT3_PRIORITY_MIDDLE_HIGH : Set external interrupt 3 priority to middle high level
 *  @arg\b      INT3_PRIORITY_HIGHEST : Set external interrupt 3 priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT3_Priority_Select(INT3_PRIORITY_LOWEST);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT3_Priority_Select
#define INT3_PRIORITY_LOWEST          0x0000
#define INT3_PRIORITY_MIDDLE_LOW      0x0080
#define INT3_PRIORITY_MIDDLE_HIGH     0x8000
#define INT3_PRIORITY_HIGHEST         0x8080
/// @endcond
#define __DRV_INT3_Priority_Select(__SELECT__)\
    MWT(\
        IP0L = (IP0L & ~PX3L) | LOBYTE(__SELECT__);\
        IP0H = (IP0H & ~PX3H) | HIBYTE(__SELECT__);\
    )


/**
 *******************************************************************************
 * @brief       Set nINT3 filter mode type
 * @details
 * @param[in]   __SELECT__ : Set external interrupt 3 filter mode type
 *  @arg\b      INT3_FILTER_DISABLE : Disable external interrupt 3 filter mode type (Default)
 *  @arg\b      INT3_FILTER_SYSCLK_X3 : Set external interrupt 3 filter mode type to SYSCLKx3
 *  @arg\b      INT3_FILTER_SYSCLKDIV6X3 : Set external interrupt 3 filter mode type to SYSCLK/6x3
 *  @arg\b      INT3_FILTER_S0TOF : Set external interrupt 3 filter mode type to S0TOF
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT3_FilterMode_Select(INT3_FILTER_SYSCLKDIV6x3);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT3_FilterMode_Select
#define INT3_FILTER_DISABLE       0x0000
#define INT3_FILTER_SYSCLK_X3     0x0008
#define INT3_FILTER_SYSCLK_DIV6X3 0x0800
#define INT3_FILTER_S0TOF         0x0808
/// @endcond
#define __DRV_INT3_FilterMode_Select(__SELECT__)\
    MWT(\
        XICFG = (XICFG & ~X3FLT) | LOBYTE(__SELECT__);\
        __DRV_SFR_PageIndex(XICFG1_Page);\
        XICFG1 = (XICFG1 & ~X3FLT1) | HIBYTE(__SELECT__);\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Enable system flag interrupt
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_SystemFlag_IT_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT_SystemFlag_IT_Enable()        EIE1 |= ESF



/**
 *******************************************************************************
 * @brief       Disable system flag interrupt
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_SystemFlag_IT_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT_SystemFlag_IT_Disable()        EIE1 &= ~ESF


/**
 *******************************************************************************
 * @brief       Enable/Disable system flag interrupt
 * @details
 * @param[in]   __STATE__ : config system flag interrupt control bit
 *  @arg\b      MW_DISABLE : Set system flag interrupt disable (Default)
 *  @arg\b      MW_ENABLE : Set system flag interrupt enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_SystemFlag_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_INT_SystemFlag_IT_Cmd(__STATE__)\
    MWT(\
        __STATE__==MW_ENABLE?(EIE1|=(ESF)):(EIE1&=~(ESF));\
    )        




 /**
 *******************************************************************************
 * @brief       Set Timer0 interrupt priority level
 * @details
 * @param[in]   __SELECT__ : Set timer0 priority level
 *  @arg\b      TIMER0_PRIORITY_LOWEST : Set timer0 priority to lowest level (Default)
 *  @arg\b      TIMER0_PRIORITY_MIDDLE_LOW : Set timer0 priority to middle low level
 *  @arg\b      TIMER0_PRIORITY_MIDDLE_HIGH : Set timer0 priority to middle high level
 *  @arg\b      TIMER0_PRIORITY_HIGHEST : Set timer0 priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_Timer0_Priority_Select(TIMER0_PRIORITY_LOWEST);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT_Timer0_Priority_Select
#define TIMER0_PRIORITY_LOWEST          0x0000
#define TIMER0_PRIORITY_MIDDLE_LOW      0x0002
#define TIMER0_PRIORITY_MIDDLE_HIGH     0x0200
#define TIMER0_PRIORITY_HIGHEST         0x0202
/// @endcond
#define __DRV_INT_Timer0_Priority_Select(__SELECT__)\
    MWT(\
        IP0L = (IP0L & ~PT0L) | LOBYTE(__SELECT__);\
        IP0H = (IP0H & ~PT0H) | HIBYTE(__SELECT__);\
    )
    

/**
 *******************************************************************************
 * @brief       Set Timer1 interrupt priority level
 * @details
 * @param[in]   __SELECT__ : Set timer1 priority level
 *  @arg\b      TIMER1_PRIORITY_LOWEST : Set timer1 priority to lowest level (Default)
 *  @arg\b      TIMER1_PRIORITY_MIDDLE_LOW : Set timer1 priority to middle low level
 *  @arg\b      TIMER1_PRIORITY_MIDDLE_HIGH : Set timer1 priority to middle high level
 *  @arg\b      TIMER1_PRIORITY_HIGHEST : Set timer1 priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_Timer1_Priority_Select(TIMER1_PRIORITY_LOWEST);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT_Timer1_Priority_Select
#define TIMER1_PRIORITY_LOWEST          0x0000
#define TIMER1_PRIORITY_MIDDLE_LOW      0x0008
#define TIMER1_PRIORITY_MIDDLE_HIGH     0x0800
#define TIMER1_PRIORITY_HIGHEST         0x0808
/// @endcond
#define __DRV_INT_Timer1_Priority_Select(__SELECT__)\
    MWT(\
        IP0L = (IP0L & ~PT1L) | LOBYTE(__SELECT__);\
        IP0H = (IP0H & ~PT1H) | HIBYTE(__SELECT__);\
    )

/**
 *******************************************************************************
 * @brief       Set Serial Port 0 interrupt priority level
 * @details
 * @param[in]   __SELECT__ : Set serial port 0 priority level
 *  @arg\b      S0_PRIORITY_LOWEST : Set S0 priority to lowest level (Default)
 *  @arg\b      S0_PRIORITY_MIDDLE_LOW : Set S0 priority to middle low level
 *  @arg\b      S0_PRIORITY_MIDDLE_HIGH : Set S0 priority to middle high level
 *  @arg\b      S0_PRIORITY_HIGHEST : Set S0 priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_S0_Priority_Select(S0_PRIORITY_LOWEST);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT_S0_Priority_Select
#define S0_PRIORITY_LOWEST          0x0000
#define S0_PRIORITY_MIDDLE_LOW      0x0010
#define S0_PRIORITY_MIDDLE_HIGH     0x1000
#define S0_PRIORITY_HIGHEST         0x1010
/// @endcond
#define __DRV_INT_S0_Priority_Select(__SELECT__)\
    MWT(\
        IP0L = (IP0L & ~PSL) | LOBYTE(__SELECT__);\
        IP0H = (IP0H & ~PSH) | HIBYTE(__SELECT__);\
    )



/**
 *******************************************************************************
 * @brief       Set Timer2 interrupt priority level
 * @details
 * @param[in]   __SELECT__ : Set timer2 priority level
 *  @arg\b      TIMER2_PRIORITY_LOWEST : Set timer2 priority to lowest level (Default)
 *  @arg\b      TIMER2_PRIORITY_MIDDLE_LOW : Set timer2 priority to middle low level
 *  @arg\b      TIMER2_PRIORITY_MIDDLE_HIGH : Set timer2 priority to middle high level
 *  @arg\b      TIMER2_PRIORITY_HIGHEST : Set timer2 priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_Timer2_Priority_Select(TIMER2_PRIORITY_LOWEST);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT_Timer2_Priority_Select
#define TIMER2_PRIORITY_LOWEST          0x0000
#define TIMER2_PRIORITY_MIDDLE_LOW      0x0020
#define TIMER2_PRIORITY_MIDDLE_HIGH     0x2000
#define TIMER2_PRIORITY_HIGHEST         0x2020
/// @endcond
#define __DRV_INT_Timer2_Priority_Select(__SELECT__)\
    MWT(\
        IP0L = (IP0L & ~PT2L) | LOBYTE(__SELECT__);\
        IP0H = (IP0H & ~PT2H) | HIBYTE(__SELECT__);\
    )
    

/**
 *******************************************************************************
 * @brief       Set SPI interrupt priority level
 * @details
 * @param[in]   __SELECT__ : Set SPI priority level
 *  @arg\b      SPI_PRIORITY_LOWEST : Set SPI priority to lowest level (Default)
 *  @arg\b      SPI_PRIORITY_MIDDLE_LOW : Set SPI priority to middle low level
 *  @arg\b      SPI_PRIORITY_MIDDLE_HIGH : Set SPI priority to middle high level
 *  @arg\b      SPI_PRIORITY_HIGHEST : Set SPI priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_SPI_Priority_Select(SPI_PRIORITY_LOWEST);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT_SPI_Priority_Select
#define SPI_PRIORITY_LOWEST          0x0000
#define SPI_PRIORITY_MIDDLE_LOW      0x0001
#define SPI_PRIORITY_MIDDLE_HIGH     0x0100
#define SPI_PRIORITY_HIGHEST         0x0101
/// @endcond
#define __DRV_INT_SPI_Priority_Select(__SELECT__)\
    MWT(\
        EIP1L = (EIP1L & ~PSPIL) | LOBYTE(__SELECT__);\
        EIP1H = (EIP1H & ~PSPIH) | HIBYTE(__SELECT__);\
    )
    

/**
 *******************************************************************************
 * @brief       Set ADC interrupt priority level
 * @details
 * @param[in]   __SELECT__ : Set ADC priority level
 *  @arg\b      ADC_PRIORITY_LOWEST : Set ADC priority to lowest level (Default)
 *  @arg\b      ADC_PRIORITY_MIDDLE_LOW : Set ADC priority to middle low level
 *  @arg\b      ADC_PRIORITY_MIDDLE_HIGH : Set ADC priority to middle high level
 *  @arg\b      ADC_PRIORITY_HIGHEST : Set ADC priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_ADC_Priority_Select(ADC_PRIORITY_LOWEST);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT_ADC_Priority_Select
#define ADC_PRIORITY_LOWEST          0x0000
#define ADC_PRIORITY_MIDDLE_LOW      0x0002
#define ADC_PRIORITY_MIDDLE_HIGH     0x0200
#define ADC_PRIORITY_HIGHEST         0x0202
/// @endcond
#define __DRV_INT_ADC_Priority_Select(__SELECT__)\
    MWT(\
        EIP1L = (EIP1L & ~PADCL) | LOBYTE(__SELECT__);\
        EIP1H = (EIP1H & ~PADCH) | HIBYTE(__SELECT__);\
    )
    

/**
 *******************************************************************************
 * @brief       Set PCA interrupt priority level
 * @details
 * @param[in]   __SELECT__ : Set PCA priority level
 *  @arg\b      PCA_PRIORITY_LOWEST : Set PCA priority to lowest level (Default)
 *  @arg\b      PCA_PRIORITY_MIDDLE_LOW : Set PCA priority to middle low level
 *  @arg\b      PCA_PRIORITY_MIDDLE_HIGH : Set PCA priority to middle high level
 *  @arg\b      PCA_PRIORITY_HIGHEST : Set PCA priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_PCA_Priority_Select(PCA_PRIORITY_LOWEST);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT_PCA_Priority_Select
#define PCA_PRIORITY_LOWEST          0x0000
#define PCA_PRIORITY_MIDDLE_LOW      0x0004
#define PCA_PRIORITY_MIDDLE_HIGH     0x0400
#define PCA_PRIORITY_HIGHEST         0x0404
/// @endcond
#define __DRV_INT_PCA_Priority_Select(__SELECT__)\
    MWT(\
        EIP1L = (EIP1L & ~PPCAL) | LOBYTE(__SELECT__);\
        EIP1H = (EIP1H & ~PPCAH) | HIBYTE(__SELECT__);\
    )
    

/**
 *******************************************************************************
 * @brief       Set system flag interrupt priority level
 * @details
 * @param[in]   __SELECT__ : Set system flag priority level
 *  @arg\b      SF_PRIORITY_LOWEST : Set system flag priority to lowest level (Default)
 *  @arg\b      SF_PRIORITY_MIDDLE_LOW : Set system flag priority to middle low level
 *  @arg\b      SF_PRIORITY_MIDDLE_HIGH : Set system flag priority to middle high level
 *  @arg\b      SF_PRIORITY_HIGHEST : Set system flag priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_SYSFLAG_Priority_Select(PCA_PRIORITY_LOWEST);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT_SYSFLAG_Priority_Select
#define SF_PRIORITY_LOWEST          0x0000
#define SF_PRIORITY_MIDDLE_LOW      0x0008
#define SF_PRIORITY_MIDDLE_HIGH     0x0800
#define SF_PRIORITY_HIGHEST         0x0808
/// @endcond
#define __DRV_INT_SYSFLAG_Priority_Select(__SELECT__)\
    MWT(\
        EIP1L = (EIP1L & ~PSFL) | LOBYTE(__SELECT__);\
        EIP1H = (EIP1H & ~PSFH) | HIBYTE(__SELECT__);\
    )
    

/**
 *******************************************************************************
 * @brief       Set KBI interrupt priority level
 * @details
 * @param[in]   __SELECT__ : Set KBI priority level
 *  @arg\b      KBI_PRIORITY_LOWEST : Set KBI priority to lowest level (Default)
 *  @arg\b      KBI_PRIORITY_MIDDLE_LOW : Set KBI priority to middle low level
 *  @arg\b      KBI_PRIORITY_MIDDLE_HIGH : Set KBI priority to middle high level
 *  @arg\b      KBI_PRIORITY_HIGHEST : Set KBI priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_KBI_Priority_Select(KBI_PRIORITY_LOWEST);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT_KBI_Priority_Select
#define KBI_PRIORITY_LOWEST          0x0000
#define KBI_PRIORITY_MIDDLE_LOW      0x0020
#define KBI_PRIORITY_MIDDLE_HIGH     0x2000
#define KBI_PRIORITY_HIGHEST         0x2020
/// @endcond
#define __DRV_INT_KBI_Priority_Select(__SELECT__)\
    MWT(\
        EIP1L = (EIP1L & ~PKBL) | LOBYTE(__SELECT__);\
        EIP1H = (EIP1H & ~PKBH) | HIBYTE(__SELECT__);\
    )
    

/**
 *******************************************************************************
 * @brief       Set I2C0 interrupt priority level
 * @details
 * @param[in]   __SELECT__ : Set I2C0 priority level
 *  @arg\b      I2C0_PRIORITY_LOWEST : Set I2C0 priority to lowest level (Default)
 *  @arg\b      I2C0_PRIORITY_MIDDLE_LOW : Set I2C0 priority to middle low level
 *  @arg\b      I2C0_PRIORITY_MIDDLE_HIGH : Set I2C0 priority to middle high level
 *  @arg\b      I2C0_PRIORITY_HIGHEST : Set I2C0 priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_I2C0_Priority_Select(I2C0_PRIORITY_LOWEST);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT_I2C0_Priority_Select
#define I2C0_PRIORITY_LOWEST          0x0000
#define I2C0_PRIORITY_MIDDLE_LOW      0x0040
#define I2C0_PRIORITY_MIDDLE_HIGH     0x4000
#define I2C0_PRIORITY_HIGHEST         0x4040
/// @endcond
#define __DRV_INT_I2C0_Priority_Select(__SELECT__)\
    MWT(\
        EIP1L = (EIP1L & ~PTWI0L) | LOBYTE(__SELECT__);\
        EIP1H = (EIP1H & ~PTWI0H) | HIBYTE(__SELECT__);\
    )


/**
 *******************************************************************************
 * @brief       Set AC0 interrupt priority level
 * @details
 * @param[in]   __SELECT__ : Set AC0 priority level
 *  @arg\b      AC0_PRIORITY_LOWEST : Set AC0 priority to lowest level (Default)
 *  @arg\b      AC0_PRIORITY_MIDDLE_LOW : Set AC0 priority to middle low level
 *  @arg\b      AC0_PRIORITY_MIDDLE_HIGH : Set AC0 priority to middle high level
 *  @arg\b      AC0_PRIORITY_HIGHEST : Set AC0 priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_AC0_Priority_Select(AC0_PRIORITY_LOWEST);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT_AC0_Priority_Select
#define AC0_PRIORITY_LOWEST          0x0000
#define AC0_PRIORITY_MIDDLE_LOW      0x0080
#define AC0_PRIORITY_MIDDLE_HIGH     0x8000
#define AC0_PRIORITY_HIGHEST         0x8080
/// @endcond
#define __DRV_INT_AC0_Priority_Select(__SELECT__)\
    MWT(\
        EIP1L = (EIP1L & ~PAC0L) | LOBYTE(__SELECT__);\
        EIP1H = (EIP1H & ~PAC0H) | HIBYTE(__SELECT__);\
    )

/**
 *******************************************************************************
 * @brief       Set Serial Port 1 interrupt priority level
 * @details
 * @param[in]   __SELECT__ : Set serial port 1 priority level
 *  @arg\b      S1_PRIORITY_LOWEST : Set S1 priority to lowest level (Default)
 *  @arg\b      S1_PRIORITY_MIDDLE_LOW : Set S1 priority to middle low level
 *  @arg\b      S1_PRIORITY_MIDDLE_HIGH : Set S1 priority to middle high level
 *  @arg\b      S1_PRIORITY_HIGHEST : Set S1 priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_S1_Priority_Select(S1_PRIORITY_LOWEST);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT_S1_Priority_Select
#define S1_PRIORITY_LOWEST          0x0000
#define S1_PRIORITY_MIDDLE_LOW      0x0010
#define S1_PRIORITY_MIDDLE_HIGH     0x1000
#define S1_PRIORITY_HIGHEST         0x1010
/// @endcond
#define __DRV_INT_S1_Priority_Select(__SELECT__)\
    MWT(\
        EIP1L = (EIP1L = ~PS1L) | LOBYTE(__SELECT__);\
        EIP1H = (EIP1H & ~PS1H) | HIBYTE(__SELECT__);\
    )



/**
 *******************************************************************************
 * @brief       Set Timer3 interrupt priority level
 * @details
 * @param[in]   __SELECT__ : Set timer3 priority level
 *  @arg\b      TIMER3_PRIORITY_LOWEST : Set timer3 priority to lowest level (Default)
 *  @arg\b      TIMER3_PRIORITY_MIDDLE_LOW : Set timer3 priority to middle low level
 *  @arg\b      TIMER3_PRIORITY_MIDDLE_HIGH : Set timer3 priority to middle high level
 *  @arg\b      TIMER3_PRIORITY_HIGHEST : Set timer3 priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_Timer3_Priority_Select(TIMER3_PRIORITY_LOWEST);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT_Timer3_Priority_Select
#define TIMER3_PRIORITY_LOWEST          0x0000
#define TIMER3_PRIORITY_MIDDLE_LOW      0x0001
#define TIMER3_PRIORITY_MIDDLE_HIGH     0x0100
#define TIMER3_PRIORITY_HIGHEST         0x0101
/// @endcond
#define __DRV_INT_Timer3_Priority_Select(__SELECT__)\
    MWT(\
        EIP2L = (EIP2L & ~PT3L) | LOBYTE(__SELECT__);\
        EIP2H = (EIP2H & ~PT3H) | HIBYTE(__SELECT__);\
    )
    

/**
 *******************************************************************************
 * @brief       Set AC1 interrupt priority level
 * @details
 * @param[in]   __SELECT__ : Set AC1 priority level
 *  @arg\b      AC1_PRIORITY_LOWEST : Set AC1 priority to lowest level (Default)
 *  @arg\b      AC1_PRIORITY_MIDDLE_LOW : Set AC1 priority to middle low level
 *  @arg\b      AC1_PRIORITY_MIDDLE_HIGH : Set AC1 priority to middle high level
 *  @arg\b      AC1_PRIORITY_HIGHEST : Set AC1 priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_AC1_Priority_Select(AC1_PRIORITY_LOWEST);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT_AC1_Priority_Select
#define AC1_PRIORITY_LOWEST          0x0000
#define AC1_PRIORITY_MIDDLE_LOW      0x0002
#define AC1_PRIORITY_MIDDLE_HIGH     0x0200
#define AC1_PRIORITY_HIGHEST         0x0202
/// @endcond
#define __DRV_INT_AC1_Priority_Select(__SELECT__)\
    MWT(\
        EIP2L = (EIP2L & ~PAC1L) | LOBYTE(__SELECT__);\
        EIP2H = (EIP2H & ~PAC1H) | HIBYTE(__SELECT__);\
    )

       
/**
 *******************************************************************************
 * @brief       Set DMA interrupt priority level
 * @details
 * @param[in]   __SELECT__ : Set DMA priority level
 *  @arg\b      DMA_PRIORITY_LOWEST : Set DMA priority to lowest level (Default)
 *  @arg\b      DMA_PRIORITY_MIDDLE_LOW : Set DMA priority to middle low level
 *  @arg\b      DMA_PRIORITY_MIDDLE_HIGH : Set DMA priority to middle high level
 *  @arg\b      DMA_PRIORITY_HIGHEST : Set DMA priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_DMA_Priority_Select(DMA_PRIORITY_LOWEST);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT_DMA_Priority_Select
#define DMA_PRIORITY_LOWEST          0x00
#define DMA_PRIORITY_MIDDLE_LOW      0x40
#define DMA_PRIORITY_MIDDLE_HIGH     0x80
#define DMA_PRIORITY_HIGHEST         0xC0
/// @endcond
#define __DRV_INT_DMA_Priority_Select(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(DMACG0_Page);\
        DMACG0 = (DMACG0 & ~(PDMAH | PDMAL)) | __SELECT__;\
        __DRV_SFR_PageIndex(0);\
    )       


/**
 *******************************************************************************
 * @brief       Set AC2 interrupt priority level
 * @details
 * @param[in]   __SELECT__ : Set AC2 priority level
 *  @arg\b      AC2_PRIORITY_LOWEST : Set AC2 priority to lowest level (Default)
 *  @arg\b      AC2_PRIORITY_MIDDLE_LOW : Set AC2 priority to middle low level
 *  @arg\b      AC2_PRIORITY_MIDDLE_HIGH : Set AC2 priority to middle high level
 *  @arg\b      AC2_PRIORITY_HIGHEST : Set AC2 priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_AC2_Priority_Select(AC2_PRIORITY_LOWEST);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT_AC2_Priority_Select
#define AC2_PRIORITY_LOWEST          0x0000
#define AC2_PRIORITY_MIDDLE_LOW      0x0080
#define AC2_PRIORITY_MIDDLE_HIGH     0x8000
#define AC2_PRIORITY_HIGHEST         0x8080
/// @endcond
#define __DRV_INT_AC2_Priority_Select(__SELECT__)\
    MWT(\
        EIP2L = (EIP2L & ~PAC2L) | LOBYTE(__SELECT__);\
        EIP2H = (EIP2H & ~PAC2H) | HIBYTE(__SELECT__);\
    )


/**
 *******************************************************************************
 * @brief       Set Serial Port 2 interrupt priority level
 * @details
 * @param[in]   __SELECT__ : Set serial port 2 priority level
 *  @arg\b      S2_PRIORITY_LOWEST : Set S2 priority to lowest level (Default)
 *  @arg\b      S2_PRIORITY_MIDDLE_LOW : Set S2 priority to middle low level
 *  @arg\b      S2_PRIORITY_MIDDLE_HIGH : Set S2 priority to middle high level
 *  @arg\b      S2_PRIORITY_HIGHEST : Set S2 priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_S2_Priority_Select(S2_PRIORITY_LOWEST);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT_S2_Priority_Select
#define S2_PRIORITY_LOWEST          0x0000
#define S2_PRIORITY_MIDDLE_LOW      0x0004
#define S2_PRIORITY_MIDDLE_HIGH     0x0400
#define S2_PRIORITY_HIGHEST         0x0404
/// @endcond
#define __DRV_INT_S2_Priority_Select(__SELECT__)\
    MWT(\
        EIP2L = (EIP2L & ~PS2L) | LOBYTE(__SELECT__);\
        EIP2H = (EIP2H & ~PS2H) | HIBYTE(__SELECT__);\
    )


/**
 *******************************************************************************
 * @brief       Set Timer4 interrupt priority level
 * @details
 * @param[in]   __SELECT__ : Set timer4 priority level
 *  @arg\b      TIMER4_PRIORITY_LOWEST : Set timer4 priority to lowest level (Default)
 *  @arg\b      TIMER4_PRIORITY_MIDDLE_LOW : Set timer4 priority to middle low level
 *  @arg\b      TIMER4_PRIORITY_MIDDLE_HIGH : Set timer4 priority to middle high level
 *  @arg\b      TIMER4_PRIORITY_HIGHEST : Set timer4 priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_Timer4_Priority_Select(TIMER4_PRIORITY_LOWEST);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT_Timer4_Priority_Select
#define TIMER4_PRIORITY_LOWEST          0x0000
#define TIMER4_PRIORITY_MIDDLE_LOW      0x0008
#define TIMER4_PRIORITY_MIDDLE_HIGH     0x0800
#define TIMER4_PRIORITY_HIGHEST         0x0808
/// @endcond
#define __DRV_INT_Timer4_Priority_Select(__SELECT__)\
    MWT(\
        EIP2L = (EIP2L & ~PT4L) | LOBYTE(__SELECT__);\
        EIP2H = (EIP2H & ~PT4H) | HIBYTE(__SELECT__);\
    )


/**
 *******************************************************************************
 * @brief       Set Serial Port 3 interrupt priority level
 * @details
 * @param[in]   __SELECT__ : Set serial port 3 priority level
 *  @arg\b      S3_PRIORITY_LOWEST : Set S3 priority to lowest level (Default)
 *  @arg\b      S3_PRIORITY_MIDDLE_LOW : Set S3 priority to middle low level
 *  @arg\b      S3_PRIORITY_MIDDLE_HIGH : Set S3 priority to middle high level
 *  @arg\b      S3_PRIORITY_HIGHEST : Set S3 priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_S3_Priority_Select(S3_PRIORITY_LOWEST);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT_S3_Priority_Select
#define S3_PRIORITY_LOWEST          0x0000
#define S3_PRIORITY_MIDDLE_LOW      0x0010
#define S3_PRIORITY_MIDDLE_HIGH     0x1000
#define S3_PRIORITY_HIGHEST         0x1010
/// @endcond
#define __DRV_INT_S3_Priority_Select(__SELECT__)\
    MWT(\
        EIP2L = (EIP2L & ~PS3L) | LOBYTE(__SELECT__);\
        EIP2H = (EIP2H & ~PS3H) | HIBYTE(__SELECT__);\
    )


/**
 *******************************************************************************
 * @brief       Set I2C1 interrupt priority level
 * @details
 * @param[in]   __SELECT__ : Set I2C1 priority level
 *  @arg\b      I2C1_PRIORITY_LOWEST : Set I2C1 priority to lowest level (Default)
 *  @arg\b      I2C1_PRIORITY_MIDDLE_LOW : Set I2C1 priority to middle low level
 *  @arg\b      I2C1_PRIORITY_MIDDLE_HIGH : Set I2C1 priority to middle high level
 *  @arg\b      I2C1_PRIORITY_HIGHEST : Set I2C1 priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_INT_I2C1_Priority_Select(I2C1_PRIORITY_LOWEST);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_INT_I2C1_Priority_Select
#define I2C1_PRIORITY_LOWEST          0x0000
#define I2C1_PRIORITY_MIDDLE_LOW      0x0040
#define I2C1_PRIORITY_MIDDLE_HIGH     0x4000
#define I2C1_PRIORITY_HIGHEST         0x4040
/// @endcond
#define __DRV_INT_I2C1_Priority_Select(__SELECT__)\
    MWT(\
        EIP2L = (EIP2L & ~PTWI1L) | LOBYTE(__SELECT__);\
        EIP2H = (EIP2H & ~PTWI1H) | HIBYTE(__SELECT__);\
    )


#endif  //_MG82F6D64_INT_DRV_H
