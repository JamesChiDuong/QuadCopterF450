/**
 ******************************************************************************
 *
 * @file        MG82F6D64_RTC_DRV.H
 *
 * @brief       This is the C code format driver head file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     V1.10
 * @date        2022/03/28
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

#ifndef __MG82F6D64_RTC_DRV_H
#define __MG82F6D64_RTC_DRV_H
/*--------------------------------------------------------------
RTC Function :
    RTC Initialization -
        1.RTC Clock Source Selection, RCSS : __DRV_RTC_ClockSource_Select(__SELECT__);
            __SELECT__ :
                RTC_CLK_P60     RTC_CLK_ILRCO   RTC_CLK_WDTPS
                RTC_CLK_WDTOF   RTC_CLK_SYSCLK  RTC_CLK_SYSCLK_12

        2.RTC Prescaler, RTCCS : __DRV_RTC_SetPrescaler(__PRESCALER__);
            __PRESCALER__ :
                RTC_RTCCS_DIV_32768     RTC_RTCCS_DIV_16384     RTC_RTCCS_DIV_8192      RTC_RTCCS_DIV_4096
                RTC_RTCCS_DIV_2048      RTC_RTCCS_DIV_1024      RTC_RTCCS_DIV_512       RTC_RTCCS_DIV_256
                RTC_RTCCS_DIV_128       RTC_RTCCS_DIV_64        RTC_RTCCS_DIV_32        RTC_RTCCS_DIV_16
                RTC_RTCCS_DIV_8         RTC_RTCCS_DIV_4         RTC_RTCCS_DIV_2         RTC_RTCCS_DIV_1

        3.RTC counter reload value, RTCRL : __DRV_RTC_SetReloaderValue(__VALUE__);
            __VALUE__ :
               (Decimal) 0~63

        4.RTC Output enable, RTCO : __DRV_RTC_SetRTCKO(__STATE__);
            __STATE__ :
                MW_ENABLE / MW_DISABLE

        5.RTC counter value, RTCCT : __DRV_RTC_SetCounterValue(__VALUE__);
            __VALUE__ :
                (Decimal) 0~63

        6.Get RTC counter value, RTCCT : __DRV_RTC_GetCounterValue(__VALUE__);
            __VALUE__ :
                (Decimal) 0~63

        7.Get RTC Reload counter value, RTCRL : __DRV_RTC_GetReloaderValue(__VALUE__);
            __VALUE__ :
                (Decimal) 0~63

    RTC Start/Stop -
        1.RTC Enable, RTCE : __DRV_RTC_Cmd(__STATE__);
            __STATE__ :
                MW_ENABLE / MW_DISABLE

    Interrupt -
        1.RTC Interrupt : __DRV_RTC_IT_Cmd(__STATE__);
            __STATE__ :
                MW_ENABLE / MW_DISABLE

        2.Clearing RTCF Flag : __DRV_RTC_ClearFlag(void);
--------------------------------------------------------------*/


/// @cond __DRV_Wizard_Without_Doxygen
//RTC CLK Source
#define RTC_CLK_P60         0
#define RTC_CLK_ILRCO       1
#define RTC_CLK_WDTPS       2
#define RTC_CLK_WDTOF       3
#define RTC_CLK_SYSCLK      4
#define RTC_CLK_SYSCLK_12   5

//RTC Prescaler RTCCS
#define RTC_RTCCS_DIV_32768 0
#define RTC_RTCCS_DIV_16384 1
#define RTC_RTCCS_DIV_8192  2
#define RTC_RTCCS_DIV_4096  3
#define RTC_RTCCS_DIV_2048  4
#define RTC_RTCCS_DIV_1024  5
#define RTC_RTCCS_DIV_512   6
#define RTC_RTCCS_DIV_256   7
#define RTC_RTCCS_DIV_128   8
#define RTC_RTCCS_DIV_64    9
#define RTC_RTCCS_DIV_32    10
#define RTC_RTCCS_DIV_16    11
#define RTC_RTCCS_DIV_8     12
#define RTC_RTCCS_DIV_4     13
#define RTC_RTCCS_DIV_2     14
#define RTC_RTCCS_DIV_1     15

//RTC Prescaler RPSC
#define RTC_RPSC_0  0
#define RTC_RPSC_1  1
#define RTC_RPSC_2  2
#define RTC_RPSC_3  3
#define RTC_RPSC_4  4
#define RTC_RPSC_5  5
#define RTC_RPSC_6  6
#define RTC_RPSC_7  7
/// @endcond
/**
 *******************************************************************************
 * @brief       RTC clock source selection.
 * @details
 * @param[in]   __SELECT__ : clock source parameters including
 *  @arg\b      RTC_CLK_P60          : RTC clock source selection, P60.
 *  @arg\b      RTC_CLK_ILRCO        : RTC clock source selection, ILRCO.
 *  @arg\b      RTC_CLK_WDTPS        : RTC clock source selection, WDTPS.
 *  @arg\b      RTC_CLK_WDTOF        : RTC clock source selection, WDTOF.
 *  @arg\b      RTC_CLK_SYSCLK       : RTC clock source selection, SYSCLK.
 *  @arg\b      RTC_CLK_SYSCLK_12    : RTC clock source selection, SYSCLK/12.
 * @return      None
 * @note
 * @par Example
 * @code
 *  __DRV_RTC_SetClkSource(RTC_CLK_ILRCO);
 * @endcode
 *******************************************************************************
 */
#define __DRV_RTC_ClockSource_Select(__SELECT__)\
    MWT(\
        uint8_t P_data;\
        P_data = DRV_PageP_Read(CKCON4_P);\
        P_data &= ~(RCSS0_P |RCSS1_P |RCSS2_P );\
        DRV_PageP_Write(CKCON4_P,P_data | (__SELECT__<<5));\
    )

/**
 *******************************************************************************
 * @brief       Setting up RTC prescaler
 * @details
 * @param[in]   __PRESCALER__ : prescaler value.
 *  @arg\b      RTC_RTCCS_DIV_32768 : divides RTC clock source by 32768.
 *  @arg\b      RTC_RTCCS_DIV_16384 : divides RTC clock source by 16384.
 *  @arg\b      RTC_RTCCS_DIV_8192 : divides RTC clock source by 8192.
 *  @arg\b      RTC_RTCCS_DIV_4096 : divides RTC clock source by 4096.
 *  @arg\b      RTC_RTCCS_DIV_2048 : divides RTC clock source by 2048.
 *  @arg\b      RTC_RTCCS_DIV_1024 : divides RTC clock source by 1024.
 *  @arg\b      RTC_RTCCS_DIV_512 : divides RTC clock source by 512.
 *  @arg\b      RTC_RTCCS_DIV_256 : divides RTC clock source by 256.
 *  @arg\b      RTC_RTCCS_DIV_128 : divides RTC clock source by 128.
 *  @arg\b      RTC_RTCCS_DIV_64 : divides RTC clock source by 64.
 *  @arg\b      RTC_RTCCS_DIV_32 : divides RTC clock source by 32.
 *  @arg\b      RTC_RTCCS_DIV_16 : divides RTC clock source by 16.
 *  @arg\b      RTC_RTCCS_DIV_8 : divides RTC clock source by 8.
 *  @arg\b      RTC_RTCCS_DIV_4 : divides RTC clock source by 4.
 *  @arg\b      RTC_RTCCS_DIV_2 : divides RTC clock source by 2.
 *  @arg\b      RTC_RTCCS_DIV_1 : divides RTC clock source by 1.
 * @return      None
 * @note
 * @par Example
 * @code
 *  __DRV_RTC_SetPrescaler(RTC_RTCCS_DIV_32768);
 * @endcode
 *******************************************************************************
 */
#define __DRV_RTC_SetPrescaler(__PRESCALER__)\
    MWT(\
        uint8_t P_data;\
        P_data = DRV_PageP_Read(CKCON4_P);\
        P_data &= ~(RTCCS3_P |RTCCS2_P);\
        DRV_PageP_Write(CKCON4_P,P_data| (__PRESCALER__ & 0xC)>>2);\
        P_data = DRV_PageP_Read(RTCTM_P);\
        P_data &= ~(RTCCS1_P |RTCCS0_P);\
        DRV_PageP_Write(RTCTM_P,P_data|((__PRESCALER__ & 0x3) << 6));\
    )

/**
 *******************************************************************************
 * @brief       Setting RTC counter value.
 * @details
 * @param[in]   __VALUE__ : counter value.
 * @return      None
 * @note
 * @par Example
 * @code
 *  __DRV_RTC_SetCounterValue(0);
 * @endcode
 *******************************************************************************
 */
#define __DRV_RTC_SetCounterValue(__VALUE__)\
    MWT(\
        uint8_t P_data;\
        P_data = DRV_PageP_Read(RTCTM_P);\
        P_data &= ~(RTCCT0_P|RTCCT1_P|RTCCT2_P|RTCCT3_P|RTCCT4_P|RTCCT5_P);\
        DRV_PageP_Write(RTCTM_P,P_data | (__VALUE__ & 0x3F));\
    )

/**
 *******************************************************************************
 * @brief       Getting RTC counter value.
 * @details
 * @param[in]   __VALUE__ : counter value.
 * @return      None
 * @note
 * @par Example
 * @code
 *  __DRV_RTC_GetCounterValue(Value);
 * @endcode
 *******************************************************************************
 */
#define __DRV_RTC_GetCounterValue(__VALUE__)\
    MWT(\
        __VALUE__ = (RTCTM & 0x3F);\
    )

/**
 *******************************************************************************
 * @brief       Setting RTC reloader value.
 * @details
 * @param[in]   __VALUE__ : reloader value.
 * @return      None
 * @note
 * @par Example
 * @code
 *  __DRV_RTC_SetReloaderValue(63);
 * @endcode
 *******************************************************************************
 */
#define __DRV_RTC_SetReloaderValue(__VALUE__)\
    MWT(\
        uint8_t P_data;\
        P_data = DRV_PageP_Read(RTCCR_P);\
        P_data &= ~(RTCRL0_P|RTCRL1_P|RTCRL2_P|RTCRL3_P|RTCRL4_P|RTCRL5_P);\
        DRV_PageP_Write(RTCCR_P,P_data | (__VALUE__ & 0x3F));\
    )

/**
 *******************************************************************************
 * @brief       Getting RTC reloader value.
 * @details
 * @param[in]   __VALUE__ : reloader value.
 * @return      None
 * @note
 * @par Example
 * @code
 *  __DRV_RTC_GetReloaderValue(Value);
 * @endcode
 *******************************************************************************
 */
#define __DRV_RTC_GetReloaderValue(__VALUE__)\
    MWT(\
        __VALUE__ = (RTCCR & 0x3F);\
    )

/**
 *******************************************************************************
 * @brief       Enabling RTC output.
 * @details
 * @param[in]   __STATE__ : enables/disables RTCKO output on P45.
 *  @arg\b      MW_ENABLE : enables the RTCKO output on P45.
 *  @arg\b      MW_DISABLE : disables the RTCKO output.
 * @return      None
 * @note
 * @par Example
 * @code
 *  __DRV_RTC_SetRTCKO(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_RTC_SetRTCKO(__STATE__)\
    MWT(\
        uint8_t P_data;\
        P_data = DRV_PageP_Read(RTCCR_P);\
        P_data &= ~(RTCO_P);\
        __STATE__ == MW_ENABLE ? DRV_PageP_Write(RTCCR_P,P_data | RTCO_P) : DRV_PageP_Write(RTCCR_P,P_data);\
    )

/**
 *******************************************************************************
 * @brief       Enabling RTC.
 * @details
 * @param[in]   __STATE__ : enables/disables RTC counter.
 *  @arg\b      MW_ENABLE : enables the RTC counter and sets RTCF when RTCCT overflows.
 *                      When RTCE is set, CPU cannot access RTCTM. RTCTM must be accessed when RTCE is cleared.
 *  @arg\b      MW_DISABLE : stops RTC counter, RTCCT.
 * @return      None
 * @note
 * @par Example
 * @code
 *  __DRV_RTC_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_RTC_Cmd(__STATE__)\
    MWT(\
        uint8_t P_data;\
        P_data = DRV_PageP_Read(RTCCR_P);\
        P_data &= ~(RTCE_P);\
        __STATE__ == MW_ENABLE ? DRV_PageP_Write(RTCCR_P,P_data | RTCE_P) : DRV_PageP_Write(RTCCR_P,P_data);\
    )

/**
 *******************************************************************************
 * @brief       Enabling RTCF interrupt.
 * @details
 * @param[in]   __STATE__ : enables/disables RTC interrupt.
 *  @arg\b      MW_ENABLE : enables RTCF interrupt. If it is enabled, RTCF will wake CPU up in idle or power-down mode.
 *  @arg\b      MW_DISABLE : disables RTCF interrupt.
 * @return      None
 * @note
 * @par Example
 * @code
 *  __DRV_RTC_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_RTC_IT_Cmd(__STATE__)\
    MWT(\
        __STATE__ == MW_ENABLE ? (SFIE |=  RTCFIE) : (SFIE &=  ~RTCFIE);\
    )

/**
 *******************************************************************************
 * @brief       Get RTCF flag.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_RTC_GetFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_RTC_GetFlag()         PCON1 & RTCF

/**
 *******************************************************************************
 * @brief       Clearing RTC flag.
 * @details
 * @return      None
 * @note
 * @par Example
 * @code
 *  __DRV_RTC_ClearFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_RTC_ClearFlag()\
    MWT(\
        PCON1 = RTCF;\
    )

#endif
