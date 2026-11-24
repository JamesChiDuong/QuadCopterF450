/**
 ******************************************************************************
 *
 * @file        MG82F6D64_CLK_MID.H
 *
 * @brief       This is the C code format driver head file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     V1.02
 * @date        2022/06/10
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

#ifndef _MG82F6D64_CLK_MID_H
#define _MG82F6D64_CLK_MID_H


/**
 *******************************************************************************
 * @brief       Easy select system clock and cpu clock.
 * @details
 * @param[in]   __SELECT__ : System Clock Selection.
 *  @arg\b      SYS_12M_CPU_12M                : PCA CKMIX16=NA, MCKDO=12MHz; SYSCLK=CPUCLK=12MHz (Default)
 *  @arg\b      SYS_24M_CPU_24M                : PCA CKMIX16=96MHz, MCKDO=24MHz; SYSCLK=CPUCLK=24MHz
 *  @arg\b      SYS_32M_CPU_16M                : PCA CKMIX16=96MHz, MCKDO=32MHz; SYSCLK=32MHz, CPUCLK=16MHz
 *  @arg\b      SYS_48M_CPU_24M                : PCA CKMIX16=96MHz, MCKDO=48MHz; SYSCLK=48MHz, CPUCLK=24MHz
 *  @arg\b      SYS_11M0592_CPU_11M0592        : PCA CKMIX16=NA, MCKDO=11.059MHz; SYSCLK=11.059MHz, CPUCLK=11.059MHz
 *  @arg\b      SYS_22M118_CPU_22M118          : PCA CKMIX16=88.474MHz, MCKDO=22.118MHz; SYSCLK=CPUCLK=22.118MHz
 *  @arg\b      SYS_29M491_CPU_14M736          : PCA CKMIX16=88.47MHz, MCKDO=29.49MHz; SYSCLK=29.49MHz, CPUCLK=14.75MHz
 *  @arg\b      SYS_44M236_CPU_22M118          : PCA CKMIX16=88.47MHz, MCKDO=44.24MHz; SYSCLK=44.24MHz, CPUCLK=22.12MHz
 *  @arg\b      SYS_32K_CPU_32K                : PCA CKMIX16=NA, MCKDO=ILRCO(32KHz); SYSCLK=CPUCLK=ILRCO(32KHz)
 *  @arg\b      SYS_CPU_ECKI                   : PCA CKMIX16=NA, MCKDO=ECKI; SYSCLK=CPUCLK=ECKI(0~25MHz)
 *  @arg\b      PCA_12M_SYS_12M_CPU_12M        : PCA CKMIX16=NA, MCKDO=12MHz; SYSCLK=12MHz, CPUCLK=12MHz (Default)
 *  @arg\b      PCA_96M_SYS_24M_CPU_24M        : PCA CKMIX16=96MHz, MCKDO=24MHz; SYSCLK=24MHz, CPUCLK=24MHz
 *  @arg\b      PCA_96M_SYS_32M_CPU_16M        : PCA CKMIX16=96MHz, MCKDO=32MHz; SYSCLK=32MHz, CPUCLK=16MHz
 *  @arg\b      PCA_96M_SYS_32M_CPU_32M        : PCA CKMIX16=96MHz, MCKDO=32MHz; SYSCLK=32MHz, CPUCLK=32MHz
 *  @arg\b      PCA_96M_SYS_48M_CPU_24M        : PCA CKMIX16=96MHz, MCKDO=48MHz; SYSCLK=48MHz, CPUCLK=24MHz
 *  @arg\b      PCA_114M_SYS_36M_CPU_36M       : PCA CKMIX16=144MHz, MCKDO=36MHz; SYSCLK=36MHz, CPUCLK=36MHz
 *  @arg\b      PCA_114M_SYS_48M_CPU_24M       : PCA CKMIX16=144MHz, MCKDO=48MHz; SYSCLK=48MHz, CPUCLK=24MHz
 *  @arg\b      PCA_11M06_SYS_11M06_CPU_11M06  : PCA CKMIX16=NA, MCKDO=11.06MHz; SYSCLK=11.06MHz, CPUCLK=11.06MHz
 *  @arg\b      PCA_88M47_SYS_22M12_CPU_22M12  : PCA CKMIX16=88.47MHz, MCKDO=22.12MHz; SYSCLK=22.12MHz, CPUCLK=22.12MHz
 *  @arg\b      PCA_88M47_SYS_29M49_CPU_14M75  : PCA CKMIX16=88.47MHz, MCKDO=29.49MHz; SYSCLK=29.49MHz, CPUCLK=14.75MHz
 *  @arg\b      PCA_88M47_SYS_29M49_CPU_29M49  : PCA CKMIX16=88.47MHz, MCKDO=29.49MHz; SYSCLK=29.49MHz, CPUCLK=29.49MHz
 *  @arg\b      PCA_88M47_SYS_44M24_CPU_22M12  : PCA CKMIX16=88.47MHz, MCKDO=44.24MHz; SYSCLK=44.24MHz, CPUCLK=22.12MHz
 *  @arg\b      PCA_132M71_SYS_33M18_CPU_33M18 : PCA CKMIX16=132.71MHz, MCKDO=33.18MHz; SYSCLK=33.18MHz, CPUCLK=33.18MHz
 *  @arg\b      PCA_132M71_SYS_44M24_CPU_22M12 : PCA CKMIX16=132.71MHz, MCKDO=44.24MHz; SYSCLK=44.24MHz, CPUCLK=22.12MHz
 *  @arg\b      PCA_SYS_CPU_XTAL_2M_to_25M     : PCA CKMIX16=NA, MCKDO=XTAL; SYSCLK=CPUCLK=XTAL(2MHz~25MHz)
 *  @arg\b      PCA_SYS_CPU_XTAL_32768         : PCA CKMIX16=NA, MCKDO=XTAL(32768); SYSCLK=CPUCLK=XTAL(32768)
 *  @arg\b      PCA_SYS_CPU_XTAL_32768_LowGain : PCA CKMIX16=NA, MCKDO=XTAL(32768)(Low Gain); SYSCLK=CPUCLK=XTAL(32768)(Low Gain)
 *  @arg\b      PCA_SYS_CPU_ILRCO_32K          : PCA CKMIX16=NA, MCKDO=ILRCO(32KHz); SYSCLK=CPUCLK=ILRCO(32KHz)
 *  @arg\b      PCA_SYS_CPU_ECKI               : PCA CKMIX16=NA, MCKDO=ECKI; SYSCLK=CPUCLK=ECKI(0~25MHz)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_CLK_EasySelect(SYS_12M_CPU_12M);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_CLK_Easy_Select
#define SYS_12M_CPU_12M                 0x80025010  //PCA CKMIX16=NA, MCKDO=12MHz; SYSCLK=CPUCLK=12MHz (Default)
#define SYS_24M_CPU_24M                 0x80025450  //PCA CKMIX16=96MHz, MCKDO=24MHz; SYSCLK=CPUCLK=24MHz
#define SYS_32M_CPU_16M                 0x80025858  //PCA CKMIX16=96MHz, MCKDO=32MHz; SYSCLK=32MHz, CPUCLK=16MHz
#define SYS_48M_CPU_24M                 0x80025C58  //PCA CKMIX16=96MHz, MCKDO=48MHz; SYSCLK=48MHz, CPUCLK=24MHz
#define SYS_11M0592_CPU_11M0592         0x80025090  //PCA CKMIX16=NA, MCKDO=11.059MHz; SYSCLK=11.059MHz, CPUCLK=11.059MHz
#define SYS_22M118_CPU_22M118           0x800254D0  //PCA CKMIX16=88.474MHz, MCKDO=22.118MHz; SYSCLK=CPUCLK=22.118MHz
#define SYS_29M491_CPU_14M736           0x800258D8  //PCA CKMIX16=88.47MHz, MCKDO=29.49MHz; SYSCLK=29.49MHz, CPUCLK=14.75MHz
#define SYS_44M236_CPU_22M118           0x80025CD8  //PCA CKMIX16=88.47MHz, MCKDO=44.24MHz; SYSCLK=44.24MHz, CPUCLK=22.12MHz
#define SYS_32K_CPU_32K                 0x00024210  //PCA CKMIX16=NA, MCKDO=ILRCO(32KHz); SYSCLK=CPUCLK=ILRCO(32KHz)
#define SYS_CPU_ECKI                    0x80024310  //PCA CKMIX16=NA, MCKDO=ECKI; SYSCLK=CPUCLK=ECKI(0~25MHz)
#define PCA_12M_SYS_12M_CPU_12M         0x80025010  //PCA CKMIX16=NA, MCKDO=12MHz; SYSCLK=12MHz, CPUCLK=12MHz (Default)
#define PCA_96M_SYS_24M_CPU_24M         0x80025450  //PCA CKMIX16=96MHz, MCKDO=24MHz; SYSCLK=24MHz, CPUCLK=24MHz
#define PCA_96M_SYS_32M_CPU_16M         0x80025858  //PCA CKMIX16=96MHz, MCKDO=32MHz; SYSCLK=32MHz, CPUCLK=16MHz
#define PCA_96M_SYS_32M_CPU_32M         0xA0025850  //PCA CKMIX16=96MHz, MCKDO=32MHz; SYSCLK=32MHz, CPUCLK=32MHz
#define PCA_96M_SYS_48M_CPU_24M         0x80025C58  //PCA CKMIX16=96MHz, MCKDO=48MHz; SYSCLK=48MHz, CPUCLK=24MHz
#define PCA_114M_SYS_36M_CPU_36M        0xA1025450  //PCA CKMIX16=144MHz, MCKDO=36MHz; SYSCLK=36MHz, CPUCLK=36MHz
#define PCA_114M_SYS_48M_CPU_24M        0x81025858  //PCA CKMIX16=144MHz, MCKDO=48MHz; SYSCLK=48MHz, CPUCLK=24MHz
#define PCA_11M06_SYS_11M06_CPU_11M06   0x80025090  //PCA CKMIX16=NA, MCKDO=11.06MHz; SYSCLK=11.06MHz, CPUCLK=11.06MHz
#define PCA_88M47_SYS_22M12_CPU_22M12   0x800254D0  //PCA CKMIX16=88.47MHz, MCKDO=22.12MHz; SYSCLK=22.12MHz, CPUCLK=22.12MHz
#define PCA_88M47_SYS_29M49_CPU_14M75   0x800258D8  //PCA CKMIX16=88.47MHz, MCKDO=29.49MHz; SYSCLK=29.49MHz, CPUCLK=14.75MHz
#define PCA_88M47_SYS_29M49_CPU_29M49   0xA00258D0  //PCA CKMIX16=88.47MHz, MCKDO=29.49MHz; SYSCLK=29.49MHz, CPUCLK=29.49MHz
#define PCA_88M47_SYS_44M24_CPU_22M12   0x80025CD8  //PCA CKMIX16=88.47MHz, MCKDO=44.24MHz; SYSCLK=44.24MHz, CPUCLK=22.12MHz
#define PCA_132M71_SYS_33M18_CPU_33M18  0xA10254D0  //PCA CKMIX16=132.71MHz, MCKDO=33.18MHz; SYSCLK=33.18MHz, CPUCLK=33.18MHz
#define PCA_132M71_SYS_44M24_CPU_22M12  0x810258D8  //PCA CKMIX16=132.71MHz, MCKDO=44.24MHz; SYSCLK=44.24MHz, CPUCLK=22.12MHz
#define PCA_SYS_CPU_XTAL_2M_to_25M      0x80026110  //PCA CKMIX16=NA, MCKDO=XTAL; SYSCLK=CPUCLK=XTAL(2MHz~25MHz)
#define PCA_SYS_CPU_XTAL_32768          0x00022110  //PCA CKMIX16=NA, MCKDO=XTAL(32768); SYSCLK=CPUCLK=XTAL(32768)
#define PCA_SYS_CPU_XTAL_32768_LowGain  0x0002A110  //PCA CKMIX16=NA, MCKDO=XTAL(32768)(Low Gain); SYSCLK=CPUCLK=XTAL(32768)(Low Gain)
#define PCA_SYS_CPU_ILRCO_32K           0x00024210  //PCA CKMIX16=NA, MCKDO=ILRCO(32KHz); SYSCLK=CPUCLK=ILRCO(32KHz)
#define PCA_SYS_CPU_ECKI                0x80024310  //PCA CKMIX16=NA, MCKDO=ECKI; SYSCLK=CPUCLK=ECKI(0~25MHz)
/// @endcond


#define __DRV_CLK_Easy_Select(__SELECT__)\
    MWT(\
        /* ======================================= */\
        /* --- Set at beginning for clock switch flow stable --- */\
        __DRV_CLK_CPUCLK_FASTER_25MHz();\
        /* --- MCKS[1:0]=00: MCK=OSCin --- */\
        __DRV_CLK_MCK_Select(MCK_OSCin);\
        /* ======================================= */\
        /* --- IHRCOE=1 --- */\
        __DRV_CLK_IHRCO_Select(IHRCO_12MHz);\
        __DRV_CLK_OSCin_Select(OSCin_IHRCO);\
        /* --- XTALE=1, IHRCOE=1, OSCin=IHRCO --- */\
        IFD = u32B1(__SELECT__)&(XTGS1_P|XTGS0_P|XTALE_P|IHRCOE_P);\
        IFD |= IHRCOE_P;\
        DRV_PageP_Write(CKCON2_P, IFD);\
        /* --- Set AFS, ENCKM, CKMIS[1:0], CCKS=0, SCKS[2:0]=0 --- */\
        IFD = u32B0(__SELECT__)&(AFS_P|ENCKM_P|CKMIS1_P|CKMIS0_P);\
        DRV_PageP_Write(CKCON0_P, IFD);\
        /* --- CKCON5 : CKMS0 --- */\
        DRV_PageP_Write(CKCON5_P, (u32B3(__SELECT__)&CKMS0_P));\
        /* --- OSCin=XTAL, wait XTOR=1, enable MCDF --- */\
        SFIE|=(MCDRE|MCDFIE);\
        if ((__SELECT__==PCA_SYS_CPU_XTAL_2M_to_25M) ||\
            (__SELECT__==PCA_SYS_CPU_XTAL_32768) ||\
            (__SELECT__==PCA_SYS_CPU_XTAL_32768_LowGain))\
        {\
            while(!(CKCON1 & XTOR));\
        }\
        /* --- OSCin=ECKI, Set P60 Open-Drain Output, P60=1 --- */\
        (__SELECT__==PCA_SYS_CPU_ECKI) ? _push_(SFRPI) : nop();\
        (__SELECT__==PCA_SYS_CPU_ECKI) ? __DRV_SFR_PageIndex(P6M0_Page) : nop();\
        (__SELECT__==PCA_SYS_CPU_ECKI) ? P6M0&=~0x01 : nop();\
        (__SELECT__==PCA_SYS_CPU_ECKI) ? P60=1 : nop();\
        (__SELECT__==PCA_SYS_CPU_ECKI) ? __DRV_SFR_PageIndex(P6M1_Page) : nop();\
        (__SELECT__==PCA_SYS_CPU_ECKI) ? P6M1&=~0x01 : nop();\
        (__SELECT__==PCA_SYS_CPU_ECKI) ? _pop_(SFRPI) : nop();\
        /* --- delay 32us time for stable --- */\
        IFD=255;\
        do{IFD--;}while(IFD!=0);\
        /* --- Set OSCin --- */\
        __DRV_CLK_OSCin_Select(u32B1(__SELECT__)&(OSCS1_P|OSCS0_P));\
        /* ======================================= */\
        /* --- delay 100us for CKM stable --- */\
        IFD = u32B0(__SELECT__)&ENCKM_P;\
        if (IFD)\
        {\
            IFD=255;\
            do{IFD--;}while(IFD!=0);\
            do{IFD--;}while(IFD!=0);\
            do{IFD--;}while(IFD!=0);\
            do{IFD--;}while(IFD!=0);\
        }\
        /* ======================================= */\
        /* --- CKCON0 : AFS, ENCKM, CKMIS[1:0], CCKS, SCKS[2:0] --- */\
        DRV_PageP_Write(CKCON0_P, u32B0(__SELECT__));\
        /* ======================================= */\
        /* --- CKCON3 : WDTCS[1:0], FWKP, WDTFS, MCKD[1:0], MCDS[1:0] --- */\
        /*__DRV_CLK_MCKDO_Select(u32B2(__SELECT__)&(MCKD1_P|MCKD0_P));*/\
        /*__DRV_CLK_Wakeup_Select(u32B2(__SELECT__)&FWKP_P);*/\
        DRV_PageP_Read (CKCON3_P);\
        IFD &= ~(FWKP_P|MCKD1_P|MCKD0_P|MCDS1_P|MCDS0_P);\
        IFD |= (u32B2(__SELECT__)&(FWKP_P|MCKD1_P|MCKD0_P|MCDS1_P|MCDS0_P));\
        DRV_PageP_Write(CKCON3_P,IFD);\
        /* ======================================= */\
        /* --- Set last MCKS (OSCin, CKMIx4, CKMIx5, CKMIx8) --- */\
        DRV_PageP_Write(CKCON2_P, u32B1(__SELECT__));\
        /* ======================================= */\
        /* --- DCON0.7.5: HSE, HSE1 --- */\
        __DRV_CLK_CPUCLK_Range_Select(u32B3(__SELECT__)&(HSE_P|HSE1_P));\
    )


#endif  //_MG82F6D64_CLK_MID_H


