#ifndef _POWER_DRIVER_H
#define _POWER_DRIVER_H
#include "ADC_Driver.h"
#include "API_Macro_MG82F6D64.H"
#include "Config.h"
#include "IAP_Driver.h"

#define POWER_Setting()

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

#ifndef MWT
#define MWT(code) \
    do            \
    {             \
        code      \
    } while (0)
#endif

#define MW_ENABLE 1
#define MW_DISABLE 0

#define __Power_BODx_BOD1_AwakeBOD1inPD(__STATE__)                             \
    MWT(                                                                       \
        uint8_t P_data;                                                        \
        P_data = PageP_Read(PCON2_P);                                          \
        __STATE__ == MW_ENABLE ? (P_data |= AWBOD1_P) : (P_data &= ~AWBOD1_P); \
        PageP_Write(PCON2_P, P_data);)

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
 *******************************************************************************/
/// @cond
#define BOD1_VoltageLevel_2V0 0
#define BOD1_VoltageLevel_2V4 1
#define BOD1_VoltageLevel_3V7 2
#define BOD1_VoltageLevel_4V2 3
/// @endcond
#define __Power_BODx_BOD1_MonitoredLevelSelect(__SELECT__) \
    MWT(                                                   \
        uint8_t P_data;                                    \
        P_data = PageP_Read(PCON2_P);                      \
        P_data &= ~(BO1S1_P | BO1S0_P);                    \
        PageP_Write(PCON2_P, P_data | (__SELECT__ << 4));)

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
#define __Power_BODx_SetBOD0RST_Cmd(__STATE__)                               \
    MWT(                                                                     \
        uint8_t P_data;                                                      \
        P_data = PageP_Read(PCON2_P);                                        \
        __STATE__ == MW_ENABLE ? (P_data |= BO0RE_P) : (P_data &= ~BO0RE_P); \
        PageP_Write(PCON2_P, P_data);)

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
#define __Power_BODx_SetBOD1RST_Cmd(__STATE__)                               \
    MWT(                                                                     \
        uint8_t P_data;                                                      \
        P_data = PageP_Read(PCON2_P);                                        \
        __STATE__ == MW_ENABLE ? (P_data |= BO1RE_P) : (P_data &= ~BO1RE_P); \
        PageP_Write(PCON2_P, P_data);)

#define Power_SettingDefault()                                         \
    MWT(                                                               \
        __Power_BODx_BOD1_AwakeBOD1inPD(MW_DISABLE);                   \
        __Power_BODx_BOD1_MonitoredLevelSelect(BOD1_VoltageLevel_2V0); \
        __Power_BODx_SetBOD0RST_Cmd(MW_DISABLE);                       \
        __Power_BODx_SetBOD1RST_Cmd(MW_DISABLE);)
#endif