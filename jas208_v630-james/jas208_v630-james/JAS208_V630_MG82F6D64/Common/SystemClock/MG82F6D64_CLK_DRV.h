/**
 ******************************************************************************
 *
 * @file        MG82F6D64_CLK_DRV.H
 *
 * @brief       This is the C code format driver head file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     V1.10
 * @date        2022/6/10
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
 * @par         Figure
 * @image       html DRV_CLK_Figure.png width=1024
 ******************************************************************************
 ******************************************************************************
 */

#ifndef _MG82F6D64_CLK_DRV_H
#define _MG82F6D64_CLK_DRV_H


/**
 *******************************************************************************
 * @brief       Select IHRCO 12MHz
 * @details
 * @return      None
 * @par         Figure
 * @image       html DRV_CLK_IHRCO_12MHz.png
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_IHRCO_12MHz();
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_IHRCO_12MHz()\
    MWT(\
        DRV_PageP_Read(CKCON2_P);\
        IFD |= IHRCOE_P;\
        DRV_PageP_Write(CKCON2_P, IFD);\
        CKCON0 &= ~AFS;\
    )


/**
 *******************************************************************************
 * @brief       Select IHRCO 11.059MHz
 * @details
 * @return      None
 * @par         Figure
 * @image       html DRV_CLK_IHRCO_11MHz.png
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_IHRCO_11M059KHz();
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_IHRCO_11M059KHz()\
    MWT(\
        DRV_PageP_Read(CKCON2_P);\
        IFD |= IHRCOE_P;\
        DRV_PageP_Write(CKCON2_P, IFD);\
        CKCON0 |= AFS;\
    )


/**
 *******************************************************************************
 * @brief       Disable IHRCO
 * @details
 * @return      None
 * @par         Figure
 * @image       html DRV_CLK_IHRCO_Disable.png
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_IHRCO_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_IHRCO_Disable()\
    MWT(\
        DRV_PageP_Read(CKCON2_P);\
        IFD &= ~IHRCOE_P;\
        DRV_PageP_Write(CKCON2_P, IFD);\
    )


/**
 *******************************************************************************
 * @brief       Select IHRCO 12MHz / 11.059MHz / Disable
 * @details
 * @param[in]   __SELECT__ : Select IHRCO 12MHz / 11.059MHz / Disable
 *  @arg\b      IHRCO_DISABLE : Disable internal high frequency.
 *  @arg\b      IHRCO_12MHz : Internal 12MHz input. (Default)
 *  @arg\b      IHRCO_11M059KHz : Internal 11.0592MHz input.
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_IHRCO_Select(IHRCO_12MHz);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_CLK_IHRCO_Select
#define IHRCO_DISABLE    0   // Disable IHECO
#define IHRCO_12MHz      1   // Select IHECO clock select 12MHz
#define IHRCO_11M059KHz  2   // Select IHECO clock select 11.0952MHz
/// @endcond

#define __DRV_CLK_IHRCO_Select(__SELECT__)\
    MWT(\
        DRV_PageP_Read(CKCON2_P);\
        __SELECT__ == IHRCO_DISABLE ? (IFD &= ~IHRCOE_P) : (IFD |= IHRCOE_P);\
        DRV_PageP_Write(CKCON2_P, IFD);\
        __SELECT__ == IHRCO_11M059KHz ? (CKCON0 |= AFS) : (CKCON0 &= ~AFS);\
    )


/**
 *******************************************************************************
 * @brief       External crystal enable. (XTAL)
 * @details
 * @return      None
 * @note        Frequency range : 0~25MHz
 * @par         Example
 * @code
 *    __DRV_CLK_XTAL_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_XTAL_Enable()\
    MWT(\
        DRV_PageP_Read(CKCON2_P);\
        IFD |= XTALE_P;\
        DRV_PageP_Write(CKCON2_P, IFD);\
    )


/**
 *******************************************************************************
 * @brief       External crystal disable. (XTAL)
 * @details
 * @return      None
 * @note        Frequency range : 0~25MHz
 * @par         Example
 * @code
 *    __DRV_CLK_XTAL_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_XTAL_Disable()\
    MWT(\
        DRV_PageP_Read(CKCON2_P);\
        IFD &= ~XTALE_P;\
        DRV_PageP_Write(CKCON2_P, IFD);\
    )


/**
 *******************************************************************************
 * @brief       External crystal enable/disable. (XTAL)
 * @details
 * @param[in]   __STATE__ : OSCin source selection.
 *  @arg\b      MW_ENABLE : Enable XTAL oscillating circuit. (0~25MHz)
 *  @arg\b      MW_DISABLE : Disable XTAL oscillating circuit. (Default)
 * @return      None
 * @note        Frequency range : 0~25MHz
 * @par         Example
 * @code
 *    __DRV_CLK_XTAL_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_XTAL_Cmd(__STATE__)\
    MWT(\
        DRV_PageP_Read(CKCON2_P);\
        __STATE__ == MW_ENABLE ? (IFD |= XTALE_P) : (IFD &= ~XTALE_P);\
        DRV_PageP_Write(CKCON2_P, IFD);\
    )


/**
 *******************************************************************************
 * @brief       External crystal driving control (XTAL Gain)
 * @details
 * @param[in]   __SELECT__ : External crystal driving control.
 *  @arg\b      XTAL_Low_Gain_32768 : Low gain for 32.768KHz
 *  @arg\b      XTAL_Gain_2MHz_to_25MHz : Medium gain for 2MHz~25MHz. (Default)
 *  @arg\b      XTAL_Lower_Gain_32768 : Lower gain for 32.768KHz
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_CLK_XTAL_Gain_Select(XTAL_Low_Gain_32768);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_CLK_XTAL_Gain_Select
#define XTAL_Low_Gain_32768      0x00
#define XTAL_Gain_2MHz_to_25MHz  XTGS0_P
#define XTAL_Lower_Gain_32768    XTGS1_P
/// @endcond

#define __DRV_CLK_XTAL_Gain_Select(__SELECT__)\
    MWT(\
        DRV_PageP_Read(CKCON2_P);\
        IFD &= ~(XTGS1_P | XTGS0_P);\
        IFD |= __SELECT__;\
        DRV_PageP_Write(CKCON2_P, IFD);\
    )


/**
 *******************************************************************************
 * @brief       Missing clock detection and interrupt enable/disable.
 * @details
 * @param[in]   __STATE__ : MCDF detection and interrupt enable/disable.
 *  @arg\b      MW_ENABLE : Enable MCD module and enable MCDF interrupt. (Default)
 *  @arg\b      MW_DISABLE : Disable MCDF interrupt.
 * @return
 * @note
 * @par         Example
 * @code
 *    __DRV_CLK_XTAL_MCDF_IT_Cmd(MW_DISABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_XTAL_MCDF_IT_Cmd(__STATE__)\
    MWT(\
        __STATE__ == MW_DISABLE ? (SFIE&=~MCDFIE) : (SFIE|=MCDFIE);\
    )


/**
 *******************************************************************************
 * @brief       Missing-Clock-Detection event causes system reset enable/disable.
 * @details
 * @param[in]   __STATE__ : MCD event causes a system reset.
 *  @arg\b      MW_ENABLE : Enable MCD event to trigger a system reset. (Default)
 *  @arg\b      MW_DISABLE : Disable MCD event to trigger a system reset.
 * @note
 * @par         Example
 * @code
 *    __DRV_CLK_XTAL_MCDF_RST_Cmd(XTAL_MCD_Reset_Enable);
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_XTAL_MCDF_RST_Cmd(__STATE__)\
    MWT(\
    __STATE__ == MW_DISABLE ? (SFIE&=~MCDRE) : (SFIE|=MCDRE);\
    )


/**
 *******************************************************************************
 * @brief       Get Missing Clock Detection Flag
 * @details
 * @return
 * @note
 * @par         Example
 * @code
 *    8bitReg(user define)=__DRV_CLK_GetMCDF();
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_MCDF_GetFlag()    (PCON1 & MCDF)


/**
 *******************************************************************************
 * @brief       Clear Missing Clock Detection Flag
 * @details
 * @return
 * @note
 * @par         Example
 * @code
 *    __DRV_CLK_ClearMCDF();
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_MCDF_ClearFlag()  PCON1 = MCDF


/**
 *******************************************************************************
 * @brief       Get crystal oscillating ready. (Read Only.)
 * @details
 * @return      return crystal oscillating (XTAL) status.
 * @note
 * @par         Example
 * @code
 *    8bitReg(user define)=__DRV_CLK_XTAL_Status();
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_CLK_XTAL_Status
#define XTAL_Ready      XTOR
#define XTAL_Not_Ready  0x00
/// @endcond

#define __DRV_CLK_XTAL_Status()     (CKCON1 & XTOR)


/**
 *******************************************************************************
 * @brief       OSCin source selection
 * @details
 * @param[in]   __SELECT__ : OSCin source selection.
 *  @arg\b      OSCin_IHRCO : Internal high frequency input. (12MHz/11.0592MHz) (Default)
 *  @arg\b      OSCin_XTAL : External crystalintput (32KHz~25MHz).
 *  @arg\b      OSCin_ILRCO : Internal Low frequency input. (32KHz)
 *  @arg\b      OSCin_ECKI_P60 : External OSC from P6.0 (0~25MHz).
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_OSCin_Select(OSCin_IHRCO);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_CLK_OSCin_Select
#define OSCin_IHRCO     0x00
#define OSCin_XTAL      OSCS0_P
#define OSCin_ILRCO     OSCS1_P
#define OSCin_ECKI_P60  (OSCS1_P | OSCS0_P)
/// @endcond

#define __DRV_CLK_OSCin_Select(__SELECT__)\
    MWT(\
        /* --- Set P60 Open-Drain Output, P60=1 --- */\
        DRV_PageP_Read(CKCON2_P);\
        IFD &= ~OSCin_ECKI_P60;\
        IFD |= __SELECT__;\
        DRV_PageP_Write(CKCON2_P, IFD);\
    )


/**
 *******************************************************************************
 * @brief       Multiplier (PLL) Enable
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_CKM_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_PLL_Enable()      CKCON0 |= ENCKM


/**
 *******************************************************************************
 * @brief       Multiplier (PLL) Disable
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_CKM_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_PLL_Disable()     CKCON0 &= ~ENCKM


/**
 *******************************************************************************
 * @brief       Multiplier (PLL) Enable / Disable
 * @details
 * @param[in]   __STATE__ : Multiplier Enable / Disable
 *  @arg\b      MW_ENABLE : Multiplier (PLL) Enable
 *  @arg\b      MW_DISALBE : Multiplier (PLL) Disable (Defulat)
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_PLL_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_PLL_Cmd(__STATE__)\
    MWT(\
        __STATE__ == MW_ENABLE ? (CKCON0 |= ENCKM) : (CKCON0 &= ~ENCKM);\
    )


/**
 *******************************************************************************
 * @brief       Multiplier (PLL) Config
 * @details
 * @param[in]   __SELECT__ : Clock multiplier input selection.
 *  @arg\b      CKM_OSCin_DIV_1 : OSCin=5~7MHz
 *  @arg\b      CKM_OSCin_DIV_2 : OSCin=10~14MHz (Default)
 *  @arg\b      CKM_OSCin_DIV_4 : OSCin=20~28MHz
 * @param[in]   __MODE__ : CKM mode selection.
 *  @arg\b      CKMI_X4X5X8 : Set CKM to CKMI X8 output mode. (96MHz for PCA)(Default)
 *  @arg\b      CKMI_X6X8X12 : Set CKM to CKMI X12 output mode. (144MHz for PCA)
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_PLL_Config(CKM_OSCin_DIV_2, PLLI_X4X5X8);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_CLK_PLL_Config
#define CKM_OSCin_DIV_1  0x00     // OSCin=5~7MHz
#define CKM_OSCin_DIV_2  CKMIS0   // OSCin=10~14MHz (Default)
#define CKM_OSCin_DIV_4  CKMIS1   // OSCin=20~28MHz
#define PLLI_X4X5X8      0x00     // Multiplier Mode Select X4 / X5.33 / X8
#define PLLI_X6X8X12     CKMS0_P  // Multiplier Mode Select X6 / X8 / X12
#define CKMI_X4X5X8      0x00     // Multiplier Mode Select X4 / X5.33 / X8
#define CKMI_X6X8X12     CKMS0_P  // Multiplier Mode Select X6 / X8 / X12
/// @endcond

#define __DRV_CLK_PLL_Config(__SELECT__, __MODE__)\
    MWT(\
        IFD = CKCON0;\
        IFD &= ~(CKMIS1|CKMIS0);\
        IFD |= __SELECT__;\
        CKCON0 = IFD;\
        DRV_PageP_Read(CKCON5_P);\
        __MODE__ == CKMI_X6X8X12 ? (IFD |= CKMS0_P ) : (IFD &= ~CKMS0_P);\
        DRV_PageP_Write(CKCON5_P, IFD);\
    )


/**
 *******************************************************************************
 * @brief       Multiplier clock select (MCK)
 * @details
 * @param[in]   __SELECT__ : MCK source selection.
 *  @arg\b      MCK_OSCin : OSCin (Default)
 *  @arg\b      MCK_CKMI_X4X6 : 24MHz/36MHz/22.118MHz/33.177MHz
 *  @arg\b      MCK_CKMI_X5X8 : 32MHz/48MHz/29.491MHz/44.236MHz
 *  @arg\b      MCK_CKMI_X8X12 : 48MHz/72MHz/44.236MHz/66.354MHz
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_MCK_Select(MCK_OSCin);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_CLK_MCK_Select
#define MCK_OSCin       0x00
#define MCK_CKMI_X4X6   MCKS0_P
#define MCK_CKMI_X5X8   MCKS1_P              // X5 is X5.33
#define MCK_CKMI_X8X12  (MCKS1_P | MCKS0_P)
/// @endcond

#define __DRV_CLK_MCK_Select(__SELECT__)\
    MWT(\
        DRV_PageP_Read(CKCON2_P);\
        IFD &= ~MCK_CKMI_X8X12;\
        IFD |= __SELECT__;\
        DRV_PageP_Write(CKCON2_P, IFD);\
    )


/**
 *******************************************************************************
 * @brief       Select multi-clock divide output select (MCKDO)
 * @details
 * @param[in]   __SELECT__ : MCK divider output selection.
 *  @arg\b      MCKDO_MCK_DIV_1 : MCKDO = MCK / 1 (Default)
 *  @arg\b      MCKDO_MCK_DIV_2 : MCKDO = MCK / 2
 *  @arg\b      MCKDO_MCK_DIV_4 : MCKDO = MCK / 4
 *  @arg\b      MCKDO_MCK_DIV_8 : MCKDO = MCK / 8
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_MCKDO_Select(MCKDO_MCK_DIV_1);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_CLK_MCKDO_Select
#define MCKDO_MCK_DIV_1  0x00
#define MCKDO_MCK_DIV_2  MCKD0_P
#define MCKDO_MCK_DIV_4  MCKD1_P
#define MCKDO_MCK_DIV_8  (MCKD1_P | MCKD0_P)
/// @endcond

#define __DRV_CLK_MCKDO_Select(__SELECT__)\
    MWT(\
        DRV_PageP_Read(CKCON3_P);\
        IFD &= ~MCKDO_MCK_DIV_8;\
        IFD |= __SELECT__;\
        DRV_PageP_Write(CKCON3_P, IFD);\
    )


/**
 *******************************************************************************
 * @brief       Select system clock divide from MCKDO (SYSCLK)
 * @details
 * @param[in]   __SELECT__ : Programmable system clock selection.
 *  @arg\b      SYSCLK_MCKDO_DIV_1 : SYSCLK = MCKDO / 1 (Default)
 *  @arg\b      SYSCLK_MCKDO_DIV_2 : SYSCLK = MCKDO / 2
 *  @arg\b      SYSCLK_MCKDO_DIV_2 : SYSCLK = MCKDO / 4
 *  @arg\b      SYSCLK_MCKDO_DIV_2 : SYSCLK = MCKDO / 8
 *  @arg\b      SYSCLK_MCKDO_DIV_2 : SYSCLK = MCKDO / 16
 *  @arg\b      SYSCLK_MCKDO_DIV_2 : SYSCLK = MCKDO / 32
 *  @arg\b      SYSCLK_MCKDO_DIV_2 : SYSCLK = MCKDO / 64
 *  @arg\b      SYSCLK_MCKDO_DIV_2 : SYSCLK = MCKDO / 128
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_SYSCLK_Select(SYSCLK_MCKDO_DIV_1);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_CLK_SYSCLK_Select
#define SYSCLK_MCKDO_DIV_1    0x00                 //SYSCLK=MCKDO/1 (Default)
#define SYSCLK_MCKDO_DIV_2    SCKS0                //SYSCLK=MCKDO/2
#define SYSCLK_MCKDO_DIV_4    SCKS1                //SYSCLK=MCKDO/4
#define SYSCLK_MCKDO_DIV_8    (SCKS1|SCKS0)        //SYSCLK=MCKDO/8
#define SYSCLK_MCKDO_DIV_16   SCKS2                //SYSCLK=MCKDO/16
#define SYSCLK_MCKDO_DIV_32   (SCKS2|SCKS0)        //SYSCLK=MCKDO/32
#define SYSCLK_MCKDO_DIV_64   (SCKS2|SCKS1)        //SYSCLK=MCKDO/64
#define SYSCLK_MCKDO_DIV_128  (SCKS2|SCKS1|SCKS0)  //SYSCLK=MCKDO/128
/// @endcond

#define __DRV_CLK_SYSCLK_Select(__SELECT__)\
    MWT(\
        CKCON0 = (CKCON0 & (~SYSCLK_MCKDO_DIV_128)) | __SELECT__;\
    )


/**
 *******************************************************************************
 * @brief       Select CPU clock = system clock / 1
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_CPUCLK_SYSCLK_DIV_1();
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_CPUCLK_SYSCLK_DIV_1()     CKCON0 = CKCON0 & (~CCKS)


/**
 *******************************************************************************
 * @brief       Select CPU clock = system clock / 2
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_CPUCLK_SYSCLK_DIV_2();
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_CPUCLK_SYSCLK_DIV_2()     CKCON0 = CKCON0 | CCKS


/**
 *******************************************************************************
 * @brief       Select CPU clock divide from system clock (CPUCLK)
 * @details
 * @param[in]   __SELECT__ : CPU clock select.
 *  @arg\b      CPUCLK_SYSCLK_DIV_1 : CPUCLK = SYSCLK = MCKDO / 1 (Default)
 *  @arg\b      CPUCLK_SYSCLK_DIV_2 : CPUCLK = SYSCLK = MCKDO / 2
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_CPUCLK_Select(CPUCLK_SYSCLK_DIV_1);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_CLK_CPUCLK_Select
#define CPUCLK_SYSCLK_DIV_1  0x00
#define CPUCLK_SYSCLK_DIV_2  CCKS
/// @endcond

#define __DRV_CLK_CPUCLK_Select(__SELECT__)\
    MWT(\
        (__SELECT__ == CPUCLK_SYSCLK_DIV_2 ? (CKCON0 |= CCKS) : (CKCON0 &= ~CCKS));\
    )


/**
 *******************************************************************************
 * @brief       CPU running in lower speed mode (6MHz > CPUCLK)
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_CPUCLK_SLOWER_6MHz();
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_CPUCLK_SLOWER_6MHz()\
    MWT(\
        DRV_PageP_Read(DCON0_P);\
        IFD &= ~(HSE_P | HSE1_P);\
        DRV_PageP_Write(DCON0_P, IFD);\
    )


/**
 *******************************************************************************
 * @brief       MCU full speed operation (25MHz > CPUCLK > 6MHz)
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_CPUCLK_6MHz_to_24MHz();
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_CPUCLK_6MHz_to_24MHz()\
    MWT(\
        DRV_PageP_Read(DCON0_P);\
        IFD |= HSE_P;\
        IFD &= ~HSE1_P;\
        DRV_PageP_Write(DCON0_P, IFD);\
    )


/**
 *******************************************************************************
 * @brief       MCU for ultra-high speed operation (CPUCLK > 25MHz)
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_CPUCLK_FASTER_25MHz();
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_CPUCLK_FASTER_25MHz()\
    MWT(\
        DRV_PageP_Read(DCON0_P);\
        IFD |= (HSE_P | HSE1_P);\
        DRV_PageP_Write(DCON0_P, IFD);\
    )


/**
 *******************************************************************************
 * @brief       MCU CPUCLK range config
 * @details
 * @param[in]   __SELECT__ : CPUCLK range config.
 *  @arg\b      SLOWER_CPUCLK : CPUCLK < 6MHz or CPUCLK = 6MHz
 *  @arg\b      MIDDLE_CPUCLK : 25MHz > CPUCLK > 6MHz (Default)
 *  @arg\b      FASTER_CPUCLK : CPUCLK > 25MHz
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_CPUCLK_Range_Select(MIDDLE_CPUCLK);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_CLK_CPUCLK_Range_Select
#define SLOWER_CPUCLK  0x00              //CPUCLK < 6MHz or CPUCLK = 6MHz
#define MIDDLE_CPUCLK  HSE_P             //25MHz > CPUCLK > 6MHz (Default)
#define FASTER_CPUCLK  (HSE_P | HSE1_P)  //CPUCLK > 25MHz
/// @endcond

#define __DRV_CLK_CPUCLK_Range_Select(__SELECT__)\
    MWT(\
        DRV_PageP_Read(DCON0_P);\
        IFD &= ~FASTER_CPUCLK;\
        IFD |= __SELECT__;\
        DRV_PageP_Write(DCON0_P, IFD);\
    )


/**
 *******************************************************************************
 * @brief       MCU normal wakeup from power down.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_NormalWakeup120us();
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_NormalWakeup120us()\
    MWT(\
        DRV_PageP_Read(CKCON3_P);\
        IFD &= ~FWKP_P;\
        DRV_PageP_Write(CKCON3_P, IFD);\
    )


/**
 *******************************************************************************
 * @brief       MCU fast wakeup from power down.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_FastWakeup30us();
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_FastWakeup30us()\
    MWT(\
        DRV_PageP_Read(CKCON3_P);\
        IFD |= FWKP_P;\
        DRV_PageP_Write(CKCON3_P, IFD);\
    )


/**
 *******************************************************************************
 * @brief       MCU wakeup select from power down.
 * @details
 * @param[in]   __SELECT__ : Power down wakeup select
 *  @arg\b      NORMAL_WAKEUP_120us : Power down normal wakeup. (120us)(Default)
 *  @arg\b      FAST_WAKEUP_30us : Power down fast wakeup. (30us)
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_Wakeup_Select(NORMAL_WAKEUP_120us);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_CLK_Wakeup_Select
#define NORMAL_WAKEUP_120us  0x00
#define FAST_WAKEUP_30us     FWKP_P
/// @endcond

#define __DRV_CLK_Wakeup_Select(__SELECT__)\
    MWT(\
        DRV_PageP_Read(CKCON3_P);\
        __SELECT__ == FAST_WAKEUP_30us ? (IFD |= FWKP_P ):(IFD &= ~FWKP_P);\
        DRV_PageP_Write(CKCON3_P, IFD);\
    )


/**
 *******************************************************************************
 * @brief       P6.0 multi-function select
 * @param[in]   __SELECT__ : P6.0 multi-function
 *  @arg\b      P60_GPIO : P6.0 is GPIO function (Default)
 *  @arg\b      P60_MCK_DIV_1 : P6.0 output MCK/1
 *  @arg\b      P60_MCK_DIV_2 : P6.0 output MCK/2
 *  @arg\b      P60_MCK_DIV_4 : P6.0 output MCK/4
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_P60Mux_Select(P60_MCK_DIV_1);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_CLK_P60Mux_Select
#define P60_GPIO       0x00
#define P60_MCK_DIV_1  P60OC0
#define P60_MCK_DIV_2  P60OC1
#define P60_MCK_DIV_4  (P60OC0 | P60OC1)
/// @endcond

#define __DRV_CLK_P60Mux_Select(__SELECT__)\
    MWT(\
        AUXR0 = (AUXR0 & ~P60_MCK_DIV_4)|__SELECT__;\
    )


/**
 *******************************************************************************
 * @brief       Enable P60 fast driving.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_P60FastDrive_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_P60FastDrive_Enable()     AUXR0 |= P60FD


/**
 *******************************************************************************
 * @brief       Disable P60 fast driving.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_P60FastDrive_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_P60FastDrive_Disable()    AUXR0 &= ~P60FD


/**
 *******************************************************************************
 * @brief       P60 fast drive Enable / Disalbe.
 * @details
 * @param[in]   __STATE__ : Enable / Disable
 *  @arg\b      MW_ENABLE : P6.0 fast drive enable.
 *  @arg\b      MW_DISALBE : P6.0 fast drive disable. (Defulat)
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_P60FastDrive_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_P60FastDrive_Cmd(__STATE__)\
    MWT(\
        __STATE__ == MW_ENABLE ? (AUXR0 |= P60FD ):(AUXR0 &= ~P60FD);\
    )


/**
 *******************************************************************************
 * @brief       CKCON0 unProtected (Defulat)
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_CKCON0_UnProtected();
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_CKCON0_UnProtected()\
    MWT(\
        DRV_PageP_Read(SPCON0_P);\
        IFD &= ~CKCTL0_P;\
        DRV_PageP_Write(SPCON0_P, IFD);\
    )


/**
 *******************************************************************************
 * @brief       CKCON0 Protected
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_CKCON0_Protected();
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_CKCON0_Protected()\
    MWT(\
        DRV_PageP_Read(SPCON0_P);\
        IFD |= CKCTL0_P;\
        DRV_PageP_Write(SPCON0_P, IFD);\
    )


/**
 *******************************************************************************
 * @brief       CKCON0 access control
 * @details
 * @param[in]   __STATE__ : Enable / Disable
 *  @arg\b      MW_ENABLE : CKCON0 Protected
 *  @arg\b      MW_DISALBE : CKCON0 unProtected (Defulat)
 * @return      None
 * @note
 * @par         Example
 * @code
 *  __DRV_CLK_CKCON_Protection_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_CLK_CKCON_Protection_Cmd(__STATE__)\
    MWT(\
        DRV_PageP_Read(SPCON0_P);\
        __STATE__== MW_ENABLE ? (IFD |= CKCTL0_P) : (IFD &= ~CKCTL0_P);\
        DRV_PageP_Write(SPCON0_P, IFD);\
    )


#endif  //_MG82F6D64_CLK_DRV_H



