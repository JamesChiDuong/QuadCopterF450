/**
 ******************************************************************************
 *
 * @file        MG82F6D64_EMB_DRV.H
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

 
#ifndef _MG82F6D64_EMB_DRV_H
#define _MG82F6D64_EMB_DRV_H

/**
 *******************************************************************************
 * @brief       Select EMB Read/Write command pulse width
 * @details
 * @param[in]   __SELECT__ : Programmable EMB Read/Write command pulse width selection.
 *  @arg\b      EMB_CMD_PULSE_1_SYSCLK : nRD and nWR pulse width = 1 SYSCLK/CPUCLK cycle (Default)
 *  @arg\b      EMB_CMD_PULSE_2_SYSCLK : nRD and nWR pulse width = 2 SYSCLK/CPUCLK cycle
 *  @arg\b      EMB_CMD_PULSE_3_SYSCLK : nRD and nWR pulse width = 3 SYSCLK/CPUCLK cycle
 *  @arg\b      EMB_CMD_PULSE_4_SYSCLK : nRD and nWR pulse width = 4 SYSCLK/CPUCLK cycle
 *  @arg\b      EMB_CMD_PULSE_5_SYSCLK : nRD and nWR pulse width = 5 SYSCLK/CPUCLK cycle
 *  @arg\b      EMB_CMD_PULSE_6_SYSCLK : nRD and nWR pulse width = 6 SYSCLK/CPUCLK cycle
 *  @arg\b      EMB_CMD_PULSE_7_SYSCLK : nRD and nWR pulse width = 7 SYSCLK/CPUCLK cycle
 *  @arg\b      EMB_CMD_PULSE_8_SYSCLK : nRD and nWR pulse width = 8 SYSCLK/CPUCLK cycle
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_EMB_CommandPulse_Width_Select(EMB_CMD_PULSE_2_SYSCLK);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_EMB_CommandPulse_Width_Select
#define EMB_CMD_PULSE_1_SYSCLK          0x00
#define EMB_CMD_PULSE_2_SYSCLK          RWS0
#define EMB_CMD_PULSE_3_SYSCLK          RWS1 
#define EMB_CMD_PULSE_4_SYSCLK         (RWS1 | RWS0)
#define EMB_CMD_PULSE_5_SYSCLK          RWS2
#define EMB_CMD_PULSE_6_SYSCLK         (RWS2 | RWS0)
#define EMB_CMD_PULSE_7_SYSCLK         (RWS2 | RWS1)
#define EMB_CMD_PULSE_8_SYSCLK         (RWS2 | RWS1 | RWS0)
/// @endcond
#define __DRV_EMB_CommandPulse_Width_Select(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(EMBTC_Page);\
        EMBTC = (EMBTC & ~EMB_CMD_PULSE_8_SYSCLK) | __SELECT__;\
        __DRV_SFR_PageIndex(0);\
    )



/**
 *******************************************************************************
 * @brief       Select EMB Data Bus Setup/Hold Time Control for Write(nWR) pulse
 * @details
 * @param[in]   __SELECT__ : Programmable EMB nWR Setup/Hold Time Control selection.
 *  @arg\b      EMB_DATA_SETUPHOLD_PULSE_1_SYSCLK : Data bus setup/hold time width = 1 SYSCLK/CPUCLK cycle (Default)
 *  @arg\b      EMB_DATA_SETUPHOLD_PULSE_2_SYSCLK : Data bus setup/hold time width = 2 SYSCLK/CPUCLK cycle
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_EMB_DataSetupHold_Time_Select(EMB_DATA_SETUPHOLD_PULSE_1_SYSCLK);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_EMB_DataSetupHold_Time_Select
#define EMB_DATA_SETUPHOLD_PULSE_1_SYSCLK          0x00
#define EMB_DATA_SETUPHOLD_PULSE_2_SYSCLK          RWSH
/// @endcond
#define __DRV_EMB_DataSetupHold_Time_Select(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(EMBTC_Page);\
        EMBTC = (EMBTC & ~EMB_CMD_PULSE_2_SYSCLK) | __SELECT__;\
        __DRV_SFR_PageIndex(0);\
    )




/**
 *******************************************************************************
 * @brief       EMB nRD/nWR Port Selection
 * @details
 * @param[in]   __SELECT__ : Set EMB nRD/nWR Port
 *  @arg\b      EMB_nRDnWR_PORT_SELECT_0 : Set EMB nRD/nWR to P37/P36 (Default)
 *  @arg\b      EMB_nRDnWR_PORT_SELECT_1 : Set EMB nRD/nWR to P36/P37
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_EMB_nRDnWR_PinMux_Select(EMB_nRDnWR_PORT_SELECT_0);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_EMB_nRDnWR_PinMux_Select
#define EMB_nRDnWR_PORT_SELECT_0       0x00
#define EMB_nRDnWR_PORT_SELECT_1       RWPS0
/// @endcond
#define __DRV_EMB_nRDnWR_PinMux_Select(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR14_Page);\
        AUXR14 = (AUXR14 & ~EMB_nRDnWR_PORT_SELECT_1) | __SELECT__;\
        __DRV_SFR_PageIndex(0);\
    )



/**
 *******************************************************************************
 * @brief       EMB Data Bus Port Selection
 * @details
 * @param[in]   __SELECT__ : Set EMB Data Bus Port
 *  @arg\b      EMB_DATA_PORT_SELECT_PORT0 : Set EMB Data Bus to Port0 (Default)
 *  @arg\b      EMB_DATA_PORT_SELECT_PORT2 : Set EMB Data Bus to Port2
 *  @arg\b      EMB_DATA_PORT_SELECT_PORT567 : Set EMB Data Bus to Port5/6/7
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_EMB_DataBus_PinMux_Select(EMB_DATA_PORT_SELECT_PORT2);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_EMB_DataBus_PinMux_Select
#define EMB_DATA_PORT_SELECT_PORT0       0x00
#define EMB_DATA_PORT_SELECT_PORT2       ADLPS0
#define EMB_DATA_PORT_SELECT_PORT567     ADLPS1
/// @endcond
#define __DRV_EMB_DataBus_PinMux_Select(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR14_Page);\
        AUXR14 = (AUXR14 & ~(ADLPS1 | ADLPS0)) | __SELECT__;\
        __DRV_SFR_PageIndex(0);\
    )

#endif  //_MG82F6D64_EMB_DRV_H
