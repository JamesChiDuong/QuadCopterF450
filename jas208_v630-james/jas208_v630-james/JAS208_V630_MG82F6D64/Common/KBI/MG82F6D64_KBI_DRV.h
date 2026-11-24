/**
 ******************************************************************************
 *
 * @file        MG82F6D64_KBI_DRV.H
 *
 * @brief       This is the C code format driver head file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     V1.02
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

 
#ifndef _MG82F6D64_KBI_DRV_H
#define _MG82F6D64_KBI_DRV_H


/**
 *******************************************************************************
 * @brief       Enable KBI interrupt
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_KBI_IT_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_KBI_IT_Enable()        EIE1 |= EKB


/**
 *******************************************************************************
 * @brief       Disable KBI interrupt
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_KBI_IT_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_KBI_IT_Disable()        EIE1 &= ~EKB


/**
 *******************************************************************************
 * @brief       Enable/Disable KBI interrupt
 * @details
 * @param[in]   __STATE__ : config inversion control bit
 *  @arg\b      MW_DISABLE : Set KBI interrupt disable (default)
 *  @arg\b      MW_ENABLE : Set KBI interrupt enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_KBI_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_KBI_IT_Cmd(__STATE__)\
    MWT(\
        (__STATE__==MW_ENABLE?(EIE1 |= EKB):(EIE1 &= ~EKB));\
    )




/**
 *******************************************************************************
 * @brief       Set KBI interrupt flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_KBI_SetFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_KBI_SetFlag()        KBCON |= KBIF




/**
 *******************************************************************************
 * @brief       Get KBI interrupt flag
 * @details
 * @return      return KBIF status
 * @note
 * @par         Example
 * @code
 *    __DRV_KBI_GetFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_KBI_GetFlag()        KBCON & KBIF



/**
 *******************************************************************************
 * @brief       Clear KBI interrupt flag
 * @details 
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_KBI_ClearFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_KBI_ClearFlag()        KBCON &= ~KBIF




/**
 *******************************************************************************
 * @brief       Set KBI low nibble input pin source
 * @details
 * @param[in]   __SELECT__ : Set KBI low nibble input pin source
 *  @arg\b      KBI0TO3_PORT1: Set KBI low nibble input pin source to Port1 (default)
 *  @arg\b      KBI0TO3_PORT0: Set KBI low nibble input pin source to Port0
 *  @arg\b      KBI0TO3_PORT2: Set KBI low nibble input pin source to Port2
 *  @arg\b      KBI0TO3_PORT4: Set KBI low nibble input pin source to Port4
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_KBI_PinMux_LowNibble_Select(KBI0TO3_PORT0);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_KBI_PinMux_LowNibble_Select
#define KBI0TO3_PORT1     0x00
#define KBI0TO3_PORT0     0x10
#define KBI0TO3_PORT2     0x20
#define KBI0TO3_PORT4     0x30
/// @endcond
#define __DRV_KBI_PinMux_LowNibble_Select(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR6_Page);\
        AUXR6 = (AUXR6 & ~(KBILPS1 | KBILPS0)) | __SELECT__;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Set KBI high nibble input pin source
 * @details
 * @param[in]   __SELECT__ : Set KBI high nibble input pin source
 *  @arg\b      KBI4TO7_PORT1: Set KBI high nibble input pin source to Port1 (default)
 *  @arg\b      KBI4TO7_PORT0: Set KBI high nibble input pin source to Port0
 *  @arg\b      KBI4TO7_PORT2: Set KBI high nibble input pin source to Port2
 *  @arg\b      KBI4TO7_PORT6: Set KBI high nibble input pin source to Port6
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_KBI_PinMux_HighNibble_Select(KBI4TO7_PORT2);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_KBI_PinMux_HighNibble_Select
#define KBI4TO7_PORT1     0x00
#define KBI4TO7_PORT0     0x40
#define KBI4TO7_PORT2     0x80
#define KBI4TO7_PORT6     0xC0
/// @endcond
#define __DRV_KBI_PinMux_HighNibble_Select(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR6_Page);\
        AUXR6 = (AUXR6 & ~(KBIHPS1 | KBIHPS0)) | __SELECT__;\
        __DRV_SFR_PageIndex(0);\
    )




/**
 *******************************************************************************
 * @brief       Set KBI event trigger type
 * @details
 * @param[in]   __SELECT__ : Set KBI event trigger type
 *  @arg\b      KBI_TRIGGER_TYPE_LEVEL: Set KBI event trigger type to level trigger (default)
 *  @arg\b      KBI_TRIGGER_TYPE_EDGE: Set KBI event trigger type to edge trigger
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_KBI_TriggerType_Select(KBI_TRIGGER_TYPE_EDGE);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_KBI_TriggerType_Select
#define KBI_TRIGGER_TYPE_LEVEL    0x00
#define KBI_TRIGGER_TYPE_EDGE     0x20
/// @endcond
#define __DRV_KBI_TriggerType_Select(__SELECT__)\
    MWT(\
        KBCON = (KBCON & ~KBES) | __SELECT__;\
    )


/**
 *******************************************************************************
 * @brief       Set KBI pattern matching polarity type
 * @details
 * @param[in]   __SELECT__ : Set KBI pattern matching polarity type
 *  @arg\b      KBI_MATCHING_TYPE_NOT_EQUAL_PATTERN: Set to not equal pattern trigger KBI interrupt (default)
 *  @arg\b      KBI_MATCHING_TYPE_EQUAL_PATTERN: Set to equal pattern trigger KBI interrupt
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_KBI_PatternMatchingType_Select(KBI_MATCHING_TYPE_EQUAL_PATTERN);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_KBI_PatternMatchingType_Select
#define KBI_MATCHING_TYPE_NOT_EQUAL_PATTERN    0x00
#define KBI_MATCHING_TYPE_EQUAL_PATTERN        0x02
/// @endcond
#define __DRV_KBI_PatternMatchingType_Select(__SELECT__)\
    MWT(\
        KBCON = (KBCON & ~PATN_SEL) | __SELECT__;\
    )

/**
 *******************************************************************************
 * @brief       Set KBI interrupt priority
 * @details
 * @param[in]   __SELECT__ : Set KBI interrupt priority level
 *  @arg\b      KBI_PRIORITY_LOWEST: Set KBI interrupt priority to lowest level (default)
 *  @arg\b      KBI_PRIORITY_MIDDLE_LOW: Set KBI interrupt priority to middle low level
 *  @arg\b      KBI_PRIORITY_MIDDLE_HIGH: Set KBI interrupt priority to middle high level
 *  @arg\b      KBI_PRIORITY_HIGHEST: Set KBI interrupt priority to highest level
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_KBI_Priority_Select(KBI_PRIORITY_LOWEST);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_KBI_Priority_Select
#define KBI_PRIORITY_LOWEST           0x0000
#define KBI_PRIORITY_MIDDLE_LOW       0x0020
#define KBI_PRIORITY_MIDDLE_HIGH      0x2000
#define KBI_PRIORITY_HIGHEST          0x2020
/// @endcond
#define __DRV_KBI_Priority_Select(__SELECT__)\
    MWT(\
        EIP1L = (EIP1L & ~PKBL) | LOBYTE(__SELECT__);\
        EIP1H = (EIP1H & ~PKBH) | HIBYTE(__SELECT__);\
    )

/**
 *******************************************************************************
 * @brief       Set KBI filter mode type
 * @details
 * @param[in]   __SELECT__ : Set KBI filter mode type
 *  @arg\b      KBI_FILTER_DISABLE: Disable KBI filter mode type (default)
 *  @arg\b      KBI_FILTER_SYSCLK_X3: Set KBI filter mode type to SYSCLKx3
 *  @arg\b      KBI_FILTER_SYSCLKDIV6X3: Set KBI filter mode type to SYSCLK/6x3
 *  @arg\b      KBI_FILTER_S0TOF: Set KBI filter mode type to S0TOF
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_KBI_FilterMode_Select(KBI_FILTER_DISABLE);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_KBI_FilterMode_Select
#define KBI_FILTER_DISABLE          0x00
#define KBI_FILTER_SYSCLK_X3        0x40
#define KBI_FILTER_SYSCLK_DIV6X3    0x80
#define KBI_FILTER_S0TOF            0xC0
/// @endcond
#define __DRV_KBI_FilterMode_Select(__SELECT__)\
    MWT(\
        KBCON = (KBCON & ~(KBCS1 | KBCS0)) | __SELECT__;\
    )




/**
 *******************************************************************************
 * @brief       Set KBI pattern
 * @details
 * @param[in]   __PATTERN__ : Set KBI pattern to KBPATN (0x00 ~ 0xFF)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_KBI_SetPattern(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_KBI_SetPattern(__PATTERN__)        KBPATN = __PATTERN__






/**
 *******************************************************************************
 * @brief       Get KBI pattern
 * @details
 * @param[in]   __PATTERN__ : The place to store KBPATN data
 * @return      None
 * @note
 * @par         Example
 * @code
 *    unsigned char tmp = 0;
 *    __DRV_KBI_GetPattern(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_KBI_GetPattern(__PATTERN__)        __PATTERN__ = KBPATN




/**
 *******************************************************************************
 * @brief       Set KBI port pin mask
 * @details
 * @param[in]   __MASK__ : Set KBI interrupt mask bit to KBMASK (0x00 ~ 0xFF)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_KBI_SetPortPinMask(0xF0);
 * @endcode
 *******************************************************************************
 */
#define __DRV_KBI_SetPortPinMask(__MASK__)        KBMASK = __MASK__



/**
 *******************************************************************************
 * @brief       Get KBI port pin mask
 * @details
 * @param[in]   __MASK__ : The place to store KBMASK data
 * @return      None
 * @note
 * @par         Example
 * @code
 *    unsigned char tmp = 0;
 *    __DRV_KBI_GetPortPinMask(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_KBI_GetPortPinMask(__MASK__)        __MASK__ = KBMASK




bool DRV_KBI_GetFlag(void);
#endif  //_MG82F6D64_KBI_DRV_H
