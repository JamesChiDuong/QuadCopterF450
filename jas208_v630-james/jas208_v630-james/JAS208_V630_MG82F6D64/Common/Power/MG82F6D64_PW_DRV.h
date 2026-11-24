/**
 ******************************************************************************
 *
 * @file        MG82F6D64_PW_DRV.h
 *
 * @brief       This is the C code format driver head file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     V1.10
 * @date        2022/06/10
 * @author      Megawin Software Center
 * @copyright   Copyright (c) 2019 MegaWin Technology Co., Ltd.
 *              All rights reserved.
 *
 ******************************************************************************
 * @par         Disclaimer
 *      The Demo software is provided "AS IS" without any warranty, either
 *      expressed or implied, including, but not limited to, the implied warranties
 *      of merchantability and fitness for a particular purpose.  The author will
 *      not be liable for any special, incidental, consequential or indirect
 *      damages due to loss of data or any other reason.
 *      These statements agree with the worldwide and local dictated laws about
 *      authorship and violence against these laws.
 ******************************************************************************
 ******************************************************************************
 */

#ifndef __MG82F6D64_PW_DRV_H
#define __MG82F6D64_PW_DRV_H

/*--------------------------------------------------------------
Power Function :
    Power Down
    Idle Mode

BODx Function :
    BODx Initialization -
    |----BOD0 Initialization
            1.BOD0 Reset : __DRV_BODx_SetBOD0RST_Cmd(__STATE__);
                __STATE__ :
                    MW_ENABLE / MW_DISABLE

    |----BOD1 Initialization
            1.BOD1 Enable : __DRV_BODx_BOD1_MonitorsVDD(__STATE__);
                __STATE__ :
                    MW_ENABLE / MW_DISABLE

            2.Brown-Out Detector 1 Level : __DRV_BODx_BOD1_MonitoredLevelSelect(__SELECT__);
                __LEVEL__ :
                    BOD1_VoltageLevel_2V0   BOD1_VoltageLevel_2V4   BOD1_VoltageLevel_3V7   BOD1_VoltageLevel_4V2

            3.Awake BOD1 in PD mode  : __DRV_BODx_BOD1_AwakeBOD1inPD(__STATE__);
                __STATE__ :
                    MW_ENABLE / MW_DISABLE

            4.BOD1 Reset : __DRV_BODx_SetBOD1RST_Cmd(__STATE__);
                __STATE__ :
                    MW_ENABLE / MW_DISABLE

    Brown-Out Detection Interrupt -
        1. BOD0 Interrupt : __DRV_BODx_BOD0_IT_Cmd(__STATE__);
            __STATE__ :
                MW_ENABLE / MW_DISABLE

        2. BOD1 Interrupt : __DRV_BODx_BOD1_IT_Cmd(__STATE__);
            __STATE__ :
                MW_ENABLE / MW_DISABLE

        3. Get BOF0 Flag : __DRV_BODx_BOD0_GetFlag(void);

        4. Clear BOF0 Flag : __DRV_BODx_BOD0_ClearFlag(void);

        5. Get BOF1 Flag : __DRV_BODx_BOD1_GetFlag(void);

        6. Clear BOF1 Flag : __DRV_BODx_BOD1_ClearFlag(void);

--------------------------------------------------------------*/

/**
 *******************************************************************************
 * @brief       MCU Power Down.
 * @details
 * @return      None
 * @note
 * @par Example
 * @code
 *  __DRV_PW_PowerDown_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PW_PowerDown_Enable() \
    MWT(                            \
        PCON0 |= PD;)

/**
 *******************************************************************************
 * @brief       MCU Idle Mode.
 * @details
 * @return      None
 * @note
 * @par Example
 * @code
 *  __DRV_PW_PowerDown_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PW_IdleMode_Enable() \
    MWT(                           \
        PCON0 |= IDL;)

/**
 *******************************************************************************
 * @brief       Enabling BOD0 reset.
 * @details
 * @param[in]   __STATE__ : enables/disables BOD0 reset.
 *  @arg\b      MW_DISABLE : disables BOD0 to trigger a system reset when BOF0 is set. (Default)
 *  @arg\b      MW_ENABLE : enables BOD0 to trigger a system reset when BOF0 is set(VDD meets 1.7V).
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_BODx_SetBOD0_RST(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_BODx_SetBOD0RST_Cmd(__STATE__)                                 \
    MWT(                                                                     \
        uint8_t P_data;                                                      \
        P_data = DRV_PageP_Read(PCON2_P);                                    \
        __STATE__ == MW_ENABLE ? (P_data |= BO0RE_P) : (P_data &= ~BO0RE_P); \
        DRV_PageP_Write(PCON2_P, P_data);)

/**
 *******************************************************************************
 * @brief       Enabling BOD1 reset.
 * @details
 * @param[in]   __STATE__ :  enables/disables BOD1 reset.
 *  @arg\b      MW_DISABLE : disables BOD1 to trigger a system reset when BOF1 is set. (Default)
 *  @arg\b      MW_ENABLE : enables BOD1 to trigger a system reset when BOF1 is set.
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_BODx_SetBOD1_RST(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_BODx_SetBOD1RST_Cmd(__STATE__)                                 \
    MWT(                                                                     \
        uint8_t P_data;                                                      \
        P_data = DRV_PageP_Read(PCON2_P);                                    \
        __STATE__ == MW_ENABLE ? (P_data |= BO1RE_P) : (P_data &= ~BO1RE_P); \
        DRV_PageP_Write(PCON2_P, P_data);)

/**
 *******************************************************************************
 * @brief       Monitoring the level selection of brown-out detector 1.
 * @details
 * @param[in]   __SELECT__ : BOD1 detecting level.
 *  @arg\b      BOD1_VoltageLevel_2V0 : BOD1 detecting level equals 2.0V. (Default)
 *  @arg\b      BOD1_VoltageLevel_2V4 : BOD1 detecting level equals 2.4V.
 *  @arg\b      BOD1_VoltageLevel_3V7 : BOD1 detecting level equals 3.7V.
 *  @arg\b      BOD1_VoltageLevel_4V2 : BOD1 detecting level equals 4.2V.
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_BODx_BOD1_MonitoredLevelSelect(BOD1_VoltageLevel_2V0);
 * @endcode
 *******************************************************************************
 */
/// @cond
#define BOD1_VoltageLevel_2V0 0
#define BOD1_VoltageLevel_2V4 1
#define BOD1_VoltageLevel_3V7 2
#define BOD1_VoltageLevel_4V2 3
/// @endcond
#define __DRV_BODx_BOD1_MonitoredLevelSelect(__SELECT__) \
    MWT(                                                 \
        uint8_t P_data;                                  \
        P_data = DRV_PageP_Read(PCON2_P);                \
        P_data &= ~(BO1S1_P | BO1S0_P);                  \
        DRV_PageP_Write(PCON2_P, P_data | (__SELECT__ << 4));)

/**
 *******************************************************************************
 * @brief       Awaking BOD1 in PD mode.
 * @details
 * @param[in]   __STATE__ : enables/disables BOD1 in PD mode.
 *  @arg\b      MW_DISABLE : BOD1 is disabled in power-down mode. (Default)
 *  @arg\b      MW_ENABLE : BOD1 keeps operations in power-down mode.
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_BODx_BOD1_AwakeBOD1inPD(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */

#define __DRV_BODx_BOD1_AwakeBOD1inPD(__STATE__)                               \
    MWT(                                                                       \
        uint8_t P_data;                                                        \
        P_data = DRV_PageP_Read(PCON2_P);                                      \
        __STATE__ == MW_ENABLE ? (P_data |= AWBOD1_P) : (P_data &= ~AWBOD1_P); \
        DRV_PageP_Write(PCON2_P, P_data);)

/**
 *******************************************************************************
 * @brief       Enabling BOD1 that monitors VDD power drop at a specified voltage level.
 * @details
 * @param[in]   __STATE__ : enables/disables BOD1 to monitor VDD power-drop.
 *  @arg\b      MW_DISABLE : disables BOD1 to slow down the chip power consumption.
 *  @arg\b      MW_ENABLE : enables BOD1 to monitor VDD power drop. (Default)
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_BODx_BOD1_MonitorsVDD(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_BODx_BOD1_MonitorsVDD(__STATE__)                                \
    MWT(                                                                      \
        uint8_t P_data;                                                       \
        P_data = DRV_PageP_Read(PCON2_P);                                     \
        __STATE__ == MW_DISABLE ? (P_data &= ~EBOD1_P) : (P_data |= EBOD1_P); \
        DRV_PageP_Write(PCON2_P, P_data);)

/**
 *******************************************************************************
 * @brief       Enabling BOD0 interrupt.
 * @details
 * @param[in]   __STATE__ :  enables/disables BOD0 interrupt.
 *  @arg\b      MW_DISABLE : disables BOD0 interrupt. (Default)
 *  @arg\b      MW_ENABLE : enables BOD0 interrupt.
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_BODx_BOD0_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_BODx_BOD0_IT_Cmd(__STATE__) \
    MWT(                                  \
        __STATE__ == MW_ENABLE ? (SFIE |= BOF0IE) : (SFIE &= ~BOF0IE);)

/**
 *******************************************************************************
 * @brief       Enabling BOD1 interrupt.
 * @details
 * @param[in]   __STATE__ :  enables/disables BOD1 interrupt.
 *  @arg\b      MW_DISABLE : disables BOD1 interrupt. (Defualt)
 *  @arg\b      MW_ENABLE : enables BOD1 interrupt.
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_BODx_BOD1_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_BODx_BOD1_IT_Cmd(__STATE__) \
    MWT(                                  \
        __STATE__ == MW_ENABLE ? (SFIE |= BOF1IE) : (SFIE &= ~BOF1IE);)

/**
 *******************************************************************************
 * @brief       Get BOD0 flag.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_BODx_BOD0_GetFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_BODx_BOD0_GetFlag() PCON1 &BOF0

/**
 *******************************************************************************
 * @brief       Clear BOD0 flag.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_BODx_BOD0_ClearFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_BODx_BOD0_ClearFlag() \
    MWT(                            \
        PCON1 = BOF0;)

/**
 *******************************************************************************
 * @brief       Get BOD1 flag.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_BODx_BOD1_GetFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_BODx_BOD1_GetFlag() PCON1 &BOF1

/**
 *******************************************************************************
 * @brief       Clearing BOD1 flag.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_BODx_BOD1_ClearFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_BODx_BOD1_ClearFlag() \
    MWT(                            \
        PCON1 = BOF1;)

#endif
