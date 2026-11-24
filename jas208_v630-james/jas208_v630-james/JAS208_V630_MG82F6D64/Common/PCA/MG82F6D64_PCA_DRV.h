/**
 ******************************************************************************
 *
 * @file        MG82F6D64_PCA_DRV.h
 *
 * @brief       This is the C code format driver head file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     V1.02
 * @date        2022/06/09
 * @author      Megawin Software Center
 * @copyright   Copyright (c) 2019 Megawin Technology Co., Ltd.
 *              All rights reserved.
 *
 ******************************************************************************
 * @par         Disclaimer
 *      The Demo software is provided "AS IS"  without any warranty, either
 *      expressed or implied, including, but not limited to, the implied warranties
 *      of merchantability and fitness for a particular purpose.  The author will
 *      not be liable for any special, incidental, consequential or indirect
 *      damages due to loss of data or any other reason.
 *      These __STATE__ments agree with the world wide and local dictated laws about
 *      authorship and violence against these laws.
 ******************************************************************************
 ******************************************************************************
 */

#ifndef _MG82F6D64_PCA_DRV_H
#define _MG82F6D64_PCA_DRV_H

/// @cond Parameter_definition

//!@{
//! defgroup PCA input capture edge definition
#define PCA_IC_POSITIVE_EDGE        (CAPP0)
#define PCA_IC_NEGATIVE_EDGE        (CAPN0)
#define PCA_IC_DUAL_EDGE            (CAPP0 | CAPN0)
//!@}

//!@{
//! defgroup PCA clock source definition
#define PCA_CK_SYSCLK_DIV_12        (0x00)
#define PCA_CK_SYSCLK_DIV_2         (CPS0)
#define PCA_CK_T0OF                 (CPS1)
#define PCA_CK_ECI                  (CPS1 | CPS0)
#define PCA_CK_CKMX16               (CPS2)
#define PCA_CK_SYSCLK_DIV_1         (CPS2 | CPS0)
#define PCA_CK_S0TOF                (CPS2 | CPS1)
#define PCA_CK_MCKDO                (CPS2 | CPS1 | CPS0)
//!@}

//!@{
//! defgroup PCA PWM Unbuffered definition
#define PCA_PWM_UNBUFFER           (0x00)
#define PCA_PWM01_BUFFER           (BME0)
#define PCA_PWM23_BUFFER           (BME2)
#define PCA_PWM45_BUFFER           (BME4)
#define PCA_PWM67_BUFFER           (BME6)
//!@}


//!@{
//! defgroup PCA Dead Time Control definition
#define PCA_DTCK_SYSTEM_DIV_1          (0x00)
#define PCA_DTCK_SYSTEM_DIV_2          (DTPS0)
#define PCA_DTCK_SYSTEM_DIV_4          (DTPS1)
#define PCA_DTCK_SYSTEM_DIV_8          (DTPS1 | DTPS0)
//!@}

//!@{
//! defgroup PCA PWM Resolution
#define PCA_PWM_8BIT     (0x00)
#define PCA_PWM_10BIT    (P0RS0)
#define PCA_PWM_12BIT    (P0RS1)
#define PCA_PWM_16BIT    (P0RS1 | P0RS0)
//!@}

//!@{
//! defgroup PCA CEX1 CEX3 CEX5 Pin select
#define CEX1_P23_CEX3_P25_CEX5_P27     (0x00)
#define CEX1_P33_CEX3_P34_CEX5_P35     (0x01)
//!@}

//!@{
//! defgroup PCA CEX0 CEX2 CEX4 Pin select
#define CEX0_P22_CEX2_P24_CEX4_P26     (0x00)
#define CEX0_P40_CEX2_P41_CEX4_P32     (0x01)
//!@}

//!@{
//! defgroup PWM0A,PWM0B Pin select
#define PWM0A_P36_PWM0B_P37 (0x00)
#define PWM0A_P60_PWM0B_P61 (0x01)
//!@}

//!@{
//! defgroup PWM2A,PWM2B Pin select
#define PWM2A_P40_PWM2B_P41 (0x00)
#define PWM2A_P34_PWM2B_P35 (0x01)
//!@}

//!@{
//! defgroup CEX2 input Pin select
#define FROM_CEX2 (0x00)
#define FROM_T3EXI (0x01)
//!@}

//!@{
//! defgroup CEX4 input Pin select
#define FROM_CEX4 (0x00)
#define FROM_T2EXI (0x01)
//!@}

//!@{
//! defgroup PWM6,PWM7 Pin select
#define PWM6_P62_PWM7_P63 (0x00)
#define PWM6_P60_PWM7_P61 (0x01)
//!@}

//!@{
//! defgroup PWM break mode
#define LATCH (0x00)
#define CYCLE (0x01)
//!@}

//!@{
//! defgroup ECI Pin select
#define ECI_P21 (0x00)
#define ECI_P46 (0x01)
//!@}

//!@{
//! defgroup C0CKO Pin select
#define C0CKO_P47 (0x00)
#define C0CKO_P72 (0x01)
//!@}

//!@{
//! defgroup Break Source0 select
#define BREAK0_DISABLE (0x00)
#define BREAK0_AC0OUT  (PBKE00)
#define BREAK0_INT1ET  (PBKE01)
#define BREAK0_T2EXI   (PBKE01|PBKE00)
#define BREAK0_KBIET   (PBKE02)
#define BREAK0_AC1OUT  (PBKE02|PBKE00)
#define BREAK0_INT3ET  (PBKE02|PBKE01)
#define BREAK0_ADCWI   (PBKE02|PBKE01|PBKE00)
//!@}

//!@{
//! defgroup Break Source1 select
#define BREAK1_DISABLE (0x00)
#define BREAK1_INT2ET  (PBKE10)
#define BREAK1_T3EXI   (PBKE11)
#define BREAK1_KBIET   (PBKE11|PBKE10)
//!@}

//!@{
//! defgroup FIFO trigger source
#define FIFO_T0OF     0x00
#define FIFO_T1OF     (C0FDC0)
#define FIFO_T3OF     (C0FDC1)
#define FIFO_S0TOF    (C0FDC1|C0FDC0)
//!@}

//!@{
//! defgroup PWM break mode
#define CPU_WRITING (0x00)
#define PWM_CYCLE   (0x01)
//!@}

/// @endcond

/**
 *******************************************************************************
 * @brief       Enables the PCA.
 * @details     PCA start count
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_Enable()    CR = 1

/**
 *******************************************************************************
 * @brief       Disables the PCA.
 * @details     PCA stop count
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_Disable()    CR = 0

/**
 *******************************************************************************
 * @brief       Configure PCA Clock source.
 * @details
 * @param[in]   __CLOCKSRC__ : specifies PCA clock selection
 *  @arg\b      PCA_CK_SYSCLK_DIV_12 : PCA clock source by system clock with divider 12.
 *  @arg\b      PCA_CK_SYSCLK_DIV_2 : PCA clock source by system clock with divider 2.
 *  @arg\b      PCA_CK_T0OF : PCA clock source by Timer0 overflow.
 *  @arg\b      PCA_CK_ECI : PCA clock source by ECI pin.
 *  @arg\b      PCA_CK_CKMX16 : PCA clock source by CKMIX16.
 *  @arg\b      PCA_CK_SYSCLK_DIV_1 : PCA clock source by system clock.
 *  @arg\b      PCA_CK_S0TOF : PCA clock source by S0BRG overflow.
 *  @arg\b      PCA_CK_MCKDO : PCA clock source by MCK divider output.
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_ClockSource_Select(PCA_CK_SYSCLK_DIV_1);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_ClockSource_Select(__CLOCKSRC__)\
      MWT(\
        CMOD &=~(CPS2 | CPS1 | CPS0);\
        CMOD |= __CLOCKSRC__ & (CPS2 | CPS1 | CPS0);\
        )

/**
 *******************************************************************************
 * @brief       Set PCA counter value
 * @param[in]   __VALUE__: PCA counter value
 *  @arg\b      Value : 0~65535
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetCounterValue(0);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetCounterValue(__VALUE__)\
      MWT(\
          CH=HIBYTE(__VALUE__);\
          CL=LOBYTE(__VALUE__);\
         )

/**
 *******************************************************************************
 * @brief       Get PCA counter value
 * @return      16bit data from PCA counter Value
 * @note
 * @par         Example
 * @code
 *    16bitReg(user define) = __DRV_PCA_GetCounterValue();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_GetCounterValue() (CH << 8 | CL)

/**
 *******************************************************************************
 * @brief       Set PCA counter reload value
 * @param[in]   __VALUE__: PCA counter reload value
 *  @arg\b      Value : 0~65535
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetCounterReloadValue(0);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetCounterReloadValue(__VALUE__)\
      MWT(\
          CHRL=HIBYTE(__VALUE__);\
          CLRL=LOBYTE(__VALUE__);\
         )

/**
 *******************************************************************************
 * @brief       Get PCA counter reload value
 * @return      16bit data from PCA counter reload Value
 * @note
 * @par         Example
 * @code
 *    16bitReg(user define) = __DRV_PCA_GetCounterReloadValue();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_GetCounterReloadValue() (CHRL << 8 | CLRL)

/**
 *******************************************************************************
 * @brief       Configure the PCA module0 output inverted or non-inverted.
 * @details
 * @param[in]   __STATE__: specifies the PWM0 output Inverted
 *  @arg\b      MW_ENABLE: Inverted Compare/PWM output.
 *  @arg\b      MW_DISABLE: Non-inverted Compare/PWM output.
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM0_SetInverse(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM0_SetInverse(__STATE__)\
        __STATE__==MW_ENABLE?(PCAPWM0|=(P0INV)):(PCAPWM0&=~(P0INV))

/**
 *******************************************************************************
 * @brief       Configure the PCA module1 output inverted or non-inverted.
 * @details
 * @param[in]   __STATE__: specifies the PWM1 output Inverted
 *  @arg\b      MW_ENABLE: Inverted Compare/PWM output.
 *  @arg\b      MW_DISABLE: Non-inverted Compare/PWM output.
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM1_SetInverse(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM1_SetInverse(__STATE__)\
        __STATE__==MW_ENABLE?(PCAPWM1|=(P1INV)):(PCAPWM1&=~(P1INV))

/**
 *******************************************************************************
 * @brief       Configure the PCA module2 output inverted or non-inverted.
 * @details
 * @param[in]   __STATE__: specifies the PWM2 output Inverted
 *  @arg\b      MW_ENABLE: Inverted Compare/PWM output.
 *  @arg\b      MW_DISABLE: Non-inverted Compare/PWM output.
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM2_SetInverse(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM2_SetInverse(__STATE__)\
        __STATE__==MW_ENABLE?(PCAPWM2|=(P2INV)):(PCAPWM2&=~(P2INV))

/**
 *******************************************************************************
 * @brief       Configure the PCA module3 output inverted or non-inverted.
 * @details
 * @param[in]   __STATE__: specifies the PWM3 output Inverted
 *  @arg\b      MW_ENABLE: Inverted Compare/PWM output.
 *  @arg\b      MW_DISABLE: Non-inverted Compare/PWM output.
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM3_SetInverse(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM3_SetInverse(__STATE__)\
        __STATE__==MW_ENABLE?(PCAPWM3|=(P3INV)):(PCAPWM3&=~(P3INV))

/**
 *******************************************************************************
 * @brief       Configure the PCA module4 output inverted or non-inverted.
 * @details
 * @param[in]   __STATE__: specifies the PWM4 output Inverted
 *  @arg\b      MW_ENABLE: Inverted Compare/PWM output.
 *  @arg\b      MW_DISABLE: Non-inverted Compare/PWM output.
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM4_SetInverse(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM4_SetInverse(__STATE__)\
        __STATE__==MW_ENABLE?(PCAPWM4|=(P4INV)):(PCAPWM4&=~(P4INV))

/**
 *******************************************************************************
 * @brief       Configure the PCA module5 output inverted or non-inverted.
 * @details
 * @param[in]   __STATE__: specifies the PWM5 output Inverted
 *  @arg\b      MW_ENABLE: Inverted Compare/PWM output.
 *  @arg\b      MW_DISABLE: Non-inverted Compare/PWM output.
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM5_SetInverse(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM5_SetInverse(__STATE__)\
        __STATE__==MW_ENABLE?(PCAPWM5|=(P5INV)):(PCAPWM5&=~(P5INV))

/**
 *******************************************************************************
 * @brief       Configure the PCA module6 output inverted or non-inverted.
 * @details
 * @param[in]   __STATE__: specifies the PWM6 output Inverted
 *  @arg\b      MW_ENABLE: Inverted Compare/PWM output.
 *  @arg\b      MW_DISABLE: Non-inverted Compare/PWM output.
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM6_SetInverse(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM6_SetInverse(__STATE__)\
      MWT(\
        __DRV_SFR_PageIndex(PCAPWM6_Page);\
        __STATE__==MW_ENABLE?(PCAPWM6|=(P6INV)):(PCAPWM6&=~(P6INV));\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief       Configure the PCA module7 output inverted or non-inverted.
 * @details
 * @param[in]   __STATE__: specifies the PWM7 output Inverted
 *  @arg\b      MW_ENABLE: Inverted Compare/PWM output.
 *  @arg\b      MW_DISABLE: Non-inverted Compare/PWM output.
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM7_SetInverse(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM7_SetInverse(__STATE__)\
      MWT(\
        __DRV_SFR_PageIndex(PCAPWM7_Page);\
        __STATE__==MW_ENABLE?(PCAPWM7|=(P7INV)):(PCAPWM7&=~(P7INV));\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief       Initializes the PCA base unit for input capture according to the specified
 * @details
 * @param[in]   __CKSRC__ : specifies PCA clock selection
 *  @arg\b      PCA_CK_SYSCLK_DIV_12 : PCA clock source by system clock with divider 12
 *  @arg\b      PCA_CK_SYSCLK_DIV_2 : PCA clock source by system clock with divider 2
 *  @arg\b      PCA_CK_T0OF : PCA clock source by timer0 overflow
 *  @arg\b      PCA_CK_ECI : PCA clock source by ECI pin
 *  @arg\b      PCA_CK_CKMX16 : PCA clock source by CKMIX16
 *  @arg\b      PCA_CK_SYSCLK_DIV_1 : PCA clock source by system clock
 *  @arg\b      PCA_CK_S0TOF : PCA clock source by S0BRG overflow
 *  @arg\b      PCA_CK_MCKDO : PCA clock source by MCK divider output
 * @param[in]   __PERIOD__ : specifies PCA period (0~65536)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_IC_Init(PCA_CK_SYSCLK_DIV_2, 0x1023);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_IC_Init(__CKSRC__,__PERIOD__)\
      MWT(\
         __DRV_PCA_Disable();\
         CH = 0; CHRL = HIBYTE(65536-(__PERIOD__));\
         CL = 0; CLRL = LOBYTE(65536-(__PERIOD__));\
         __DRV_PCA_ClockSource_Select(__CKSRC__);\
        )

/**
 *******************************************************************************
 * @brief       DeInitializes the PCA peripheral
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_DeInit();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_DeInit()\
      MWT(\
        __DRV_PCA_Disable();\
        CMOD = 0;\
        )

/**
 *******************************************************************************
 * @brief       PCA counter Idle control.
 * @details
 * @param[in]   __STATE__: Specifies PCA cannot operate in idle mode
 *  @arg\b      MW_ENABLE: Lets the PCA counter be gated off during Idle mode.
 *  @arg\b      MW_DISABLE: Lets the PCA counter continue functioning during Idle mode.(Default)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetIdleGateOff(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetIdleGateOff(__STATE__)\
        __STATE__==MW_ENABLE?(CMOD|=(CIDL)):(CMOD&=~(CIDL))

/**
 *******************************************************************************
 * @brief       Starts the PCA Input Capture channel 0 measurement.
 * @details
 * @param[in]   __EDGE__ : specifies PCA capture edge selection
 *  @arg\b      PCA_IC_POSITIVE_EDGE : Capture by a Positive edge trigger
 *  @arg\b      PCA_IC_NEGATIVE_EDGE : Capture by a Negative edge trigger
 *  @arg\b      PCA_IC_DUAL_EDGE : Capture by a transition
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_IC0_Start(PCA_IC_POSITIVE_EDGE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_IC0_Start(__EDGE__)\
      MWT(\
         CCAPM0 &= 0x01;\
         CCAPM0 |= __EDGE__&PCA_IC_DUAL_EDGE;\
         __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Starts the PCA Input Capture channel 1 measurement.
 * @details
 * @param[in]   __EDGE__ : specifies PCA capture edge selection
 *  @arg\b      PCA_IC_POSITIVE_EDGE : Capture by a Positive edge trigger
 *  @arg\b      PCA_IC_NEGATIVE_EDGE : Capture by a Negative edge trigger
 *  @arg\b      PCA_IC_DUAL_EDGE : Capture by a transition
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_IC1_Start(PCA_IC_POSITIVE_EDGE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_IC1_Start(__EDGE__)\
      MWT(\
         CCAPM1 &= 0x01;\
         CCAPM1 |= __EDGE__&PCA_IC_DUAL_EDGE;\
         __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Starts the PCA Input Capture channel 2 measurement.
 * @details
 * @param[in]   __EDGE__ : specifies PCA capture edge selection
 *  @arg\b      PCA_IC_POSITIVE_EDGE : Capture by a Positive edge trigger
 *  @arg\b      PCA_IC_NEGATIVE_EDGE : Capture by a Negative edge trigger
 *  @arg\b      PCA_IC_DUAL_EDGE : Capture by a transition
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_IC2_Start(PCA_IC_POSITIVE_EDGE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_IC2_Start(__EDGE__)\
      MWT(\
         CCAPM2 &= 0x01;\
         CCAPM2 |= __EDGE__&PCA_IC_DUAL_EDGE;\
         __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Starts the PCA Input Capture channel 3 measurement.
 * @details
 * @param[in]   __EDGE__ : specifies PCA capture edge selection
 *  @arg\b      PCA_IC_POSITIVE_EDGE : Capture by a Positive edge trigger
 *  @arg\b      PCA_IC_NEGATIVE_EDGE : Capture by a Negative edge trigger
 *  @arg\b      PCA_IC_DUAL_EDGE : Capture by a transition
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_IC3_Start(PCA_IC_POSITIVE_EDGE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_IC3_Start(__EDGE__)\
      MWT(\
         CCAPM3 &= 0x01;\
         CCAPM3 |= __EDGE__&PCA_IC_DUAL_EDGE;\
         __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Starts the PCA Input Capture channel 4 measurement.
 * @details
 * @param[in]   __EDGE__ : specifies PCA capture edge selection
 *  @arg\b      PCA_IC_POSITIVE_EDGE : Capture by a Positive edge trigger
 *  @arg\b      PCA_IC_NEGATIVE_EDGE : Capture by a Negative edge trigger
 *  @arg\b      PCA_IC_DUAL_EDGE : Capture by a transition
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_IC4_Start(PCA_IC_POSITIVE_EDGE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_IC4_Start(__EDGE__)\
      MWT(\
         CCAPM4 &= 0x01;\
         CCAPM4 |= __EDGE__&PCA_IC_DUAL_EDGE;\
         __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Starts the PCA Input Capture channel 5 measurement.
 * @details
 * @param[in]   __EDGE__ : specifies PCA capture edge selection
 *  @arg\b      PCA_IC_POSITIVE_EDGE : Capture by a Positive edge trigger
 *  @arg\b      PCA_IC_NEGATIVE_EDGE : Capture by a Negative edge trigger
 *  @arg\b      PCA_IC_DUAL_EDGE : Capture by a transition
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_IC5_Start(PCA_IC_POSITIVE_EDGE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_IC5_Start(__EDGE__)\
      MWT(\
         CCAPM5 &= 0x01;\
         CCAPM5 |= __EDGE__&PCA_IC_DUAL_EDGE;\
         __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Stop the PCA Input Capture channel 0 measurement.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_IC0_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_IC0_Stop()\
        (CCAPM0 &= 0x01)

/**
 *******************************************************************************
 * @brief       Stop the PCA Input Capture channel 1 measurement.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_IC1_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_IC1_Stop()\
        (CCAPM1 &= 0x01)

/**
 *******************************************************************************
 * @brief       Stop the PCA Input Capture channel 2 measurement.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_IC2_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_IC2_Stop()\
        (CCAPM2 &= 0x01)

/**
 *******************************************************************************
 * @brief       Stop the PCA Input Capture channel 3 measurement.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_IC3_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_IC3_Stop()\
        (CCAPM3 &= 0x01)

/**
 *******************************************************************************
 * @brief       Stop the PCA Input Capture channel 4 measurement.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_IC4_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_IC4_Stop()\
        (CCAPM4 &= 0x01)

/**
 *******************************************************************************
 * @brief       Stop the PCA Input Capture channel 5 measurement.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_IC5_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_IC5_Stop()\
        (CCAPM5 &= 0x01)


/**
 *******************************************************************************
 * @brief       Clear the PCA overflow interrupt flag.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_Overflow_ClearFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_Overflow_ClearFlag()  CF = 0;

/**
 *******************************************************************************
 * @brief       Clear the PCA PWM break interrupt flag.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_Break_ClearFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_Break_ClearFlag()  AUXR0&=~PBKF;

/**
 *******************************************************************************
 * @brief       Clear the PCA modele0 interrupt flag.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_Module0_ClearFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_Module0_ClearFlag()  CCF0 = 0;

/**
 *******************************************************************************
 * @brief       Clear the PCA modele1 interrupt flag.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_Module1_ClearFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_Module1_ClearFlag()  CCF1 = 0;

/**
 *******************************************************************************
 * @brief       Clear the PCA modele2 interrupt flag.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_Module2_ClearFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_Module2_ClearFlag()  CCF2 = 0;

/**
 *******************************************************************************
 * @brief       Clear the PCA modele3 interrupt flag.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_Module3_ClearFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_Module3_ClearFlag()  CCF3 = 0;

/**
 *******************************************************************************
 * @brief       Clear the PCA modele4 interrupt flag.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_Module4_ClearFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_Module4_ClearFlag()  CCF4 = 0;

/**
 *******************************************************************************
 * @brief       Clear the PCA modele5 interrupt flag.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_Module5_ClearFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_Module5_ClearFlag()  CCF5 = 0;

/**
 *******************************************************************************
 * @brief       Clear the PCA modele6 interrupt flag.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_Module6_ClearFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_Module6_ClearFlag()\
      MWT(\
        __DRV_SFR_PageIndex(PCAPWM6_Page);\
        PCAPWM6 &= ~CCF6;\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief       Clear the PCA modele7 interrupt flag.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_Module7_ClearFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_Module7_ClearFlag()\
      MWT(\
        __DRV_SFR_PageIndex(PCAPWM7_Page);\
        PCAPWM7 &= ~CCF7;\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief       Get the PCA Input Capture channel 0 Value
 * @details
 * @return      16bit data from Capture Value
 * @note
 * @par         Example
 * @code
 *    16bitReg(user define) = __DRV_PCA_GetIC0Value();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_GetIC0Value() (CCAP0H << 8 | CCAP0L)

/**
 *******************************************************************************
 * @brief       Get the PCA Input Capture channel 1 Value
 * @details
 * @return      16bit data from Capture Value
 * @note
 * @par         Example
 * @code
 *    16bitReg(user define) = __DRV_PCA_GetIC1Value();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_GetIC1Value() (CCAP1H << 8 | CCAP1L)

/**
 *******************************************************************************
 * @brief       Get the PCA Input Capture channel 2 Value
 * @details
 * @return      16bit data from Capture Value
 * @note
 * @par         Example
 * @code
 *    16bitReg(user define) = __DRV_PCA_GetIC2Value();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_GetIC2Value() (CCAP2H << 8 | CCAP2L)

/**
 *******************************************************************************
 * @brief       Get the PCA Input Capture channel 3 Value
 * @details
 * @return      16bit data from Capture Value
 * @note
 * @par         Example
 * @code
 *    16bitReg(user define) = __DRV_PCA_GetIC3Value();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_GetIC3Value() (CCAP3H << 8 | CCAP3L)

/**
 *******************************************************************************
 * @brief       Get the PCA Input Capture channel 4 Value
 * @details
 * @return      16bit data from Capture Value
 * @note
 * @par         Example
 * @code
 *    16bitReg(user define) = __DRV_PCA_GetIC4Value();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_GetIC4Value() (CCAP4H << 8 | CCAP4L)

/**
 *******************************************************************************
 * @brief       Get the PCA Input Capture channel 5 Value
 * @details
 * @return      16bit data from Capture Value
 * @note
 * @par         Example
 * @code
 *    16bitReg(user define) = __DRV_PCA_GetIC5Value();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_GetIC5Value() (CCAP5H << 8 | CCAP5L)

/**
 *******************************************************************************
 * @brief       Initializes the PCA base unit for output compare according to the specified
 * @details
 * @param[in]   __CKSRC__ : specifies PCA clock selection
 *  @arg\b      PCA_CK_SYSCLK_DIV_12 : PCA clock source by system clock with divider 12
 *  @arg\b      PCA_CK_SYSCLK_DIV_2 : PCA clock source by system clock with divider 2
 *  @arg\b      PCA_CK_T0OF : PCA clock source by timer0 overflow
 *  @arg\b      PCA_CK_ECI : PCA clock source by ECI pin
 *  @arg\b      PCA_CK_CKMX16 : PCA clock source by CKMIX16
 *  @arg\b      PCA_CK_SYSCLK_DIV_1 : PCA clock source by system clock
 *  @arg\b      PCA_CK_S0TOF : PCA clock source by S0BRG overflow
 *  @arg\b      PCA_CK_MCKDO : PCA clock source by MCK divider output
 * @param[in]   __BUFFERED__ : Buffered mode Configure
 *  @arg\b      PCA_PWM_UNBUFFER : Unbuffered mode
 *  @arg\b      PCA_PWM01_BUFFER : PWM channel 0 and channel 1 are buffered mode
 *  @arg\b      PCA_PWM23_BUFFER : PWM channel 2 and channel 3 are buffered mode
 *  @arg\b      PCA_PWM45_BUFFER : PWM channel 4 and channel 5 are buffered mode
 *  @arg\b      PCA_PWM67_BUFFER : PWM channel 6 and channel 7 are buffered mode
 * @param[in]   __PERIOD__ : specifies PCA period (0~65536)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_OC_Init(PCA_CK_SYSCLK_DIV_2,PCA_PWM01_BUFFER | PCA_PWM23_BUFFER, 1023);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_OC_Init(__CKSRC__,__BUFFERED__,__PERIOD__)\
      MWT(\
        __DRV_PCA_Disable();\
        CH = 0; CHRL = HIBYTE(65536-(__PERIOD__));\
        CL = 0; CLRL = LOBYTE(65536-(__PERIOD__));\
        __DRV_PCA_ClockSource_Select(__CKSRC__);\
        CMOD &=0x8F;\
        CMOD |=__BUFFERED__&0x70;\
        __DRV_SFR_PageIndex(CCAPM6_Page);\
        ((__BUFFERED__&(PCA_PWM67_BUFFER))==(0x00)?(CCAPM6 &= ~BME6):(CCAPM6 |= BME6));\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief       Starts the PCA channel 0 output compare.
 * @details
 * @param[in]  __RES__ : PWMn Resolution Setting
 * @arg\b      PCA_PWM_8BIT : PWMn Resolution the 8bit
 * @arg\b      PCA_PWM_10BIT : PWMn Resolution the 10bit
 * @arg\b      PCA_PWM_12BIT : PWMn Resolution the 12bit
 * @arg\b      PCA_PWM_16BIT : PWMn Resolution the 16bit
 * @param[in]  __VALUE__ : specifies PCA Channel 0 compare value(0~65535)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_OC0_Start(PCA_PWM_10BIT,256);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_OC0_Start(__RES__,__VALUE__)\
      MWT(\
        PCAPWM0 &= ~(P0RS1 | P0RS0);\
        PCAPWM0 |= __RES__&PCA_PWM_16BIT;\
        CCAPM0 = (ECOM0 | TOG0 | PWM0);\
        __RES__==PCA_PWM_8BIT?(CCAP0H = LOBYTE(__VALUE__)):(CCAP0H = HIBYTE(__VALUE__));\
        CCAP0L = LOBYTE(__VALUE__);\
        __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Starts the PCA channel 1 output compare.
 * @details
 * @param[in]  __RES__ : PWMn Resolution Setting
 * @arg\b      PCA_PWM_8BIT : PWMn Resolution the 8bit
 * @arg\b      PCA_PWM_10BIT : PWMn Resolution the 10bit
 * @arg\b      PCA_PWM_12BIT : PWMn Resolution the 12bit
 * @arg\b      PCA_PWM_16BIT : PWMn Resolution the 16bit
 * @param[in]  __VALUE__ : specifies PCA Channel 1 compare value(0~65535)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_OC1_Start(PCA_PWM_10BIT,256);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_OC1_Start(__RES__,__VALUE__)\
      MWT(\
        PCAPWM1 &= ~(P0RS1 | P0RS0);\
        PCAPWM1 |= __RES__&PCA_PWM_16BIT;\
        CCAPM1 = (ECOM0 | TOG0 | PWM0);\
        __RES__==PCA_PWM_8BIT?(CCAP1H = LOBYTE(__VALUE__)):(CCAP1H = HIBYTE(__VALUE__));\
        CCAP1L = LOBYTE(__VALUE__);\
        __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Starts the PCA channel 2 output compare.
 * @details
 * @param[in]  __RES__ : PWMn Resolution Setting
 * @arg\b      PCA_PWM_8BIT : PWMn Resolution the 8bit
 * @arg\b      PCA_PWM_10BIT : PWMn Resolution the 10bit
 * @arg\b      PCA_PWM_12BIT : PWMn Resolution the 12bit
 * @arg\b      PCA_PWM_16BIT : PWMn Resolution the 16bit
 * @param[in]  __VALUE__ : specifies PCA Channel 2 compare value(0~65535)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_OC2_Start(PCA_PWM_10BIT,256);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_OC2_Start(__RES__,__VALUE__)\
      MWT(\
        PCAPWM2 &= ~(P0RS1 | P0RS0);\
        PCAPWM2 |= __RES__&PCA_PWM_16BIT;\
        CCAPM2 = (ECOM0 | TOG0 | PWM0);\
        __RES__==PCA_PWM_8BIT?(CCAP2H = LOBYTE(__VALUE__)):(CCAP2H = HIBYTE(__VALUE__));\
        CCAP2L = LOBYTE(__VALUE__);\
        __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Starts the PCA channel 3 output compare.
 * @details
 * @param[in]  __RES__ : PWMn Resolution Setting
 * @arg\b      PCA_PWM_8BIT : PWMn Resolution the 8bit
 * @arg\b      PCA_PWM_10BIT : PWMn Resolution the 10bit
 * @arg\b      PCA_PWM_12BIT : PWMn Resolution the 12bit
 * @arg\b      PCA_PWM_16BIT : PWMn Resolution the 16bit
 * @param[in]  __VALUE__ : specifies PCA Channel 3 compare value(0~65535)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_OC3_Start(PCA_PWM_10BIT,256);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_OC3_Start(__RES__,__VALUE__)\
      MWT(\
        PCAPWM3 &= ~(P0RS1 | P0RS0);\
        PCAPWM3 |= __RES__&PCA_PWM_16BIT;\
        CCAPM3 = (ECOM0 | TOG0 | PWM0);\
        __RES__==PCA_PWM_8BIT?(CCAP3H = LOBYTE(__VALUE__)):(CCAP3H = HIBYTE(__VALUE__));\
        CCAP3L = LOBYTE(__VALUE__);\
        __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Starts the PCA channel 4 output compare.
 * @details
 * @param[in]  __RES__ : PWMn Resolution Setting
 * @arg\b      PCA_PWM_8BIT : PWMn Resolution the 8bit
 * @arg\b      PCA_PWM_10BIT : PWMn Resolution the 10bit
 * @arg\b      PCA_PWM_12BIT : PWMn Resolution the 12bit
 * @arg\b      PCA_PWM_16BIT : PWMn Resolution the 16bit
 * @param[in]  __VALUE__ : specifies PCA Channel 4 compare value(0~65535)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_OC4_Start(PCA_PWM_10BIT,256);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_OC4_Start(__RES__,__VALUE__)\
      MWT(\
        PCAPWM4 &= ~(P0RS1 | P0RS0);\
        PCAPWM4 |= __RES__&PCA_PWM_16BIT;\
        CCAPM4 = (ECOM0 | TOG0 | PWM0);\
        __RES__==PCA_PWM_8BIT?(CCAP4H = LOBYTE(__VALUE__)):(CCAP4H = HIBYTE(__VALUE__));\
        CCAP4L = LOBYTE(__VALUE__);\
        __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Starts the PCA channel 5 output compare.
 * @details
 * @param[in]  __RES__ : PWMn Resolution Setting
 * @arg\b      PCA_PWM_8BIT : PWMn Resolution the 8bit
 * @arg\b      PCA_PWM_10BIT : PWMn Resolution the 10bit
 * @arg\b      PCA_PWM_12BIT : PWMn Resolution the 12bit
 * @arg\b      PCA_PWM_16BIT : PWMn Resolution the 16bit
 * @param[in]  __VALUE__ : specifies PCA Channel 5 compare value(0~65535)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_OC5_Start(PCA_PWM_10BIT,256);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_OC5_Start(__RES__,__VALUE__)\
      MWT(\
        PCAPWM5 &= ~(P0RS1 | P0RS0);\
        PCAPWM5 |= __RES__&PCA_PWM_16BIT;\
        CCAPM5 = (ECOM0 | TOG0 | PWM0);\
        __RES__==PCA_PWM_8BIT?(CCAP5H = LOBYTE(__VALUE__)):(CCAP5H = HIBYTE(__VALUE__));\
        CCAP5L = LOBYTE(__VALUE__);\
        __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Starts the PCA channel 6 output compare.
 * @details
 * @param[in]  __RES__ : PWMn Resolution Setting
 * @arg\b      PCA_PWM_8BIT : PWMn Resolution the 8bit
 * @arg\b      PCA_PWM_10BIT : PWMn Resolution the 10bit
 * @arg\b      PCA_PWM_12BIT : PWMn Resolution the 12bit
 * @arg\b      PCA_PWM_16BIT : PWMn Resolution the 16bit
 * @param[in]  __VALUE__ : specifies PCA Channel 6 compare value(0~65535)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_OC6_Start(PCA_PWM_10BIT,256);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_OC6_Start(__RES__,__VALUE__)\
      MWT(\
        __DRV_SFR_PageIndex(PCAPWM6_Page);\
        PCAPWM6 &= ~(P0RS1 | P0RS0);\
        PCAPWM6 |= __RES__&PCA_PWM_16BIT;\
        CCAPM6 &= 0x7F;\
        CCAPM6 |= (ECOM0 | TOG0 | PWM0);\
        __RES__==PCA_PWM_8BIT?(CCAP6H = LOBYTE(__VALUE__)):(CCAP6H = HIBYTE(__VALUE__));\
        CCAP6L = LOBYTE(__VALUE__);\
        __DRV_SFR_PageIndex(0);\
        __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Starts the PCA channel 7 output compare.
 * @details
 * @param[in]  __RES__ : PWMn Resolution Setting
 * @arg\b      PCA_PWM_8BIT : PWMn Resolution the 8bit
 * @arg\b      PCA_PWM_10BIT : PWMn Resolution the 10bit
 * @arg\b      PCA_PWM_12BIT : PWMn Resolution the 12bit
 * @arg\b      PCA_PWM_16BIT : PWMn Resolution the 16bit
 * @param[in]  __VALUE__ : specifies PCA Channel 7 compare value(0~65535)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_OC7_Start(PCA_PWM_10BIT,256);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_OC7_Start(__RES__,__VALUE__)\
      MWT(\
        __DRV_SFR_PageIndex(PCAPWM7_Page);\
        PCAPWM7 &= ~(P0RS1 | P0RS0);\
        PCAPWM7 |= __RES__&PCA_PWM_16BIT;\
        CCAPM7 = (ECOM0 | TOG0 | PWM0);\
        __RES__==PCA_PWM_8BIT?(CCAP7H = LOBYTE(__VALUE__)):(CCAP7H = HIBYTE(__VALUE__));\
        CCAP7L = LOBYTE(__VALUE__);\
        __DRV_SFR_PageIndex(0);\
        __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Stop the PCA output compare channel 0.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_OC0_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_OC0_Stop()\
        (CCAPM0 &= 0x01)

/**
 *******************************************************************************
 * @brief       Stop the PCA output compare channel 1.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_OC1_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_OC1_Stop()\
        (CCAPM1 &= 0x01)

/**
 *******************************************************************************
 * @brief       Stop the PCA output compare channel 2.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_OC2_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_OC2_Stop()\
        (CCAPM2 &= 0x01)

/**
 *******************************************************************************
 * @brief       Stop the PCA output compare channel 3.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_OC3_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_OC3_Stop()\
        (CCAPM3 &= 0x01)

/**
 *******************************************************************************
 * @brief       Stop the PCA output compare channel 4.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_OC4_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_OC4_Stop()\
        (CCAPM4 &= 0x01)

/**
 *******************************************************************************
 * @brief       Stop the PCA output compare channel 5.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_OC5_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_OC5_Stop()\
        (CCAPM5 &= 0x01)

/**
 *******************************************************************************
 * @brief       Stop the PCA output compare channel 6.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_OC6_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_OC6_Stop()\
      MWT(\
        __DRV_SFR_PageIndex(CCAPM6_Page);\
        (CCAPM6 &= 0x01);\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief       Stop the PCA output compare channel 7.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_OC7_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_OC7_Stop()\
      MWT(\
        __DRV_SFR_PageIndex(CCAPM7_Page);\
        (CCAPM7 &= 0x01);\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief       Initializes the PCA base unit for PWM according to the specified
 * @details
 * @param[in]   __CKSRC__ : specifies PCA clock selection
 *  @arg\b      PCA_CK_SYSCLK_DIV_12 : PCA clock source by system clock with divider 12
 *  @arg\b      PCA_CK_SYSCLK_DIV_2 : PCA clock source by system clock with divider 2
 *  @arg\b      PCA_CK_T0OF : PCA clock source by timer0 overflow
 *  @arg\b      PCA_CK_ECI : PCA clock source by ECI pin
 *  @arg\b      PCA_CK_CKMX16 : PCA clock source by CKMIX16
 *  @arg\b      PCA_CK_SYSCLK_DIV_1 : PCA clock source by system clock
 *  @arg\b      PCA_CK_S0TOF : PCA clock source by S0BRG overflow
 *  @arg\b      PCA_CK_MCKDO : PCA clock source by MCK divider output
 * @param[in]   __BUFFERED__ : Buffered mode Configure
 *  @arg\b      PCA_PWM_UNBUFFER : Unbuffered mode
 *  @arg\b      PCA_PWM01_BUFFER : PWM channel 0 and channel 1 are buffered mode
 *  @arg\b      PCA_PWM23_BUFFER : PWM channel 2 and channel 3 are buffered mode
 *  @arg\b      PCA_PWM45_BUFFER : PWM channel 4 and channel 5 are buffered mode
 *  @arg\b      PCA_PWM67_BUFFER : PWM channel 6 and channel 7 are buffered mode
 * @param[in]   __PERIOD__ : specifies PCA period (0~65536)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM_Init(PCA_CK_SYSCLK_DIV_2,PCA_PWM01_BUFFER | PCA_PWM23_BUFFER, 1023);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM_Init(__CKSRC__,__BUFFERED__,__PERIOD__)\
      MWT(\
        __DRV_PCA_Disable();\
        CH = 0; CHRL = HIBYTE(65536-(__PERIOD__));\
        CL = 0; CLRL = LOBYTE(65536-(__PERIOD__));\
        __DRV_PCA_ClockSource_Select(__CKSRC__);\
        CMOD &=0x8F;\
        ((__BUFFERED__&(0x80000000+PCA_PWM67_BUFFER))==(0x80000000+PCA_PWM67_BUFFER)?(CMOD |=(__BUFFERED__&0x70)):(CMOD |=__BUFFERED__));\
        ((__BUFFERED__&(0x80000000+PCA_PWM67_BUFFER))==(0x80000000+PCA_PWM67_BUFFER)?(__DRV_SFR_PageIndex(CCAPM6_Page)):(_nop_()));\
        ((__BUFFERED__&(0x80000000+PCA_PWM67_BUFFER))==(0x80000000+PCA_PWM67_BUFFER)?(CCAPM6 |= BME6):(CCAPM6 &= ~BME6));\
        )

/**
 *******************************************************************************
 * @brief       Starts the PCA channel 0 PWM output.
 * @details
 * @param[in]   __RES__ : PWMn Resolution Setting
 * @arg\b       PCA_PWM_8BIT : PWMn Resolution the 8bit
 * @arg\b       PCA_PWM_10BIT : PWMn Resolution the 10bit
 * @arg\b       PCA_PWM_12BIT : PWMn Resolution the 12bit
 * @arg\b       PCA_PWM_16BIT : PWMn Resolution the 16bit
 * @param[in]   __DUTY__ : specifies PCA Channel 0 duty cycle(0~65535)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM0_Start(PCA_PWM_16BIT,0x8000)
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM0_Start(__RES__,__DUTY__)\
      MWT(\
        PCAPWM0 &= ~(P0RS1 | P0RS0);\
        PCAPWM0 |= __RES__&PCA_PWM_16BIT;\
        CCAPM0 = (ECOM0  | PWM0);\
        __RES__==PCA_PWM_8BIT?(CCAP0H = LOBYTE(__DUTY__)):(CCAP0H = HIBYTE(__DUTY__));\
        CCAP0L = LOBYTE(__DUTY__);\
        __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Starts the PCA channel 1 PWM output.
 * @details
 * @param[in]   __RES__ : PWMn Resolution Setting
 * @arg\b       PCA_PWM_8BIT : PWMn Resolution the 8bit
 * @arg\b       PCA_PWM_10BIT : PWMn Resolution the 10bit
 * @arg\b       PCA_PWM_12BIT : PWMn Resolution the 12bit
 * @arg\b       PCA_PWM_16BIT : PWMn Resolution the 16bit
 * @param[in]   __DUTY__ : specifies PCA Channel 1 duty cycle(0~65535)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM1_Start(PCA_PWM_16BIT,0x8000)
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM1_Start(__RES__,__DUTY__)\
      MWT(\
        PCAPWM1 &= ~(P0RS1 | P0RS0);\
        PCAPWM1 |= __RES__&PCA_PWM_16BIT;\
        CCAPM1 = (ECOM0  | PWM0);\
        __RES__==PCA_PWM_8BIT?(CCAP1H = LOBYTE(__DUTY__)):(CCAP1H = HIBYTE(__DUTY__));\
        CCAP1L = LOBYTE(__DUTY__);\
        __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Starts the PCA channel 2 PWM output.
 * @details
 * @param[in]   __RES__ : PWMn Resolution Setting
 * @arg\b       PCA_PWM_8BIT : PWMn Resolution the 8bit
 * @arg\b       PCA_PWM_10BIT : PWMn Resolution the 10bit
 * @arg\b       PCA_PWM_12BIT : PWMn Resolution the 12bit
 * @arg\b       PCA_PWM_16BIT : PWMn Resolution the 16bit
 * @param[in]   __DUTY__ : specifies PCA Channel 2 duty cycle(0~65535)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM2_Start(PCA_PWM_16BIT,0x8000)
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM2_Start(__RES__,__DUTY__)\
      MWT(\
        PCAPWM2 &= ~(P0RS1 | P0RS0);\
        PCAPWM2 |= __RES__&PCA_PWM_16BIT;\
        CCAPM2 = (ECOM0  | PWM0);\
        __RES__==PCA_PWM_8BIT?(CCAP2H = LOBYTE(__DUTY__)):(CCAP2H = HIBYTE(__DUTY__));\
        CCAP2L = LOBYTE(__DUTY__);\
        __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Starts the PCA channel 3 PWM output.
 * @details
 * @param[in]   __RES__ : PWMn Resolution Setting
 * @arg\b       PCA_PWM_8BIT : PWMn Resolution the 8bit
 * @arg\b       PCA_PWM_10BIT : PWMn Resolution the 10bit
 * @arg\b       PCA_PWM_12BIT : PWMn Resolution the 12bit
 * @arg\b       PCA_PWM_16BIT : PWMn Resolution the 16bit
 * @param[in]   __DUTY__ : specifies PCA Channel 3 duty cycle(0~65535)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM3_Start(PCA_PWM_16BIT,0x8000)
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM3_Start(__RES__,__DUTY__)\
      MWT(\
        PCAPWM3 &= ~(P0RS1 | P0RS0);\
        PCAPWM3 |= __RES__&PCA_PWM_16BIT;\
        CCAPM3 = (ECOM0  | PWM0);\
        __RES__==PCA_PWM_8BIT?(CCAP3H = LOBYTE(__DUTY__)):(CCAP3H = HIBYTE(__DUTY__));\
        CCAP3L = LOBYTE(__DUTY__);\
        __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Starts the PCA channel 4 PWM output.
 * @details
 * @param[in]   __RES__ : PWMn Resolution Setting
 * @arg\b       PCA_PWM_8BIT : PWMn Resolution the 8bit
 * @arg\b       PCA_PWM_10BIT : PWMn Resolution the 10bit
 * @arg\b       PCA_PWM_12BIT : PWMn Resolution the 12bit
 * @arg\b       PCA_PWM_16BIT : PWMn Resolution the 16bit
 * @param[in]   __DUTY__ : specifies PCA Channel 4 duty cycle(0~65535)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM4_Start(PCA_PWM_16BIT,0x8000)
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM4_Start(__RES__,__DUTY__)\
      MWT(\
        PCAPWM4 &= ~(P0RS1 | P0RS0);\
        PCAPWM4 |= __RES__&PCA_PWM_16BIT;\
        CCAPM4 = (ECOM0  | PWM0);\
        __RES__==PCA_PWM_8BIT?(CCAP4H = LOBYTE(__DUTY__)):(CCAP4H = HIBYTE(__DUTY__));\
        CCAP4L = LOBYTE(__DUTY__);\
        __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Starts the PCA channel 5 PWM output.
 * @details
 * @param[in]   __RES__ : PWMn Resolution Setting
 * @arg\b       PCA_PWM_8BIT : PWMn Resolution the 8bit
 * @arg\b       PCA_PWM_10BIT : PWMn Resolution the 10bit
 * @arg\b       PCA_PWM_12BIT : PWMn Resolution the 12bit
 * @arg\b       PCA_PWM_16BIT : PWMn Resolution the 16bit
 * @param[in]   __DUTY__ : specifies PCA Channel 5 duty cycle(0~65535)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM5_Start(PCA_PWM_16BIT,0x8000)
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM5_Start(__RES__,__DUTY__)\
      MWT(\
        PCAPWM5 &= ~(P0RS1 | P0RS0);\
        PCAPWM5 |= __RES__&PCA_PWM_16BIT;\
        CCAPM5 = (ECOM0  | PWM0);\
        __RES__==PCA_PWM_8BIT?(CCAP5H = LOBYTE(__DUTY__)):(CCAP5H = HIBYTE(__DUTY__));\
        CCAP5L = LOBYTE(__DUTY__);\
        __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Starts the PCA channel 6 PWM output.
 * @details
 * @param[in]   __RES__ : PWMn Resolution Setting
 * @arg\b       PCA_PWM_8BIT : PWMn Resolution the 8bit
 * @arg\b       PCA_PWM_10BIT : PWMn Resolution the 10bit
 * @arg\b       PCA_PWM_12BIT : PWMn Resolution the 12bit
 * @arg\b       PCA_PWM_16BIT : PWMn Resolution the 16bit
 * @param[in]   __DUTY__ : specifies PCA Channel 6 duty cycle(0~65535)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM6_Start(PCA_PWM_16BIT,0x8000)
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM6_Start(__RES__,__DUTY__)\
      MWT(\
        __DRV_SFR_PageIndex(PCAPWM6_Page);\
        PCAPWM6 &= ~(P0RS1 | P0RS0);\
        PCAPWM6 |= __RES__&PCA_PWM_16BIT;\
        CCAPM6 = (ECOM0  | PWM0);\
        __RES__==PCA_PWM_8BIT?(CCAP6H = LOBYTE(__DUTY__)):(CCAP6H = HIBYTE(__DUTY__));\
        CCAP6L = LOBYTE(__DUTY__);\
        __DRV_SFR_PageIndex(0);\
        __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Starts the PCA channel 7 PWM output.
 * @details
 * @param[in]   __RES__ : PWMn Resolution Setting
 * @arg\b       PCA_PWM_8BIT : PWMn Resolution the 8bit
 * @arg\b       PCA_PWM_10BIT : PWMn Resolution the 10bit
 * @arg\b       PCA_PWM_12BIT : PWMn Resolution the 12bit
 * @arg\b       PCA_PWM_16BIT : PWMn Resolution the 16bit
 * @param[in]   __DUTY__ : specifies PCA Channel 7 duty cycle(0~65535)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM7_Start(PCA_PWM_16BIT,0x8000)
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM7_Start(__RES__,__DUTY__)\
      MWT(\
        __DRV_SFR_PageIndex(PCAPWM7_Page);\
        PCAPWM7 &= ~(P0RS1 | P0RS0);\
        PCAPWM7 |= __RES__&PCA_PWM_16BIT;\
        CCAPM7 = (ECOM0  | PWM0);\
        __RES__==PCA_PWM_8BIT?(CCAP7H = LOBYTE(__DUTY__)):(CCAP7H = HIBYTE(__DUTY__));\
        CCAP7L = LOBYTE(__DUTY__);\
        __DRV_SFR_PageIndex(0);\
        __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Stop the PCA PWM channel 0.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM0_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM0_Stop()\
        (CCAPM0 &= 0x01)

/**
 *******************************************************************************
 * @brief       Stop the PCA PWM channel 1.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM1_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM1_Stop()\
        (CCAPM1 &= 0x01)

/**
 *******************************************************************************
 * @brief       Stop the PCA PWM channel 2.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM2_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM2_Stop()\
        (CCAPM2 &= 0x01)

/**
 *******************************************************************************
 * @brief       Stop the PCA PWM channel 3.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM3_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM3_Stop()\
        (CCAPM3 &= 0x01)

/**
 *******************************************************************************
 * @brief       Stop the PCA PWM channel 4.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM4_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM4_Stop()\
        (CCAPM4 &= 0x01)

/**
 *******************************************************************************
 * @brief       Stop the PCA PWM channel 5.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM5_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM5_Stop()\
        (CCAPM5 &= 0x01)

/**
 *******************************************************************************
 * @brief       Stop the PCA PWM channel 6.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM6_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM6_Stop()\
      MWT(\
        __DRV_SFR_PageIndex(CCAPM6_Page);\
        (CCAPM6 &= 0x01);\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief       Stop the PCA PWM channel 7.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM7_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM7_Stop()\
      MWT(\
        __DRV_SFR_PageIndex(CCAPM7_Page);\
        (CCAPM7 &= 0x01);\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief	    Enable/Disable PWM0 output
 * @details
 * @param[in] __STATE__ :  Configure PWM0 output
 *      	  This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par       Example
 * @code
 *    __DRV_PCA_PWM0_Output_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM0_Output_Cmd(__STATE__)\
        __STATE__==MW_ENABLE?(PAOE|=(POE0)):(PAOE&=~(POE0))

/**
 *******************************************************************************
 * @brief	    Enable/Disable PWM1 output
 * @details
 * @param[in] __STATE__ :  Configure PWM1 output
 *      	  This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par       Example
 * @code
 *    __DRV_PCA_PWM1_Output_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM1_Output_Cmd(__STATE__)\
        __STATE__==MW_ENABLE?(PAOE|=(POE1)):(PAOE&=~(POE1))

/**
 *******************************************************************************
 * @brief	    Enable/Disable PWM2 output
 * @details
 * @param[in] __STATE__ :  Configure PWM2 output
 *      	  This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par       Example
 * @code
 *    __DRV_PCA_PWM2_Output_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM2_Output_Cmd(__STATE__)\
        __STATE__==MW_ENABLE?(PAOE|=(POE2)):(PAOE&=~(POE2))

/**
 *******************************************************************************
 * @brief	    Enable/Disable PWM3 output
 * @details
 * @param[in] __STATE__ :  Configure PWM3 output
 *      	  This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par       Example
 * @code
 *    __DRV_PCA_PWM3_Output_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM3_Output_Cmd(__STATE__)\
        __STATE__==MW_ENABLE?(PAOE|=(POE3)):(PAOE&=~(POE3))

/**
 *******************************************************************************
 * @brief	    Enable/Disable PWM4 output
 * @details
 * @param[in] __STATE__ :  Configure PWM4 output
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par       Example
 * @code
 *    __DRV_PCA_PWM4_Output_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM4_Output_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR7_Page);\
        __STATE__==MW_ENABLE?(AUXR7|=(POE4)):(AUXR7&=~(POE4));\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief	    Enable/Disable PWM5 output
 * @details
 * @param[in] __STATE__ :  Configure PWM5 output
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par       Example
 * @code
 *    __DRV_PCA_PWM5_Output_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM5_Output_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR7_Page);\
        __STATE__==MW_ENABLE?(AUXR7|=(POE5)):(AUXR7&=~(POE5));\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief	    Enable/Disable PWM6 output
 * @details
 * @param[in] __STATE__ :  Configure PWM6 output
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par       Example
 * @code
 *    __DRV_PCA_PWM6_Output_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM6_Output_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR8_Page);\
        __STATE__==MW_ENABLE?(AUXR8|=(POE6)):(AUXR8&=~(POE6));\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief	    Enable/Disable PWM7 output
 * @details
 * @param[in] __STATE__ :  Configure PWM7 output
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par       Example
 * @code
 *    __DRV_PCA_PWM7_Output_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM7_Output_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR8_Page);\
        __STATE__==MW_ENABLE?(AUXR8|=(POE7)):(AUXR8&=~(POE7));\
        __DRV_SFR_PageIndex(0);\
        )
/**
 *******************************************************************************
 * @brief	    Enable/Disable PWM0A output
 * @details
 * @param[in] __STATE__ :  Configure PWM0A output
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par       Example
 * @code
 *    __DRV_PCA_PWM0A_Output_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM0A_Output_Cmd(__STATE__)\
        __STATE__==MW_ENABLE?(PAOE|=(POE0A)):(PAOE&=~(POE0A))

/**
 *******************************************************************************
 * @brief	    Enable/Disable PWM0B output
 * @details
 * @param[in] __STATE__ :  Configure PWM0B output
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par       Example
 * @code
 *    __DRV_PCA_PWM0B_Output_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM0B_Output_Cmd(__STATE__)\
        __STATE__==MW_ENABLE?(PAOE|=(POE0B)):(PAOE&=~(POE0B))

/**
 *******************************************************************************
 * @brief	    Enable/Disable PWM2A output
 * @details
 * @param[in] __STATE__ :  Configure PWM2A output
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par       Example
 * @code
 *    __DRV_PCA_PWM2A_Output_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM2A_Output_Cmd(__STATE__)\
        __STATE__==MW_ENABLE?(PAOE|=(POE2A)):(PAOE&=~(POE2A))

/**
 *******************************************************************************
 * @brief	    Enable/Disable PWM2B output
 * @details
 * @param[in] __STATE__ :  Configure PWM2B output
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par       Example
 * @code
 *    __DRV_PCA_PWM2B_Output_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM2B_Output_Cmd(__STATE__)\
        __STATE__==MW_ENABLE?(PAOE|=(POE2B)):(PAOE&=~(POE2B))

/**
 *******************************************************************************
 * @brief	    Enable/Disable PWM central aligned
 * @details
 * @param[in] __STATE__ :  Configure PWM edge aligned or central aligned
 *  @arg\b    MW_ENABLE : PCA PWM central aligned mode
 *  @arg\b    MW_DISABLE : PCA PWM edge aligned mode
 * @return 	  None
 * @note
 * @par       Example
 * @code
 *    __DRV_PCA_PWM_CentralAligned_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM_CentralAligned_Cmd(__STATE__)\
      MWT(\
        __STATE__==MW_ENABLE?(PWMCR|=(PCAE)):(PWMCR&=~(PCAE));\
        __DRV_SFR_PageIndex(AUXR11_Page);\
        __STATE__==MW_ENABLE?(AUXR11|=(C0M0)):(AUXR11&=~(C0M0));\
        __DRV_SFR_PageIndex(0);\
        )
/**
 *******************************************************************************
 * @brief	    Enable/Disable PCA interrupts
 * @details
 * @param[in] __STATE__ :  Configure PCA interrupts
 *      	  This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par       Example
 * @code
 *    __DRV_PCA_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_IT_Cmd(__STATE__)\
        __STATE__==MW_ENABLE?(EIE1|=(EPCA)):(EIE1&=~(EPCA))

/**
 *******************************************************************************
 * @brief	    Enable/Disable PCA counter overflow interrupt
 * @details
 * @param[in] __STATE__ :  Configure PCA counter overflow interrupt
 *      	  This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par       Example
 * @code
 *    __DRV_PCA_Counter_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_Counter_IT_Cmd(__STATE__)\
        __STATE__==MW_ENABLE?(CMOD|=(ECF)):(CMOD&=~(ECF))

/**
 *******************************************************************************
 * @brief	    Enable/Disable PCA module 0 interrupt
 * @details
 * @param[in] __STATE__ :  Configure PCA Module 0 interrupts
 *      	  This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par       Example
 * @code
 *    __DRV_PCA_Module0_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_Module0_IT_Cmd(__STATE__)\
        __STATE__==MW_ENABLE?(CCAPM0|=(ECCF0)):(CCAPM0&=~(ECCF0))

/**
 *******************************************************************************
 * @brief	    Enable/Disable PCA module 1 interrupt
 * @details
 * @param[in] __STATE__ :  Configure PCA Module 1 interrupts
 *      	  This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par       Example
 * @code
 *    __DRV_PCA_Module1_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_Module1_IT_Cmd(__STATE__)\
        __STATE__==MW_ENABLE?(CCAPM1|=(ECCF1)):(CCAPM1&=~(ECCF1))

/**
 *******************************************************************************
 * @brief	    Enable/Disable PCA module 2 interrupt
 * @details
 * @param[in] __STATE__ :  Configure PCA Module 2 interrupts
 *      	  This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par       Example
 * @code
 *    __DRV_PCA_Module2_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_Module2_IT_Cmd(__STATE__)\
        __STATE__==MW_ENABLE?(CCAPM2|=(ECCF2)):(CCAPM2&=~(ECCF2))

/**
 *******************************************************************************
 * @brief	    Enable/Disable PCA module 3 interrupt
 * @details
 * @param[in] __STATE__ :  Configure PCA Module 3 interrupts
 *      	  This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par       Example
 * @code
 *    __DRV_PCA_Module3_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_Module3_IT_Cmd(__STATE__)\
        __STATE__==MW_ENABLE?(CCAPM3|=(ECCF3)):(CCAPM3&=~(ECCF3))

/**
 *******************************************************************************
 * @brief	    Enable/Disable PCA module 4 interrupt
 * @details
 * @param[in] __STATE__ :  Configure PCA Module 4 interrupts
 *      	  This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par       Example
 * @code
 *    __DRV_PCA_Module4_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_Module4_IT_Cmd(__STATE__)\
        __STATE__==MW_ENABLE?(CCAPM4|=(ECCF4)):(CCAPM4&=~(ECCF4))

/**
 *******************************************************************************
 * @brief	    Enable/Disable PCA module 5 interrupt
 * @details
 * @param[in] __STATE__ :  Configure PCA Module 5 interrupts
 *      	  This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par       Example
 * @code
 *    __DRV_PCA_Module5_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_Module5_IT_Cmd(__STATE__)\
        __STATE__==MW_ENABLE?(CCAPM5|=(ECCF5)):(CCAPM5&=~(ECCF5))

/**
 *******************************************************************************
 * @brief	    Enable/Disable PCA module 6 interrupt
 * @details
 * @param[in] __STATE__ :  Configure PCA Module 6 interrupts
 *      	  This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par       Example
 * @code
 *    __DRV_PCA_Module6_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_Module6_IT_Cmd(__STATE__)\
      MWT(\
        __DRV_SFR_PageIndex(CCAPM6_Page);\
        __STATE__==MW_ENABLE?(CCAPM6|=(ECCF6)):(CCAPM6&=~(ECCF6));\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief	    Enable/Disable PCA module 7 interrupt
 * @details
 * @param[in] __STATE__ :  Configure PCA Module 7 interrupts
 *      	  This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par       Example
 * @code
 *    __DRV_PCA_Module7_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_Module7_IT_Cmd(__STATE__)\
      MWT(\
        __DRV_SFR_PageIndex(CCAPM7_Page);\
        __STATE__==MW_ENABLE?(CCAPM7|=(ECCF7)):(CCAPM7&=~(ECCF7));\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief       Initializes the PCA base unit for PWM with dead time according to the specified
 * @details
 * @param[in]   __CKSRC__ : specifies PCA clock selection
 *  @arg\b      PCA_CK_SYSCLK_DIV_12 : PCA clock source by system clock with divider 12
 *  @arg\b      PCA_CK_SYSCLK_DIV_2 : PCA clock source by system clock with divider 2
 *  @arg\b      PCA_CK_T0OF : PCA clock source by timer0 overflow
 *  @arg\b      PCA_CK_ECI : PCA clock source by ECI pin
 *  @arg\b      PCA_CK_CKMX16 : PCA clock source by CKMIX16
 *  @arg\b      PCA_CK_SYSCLK_DIV_1 : PCA clock source by system clock
 *  @arg\b      PCA_CK_S0TOF : PCA clock source by S0BRG overflow
 *  @arg\b      PCA_CK_MCKDO : PCA clock source by MCK divider output
 * @param[in]   __PERIOD__ : specifies PCA period (0~65536)
 * @param[in]   __DTCKSRC__ : specifies Dead Time counter
 *  @arg\b      PCA_DTCK_SYSTEM_DIV_1 : Clock Pre-Scaler of Dead Time counter by system clock
 *  @arg\b      PCA_DTCK_SYSTEM_DIV_2 : Clock Pre-Scaler of Dead Time counter by system clock with divider 2
 *  @arg\b      PCA_DTCK_SYSTEM_DIV_4 : Clock Pre-Scaler of Dead Time counter by system clock with divider 4
 *  @arg\b      PCA_DTCK_SYSTEM_DIV_8 : Clock Pre-Scaler of Dead Time counter by system clock with divider 8
 * @param[in]   __DTPERIOD__ : specifies Dead-Time period (0~63)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM_DTG_Init(PCA_CK_SYSCLK_DIV_2,1023,PCA_DTCK_SYSTEM_DIV_1,2);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM_DTG_Init(__CKSRC__,__PERIOD__,__DTCKSRC__,__DTPERIOD__)\
      MWT(\
        __DRV_PCA_Disable();\
        CH = 0; CHRL = HIBYTE(65536-(__PERIOD__));\
        CL = 0; CLRL = LOBYTE(65536-(__PERIOD__));\
        __DRV_PCA_ClockSource_Select(__CKSRC__);\
        __DRV_SFR_PageIndex(PDTCRA_Page);\
        PDTCRA = __DTCKSRC__ | (__DTPERIOD__ & 0x3F);\
        __DRV_SFR_PageIndex(0);\
        )


/**
 *******************************************************************************
 * @brief       Starts the PWM with dead time channel 0 and channel 1 PWM output.
 * @details
 * @param[in]  __RES__ : PWMn Resolution Setting
 * @arg\b      PCA_PWM_8BIT : PWMn Resolution the 8bit
 * @arg\b      PCA_PWM_10BIT : PWMn Resolution the 10bit
 * @arg\b      PCA_PWM_12BIT : PWMn Resolution the 12bit
 * @arg\b      PCA_PWM_16BIT : PWMn Resolution the 16bit
 * @param[in]  __DUTY__ : specifies PCA duty cycle(0~65535)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM01_DTG_Start(PCA_PWM_8BIT,254);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM01_DTG_Start(__RES__,__DUTY__)\
      MWT(\
        PCAPWM0 &= ~(P0RS1 | P0RS0);\
        PCAPWM1 &= ~(P0RS1 | P0RS0);\
        PCAPWM0 |= __RES__&PCA_PWM_16BIT;\
        PCAPWM1 |= __RES__&PCA_PWM_16BIT;\
        CMOD |= BME0;\
        CCAPM0 = (ECOM0  | PWM0 | DTE0);\
        CCAPM1 = (ECOM0  | PWM0 );\
        __RES__==PCA_PWM_8BIT?(CCAP0H = CCAP1H = LOBYTE(__DUTY__)):(CCAP0H = (CCAP1H = HIBYTE(__DUTY__)));\
        CCAP0L = CCAP1L = LOBYTE(__DUTY__);\
        __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Starts the PWM with dead time channel 2 and channel 3 PWM output.
 * @details
 * @param[in]  __RES__ : PWMn Resolution Setting
 * @arg\b      PCA_PWM_8BIT : PWMn Resolution the 8bit
 * @arg\b      PCA_PWM_10BIT : PWMn Resolution the 10bit
 * @arg\b      PCA_PWM_12BIT : PWMn Resolution the 12bit
 * @arg\b      PCA_PWM_16BIT : PWMn Resolution the 16bit
 * @param[in]  __DUTY__ : specifies PCA duty cycle(0~65535)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM23_DTG_Start(PCA_PWM_8BIT,254);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM23_DTG_Start(__RES__,__DUTY__)\
      MWT(\
        PCAPWM2 &= ~(P0RS1 | P0RS0);\
        PCAPWM3 &= ~(P0RS1 | P0RS0);\
        PCAPWM2 |= __RES__&PCA_PWM_16BIT;\
        PCAPWM3 |= __RES__&PCA_PWM_16BIT;\
        CMOD |= BME2;\
        CCAPM2 = (ECOM0  | PWM0 | DTE0);\
        CCAPM3 = (ECOM0  | PWM0 );\
        __RES__==PCA_PWM_8BIT?(CCAP2H = CCAP3H = LOBYTE(__DUTY__)):(CCAP2H = CCAP3H = HIBYTE(__DUTY__));\
        CCAP2L = CCAP3L = LOBYTE(__DUTY__);\
        __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Starts the PWM with dead time channel 4 and channel 5 PWM output.
 * @details
 * @param[in]  __RES__ : PWMn Resolution Setting
 * @arg\b      PCA_PWM_8BIT : PWMn Resolution the 8bit
 * @arg\b      PCA_PWM_10BIT : PWMn Resolution the 10bit
 * @arg\b      PCA_PWM_12BIT : PWMn Resolution the 12bit
 * @arg\b      PCA_PWM_16BIT : PWMn Resolution the 16bit
 * @param[in]  __DUTY__ : specifies PCA duty cycle(0~65535)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM45_DTG_Start(PCA_PWM_8BIT,254);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM45_DTG_Start(__RES__,__DUTY__)\
      MWT(\
        PCAPWM4 &= ~(P0RS1 | P0RS0);\
        PCAPWM5 &= ~(P0RS1 | P0RS0);\
        PCAPWM4 |= __RES__&PCA_PWM_16BIT;\
        PCAPWM5 |= __RES__&PCA_PWM_16BIT;\
        CMOD |= BME4;\
        CCAPM4 = (ECOM0  | PWM0 | DTE0);\
        CCAPM5 = (ECOM0  | PWM0 );\
        __RES__==PCA_PWM_8BIT?(CCAP4H = CCAP5H = LOBYTE(__DUTY__)):(CCAP4H = CCAP5H = HIBYTE(__DUTY__));\
        CCAP4L = CCAP5L = LOBYTE(__DUTY__);\
        __DRV_PCA_Enable();\
        )

/**
 *******************************************************************************
 * @brief       Stop the PCA PWM with dead time channel 0 and channel 1 measurement.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM01_DTG_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM01_DTG_Stop()\
      MWT(\
        CCAPM0 &= 0x01;\
        CCAPM1 &= 0x01;\
        )

/**
 *******************************************************************************
 * @brief       Stop the PCA PWM with dead time channel 2 and channel 3 measurement.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM23_DTG_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM23_DTG_Stop()\
      MWT(\
        CCAPM2 &= 0x01;\
        CCAPM3 &= 0x01;\
        )

/**
 *******************************************************************************
 * @brief       Stop the PCA PWM with dead time channel 4 and channel 5 measurement.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM45_DTG_Stop();
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM45_DTG_Stop()\
      MWT(\
        CCAPM4 &= 0x01;\
        CCAPM5 &= 0x01;\
        )

/**
 *******************************************************************************
 * @brief       Update the PCA channel 0 duty.
 * @details
 * @param[in]   __DUTY__ : specifies PCA Channel 0 duty cycle Value
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetPWM0DutyValue(256);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetPWM0DutyValue(__DUTY__)\
      MWT(\
        CCAP0H = HIBYTE(__DUTY__);\
        CCAP0L = LOBYTE(__DUTY__);\
        )

/**
 *******************************************************************************
 * @brief       Update the PCA channel 1 duty.
 * @details
 * @param[in]   __DUTY__ : specifies PCA Channel 1 duty cycle Value
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetPWM1DutyValue(256);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetPWM1DutyValue(__DUTY__)\
      MWT(\
        CCAP1H = HIBYTE(__DUTY__);\
        CCAP1L = LOBYTE(__DUTY__);\
        )

/**
 *******************************************************************************
 * @brief       Update the PCA channel 2 duty.
 * @details
 * @param[in]   __DUTY__ : specifies PCA Channel 2 duty cycle Value
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetPWM2DutyValue(256);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetPWM2DutyValue(__DUTY__)\
      MWT(\
        CCAP2H = HIBYTE(__DUTY__);\
        CCAP2L = LOBYTE(__DUTY__);\
        )

/**
 *******************************************************************************
 * @brief       Update the PCA channel 3 duty.
 * @details
 * @param[in]   __DUTY__ : specifies PCA Channel 3 duty cycle Value
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetPWM3DutyValue(256);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetPWM3DutyValue(__DUTY__)\
      MWT(\
        CCAP3H = HIBYTE(__DUTY__);\
        CCAP3L = LOBYTE(__DUTY__);\
        )

/**
 *******************************************************************************
 * @brief       Update the PCA channel 4 duty.
 * @details
 * @param[in]   __DUTY__ : specifies PCA Channel 4 duty cycle Value
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetPWM4DutyValue(256);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetPWM4DutyValue(__DUTY__)\
      MWT(\
        CCAP4H = HIBYTE(__DUTY__);\
        CCAP4L = LOBYTE(__DUTY__);\
        )

/**
 *******************************************************************************
 * @brief       Update the PCA channel 5 duty.
 * @details
 * @param[in]   __DUTY__ : specifies PCA Channel 5 duty cycle Value
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetPWM5DutyValue(256);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetPWM5DutyValue(__DUTY__)\
      MWT(\
        CCAP5H = HIBYTE(__DUTY__);\
        CCAP5L = LOBYTE(__DUTY__);\
        )

/**
 *******************************************************************************
 * @brief       Update the PCA channel 6 duty.
 * @details
 * @param[in]   __DUTY__ : specifies PCA Channel 6 duty cycle Value
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetPWM6DutyValue(256);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetPWM6DutyValue(__DUTY__)\
      MWT(\
        __DRV_SFR_PageIndex(CCAP6H_Page);\
        CCAP6H = HIBYTE(__DUTY__);\
        CCAP6L = LOBYTE(__DUTY__);\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief       Update the PCA channel 7 duty.
 * @details
 * @param[in]   __DUTY__ : specifies PCA Channel 7 duty cycle Value
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetPWM7DutyValue(256);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetPWM7DutyValue(__DUTY__)\
      MWT(\
        __DRV_SFR_PageIndex(CCAP7H_Page);\
        CCAP7H = HIBYTE(__DUTY__);\
        CCAP7L = LOBYTE(__DUTY__);\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief       Configure the PCA Dead Time period
 * @details
 * @param[in]   __DTCKSRC__ : specifies Dead Time counter
 *  @arg\b      PCA_DTCK_SYSTEM_DIV_1 : Clock Pre-Scaler of Dead Time counter by system clock
 *  @arg\b      PCA_DTCK_SYSTEM_DIV_2 : Clock Pre-Scaler of Dead Time counter by system clock with divider 2
 *  @arg\b      PCA_DTCK_SYSTEM_DIV_4 : Clock Pre-Scaler of Dead Time counter by system clock with divider 4
 *  @arg\b      PCA_DTCK_SYSTEM_DIV_8 : Clock Pre-Scaler of Dead Time counter by system clock with divider 8
 * @param[in]   __DTPERIOD__ : specifies Dead-Time period (0~63)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM_DTG_period(PCA_DTCK_SYSTEM_DIV_1,2);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM_DTG_period(__DTCKSRC__,__DTPERIOD__)\
      MWT(\
        __DRV_SFR_PageIndex(PDTCRA_Page);\
        PDTCRA = __DTCKSRC__ | (__DTPERIOD__ & 0x3F);\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief       Configure the PCA CEX1/3/5 Pin
 * @details
 * @param[in]   __STATE__ : specifies CEX1/3/5 Port pin
 *  @arg\b      CEX1_P23_CEX3_P25_CEX5_P27 : CEX1=P23,CEX3=P25,CEX5=P27
 *  @arg\b      CEX1_P33_CEX3_P34_CEX5_P35 : CEX1=P33,CEX3=P34,CEX5=P35
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_CEX135Mux_Select(CEX1_P33_CEX3_P34_CEX5_P35);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_CEX135Mux_Select(__STATE__)\
      MWT(\
        __DRV_SFR_PageIndex(AUXR5_Page);\
        __STATE__==CEX1_P33_CEX3_P34_CEX5_P35?(AUXR5|=(C0PS1)):(AUXR5&=~(C0PS1));\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief       Configure the PCA CEX0/2/4 Pin
 * @details
 * @param[in]   __STATE__ : specifies CEX0/2/4 Port pin
 *  @arg\b      CEX0_P22_CEX2_P24_CEX4_P26 : CEX0=P22,CEX2=P24,CEX4=P26
 *  @arg\b      CEX0_P40_CEX2_P41_CEX4_P32 : CEX0=P40,CEX2=P41,CEX4=P32
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_CEX024Mux_Select(CEX0_P40_CEX2_P41_CEX4_P32);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_CEX024Mux_Select(__STATE__)\
      MWT(\
        __DRV_SFR_PageIndex(AUXR5_Page);\
        __STATE__==CEX0_P40_CEX2_P41_CEX4_P32?(AUXR5|=(C0PS0)):(AUXR5&=~(C0PS0));\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief       Configure the PCA PWM0A and PWM0B Pin
 * @details
 * @param[in]   __STATE__ : specifies PWM0x Port pin
 *  @arg\b      PWM0A_P36_PWM0B_P37 : PWM0A=P36,PWM0B=P37
 *  @arg\b      PWM0A_P60_PWM0B_P61 : PWM0A=P60,PWM0B=P61
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM0ABMux_Select(PWM0A_P60_PWM0B_P61);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM0ABMux_Select(__STATE__)\
      MWT(\
        __DRV_SFR_PageIndex(AUXR5_Page);\
        __STATE__==PWM0A_P60_PWM0B_P61?(AUXR5|=(C0PPS0)):(AUXR5&=~(C0PPS0));\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief       Configure the PCA PWM2A and PWM2B Pin
 * @details
 * @param[in]   __STATE__ : specifies PWM2x Port pin
 *  @arg\b      PWM2A_P40_PWM2B_P41 : PWM2A=P40,PWM2B=P41
 *  @arg\b      PWM2A_P34_PWM2B_P35 : PWM2A=P34,PWM2B=P35
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM2ABMux_Select(PWM2A_P34_PWM2B_P35);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM2ABMux_Select(__STATE__)\
      MWT(\
        __DRV_SFR_PageIndex(AUXR5_Page);\
        __STATE__==PWM2A_P34_PWM2B_P35?(AUXR5|=(C0PPS1)):(AUXR5&=~(C0PPS1));\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief       Configure the PCA Channel 2 input port Pin
 * @details
 * @param[in]   __STATE__ : specifies CEX2 capture input Port pin
 *  @arg\b      FROM_CEX2 : Input by CEX2
 *  @arg\b      FROM_T3EXI : From Timer 3 module
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_CEX2InputMux_Select(FROM_T3EXI);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_CEX2InputMux_Select(__STATE__)\
      MWT(\
        __DRV_SFR_PageIndex(AUXR5_Page);\
        __STATE__==FROM_T2EXI?(AUXR5|=(C0IC2S0)):(AUXR5&=~(C0IC2S0));\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief       Configure the PCA Channel 4 input port Pin
 * @details
 * @param[in]   __STATE__ : specifies CEX4 capture input Port pin
 *  @arg\b      FROM_CEX4 : Input by CEX4
 *  @arg\b      FROM_T2EXI : From Timer 2 module
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_CEX4InputMux_Select(FROM_T2EXI);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_CEX4InputMux_Select(__STATE__)\
      MWT(\
        __DRV_SFR_PageIndex(AUXR5_Page);\
        __STATE__==FROM_T2EXI?(AUXR5|=(C0IC4S0)):(AUXR5&=~(C0IC4S0));\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief       Configure the PCA PWM6 and PWM7 Pin
 * @details
 * @param[in]   __STATE__ : specifies PWM6 and PWM7 Port pin
 *  @arg\b      PWM6_P62_PWM7_P63 : PWM6=P62,PWM7=P63
 *  @arg\b      PWM6_P60_PWM7_P61 : PWM6=P60,PWM7=P61
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM67Mux_Select(PWM6_P60_PWM7_P61);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM67Mux_Select(__STATE__)\
      MWT(\
        __DRV_SFR_PageIndex(AUXR8_Page);\
        __STATE__==PWM6_P60_PWM7_P61?(AUXR8|=(C0PPS2)):(AUXR8&=~(C0PPS2));\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief       Configure the PCA ECI Pin
 * @details
 * @param[in]   __STATE__ : specifies ECI Port pin
 *  @arg\b      ECI_P21 : ECI=P21
 *  @arg\b      ECI_P46 : ECI=P46
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_ECIMux_Select(ECI_P46);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_ECIMux_Select(__STATE__)\
      MWT(\
        __DRV_SFR_PageIndex(AUXR5_Page);\
        __STATE__==ECI_P46?(AUXR5|=(ECIPS0)):(AUXR5&=~(ECIPS0));\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief       Configure the PCA Clock Output port pin Selection.
 * @details
 * @param[in]   __STATE__ : specifies C0CKO Port pin
 *  @arg\b      C0CKO_P47 : C0CKO=P47
 *  @arg\b      C0CKO_P72 : C0CKO=P72
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_C0CKOMux_Select(C0CKO_P72);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_C0CKOMux_Select(__STATE__)\
      MWT(\
        __DRV_SFR_PageIndex(AUXR5_Page);\
        __STATE__==C0CKO_P72?(AUXR5|=(C0COPS)):(AUXR5&=~(C0COPS));\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief       Configure PWM Break Mode
 * @details
 * @param[in]   __STATE__ : PWM break mode is latched or cycle-by-cycle.
 *  @arg\b      LATCH : Latched Mode.
 *  @arg\b      CYCLE : Cycle-by-cycle Mode
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM_BreakMode_Select(CYCLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM_BreakMode_Select(__STATE__)\
        __STATE__==CYCLE?(PWMCR|=(PBKM)):(PWMCR&=~(PBKM))

/**
 *******************************************************************************
 * @brief       Configure Extend Dead-Time in PWM Period.
 * @details
 * @param[in]   __STATE__ : Enabled Extend Dead-Time
 *  @arg\b      MW_ENABLE : Enable M + 2P on PWM channel.
 *  @arg\b      MW_DISABLE : Disable M + 2P
 * @return      None
 * @note        Dead time affects PWM frequency
 * @par         Example
 * @code
 *    __DRV_PCA_PWM_ExDTG_period(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM_ExDTG_period(__STATE__)\
        __STATE__==MW_ENABLE?(PWMCR|=(EXDT)):(PWMCR&=~(EXDT))

/**
 *******************************************************************************
 * @brief	  Enable/Disable PCA clock output
 * @details   Enable PCA0 clock output with PCA0 base timer overflow rate/2.
 * @param[in] __STATE__ : Configure PCA clock output
 *      	  This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par       Example
 * @code
 *    __DRV_PCA_SetClockOutput(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetClockOutput(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR7_Page);\
        __STATE__==MW_ENABLE?(AUXR7|=(C0CKOE)):(AUXR7&=~(C0CKOE));\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief       Configure PWM Break Source 0
 * @details
 * @param[in]   __STATE__ : PWM break Source0 select.
 *  @arg\b      BREAK0_DISABLE : Disable break Source0
 *  @arg\b      BREAK0_AC0OUT : AC0 Output
 *  @arg\b      BREAK0_INT1ET : nINT1 active
 *  @arg\b      BREAK0_T2EXI : T2EXI(from Timer2)
 *  @arg\b      BREAK0_KBIET :  KBI match active
 *  @arg\b      BREAK0_AC1OUT : AC1 Output
 *  @arg\b      BREAK0_INT3ET : nINT3 active
 *  @arg\b      BREAK0_ADCWI : ADC window detect flag
 * @return      None
 * @note        This function is only active on CEXn output mode (n=0~5).
 * @par         Example
 * @code
 *    __DRV_PCA_PWM_BreakSource0_Select(BREAK0_AC0OUT);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM_BreakSource0_Select(__STATE__)\
    MWT(\
        PWMCR&= ~(PBKE02|PBKE01|PBKE00);\
        PWMCR|=__STATE__&(PBKE02|PBKE01|PBKE00);\
       )

/**
 *******************************************************************************
 * @brief       Configure PWM Break Source 1
 * @details
 * @param[in]   __STATE__ : PWM break Source1 select.
 *  @arg\b      BREAK1_DISABLE : Disable break Source1
 *  @arg\b      BREAK1_INT2ET : nINT2 active
 *  @arg\b      BREAK1_T3EXI : T3EXI(from Timer3)
 *  @arg\b      BREAK1_KBIET : KBI match active
 * @return      None
 * @note        This function is only active on CEXn output mode (n=0~5).
 * @par         Example
 * @code
 *    __DRV_PCA_PWM_BreakSource1_Select(Break1_INT2ET);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM_BreakSource1_Select(__STATE__)\
    MWT(\
        PWMCR&= ~(PBKE11|PBKE10);\
        PWMCR|=__STATE__&(PBKE11|PBKE10);\
       )

/**
 *******************************************************************************
 * @brief       Configure PCA FIFO data mode trigger source
 * @details
 * @param[in]   __STATE__: FIFO data mode trigger source
 *  @arg\b      FIFO_T0OF : The FIFO mode trigger from Timer0 overflow.
 *  @arg\b      FIFO_T1OF : The FIFO mode trigger from Timer1 overflow.
 *  @arg\b      FIFO_T3OF : The FIFO mode trigger from Timer3 overflow.
 *  @arg\b      FIFO_S0TOF : The FIFO mode trigger from S0BRG overflow.
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_FIFOTriggerSource_Select(FIFO_T0OF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_FIFOTriggerSource_Select(__STATE__)\
      MWT(\
          __DRV_SFR_PageIndex(AUXR9_Page);\
          AUXR9&=~(C0FDC1|C0FDC0);\
          AUXR9|=__STATE__&(C0FDC1|C0FDC0);\
          __DRV_SFR_PageIndex(0);\
          )

/**
 *******************************************************************************
 * @brief       Set PWM 0 extened 9th bit(MSB bit), associated with CCAPnL to become a 9-bit resgister used in PWM mode.(CCAPnL)
 * @param[in]   __VALUE__: PWM 9th bit value is 0 or 1
 *  @arg\b      Value : 0~1
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetPWM0DutyValue9thLow(0);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetPWM0DutyValue9thLow(__VALUE__)\
        __VALUE__==0?(PCAPWM0&=~(ECAP0L)):(PCAPWM0|=(ECAP0L));\

/**
 *******************************************************************************
 * @brief       Set PWM 1 extened 9th bit(MSB bit), associated with CCAPnL to become a 9-bit resgister used in PWM mode.(CCAPnL)
 * @param[in]   __VALUE__: PWM 9th bit value is 0 or 1
 *  @arg\b      Value : 0~1
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetPWM1DutyValue9thLow(0);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetPWM1DutyValue9thLow(__VALUE__)\
        __VALUE__==0?(PCAPWM1&=~(ECAP1L)):(PCAPWM1|=(ECAP1L));\

/**
 *******************************************************************************
 * @brief       Set PWM 2 extened 9th bit(MSB bit), associated with CCAPnL to become a 9-bit resgister used in PWM mode.(CCAPnL)
 * @param[in]   __VALUE__: PWM 9th bit value is 0 or 1
 *  @arg\b      Value : 0~1
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetPWM2DutyValue9thLow(0);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetPWM2DutyValue9thLow(__VALUE__)\
        __VALUE__==0?(PCAPWM2&=~(ECAP2L)):(PCAPWM2|=(ECAP2L));\

/**
 *******************************************************************************
 * @brief       Set PWM 3 extened 9th bit(MSB bit), associated with CCAPnL to become a 9-bit resgister used in PWM mode.(CCAPnL)
 * @param[in]   __VALUE__: PWM 9th bit value is 0 or 1
 *  @arg\b      Value : 0~1
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetPWM3DutyValue9thLow(0);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetPWM3DutyValue9thLow(__VALUE__)\
        __VALUE__==0?(PCAPWM3&=~(ECAP3L)):(PCAPWM3|=(ECAP3L));\

/**
 *******************************************************************************
 * @brief       Set PWM 4 extened 9th bit(MSB bit), associated with CCAPnL to become a 9-bit resgister used in PWM mode.(CCAPnL)
 * @param[in]   __VALUE__: PWM 9th bit value is 0 or 1
 *  @arg\b      Value : 0~1
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetPWM4DutyValue9thLow(0);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetPWM4DutyValue9thLow(__VALUE__)\
        __VALUE__==0?(PCAPWM4&=~(ECAP4L)):(PCAPWM4|=(ECAP4L));\

/**
 *******************************************************************************
 * @brief       Set PWM 5 extened 9th bit(MSB bit), associated with CCAPnL to become a 9-bit resgister used in PWM mode.(CCAPnL)
 * @param[in]   __VALUE__: PWM 9th bit value is 0 or 1
 *  @arg\b      Value : 0~1
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetPWM5DutyValue9thLow(0);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetPWM5DutyValue9thLow(__VALUE__)\
        __VALUE__==0?(PCAPWM5&=~(ECAP5L)):(PCAPWM5|=(ECAP5L));\

/**
 *******************************************************************************
 * @brief       Set PWM 6 extened 9th bit(MSB bit), associated with CCAPnL to become a 9-bit resgister used in PWM mode.(CCAPnL)
 * @param[in]   __VALUE__: PWM 9th bit value is 0 or 1
 *  @arg\b      Value : 0~1
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetPWM6DutyValue9thLow(0);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetPWM6DutyValue9thLow(__VALUE__)\
      MWT(\
          __DRV_SFR_PageIndex(PCAPWM6_Page);\
          __VALUE__==0?(PCAPWM6&=~(ECAP6L)):(PCAPWM6|=(ECAP6L));\
          __DRV_SFR_PageIndex(0);\
         )

/**
 *******************************************************************************
 * @brief       Set PWM 7 extened 9th bit(MSB bit), associated with CCAPnL to become a 9-bit resgister used in PWM mode.(CCAPnL)
 * @param[in]   __VALUE__: PWM 9th bit value is 0 or 1
 *  @arg\b      Value : 0~1
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetPWM7DutyValue9thLow(0);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetPWM7DutyValue9thLow(__VALUE__)\
      MWT(\
          __DRV_SFR_PageIndex(PCAPWM7_Page);\
          __VALUE__==0?(PCAPWM7&=~(ECAP7L)):(PCAPWM7|=(ECAP7L));\
          __DRV_SFR_PageIndex(0);\
         )
/**
 *******************************************************************************
 * @brief       Set PWM 0 extened 9th bit(MSB bit), associated with CCAPnH to become a 9-bit resgister used in PWM mode.(CCAPnH)
 * @param[in]   __VALUE__: PWM 9th bit value is 0 or 1
 *  @arg\b      Value : 0~1
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetPWM0DutyValue9thHigh(0);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetPWM0DutyValue9thHigh(__VALUE__)\
        __VALUE__==0?(PCAPWM0&=~(ECAP0H)):(PCAPWM0|=(ECAP0H));\

/**
 *******************************************************************************
 * @brief       Set PWM 1 extened 9th bit(MSB bit), associated with CCAPnH to become a 9-bit resgister used in PWM mode.(CCAPnH)
 * @param[in]   __VALUE__: PWM 9th bit value is 0 or 1
 *  @arg\b      Value : 0~1
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetPWM1DutyValue9thHigh(0);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetPWM1DutyValue9thHigh(__VALUE__)\
        __VALUE__==0?(PCAPWM1&=~(ECAP1H)):(PCAPWM1|=(ECAP1H));\

/**
 *******************************************************************************
 * @brief       Set PWM 2 extened 9th bit(MSB bit), associated with CCAPnH to become a 9-bit resgister used in PWM mode.(CCAPnH)
 * @param[in]   __VALUE__: PWM 9th bit value is 0 or 1
 *  @arg\b      Value : 0~1
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetPWM2DutyValue9thHigh(0);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetPWM2DutyValue9thHigh(__VALUE__)\
        __VALUE__==0?(PCAPWM2&=~(ECAP2H)):(PCAPWM2|=(ECAP2H));\

/**
 *******************************************************************************
 * @brief       Set PWM 3 extened 9th bit(MSB bit), associated with CCAPnH to become a 9-bit resgister used in PWM mode.(CCAPnH)
 * @param[in]   __VALUE__: PWM 9th bit value is 0 or 1
 *  @arg\b      Value : 0~1
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetPWM3DutyValue9thHigh(0);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetPWM3DutyValue9thHigh(__VALUE__)\
        __VALUE__==0?(PCAPWM3&=~(ECAP3H)):(PCAPWM3|=(ECAP3H));\

/**
 *******************************************************************************
 * @brief       Set PWM 4 extened 9th bit(MSB bit), associated with CCAPnH to become a 9-bit resgister used in PWM mode.(CCAPnH)
 * @param[in]   __VALUE__: PWM 9th bit value is 0 or 1
 *  @arg\b      Value : 0~1
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetPWM4DutyValue9thHigh(0);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetPWM4DutyValue9thHigh(__VALUE__)\
        __VALUE__==0?(PCAPWM4&=~(ECAP4H)):(PCAPWM4|=(ECAP4H));\

/**
 *******************************************************************************
 * @brief       Set PWM 5 extened 9th bit(MSB bit), associated with CCAPnH to become a 9-bit resgister used in PWM mode.(CCAPnH)
 * @param[in]   __VALUE__: PWM 9th bit value is 0 or 1
 *  @arg\b      Value : 0~1
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetPWM5DutyValue9thHigh(0);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetPWM5DutyValue9thHigh(__VALUE__)\
        __VALUE__==0?(PCAPWM5&=~(ECAP5H)):(PCAPWM5|=(ECAP5H));\

/**
 *******************************************************************************
 * @brief       Set PWM 6 extened 9th bit(MSB bit), associated with CCAPnH to become a 9-bit resgister used in PWM mode.(CCAPnH)
 * @param[in]   __VALUE__: PWM 9th bit value is 0 or 1
 *  @arg\b      Value : 0~1
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetPWM6DutyValue9thHigh(0);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetPWM6DutyValue9thHigh(__VALUE__)\
      MWT(\
          __DRV_SFR_PageIndex(PCAPWM6_Page);\
          __VALUE__==0?(PCAPWM6&=~(ECAP6H)):(PCAPWM6|=(ECAP6H));\
          __DRV_SFR_PageIndex(0);\
         )

/**
 *******************************************************************************
 * @brief       Set PWM 7 extened 9th bit(MSB bit), associated with CCAPnH to become a 9-bit resgister used in PWM mode.(CCAPnH)
 * @param[in]   __VALUE__: PWM 9th bit value is 0 or 1
 *  @arg\b      Value : 0~1
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_SetPWM7DutyValue9thHigh(0);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_SetPWM7DutyValue9thHigh(__VALUE__)\
      MWT(\
          __DRV_SFR_PageIndex(PCAPWM7_Page);\
          __VALUE__==0?(PCAPWM7&=~(ECAP7H)):(PCAPWM7|=(ECAP7H));\
          __DRV_SFR_PageIndex(0);\
         )

/**
 *******************************************************************************
 * @brief       Set PWM0 resolution
 * @param[in]   __VALUE__: PWM0 resolution
 *  @arg\b      PCA_PWM_8BIT : 8 bit PWMn, the overflow is active when [CH, CL] counts XXXX-XXXX-1111-1111 -> XXXX-XXXX-0000-0000.
 *  @arg\b      PCA_PWM_10BIT : 10 bit PWMn, the overflow is active when [CH, CL] counts XXXX-XX11-1111-1111 -> XXXX-XX00-0000-0000.
 *  @arg\b      PCA_PWM_12BIT : 12 bit PWMn, the overflow is active when [CH, CL] counts XXXX-1111-1111-1111 -> XXXX-0000-0000-0000.
 *  @arg\b      PCA_PWM_16BIT : 16 bit PWMn, the overflow is active when [CH, CL] counts 1111-1111-1111-1111 -> 0000-0000-0000-0000.
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM0Resolution_Select(PCA_PWM_8BIT);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM0Resolution_Select(__VALUE__)\
      MWT(\
          PCAPWM0&=~(P0RS1|P0RS0);\
          PCAPWM0|=__VALUE__&(P0RS1|P0RS0);\
         )

/**
 *******************************************************************************
 * @brief       Set PWM1 resolution
 * @param[in]   __VALUE__: PWM1 resolution
 *  @arg\b      PCA_PWM_8BIT : 8 bit PWMn, the overflow is active when [CH, CL] counts XXXX-XXXX-1111-1111 -> XXXX-XXXX-0000-0000.
 *  @arg\b      PCA_PWM_10BIT : 10 bit PWMn, the overflow is active when [CH, CL] counts XXXX-XX11-1111-1111 -> XXXX-XX00-0000-0000.
 *  @arg\b      PCA_PWM_12BIT : 12 bit PWMn, the overflow is active when [CH, CL] counts XXXX-1111-1111-1111 -> XXXX-0000-0000-0000.
 *  @arg\b      PCA_PWM_16BIT : 16 bit PWMn, the overflow is active when [CH, CL] counts 1111-1111-1111-1111 -> 0000-0000-0000-0000.
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM1Resolution_Select(PCA_PWM_8BIT);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM1Resolution_Select(__VALUE__)\
      MWT(\
          PCAPWM1&=~(P1RS1|P1RS0);\
          PCAPWM1|=__VALUE__&(P1RS1|P1RS0);\
         )

/**
 *******************************************************************************
 * @brief       Set PWM2 resolution
 * @param[in]   __VALUE__: PWM2 resolution
 *  @arg\b      PCA_PWM_8BIT : 8 bit PWMn, the overflow is active when [CH, CL] counts XXXX-XXXX-1111-1111 -> XXXX-XXXX-0000-0000.
 *  @arg\b      PCA_PWM_10BIT : 10 bit PWMn, the overflow is active when [CH, CL] counts XXXX-XX11-1111-1111 -> XXXX-XX00-0000-0000.
 *  @arg\b      PCA_PWM_12BIT : 12 bit PWMn, the overflow is active when [CH, CL] counts XXXX-1111-1111-1111 -> XXXX-0000-0000-0000.
 *  @arg\b      PCA_PWM_16BIT : 16 bit PWMn, the overflow is active when [CH, CL] counts 1111-1111-1111-1111 -> 0000-0000-0000-0000.
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM2Resolution_Select(PCA_PWM_8BIT);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM2Resolution_Select(__VALUE__)\
      MWT(\
          PCAPWM2&=~(P2RS1|P2RS0);\
          PCAPWM2|=__VALUE__&(P2RS1|P2RS0);\
         )

/**
 *******************************************************************************
 * @brief       Set PWM3 resolution
 * @param[in]   __VALUE__: PWM3 resolution
 *  @arg\b      PCA_PWM_8BIT : 8 bit PWMn, the overflow is active when [CH, CL] counts XXXX-XXXX-1111-1111 -> XXXX-XXXX-0000-0000.
 *  @arg\b      PCA_PWM_10BIT : 10 bit PWMn, the overflow is active when [CH, CL] counts XXXX-XX11-1111-1111 -> XXXX-XX00-0000-0000.
 *  @arg\b      PCA_PWM_12BIT : 12 bit PWMn, the overflow is active when [CH, CL] counts XXXX-1111-1111-1111 -> XXXX-0000-0000-0000.
 *  @arg\b      PCA_PWM_16BIT : 16 bit PWMn, the overflow is active when [CH, CL] counts 1111-1111-1111-1111 -> 0000-0000-0000-0000.
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM3Resolution_Select(PCA_PWM_8BIT);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM3Resolution_Select(__VALUE__)\
      MWT(\
          PCAPWM3&=~(P3RS1|P3RS0);\
          PCAPWM3|=__VALUE__&(P3RS1|P3RS0);\
         )

/**
 *******************************************************************************
 * @brief       Set PWM4 resolution
 * @param[in]   __VALUE__: PWM4 resolution
 *  @arg\b      PCA_PWM_8BIT : 8 bit PWMn, the overflow is active when [CH, CL] counts XXXX-XXXX-1111-1111 -> XXXX-XXXX-0000-0000.
 *  @arg\b      PCA_PWM_10BIT : 10 bit PWMn, the overflow is active when [CH, CL] counts XXXX-XX11-1111-1111 -> XXXX-XX00-0000-0000.
 *  @arg\b      PCA_PWM_12BIT : 12 bit PWMn, the overflow is active when [CH, CL] counts XXXX-1111-1111-1111 -> XXXX-0000-0000-0000.
 *  @arg\b      PCA_PWM_16BIT : 16 bit PWMn, the overflow is active when [CH, CL] counts 1111-1111-1111-1111 -> 0000-0000-0000-0000.
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM4Resolution_Select(PCA_PWM_8BIT);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM4Resolution_Select(__VALUE__)\
      MWT(\
          PCAPWM4&=~(P4RS1|P4RS0);\
          PCAPWM4|=__VALUE__&(P4RS1|P4RS0);\
         )

/**
 *******************************************************************************
 * @brief       Set PWM5 resolution
 * @param[in]   __VALUE__: PWM5 resolution
 *  @arg\b      PCA_PWM_8BIT : 8 bit PWMn, the overflow is active when [CH, CL] counts XXXX-XXXX-1111-1111 -> XXXX-XXXX-0000-0000.
 *  @arg\b      PCA_PWM_10BIT : 10 bit PWMn, the overflow is active when [CH, CL] counts XXXX-XX11-1111-1111 -> XXXX-XX00-0000-0000.
 *  @arg\b      PCA_PWM_12BIT : 12 bit PWMn, the overflow is active when [CH, CL] counts XXXX-1111-1111-1111 -> XXXX-0000-0000-0000.
 *  @arg\b      PCA_PWM_16BIT : 16 bit PWMn, the overflow is active when [CH, CL] counts 1111-1111-1111-1111 -> 0000-0000-0000-0000.
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM5Resolution_Select(PCA_PWM_8BIT);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM5Resolution_Select(__VALUE__)\
      MWT(\
          PCAPWM5&=~(P5RS1|P5RS0);\
          PCAPWM5|=__VALUE__&(P5RS1|P5RS0);\
         )

/**
 *******************************************************************************
 * @brief       Set PWM6 resolution
 * @param[in]   __VALUE__: PWM6 resolution
 *  @arg\b      PCA_PWM_8BIT : 8 bit PWMn, the overflow is active when [CH, CL] counts XXXX-XXXX-1111-1111 -> XXXX-XXXX-0000-0000.
 *  @arg\b      PCA_PWM_10BIT : 10 bit PWMn, the overflow is active when [CH, CL] counts XXXX-XX11-1111-1111 -> XXXX-XX00-0000-0000.
 *  @arg\b      PCA_PWM_12BIT : 12 bit PWMn, the overflow is active when [CH, CL] counts XXXX-1111-1111-1111 -> XXXX-0000-0000-0000.
 *  @arg\b      PCA_PWM_16BIT : 16 bit PWMn, the overflow is active when [CH, CL] counts 1111-1111-1111-1111 -> 0000-0000-0000-0000.
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM6Resolution_Select(PCA_PWM_8BIT);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM6Resolution_Select(__VALUE__)\
      MWT(\
          __DRV_SFR_PageIndex(PCAPWM6_Page);\
          PCAPWM6&=~(P6RS1|P6RS0);\
          PCAPWM6|=__VALUE__&(P6RS1|P6RS0);\
          __DRV_SFR_PageIndex(0);\
         )

/**
 *******************************************************************************
 * @brief       Set PWM7 resolution
 * @param[in]   __VALUE__: PWM7 resolution
 *  @arg\b      PCA_PWM_8BIT : 8 bit PWMn, the overflow is active when [CH, CL] counts XXXX-XXXX-1111-1111 -> XXXX-XXXX-0000-0000.
 *  @arg\b      PCA_PWM_10BIT : 10 bit PWMn, the overflow is active when [CH, CL] counts XXXX-XX11-1111-1111 -> XXXX-XX00-0000-0000.
 *  @arg\b      PCA_PWM_12BIT : 12 bit PWMn, the overflow is active when [CH, CL] counts XXXX-1111-1111-1111 -> XXXX-0000-0000-0000.
 *  @arg\b      PCA_PWM_16BIT : 16 bit PWMn, the overflow is active when [CH, CL] counts 1111-1111-1111-1111 -> 0000-0000-0000-0000.
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_PWM7Resolution_Select(PCA_PWM_8BIT);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_PWM7Resolution_Select(__VALUE__)\
      MWT(\
          __DRV_SFR_PageIndex(PCAPWM7_Page);\
          PCAPWM7&=~(P7RS1|P7RS0);\
          PCAPWM7|=__VALUE__&(P7RS1|P7RS0);\
          __DRV_SFR_PageIndex(0);\
         )

/**
 *******************************************************************************
 * @brief       Configure POEn control Mode
 * @details
 * @param[in]   __STATE__ : POEn control is CPU writing or PWM cycle.
 *  @arg\b      CPU_WRITING : POEn function is active immediately after CPU writing.
 *  @arg\b      PWM_CYCLE : POEn function is aligned to PWM cycle.
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_PCA_POEnControl_Select(PWM_CYCLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_PCA_POEnControl_Select(__STATE__)\
      MWT(\
          __DRV_SFR_PageIndex(AUXR11_Page);\
          __STATE__==PWM_CYCLE?(AUXR11|=(POEM0)):(AUXR11&=~(POEM0));\
          __DRV_SFR_PageIndex(0);\
         )


bool DRV_PCA_GetModule0Flag(void);
bool DRV_PCA_GetModule1Flag(void);
bool DRV_PCA_GetModule2Flag(void);
bool DRV_PCA_GetModule3Flag(void);
bool DRV_PCA_GetModule4Flag(void);
bool DRV_PCA_GetModule5Flag(void);
bool DRV_PCA_GetModule6Flag(void);
bool DRV_PCA_GetModule7Flag(void);
bool DRV_PCA_GetOverflowFlag(void);
bool DRV_PCA_GetBreakFlag(void);
uint16_t DRV_PCA_GetPWM0DutyValue(void);
uint16_t DRV_PCA_GetPWM1DutyValue(void);
uint16_t DRV_PCA_GetPWM2DutyValue(void);
uint16_t DRV_PCA_GetPWM3DutyValue(void);
uint16_t DRV_PCA_GetPWM4DutyValue(void);
uint16_t DRV_PCA_GetPWM5DutyValue(void);
uint16_t DRV_PCA_GetPWM6DutyValue(void);
uint16_t DRV_PCA_GetPWM7DutyValue(void);
#endif
