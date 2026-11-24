/**
 ******************************************************************************
 *
 * @file        MG82F6D64_TIMER_DRV.H
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

#ifndef MG82F6D64_TIMER_DRV_H
#define MG82F6D64_TIMER_DRV_H
/*--------------------------------------------------------------
Timer Function :
    1.Mode Select
        Timer0 Moder Select
            |----Timer0 Mode : __DRV_TIMER0_Mode_Select(__MODE__);
            __MODE__ :
                TIMER0_MODE0_8BIT_PWM / TIMER0_MODE1_16BIT_TIMER / TIMER0_MODE2_8BIT_AUTORELOAD / TIMER0_MODE3_TWO_8BIT

        Timer1 Moder Select
            |----Timer1 Mode : __DRV_TIMER1_Mode_Select(__MODE__);
            __MODE__ :
                TIMER1_MODE0_8BIT_PWM / TIMER1_MODE1_16BIT_TIMER / TIMER1_MODE2_8BIT_AUTORELOAD / TIMER1_MODE3_TIMER1_STOPPED

        Timer2 Moder Select
            |----Timer2 Mode : __DRV_TIMER2_Mode_Select(__MODE__);
            __MODE__ :
                TIMER2_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT           / TIMER2_MODE1_16BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
                TIMER2_MODE2_16BIT_CAPTURE                                     / TIMER2_MODE3_16BIT_CAPTURE_WITH_AUTO_ZERO
                TIMER2_MODE6_DUTY_CAPTURE                                      / TIMER2_SPLIT_MODE0_TWO_8BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
                TIMER2_SPLIT_MODE1_TWO_8BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT / TIMER2_SPLIT_MODE2_TWO_8BIT_CAPTURE
                TIMER2_SPLIT_MODE3_TWO_8BIT_CAPTURE_WITH_AUTO_ZERO             / TIMER2_SPLIT_MODE4_8BIT_PWM
                TIMER2_SPLIT_MODE6_DUTY_CAPTURE                                / TIMER2_BAUD_RATE_GENERATOR

        Timer3 Moder Select
            |----Timer3 Mode : __DRV_TIMER3_Mode_Select(__MODE__);
            __MODE__ :
                TIMER3_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT           / TIMER3_MODE1_16BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
                TIMER3_MODE2_16BIT_CAPTURE                                     / TIMER3_MODE3_16BIT_CAPTURE_WITH_AUTO_ZERO
                TIMER3_MODE6_DUTY_CAPTURE                                      / TIMER3_SPLIT_MODE0_TWO_8BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
                TIMER3_SPLIT_MODE1_TWO_8BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT / TIMER3_SPLIT_MODE2_TWO_8BIT_CAPTURE
                TIMER3_SPLIT_MODE3_TWO_8BIT_CAPTURE_WITH_AUTO_ZERO             / TIMER3_SPLIT_MODE4_8BIT_PWM
                TIMER3_SPLIT_MODE6_DUTY_CAPTURE

        Timer4 Moder Select
            |----Timer4 Mode : __DRV_TIMER4_Mode_Select(__MODE__);
            __MODE__ :
                TIMER4_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT           / TIMER4_MODE1_16BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
                TIMER4_MODE2_16BIT_CAPTURE                                     / TIMER4_MODE3_16BIT_CAPTURE_WITH_AUTO_ZERO
                TIMER4_MODE6_DUTY_CAPTURE                                      / TIMER4_SPLIT_MODE0_TWO_8BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
                TIMER4_SPLIT_MODE1_TWO_8BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT / TIMER4_SPLIT_MODE2_TWO_8BIT_CAPTURE
                TIMER4_SPLIT_MODE3_TWO_8BIT_CAPTURE_WITH_AUTO_ZERO             / TIMER4_SPLIT_MODE4_8BIT_PWM
                TIMER4_SPLIT_MODE6_DUTY_CAPTURE

    2.Timer Clock Source Select
        Timer0 Clock Source Select
            |----Timer0 Clock Source : __DRV_TIMER0_Clock_Source_Select(__MODE__);
            __MODE__ :
                TIMER0_CLOCK_SOURCE_SYSCLK_DIV_12  / TIMER0_CLOCK_SOURCE_T0_PIN        / TIMER0_CLOCK_SOURCE_SYSCLK
                TIMER0_CLOCK_SOURCE_ILRCO          / TIMER0_CLOCK_SOURCE_SYSCLK_DIV_48 / TIMER0_CLOCK_SOURCE_WDTPS
                TIMER0_CLOCK_SOURCE_SYSCLK_DIV_192 / TIMER0_CLOCK_SOURCE_T1OF

        Timer0 Gating Source Select
            |----Timer0 Gating Source : __DRV_TIMER0_Gating_Source_Select(__MODE__);
            __MODE__ :
                TIMER0_GATING_SOURCE_DISABLE  / TIMER0_GATING_SOURCE_INT0ET
                TIMER0_GATING_SOURCE_TF2      / TIMER0_GATING_SOURCE_KBIET

        Timer1 Clock Source Select
            |----Timer1 Clock Source : __DRV_TIMER1_Clock_Source_Select(__MODE__);
            __MODE__ :
                TIMER1_CLOCK_SOURCE_SYSCLK_DIV_12  / TIMER1_CLOCK_SOURCE_T1_PIN
                TIMER1_CLOCK_SOURCE_SYSCLK         / TIMER1_CLOCK_SOURCE_SYSCLK_DIV_48

        Timer1 Gating Source Select
            |----Timer1 Gating Source : __DRV_TIMER1_Gating_Source_Select(__MODE__);
            __MODE__ :
                TIMER1_GATING_SOURCE_DISABLE  / TIMER1_GATING_SOURCE_INT1ET
                TIMER1_GATING_SOURCE_TF3      / TIMER1_GATING_SOURCE_TI1

        Timer2 Clock Source Select
            |----Timer2 Clock Source : __DRV_TIMER2_Clock_Source_Select(__MODE__);
            __MODE__ :
                TIMER2_CLOCK_SOURCE_SYSCLK_DIV_12  / TIMER2_CLOCK_SOURCE_T2_PIN / TIMER2_CLOCK_SOURCE_SYSCLK
                TIMER2_CLOCK_SOURCE_INT0ET         / TIMER2_CLOCK_SOURCE_S0TOF  / TIMER2_CLOCK_SOURCE_T0OF
                TIMER2_CLOCK_SOURCE_AC0ES          / TIMER2_CLOCK_SOURCE_KBIET

        TIMER2 TH2 Clock Source in Split Mode
            |----Timer2 TH2 Clock Source : __DRV_TIMER2_TH2_Clock_Source_Select(__MODE__);
            __MODE__ :
                TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12  / TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_T2_PIN
                TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK         / TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_TL2OF
                TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_S0TOF          / TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_T0OF
                TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_AC0ES          / TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_KBIET

        TIMER2 TL2 Clock Source in Split Mode
            |----Timer2 TL2 Clock Source : __DRV_TIMER2_TL2_Clock_Source_Select(__MODE__);
            __MODE__ :
                TIMER2_TL2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12  / TIMER2_TL2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK
                TIMER2_TL2_SPLIT_MODE_CLOCK_SOURCE_S0TOF          / TIMER2_TL2_SPLIT_MODE_CLOCK_SOURCE_INT0ET

        Timer3 Clock Source Select
            |----Timer3 Clock Source : __DRV_TIMER3_Clock_Source_Select(__MODE__);
            __MODE__ :
                TIMER3_CLOCK_SOURCE_SYSCLK_DIV_12  / TIMER3_CLOCK_SOURCE_T3_PIN      / TIMER3_CLOCK_SOURCE_SYSCLK
                TIMER3_CLOCK_SOURCE_INT1ET         / TIMER3_CLOCK_SOURCE_S1TOF       / TIMER3_CLOCK_SOURCE_T0OF
                TIMER3_CLOCK_SOURCE_AC1ES          / TIMER3_CLOCK_SOURCE_T1OF

        TIMER3 TH3 Clock Source in Split Mode
            |----Timer3 TH3 Clock Source : __DRV_TIMER3_TH3_Clock_Source_Select(__MODE__);
            __MODE__ :
                TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12  / TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_T3_PIN
                TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK         / TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_TL3OF
                TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_S1TOF          / TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_T0OF
                TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_AC1ES          / TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_T1OF

        TIMER3 TL3 Clock Source in Split Mode
            |----Timer3 TL3 Clock Source : __DRV_TIMER3_TL3_Clock_Source_Select(__MODE__);
            __MODE__ :
                TIMER3_TL3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12  / TIMER3_TL3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK
                TIMER3_TL3_SPLIT_MODE_CLOCK_SOURCE_S1TOF          / TIMER3_TL3_SPLIT_MODE_CLOCK_SOURCE_INT1ET

        Timer4 Clock Source Select
            |----Timer4 Clock Source : __DRV_TIMER4_Clock_Source_Select(__MODE__);
            __MODE__ :
                TIMER4_CLOCK_SOURCE_SYSCLK_DIV_12  / TIMER4_CLOCK_SOURCE_T4_PIN      / TIMER4_CLOCK_SOURCE_SYSCLK
                TIMER4_CLOCK_SOURCE_INT2ET         / TIMER4_CLOCK_SOURCE_RESERVED    / TIMER4_CLOCK_SOURCE_T0OF
                TIMER4_CLOCK_SOURCE_AC2ES          / TIMER4_CLOCK_SOURCE_AC0ES

        TIMER4 TH4 Clock Source in Split Mode
            |----Timer4 TH4 Clock Source : __DRV_TIMER4_TH4_Clock_Source_Select(__MODE__);
            __MODE__ :
                TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12  / TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_T4_PIN
                TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK         / TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_TL4OF
                TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_RESERVED       / TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_T0OF
                TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_AC2ES          / TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_AC0ES

        TIMER4 TL4 Clock Source in Split Mode
            |----Timer4 TL4 Clock Source : __DRV_TIMER4_TL4_Clock_Source_Select(__MODE__);
            __MODE__ :
                TIMER4_TL4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12  / TIMER4_TL4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK
                TIMER4_TL4_SPLIT_MODE_CLOCK_SOURCE_RESERVED       / TIMER4_TL4_SPLIT_MODE_CLOCK_SOURCE_INT2ET

        TIMER5 Clock Source Select
            |----Timer5 Clock Source : __DRV_TIMER5_Clock_Source_Select(__MODE__);
            __MODE__ :
                TIMER5_CLOCK_SOURCE_SYSCLK_DIV_12  / TIMER5_CLOCK_SOURCE_T5_PIN
                TIMER5_CLOCK_SOURCE_SYSCLK         / TIMER5_CLOCK_SOURCE_T2EXI

        Timer5 Gating Source Select
            |----Timer5 Gating Source : __DRV_TIMER5_Gating_Source_Select(__MODE__);
            __MODE__ :
                TIMER5_GATING_SOURCE_DISABLE  / TIMER5_GATING_SOURCE_INT0ET
                TIMER5_GATING_SOURCE_INT2ET   / TIMER5_GATING_SOURCE_T3EXI

        TIMER6 Clock Source Select
            |----Timer6 Clock Source : __DRV_TIMER6_Clock_Source_Select(__MODE__);
            __MODE__ :
                TIMER6_CLOCK_SOURCE_SYSCLK_DIV_12  / TIMER6_CLOCK_SOURCE_T6_PIN
                TIMER6_CLOCK_SOURCE_SYSCLK         / TIMER6_CLOCK_SOURCE_T3EXI

        Timer6 Gating Source Select
            |----Timer6 Gating Source : __DRV_TIMER6_Gating_Source_Select(__MODE__);
            __MODE__ :
                TIMER6_GATING_SOURCE_DISABLE  / TIMER6_GATING_SOURCE_INT1ET
                TIMER6_GATING_SOURCE_INT2ET   / TIMER6_GATING_SOURCE_T2EXI

    3.Timer Interrupt
        Enable/Disable Interrupt
            |----Enable Disable TIMERx Interrupt        : __DRV_TIMERx_IT_Cmd(__STATE__);       (x = 0~6)
            |----Enable Disable TIMERx TFxL Interrupt   : __DRV_TIMERx_TFxL_IT_Cmd(__STATE__);  (x = 2~4)
            __STATE__ :
                MW_DISABLE  / MW_ENABLE

        TIMER Interrupt Ignored (T2 T3 T4 OK)
            |----TIMERx TFx Interrupt Ignored : __DRV_TIMERx_TFx_Interrupt_Ignored_Cmd(__STATE__); (x = 2~4)
            __STATE__ :
                MW_DISABLE  / MW_ENABLE

    4.Timer Run Control
        |----TIMERx Run Control       : __DRV_TIMERx_Run_Cmd(__STATE__);    (x = 0~6)
        |----TIMERx TLx Run Control   : __DRV_TIMERx_TLx_Run_Cmd(__STATE__);(x = 2~4)
        __STATE__ :
            MW_DISABLE  / MW_ENABLE
        TIMER Clear Control (x = 2~4)
            |----TIMERx Clear Control : __DRV_TIMERx_TRxL_Clear_Cmd(__STATE__);
            __STATE__ :
                MW_DISABLE  / MW_ENABLE

    5.Timer Clock Output
        Clock Output
            |----TIMERx TxCKO             : __DRV_TIMERx_TxCKO_Cmd(__STATE__);      (x = 0~1)
            |----TIMERx Clock Out Control : __DRV_TIMERx_Clock_Out_Cmd(__STATE__);  (x = 2~4)
            __STATE__ :
                MW_DISABLE  / MW_ENABLE

        TIMER0 Pin MUX
            |----TIMER0 T0CKO Pin MUX : __DRV_TIMER0_T0CKO_PinMux_Select(__SELECT__);
            __SELECT__ :
                TIMER0_T0CKO_P34  / TIMER0_T0CKO_P44 / TIMER0_T0CKO_P46 / TIMER0_T0CKO_P17

        TIMER1 Pin MUX
            |----TIMER1 T1CKO Pin MUX : __DRV_TIMER1_T1CKO_PinMux_Select(__SELECT__);
            __SELECT__ :
                TIMER1_T1CKO_P35  / TIMER1_T1CKO_P45 / TIMER1_T1CKO_P36 / TIMER1_T1CKO_P37

        TIMER2 Pin MUX
            |----TIMER2 Pin Mux : __DRV_TIMER2_PinMUX_Select(__SELECT__);
            __SELECT__ :
                TIMER2_PINMUX_T2_T2CKO_P10_T2EX_P11  / TIMER2_PINMUX_T2_T2CKO_P32_T2EX_P32
                TIMER2_PINMUX_T2_T2CKO_P67_T2EX_P57  / TIMER2_PINMUX_T2_T2CKO_P45_T2EX_P44

        TIMER3 Pin MUX
            |----TIMER3 Pin Mux : __DRV_TIMER3_PinMUX_Select(__SELECT__);
            __SELECT__ :
                TIMER3_PINMUX_T3_T3CKO_P46_T3EX_P00  / TIMER3_PINMUX_T3_T3CKO_P33_T3EX_P32
                TIMER3_PINMUX_T3_T3CKO_P21_T3EX_P20  / TIMER3_PINMUX_T3_T3CKO_P66_T3EX_P65

        TIMER4 Pin MUX
            |----TIMER4 Pin Mux : __DRV_TIMER4_PinMUX_Select(__SELECT__);
            __SELECT__ :
                TIMER4_PINMUX_T4_T4CKO_P70_T4EX_P71  / TIMER4_PINMUX_T4_T4CKO_P37_T4EX_P36
                TIMER4_PINMUX_T4_T4CKO_P72_T4EX_P50  / TIMER4_PINMUX_T4_T4CKO_P36_T4EX_P37

    6.Timer Counter Register
        Timer0~6 High/Low Byte Register
            |----TIMERx High Byte Register : __DRV_TIMERx_SetHighByte(__RELOAD__); (x = 0~6)
            |----TIMERx Low Byte Register  : __DRV_TIMERx_SetLowByte(__RELOAD__);  (x = 0~6)
            __RELOAD__ :
                0x00 ~ 0xFF

        Timer0/1 8 Bit Register
            |----TIMERx 8Bit Timer/Counter Auto Reload  : __DRV_TIMERx_Set8BitIntervalAutoReload(__RELOAD__); (x = 0,1)
            __RELOAD__ :
                0x00 ~ 0xFF

        Timer0~4 16 Bit Register    *FA237 name => __DRV_TIMERx_Set16BitInterval
            |----TIMERx 16 Bit Timer/Counter Reload  : __DRV_TIMERx_Set16BitCounter(__RELOAD__); (x = 0~6)
            __RELOAD__ :
                0x0000 ~ 0xFFFF

        TIMERx Capture High/Low Byte Register(AutoReload Reister) (x = 2~4)
            |----TIMERx Capture HighByte Register : __DRV_TIMERx_SetRCAPxH(__RELOAD__);
            |----TIMERx Capture LowByte Register  : __DRV_TIMERx_SetRCAPxL(__RELOAD__);
            __RELOAD__ :
               0x00 ~ 0xFF

        Timer5/6 High/Low Byte Auto Reload Register
            |----TIMERx High Byte Reload Register : __DRV_TIMERx_SetTHRx(__RELOAD__); (x = 5,6)
            |----TIMERx Low Byte Reload Register  : __DRV_TIMERx_SetTLRx(__RELOAD__); (x = 5,6)
            __RELOAD__ :
                0x00 ~ 0xFF

        TIMERx 16 Bit Auto Reload   *FA237 name => __DRV_TIMERx_Set16BitCaptureInterval
            |----TIMERx 16 Bit Auto Reload : __DRV_TIMERx_Set16Bit_AutoReloadValue(__RELOAD__); (x = 2~6)
            __RELOAD__ :
               0x0000 ~ 0xFFFF

    7.Timer Capture relation
        TIMERx Capture Source Select (x = 2~4)
            |---TIMER2 Capture Source : __DRV_TIMER2_Capture_Source_Select(__MODE__);
            __MODE__ :
                TIMER2_CAPTURE_SOURCE_T2EX_PIN  / TIMER2_CAPTURE_SOURCE_RXD0      / TIMER2_CAPTURE_SOURCE_P60
                TIMER2_CAPTURE_SOURCE_INT2ET    / TIMER2_CAPTURE_SOURCE_ILRCO     / TIMER2_CAPTURE_SOURCE_AC0OUT
                TIMER2_CAPTURE_SOURCE_KBIET     / TIMER2_CAPTURE_SOURCE_TWI0_SCL

            |---TIMER3 Capture Source : __DRV_TIMER3_Capture_Source_Select(__MODE__);
            __MODE__ :
                TIMER3_CAPTURE_SOURCE_T3EX_PIN   / TIMER3_CAPTURE_SOURCE_INT0ET    / TIMER3_CAPTURE_SOURCE_P60
                TIMER3_CAPTURE_SOURCE_NSS_PIN    / TIMER3_CAPTURE_SOURCE_KBIET     / TIMER3_CAPTURE_SOURCE_AC0OUT
                TIMER3_CAPTURE_SOURCE_AC1OUT     / TIMER3_CAPTURE_SOURCE_ILRCO

            |---TIMER4 Capture Source : __DRV_TIMER4_Capture_Source_Select(__MODE__);
            __MODE__ :
                TIMER4_CAPTURE_SOURCE_T4EX_PIN   / TIMER4_CAPTURE_SOURCE_NSS_PIN    / TIMER4_CAPTURE_SOURCE_INT3ET
                TIMER4_CAPTURE_SOURCE_RESERVED   / TIMER4_CAPTURE_SOURCE_RESERVED   / TIMER4_CAPTURE_SOURCE_AC0OUT
                TIMER4_CAPTURE_SOURCE_AC2OUT     / TIMER4_CAPTURE_SOURCE_TWI1_SCL

        TIMERx Capture Source Dectect (x = 2~4)
            |----TIMERx Capture Source Dectect : __DRV_TIMERx_Capture_Source_Dectect(__MODE__);
            __MODE__ :
               Timerx_IGNORED / Timerx_FALLING_EDGE / Timerx_RISING_EDGE / Timerx_DUAL_EDGE

    8.Timer PWM relation (x = 2~4)
         TIMERx TxCKO Invert Control (Split Mode4 8Bits PWM )
            |----TIMERx TxCKO Invert Control : __DRV_TIMERx_TxCKO_Invert_Cmd(__STATE__);
            __STATE__ :
                MW_DISABLE  / MW_ENABLE

    9.Timer Global Function
         TIMER Globel Enable Control
            |----TIMER Globel Enable Control : __DRV_TIMER_Globel_Enable_Cmd(__STATE__);
            __STATE__ :
                TR0E / TR1E / TR2E / TR3E / TR4E / TR2LE / TR3LE / TR4LE

         TIMER Globel Reload Control
            |----TIMER Globel Reload Control : __DRV_TIMER_Globel_Reload_Cmd(__STATE__);
            __STATE__ :
                T0RLC / T1RLC / T2RLC / T3RLC / T4RLC / TL2RLC / TL3RLC / TL4RLC

         TIMER Globel Stop Control
            |----TIMER Globel Stop Control : __DRV_TIMER_Globel_Stop_Cmd(__STATE__);
            __STATE__ :
                T0SC / T1SC / T2SC / T3SC / T4SC / TL2SC / TL3SC / TL4SC

    10.Timer Function Access Control
         TIMER2 TF2L,TL2IE,T2EIP Access Control (x = 2)
            |----TIMER2 Set TL2IS : __DRV_TIMER2_Function_Access_Control_Cmd(__STATE__);
            __STATE__ :
                MW_DISABLE  / MW_ENABLE

         TIMERx TLxX12/TxEIP Access Control (x = 3,4)
            |----TIMERx Set TLxIS : __DRV_TIMERx_Function_Access_Control_Cmd(__STATE__);
            __STATE__ :
                MW_DISABLE  / MW_ENABLE

    11.Input Signal Inversion Control (x = 2~4)
        |----TxEXI input signal inversion control bit : __DRV_TIMERx_Input_Signal_Inversion(__STATE__);
            __STATE__ :
                MW_DISABLE  / MW_ENABLE

    *Other C Function
        Interrupt
            |----Get TFx    : DRV_TIMERx_GetTFx(void);    (x =0~6)
            |----Get TFxL   : DRV_TIMERx_GetTFxL(void);   (x =2~4)
            |----Get EXFx   : DRV_TIMERx_GetEXFx(void);   (x =2~4)
            |----Clear TFx  : DRV_TIMERx_ClearTFx(void);  (x =0~6)
            |----Clear TFxL : DRV_TIMERx_ClearTFx(void);  (x =2~4)
            |----Clear EXFx : DRV_TIMERx_ClearEXFx(void); (x =2~4)
            |----Set TFx    : DRV_TIMERx_SetTFx(void);    (x =0~6)
            |----Set TFxL   : DRV_TIMERx_SetTFxL(void);   (x =2~4)
            |----Set EXFx   : DRV_TIMERx_SetEXFx(void);   (x =2~4)

        Timex Read Counter Register Value (x =0~6)
            |----TIMERx Read Counter High Byte : DRV_TIMERx_GetTHx(void);
            |----TIMERx Read Counter Low Byte  : DRV_TIMERx_GetTLx(void);

        Timex Read Capture Register Value (x =2~4)
            |----TIMERx Read Capture High Byte : DRV_TIMERx_GetRCAPxH(void);
            |----TIMERx Read Capture Low Byte  : DRV_TIMERx_GetRCAPxL(void);
--------------------------------------------------------------*/

/**
*****************************************************************************
* @brief        TIMER0 Interrupt
* @details      Enable Disable TIMER0 Interrupt
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER0_IT_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER0_IT_Cmd(__STATE__) \
    MWT(                               \
        __STATE__ == 0 ? (ET0 = 0) : (ET0 = 1);)

/**
*****************************************************************************
* @brief        TIMER1 Interrupt
* @details      Enable Disable TIMER1 Interrupt
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER1_IT_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER1_IT_Cmd(__STATE__) \
    MWT(                               \
        __STATE__ == 0 ? (ET1 = 0) : (ET1 = 1);)

/**
*****************************************************************************
* @brief        TIMER2 Interrupt
* @details      Enable Disable TIMER2 Interrupt
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_IT_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_IT_Cmd(__STATE__) \
    MWT(                               \
        __STATE__ == 0 ? (ET2 = 0) : (ET2 = 1);)

/**
*****************************************************************************
* @brief        TIMER3 Interrupt
* @details      Enable Disable TIMER3 Interrupt
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_IT_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_IT_Cmd(__STATE__) \
    MWT(                               \
        __STATE__ == 0 ? (EIE2 = EIE2 & (~ET3)) : (EIE2 = EIE2 | ET3);)

/**
*****************************************************************************
* @brief        TIMER4 Interrupt
* @details      Enable Disable TIMER4 Interrupt
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_IT_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_IT_Cmd(__STATE__) \
    MWT(                               \
        __STATE__ == 0 ? (EIE2 = EIE2 & (~ET4)) : (EIE2 = EIE2 | ET4);)

/**
*****************************************************************************
* @brief        TIMER5 Interrupt
* @details      Enable Disable TIMER5 Interrupt
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER5_IT_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER5_IT_Cmd(__STATE__)            \
    MWT(                                          \
        __DRV_SFR_PageIndex(T5CON_Page);          \
        __STATE__ == 0 ? (T5IE = 0) : (T5IE = 1); \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER6 Interrupt
* @details      Enable Disable TIMER6 Interrupt
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER6_IT_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER6_IT_Cmd(__STATE__)            \
    MWT(                                          \
        __DRV_SFR_PageIndex(T6CON_Page);          \
        __STATE__ == 0 ? (T6IE = 0) : (T6IE = 1); \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER2 TF2L Interrupt
* @details      Enable Disable TIMER2 TF2L Interrupt
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         Before set TCLK_TL2IE,please ensure TL2IS=1((__DRV_TIMER2_Access_TF2L_TL2IE_Function_Cmd(MW_ENABLE)))
* @par          Example
* @code
                __DRV_TIMER2_TF2L_IT_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_TF2L_IT_Cmd(__STATE__) \
    MWT(                                    \
        __STATE__ == 0 ? (TCLK_TL2IE = 0) : (TCLK_TL2IE = 1);)

/**
*****************************************************************************
* @brief        TIMER3 TF3L Interrupt
* @details      Enable Disable TIMER3 TF3L Interrupt
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_TF3L_IT_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
    */
#define __DRV_TIMER3_TF3L_IT_Cmd(__STATE__)         \
    MWT(                                            \
        __DRV_SFR_PageIndex(T3CON_Page);            \
        __STATE__ == 0 ? (TL3IE = 0) : (TL3IE = 1); \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 TF4L Interrupt
* @details      Enable Disable TIMER4 TF4L Interrupt
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_TF4L_IT_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
    */
#define __DRV_TIMER4_TF4L_IT_Cmd(__STATE__)         \
    MWT(                                            \
        __DRV_SFR_PageIndex(T4CON_Page);            \
        __STATE__ == 0 ? (TL4IE = 0) : (TL4IE = 1); \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER0 Run Control
* @details      Enable Disable TIMER0 RUN
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER0_Run_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/

#define __DRV_TIMER0_Run_Cmd(__STATE__) \
    MWT(                                \
        __STATE__ == 0 ? (TR0 = 0) : (TR0 = 1);)

/**
*****************************************************************************
* @brief        TIMER1 Run Control
* @details      Enable Disable TIMER1 RUN
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER1_Run_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER1_Run_Cmd(__STATE__) \
    MWT(                                \
        __STATE__ == 0 ? (TR1 = 0) : (TR1 = 1);)

/**
*****************************************************************************
* @brief        TIMER2 Run Control
* @details      Enable Disable TIMER2 RUN
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_Run_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_Run_Cmd(__STATE__) \
    MWT(                                \
        __STATE__ == 0 ? (TR2 = 0) : (TR2 = 1);)

/**
*****************************************************************************
* @brief        TIMER3 Run Control
* @details      Enable Disable TIMER3 RUN
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_Run_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_Run_Cmd(__STATE__)         \
    MWT(                                        \
        __DRV_SFR_PageIndex(T3CON_Page);        \
        __STATE__ == 0 ? (TR3 = 0) : (TR3 = 1); \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 Run Control
* @details      Enable Disable TIMER4 RUN
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_Run_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_Run_Cmd(__STATE__)         \
    MWT(                                        \
        __DRV_SFR_PageIndex(T4CON_Page);        \
        __STATE__ == 0 ? (TR4 = 0) : (TR4 = 1); \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER5 Run Control
* @details      Enable Disable TIMER5 RUN
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER5_Run_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER5_Run_Cmd(__STATE__)         \
    MWT(                                        \
        __DRV_SFR_PageIndex(T5CON_Page);        \
        __STATE__ == 0 ? (TR5 = 0) : (TR5 = 1); \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER6 Run Control
* @details      Enable Disable TIMER6 RUN
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER6_Run_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER6_Run_Cmd(__STATE__)         \
    MWT(                                        \
        __DRV_SFR_PageIndex(T6CON_Page);        \
        __STATE__ == 0 ? (TR6 = 0) : (TR6 = 1); \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER0 Mode
* @details      Set T0M1 T0M0
* @param[in]    __MODE__ :
*  @arg\b       TIMER0_MODE0_8BIT_PWM
*  @arg\b       TIMER0_MODE1_16BIT_TIMER
*  @arg\b       TIMER0_MODE2_8BIT_AUTORELOAD
*  @arg\b       TIMER0_MODE3_TWO_8BIT
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_Timer0_Mode_Select(TIMER0_MODE0_8BIT_PWM)
* @endcode
*******************************************************************************
*/
///@cond __DRV_Timer0_Mode_Select
#define TIMER0_MODE0_8BIT_PWM 0x00
#define TIMER0_MODE1_16BIT_TIMER 0x01
#define TIMER0_MODE2_8BIT_AUTORELOAD 0x02
#define TIMER0_MODE3_TWO_8BIT 0x03
///@endcond
#define __DRV_TIMER0_Mode_Select(__MODE__) \
    MWT(                                   \
        __DRV_TIMER0_SetT0M1(__MODE__);    \
        __DRV_TIMER0_SetT0M0(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER0 Operating Mode
* @details      Set T0M1
* @param[in]    __MODE__ :
*  @arg\b       TIMER0_MODE0_8BIT_PWM
*  @arg\b       TIMER0_MODE1_16BIT_TIMER
*  @arg\b       TIMER0_MODE2_8BIT_AUTORELOAD
*  @arg\b       TIMER0_MODE3_TWO_8BIT
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER0_SetT0M1(TIMER0_MODE0_8BIT_PWM)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER0_SetT0M1(__MODE__)                                                                      \
    MWT(                                                                                                    \
        (__MODE__ ^ 0x00) == 0 ? (TMOD = TMOD & (~T0M1)) : (__MODE__ ^ 0x01) == 0 ? (TMOD = TMOD & (~T0M1)) \
                                                       : (__MODE__ ^ 0x02) == 0   ? (TMOD = TMOD | (T0M1))  \
                                                       : (__MODE__ ^ 0x03) == 0   ? (TMOD = TMOD | (T0M1))  \
                                                                                  : _nop_();)

/**
*****************************************************************************
* @brief        TIMER0 Operating Mode
* @details      Set T0M0
* @param[in]    __MODE__ :
*  @arg\b       TIMER0_MODE0_8BIT_PWM
*  @arg\b       TIMER0_MODE1_16BIT_TIMER
*  @arg\b       TIMER0_MODE2_8BIT_AUTORELOAD
*  @arg\b       TIMER0_MODE3_TWO_8BIT
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER0_SetT0M0(TIMER0_MODE0_8BIT_PWM)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER0_SetT0M0(__MODE__)                                                                      \
    MWT(                                                                                                    \
        (__MODE__ ^ 0x00) == 0 ? (TMOD = TMOD & (~T0M0)) : (__MODE__ ^ 0x01) == 0 ? (TMOD = TMOD | (T0M0))  \
                                                       : (__MODE__ ^ 0x02) == 0   ? (TMOD = TMOD & (~T0M0)) \
                                                       : (__MODE__ ^ 0x03) == 0   ? (TMOD = TMOD | (T0M0))  \
                                                                                  : _nop_();)

/**
*****************************************************************************
* @brief        TIMER1 Mode
* @details      Set T1M1 T1M0
* @param[in]    __MODE__ :
*  @arg\b       TIMER1_MODE0_8BIT_PWM
*  @arg\b       TIMER1_MODE1_16BIT_TIMER
*  @arg\b       TIMER1_MODE2_8BIT_AUTORELOAD
*  @arg\b       TIMER1_MODE3_TIMER1_STOPPED
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER1_Mode_Select(TIMER1_MODE0_8BIT_PWM)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER1_Mode_Select
#define TIMER1_MODE0_8BIT_PWM 0x00
#define TIMER1_MODE1_16BIT_TIMER 0x01
#define TIMER1_MODE2_8BIT_AUTORELOAD 0x02
#define TIMER1_MODE3_TIMER1_STOPPED 0x03
///@endcond
#define __DRV_TIMER1_Mode_Select(__MODE__) \
    MWT(                                   \
        __DRV_TIMER1_SetT1M1(__MODE__);    \
        __DRV_TIMER1_SetT1M0(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER1 Operating Mode
* @details      Set T1M1
* @param[in]    __MODE__ :
*  @arg\b       TIMER1_MODE0_8BIT_PWM
*  @arg\b       TIMER1_MODE1_16BIT_TIMER
*  @arg\b       TIMER1_MODE2_8BIT_AUTORELOAD
*  @arg\b       TIMER1_MODE3_TIMER1_STOPPED
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER1_SetT1M1(TIMER1_MODE0_8BIT_PWM)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER1_SetT1M1(__MODE__)                                                                      \
    MWT(                                                                                                    \
        (__MODE__ ^ 0x00) == 0 ? (TMOD = TMOD & (~T1M1)) : (__MODE__ ^ 0x01) == 0 ? (TMOD = TMOD & (~T1M1)) \
                                                       : (__MODE__ ^ 0x02) == 0   ? (TMOD = TMOD | (T1M1))  \
                                                       : (__MODE__ ^ 0x03) == 0   ? (TMOD = TMOD | (T1M1))  \
                                                                                  : _nop_();)

/**
*****************************************************************************
* @brief        TIMER1 Operating Mode
* @details      Set T1M0
* @param[in]    __MODE__ :
*  @arg\b       TIMER1_MODE0_8BIT_PWM
*  @arg\b       TIMER1_MODE1_16BIT_TIMER
*  @arg\b       TIMER1_MODE2_8BIT_AUTORELOAD
*  @arg\b       TIMER1_MODE3_TIMER1_STOPPED
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER1_SetT1M0(TIMER1_MODE0_8BIT_PWM)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER1_SetT1M0(__MODE__)                                                                      \
    MWT(                                                                                                    \
        (__MODE__ ^ 0x00) == 0 ? (TMOD = TMOD & (~T1M0)) : (__MODE__ ^ 0x01) == 0 ? (TMOD = TMOD | (T1M0))  \
                                                       : (__MODE__ ^ 0x02) == 0   ? (TMOD = TMOD & (~T1M0)) \
                                                       : (__MODE__ ^ 0x03) == 0   ? (TMOD = TMOD | (T1M0))  \
                                                                                  : _nop_();)

/**
*****************************************************************************
* @brief        TIMER0 Clock Source
* @details      Set T0XL T0X12 T0C_T
* @param[in]    __MODE__ :
*  @arg\b       TIMER0_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER0_CLOCK_SOURCE_T0_PIN
*  @arg\b       TIMER0_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER0_CLOCK_SOURCE_ILRCO
*  @arg\b       TIMER0_CLOCK_SOURCE_SYSCLK_DIV_48
*  @arg\b       TIMER0_CLOCK_SOURCE_WDTPS
*  @arg\b       TIMER0_CLOCK_SOURCE_SYSCLK_DIV_192
*  @arg\b       TIMER0_CLOCK_SOURCE_T1OF
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER0_Clock_Source_Select(TIMER0_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER0_Clock_Source_Select
#define TIMER0_CLOCK_SOURCE_SYSCLK_DIV_12 0x00
#define TIMER0_CLOCK_SOURCE_T0_PIN 0x01
#define TIMER0_CLOCK_SOURCE_SYSCLK 0x02
#define TIMER0_CLOCK_SOURCE_ILRCO 0x03
#define TIMER0_CLOCK_SOURCE_SYSCLK_DIV_48 0x04
#define TIMER0_CLOCK_SOURCE_WDTPS 0x05
#define TIMER0_CLOCK_SOURCE_SYSCLK_DIV_192 0x06
#define TIMER0_CLOCK_SOURCE_T1OF 0x07
///@endcond
#define __DRV_TIMER0_Clock_Source_Select(__MODE__) \
    MWT(                                           \
        __DRV_TIMER0_SetT0XL(__MODE__);            \
        __DRV_TIMER0_SetT0X12(__MODE__);           \
        __DRV_TIMER0_SetT0C_T(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER0 Clock Source
* @details      Set T0XL
* @param[in]    __MODE__ :
*  @arg\b       TIMER0_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER0_CLOCK_SOURCE_T0_PIN
*  @arg\b       TIMER0_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER0_CLOCK_SOURCE_ILRCO
*  @arg\b       TIMER0_CLOCK_SOURCE_SYSCLK_DIV_48
*  @arg\b       TIMER0_CLOCK_SOURCE_WDTPS
*  @arg\b       TIMER0_CLOCK_SOURCE_SYSCLK_DIV_192
*  @arg\b       TIMER0_CLOCK_SOURCE_T1OF
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER0_SetT0XL(TIMER0_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER0_SetT0XL(__MODE__)                                                                          \
    MWT(                                                                                                        \
        (__MODE__ ^ 0x00) == 0 ? (AUXR3 = AUXR3 & (~T0XL)) : (__MODE__ ^ 0x01) == 0 ? (AUXR3 = AUXR3 & (~T0XL)) \
                                                         : (__MODE__ ^ 0x02) == 0   ? (AUXR3 = AUXR3 & (~T0XL)) \
                                                         : (__MODE__ ^ 0x03) == 0   ? (AUXR3 = AUXR3 & (~T0XL)) \
                                                         : (__MODE__ ^ 0x04) == 0   ? (AUXR3 = AUXR3 | (T0XL))  \
                                                         : (__MODE__ ^ 0x05) == 0   ? (AUXR3 = AUXR3 | (T0XL))  \
                                                         : (__MODE__ ^ 0x06) == 0   ? (AUXR3 = AUXR3 | (T0XL))  \
                                                         : (__MODE__ ^ 0x07) == 0   ? (AUXR3 = AUXR3 | (T0XL))  \
                                                                                    : _nop_();)

/**
*****************************************************************************
* @brief        TIMER0 Clock Source
* @details      Set T0X12
* @param[in]    __MODE__ :
*  @arg\b       TIMER0_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER0_CLOCK_SOURCE_T0_PIN
*  @arg\b       TIMER0_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER0_CLOCK_SOURCE_ILRCO
*  @arg\b       TIMER0_CLOCK_SOURCE_SYSCLK_DIV_48
*  @arg\b       TIMER0_CLOCK_SOURCE_WDTPS
*  @arg\b       TIMER0_CLOCK_SOURCE_SYSCLK_DIV_192
*  @arg\b       TIMER0_CLOCK_SOURCE_T1OF
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER0_SetT0X12(TIMER0_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER0_SetT0X12(__MODE__)                                                                           \
    MWT(                                                                                                          \
        (__MODE__ ^ 0x00) == 0 ? (AUXR2 = AUXR2 & (~T0X12)) : (__MODE__ ^ 0x01) == 0 ? (AUXR2 = AUXR2 & (~T0X12)) \
                                                          : (__MODE__ ^ 0x02) == 0   ? (AUXR2 = AUXR2 | (T0X12))  \
                                                          : (__MODE__ ^ 0x03) == 0   ? (AUXR2 = AUXR2 | (T0X12))  \
                                                          : (__MODE__ ^ 0x04) == 0   ? (AUXR2 = AUXR2 & (~T0X12)) \
                                                          : (__MODE__ ^ 0x05) == 0   ? (AUXR2 = AUXR2 & (~T0X12)) \
                                                          : (__MODE__ ^ 0x06) == 0   ? (AUXR2 = AUXR2 | (T0X12))  \
                                                          : (__MODE__ ^ 0x07) == 0   ? (AUXR2 = AUXR2 | (T0X12))  \
                                                                                     : _nop_();)

/**
*****************************************************************************
* @brief        TIMER0 Clock Source
* @details      Set T0C_T
* @param[in]    __MODE__ :
*  @arg\b       TIMER0_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER0_CLOCK_SOURCE_T0_PIN
*  @arg\b       TIMER0_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER0_CLOCK_SOURCE_ILRCO
*  @arg\b       TIMER0_CLOCK_SOURCE_SYSCLK_DIV_48
*  @arg\b       TIMER0_CLOCK_SOURCE_WDTPS
*  @arg\b       TIMER0_CLOCK_SOURCE_SYSCLK_DIV_192
*  @arg\b       TIMER0_CLOCK_SOURCE_T1OF
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER0_SetT0C_T(TIMER0_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER0_SetT0C_T(__MODE__)                                                                       \
    MWT(                                                                                                      \
        (__MODE__ ^ 0x00) == 0 ? (TMOD = TMOD & (~T0C_T)) : (__MODE__ ^ 0x01) == 0 ? (TMOD = TMOD | (T0C_T))  \
                                                        : (__MODE__ ^ 0x02) == 0   ? (TMOD = TMOD & (~T0C_T)) \
                                                        : (__MODE__ ^ 0x03) == 0   ? (TMOD = TMOD | (T0C_T))  \
                                                        : (__MODE__ ^ 0x04) == 0   ? (TMOD = TMOD & (~T0C_T)) \
                                                        : (__MODE__ ^ 0x05) == 0   ? (TMOD = TMOD | (T0C_T))  \
                                                        : (__MODE__ ^ 0x06) == 0   ? (TMOD = TMOD & (~T0C_T)) \
                                                        : (__MODE__ ^ 0x07) == 0   ? (TMOD = TMOD | (T0C_T))  \
                                                                                   : _nop_();)

/**
*****************************************************************************
* @brief        TIMER1 Clock Source
* @details      Set  T1X12 T1C_T
* @param[in]    __MODE__ :
*  @arg\b       TIMER1_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER1_CLOCK_SOURCE_T1_PIN
*  @arg\b       TIMER1_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER1_CLOCK_SOURCE_SYSCLK_DIV_48
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER1_Clock_Source_Select(TIMER1_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER1_Clock_Source_Select
#define TIMER1_CLOCK_SOURCE_SYSCLK_DIV_12 0x00
#define TIMER1_CLOCK_SOURCE_T1_PIN 0x01
#define TIMER1_CLOCK_SOURCE_SYSCLK 0x02
#define TIMER1_CLOCK_SOURCE_SYSCLK_DIV_48 0x03
///@endcond
#define __DRV_TIMER1_Clock_Source_Select(__MODE__) \
    MWT(                                           \
        __DRV_TIMER1_SetT1X12(__MODE__);           \
        __DRV_TIMER1_SetT1C_T(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER1 Clock Source
* @details      Set T1X12
* @param[in]    __MODE__ :
*  @arg\b       TIMER0_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER0_CLOCK_SOURCE_T1_PIN
*  @arg\b       TIMER0_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER0_CLOCK_SOURCE_SYSCLK_DIV_48
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER1_SetT1X12(TIMER1_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER1_SetT1X12(__MODE__)                                                                           \
    MWT(                                                                                                          \
        (__MODE__ ^ 0x00) == 0 ? (AUXR2 = AUXR2 & (~T1X12)) : (__MODE__ ^ 0x01) == 0 ? (AUXR2 = AUXR2 & (~T1X12)) \
                                                          : (__MODE__ ^ 0x02) == 0   ? (AUXR2 = AUXR2 | (T1X12))  \
                                                          : (__MODE__ ^ 0x03) == 0   ? (AUXR2 = AUXR2 | (T1X12))  \
                                                                                     : _nop_();)

/**
*****************************************************************************
* @brief        TIMER1 Clock Source
* @details      Set T1C_T
* @param[in]    __MODE__ :
*  @arg\b       TIMER0_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER0_CLOCK_SOURCE_T1_PIN
*  @arg\b       TIMER0_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER0_CLOCK_SOURCE_SYSCLK_DIV_48
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER1_SetT1C_T(TIMER1_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER1_SetT1C_T(__MODE__)                                                                       \
    MWT(                                                                                                      \
        (__MODE__ ^ 0x00) == 0 ? (TMOD = TMOD & (~T1C_T)) : (__MODE__ ^ 0x01) == 0 ? (TMOD = TMOD | (T1C_T))  \
                                                        : (__MODE__ ^ 0x02) == 0   ? (TMOD = TMOD & (~T1C_T)) \
                                                        : (__MODE__ ^ 0x03) == 0   ? (TMOD = TMOD | (T1C_T))  \
                                                                                   : _nop_();)

/**
*****************************************************************************
* @brief        TIMER5 Gating Source
* @details      Set T5GAT1 T5GAT0
* @param[in]    __MODE__ :
*  @arg\b       TIMER5_GATING_SOURCE_DISABLE
*  @arg\b       TIMER5_GATING_SOURCE_INT0ET
*  @arg\b       TIMER5_GATING_SOURCE_INT2ET
*  @arg\b       TIMER5_GATING_SOURCE_T3EXI
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER5_Gating_Source_Select(TIMER5_GATING_SOURCE_DISABLE)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER5_Gating_Source_Select
#define TIMER5_GATING_SOURCE_DISABLE 0x00
#define TIMER5_GATING_SOURCE_INT0ET 0x01
#define TIMER5_GATING_SOURCE_INT2ET 0x02
#define TIMER5_GATING_SOURCE_T3EXI 0x03
///@endcond
#define __DRV_TIMER5_Gating_Source_Select(__MODE__) \
    MWT(                                            \
        __DRV_TIMER5_SetT5GAT1(__MODE__);           \
        __DRV_TIMER5_SetT5GAT0(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER5 Gating Source
* @details      Set T5GAT1
* @param[in]    __MODE__ :
*  @arg\b       TIMER5_GATING_SOURCE_DISABLE
*  @arg\b       TIMER5_GATING_SOURCE_INT0ET
*  @arg\b       TIMER5_GATING_SOURCE_INT2ET
*  @arg\b       TIMER5_GATING_SOURCE_T3EXI
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER5_SetT5GAT1(TIMER5_GATING_SOURCE_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER5_SetT5GAT1(__MODE__)                                               \
    MWT(                                                                               \
        __DRV_SFR_PageIndex(T5CON_Page);                                               \
        ((__MODE__ ^ 0x00) == 0 ? (T5GAT1 = 0) : (__MODE__ ^ 0x01) == 0 ? (T5GAT1 = 0) \
                                             : (__MODE__ ^ 0x02) == 0   ? (T5GAT1 = 1) \
                                             : (__MODE__ ^ 0x03) == 0   ? (T5GAT1 = 1) \
                                                                        : _nop_());      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER5 Gating Source
* @details      Set T5GAT0
* @param[in]    __MODE__ :
*  @arg\b       TIMER5_GATING_SOURCE_DISABLE
*  @arg\b       TIMER5_GATING_SOURCE_INT0ET
*  @arg\b       TIMER5_GATING_SOURCE_INT2ET
*  @arg\b       TIMER5_GATING_SOURCE_T3EXI
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER5_SetT5GAT0(TIMER5_GATING_SOURCE_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER5_SetT5GAT0(__MODE__)                                               \
    MWT(                                                                               \
        __DRV_SFR_PageIndex(T5CON_Page);                                               \
        ((__MODE__ ^ 0x00) == 0 ? (T5GAT0 = 0) : (__MODE__ ^ 0x01) == 0 ? (T5GAT0 = 1) \
                                             : (__MODE__ ^ 0x02) == 0   ? (T5GAT0 = 0) \
                                             : (__MODE__ ^ 0x03) == 0   ? (T5GAT0 = 1) \
                                                                        : _nop_());      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER6 Gating Source
* @details      Set T6GAT1 T6GAT0
* @param[in]    __MODE__ :
*  @arg\b       TIMER6_GATING_SOURCE_DISABLE
*  @arg\b       TIMER6_GATING_SOURCE_INT1ET
*  @arg\b       TIMER6_GATING_SOURCE_INT2ET
*  @arg\b       TIMER6_GATING_SOURCE_T2EXI
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER6_Gating_Source_Select(TIMER6_GATING_SOURCE_DISABLE)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER6_Gating_Source_Select
#define TIMER6_GATING_SOURCE_DISABLE 0x00
#define TIMER6_GATING_SOURCE_INT1ET 0x01
#define TIMER6_GATING_SOURCE_INT2ET 0x02
#define TIMER6_GATING_SOURCE_T2EXI 0x03
///@endcond
#define __DRV_TIMER6_Gating_Source_Select(__MODE__) \
    MWT(                                            \
        __DRV_TIMER6_SetT6GAT1(__MODE__);           \
        __DRV_TIMER6_SetT6GAT0(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER6 Gating Source
* @details      Set T6GAT1
* @param[in]    __MODE__ :
*  @arg\b       TIMER6_GATING_SOURCE_DISABLE
*  @arg\b       TIMER6_GATING_SOURCE_INT1ET
*  @arg\b       TIMER6_GATING_SOURCE_INT2ET
*  @arg\b       TIMER6_GATING_SOURCE_T2EXI
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER6_SetT6GAT1(TIMER6_GATING_SOURCE_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER6_SetT6GAT1(__MODE__)                                               \
    MWT(                                                                               \
        __DRV_SFR_PageIndex(T6CON_Page);                                               \
        ((__MODE__ ^ 0x00) == 0 ? (T6GAT1 = 0) : (__MODE__ ^ 0x01) == 0 ? (T6GAT1 = 0) \
                                             : (__MODE__ ^ 0x02) == 0   ? (T6GAT1 = 1) \
                                             : (__MODE__ ^ 0x03) == 0   ? (T6GAT1 = 1) \
                                                                        : _nop_());      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER6 Gating Source
* @details      Set T6GAT0
* @param[in]    __MODE__ :
*  @arg\b       TIMER6_GATING_SOURCE_DISABLE
*  @arg\b       TIMER6_GATING_SOURCE_INT1ET
*  @arg\b       TIMER6_GATING_SOURCE_INT2ET
*  @arg\b       TIMER6_GATING_SOURCE_T2EXI
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER6_SetT6GAT0(TIMER6_GATING_SOURCE_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER6_SetT6GAT0(__MODE__)                                               \
    MWT(                                                                               \
        __DRV_SFR_PageIndex(T6CON_Page);                                               \
        ((__MODE__ ^ 0x00) == 0 ? (T6GAT0 = 0) : (__MODE__ ^ 0x01) == 0 ? (T6GAT0 = 1) \
                                             : (__MODE__ ^ 0x02) == 0   ? (T6GAT0 = 0) \
                                             : (__MODE__ ^ 0x03) == 0   ? (T6GAT0 = 1) \
                                                                        : _nop_());      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER0 Gating Source
* @details      Set T0G1 T0GATE
* @param[in]    __MODE__ :
*  @arg\b       TIMER0_GATING_SOURCE_DISABLE
*  @arg\b       TIMER0_GATING_SOURCE_INT0ET
*  @arg\b       TIMER0_GATING_SOURCE_TF2
*  @arg\b       TIMER0_GATING_SOURCE_KBIET
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER0_Gating_Source_Select(TIMER0_GATING_SOURCE_DISABLE)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER0_Gating_Source_Select
#define TIMER0_GATING_SOURCE_DISABLE 0x00
#define TIMER0_GATING_SOURCE_INT0ET 0x01
#define TIMER0_GATING_SOURCE_TF2 0x02
#define TIMER0_GATING_SOURCE_KBIET 0x03
///@endcond
#define __DRV_TIMER0_Gating_Source_Select(__MODE__) \
    MWT(                                            \
        __DRV_TIMER0_SetT0G1(__MODE__);             \
        __DRV_TIMER0_SetT0GATE(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER0 Gating Source
* @details      Set T0G1
* @param[in]    __MODE__ :
*  @arg\b       TIMER0_GATING_SOURCE_DISABLE
*  @arg\b       TIMER0_GATING_SOURCE_INT0ET
*  @arg\b       TIMER0_GATING_SOURCE_TF2
*  @arg\b       TIMER0_GATING_SOURCE_KBIET
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER0_SetT0G1(TIMER0_GATING_SOURCE_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER0_SetT0G1(__MODE__)                                                                           \
    MWT(                                                                                                         \
        __DRV_SFR_PageIndex(AUXR9_Page);                                                                         \
        ((__MODE__ ^ 0x00) == 0 ? (AUXR9 = AUXR9 & (~T0G1)) : (__MODE__ ^ 0x01) == 0 ? (AUXR9 = AUXR9 & (~T0G1)) \
                                                          : (__MODE__ ^ 0x02) == 0   ? (AUXR9 = AUXR9 | (T0G1))  \
                                                          : (__MODE__ ^ 0x03) == 0   ? (AUXR9 = AUXR9 | (T0G1))  \
                                                                                     : _nop_());                   \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER0 Gating Source
* @details      Set T0GATE
* @param[in]    __MODE__ :
*  @arg\b       TIMER0_GATING_SOURCE_DISABLE
*  @arg\b       TIMER0_GATING_SOURCE_INT0ET
*  @arg\b       TIMER0_GATING_SOURCE_TF2
*  @arg\b       TIMER0_GATING_SOURCE_KBIET
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER0_SetT0GATE(TIMER0_GATING_SOURCE_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER0_SetT0GATE(__MODE__)                                                                        \
    MWT(                                                                                                        \
        (__MODE__ ^ 0x00) == 0 ? (TMOD = TMOD & (~T0GATE)) : (__MODE__ ^ 0x01) == 0 ? (TMOD = TMOD | (T0GATE))  \
                                                         : (__MODE__ ^ 0x02) == 0   ? (TMOD = TMOD & (~T0GATE)) \
                                                         : (__MODE__ ^ 0x03) == 0   ? (TMOD = TMOD | (T0GATE))  \
                                                                                    : _nop_();)

/**
*****************************************************************************
* @brief        TIMER1 Gating Source
* @details      Set T1G1 T1GATE
* @param[in]    __MODE__ :
*  @arg\b       TIMER1_GATING_SOURCE_DISABLE
*  @arg\b       TIMER1_GATING_SOURCE_INT1ET
*  @arg\b       TIMER1_GATING_SOURCE_TF3
*  @arg\b       TIMER1_GATING_SOURCE_TI1
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER1_Gating_Source_Select(TIMER1_GATING_SOURCE_DISABLE)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER1_Gating_Source_Select
#define TIMER1_GATING_SOURCE_DISABLE 0x00
#define TIMER1_GATING_SOURCE_INT1ET 0x01
#define TIMER1_GATING_SOURCE_TF3 0x02
#define TIMER1_GATING_SOURCE_TI1 0x03
///@endcond
#define __DRV_TIMER1_Gating_Source_Select(__MODE__) \
    MWT(                                            \
        __DRV_TIMER1_SetT1G1(__MODE__);             \
        __DRV_TIMER1_SetT1GATE(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER1 Gating Source
* @details      Set T1G1
* @param[in]    __MODE__ :
*  @arg\b       TIMER1_GATING_SOURCE_DISABLE
*  @arg\b       TIMER1_GATING_SOURCE_INT1ET
*  @arg\b       TIMER1_GATING_SOURCE_TF3
*  @arg\b       TIMER1_GATING_SOURCE_TI1
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER1_SetT1G1(TIMER1_GATING_SOURCE_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER1_SetT1G1(__MODE__)                                                                           \
    MWT(                                                                                                         \
        __DRV_SFR_PageIndex(AUXR9_Page);                                                                         \
        ((__MODE__ ^ 0x00) == 0 ? (AUXR9 = AUXR9 & (~T1G1)) : (__MODE__ ^ 0x01) == 0 ? (AUXR9 = AUXR9 & (~T1G1)) \
                                                          : (__MODE__ ^ 0x02) == 0   ? (AUXR9 = AUXR9 | (T1G1))  \
                                                          : (__MODE__ ^ 0x03) == 0   ? (AUXR9 = AUXR9 | (T1G1))  \
                                                                                     : _nop_());                   \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER1 Gating Source
* @details      Set T1GATE
* @param[in]    __MODE__ :
*  @arg\b       TIMER1_GATING_SOURCE_DISABLE
*  @arg\b       TIMER1_GATING_SOURCE_INT1ET
*  @arg\b       TIMER1_GATING_SOURCE_TF3
*  @arg\b       TIMER1_GATING_SOURCE_TI1
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER1_SetT1GATE(TIMER1_GATING_SOURCE_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER1_SetT1GATE(__MODE__)                                                                        \
    MWT(                                                                                                        \
        (__MODE__ ^ 0x00) == 0 ? (TMOD = TMOD & (~T1GATE)) : (__MODE__ ^ 0x01) == 0 ? (TMOD = TMOD | (T1GATE))  \
                                                         : (__MODE__ ^ 0x02) == 0   ? (TMOD = TMOD & (~T1GATE)) \
                                                         : (__MODE__ ^ 0x03) == 0   ? (TMOD = TMOD | (T1GATE))  \
                                                                                    : _nop_();)

/**
*******************************************************************************
* @brief        TIMER0 High Byte Register
* @details      Set TH0
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER0_SetHighByte(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER0_SetHighByte(__RELOAD__) TH0 = __RELOAD__

/**
*******************************************************************************
* @brief        TIMER0 Low Byte Register
* @details      Set TL0
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER0_SetLowByte(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER0_SetLowByte(__RELOAD__) TL0 = __RELOAD__

/**
*******************************************************************************
* @brief        TIMER0 16 Bit Timer/Counter
* @details      Set TH0 TL0
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER0_Set16BitCounter(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER0_Set16BitCounter(__RELOAD__) \
    MWT(                                         \
        TH0 = HIBYTE(__RELOAD__);                \
        TL0 = LOBYTE(__RELOAD__) & 0x00ff;)

/**
*******************************************************************************
* @brief        TIMER0 8Bit Timer/Counter Auto Reload
* @details      Set TL0 TH0
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER0_Set8BitIntervalAutoReload(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER0_Set8BitIntervalAutoReload(__RELOAD__) TL0 = TH0 = __RELOAD__

/**
*******************************************************************************
* @brief        TIMER1 High Byte Register
* @details      Set TH1
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER1_SetHighByte(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER1_SetHighByte(__RELOAD__) TH1 = __RELOAD__

/**
*******************************************************************************
* @brief        TIMER1 Low Byte Register
* @details      Set TL1
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER1_SetLowByte(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER1_SetLowByte(__RELOAD__) TL1 = __RELOAD__

/**
*******************************************************************************
* @brief        TIMER1 16 Bit Timer/Counter
* @details      Set TH1 TL1
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER1_Set16BitCounter(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER1_Set16BitCounter(__RELOAD__) \
    MWT(                                         \
        TH1 = HIBYTE(__RELOAD__);                \
        TL1 = LOBYTE(__RELOAD__) & 0x00FF;)

/**
*******************************************************************************
* @brief        TIMER1 8Bit Timer/Counter Auto Reload
* @details      Set TL1 TH1
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER1_Set8BitIntervalAutoReload(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER1_Set8BitIntervalAutoReload(__RELOAD__) TL1 = TH1 = __RELOAD__

/**
*******************************************************************************
* @brief        TIMER0 T0CKO
* @details      Set T0CKOE
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER0_T0CKO_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER0_T0CKO_Cmd(__STATE__) \
    MWT(                                  \
        __STATE__ == 0 ? (AUXR2 = AUXR2 & (~T0CKOE)) : (AUXR2 = AUXR2 | (T0CKOE));)

/**
*******************************************************************************
* @brief        TIMER0 Clock out
* @details      Set T0CKOE
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER0_Clock_Out_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER0_Clock_Out_Cmd(__STATE__) \
    MWT(                                      \
        __STATE__ == 0 ? (AUXR2 = AUXR2 & (~T0CKOE)) : (AUXR2 = AUXR2 | (T0CKOE));)

/**
*******************************************************************************
* @brief        TIMER1 T1CKO
* @details      Set T1CKOE
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER1_T1CKO_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER1_T1CKO_Cmd(__STATE__) \
    MWT(                                  \
        __STATE__ == 0 ? (AUXR2 = AUXR2 & (~T1CKOE)) : (AUXR2 = AUXR2 | (T1CKOE));)

/**
*******************************************************************************
* @brief        TIMER1 Clock out
* @details      Set T1CKOE
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER1_Clock_Out_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER1_Clock_Out_Cmd(__STATE__) \
    MWT(                                      \
        __STATE__ == 0 ? (AUXR2 = AUXR2 & (~T1CKOE)) : (AUXR2 = AUXR2 | (T1CKOE));)

/**
*******************************************************************************
* @brief        TIMER0 T0CKO Pin MUX
* @details      Set T0PS1 T0PS0
* @param[in]    __SELECT__ :
*  @arg\b       TIMER0_T0CKO_P34
*  @arg\b       TIMER0_T0CKO_P44
*  @arg\b       TIMER0_T0CKO_P46
*  @arg\b       TIMER0_T0CKO_P17
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER0_T0CKO_PinMux_Select(TIMER0_T0CKO_P34)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER0_T0CKO_PinMux_Select
#define TIMER0_T0CKO_P34 0x00
#define TIMER0_T0CKO_P44 0x01
#define TIMER0_T0CKO_P46 0x02
#define TIMER0_T0CKO_P17 0x03
///@endcond
#define __DRV_TIMER0_T0CKO_PinMux_Select(__SELECT__) \
    MWT(                                             \
        __DRV_TIMER0_SetT0PS1(__SELECT__);           \
        __DRV_TIMER0_SetT0PS0(__SELECT__);)

/**
*******************************************************************************
* @brief        TIMER0 T0CKO Pin MUX
* @details      Set T0PS1
* @param[in]    __SELECT__ :
*  @arg\b       TIMER0_T0CKO_P34
*  @arg\b       TIMER0_T0CKO_P44
*  @arg\b       TIMER0_T0CKO_P46
*  @arg\b       TIMER0_T0CKO_P17
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER0_SetT0PS1(TIMER0_T0CKO_P34)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER0_SetT0PS1(__SELECT__)                                                                             \
    MWT(                                                                                                              \
        (__SELECT__ ^ 0x00) == 0 ? (AUXR3 = AUXR3 & (~T0PS1)) : (__SELECT__ ^ 0x01) == 0 ? (AUXR3 = AUXR3 & (~T0PS1)) \
                                                            : (__SELECT__ ^ 0x02) == 0   ? (AUXR3 = AUXR3 | (T0PS1))  \
                                                            : (__SELECT__ ^ 0x03) == 0   ? (AUXR3 = AUXR3 | (T0PS1))  \
                                                                                         : _nop_();)

/**
*******************************************************************************
* @brief        TIMER0 T0CKO Pin MUX
* @details      Set T0PS0
* @param[in]    __SELECT__ :
*  @arg\b       TIMER0_T0CKO_P34
*  @arg\b       TIMER0_T0CKO_P44
*  @arg\b       TIMER0_T0CKO_P46
*  @arg\b       TIMER0_T0CKO_P17
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER0_SetT0PS0(TIMER0_T0CKO_P34)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER0_SetT0PS0(__SELECT__)                                                                             \
    MWT(                                                                                                              \
        (__SELECT__ ^ 0x00) == 0 ? (AUXR3 = AUXR3 & (~T0PS0)) : (__SELECT__ ^ 0x01) == 0 ? (AUXR3 = AUXR3 | (T0PS0))  \
                                                            : (__SELECT__ ^ 0x02) == 0   ? (AUXR3 = AUXR3 & (~T0PS0)) \
                                                            : (__SELECT__ ^ 0x03) == 0   ? (AUXR3 = AUXR3 | (T0PS0))  \
                                                                                         : _nop_();)

/**
*******************************************************************************
* @brief        TIMER1 T1CKO Pin MUX
* @details      Set T1PS1 T1PS0
* @param[in]    __SELECT__ :
*  @arg\b       TIMER1_T1CKO_P35
*  @arg\b       TIMER1_T1CKO_P45
*  @arg\b       TIMER1_T1CKO_P36
*  @arg\b       TIMER1_T1CKO_P37
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER1_T1CKO_PinMux_Select(TIMER1_T1CKO_P35)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER1_T1CKO_PinMux_Select
#define TIMER1_T1CKO_P35 0x00
#define TIMER1_T1CKO_P45 0x01
#define TIMER1_T1CKO_P36 0x02
#define TIMER1_T1CKO_P37 0x03
///@endcond
#define __DRV_TIMER1_T1CKO_PinMux_Select(__SELECT__) \
    MWT(                                             \
        __DRV_TIMER1_SetT1PS1(__SELECT__);           \
        __DRV_TIMER1_SetT1PS0(__SELECT__);)

/**
*******************************************************************************
* @brief        TIMER1 T1CKO Pin MUX
* @details      Set T1PS1
* @param[in]    __SELECT__ :
*  @arg\b       TIMER1_T1CKO_P35
*  @arg\b       TIMER1_T1CKO_P45
*  @arg\b       TIMER1_T1CKO_P36
*  @arg\b       TIMER1_T1CKO_P37
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER1_SetT1PS1(TIMER1_T1CKO_P35)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER1_SetT1PS1(__SELECT__)                                                                              \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(AUXR4_Page);                                                                               \
        ((__SELECT__ ^ 0x00) == 0 ? (AUXR4 = AUXR4 & (~T1PS1)) : (__SELECT__ ^ 0x01) == 0 ? (AUXR4 = AUXR4 & (~T1PS1)) \
                                                             : (__SELECT__ ^ 0x02) == 0   ? (AUXR4 = AUXR4 | (T1PS1))  \
                                                             : (__SELECT__ ^ 0x03) == 0   ? (AUXR4 = AUXR4 | (T1PS1))  \
                                                                                          : _nop_());                    \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER1 T1CKO Pin MUX
* @details      Set T1PS0
* @param[in]    __SELECT__ :
*  @arg\b       TIMER1_T1CKO_P35
*  @arg\b       TIMER1_T1CKO_P45
*  @arg\b       TIMER1_T1CKO_P36
*  @arg\b       TIMER1_T1CKO_P37
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER1_SetT1PS0(TIMER1_T1CKO_P35)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER1_SetT1PS0(__SELECT__)                                                                              \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(AUXR4_Page);                                                                               \
        ((__SELECT__ ^ 0x00) == 0 ? (AUXR4 = AUXR4 & (~T1PS0)) : (__SELECT__ ^ 0x01) == 0 ? (AUXR4 = AUXR4 | (T1PS0))  \
                                                             : (__SELECT__ ^ 0x02) == 0   ? (AUXR4 = AUXR4 & (~T1PS0)) \
                                                             : (__SELECT__ ^ 0x03) == 0   ? (AUXR4 = AUXR4 | (T1PS0))  \
                                                                                          : _nop_());                    \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER2 TL2 Run Control
* @details      Set TR2L
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_TL2_Run_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_TL2_Run_Cmd(__STATE__) \
    MWT(                                    \
        __STATE__ == 0 ? (T2MOD = T2MOD & (~TR2L)) : (T2MOD = T2MOD | (TR2L));)

/**
*****************************************************************************
* @brief        TIMER2 TR2L Clear Control (Mode0~Mode3)
* @details      Set TR2LC
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_TR2LC_Clear_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_TR2L_Clear_Cmd(__STATE__) \
    MWT(                                       \
        __STATE__ == 0 ? (T2MOD = T2MOD & (~TR2LC)) : (T2MOD = T2MOD | (TR2LC));)

/**
*****************************************************************************
* @brief        TIMER2 T2CKO Invert Control (Mode4 8 Bit Pwm )
* @details      Set TR2LC
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_T2CKO_Invert_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_T2CKO_Invert_Cmd(__STATE__) \
    MWT(                                         \
        __STATE__ == 0 ? (T2MOD = T2MOD & (~TR2LC)) : (T2MOD = T2MOD | (TR2LC));)

/**
*****************************************************************************
* @brief        TIMER2 Operating Mode
* @details      Set T2SPL T2MS1 CP/RL2 T2MS0
* @param[in]    __MODE__ :
*  @arg\b       TIMER2_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER2_MODE1_16BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER2_MODE2_16BIT_CAPTURE
*  @arg\b       TIMER2_MODE3_16BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER2_MODE6_DUTY_CAPTURE
*  @arg\b       TIMER2_SPLIT_MODE0_TWO_8BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER2_SPLIT_MODE1_TWO_8BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER2_SPLIT_MODE2_TWO_8BIT_CAPTURE
*  @arg\b       TIMER2_SPLIT_MODE3_TWO_8BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER2_SPLIT_MODE4_8BIT_PWM
*  @arg\b       TIMER2_SPLIT_MODE6_DUTY_CAPTURE
*  @arg\b       TIMER2_BAUD_RATE_GENERATOR
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_Mode_Select(TIMER2_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER2_Mode_Select
#define TIMER2_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT 0x00
#define TIMER2_MODE1_16BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT 0x01
#define TIMER2_MODE2_16BIT_CAPTURE 0x02
#define TIMER2_MODE3_16BIT_CAPTURE_WITH_AUTO_ZERO 0x03
#define TIMER2_SPLIT_MODE0_TWO_8BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT 0x04
#define TIMER2_SPLIT_MODE1_TWO_8BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT 0x05
#define TIMER2_SPLIT_MODE2_TWO_8BIT_CAPTURE 0x06
#define TIMER2_SPLIT_MODE3_TWO_8BIT_CAPTURE_WITH_AUTO_ZERO 0x07
#define TIMER2_SPLIT_MODE4_8BIT_PWM 0x08
#define TIMER2_MODE6_DUTY_CAPTURE 0x09
#define TIMER2_SPLIT_MODE6_DUTY_CAPTURE 0x0A
#define TIMER2_BAUD_RATE_GENERATOR 0x0B
///@endcond
#define __DRV_TIMER2_Mode_Select(__MODE__) \
    MWT(                                   \
        __DRV_TIMER2_SetT2SPL(__MODE__);   \
        __DRV_TIMER2_SetT2MS1(__MODE__);   \
        __DRV_TIMER2_SetCP_RL2(__MODE__);  \
        __DRV_TIMER2_SetT2MS0(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER2 Operating Mode
* @details      Set T2SPL
* @param[in]    __MODE__ :
*  @arg\b       TIMER2_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER2_MODE1_16BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER2_MODE2_16BIT_CAPTURE
*  @arg\b       TIMER2_MODE3_16BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER2_MODE6_DUTY_CAPTURE
*  @arg\b       TIMER2_SPLIT_MODE0_TWO_8BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER2_SPLIT_MODE1_TWO_8BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER2_SPLIT_MODE2_TWO_8BIT_CAPTURE
*  @arg\b       TIMER2_SPLIT_MODE3_TWO_8BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER2_SPLIT_MODE4_8BIT_PWM
*  @arg\b       TIMER2_SPLIT_MODE6_DUTY_CAPTURE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_SetT2SPL(TIMER2_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_SetT2SPL(__MODE__)                                                                           \
    MWT(                                                                                                          \
        (__MODE__ ^ 0x00) == 0 ? (T2MOD = T2MOD & (~T2SPL)) : (__MODE__ ^ 0x01) == 0 ? (T2MOD = T2MOD & (~T2SPL)) \
                                                          : (__MODE__ ^ 0x02) == 0   ? (T2MOD = T2MOD & (~T2SPL)) \
                                                          : (__MODE__ ^ 0x03) == 0   ? (T2MOD = T2MOD & (~T2SPL)) \
                                                          : (__MODE__ ^ 0x04) == 0   ? (T2MOD = T2MOD | (T2SPL))  \
                                                          : (__MODE__ ^ 0x05) == 0   ? (T2MOD = T2MOD | (T2SPL))  \
                                                          : (__MODE__ ^ 0x06) == 0   ? (T2MOD = T2MOD | (T2SPL))  \
                                                          : (__MODE__ ^ 0x07) == 0   ? (T2MOD = T2MOD | (T2SPL))  \
                                                          : (__MODE__ ^ 0x08) == 0   ? (T2MOD = T2MOD | (T2SPL))  \
                                                          : (__MODE__ ^ 0x09) == 0   ? (T2MOD = T2MOD & (~T2SPL)) \
                                                          : (__MODE__ ^ 0x0A) == 0   ? (T2MOD = T2MOD | (T2SPL))  \
                                                          : (__MODE__ ^ 0x0B) == 0   ? (T2MOD = T2MOD & (~T2SPL)) \
                                                                                     : _nop_();)

/**
*****************************************************************************
* @brief        TIMER2 Operating Mode
* @details      Set T2MS1
* @param[in]    __MODE__ :
*  @arg\b       TIMER2_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER2_MODE1_16BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER2_MODE2_16BIT_CAPTURE
*  @arg\b       TIMER2_MODE3_16BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER2_MODE6_DUTY_CAPTURE
*  @arg\b       TIMER2_SPLIT_MODE0_TWO_8BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER2_SPLIT_MODE1_TWO_8BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER2_SPLIT_MODE2_TWO_8BIT_CAPTURE
*  @arg\b       TIMER2_SPLIT_MODE3_TWO_8BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER2_SPLIT_MODE4_8BIT_PWM
*  @arg\b       TIMER2_SPLIT_MODE6_DUTY_CAPTURE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_SetT2MS1(TIMER2_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_SetT2MS1(__MODE__)                                                                                \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(T2MOD1_Page);                                                                              \
        ((__MODE__ ^ 0x00) == 0 ? (T2MOD1 = T2MOD1 & (~T2MS1)) : (__MODE__ ^ 0x01) == 0 ? (T2MOD1 = T2MOD1 & (~T2MS1)) \
                                                             : (__MODE__ ^ 0x02) == 0   ? (T2MOD1 = T2MOD1 & (~T2MS1)) \
                                                             : (__MODE__ ^ 0x03) == 0   ? (T2MOD1 = T2MOD1 & (~T2MS1)) \
                                                             : (__MODE__ ^ 0x04) == 0   ? (T2MOD1 = T2MOD1 & (~T2MS1)) \
                                                             : (__MODE__ ^ 0x05) == 0   ? (T2MOD1 = T2MOD1 & (~T2MS1)) \
                                                             : (__MODE__ ^ 0x06) == 0   ? (T2MOD1 = T2MOD1 & (~T2MS1)) \
                                                             : (__MODE__ ^ 0x07) == 0   ? (T2MOD1 = T2MOD1 & (~T2MS1)) \
                                                             : (__MODE__ ^ 0x08) == 0   ? (T2MOD1 = T2MOD1 | (T2MS1))  \
                                                             : (__MODE__ ^ 0x09) == 0   ? (T2MOD1 = T2MOD1 | (T2MS1))  \
                                                             : (__MODE__ ^ 0x0A) == 0   ? (T2MOD1 = T2MOD1 | (T2MS1))  \
                                                             : (__MODE__ ^ 0x0B) == 0   ? (T2MOD1 = T2MOD1 & (~T2MS1)) \
                                                                                        : _nop_());                      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER2 Operating Mode
* @details      Set CP/RL2
* @param[in]    __MODE__ :
*  @arg\b       TIMER2_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER2_MODE1_16BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER2_MODE2_16BIT_CAPTURE
*  @arg\b       TIMER2_MODE3_16BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER2_MODE6_DUTY_CAPTURE
*  @arg\b       TIMER2_SPLIT_MODE0_TWO_8BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER2_SPLIT_MODE1_TWO_8BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER2_SPLIT_MODE2_TWO_8BIT_CAPTURE
*  @arg\b       TIMER2_SPLIT_MODE3_TWO_8BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER2_SPLIT_MODE4_8BIT_PWM
*  @arg\b       TIMER2_SPLIT_MODE6_DUTY_CAPTURE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_SetCP_RL2(TIMER2_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_SetCP_RL2(__MODE__)                                              \
    MWT(                                                                              \
        (__MODE__ ^ 0x00) == 0 ? (CP_RL2 = 0) : (__MODE__ ^ 0x01) == 0 ? (CP_RL2 = 0) \
                                            : (__MODE__ ^ 0x02) == 0   ? (CP_RL2 = 1) \
                                            : (__MODE__ ^ 0x03) == 0   ? (CP_RL2 = 1) \
                                            : (__MODE__ ^ 0x04) == 0   ? (CP_RL2 = 0) \
                                            : (__MODE__ ^ 0x05) == 0   ? (CP_RL2 = 0) \
                                            : (__MODE__ ^ 0x06) == 0   ? (CP_RL2 = 1) \
                                            : (__MODE__ ^ 0x07) == 0   ? (CP_RL2 = 1) \
                                            : (__MODE__ ^ 0x08) == 0   ? (CP_RL2 = 0) \
                                            : (__MODE__ ^ 0x09) == 0   ? (CP_RL2 = 1) \
                                            : (__MODE__ ^ 0x0A) == 0   ? (CP_RL2 = 1) \
                                            : (__MODE__ ^ 0x0B) == 0   ? (CP_RL2 = 0) \
                                                                       : _nop_();)

/**
*****************************************************************************
* @brief        TIMER2 Operating Mode
* @details      Set T2MS0
* @param[in]    __MODE__ :
*  @arg\b       TIMER2_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER2_MODE1_16BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER2_MODE2_16BIT_CAPTURE
*  @arg\b       TIMER2_MODE3_16BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER2_MODE6_DUTY_CAPTURE
*  @arg\b       TIMER2_SPLIT_MODE0_TWO_8BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER2_SPLIT_MODE1_TWO_8BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER2_SPLIT_MODE2_TWO_8BIT_CAPTURE
*  @arg\b       TIMER2_SPLIT_MODE3_TWO_8BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER2_SPLIT_MODE4_8BIT_PWM
*  @arg\b       TIMER2_SPLIT_MODE6_DUTY_CAPTURE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_SetT2MS0(TIMER2_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_SetT2MS0(__MODE__)                                                                           \
    MWT(                                                                                                          \
        (__MODE__ ^ 0x00) == 0 ? (T2MOD = T2MOD & (~T2MS0)) : (__MODE__ ^ 0x01) == 0 ? (T2MOD = T2MOD | (T2MS0))  \
                                                          : (__MODE__ ^ 0x02) == 0   ? (T2MOD = T2MOD & (~T2MS0)) \
                                                          : (__MODE__ ^ 0x03) == 0   ? (T2MOD = T2MOD | (T2MS0))  \
                                                          : (__MODE__ ^ 0x04) == 0   ? (T2MOD = T2MOD & (~T2MS0)) \
                                                          : (__MODE__ ^ 0x05) == 0   ? (T2MOD = T2MOD | (T2MS0))  \
                                                          : (__MODE__ ^ 0x06) == 0   ? (T2MOD = T2MOD & (~T2MS0)) \
                                                          : (__MODE__ ^ 0x07) == 0   ? (T2MOD = T2MOD | (T2MS0))  \
                                                          : (__MODE__ ^ 0x08) == 0   ? (T2MOD = T2MOD & (~T2MS0)) \
                                                          : (__MODE__ ^ 0x09) == 0   ? (T2MOD = T2MOD & (~T2MS0)) \
                                                          : (__MODE__ ^ 0x0A) == 0   ? (T2MOD = T2MOD & (~T2MS0)) \
                                                          : (__MODE__ ^ 0x0B) == 0   ? (T2MOD = T2MOD & (~T2MS0)) \
                                                                                     : _nop_();)

/**
*****************************************************************************
* @brief        TIMER2 Clock Out Control
* @details      Set T2OE
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_Clock_Out_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_Clock_Out_Cmd(__STATE__) \
    MWT(                                      \
        __STATE__ == 0 ? (T2MOD = T2MOD & (~T2OE)) : (T2MOD = T2MOD | (T2OE));)

/**
*****************************************************************************
* @brief        TIMER2 Clock Source
* @details      Set T2CKS T2X12 C/T2
* @param[in]    __MODE__ :
*  @arg\b       TIMER2_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER2_CLOCK_SOURCE_T2_PIN
*  @arg\b       TIMER2_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER2_CLOCK_SOURCE_INT0ET
*  @arg\b       TIMER2_CLOCK_SOURCE_S0TOF
*  @arg\b       TIMER2_CLOCK_SOURCE_T0OF
*  @arg\b       TIMER2_CLOCK_SOURCE_AC0ES
*  @arg\b       TIMER2_CLOCK_SOURCE_KBIET
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_Clock_Source_Select(TIMER2_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER2_Clock_Source_Select
#define TIMER2_CLOCK_SOURCE_SYSCLK_DIV_12 0x00
#define TIMER2_CLOCK_SOURCE_T2_PIN 0x01
#define TIMER2_CLOCK_SOURCE_SYSCLK 0x02
#define TIMER2_CLOCK_SOURCE_INT0ET 0x03
#define TIMER2_CLOCK_SOURCE_S0TOF 0x04
#define TIMER2_CLOCK_SOURCE_T0OF 0x05
#define TIMER2_CLOCK_SOURCE_AC0ES 0x06
#define TIMER2_CLOCK_SOURCE_KBIET 0x07
///@endcond
#define __DRV_TIMER2_Clock_Source_Select(__MODE__) \
    MWT(                                           \
        __DRV_TIMER2_SetT2CKS(__MODE__);           \
        __DRV_TIMER2_SetT2X12(__MODE__);           \
        __DRV_TIMER2_SetC_T2(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER2 Clock Source
* @details      Set T2CKS
* @param[in]    __MODE__ :
*  @arg\b       TIMER2_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER2_CLOCK_SOURCE_T2_PIN
*  @arg\b       TIMER2_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER2_CLOCK_SOURCE_INT0ET
*  @arg\b       TIMER2_CLOCK_SOURCE_S0TOF
*  @arg\b       TIMER2_CLOCK_SOURCE_T0OF
*  @arg\b       TIMER2_CLOCK_SOURCE_AC0ES
*  @arg\b       TIMER2_CLOCK_SOURCE_KBIET
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_SetT2CKS(TIMER2_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_SetT2CKS(__MODE__)                                                                                \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(T2MOD1_Page);                                                                              \
        ((__MODE__ ^ 0x00) == 0 ? (T2MOD1 = T2MOD1 & (~T2CKS)) : (__MODE__ ^ 0x01) == 0 ? (T2MOD1 = T2MOD1 & (~T2CKS)) \
                                                             : (__MODE__ ^ 0x02) == 0   ? (T2MOD1 = T2MOD1 & (~T2CKS)) \
                                                             : (__MODE__ ^ 0x03) == 0   ? (T2MOD1 = T2MOD1 & (~T2CKS)) \
                                                             : (__MODE__ ^ 0x04) == 0   ? (T2MOD1 = T2MOD1 | (T2CKS))  \
                                                             : (__MODE__ ^ 0x05) == 0   ? (T2MOD1 = T2MOD1 | (T2CKS))  \
                                                             : (__MODE__ ^ 0x06) == 0   ? (T2MOD1 = T2MOD1 | (T2CKS))  \
                                                             : (__MODE__ ^ 0x07) == 0   ? (T2MOD1 = T2MOD1 | (T2CKS))  \
                                                                                        : _nop_());                      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER2 Clock Source
* @details      Set T2X12
* @param[in]    __MODE__ :
*  @arg\b       TIMER2_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER2_CLOCK_SOURCE_T2_PIN
*  @arg\b       TIMER2_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER2_CLOCK_SOURCE_INT0ET
*  @arg\b       TIMER2_CLOCK_SOURCE_S0TOF
*  @arg\b       TIMER2_CLOCK_SOURCE_T0OF
*  @arg\b       TIMER2_CLOCK_SOURCE_AC0ES
*  @arg\b       TIMER2_CLOCK_SOURCE_KBIET
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_SetT2X12(TIMER2_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_SetT2X12(__MODE__)                                                                           \
    MWT(                                                                                                          \
        (__MODE__ ^ 0x00) == 0 ? (T2MOD = T2MOD & (~T2X12)) : (__MODE__ ^ 0x01) == 0 ? (T2MOD = T2MOD & (~T2X12)) \
                                                          : (__MODE__ ^ 0x02) == 0   ? (T2MOD = T2MOD | (T2X12))  \
                                                          : (__MODE__ ^ 0x03) == 0   ? (T2MOD = T2MOD | (T2X12))  \
                                                          : (__MODE__ ^ 0x04) == 0   ? (T2MOD = T2MOD & (~T2X12)) \
                                                          : (__MODE__ ^ 0x05) == 0   ? (T2MOD = T2MOD & (~T2X12)) \
                                                          : (__MODE__ ^ 0x06) == 0   ? (T2MOD = T2MOD | (T2X12))  \
                                                          : (__MODE__ ^ 0x07) == 0   ? (T2MOD = T2MOD | (T2X12))  \
                                                                                     : _nop_();)

/**
*****************************************************************************
* @brief        TIMER2 Clock Source
* @details      Set C/T2
* @param[in]    __MODE__ :
*  @arg\b       TIMER2_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER2_CLOCK_SOURCE_T2_PIN
*  @arg\b       TIMER2_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER2_CLOCK_SOURCE_INT0ET
*  @arg\b       TIMER2_CLOCK_SOURCE_S0TOF
*  @arg\b       TIMER2_CLOCK_SOURCE_T0OF
*  @arg\b       TIMER2_CLOCK_SOURCE_AC0ES
*  @arg\b       TIMER2_CLOCK_SOURCE_KBIET
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_SetC_T2(TIMER2_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_SetC_T2(__MODE__)                                            \
    MWT(                                                                          \
        (__MODE__ ^ 0x00) == 0 ? (C_T2 = 0) : (__MODE__ ^ 0x01) == 0 ? (C_T2 = 1) \
                                          : (__MODE__ ^ 0x02) == 0   ? (C_T2 = 0) \
                                          : (__MODE__ ^ 0x03) == 0   ? (C_T2 = 1) \
                                          : (__MODE__ ^ 0x04) == 0   ? (C_T2 = 0) \
                                          : (__MODE__ ^ 0x05) == 0   ? (C_T2 = 1) \
                                          : (__MODE__ ^ 0x06) == 0   ? (C_T2 = 0) \
                                          : (__MODE__ ^ 0x07) == 0   ? (C_T2 = 1) \
                                                                     : _nop_();)

/**
*****************************************************************************
* @brief        TIMER2 TH2 Clock Source in Split Mode
* @details      Set T2CKS T2X12 C/T2
* @param[in]    __MODE__ :
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_T2_PIN
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_TL2OF
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_S0TOF
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_T0OF
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_AC0ES
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_KBIET
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_TH2_Clock_Source_Select(TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER2_TH2_Clock_Source_Select
#define TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12 0x00
#define TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_T2_PIN 0x01
#define TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK 0x02
#define TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_TL2OF 0x03
#define TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_S0TOF 0x04
#define TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_T0OF 0x05
#define TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_AC0ES 0x06
#define TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_KBIET 0x07
///@endcond
#define __DRV_TIMER2_TH2_Clock_Source_Select(__MODE__) \
    MWT(                                               \
        __DRV_TIMER2_TH2_SetT2CKS(__MODE__);           \
        __DRV_TIMER2_TH2_SetT2X12(__MODE__);           \
        __DRV_TIMER2_TH2_SetC_T2(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER2 TH2 Clock Source in Split Mode
* @details      Set T2CKS
* @param[in]    __MODE__ :
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_T2_PIN
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_TL2OF
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_S0TOF
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_T0OF
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_AC0ES
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_KBIET
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_TH2_SetT2CKS(TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_TH2_SetT2CKS(__MODE__)                                                                            \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(1);                                                                                        \
        ((__MODE__ ^ 0x00) == 0 ? (T2MOD1 = T2MOD1 & (~T2CKS)) : (__MODE__ ^ 0x01) == 0 ? (T2MOD1 = T2MOD1 & (~T2CKS)) \
                                                             : (__MODE__ ^ 0x02) == 0   ? (T2MOD1 = T2MOD1 & (~T2CKS)) \
                                                             : (__MODE__ ^ 0x03) == 0   ? (T2MOD1 = T2MOD1 & (~T2CKS)) \
                                                             : (__MODE__ ^ 0x04) == 0   ? (T2MOD1 = T2MOD1 | (T2CKS))  \
                                                             : (__MODE__ ^ 0x05) == 0   ? (T2MOD1 = T2MOD1 | (T2CKS))  \
                                                             : (__MODE__ ^ 0x06) == 0   ? (T2MOD1 = T2MOD1 | (T2CKS))  \
                                                             : (__MODE__ ^ 0x07) == 0   ? (T2MOD1 = T2MOD1 | (T2CKS))  \
                                                                                        : _nop_());                      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER2 TH2 Clock Source in Split Mode
* @details      Set T2X12
* @param[in]    __MODE__ :
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_T2_PIN
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_TL2OF
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_S0TOF
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_T0OF
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_AC0ES
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_KBIET
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_TH2_SetT2X12(TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_TH2_SetT2X12(__MODE__)                                                                       \
    MWT(                                                                                                          \
        (__MODE__ ^ 0x00) == 0 ? (T2MOD = T2MOD & (~T2X12)) : (__MODE__ ^ 0x01) == 0 ? (T2MOD = T2MOD & (~T2X12)) \
                                                          : (__MODE__ ^ 0x02) == 0   ? (T2MOD = T2MOD | (T2X12))  \
                                                          : (__MODE__ ^ 0x03) == 0   ? (T2MOD = T2MOD | (T2X12))  \
                                                          : (__MODE__ ^ 0x04) == 0   ? (T2MOD = T2MOD & (~T2X12)) \
                                                          : (__MODE__ ^ 0x05) == 0   ? (T2MOD = T2MOD & (~T2X12)) \
                                                          : (__MODE__ ^ 0x06) == 0   ? (T2MOD = T2MOD | (T2X12))  \
                                                          : (__MODE__ ^ 0x07) == 0   ? (T2MOD = T2MOD | (T2X12))  \
                                                                                     : _nop_();)

/**
*****************************************************************************
* @brief        TIMER2 TH2 Clock Source in Split Mode
* @details      Set C/T2
* @param[in]    __MODE__ :
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_T2_PIN
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_TL2OF
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_S0TOF
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_T0OF
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_AC0ES
*  @arg\b       TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_KBIET
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_TH2_SetC_T2(TIMER2_TH2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_TH2_SetC_T2(__MODE__)                                        \
    MWT(                                                                          \
        (__MODE__ ^ 0x00) == 0 ? (C_T2 = 0) : (__MODE__ ^ 0x01) == 0 ? (C_T2 = 1) \
                                          : (__MODE__ ^ 0x02) == 0   ? (C_T2 = 0) \
                                          : (__MODE__ ^ 0x03) == 0   ? (C_T2 = 1) \
                                          : (__MODE__ ^ 0x04) == 0   ? (C_T2 = 0) \
                                          : (__MODE__ ^ 0x05) == 0   ? (C_T2 = 1) \
                                          : (__MODE__ ^ 0x06) == 0   ? (C_T2 = 0) \
                                          : (__MODE__ ^ 0x07) == 0   ? (C_T2 = 1) \
                                                                     : _nop_();)

/**
*****************************************************************************
* @brief        TIMER2 TL2 Clock Source in Split Mode
* @details      Set TL2CS TL2X12
* @param[in]    __MODE__ :
*  @arg\b       TIMER2_TL2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER2_TL2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER2_TL2_SPLIT_MODE_CLOCK_SOURCE_S0TOF
*  @arg\b       TIMER2_TL2_SPLIT_MODE_CLOCK_SOURCE_INT0ET
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_TL2_Clock_Source_Select(TIMER2_TL2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER2_TL2_Clock_Source_Select
#define TIMER2_TL2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12 0x00
#define TIMER2_TL2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK 0x01
#define TIMER2_TL2_SPLIT_MODE_CLOCK_SOURCE_S0TOF 0x02
#define TIMER2_TL2_SPLIT_MODE_CLOCK_SOURCE_INT0ET 0x03
///@endcond
#define __DRV_TIMER2_TL2_Clock_Source_Select(__MODE__) \
    MWT(                                               \
        __DRV_TIMER2_TL2_SetTL2CS(__MODE__);           \
        __DRV_TIMER2_TL2_SetTL2X12(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER2 TL2 Clock Source in Split Mode
* @details      Set TL2CS
* @param[in]    __MODE__ :
*  @arg\b       TIMER2_TL2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER2_TL2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER2_TL2_SPLIT_MODE_CLOCK_SOURCE_S0TOF
*  @arg\b       TIMER2_TL2_SPLIT_MODE_CLOCK_SOURCE_INT0ET
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_TL2_SetTL2CS(TIMER2_TL2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_TL2_SetTL2CS(__MODE__)                                                                            \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(T2MOD1_Page);                                                                              \
        ((__MODE__ ^ 0x00) == 0 ? (T2MOD1 = T2MOD1 & (~TL2CS)) : (__MODE__ ^ 0x01) == 0 ? (T2MOD1 = T2MOD1 & (~TL2CS)) \
                                                             : (__MODE__ ^ 0x02) == 0   ? (T2MOD1 = T2MOD1 | (TL2CS))  \
                                                             : (__MODE__ ^ 0x03) == 0   ? (T2MOD1 = T2MOD1 | (TL2CS))  \
                                                                                        : _nop_());                      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TL2 Clock Source in Split Mode
* @details      Set TL2X12
* @param[in]    __MODE__ :
*  @arg\b       TIMER2_TL2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER2_TL2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER2_TL2_SPLIT_MODE_CLOCK_SOURCE_S0TOF
*  @arg\b       TIMER2_TL2_SPLIT_MODE_CLOCK_SOURCE_INT0ET
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_TL2_SetTL2X12(TIMER2_TL2_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_TL2_SetTL2X12(__MODE__)                                                                                    \
    MWT(                                                                                                                        \
        (__MODE__ ^ 0x00) == 0 ? (T2MOD = T2MOD & (~TL2X12_T2EIP)) : (__MODE__ ^ 0x01) == 0 ? (T2MOD = T2MOD | (TL2X12_T2EIP))  \
                                                                 : (__MODE__ ^ 0x02) == 0   ? (T2MOD = T2MOD & (~TL2X12_T2EIP)) \
                                                                 : (__MODE__ ^ 0x03) == 0   ? (T2MOD = T2MOD | (TL2X12_T2EIP))  \
                                                                                            : _nop_();)

/**
*****************************************************************************
* @brief        TIMER2 TF2L,TL2IE,T2EIP Access Control
* @details      Set TL2IS to enable TF2L and TL2IE access function on T2CON.5~4, T2EIP access on T2MOD.6.
* @details      Clear TL2IS to enable RCLK and TCLK access function on T2CON.5~4, TL2X12 access on T2MOD.6.
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_Function_Access_Control_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_Function_Access_Control_Cmd(__STATE__)                          \
    MWT(                                                                             \
        __DRV_SFR_PageIndex(T2MOD1_Page);                                            \
        __STATE__ == 0 ? (T2MOD1 = T2MOD1 & (~TL2IS)) : (T2MOD1 = T2MOD1 | (TL2IS)); \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER3 TL3X12/T3EIP Access Control
* @details      Set TL3IS
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_Function_Access_Control_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_Function_Access_Control_Cmd(__STATE__)                          \
    MWT(                                                                             \
        __DRV_SFR_PageIndex(T3MOD1_Page);                                            \
        __STATE__ == 0 ? (T3MOD1 = T3MOD1 & (~TL3IS)) : (T3MOD1 = T3MOD1 | (TL3IS)); \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 TL4X12/T4EIP Access Control
* @details      Set TL4IS
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_Function_Access_Control_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_Function_Access_Control_Cmd(__STATE__)                          \
    MWT(                                                                             \
        __DRV_SFR_PageIndex(T4MOD1_Page);                                            \
        __STATE__ == 0 ? (T4MOD1 = T4MOD1 & (~TL4IS)) : (T4MOD1 = T4MOD1 | (TL4IS)); \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER2 Capture Source
* @details      Set CP2S2 CP2S1 CP2S0
* @param[in]    __MODE__ :
*  @arg\b       TIMER2_CAPTURE_SOURCE_T2EX_PIN
*  @arg\b       TIMER2_CAPTURE_SOURCE_RXD0
*  @arg\b       TIMER2_CAPTURE_SOURCE_P60
*  @arg\b       TIMER2_CAPTURE_SOURCE_INT2ET
*  @arg\b       TIMER2_CAPTURE_SOURCE_ILRCO
*  @arg\b       TIMER2_CAPTURE_SOURCE_AC0OUT
*  @arg\b       TIMER2_CAPTURE_SOURCE_KBIET
*  @arg\b       TIMER2_CAPTURE_SOURCE_TWI0_SCL
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_Capture_Source_Select(TIMER2_CAPTURE_SOURCE_T2EX_PIN)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER2_Capture_Source_Select
#define TIMER2_CAPTURE_SOURCE_T2EX_PIN 0x00
#define TIMER2_CAPTURE_SOURCE_RXD0 0x01
#define TIMER2_CAPTURE_SOURCE_P60 0x02
#define TIMER2_CAPTURE_SOURCE_INT2ET 0x03
#define TIMER2_CAPTURE_SOURCE_ILRCO 0x04
#define TIMER2_CAPTURE_SOURCE_AC0OUT 0x05
#define TIMER2_CAPTURE_SOURCE_KBIET 0x06
#define TIMER2_CAPTURE_SOURCE_TWI0_SCL 0x07
///@endcond
#define __DRV_TIMER2_Capture_Source_Select(__MODE__) \
    MWT(                                             \
        __DRV_TIMER2_SetCP2S2(__MODE__);             \
        __DRV_TIMER2_SetCP2S1(__MODE__);             \
        __DRV_TIMER2_SetCP2S0(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER2 Capture Source
* @details      Set CP2S2
* @param[in]    __MODE__ :
*  @arg\b      TIMER2_CAPTURE_SOURCE_T2EX_PIN
*  @arg\b      TIMER2_CAPTURE_SOURCE_RXD0
*  @arg\b      TIMER2_CAPTURE_SOURCE_P60
*  @arg\b      TIMER2_CAPTURE_SOURCE_INT2ET
*  @arg\b      TIMER2_CAPTURE_SOURCE_ILRCO
*  @arg\b      TIMER2_CAPTURE_SOURCE_AC0OUT
*  @arg\b      TIMER2_CAPTURE_SOURCE_KBIET
*  @arg\b      TIMER2_CAPTURE_SOURCE_TWI0_SCL
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_SetCP2S2(TIMER2_CAPTURE_SOURCE_T2EX_PIN)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_SetCP2S2(__MODE__)                                                                                \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(T2MOD1_Page);                                                                              \
        ((__MODE__ ^ 0x00) == 0 ? (T2MOD1 = T2MOD1 & (~CP2S2)) : (__MODE__ ^ 0x01) == 0 ? (T2MOD1 = T2MOD1 & (~CP2S2)) \
                                                             : (__MODE__ ^ 0x02) == 0   ? (T2MOD1 = T2MOD1 & (~CP2S2)) \
                                                             : (__MODE__ ^ 0x03) == 0   ? (T2MOD1 = T2MOD1 & (~CP2S2)) \
                                                             : (__MODE__ ^ 0x04) == 0   ? (T2MOD1 = T2MOD1 | (CP2S2))  \
                                                             : (__MODE__ ^ 0x05) == 0   ? (T2MOD1 = T2MOD1 | (CP2S2))  \
                                                             : (__MODE__ ^ 0x06) == 0   ? (T2MOD1 = T2MOD1 | (CP2S2))  \
                                                             : (__MODE__ ^ 0x07) == 0   ? (T2MOD1 = T2MOD1 | (CP2S2))  \
                                                                                        : _nop_());                      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER2 Capture Source
* @details      Set CP2S1
* @param[in]    __MODE__ :
*  @arg\b       TIMER2_CAPTURE_SOURCE_T2EX_PIN
*  @arg\b       TIMER2_CAPTURE_SOURCE_RXD0
*  @arg\b       TIMER2_CAPTURE_SOURCE_P60
*  @arg\b       TIMER2_CAPTURE_SOURCE_INT2ET
*  @arg\b       TIMER2_CAPTURE_SOURCE_ILRCO
*  @arg\b       TIMER2_CAPTURE_SOURCE_AC0OUT
*  @arg\b       TIMER2_CAPTURE_SOURCE_KBIET
*  @arg\b       TIMER2_CAPTURE_SOURCE_TWI0_SCL
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_SetCP2S1(TIMER2_CAPTURE_SOURCE_T2EX_PIN)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_SetCP2S1(__MODE__)                                                                                \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(T2MOD1_Page);                                                                              \
        ((__MODE__ ^ 0x00) == 0 ? (T2MOD1 = T2MOD1 & (~CP2S1)) : (__MODE__ ^ 0x01) == 0 ? (T2MOD1 = T2MOD1 & (~CP2S1)) \
                                                             : (__MODE__ ^ 0x02) == 0   ? (T2MOD1 = T2MOD1 | (CP2S1))  \
                                                             : (__MODE__ ^ 0x03) == 0   ? (T2MOD1 = T2MOD1 | (CP2S1))  \
                                                             : (__MODE__ ^ 0x04) == 0   ? (T2MOD1 = T2MOD1 & (~CP2S1)) \
                                                             : (__MODE__ ^ 0x05) == 0   ? (T2MOD1 = T2MOD1 & (~CP2S1)) \
                                                             : (__MODE__ ^ 0x06) == 0   ? (T2MOD1 = T2MOD1 | (CP2S1))  \
                                                             : (__MODE__ ^ 0x07) == 0   ? (T2MOD1 = T2MOD1 | (CP2S1))  \
                                                                                        : _nop_());                      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER2 Capture Source
* @details      Set CP2S0
* @param[in]    __MODE__ :
*  @arg\b       TIMER2_CAPTURE_SOURCE_T2EX_PIN
*  @arg\b       TIMER2_CAPTURE_SOURCE_RXD0
*  @arg\b       TIMER2_CAPTURE_SOURCE_P60
*  @arg\b       TIMER2_CAPTURE_SOURCE_INT2ET
*  @arg\b       TIMER2_CAPTURE_SOURCE_ILRCO
*  @arg\b       TIMER2_CAPTURE_SOURCE_AC0OUT
*  @arg\b       TIMER2_CAPTURE_SOURCE_KBIET
*  @arg\b       TIMER2_CAPTURE_SOURCE_TWI0_SCL
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_SetCP2S0(TIMER2_CAPTURE_SOURCE_T2EX_PIN)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_SetCP2S0(__MODE__)                                                                                \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(T2MOD1_Page);                                                                              \
        ((__MODE__ ^ 0x00) == 0 ? (T2MOD1 = T2MOD1 & (~CP2S0)) : (__MODE__ ^ 0x01) == 0 ? (T2MOD1 = T2MOD1 | (CP2S0))  \
                                                             : (__MODE__ ^ 0x02) == 0   ? (T2MOD1 = T2MOD1 & (~CP2S0)) \
                                                             : (__MODE__ ^ 0x03) == 0   ? (T2MOD1 = T2MOD1 | (CP2S0))  \
                                                             : (__MODE__ ^ 0x04) == 0   ? (T2MOD1 = T2MOD1 & (~CP2S0)) \
                                                             : (__MODE__ ^ 0x05) == 0   ? (T2MOD1 = T2MOD1 | (CP2S0))  \
                                                             : (__MODE__ ^ 0x06) == 0   ? (T2MOD1 = T2MOD1 & (~CP2S0)) \
                                                             : (__MODE__ ^ 0x07) == 0   ? (T2MOD1 = T2MOD1 | (CP2S0))  \
                                                                                        : _nop_());                      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER2 Pin Mux
* @details      Set T2PS1 T2PS0
* @param[in]    __MODE__ :
*  @arg\b       TIMER2_PINMUX_T2_T2CKO_P10_T2EX_P11
*  @arg\b       TIMER2_PINMUX_T2_T2CKO_P32_T2EX_P33
*  @arg\b       TIMER2_PINMUX_T2_T2CKO_P67_T2EX_P57
*  @arg\b       TIMER2_PINMUX_T2_T2CKO_P45_T2EX_P44
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_PinMUX_Select(TIMER2_PINMUX_T2_T2CKO_P10_T2EX_P11)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER2_PinMUX_Select
#define TIMER2_PINMUX_T2_T2CKO_P10_T2EX_P11 0x00
#define TIMER2_PINMUX_T2_T2CKO_P32_T2EX_P33 0x01
#define TIMER2_PINMUX_T2_T2CKO_P67_T2EX_P57 0x02
#define TIMER2_PINMUX_T2_T2CKO_P45_T2EX_P44 0x03
///@endcond
#define __DRV_TIMER2_PinMUX_Select(__MODE__) \
    MWT(                                     \
        __DRV_TIMER2_SetT2PS1(__MODE__);     \
        __DRV_TIMER2_SetT2PS0(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER2 Pin Mux
* @details      Set T2PS1
* @param[in]    __MODE__ :
*  @arg\b       TIMER2_PINMUX_T2_T2CKO_P10_T2EX_P11
*  @arg\b       TIMER2_PINMUX_T2_T2CKO_P32_T2EX_P33
*  @arg\b       TIMER2_PINMUX_T2_T2CKO_P67_T2EX_P57
*  @arg\b       TIMER2_PINMUX_T2_T2CKO_P45_T2EX_P44
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_SetT2PS1(TIMER2_PINMUX_T2_T2CKO_P10_T2EX_P11)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_SetT2PS1(__MODE__)                                                                            \
    MWT(                                                                                                           \
        __DRV_SFR_PageIndex(AUXR4_Page);                                                                           \
        ((__MODE__ ^ 0x00) == 0 ? (AUXR4 = AUXR4 & (~T2PS1)) : (__MODE__ ^ 0x01) == 0 ? (AUXR4 = AUXR4 & (~T2PS1)) \
                                                           : (__MODE__ ^ 0x02) == 0   ? (AUXR4 = AUXR4 | (T2PS1))  \
                                                           : (__MODE__ ^ 0x03) == 0   ? (AUXR4 = AUXR4 | (T2PS1))  \
                                                                                      : _nop_());                    \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER2 Pin Mux
* @details      Set T2PS0
* @param[in]    __MODE__ :
*  @arg\b       TIMER2_PINMUX_T2_T2CKO_P10_T2EX_P11
*  @arg\b       TIMER2_PINMUX_T2_T2CKO_P32_T2EX_P33
*  @arg\b       TIMER2_PINMUX_T2_T2CKO_P67_T2EX_P57
*  @arg\b       TIMER2_PINMUX_T2_T2CKO_P45_T2EX_P44
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_SetT2PS0(TIMER2_PINMUX_T2_T2CKO_P10_T2EX_P11)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_SetT2PS0(__MODE__)                                                                            \
    MWT(                                                                                                           \
        __DRV_SFR_PageIndex(AUXR4_Page);                                                                           \
        ((__MODE__ ^ 0x00) == 0 ? (AUXR4 = AUXR4 & (~T2PS0)) : (__MODE__ ^ 0x01) == 0 ? (AUXR4 = AUXR4 | (T2PS0))  \
                                                           : (__MODE__ ^ 0x02) == 0   ? (AUXR4 = AUXR4 & (~T2PS0)) \
                                                           : (__MODE__ ^ 0x03) == 0   ? (AUXR4 = AUXR4 | (T2PS0))  \
                                                                                      : _nop_());                    \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER3 Pin Mux
* @details      Set T3PS1 T3PS0
* @param[in]    __MODE__ :
*  @arg\b       TIMER3_PINMUX_T3_T3CKO_P46_T3EX_P00
*  @arg\b       TIMER3_PINMUX_T3_T3CKO_P33_T3EX_P32
*  @arg\b       TIMER3_PINMUX_T3_T3CKO_P21_T3EX_P20
*  @arg\b       TIMER3_PINMUX_T3_T3CKO_P66_T3EX_P65
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_PinMUX_Select(TIMER3_PINMUX_T3_T3CKO_P46_T3EX_P00)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER3_PinMUX_Select
#define TIMER3_PINMUX_T3_T3CKO_P46_T3EX_P00 0x00
#define TIMER3_PINMUX_T3_T3CKO_P33_T3EX_P32 0x01
#define TIMER3_PINMUX_T3_T3CKO_P21_T3EX_P20 0x02
#define TIMER3_PINMUX_T3_T3CKO_P66_T3EX_P65 0x03
///@endcond
#define __DRV_TIMER3_PinMUX_Select(__MODE__) \
    MWT(                                     \
        __DRV_TIMER3_SetT3PS1(__MODE__);     \
        __DRV_TIMER3_SetT3PS0(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER3 Pin Mux
* @details      Set T3PS1
* @param[in]    __MODE__ :
*  @arg\b       TIMER3_PINMUX_T3_T3CKO_P46_T3EX_P00
*  @arg\b       TIMER3_PINMUX_T3_T3CKO_P33_T3EX_P32
*  @arg\b       TIMER3_PINMUX_T3_T3CKO_P21_T3EX_P20
*  @arg\b       TIMER3_PINMUX_T3_T3CKO_P66_T3EX_P65
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_SetT3PS1(TIMER3_PINMUX_T3_T3CKO_P46_T3EX_P00)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_SetT3PS1(__MODE__)                                                                            \
    MWT(                                                                                                           \
        __DRV_SFR_PageIndex(AUXR8_Page);                                                                           \
        ((__MODE__ ^ 0x00) == 0 ? (AUXR8 = AUXR8 & (~T3PS1)) : (__MODE__ ^ 0x01) == 0 ? (AUXR8 = AUXR8 & (~T3PS1)) \
                                                           : (__MODE__ ^ 0x02) == 0   ? (AUXR8 = AUXR8 | (T3PS1))  \
                                                           : (__MODE__ ^ 0x03) == 0   ? (AUXR8 = AUXR8 | (T3PS1))  \
                                                                                      : _nop_());                    \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER3 Pin Mux
* @details      Set T3PS0
* @param[in]    __MODE__ :
*  @arg\b       TIMER3_PINMUX_T3_T3CKO_P46_T3EX_P00
*  @arg\b       TIMER3_PINMUX_T3_T3CKO_P33_T3EX_P32
*  @arg\b       TIMER3_PINMUX_T3_T3CKO_P21_T3EX_P20
*  @arg\b       TIMER3_PINMUX_T3_T3CKO_P66_T3EX_P65
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_SetT3PS0(TIMER3_PINMUX_T3_T3CKO_P46_T3EX_P00)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_SetT3PS0(__MODE__)                                                                            \
    MWT(                                                                                                           \
        __DRV_SFR_PageIndex(AUXR8_Page);                                                                           \
        ((__MODE__ ^ 0x00) == 0 ? (AUXR8 = AUXR8 & (~T3PS0)) : (__MODE__ ^ 0x01) == 0 ? (AUXR8 = AUXR8 | (T3PS0))  \
                                                           : (__MODE__ ^ 0x02) == 0   ? (AUXR8 = AUXR8 & (~T3PS0)) \
                                                           : (__MODE__ ^ 0x03) == 0   ? (AUXR8 = AUXR8 | (T3PS0))  \
                                                                                      : _nop_());                    \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 Pin Mux
* @details      Set T4PS1 T4PS0
* @param[in]    __MODE__ :
*  @arg\b       TIMER4_PINMUX_T4_T4CKO_P70_T4EX_P71
*  @arg\b       TIMER4_PINMUX_T4_T4CKO_P37_T4EX_P36
*  @arg\b       TIMER4_PINMUX_T4_T4CKO_P72_T4EX_P50
*  @arg\b       TIMER4_PINMUX_T4_T4CKO_P36_T4EX_P37
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_PinMUX_Select(TIMER4_PINMUX_T4_T4CKO_P70_T3EX_P71)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER4_PinMUX_Select
#define TIMER4_PINMUX_T4_T4CKO_P70_T4EX_P71 0x00
#define TIMER4_PINMUX_T4_T4CKO_P37_T4EX_P36 0x01
#define TIMER4_PINMUX_T4_T4CKO_P72_T4EX_P50 0x02
#define TIMER4_PINMUX_T4_T4CKO_P36_T4EX_P37 0x03
///@endcond
#define __DRV_TIMER4_PinMUX_Select(__MODE__) \
    MWT(                                     \
        __DRV_TIMER4_SetT4PS1(__MODE__);     \
        __DRV_TIMER4_SetT4PS0(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER4 Pin Mux
* @details      Set T4PS1
* @param[in]    __MODE__ :
*  @arg\b       TIMER4_PINMUX_T4_T4CKO_P70_T3EX_P71
*  @arg\b       TIMER4_PINMUX_T4_T4CKO_P37_T3EX_P36
*  @arg\b       TIMER4_PINMUX_T4_T4CKO_P72_T3EX_P50
*  @arg\b       TIMER4_PINMUX_T4_T4CKO_P36_T3EX_P37
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_SetT4PS1(TIMER4_PINMUX_T4_T4CKO_P70_T3EX_P71)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_SetT4PS1(__MODE__)                                                                                \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(AUXR13_Page);                                                                              \
        ((__MODE__ ^ 0x00) == 0 ? (AUXR13 = AUXR13 & (~T4PS1)) : (__MODE__ ^ 0x01) == 0 ? (AUXR13 = AUXR13 & (~T4PS1)) \
                                                             : (__MODE__ ^ 0x02) == 0   ? (AUXR13 = AUXR13 | (T4PS1))  \
                                                             : (__MODE__ ^ 0x03) == 0   ? (AUXR13 = AUXR13 | (T4PS1))  \
                                                                                        : _nop_());                      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 Pin Mux
* @details      Set T4PS0
* @param[in]    __MODE__ :
*  @arg\b       TIMER4_PINMUX_T4_T4CKO_P70_T3EX_P71
*  @arg\b       TIMER4_PINMUX_T4_T4CKO_P37_T3EX_P36
*  @arg\b       TIMER4_PINMUX_T4_T4CKO_P72_T3EX_P50
*  @arg\b       TIMER4_PINMUX_T4_T4CKO_P36_T3EX_P37
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_SetT4PS0(TIMER4_PINMUX_T4_T4CKO_P70_T3EX_P71)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_SetT4PS0(__MODE__)                                                                                \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(AUXR13_Page);                                                                              \
        ((__MODE__ ^ 0x00) == 0 ? (AUXR13 = AUXR13 & (~T4PS0)) : (__MODE__ ^ 0x01) == 0 ? (AUXR13 = AUXR13 | (T4PS0))  \
                                                             : (__MODE__ ^ 0x02) == 0   ? (AUXR13 = AUXR13 & (~T4PS0)) \
                                                             : (__MODE__ ^ 0x03) == 0   ? (AUXR13 = AUXR13 | (T4PS0))  \
                                                                                        : _nop_());                      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER2 TF2 Interrupt Ignored
* @details      Set TF2IG
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_TF2_Interrupt_Ignored_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_TF2_Interrupt_Ignored_Cmd(__STATE__)                            \
    MWT(                                                                             \
        __DRV_SFR_PageIndex(T2MOD1_Page);                                            \
        __STATE__ == 0 ? (T2MOD1 = T2MOD1 & (~TF2IG)) : (T2MOD1 = T2MOD1 | (TF2IG)); \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER2 Capture Source Dectect
* @details      Set EXEN2 T2EXH
* @param[in]    __MODE__ :
*  @arg\b       Timer2_IGNORED
*  @arg\b       Timer2_FALLING_EDGE
*  @arg\b       Timer2_RISING_EDGE
*  @arg\b       Timer2_DUAL_EDGE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_Capture_Source_Dectect(Timer2_IGNORED)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER2_Capture_Source_Dectect
#define Timer2_IGNORED 0x00
#define Timer2_RISING_EDGE 0x01
#define Timer2_FALLING_EDGE 0x02
#define Timer2_DUAL_EDGE 0x03
///@endcond
#define __DRV_TIMER2_Capture_Source_Dectect(__MODE__) \
    MWT(                                              \
        __DRV_TIMER2_SetEXEN2(__MODE__);              \
        __DRV_TIMER2_SetT2EXH(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER2 Capture Source Dectect
* @details      Set EXEN2
* @param[in]    __MODE__ :
*  @arg\b       Timer2_IGNORED
*  @arg\b       Timer2_FALLING_EDGE
*  @arg\b       Timer2_RISING_EDGE
*  @arg\b       Timer2_DUAL_EDGE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_SetEXEN2(Timer2_IGNORED)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_SetEXEN2(__MODE__)                                             \
    MWT(                                                                            \
        (__MODE__ ^ 0x00) == 0 ? (EXEN2 = 0) : (__MODE__ ^ 0x01) == 0 ? (EXEN2 = 0) \
                                           : (__MODE__ ^ 0x02) == 0   ? (EXEN2 = 1) \
                                           : (__MODE__ ^ 0x03) == 0   ? (EXEN2 = 1) \
                                                                      : _nop_();)

/**
*****************************************************************************
* @brief        TIMER2 Capture Source Dectect
* @details      Set T2EXH
* @param[in]    __MODE__ :
*  @arg\b       Timer2_IGNORED
*  @arg\b       Timer2_FALLING_EDGE
*  @arg\b       Timer2_RISING_EDGE
*  @arg\b       Timer2_DUAL_EDGE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_SetT2EXH(Timer2_IGNORED)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_SetT2EXH(__MODE__)                                                                           \
    MWT(                                                                                                          \
        (__MODE__ ^ 0x00) == 0 ? (T2MOD = T2MOD & (~T2EXH)) : (__MODE__ ^ 0x01) == 0 ? (T2MOD = T2MOD | (T2EXH))  \
                                                          : (__MODE__ ^ 0x02) == 0   ? (T2MOD = T2MOD & (~T2EXH)) \
                                                          : (__MODE__ ^ 0x03) == 0   ? (T2MOD = T2MOD | (T2EXH))  \
                                                                                     : _nop_();)

/**
*******************************************************************************
* @brief        TIMER2 High Byte Register
* @details      Set TH2
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_SetHighByte(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_SetHighByte(__RELOAD__) (TH2 = __RELOAD__)

/**
*******************************************************************************
* @brief        TIMER2 Low Byte Register
* @details      Set TL2
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_SetLowByte(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_SetLowByte(__RELOAD__) (TL2 = __RELOAD__)

/**
*******************************************************************************
* @brief        TIMER2 16 Bit Timer/Counter
* @details      Set TH2 TL2
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_Set16BitCounter(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_Set16BitCounter(__RELOAD__) \
    MWT(                                         \
        TH2 = HIBYTE(__RELOAD__);                \
        TL2 = LOBYTE(__RELOAD__) & 0xFF;)

/**
*******************************************************************************
* @brief        TIMER2 Capture High Byte Register
* @details      Set RCAP2H
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_SetRCAP2H(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_SetRCAP2H(__RELOAD__) (RCAP2H = __RELOAD__)

/**
*******************************************************************************
* @brief        TIMER2 Capture Low Byte Register
* @details      Set RCAP2L
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_SetRCAP2L(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_SetRCAP2L(__RELOAD__) (RCAP2L = __RELOAD__)

/**
*******************************************************************************
* @brief        TIMER2 16 Bit Capture Register
* @details      Set RCAP2H RCAP2L
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_Set16Bit_AutoReloadValue(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_Set16Bit_AutoReloadValue(__RELOAD__) \
    MWT(                                                  \
        RCAP2H = HIBYTE(__RELOAD__);                      \
        RCAP2L = LOBYTE(__RELOAD__) & 0xFF;)

/**
*******************************************************************************
* @brief        TIMER Globel Enable Control
* @details      Set TREN0
* @param[in]    __STATE__ :
*  @arg\b       TR0E
*  @arg\b       TR1E
*  @arg\b       TR2E
*  @arg\b       TR3E
*  @arg\b       TR4E
*  @arg\b       TR2LE
*  @arg\b       TR3LE
*  @arg\b       TR4LE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER_Globel_Enable_Cmd(__STATE__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER_Globel_Enable_Cmd(__STATE__) \
    MWT(                                         \
        __DRV_SFR_PageIndex(TREN0_Page);         \
        TREN0 = TREN0 | (__STATE__);             \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER Globel Reload Control
* @details      Set TRLC0
* @param[in]    __STATE__ :
*  @arg\b       T0RLC
*  @arg\b       T1RLC
*  @arg\b       T2RLC
*  @arg\b       T3RLC
*  @arg\b       T4RLC
*  @arg\b       TL2RLC
*  @arg\b       TL3RLC
*  @arg\b       TL4RLC
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER_Globel_Cmd(__STATE__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER_Globel_Reload_Cmd(__STATE__) \
    MWT(                                         \
        __DRV_SFR_PageIndex(TRLC0_Page);         \
        TRLC0 = TRLC0 | (__STATE__);             \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER Globel Stop Control
* @details      Set TSPC0
* @param[in]    __STATE__ :
*  @arg\b       T0SC
*  @arg\b       T1SC
*  @arg\b       T2SC
*  @arg\b       T3SC
*  @arg\b       T4SC
*  @arg\b       TL2SC
*  @arg\b       TL3SC
*  @arg\b       TL4SC
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER_Globel_Stop_Cmd(__STATE__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER_Globel_Stop_Cmd(__STATE__) \
    MWT(                                       \
        __DRV_SFR_PageIndex(TSPC0_Page);       \
        TSPC0 = TSPC0 | (__STATE__);           \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER3 TL3 Run Control
* @details      Set TR3L
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_TL3_Run_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_TL3_Run_Cmd(__STATE__)                                    \
    MWT(                                                                       \
        __DRV_SFR_PageIndex(T3MOD_Page);                                       \
        __STATE__ == 0 ? (T3MOD = T3MOD & (~TR3L)) : (T3MOD = T3MOD | (TR3L)); \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 TL4 Run Control
* @details      Set TR4L
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_TL4_Run_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_TL4_Run_Cmd(__STATE__)                                    \
    MWT(                                                                       \
        __DRV_SFR_PageIndex(T4MOD_Page);                                       \
        __STATE__ == 0 ? (T4MOD = T4MOD & (~TR4L)) : (T4MOD = T4MOD | (TR4L)); \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER3 TR3L Clear Control (Mode0~Mode3)
* @details      Set TR3LC
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_TR3LC_Clear_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_TR3L_Clear_Cmd(__STATE__)                                   \
    MWT(                                                                         \
        __DRV_SFR_PageIndex(T3MOD_Page);                                         \
        __STATE__ == 0 ? (T3MOD = T3MOD & (~TR3LC)) : (T3MOD = T3MOD | (TR3LC)); \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 TR4L Clear Control (Mode0~Mode3)
* @details      Set TR4LC
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_TR4LC_Clear_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_TR4L_Clear_Cmd(__STATE__)                                   \
    MWT(                                                                         \
        __DRV_SFR_PageIndex(T4MOD_Page);                                         \
        __STATE__ == 0 ? (T4MOD = T4MOD & (~TR4LC)) : (T4MOD = T4MOD | (TR4LC)); \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER3 T3CKO Invert Control (Mode4 8 Bit Pwm )
* @details      Set TR3LC
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_T3CKO_Invert_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_T3CKO_Invert_Cmd(__STATE__)                                 \
    MWT(                                                                         \
        __DRV_SFR_PageIndex(T3MOD_Page);                                         \
        __STATE__ == 0 ? (T3MOD = T3MOD & (~TR3LC)) : (T3MOD = T3MOD | (TR3LC)); \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 T4CKO Invert Control (Mode4 8 Bit Pwm )
* @details      Set TR4LC
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_T4CKO_Invert_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_T4CKO_Invert_Cmd(__STATE__)                                 \
    MWT(                                                                         \
        __DRV_SFR_PageIndex(T4MOD_Page);                                         \
        __STATE__ == 0 ? (T4MOD = T4MOD & (~TR4LC)) : (T4MOD = T4MOD | (TR4LC)); \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER3 Operating Mode
* @details      Set T3SPL T3MS1 CP/RL3 T3MS0
* @param[in]    __MODE__ :
*  @arg\b       TIMER3_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER3_MODE1_16BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER3_MODE2_16BIT_CAPTURE
*  @arg\b       TIMER3_MODE3_16BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER3_MODE3_16BIT_DUTY_CAPTURE
*  @arg\b       TIMER3_SPLIT_MODE0_TWO_8BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER3_SPLIT_MODE1_TWO_8BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER3_SPLIT_MODE2_TWO_8BIT_CAPTURE
*  @arg\b       TIMER3_SPLIT_MODE3_TWO_8BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER3_SPLIT_MODE4_8BIT_PWM
*  @arg\b       TIMER3_SPLIT_MODE6_DUTY_CAPTURE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_Mode_Select(TIMER3_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER3_Mode_Select
#define TIMER3_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT 0x00
#define TIMER3_MODE1_16BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT 0x01
#define TIMER3_MODE2_16BIT_CAPTURE 0x02
#define TIMER3_MODE3_16BIT_CAPTURE_WITH_AUTO_ZERO 0x03
#define TIMER3_SPLIT_MODE0_TWO_8BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT 0x04
#define TIMER3_SPLIT_MODE1_TWO_8BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT 0x05
#define TIMER3_SPLIT_MODE2_TWO_8BIT_CAPTURE 0x06
#define TIMER3_SPLIT_MODE3_TWO_8BIT_CAPTURE_WITH_AUTO_ZERO 0x07
#define TIMER3_SPLIT_MODE4_8BIT_PWM 0x08
#define TIMER3_MODE6_DUTY_CAPTURE 0x09
#define TIMER3_SPLIT_MODE6_DUTY_CAPTURE 0x0A
///@endcond
#define __DRV_TIMER3_Mode_Select(__MODE__) \
    MWT(                                   \
        __DRV_TIMER3_SetT3SPL(__MODE__);   \
        __DRV_TIMER3_SetT3MS1(__MODE__);   \
        __DRV_TIMER3_SetCP_RL3(__MODE__);  \
        __DRV_TIMER3_SetT3MS0(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER3 Operating Mode
* @details      Set T3SPL
* @param[in]    __MODE__ :
*  @arg\b       TIMER3_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER3_MODE1_16BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER3_MODE2_16BIT_CAPTURE
*  @arg\b       TIMER3_MODE3_16BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER3_MODE3_16BIT_DUTY_CAPTURE
*  @arg\b       TIMER3_SPLIT_MODE0_TWO_8BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER3_SPLIT_MODE1_TWO_8BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER3_SPLIT_MODE2_TWO_8BIT_CAPTURE
*  @arg\b       TIMER3_SPLIT_MODE3_TWO_8BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER3_SPLIT_MODE4_8BIT_PWM
*  @arg\b       TIMER4_SPLIT_MODE6_DUTY_CAPTURE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_SetT3SPL(TIMER3_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_SetT3SPL(__MODE__)                                                                            \
    MWT(                                                                                                           \
        __DRV_SFR_PageIndex(T3MOD_Page);                                                                           \
        ((__MODE__ ^ 0x00) == 0 ? (T3MOD = T3MOD & (~T3SPL)) : (__MODE__ ^ 0x01) == 0 ? (T3MOD = T3MOD & (~T3SPL)) \
                                                           : (__MODE__ ^ 0x02) == 0   ? (T3MOD = T3MOD & (~T3SPL)) \
                                                           : (__MODE__ ^ 0x03) == 0   ? (T3MOD = T3MOD & (~T3SPL)) \
                                                           : (__MODE__ ^ 0x04) == 0   ? (T3MOD = T3MOD | (T3SPL))  \
                                                           : (__MODE__ ^ 0x05) == 0   ? (T3MOD = T3MOD | (T3SPL))  \
                                                           : (__MODE__ ^ 0x06) == 0   ? (T3MOD = T3MOD | (T3SPL))  \
                                                           : (__MODE__ ^ 0x07) == 0   ? (T3MOD = T3MOD | (T3SPL))  \
                                                           : (__MODE__ ^ 0x08) == 0   ? (T3MOD = T3MOD | (T3SPL))  \
                                                           : (__MODE__ ^ 0x09) == 0   ? (T3MOD = T3MOD & (~T3SPL)) \
                                                           : (__MODE__ ^ 0x0A) == 0   ? (T3MOD = T3MOD | (T3SPL))  \
                                                                                      : _nop_());                    \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER3 Operating Mode
* @details      Set T3MS1
* @param[in]    __MODE__ :
*  @arg\b       TIMER3_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER3_MODE1_16BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER3_MODE2_16BIT_CAPTURE
*  @arg\b       TIMER3_MODE3_16BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER3_MODE3_16BIT_DUTY_CAPTURE
*  @arg\b       TIMER3_SPLIT_MODE0_TWO_8BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER3_SPLIT_MODE1_TWO_8BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER3_SPLIT_MODE2_TWO_8BIT_CAPTURE
*  @arg\b       TIMER3_SPLIT_MODE3_TWO_8BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER3_SPLIT_MODE4_8BIT_PWM
*  @arg\b       TIMER4_SPLIT_MODE6_DUTY_CAPTURE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_SetT3MS1(TIMER3_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_SetT3MS1(__MODE__)                                                                                \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(T3MOD1_Page);                                                                              \
        ((__MODE__ ^ 0x00) == 0 ? (T3MOD1 = T3MOD1 & (~T3MS1)) : (__MODE__ ^ 0x01) == 0 ? (T3MOD1 = T3MOD1 & (~T3MS1)) \
                                                             : (__MODE__ ^ 0x02) == 0   ? (T3MOD1 = T3MOD1 & (~T3MS1)) \
                                                             : (__MODE__ ^ 0x03) == 0   ? (T3MOD1 = T3MOD1 & (~T3MS1)) \
                                                             : (__MODE__ ^ 0x04) == 0   ? (T3MOD1 = T3MOD1 & (~T3MS1)) \
                                                             : (__MODE__ ^ 0x05) == 0   ? (T3MOD1 = T3MOD1 & (~T3MS1)) \
                                                             : (__MODE__ ^ 0x06) == 0   ? (T3MOD1 = T3MOD1 & (~T3MS1)) \
                                                             : (__MODE__ ^ 0x07) == 0   ? (T3MOD1 = T3MOD1 & (~T3MS1)) \
                                                             : (__MODE__ ^ 0x08) == 0   ? (T3MOD1 = T3MOD1 | (T3MS1))  \
                                                             : (__MODE__ ^ 0x09) == 0   ? (T3MOD1 = T3MOD1 | (T3MS1))  \
                                                             : (__MODE__ ^ 0x0A) == 0   ? (T3MOD1 = T3MOD1 | (T3MS1))  \
                                                                                        : _nop_());                      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER3 Operating Mode
* @details      Set CP/RL3
* @param[in]    __MODE__ :
*  @arg\b       TIMER3_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER3_MODE1_16BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER3_MODE2_16BIT_CAPTURE
*  @arg\b       TIMER3_MODE3_16BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER3_MODE3_16BIT_DUTY_CAPTURE
*  @arg\b       TIMER3_SPLIT_MODE0_TWO_8BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER3_SPLIT_MODE1_TWO_8BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER3_SPLIT_MODE2_TWO_8BIT_CAPTURE
*  @arg\b       TIMER3_SPLIT_MODE3_TWO_8BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER3_SPLIT_MODE4_8BIT_PWM
*  @arg\b       TIMER4_SPLIT_MODE6_DUTY_CAPTURE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_SetCP_RL3(TIMER3_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_SetCP_RL3(__MODE__)                                               \
    MWT(                                                                               \
        __DRV_SFR_PageIndex(T3CON_Page);                                               \
        ((__MODE__ ^ 0x00) == 0 ? (CP_RL3 = 0) : (__MODE__ ^ 0x01) == 0 ? (CP_RL3 = 0) \
                                             : (__MODE__ ^ 0x02) == 0   ? (CP_RL3 = 1) \
                                             : (__MODE__ ^ 0x03) == 0   ? (CP_RL3 = 1) \
                                             : (__MODE__ ^ 0x04) == 0   ? (CP_RL3 = 0) \
                                             : (__MODE__ ^ 0x05) == 0   ? (CP_RL3 = 0) \
                                             : (__MODE__ ^ 0x06) == 0   ? (CP_RL3 = 1) \
                                             : (__MODE__ ^ 0x07) == 0   ? (CP_RL3 = 1) \
                                             : (__MODE__ ^ 0x08) == 0   ? (CP_RL3 = 0) \
                                             : (__MODE__ ^ 0x09) == 0   ? (CP_RL3 = 1) \
                                             : (__MODE__ ^ 0x0A) == 0   ? (CP_RL3 = 1) \
                                                                        : _nop_());      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER3 Operating Mode
* @details      Set T3MS0
* @param[in]    __MODE__ :
*  @arg\b       TIMER3_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER3_MODE1_16BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER3_MODE2_16BIT_CAPTURE
*  @arg\b       TIMER3_MODE3_16BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER3_MODE3_16BIT_DUTY_CAPTURE
*  @arg\b       TIMER3_SPLIT_MODE0_TWO_8BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER3_SPLIT_MODE1_TWO_8BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER3_SPLIT_MODE2_TWO_8BIT_CAPTURE
*  @arg\b       TIMER3_SPLIT_MODE3_TWO_8BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER3_SPLIT_MODE4_8BIT_PWM
*  @arg\b       TIMER4_SPLIT_MODE6_DUTY_CAPTURE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_SetT3MS0(TIMER3_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_SetT3MS0(__MODE__)                                                                            \
    MWT(                                                                                                           \
        __DRV_SFR_PageIndex(T3MOD_Page);                                                                           \
        ((__MODE__ ^ 0x00) == 0 ? (T3MOD = T3MOD & (~T3MS0)) : (__MODE__ ^ 0x01) == 0 ? (T3MOD = T3MOD | (T3MS0))  \
                                                           : (__MODE__ ^ 0x02) == 0   ? (T3MOD = T3MOD & (~T3MS0)) \
                                                           : (__MODE__ ^ 0x03) == 0   ? (T3MOD = T3MOD | (T3MS0))  \
                                                           : (__MODE__ ^ 0x04) == 0   ? (T3MOD = T3MOD & (~T3MS0)) \
                                                           : (__MODE__ ^ 0x05) == 0   ? (T3MOD = T3MOD | (T3MS0))  \
                                                           : (__MODE__ ^ 0x06) == 0   ? (T3MOD = T3MOD & (~T3MS0)) \
                                                           : (__MODE__ ^ 0x07) == 0   ? (T3MOD = T3MOD | (T3MS0))  \
                                                           : (__MODE__ ^ 0x08) == 0   ? (T3MOD = T3MOD & (~T3MS0)) \
                                                           : (__MODE__ ^ 0x09) == 0   ? (T3MOD = T3MOD & (~T3MS0)) \
                                                           : (__MODE__ ^ 0x0A) == 0   ? (T3MOD = T3MOD & (~T3MS0)) \
                                                                                      : _nop_());                    \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER3 Clock Out Control
* @details      Set T3OE
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         Timer3 Pin Configuration T3/T3CKO P33 T3EX P34
* @par          Example
* @code
                __DRV_TIMER3_Clock_Out_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_Clock_Out_Cmd(__STATE__)                                  \
    MWT(                                                                       \
        __DRV_SFR_PageIndex(T3MOD_Page);                                       \
        __STATE__ == 0 ? (T3MOD = T3MOD & (~T3OE)) : (T3MOD = T3MOD | (T3OE)); \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 Clock Out Control
* @details      Set T4OE
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         Timer4 Pin Configuration T4/T4CKO P70 T4EX P71
* @par          Example
* @code
                __DRV_TIMER4_Clock_Out_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_Clock_Out_Cmd(__STATE__)                                  \
    MWT(                                                                       \
        __DRV_SFR_PageIndex(T4MOD_Page);                                       \
        __STATE__ == 0 ? (T4MOD = T4MOD & (~T4OE)) : (T4MOD = T4MOD | (T4OE)); \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER3 Clock Source
* @details      Set T3CKS T3X12 C/T3
* @param[in]    __MODE__ :
*  @arg\b       TIMER3_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER3_CLOCK_SOURCE_T3_PIN
*  @arg\b       TIMER3_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER3_CLOCK_SOURCE_INT1ET
*  @arg\b       TIMER3_CLOCK_SOURCE_S1TOF
*  @arg\b       TIMER3_CLOCK_SOURCE_T0OF
*  @arg\b       TIMER3_CLOCK_SOURCE_AC1ES
*  @arg\b       TIMER3_CLOCK_SOURCE_T1OF
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_Clock_Source_Select(TIMER3_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER3_Clock_Source_Select
#define TIMER3_CLOCK_SOURCE_SYSCLK_DIV_12 0x00
#define TIMER3_CLOCK_SOURCE_T3_PIN 0x01
#define TIMER3_CLOCK_SOURCE_SYSCLK 0x02
#define TIMER3_CLOCK_SOURCE_INT1ET 0x03
#define TIMER3_CLOCK_SOURCE_S1TOF 0x04
#define TIMER3_CLOCK_SOURCE_T0OF 0x05
#define TIMER3_CLOCK_SOURCE_AC1ES 0x06
#define TIMER3_CLOCK_SOURCE_T1OF 0x07
///@endcond
#define __DRV_TIMER3_Clock_Source_Select(__MODE__) \
    MWT(                                           \
        __DRV_TIMER3_SetT3CKS(__MODE__);           \
        __DRV_TIMER3_SetT3X12(__MODE__);           \
        __DRV_TIMER3_SetC_T3(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER3 Clock Source
* @details      Set T3CKS
* @param[in]    __MODE__ :
*  @arg\b       TIMER3_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER3_CLOCK_SOURCE_T3_PIN
*  @arg\b       TIMER3_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER3_CLOCK_SOURCE_INT1ET
*  @arg\b       TIMER3_CLOCK_SOURCE_S1TOF
*  @arg\b       TIMER3_CLOCK_SOURCE_T0OF
*  @arg\b       TIMER3_CLOCK_SOURCE_AC1ES
*  @arg\b       TIMER3_CLOCK_SOURCE_T1OF
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_SetT3CKS(TIMER3_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_SetT3CKS(__MODE__)                                                                                \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(T3MOD1_Page);                                                                              \
        ((__MODE__ ^ 0x00) == 0 ? (T3MOD1 = T3MOD1 & (~T3CKS)) : (__MODE__ ^ 0x01) == 0 ? (T3MOD1 = T3MOD1 & (~T3CKS)) \
                                                             : (__MODE__ ^ 0x02) == 0   ? (T3MOD1 = T3MOD1 & (~T3CKS)) \
                                                             : (__MODE__ ^ 0x03) == 0   ? (T3MOD1 = T3MOD1 & (~T3CKS)) \
                                                             : (__MODE__ ^ 0x04) == 0   ? (T3MOD1 = T3MOD1 | (T3CKS))  \
                                                             : (__MODE__ ^ 0x05) == 0   ? (T3MOD1 = T3MOD1 | (T3CKS))  \
                                                             : (__MODE__ ^ 0x06) == 0   ? (T3MOD1 = T3MOD1 | (T3CKS))  \
                                                             : (__MODE__ ^ 0x07) == 0   ? (T3MOD1 = T3MOD1 | (T3CKS))  \
                                                                                        : _nop_());                      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER3 Clock Source
* @details      Set T3X12
* @param[in]    __MODE__ :
*  @arg\b       TIMER3_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER3_CLOCK_SOURCE_T3_PIN
*  @arg\b       TIMER3_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER3_CLOCK_SOURCE_INT1ET
*  @arg\b       TIMER3_CLOCK_SOURCE_S1TOF
*  @arg\b       TIMER3_CLOCK_SOURCE_T0OF
*  @arg\b       TIMER3_CLOCK_SOURCE_AC1ES
*  @arg\b       TIMER3_CLOCK_SOURCE_T1OF
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_SetT3X12(TIMER3_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_SetT3X12(__MODE__)                                                                            \
    MWT(                                                                                                           \
        __DRV_SFR_PageIndex(T3MOD_Page);                                                                           \
        ((__MODE__ ^ 0x00) == 0 ? (T3MOD = T3MOD & (~T3X12)) : (__MODE__ ^ 0x01) == 0 ? (T3MOD = T3MOD & (~T3X12)) \
                                                           : (__MODE__ ^ 0x02) == 0   ? (T3MOD = T3MOD | (T3X12))  \
                                                           : (__MODE__ ^ 0x03) == 0   ? (T3MOD = T3MOD | (T3X12))  \
                                                           : (__MODE__ ^ 0x04) == 0   ? (T3MOD = T3MOD & (~T3X12)) \
                                                           : (__MODE__ ^ 0x05) == 0   ? (T3MOD = T3MOD & (~T3X12)) \
                                                           : (__MODE__ ^ 0x06) == 0   ? (T3MOD = T3MOD | (T3X12))  \
                                                           : (__MODE__ ^ 0x07) == 0   ? (T3MOD = T3MOD | (T3X12))  \
                                                                                      : _nop_());                    \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER3 Clock Source
* @details      Set C/T3
* @param[in]    __MODE__ :
*  @arg\b       TIMER3_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER3_CLOCK_SOURCE_T3_PIN
*  @arg\b       TIMER3_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER3_CLOCK_SOURCE_INT1ET
*  @arg\b       TIMER3_CLOCK_SOURCE_S1TOF
*  @arg\b       TIMER3_CLOCK_SOURCE_T0OF
*  @arg\b       TIMER3_CLOCK_SOURCE_AC1ES
*  @arg\b       TIMER3_CLOCK_SOURCE_T1OF
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_SetC_T3(TIMER3_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_SetC_T3(__MODE__)                                             \
    MWT(                                                                           \
        __DRV_SFR_PageIndex(T3CON_Page);                                           \
        ((__MODE__ ^ 0x00) == 0 ? (C_T3 = 0) : (__MODE__ ^ 0x01) == 0 ? (C_T3 = 1) \
                                           : (__MODE__ ^ 0x02) == 0   ? (C_T3 = 0) \
                                           : (__MODE__ ^ 0x03) == 0   ? (C_T3 = 1) \
                                           : (__MODE__ ^ 0x04) == 0   ? (C_T3 = 0) \
                                           : (__MODE__ ^ 0x05) == 0   ? (C_T3 = 1) \
                                           : (__MODE__ ^ 0x06) == 0   ? (C_T3 = 0) \
                                           : (__MODE__ ^ 0x07) == 0   ? (C_T3 = 1) \
                                                                      : _nop_());    \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER3 TH3 Clock Source in Split Mode
* @details      Set T3CKS T3X12 C/T3
* @param[in]    __MODE__ :
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_T3_PIN
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_TL3OF
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_S1TOF
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_T0OF
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_AC1ES
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_T1OF
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_TH3_Clock_Source_Select(TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER3_TH3_Clock_Source_Select
#define TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12 0x00
#define TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_T3_PIN 0x01
#define TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK 0x02
#define TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_TL3OF 0x03
#define TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_S1TOF 0x04
#define TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_T0OF 0x05
#define TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_AC1ES 0x06
#define TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_T1OF 0x07
///@endcond
#define __DRV_TIMER3_TH3_Clock_Source_Select(__MODE__) \
    MWT(                                               \
        __DRV_TIMER3_TH3_SetT3CKS(__MODE__);           \
        __DRV_TIMER3_TH3_SetT3X12(__MODE__);           \
        __DRV_TIMER3_TH3_SetC_T3(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER3 TH3 Clock Source in Split Mode
* @details      Set T3CKS
* @param[in]    __MODE__ :
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_T3_PIN
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_TL3OF
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_S1TOF
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_T0OF
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_AC1ES
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_T1OF
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_TH3_SetT3CKS(TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_TH3_SetT3CKS(__MODE__)                                                                            \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(T3MOD1_Page);                                                                              \
        ((__MODE__ ^ 0x00) == 0 ? (T3MOD1 = T3MOD1 & (~T3CKS)) : (__MODE__ ^ 0x01) == 0 ? (T3MOD1 = T3MOD1 & (~T3CKS)) \
                                                             : (__MODE__ ^ 0x02) == 0   ? (T3MOD1 = T3MOD1 & (~T3CKS)) \
                                                             : (__MODE__ ^ 0x03) == 0   ? (T3MOD1 = T3MOD1 & (~T3CKS)) \
                                                             : (__MODE__ ^ 0x04) == 0   ? (T3MOD1 = T3MOD1 | (T3CKS))  \
                                                             : (__MODE__ ^ 0x05) == 0   ? (T3MOD1 = T3MOD1 | (T3CKS))  \
                                                             : (__MODE__ ^ 0x06) == 0   ? (T3MOD1 = T3MOD1 | (T3CKS))  \
                                                             : (__MODE__ ^ 0x07) == 0   ? (T3MOD1 = T3MOD1 | (T3CKS))  \
                                                                                        : _nop_());                      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER3 TH3 Clock Source in Split Mode
* @details      Set T3X12
* @param[in]    __MODE__ :
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_T3_PIN
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_TL3OF
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_S1TOF
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_T0OF
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_AC1ES
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_T1OF
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_TH3_SetT3X12(TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_TH3_SetT3X12(__MODE__)                                                                        \
    MWT(                                                                                                           \
        __DRV_SFR_PageIndex(T3MOD_Page);                                                                           \
        ((__MODE__ ^ 0x00) == 0 ? (T3MOD = T3MOD & (~T3X12)) : (__MODE__ ^ 0x01) == 0 ? (T3MOD = T3MOD & (~T3X12)) \
                                                           : (__MODE__ ^ 0x02) == 0   ? (T3MOD = T3MOD | (T3X12))  \
                                                           : (__MODE__ ^ 0x03) == 0   ? (T3MOD = T3MOD | (T3X12))  \
                                                           : (__MODE__ ^ 0x04) == 0   ? (T3MOD = T3MOD & (~T3X12)) \
                                                           : (__MODE__ ^ 0x05) == 0   ? (T3MOD = T3MOD & (~T3X12)) \
                                                           : (__MODE__ ^ 0x06) == 0   ? (T3MOD = T3MOD | (T3X12))  \
                                                           : (__MODE__ ^ 0x07) == 0   ? (T3MOD = T3MOD | (T3X12))  \
                                                                                      : _nop_());                    \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER3 TH3 Clock Source in Split Mode
* @details      Set C/T3
* @param[in]    __MODE__ :
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_T3_PIN
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_TL3OF
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_S1TOF
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_T0OF
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_AC1ES
*  @arg\b       TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_T1OF
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_TH3_SetC_T3(TIMER3_TH3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_TH3_SetC_T3(__MODE__)                                         \
    MWT(                                                                           \
        __DRV_SFR_PageIndex(T3CON_Page);                                           \
        ((__MODE__ ^ 0x00) == 0 ? (C_T3 = 0) : (__MODE__ ^ 0x01) == 0 ? (C_T3 = 1) \
                                           : (__MODE__ ^ 0x02) == 0   ? (C_T3 = 0) \
                                           : (__MODE__ ^ 0x03) == 0   ? (C_T3 = 1) \
                                           : (__MODE__ ^ 0x04) == 0   ? (C_T3 = 0) \
                                           : (__MODE__ ^ 0x05) == 0   ? (C_T3 = 1) \
                                           : (__MODE__ ^ 0x06) == 0   ? (C_T3 = 0) \
                                           : (__MODE__ ^ 0x07) == 0   ? (C_T3 = 1) \
                                                                      : _nop_());    \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER3 TL3 Clock Source in Split Mode
* @details      Set TL3CS TL3X12
* @param[in]    __MODE__ :
*  @arg\b       TIMER3_TL3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER3_TL3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER3_TL3_SPLIT_MODE_CLOCK_SOURCE_S1TOF
*  @arg\b       TIMER3_TL3_SPLIT_MODE_CLOCK_SOURCE_INT1ET
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_TL3_Clock_Source_Select(TIMER3_TL3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER3_TL3_Clock_Source_Select
#define TIMER3_TL3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12 0x00
#define TIMER3_TL3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK 0x01
#define TIMER3_TL3_SPLIT_MODE_CLOCK_SOURCE_S1TOF 0x02
#define TIMER3_TL3_SPLIT_MODE_CLOCK_SOURCE_INT1ET 0x03
///@endcond
#define __DRV_TIMER3_TL3_Clock_Source_Select(__MODE__) \
    MWT(                                               \
        __DRV_TIMER3_TL3_SetTL3CS(__MODE__);           \
        __DRV_TIMER3_TL3_SetTL3X12(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER3 TL3 Clock Source in Split Mode
* @details      Set TL3CS
* @param[in]    __MODE__ :
*  @arg\b       TIMER3_TL3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER3_TL3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER3_TL3_SPLIT_MODE_CLOCK_SOURCE_S1TOF
*  @arg\b       TIMER3_TL3_SPLIT_MODE_CLOCK_SOURCE_INT1ET
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_TL3_SetTL3CS(TIMER3_TL3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_TL3_SetTL3CS(__MODE__)                                                                            \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(T3MOD1_Page);                                                                              \
        ((__MODE__ ^ 0x00) == 0 ? (T3MOD1 = T3MOD1 & (~TL3CS)) : (__MODE__ ^ 0x01) == 0 ? (T3MOD1 = T3MOD1 & (~TL3CS)) \
                                                             : (__MODE__ ^ 0x02) == 0   ? (T3MOD1 = T3MOD1 | (TL3CS))  \
                                                             : (__MODE__ ^ 0x03) == 0   ? (T3MOD1 = T3MOD1 | (TL3CS))  \
                                                                                        : _nop_());                      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TL3 Clock Source in Split Mode
* @details      Set TL3X12
* @param[in]    __MODE__ :
*  @arg\b       TIMER3_TL3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER3_TL3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER3_TL3_SPLIT_MODE_CLOCK_SOURCE_S1TOF
*  @arg\b       TIMER3_TL3_SPLIT_MODE_CLOCK_SOURCE_INT1ET
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_TL3_SetTL3X12(TIMER3_TL3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_TL3_SetTL3X12(__MODE__)                                                                                     \
    MWT(                                                                                                                         \
        __DRV_SFR_PageIndex(T3MOD_Page);                                                                                         \
        ((__MODE__ ^ 0x00) == 0 ? (T3MOD = T3MOD & (~TL3X12_T3EIP)) : (__MODE__ ^ 0x01) == 0 ? (T3MOD = T3MOD | (TL3X12_T3EIP))  \
                                                                  : (__MODE__ ^ 0x02) == 0   ? (T3MOD = T3MOD & (~TL3X12_T3EIP)) \
                                                                  : (__MODE__ ^ 0x03) == 0   ? (T3MOD = T3MOD | (TL3X12_T3EIP))  \
                                                                                             : _nop_());                           \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER3 Capture Source
* @details      Set CP3S2 CP3S1 CP3S0
* @param[in]    __MODE__ :
*  @arg\b       TIMER3_CAPTURE_SOURCE_T3EX_PIN
*  @arg\b       TIMER3_CAPTURE_SOURCE_INT0ET
*  @arg\b       TIMER3_CAPTURE_SOURCE_P60
*  @arg\b       TIMER3_CAPTURE_SOURCE_NSS_PIN
*  @arg\b       TIMER3_CAPTURE_SOURCE_KBIET
*  @arg\b       TIMER3_CAPTURE_SOURCE_AC0OUT
*  @arg\b       TIMER3_CAPTURE_SOURCE_AC1OUT
*  @arg\b       TIMER3_CAPTURE_SOURCE_ILRCO
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_Capture_Source_Select(TIMER3_CAPTURE_SOURCE_T3EX_PIN)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER3_Capture_Source_Select
#define TIMER3_CAPTURE_SOURCE_T3EX_PIN 0x00
#define TIMER3_CAPTURE_SOURCE_INT0ET 0x01
#define TIMER3_CAPTURE_SOURCE_P60 0x02
#define TIMER3_CAPTURE_SOURCE_NSS_PIN 0x03
#define TIMER3_CAPTURE_SOURCE_KBIET 0x04
#define TIMER3_CAPTURE_SOURCE_AC0OUT 0x05
#define TIMER3_CAPTURE_SOURCE_AC1OUT 0x06
#define TIMER3_CAPTURE_SOURCE_ILRCO 0x07
///@endcond
#define __DRV_TIMER3_Capture_Source_Select(__MODE__) \
    MWT(                                             \
        __DRV_TH3_SetCP3S2(__MODE__);                \
        __DRV_TH3_SetCP3S1(__MODE__);                \
        __DRV_TH3_SetCP3S0(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER3 Capture Source
* @details      Set CP3S2
* @param[in]    __MODE__ :
*  @arg\b       TIMER3_CAPTURE_SOURCE_T3EX_PIN
*  @arg\b       TIMER3_CAPTURE_SOURCE_INT0ET
*  @arg\b       TIMER3_CAPTURE_SOURCE_P60
*  @arg\b       TIMER3_CAPTURE_SOURCE_NSS_PIN
*  @arg\b       TIMER3_CAPTURE_SOURCE_KBIET
*  @arg\b       TIMER3_CAPTURE_SOURCE_AC0OUT
*  @arg\b       TIMER3_CAPTURE_SOURCE_AC1OUT
*  @arg\b       TIMER3_CAPTURE_SOURCE_ILRCO
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_SetCP3S2(TIMER3_CAPTURE_SOURCE_T3EX_PIN)
* @endcode
*******************************************************************************
*/
#define __DRV_TH3_SetCP3S2(__MODE__)                                                                                   \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(T3MOD1_Page);                                                                              \
        ((__MODE__ ^ 0x00) == 0 ? (T3MOD1 = T3MOD1 & (~CP3S2)) : (__MODE__ ^ 0x01) == 0 ? (T3MOD1 = T3MOD1 & (~CP3S2)) \
                                                             : (__MODE__ ^ 0x02) == 0   ? (T3MOD1 = T3MOD1 & (~CP3S2)) \
                                                             : (__MODE__ ^ 0x03) == 0   ? (T3MOD1 = T3MOD1 & (~CP3S2)) \
                                                             : (__MODE__ ^ 0x04) == 0   ? (T3MOD1 = T3MOD1 | (CP3S2))  \
                                                             : (__MODE__ ^ 0x05) == 0   ? (T3MOD1 = T3MOD1 | (CP3S2))  \
                                                             : (__MODE__ ^ 0x06) == 0   ? (T3MOD1 = T3MOD1 | (CP3S2))  \
                                                             : (__MODE__ ^ 0x07) == 0   ? (T3MOD1 = T3MOD1 | (CP3S2))  \
                                                                                        : _nop_());                      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER3 Capture Source
* @details      Set CP3S1
* @param[in]    __MODE__ :
*  @arg\b       TIMER3_CAPTURE_SOURCE_T3EX_PIN
*  @arg\b       TIMER3_CAPTURE_SOURCE_INT0ET
*  @arg\b       TIMER3_CAPTURE_SOURCE_P60
*  @arg\b       TIMER3_CAPTURE_SOURCE_NSS_PIN
*  @arg\b       TIMER3_CAPTURE_SOURCE_KBIET
*  @arg\b       TIMER3_CAPTURE_SOURCE_AC0OUT
*  @arg\b       TIMER3_CAPTURE_SOURCE_AC1OUT
*  @arg\b       TIMER3_CAPTURE_SOURCE_ILRCO
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_SetCP3S1(TIMER3_CAPTURE_SOURCE_T3EX_PIN)
* @endcode
*******************************************************************************
*/
#define __DRV_TH3_SetCP3S1(__MODE__)                                                                                   \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(T3MOD1_Page);                                                                              \
        ((__MODE__ ^ 0x00) == 0 ? (T3MOD1 = T3MOD1 & (~CP3S1)) : (__MODE__ ^ 0x01) == 0 ? (T3MOD1 = T3MOD1 & (~CP3S1)) \
                                                             : (__MODE__ ^ 0x02) == 0   ? (T3MOD1 = T3MOD1 | (CP3S1))  \
                                                             : (__MODE__ ^ 0x03) == 0   ? (T3MOD1 = T3MOD1 | (CP3S1))  \
                                                             : (__MODE__ ^ 0x04) == 0   ? (T3MOD1 = T3MOD1 & (~CP3S1)) \
                                                             : (__MODE__ ^ 0x05) == 0   ? (T3MOD1 = T3MOD1 & (~CP3S1)) \
                                                             : (__MODE__ ^ 0x06) == 0   ? (T3MOD1 = T3MOD1 | (CP3S1))  \
                                                             : (__MODE__ ^ 0x07) == 0   ? (T3MOD1 = T3MOD1 | (CP3S1))  \
                                                                                        : _nop_());                      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER3 Capture Source
* @details      Set CP3S0
* @param[in]    __MODE__ :
*  @arg\b       TIMER3_CAPTURE_SOURCE_T3EX_PIN
*  @arg\b       TIMER3_CAPTURE_SOURCE_INT0ET
*  @arg\b       TIMER3_CAPTURE_SOURCE_P60
*  @arg\b       TIMER3_CAPTURE_SOURCE_NSS_PIN
*  @arg\b       TIMER3_CAPTURE_SOURCE_KBIET
*  @arg\b       TIMER3_CAPTURE_SOURCE_AC0OUT
*  @arg\b       TIMER3_CAPTURE_SOURCE_AC1OUT
*  @arg\b       TIMER3_CAPTURE_SOURCE_ILRCO
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_SetCP3S0(TIMER3_CAPTURE_SOURCE_T3EX_PIN)
* @endcode
*******************************************************************************
*/
#define __DRV_TH3_SetCP3S0(__MODE__)                                                                                   \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(T3MOD1_Page);                                                                              \
        ((__MODE__ ^ 0x00) == 0 ? (T3MOD1 = T3MOD1 & (~CP3S0)) : (__MODE__ ^ 0x01) == 0 ? (T3MOD1 = T3MOD1 | (CP3S0))  \
                                                             : (__MODE__ ^ 0x02) == 0   ? (T3MOD1 = T3MOD1 & (~CP3S0)) \
                                                             : (__MODE__ ^ 0x03) == 0   ? (T3MOD1 = T3MOD1 | (CP3S0))  \
                                                             : (__MODE__ ^ 0x04) == 0   ? (T3MOD1 = T3MOD1 & (~CP3S0)) \
                                                             : (__MODE__ ^ 0x05) == 0   ? (T3MOD1 = T3MOD1 | (CP3S0))  \
                                                             : (__MODE__ ^ 0x06) == 0   ? (T3MOD1 = T3MOD1 & (~CP3S0)) \
                                                             : (__MODE__ ^ 0x07) == 0   ? (T3MOD1 = T3MOD1 | (CP3S0))  \
                                                                                        : _nop_());                      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER3 TF3 Interrupt Ignored
* @details      Set TF3IG
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_TF3_Interrupt_Ignored_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_TF3_Interrupt_Ignored_Cmd(__STATE__)                            \
    MWT(                                                                             \
        __DRV_SFR_PageIndex(T3MOD1_Page);                                            \
        __STATE__ == 0 ? (T3MOD1 = T3MOD1 & (~TF3IG)) : (T3MOD1 = T3MOD1 | (TF3IG)); \
        __DRV_SFR_PageIndex(0);)

///@cond __DRV_TIMER3_Capture_Source_Dectect
#define Timer3_IGNORED 0x00
#define Timer3_RISING_EDGE 0x01
#define Timer3_FALLING_EDGE 0x02
#define Timer3_DUAL_EDGE 0x03
///@endcond
/**
*****************************************************************************
* @brief        TIMER3 Capture Source Dectect
* @details      Set EXEN3 T3EXH
* @param[in]    __MODE__ :
*  @arg\b       Timer3_IGNORED
*  @arg\b       Timer3_FALLING_EDGE
*  @arg\b       Timer3_RISING_EDGE
*  @arg\b       Timer3_DUAL_EDGE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_Capture_Source_Dectect(Timer3_IGNORED)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_Capture_Source_Dectect(__MODE__) \
    MWT(                                              \
        __DRV_TIMER3_SetEXEN3(__MODE__);              \
        __DRV_TIMER3_SetT3EXH(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER3 Capture Source Dectect
* @details      Set EXEN3
* @param[in]    __MODE__ :
*  @arg\b       Timer3_IGNORED
*  @arg\b       Timer3_FALLING_EDGE
*  @arg\b       Timer3_RISING_EDGE
*  @arg\b       Timer3_DUAL_EDGE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_SetEXEN3(Timer3_IGNORED)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_SetEXEN3(__MODE__)                                              \
    MWT(                                                                             \
        __DRV_SFR_PageIndex(T3CON_Page);                                             \
        ((__MODE__ ^ 0x00) == 0 ? (EXEN3 = 0) : (__MODE__ ^ 0x01) == 0 ? (EXEN3 = 0) \
                                            : (__MODE__ ^ 0x02) == 0   ? (EXEN3 = 1) \
                                            : (__MODE__ ^ 0x03) == 0   ? (EXEN3 = 1) \
                                                                       : _nop_());     \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER3 Capture Source Dectect
* @details      Set T3EXH
* @param[in]    __MODE__ :
*  @arg\b       Timer3_IGNORED
*  @arg\b       Timer3_FALLING_EDGE
*  @arg\b       Timer3_RISING_EDGE
*  @arg\b       Timer3_DUAL_EDGE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_SetT3EXH(Timer3_IGNORED)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_SetT3EXH(__MODE__)                                                                            \
    MWT(                                                                                                           \
        __DRV_SFR_PageIndex(T3MOD_Page);                                                                           \
        ((__MODE__ ^ 0x00) == 0 ? (T3MOD = T3MOD & (~T3EXH)) : (__MODE__ ^ 0x01) == 0 ? (T3MOD = T3MOD | (T3EXH))  \
                                                           : (__MODE__ ^ 0x02) == 0   ? (T3MOD = T3MOD & (~T3EXH)) \
                                                           : (__MODE__ ^ 0x03) == 0   ? (T3MOD = T3MOD | (T3EXH))  \
                                                                                      : _nop_());                    \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER3 High Byte Register
* @details      Set TH3
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_SetHighByte(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_SetHighByte(__RELOAD__) \
    MWT(                                     \
        __DRV_SFR_PageIndex(TH3_Page);       \
        TH3 = __RELOAD__;                    \
        __DRV_SFR_PageIndex(0);)
/**
*******************************************************************************
* @brief        TIMER3 Low Byte Register
* @details      Set TL3
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_SetLowByte(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_SetLowByte(__RELOAD__) \
    MWT(                                    \
        __DRV_SFR_PageIndex(TL3_Page);      \
        TL3 = __RELOAD__;                   \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER3 16 Bit Timer/Counter
* @details      Set TH3 TL3
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_Set16BitCounter(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_Set16BitCounter(__RELOAD__) \
    MWT(                                         \
        __DRV_SFR_PageIndex(TH3_Page);           \
        TH3 = HIBYTE(__RELOAD__);                \
        TL3 = LOBYTE(__RELOAD__) & 0xff;         \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER3 Capture High Byte Register
* @details      Set RCAP3H
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_SetRCAP3H(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_SetRCAP3H(__RELOAD__) \
    MWT(                                   \
        __DRV_SFR_PageIndex(RCAP3H_Page);  \
        RCAP3H = __RELOAD__;               \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER3 Capture Low Byte Register
* @details      Set RCAP3L
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_SetRCAP3L(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_SetRCAP3L(__RELOAD__) \
    MWT(                                   \
        __DRV_SFR_PageIndex(RCAP3L_Page);  \
        RCAP3L = __RELOAD__;               \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER3 16 Bit Capture Register
* @details      Set RCAP3H RCAP3L
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_Set16Bit_AutoReloadValue(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_Set16Bit_AutoReloadValue(__RELOAD__) \
    MWT(                                                  \
        __DRV_SFR_PageIndex(RCAP3H_Page);                 \
        RCAP3H = HIBYTE(__RELOAD__);                      \
        RCAP3L = LOBYTE(__RELOAD__) & 0xff;               \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 Operating Mode
* @details      Set T4SPL T4MS1 CP/RL4 T4MS0
* @param[in]    __MODE__ :
*  @arg\b       TIMER4_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER4_MODE1_16BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER4_MODE2_16BIT_CAPTURE
*  @arg\b       TIMER4_MODE3_16BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER4_MODE6_16BIT_DUTY_CAPTURE
*  @arg\b       TIMER4_SPLIT_MODE0_TWO_8BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER4_SPLIT_MODE1_TWO_8BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER4_SPLIT_MODE2_TWO_8BIT_CAPTURE
*  @arg\b       TIMER4_SPLIT_MODE3_TWO_8BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER4_SPLIT_MODE4_8BIT_PWM
*  @arg\b       TIMER4_SPLIT_MODE6_DUTY_CAPTURE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_Mode_Select(TIMER4_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER4_Mode_Select
#define TIMER4_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT 0x00
#define TIMER4_MODE1_16BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT 0x01
#define TIMER4_MODE2_16BIT_CAPTURE 0x02
#define TIMER4_MODE3_16BIT_CAPTURE_WITH_AUTO_ZERO 0x03
#define TIMER4_SPLIT_MODE0_TWO_8BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT 0x04
#define TIMER4_SPLIT_MODE1_TWO_8BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT 0x05
#define TIMER4_SPLIT_MODE2_TWO_8BIT_CAPTURE 0x06
#define TIMER4_SPLIT_MODE3_TWO_8BIT_CAPTURE_WITH_AUTO_ZERO 0x07
#define TIMER4_SPLIT_MODE4_8BIT_PWM 0x08
#define TIMER4_MODE6_DUTY_CAPTURE 0x09
#define TIMER4_SPLIT_MODE6_DUTY_CAPTURE 0x0A
///@endcond
#define __DRV_TIMER4_Mode_Select(__MODE__) \
    MWT(                                   \
        __DRV_TIMER4_SetT4SPL(__MODE__);   \
        __DRV_TIMER4_SetT4MS1(__MODE__);   \
        __DRV_TIMER4_SetCP_RL4(__MODE__);  \
        __DRV_TIMER4_SetT4MS0(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER4 Operating Mode
* @details      Set T4SPL
* @param[in]    __MODE__ :
*  @arg\b       TIMER4_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER4_MODE1_16BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER4_MODE2_16BIT_CAPTURE
*  @arg\b       TIMER4_MODE3_16BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER4_MODE3_16BIT_DUTY_CAPTURE
*  @arg\b       TIMER4_SPLIT_MODE0_TWO_8BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER4_SPLIT_MODE1_TWO_8BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER4_SPLIT_MODE2_TWO_8BIT_CAPTURE
*  @arg\b       TIMER4_SPLIT_MODE3_TWO_8BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER4_SPLIT_MODE4_8BIT_PWM
*  @arg\b       TIMER4_SPLIT_MODE6_DUTY_CAPTURE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_SetT4SPL(TIMER4_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_SetT4SPL(__MODE__)                                                                            \
    MWT(                                                                                                           \
        __DRV_SFR_PageIndex(T4MOD_Page);                                                                           \
        ((__MODE__ ^ 0x00) == 0 ? (T4MOD = T4MOD & (~T4SPL)) : (__MODE__ ^ 0x01) == 0 ? (T4MOD = T4MOD & (~T4SPL)) \
                                                           : (__MODE__ ^ 0x02) == 0   ? (T4MOD = T4MOD & (~T4SPL)) \
                                                           : (__MODE__ ^ 0x03) == 0   ? (T4MOD = T4MOD & (~T4SPL)) \
                                                           : (__MODE__ ^ 0x04) == 0   ? (T4MOD = T4MOD | (T4SPL))  \
                                                           : (__MODE__ ^ 0x05) == 0   ? (T4MOD = T4MOD | (T4SPL))  \
                                                           : (__MODE__ ^ 0x06) == 0   ? (T4MOD = T4MOD | (T4SPL))  \
                                                           : (__MODE__ ^ 0x07) == 0   ? (T4MOD = T4MOD | (T4SPL))  \
                                                           : (__MODE__ ^ 0x08) == 0   ? (T4MOD = T4MOD | (T4SPL))  \
                                                           : (__MODE__ ^ 0x09) == 0   ? (T4MOD = T4MOD & (~T4SPL)) \
                                                           : (__MODE__ ^ 0x0A) == 0   ? (T4MOD = T4MOD | (T4SPL))  \
                                                                                      : _nop_());                    \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 Operating Mode
* @details      Set T4MS1
* @param[in]    __MODE__ :
*  @arg\b       TIMER4_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER4_MODE1_16BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER4_MODE2_16BIT_CAPTURE
*  @arg\b       TIMER4_MODE3_16BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER4_MODE3_16BIT_DUTY_CAPTURE
*  @arg\b       TIMER4_SPLIT_MODE0_TWO_8BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER4_SPLIT_MODE1_TWO_8BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER4_SPLIT_MODE2_TWO_8BIT_CAPTURE
*  @arg\b       TIMER4_SPLIT_MODE3_TWO_8BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER4_SPLIT_MODE4_8BIT_PWM
*  @arg\b       TIMER4_SPLIT_MODE6_DUTY_CAPTURE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_SetT4MS1(TIMER4_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_SetT4MS1(__MODE__)                                                                                \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(T4MOD1_Page);                                                                              \
        ((__MODE__ ^ 0x00) == 0 ? (T4MOD1 = T4MOD1 & (~T4MS1)) : (__MODE__ ^ 0x01) == 0 ? (T4MOD1 = T4MOD1 & (~T4MS1)) \
                                                             : (__MODE__ ^ 0x02) == 0   ? (T4MOD1 = T4MOD1 & (~T4MS1)) \
                                                             : (__MODE__ ^ 0x03) == 0   ? (T4MOD1 = T4MOD1 & (~T4MS1)) \
                                                             : (__MODE__ ^ 0x04) == 0   ? (T4MOD1 = T4MOD1 & (~T4MS1)) \
                                                             : (__MODE__ ^ 0x05) == 0   ? (T4MOD1 = T4MOD1 & (~T4MS1)) \
                                                             : (__MODE__ ^ 0x06) == 0   ? (T4MOD1 = T4MOD1 & (~T4MS1)) \
                                                             : (__MODE__ ^ 0x07) == 0   ? (T4MOD1 = T4MOD1 & (~T4MS1)) \
                                                             : (__MODE__ ^ 0x08) == 0   ? (T4MOD1 = T4MOD1 | (T4MS1))  \
                                                             : (__MODE__ ^ 0x09) == 0   ? (T4MOD1 = T4MOD1 | (T4MS1))  \
                                                             : (__MODE__ ^ 0x0A) == 0   ? (T4MOD1 = T4MOD1 | (T4MS1))  \
                                                                                        : _nop_());                      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 Operating Mode
* @details      Set CP/RL4
* @param[in]    __MODE__ :
*  @arg\b       TIMER4_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER4_MODE1_16BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER4_MODE2_16BIT_CAPTURE
*  @arg\b       TIMER4_MODE3_16BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER4_MODE3_16BIT_DUTY_CAPTURE
*  @arg\b       TIMER4_SPLIT_MODE0_TWO_8BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER4_SPLIT_MODE1_TWO_8BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER4_SPLIT_MODE2_TWO_8BIT_CAPTURE
*  @arg\b       TIMER4_SPLIT_MODE3_TWO_8BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER4_SPLIT_MODE4_8BIT_PWM
*  @arg\b       TIMER4_SPLIT_MODE6_DUTY_CAPTURE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_SetCP_RL4(TIMER4_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_SetCP_RL4(__MODE__)                                               \
    MWT(                                                                               \
        __DRV_SFR_PageIndex(T4CON_Page);                                               \
        ((__MODE__ ^ 0x00) == 0 ? (CP_RL4 = 0) : (__MODE__ ^ 0x01) == 0 ? (CP_RL4 = 0) \
                                             : (__MODE__ ^ 0x02) == 0   ? (CP_RL4 = 1) \
                                             : (__MODE__ ^ 0x03) == 0   ? (CP_RL4 = 1) \
                                             : (__MODE__ ^ 0x04) == 0   ? (CP_RL4 = 0) \
                                             : (__MODE__ ^ 0x05) == 0   ? (CP_RL4 = 0) \
                                             : (__MODE__ ^ 0x06) == 0   ? (CP_RL4 = 1) \
                                             : (__MODE__ ^ 0x07) == 0   ? (CP_RL4 = 1) \
                                             : (__MODE__ ^ 0x08) == 0   ? (CP_RL4 = 0) \
                                             : (__MODE__ ^ 0x09) == 0   ? (CP_RL4 = 1) \
                                             : (__MODE__ ^ 0x0A) == 0   ? (CP_RL4 = 1) \
                                                                        : _nop_());      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 Operating Mode
* @details      Set T3MS0
* @param[in]    __MODE__ :
*  @arg\b       TIMER4_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER4_MODE1_16BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER4_MODE2_16BIT_CAPTURE
*  @arg\b       TIMER4_MODE3_16BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER4_MODE3_16BIT_DUTY_CAPTURE
*  @arg\b       TIMER4_SPLIT_MODE0_TWO_8BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT
*  @arg\b       TIMER4_SPLIT_MODE1_TWO_8BIT_AUTORELOAD_WITH_EXTERNAL_INTERRUPT
*  @arg\b       TIMER4_SPLIT_MODE2_TWO_8BIT_CAPTURE
*  @arg\b       TIMER4_SPLIT_MODE3_TWO_8BIT_CAPTURE_WITH_AUTO_ZERO
*  @arg\b       TIMER4_SPLIT_MODE4_8BIT_PWM
*  @arg\b       TIMER4_SPLIT_MODE6_DUTY_CAPTURE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_SetT4MS0(TIMER4_MODE0_16BIT_AUTORELOAD_AND_EXTERNAL_INTERRUPT)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_SetT4MS0(__MODE__)                                                                            \
    MWT(                                                                                                           \
        __DRV_SFR_PageIndex(T4MOD_Page);                                                                           \
        ((__MODE__ ^ 0x00) == 0 ? (T4MOD = T4MOD & (~T4MS0)) : (__MODE__ ^ 0x01) == 0 ? (T4MOD = T4MOD | (T4MS0))  \
                                                           : (__MODE__ ^ 0x02) == 0   ? (T4MOD = T4MOD & (~T4MS0)) \
                                                           : (__MODE__ ^ 0x03) == 0   ? (T4MOD = T4MOD | (T4MS0))  \
                                                           : (__MODE__ ^ 0x04) == 0   ? (T4MOD = T4MOD & (~T4MS0)) \
                                                           : (__MODE__ ^ 0x05) == 0   ? (T4MOD = T4MOD | (T4MS0))  \
                                                           : (__MODE__ ^ 0x06) == 0   ? (T4MOD = T4MOD & (~T4MS0)) \
                                                           : (__MODE__ ^ 0x07) == 0   ? (T4MOD = T4MOD | (T4MS0))  \
                                                           : (__MODE__ ^ 0x08) == 0   ? (T4MOD = T4MOD & (~T4MS0)) \
                                                           : (__MODE__ ^ 0x09) == 0   ? (T4MOD = T4MOD & (~T4MS0)) \
                                                           : (__MODE__ ^ 0x0A) == 0   ? (T4MOD = T4MOD & (~T4MS0)) \
                                                                                      : _nop_());                    \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 Clock Source
* @details      Set T4CKS T4X12 C/T4
* @param[in]    __MODE__ :
*  @arg\b       TIMER4_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER4_CLOCK_SOURCE_T4_PIN
*  @arg\b       TIMER4_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER4_CLOCK_SOURCE_INT2ET
*  @arg\b       TIMER4_CLOCK_SOURCE_RESERVED
*  @arg\b       TIMER4_CLOCK_SOURCE_T0OF
*  @arg\b       TIMER4_CLOCK_SOURCE_AC2ES
*  @arg\b       TIMER4_CLOCK_SOURCE_AC0ES
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_Clock_Source_Select(TIMER4_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER4_Clock_Source_Select
#define TIMER4_CLOCK_SOURCE_SYSCLK_DIV_12 0x00
#define TIMER4_CLOCK_SOURCE_T4_PIN 0x01
#define TIMER4_CLOCK_SOURCE_SYSCLK 0x02
#define TIMER4_CLOCK_SOURCE_INT2ET 0x03
#define TIMER4_CLOCK_SOURCE_RESERVED 0x04
#define TIMER4_CLOCK_SOURCE_T0OF 0x05
#define TIMER4_CLOCK_SOURCE_AC2ES 0x06
#define TIMER4_CLOCK_SOURCE_AC0ES 0x07
///@endcond
#define __DRV_TIMER4_Clock_Source_Select(__MODE__) \
    MWT(                                           \
        __DRV_TIMER4_SetT4CKS(__MODE__);           \
        __DRV_TIMER4_SetT4X12(__MODE__);           \
        __DRV_TIMER4_SetC_T4(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER5 Clock Source
* @details      Set T5CKS0 T5CKS1
* @param[in]    __MODE__ :
*  @arg\b       TIMER5_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER5_CLOCK_SOURCE_T5_PIN
*  @arg\b       TIMER5_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER5_CLOCK_SOURCE_T2EXI
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER5_Clock_Source_Select(TIMER5_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER5_Clock_Source_Select
#define TIMER5_CLOCK_SOURCE_SYSCLK_DIV_12 0x00
#define TIMER5_CLOCK_SOURCE_T5_PIN 0x01
#define TIMER5_CLOCK_SOURCE_SYSCLK 0x02
#define TIMER5_CLOCK_SOURCE_T2EXI 0x03
///@endcond
#define __DRV_TIMER5_Clock_Source_Select(__MODE__) \
    MWT(                                           \
        __DRV_TIMER5_SetT5CKS0(__MODE__);          \
        __DRV_TIMER5_SetT5CKS1(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER5 Clock Source
* @details      Set T5CKS0
* @param[in]    __MODE__ :
*  @arg\b       TIMER5_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER5_CLOCK_SOURCE_T5_PIN
*  @arg\b       TIMER5_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER5_CLOCK_SOURCE_T2EXI
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER5_SetT5CKS0(TIMER5_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER5_SetT5CKS0(__MODE__)                                               \
    MWT(                                                                               \
        __DRV_SFR_PageIndex(T5CON_Page);                                               \
        ((__MODE__ ^ 0x00) == 0 ? (T5CKS0 = 0) : (__MODE__ ^ 0x01) == 0 ? (T5CKS0 = 1) \
                                             : (__MODE__ ^ 0x02) == 0   ? (T5CKS0 = 0) \
                                             : (__MODE__ ^ 0x03) == 0   ? (T5CKS0 = 1) \
                                                                        : _nop_());      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER5 Clock Source
* @details      Set T5CKS1
* @param[in]    __MODE__ :
*  @arg\b       TIMER5_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER5_CLOCK_SOURCE_T5_PIN
*  @arg\b       TIMER5_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER5_CLOCK_SOURCE_T2EXI
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER5_SetT5CKS1(TIMER5_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER5_SetT5CKS1(__MODE__)                                               \
    MWT(                                                                               \
        __DRV_SFR_PageIndex(T5CON_Page);                                               \
        ((__MODE__ ^ 0x00) == 0 ? (T5CKS1 = 0) : (__MODE__ ^ 0x01) == 0 ? (T5CKS1 = 0) \
                                             : (__MODE__ ^ 0x02) == 0   ? (T5CKS1 = 1) \
                                             : (__MODE__ ^ 0x03) == 0   ? (T5CKS1 = 1) \
                                                                        : _nop_());      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER6 Clock Source
* @details      Set T6CKS0 T6CKS1
* @param[in]    __MODE__ :
*  @arg\b       TIMER6_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER6_CLOCK_SOURCE_T6_PIN
*  @arg\b       TIMER6_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER6_CLOCK_SOURCE_T3EXI
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER5_Clock_Source_Select(TIMER6_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER6_Clock_Source_Select
#define TIMER6_CLOCK_SOURCE_SYSCLK_DIV_12 0x00
#define TIMER6_CLOCK_SOURCE_T6_PIN 0x01
#define TIMER6_CLOCK_SOURCE_SYSCLK 0x02
#define TIMER6_CLOCK_SOURCE_T3EXI 0x03
///@endcond
#define __DRV_TIMER6_Clock_Source_Select(__MODE__) \
    MWT(                                           \
        __DRV_TIMER6_SetT6CKS0(__MODE__);          \
        __DRV_TIMER6_SetT6CKS1(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER6 Clock Source
* @details      Set T6CKS0
* @param[in]    __MODE__ :
*  @arg\b       TIMER6_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER6_CLOCK_SOURCE_T6_PIN
*  @arg\b       TIMER6_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER6_CLOCK_SOURCE_T3EXI
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER6_SetT6CKS0(TIMER6_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER6_SetT6CKS0(__MODE__)                                               \
    MWT(                                                                               \
        __DRV_SFR_PageIndex(T6CON_Page);                                               \
        ((__MODE__ ^ 0x00) == 0 ? (T6CKS0 = 0) : (__MODE__ ^ 0x01) == 0 ? (T6CKS0 = 1) \
                                             : (__MODE__ ^ 0x02) == 0   ? (T6CKS0 = 0) \
                                             : (__MODE__ ^ 0x03) == 0   ? (T6CKS0 = 1) \
                                                                        : _nop_());      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER6 Clock Source
* @details      Set T6CKS1
* @param[in]    __MODE__ :
*  @arg\b       TIMER6_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER6_CLOCK_SOURCE_T6_PIN
*  @arg\b       TIMER6_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER6_CLOCK_SOURCE_T3EXI
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER6_SetT6CKS1(TIMER6_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER6_SetT6CKS1(__MODE__)                                               \
    MWT(                                                                               \
        __DRV_SFR_PageIndex(T6CON_Page);                                               \
        ((__MODE__ ^ 0x00) == 0 ? (T6CKS1 = 0) : (__MODE__ ^ 0x01) == 0 ? (T6CKS1 = 0) \
                                             : (__MODE__ ^ 0x02) == 0   ? (T6CKS1 = 1) \
                                             : (__MODE__ ^ 0x03) == 0   ? (T6CKS1 = 1) \
                                                                        : _nop_());      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 Clock Source
* @details      Set T4CKS
* @param[in]    __MODE__ :
*  @arg\b       TIMER4_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER4_CLOCK_SOURCE_T4_PIN
*  @arg\b       TIMER4_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER4_CLOCK_SOURCE_INT2ET
*  @arg\b       TIMER4_CLOCK_SOURCE_RESERVED
*  @arg\b       TIMER4_CLOCK_SOURCE_T0OF
*  @arg\b       TIMER4_CLOCK_SOURCE_AC2ES
*  @arg\b       TIMER4_CLOCK_SOURCE_AC0ES
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_SetT4CKS(TIMER4_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_SetT4CKS(__MODE__)                                                                                \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(T4MOD1_Page);                                                                              \
        ((__MODE__ ^ 0x00) == 0 ? (T4MOD1 = T4MOD1 & (~T4CKS)) : (__MODE__ ^ 0x01) == 0 ? (T4MOD1 = T4MOD1 & (~T4CKS)) \
                                                             : (__MODE__ ^ 0x02) == 0   ? (T4MOD1 = T4MOD1 & (~T4CKS)) \
                                                             : (__MODE__ ^ 0x03) == 0   ? (T4MOD1 = T4MOD1 & (~T4CKS)) \
                                                             : (__MODE__ ^ 0x04) == 0   ? (T4MOD1 = T4MOD1 | (T4CKS))  \
                                                             : (__MODE__ ^ 0x05) == 0   ? (T4MOD1 = T4MOD1 | (T4CKS))  \
                                                             : (__MODE__ ^ 0x06) == 0   ? (T4MOD1 = T4MOD1 | (T4CKS))  \
                                                             : (__MODE__ ^ 0x07) == 0   ? (T4MOD1 = T4MOD1 | (T4CKS))  \
                                                                                        : _nop_());                      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 Clock Source
* @details      Set T4X12
* @param[in]    __MODE__ :
*  @arg\b       TIMER4_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER4_CLOCK_SOURCE_T4_PIN
*  @arg\b       TIMER4_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER4_CLOCK_SOURCE_INT2ET
*  @arg\b       TIMER4_CLOCK_SOURCE_RESERVED
*  @arg\b       TIMER4_CLOCK_SOURCE_T0OF
*  @arg\b       TIMER4_CLOCK_SOURCE_AC2ES
*  @arg\b       TIMER4_CLOCK_SOURCE_AC0ES
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_SetT4X12(TIMER4_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_SetT4X12(__MODE__)                                                                            \
    MWT(                                                                                                           \
        __DRV_SFR_PageIndex(T4MOD_Page);                                                                           \
        ((__MODE__ ^ 0x00) == 0 ? (T4MOD = T4MOD & (~T4X12)) : (__MODE__ ^ 0x01) == 0 ? (T4MOD = T4MOD & (~T4X12)) \
                                                           : (__MODE__ ^ 0x02) == 0   ? (T4MOD = T4MOD | (T4X12))  \
                                                           : (__MODE__ ^ 0x03) == 0   ? (T4MOD = T4MOD | (T4X12))  \
                                                           : (__MODE__ ^ 0x04) == 0   ? (T4MOD = T4MOD & (~T4X12)) \
                                                           : (__MODE__ ^ 0x05) == 0   ? (T4MOD = T4MOD & (~T4X12)) \
                                                           : (__MODE__ ^ 0x06) == 0   ? (T4MOD = T4MOD | (T4X12))  \
                                                           : (__MODE__ ^ 0x07) == 0   ? (T4MOD = T4MOD | (T4X12))  \
                                                                                      : _nop_());                    \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 Clock Source
* @details      Set C/T4
* @param[in]    __MODE__ :
*  @arg\b       TIMER4_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER4_CLOCK_SOURCE_T4_PIN
*  @arg\b       TIMER4_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER4_CLOCK_SOURCE_INT2ET
*  @arg\b       TIMER4_CLOCK_SOURCE_RESERVED
*  @arg\b       TIMER4_CLOCK_SOURCE_T0OF
*  @arg\b       TIMER4_CLOCK_SOURCE_AC2ES
*  @arg\b       TIMER4_CLOCK_SOURCE_AC0ES
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_SetC_T4(TIMER4_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_SetC_T4(__MODE__)                                             \
    MWT(                                                                           \
        __DRV_SFR_PageIndex(T4CON_Page);                                           \
        ((__MODE__ ^ 0x00) == 0 ? (C_T4 = 0) : (__MODE__ ^ 0x01) == 0 ? (C_T4 = 1) \
                                           : (__MODE__ ^ 0x02) == 0   ? (C_T4 = 0) \
                                           : (__MODE__ ^ 0x03) == 0   ? (C_T4 = 1) \
                                           : (__MODE__ ^ 0x04) == 0   ? (C_T4 = 0) \
                                           : (__MODE__ ^ 0x05) == 0   ? (C_T4 = 1) \
                                           : (__MODE__ ^ 0x06) == 0   ? (C_T4 = 0) \
                                           : (__MODE__ ^ 0x07) == 0   ? (C_T4 = 1) \
                                                                      : _nop_());    \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 TH4 Clock Source in Split Mode
* @details      Set T4CKS T4X12 C/T4
* @param[in]    __MODE__ :
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_T4_PIN
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_TL4OF
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_RESERVED
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_T0OF
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_AC2ES
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_AC0ES
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_TH4_Clock_Source_Select(TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER4_TH4_Clock_Source_Select
#define TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12 0x00
#define TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_T4_PIN 0x01
#define TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK 0x02
#define TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_TL4OF 0x03
#define TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_RESERVED 0x04
#define TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_T0OF 0x05
#define TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_AC2ES 0x06
#define TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_AC0ES 0x07
///@endcond
#define __DRV_TIMER4_TH4_Clock_Source_Select(__MODE__) \
    MWT(                                               \
        __DRV_TIMER4_TH4_SetT4CKS(__MODE__);           \
        __DRV_TIMER4_TH4_SetT4X12(__MODE__);           \
        __DRV_TIMER4_TH4_SetC_T4(__MODE__);)
/**
*****************************************************************************
* @brief        TIMER4 TH4 Clock Source in Split Mode
* @details      Set T4CKS
* @param[in]    __MODE__ :
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_T4_PIN
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_TL4OF
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_RESERVED
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_T0OF
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_AC2ES
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_AC0ES
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_TH4_SetT4CKS(TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_TH4_SetT4CKS(__MODE__)                                                                            \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(T4MOD1_Page);                                                                              \
        ((__MODE__ ^ 0x00) == 0 ? (T4MOD1 = T4MOD1 & (~T4CKS)) : (__MODE__ ^ 0x01) == 0 ? (T4MOD1 = T4MOD1 & (~T4CKS)) \
                                                             : (__MODE__ ^ 0x02) == 0   ? (T4MOD1 = T4MOD1 & (~T4CKS)) \
                                                             : (__MODE__ ^ 0x03) == 0   ? (T4MOD1 = T4MOD1 & (~T4CKS)) \
                                                             : (__MODE__ ^ 0x04) == 0   ? (T4MOD1 = T4MOD1 | (T4CKS))  \
                                                             : (__MODE__ ^ 0x05) == 0   ? (T4MOD1 = T4MOD1 | (T4CKS))  \
                                                             : (__MODE__ ^ 0x06) == 0   ? (T4MOD1 = T4MOD1 | (T4CKS))  \
                                                             : (__MODE__ ^ 0x07) == 0   ? (T4MOD1 = T4MOD1 | (T4CKS))  \
                                                                                        : _nop_());                      \
        __DRV_SFR_PageIndex(0);)
/**
*****************************************************************************
* @brief        TIMER4 TH4 Clock Source in Split Mode
* @details      Set T4X12
* @param[in]    __MODE__ :
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_T4_PIN
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_TL4OF
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_RESERVED
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_T0OF
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_AC2ES
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_AC0ES
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_TH4_SetT4X12(TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_TH4_SetT4X12(__MODE__)                                                                        \
    MWT(                                                                                                           \
        __DRV_SFR_PageIndex(T4MOD_Page);                                                                           \
        ((__MODE__ ^ 0x00) == 0 ? (T4MOD = T4MOD & (~T4X12)) : (__MODE__ ^ 0x01) == 0 ? (T4MOD = T4MOD & (~T4X12)) \
                                                           : (__MODE__ ^ 0x02) == 0   ? (T4MOD = T4MOD | (T4X12))  \
                                                           : (__MODE__ ^ 0x03) == 0   ? (T4MOD = T4MOD | (T4X12))  \
                                                           : (__MODE__ ^ 0x04) == 0   ? (T4MOD = T4MOD & (~T4X12)) \
                                                           : (__MODE__ ^ 0x05) == 0   ? (T4MOD = T4MOD & (~T4X12)) \
                                                           : (__MODE__ ^ 0x06) == 0   ? (T4MOD = T4MOD | (T4X12))  \
                                                           : (__MODE__ ^ 0x07) == 0   ? (T4MOD = T4MOD | (T4X12))  \
                                                                                      : _nop_());                    \
        __DRV_SFR_PageIndex(0);)
/**
*****************************************************************************
* @brief        TIMER4 TH4 Clock Source in Split Mode
* @details      Set C/T4
* @param[in]    __MODE__ :
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_T4_PIN
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_TL4OF
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_RESERVED
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_T0OF
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_AC2ES
*  @arg\b       TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_AC0ES
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_TH4_SetC_T4(TIMER4_TH4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_TH4_SetC_T4(__MODE__)                                         \
    MWT(                                                                           \
        __DRV_SFR_PageIndex(T4CON_Page);                                           \
        ((__MODE__ ^ 0x00) == 0 ? (C_T4 = 0) : (__MODE__ ^ 0x01) == 0 ? (C_T4 = 1) \
                                           : (__MODE__ ^ 0x02) == 0   ? (C_T4 = 0) \
                                           : (__MODE__ ^ 0x03) == 0   ? (C_T4 = 1) \
                                           : (__MODE__ ^ 0x04) == 0   ? (C_T4 = 0) \
                                           : (__MODE__ ^ 0x05) == 0   ? (C_T4 = 1) \
                                           : (__MODE__ ^ 0x06) == 0   ? (C_T4 = 0) \
                                           : (__MODE__ ^ 0x07) == 0   ? (C_T4 = 1) \
                                                                      : _nop_());    \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 TL4 Clock Source in Split Mode
* @details      Set TL4CS TL4X12
* @param[in]    __MODE__ :
*  @arg\b       TIMER4_TL4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER4_TL4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER4_TL4_SPLIT_MODE_CLOCK_SOURCE_RESERVED
*  @arg\b       TIMER4_TL4_SPLIT_MODE_CLOCK_SOURCE_INT2ET
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_TL4_Clock_Source_Select(TIMER4_TL4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER4_TL4_Clock_Source_Select
#define TIMER4_TL4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12 0x00
#define TIMER4_TL4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK 0x01
#define TIMER4_TL4_SPLIT_MODE_CLOCK_SOURCE_RESERVED 0x02
#define TIMER4_TL4_SPLIT_MODE_CLOCK_SOURCE_INT2ET 0x03
///@endcond
#define __DRV_TIMER4_TL4_Clock_Source_Select(__MODE__) \
    MWT(                                               \
        __DRV_TIMER4_TL4_SetTL4CS(__MODE__);           \
        __DRV_TIMER4_TL4_SetTL4X12(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER4 TL4 Clock Source in Split Mode
* @details      Set TL4CS
* @param[in]    __MODE__ :
*  @arg\b       TIMER4_TL4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER4_TL4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER4_TL4_SPLIT_MODE_CLOCK_SOURCE_RESERVED
*  @arg\b       TIMER4_TL4_SPLIT_MODE_CLOCK_SOURCE_INT2ET
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_TL3_SetTL3CS(TIMER3_TL3_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_TL4_SetTL4CS(__MODE__)                                                                            \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(T4MOD1_Page);                                                                              \
        ((__MODE__ ^ 0x00) == 0 ? (T4MOD1 = T4MOD1 & (~TL4CS)) : (__MODE__ ^ 0x01) == 0 ? (T4MOD1 = T4MOD1 & (~TL4CS)) \
                                                             : (__MODE__ ^ 0x02) == 0   ? (T4MOD1 = T4MOD1 | (TL4CS))  \
                                                             : (__MODE__ ^ 0x03) == 0   ? (T4MOD1 = T4MOD1 | (TL4CS))  \
                                                                                        : _nop_());                      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TL4 Clock Source in Split Mode
* @details      Set TL4X12
* @param[in]    __MODE__ :
*  @arg\b       TIMER4_TL4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12
*  @arg\b       TIMER4_TL4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK
*  @arg\b       TIMER4_TL4_SPLIT_MODE_CLOCK_SOURCE_RESERVED
*  @arg\b       TIMER4_TL4_SPLIT_MODE_CLOCK_SOURCE_INT2ET
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_TL4_SetTL4X12(TIMER4_TL4_SPLIT_MODE_CLOCK_SOURCE_SYSCLK_DIV_12)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_TL4_SetTL4X12(__MODE__)                                                                                     \
    MWT(                                                                                                                         \
        __DRV_SFR_PageIndex(T4MOD_Page);                                                                                         \
        ((__MODE__ ^ 0x00) == 0 ? (T4MOD = T4MOD & (~TL4X12_T4EIP)) : (__MODE__ ^ 0x01) == 0 ? (T4MOD = T4MOD | (TL4X12_T4EIP))  \
                                                                  : (__MODE__ ^ 0x02) == 0   ? (T4MOD = T4MOD & (~TL4X12_T4EIP)) \
                                                                  : (__MODE__ ^ 0x03) == 0   ? (T4MOD = T4MOD | (TL4X12_T4EIP))  \
                                                                                             : _nop_());                           \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 Capture Source
* @details      Set CP4S2 CP4S1 CP4S0
* @param[in]    __MODE__ :
*  @arg\b       TIMER4_CAPTURE_SOURCE_T4EX_PIN
*  @arg\b       TIMER4_CAPTURE_SOURCE_NSS_PIN
*  @arg\b       TIMER4_CAPTURE_SOURCE_INT3ET
*  @arg\b       TIMER4_CAPTURE_SOURCE_RESERVED
*  @arg\b       TIMER4_CAPTURE_SOURCE_RESERVED
*  @arg\b       TIMER4_CAPTURE_SOURCE_AC0OUT
*  @arg\b       TIMER4_CAPTURE_SOURCE_AC2OUT
*  @arg\b       TIMER4_CAPTURE_SOURCE_TWI1_SCL
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_Capture_Source_Select(TIMER4_CAPTURE_SOURCE_T4EX_PIN)
* @endcode
*******************************************************************************
*/
///@cond __DRV_TIMER4_Capture_Source_Select
#define TIMER4_CAPTURE_SOURCE_T4EX_PIN 0x00
#define TIMER4_CAPTURE_SOURCE_NSS_PIN 0x01
#define TIMER4_CAPTURE_SOURCE_INT3ET 0x02
#define TIMER4_CAPTURE_SOURCE_RESERVED 0x03
#define TIMER4_CAPTURE_SOURCE_RESERVED2 0x04
#define TIMER4_CAPTURE_SOURCE_AC0OUT 0x05
#define TIMER4_CAPTURE_SOURCE_AC2OUT 0x06
#define TIMER4_CAPTURE_SOURCE_TWI1_SCL 0x07
///@endcond
#define __DRV_TIMER4_Capture_Source_Select(__MODE__) \
    MWT(                                             \
        __DRV_TH4_SetCP4S2(__MODE__);                \
        __DRV_TH4_SetCP4S1(__MODE__);                \
        __DRV_TH4_SetCP4S0(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER4 Capture Source
* @details      Set CP4S2
* @param[in]    __MODE__ :
*  @arg\b       TIMER4_CAPTURE_SOURCE_T4EX_PIN
*  @arg\b       TIMER4_CAPTURE_SOURCE_NSS_PIN
*  @arg\b       TIMER4_CAPTURE_SOURCE_INT3ET
*  @arg\b       TIMER4_CAPTURE_SOURCE_RESERVED
*  @arg\b       TIMER4_CAPTURE_SOURCE_RESERVED
*  @arg\b       TIMER4_CAPTURE_SOURCE_AC0OUT
*  @arg\b       TIMER4_CAPTURE_SOURCE_AC2OUT
*  @arg\b       TIMER4_CAPTURE_SOURCE_TWI1_SCL
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_SetCP4S2(TIMER4_CAPTURE_SOURCE_T4EX_PIN)
* @endcode
*******************************************************************************
*/
#define __DRV_TH4_SetCP4S2(__MODE__)                                                                                   \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(T4MOD1_Page);                                                                              \
        ((__MODE__ ^ 0x00) == 0 ? (T4MOD1 = T4MOD1 & (~CP4S2)) : (__MODE__ ^ 0x01) == 0 ? (T4MOD1 = T4MOD1 & (~CP4S2)) \
                                                             : (__MODE__ ^ 0x02) == 0   ? (T4MOD1 = T4MOD1 & (~CP4S2)) \
                                                             : (__MODE__ ^ 0x03) == 0   ? (T4MOD1 = T4MOD1 & (~CP4S2)) \
                                                             : (__MODE__ ^ 0x04) == 0   ? (T4MOD1 = T4MOD1 | (CP4S2))  \
                                                             : (__MODE__ ^ 0x05) == 0   ? (T4MOD1 = T4MOD1 | (CP4S2))  \
                                                             : (__MODE__ ^ 0x06) == 0   ? (T4MOD1 = T4MOD1 | (CP4S2))  \
                                                             : (__MODE__ ^ 0x07) == 0   ? (T4MOD1 = T4MOD1 | (CP4S2))  \
                                                                                        : _nop_());                      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 Capture Source
* @details      Set CP4S1
* @param[in]    __MODE__ :
*  @arg\b       TIMER4_CAPTURE_SOURCE_T4EX_PIN
*  @arg\b       TIMER4_CAPTURE_SOURCE_NSS_PIN
*  @arg\b       TIMER4_CAPTURE_SOURCE_INT3ET
*  @arg\b       TIMER4_CAPTURE_SOURCE_RESERVED
*  @arg\b       TIMER4_CAPTURE_SOURCE_RESERVED
*  @arg\b       TIMER4_CAPTURE_SOURCE_AC0OUT
*  @arg\b       TIMER4_CAPTURE_SOURCE_AC2OUT
*  @arg\b       TIMER4_CAPTURE_SOURCE_TWI1_SCL
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_SetCP4S1(TIMER4_CAPTURE_SOURCE_T4EX_PIN)
* @endcode
*******************************************************************************
*/
#define __DRV_TH4_SetCP4S1(__MODE__)                                                                                   \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(T4MOD1_Page);                                                                              \
        ((__MODE__ ^ 0x00) == 0 ? (T4MOD1 = T4MOD1 & (~CP4S1)) : (__MODE__ ^ 0x01) == 0 ? (T4MOD1 = T4MOD1 & (~CP4S1)) \
                                                             : (__MODE__ ^ 0x02) == 0   ? (T4MOD1 = T4MOD1 | (CP4S1))  \
                                                             : (__MODE__ ^ 0x03) == 0   ? (T4MOD1 = T4MOD1 | (CP4S1))  \
                                                             : (__MODE__ ^ 0x04) == 0   ? (T4MOD1 = T4MOD1 & (~CP4S1)) \
                                                             : (__MODE__ ^ 0x05) == 0   ? (T4MOD1 = T4MOD1 & (~CP4S1)) \
                                                             : (__MODE__ ^ 0x06) == 0   ? (T4MOD1 = T4MOD1 | (CP4S1))  \
                                                             : (__MODE__ ^ 0x07) == 0   ? (T4MOD1 = T4MOD1 | (CP4S1))  \
                                                                                        : _nop_());                      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 Capture Source
* @details      Set CP4S0
* @param[in]    __MODE__ :
*  @arg\b       TIMER4_CAPTURE_SOURCE_T4EX_PIN
*  @arg\b       TIMER4_CAPTURE_SOURCE_NSS_PIN
*  @arg\b       TIMER4_CAPTURE_SOURCE_INT3ET
*  @arg\b       TIMER4_CAPTURE_SOURCE_RESERVED
*  @arg\b       TIMER4_CAPTURE_SOURCE_RESERVED
*  @arg\b       TIMER4_CAPTURE_SOURCE_AC0OUT
*  @arg\b       TIMER4_CAPTURE_SOURCE_AC2OUT
*  @arg\b       TIMER4_CAPTURE_SOURCE_TWI1_SCL
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_SetCP4S0(TIMER4_CAPTURE_SOURCE_T4EX_PIN)
* @endcode
*******************************************************************************
*/
#define __DRV_TH4_SetCP4S0(__MODE__)                                                                                   \
    MWT(                                                                                                               \
        __DRV_SFR_PageIndex(T4MOD1_Page);                                                                              \
        ((__MODE__ ^ 0x00) == 0 ? (T4MOD1 = T4MOD1 & (~CP4S0)) : (__MODE__ ^ 0x01) == 0 ? (T4MOD1 = T4MOD1 | (CP4S0))  \
                                                             : (__MODE__ ^ 0x02) == 0   ? (T4MOD1 = T4MOD1 & (~CP4S0)) \
                                                             : (__MODE__ ^ 0x03) == 0   ? (T4MOD1 = T4MOD1 | (CP4S0))  \
                                                             : (__MODE__ ^ 0x04) == 0   ? (T4MOD1 = T4MOD1 & (~CP4S0)) \
                                                             : (__MODE__ ^ 0x05) == 0   ? (T4MOD1 = T4MOD1 | (CP4S0))  \
                                                             : (__MODE__ ^ 0x06) == 0   ? (T4MOD1 = T4MOD1 & (~CP4S0)) \
                                                             : (__MODE__ ^ 0x07) == 0   ? (T4MOD1 = T4MOD1 | (CP4S0))  \
                                                                                        : _nop_());                      \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 TF4 Interrupt Ignored
* @details      Set TF4IG
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_TF4_Interrupt_Ignored_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_TF4_Interrupt_Ignored_Cmd(__STATE__)                            \
    MWT(                                                                             \
        __DRV_SFR_PageIndex(T4MOD1_Page);                                            \
        __STATE__ == 0 ? (T4MOD1 = T4MOD1 & (~TF4IG)) : (T4MOD1 = T4MOD1 | (TF4IG)); \
        __DRV_SFR_PageIndex(0);)

///@cond __DRV_TIMER4_Capture_Source_Dectect
#define Timer4_IGNORED 0x00
#define Timer4_RISING_EDGE 0x01
#define Timer4_FALLING_EDGE 0x02
#define Timer4_DUAL_EDGE 0x03
///@endcond

/**
*****************************************************************************
* @brief        TIMER4 Capture Source Dectect
* @details      Set EXEN4 T4EXH
* @param[in]    __MODE__ :
*  @arg\b       Timer4_IGNORED
*  @arg\b       Timer4_FALLING_EDGE
*  @arg\b       Timer4_RISING_EDGE
*  @arg\b       Timer4_DUAL_EDGE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_Capture_Source_Dectect(Timer4_IGNORED)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_Capture_Source_Dectect(__MODE__) \
    MWT(                                              \
        __DRV_TIMER4_SetEXEN4(__MODE__);              \
        __DRV_TIMER4_SetT4EXH(__MODE__);)

/**
*****************************************************************************
* @brief        TIMER4 Capture Source Dectect
* @details      Set EXEN4
* @param[in]    __MODE__ :
*  @arg\b       Timer4_IGNORED
*  @arg\b       Timer4_FALLING_EDGE
*  @arg\b       Timer4_RISING_EDGE
*  @arg\b       Timer4_DUAL_EDGE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_SetEXEN4(Timer4_IGNORED)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_SetEXEN4(__MODE__)                                              \
    MWT(                                                                             \
        __DRV_SFR_PageIndex(T4CON_Page);                                             \
        ((__MODE__ ^ 0x00) == 0 ? (EXEN4 = 0) : (__MODE__ ^ 0x01) == 0 ? (EXEN4 = 0) \
                                            : (__MODE__ ^ 0x02) == 0   ? (EXEN4 = 1) \
                                            : (__MODE__ ^ 0x03) == 0   ? (EXEN4 = 1) \
                                                                       : _nop_());     \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 Capture Source Dectect
* @details      Set T4EXH
* @param[in]    __MODE__ :
*  @arg\b       Timer4_IGNORED
*  @arg\b       Timer4_FALLING_EDGE
*  @arg\b       Timer4_RISING_EDGE
*  @arg\b       Timer4_DUAL_EDGE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_SetT4EXH(Timer4_IGNORED)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_SetT4EXH(__MODE__)                                                                            \
    MWT(                                                                                                           \
        __DRV_SFR_PageIndex(T4MOD_Page);                                                                           \
        ((__MODE__ ^ 0x00) == 0 ? (T4MOD = T4MOD & (~T4EXH)) : (__MODE__ ^ 0x01) == 0 ? (T4MOD = T4MOD | (T4EXH))  \
                                                           : (__MODE__ ^ 0x02) == 0   ? (T4MOD = T4MOD & (~T4EXH)) \
                                                           : (__MODE__ ^ 0x03) == 0   ? (T4MOD = T4MOD | (T4EXH))  \
                                                                                      : _nop_());                    \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER4 High Byte Register
* @details      Set TH4
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_SetHighByte(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_SetHighByte(__RELOAD__) \
    MWT(                                     \
        __DRV_SFR_PageIndex(TH4_Page);       \
        TH4 = __RELOAD__;                    \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER5 High Byte Register
* @details      Set TH5
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER5_SetHighByte(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER5_SetHighByte(__RELOAD__) \
    MWT(                                     \
        __DRV_SFR_PageIndex(TH5_Page);       \
        TH5 = __RELOAD__;                    \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER6 High Byte Register
* @details      Set TH6
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER6_SetHighByte(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER6_SetHighByte(__RELOAD__) \
    MWT(                                     \
        __DRV_SFR_PageIndex(TH6_Page);       \
        TH6 = __RELOAD__;                    \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER5 High Byte Reload Register
* @details      Set THR5
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER5_SetTHR5(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER5_SetTHR5(__RELOAD__) \
    MWT(                                 \
        __DRV_SFR_PageIndex(THR5_Page);  \
        THR5 = __RELOAD__;               \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER6 High Byte Reload Register
* @details      Set THR6
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER6_SetTHR6(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER6_SetTHR6(__RELOAD__) \
    MWT(                                 \
        __DRV_SFR_PageIndex(THR6_Page);  \
        THR6 = __RELOAD__;               \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER5 Low Byte Reload Register
* @details      Set TLR5
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER5_SetTLR5(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER5_SetTLR5(__RELOAD__) \
    MWT(                                 \
        __DRV_SFR_PageIndex(TLR5_Page);  \
        TLR5 = __RELOAD__;               \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER6 Low Byte Reload Register
* @details      Set TLR6
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER6_SetTLR6(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER6_SetTLR6(__RELOAD__) \
    MWT(                                 \
        __DRV_SFR_PageIndex(TLR6_Page);  \
        TLR6 = __RELOAD__;               \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER4 Low Byte Register
* @details      Set TL4
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_SetLowByte(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_SetLowByte(__RELOAD__) \
    MWT(                                    \
        __DRV_SFR_PageIndex(TL4_Page);      \
        TL4 = __RELOAD__;                   \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER5 Low Byte Register
* @details      Set TL5
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER5_SetLowByte(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER5_SetLowByte(__RELOAD__) \
    MWT(                                    \
        __DRV_SFR_PageIndex(TL5_Page);      \
        TL5 = __RELOAD__;                   \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER6 Low Byte Register
* @details      Set TL6
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER6_SetLowByte(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER6_SetLowByte(__RELOAD__) \
    MWT(                                    \
        __DRV_SFR_PageIndex(TL6_Page);      \
        TL6 = __RELOAD__;                   \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER4 16 Bit Timer/Counter
* @details      Set TH4 TL4
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_Set16BitCounter(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_Set16BitCounter(__RELOAD__) \
    MWT(                                         \
        __DRV_SFR_PageIndex(TH4_Page);           \
        TH4 = HIBYTE(__RELOAD__);                \
        TL4 = LOBYTE(__RELOAD__) & 0xff;         \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER5 16 Bit Timer/Counter
* @details      Set TH5 TL5
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER5_Set16BitCounter(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER5_Set16BitCounter(__RELOAD__) \
    MWT(                                         \
        __DRV_SFR_PageIndex(TH5_Page);           \
        TH5 = HIBYTE(__RELOAD__);                \
        TL5 = LOBYTE(__RELOAD__) & 0xff;         \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER6 16 Bit Timer/Counter
* @details      Set TH6 TL6
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER6_Set16BitCounter(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER6_Set16BitCounter(__RELOAD__) \
    MWT(                                         \
        __DRV_SFR_PageIndex(TH6_Page);           \
        TH6 = HIBYTE(__RELOAD__);                \
        TL6 = LOBYTE(__RELOAD__) & 0xff;         \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER4 Capture High Byte Register
* @details      Set RCAP4H
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_SetRCAP4H(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_SetRCAP4H(__RELOAD__) \
    MWT(                                   \
        __DRV_SFR_PageIndex(RCAP4H_Page);  \
        RCAP4H = __RELOAD__;               \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER4 Capture Low Byte Register
* @details      Set RCAP4L
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_SetRCAP4L(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_SetRCAP4L(__RELOAD__) \
    MWT(                                   \
        __DRV_SFR_PageIndex(RCAP4L_Page);  \
        RCAP4L = __RELOAD__;               \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER4 16 Bit Capture Register
* @details      Set RCAP4H RCAP4L
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_Set16Bit_AutoReloadValue(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_Set16Bit_AutoReloadValue(__RELOAD__) \
    MWT(                                                  \
        __DRV_SFR_PageIndex(RCAP4H_Page);                 \
        RCAP4H = HIBYTE(__RELOAD__);                      \
        RCAP4L = LOBYTE(__RELOAD__) & 0xff;               \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER5 16 Bit Reload Register
* @details      Set THR5 TLR5
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER5_Set16Bit_AutoReloadValue(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER5_Set16Bit_AutoReloadValue(__RELOAD__) \
    MWT(                                                  \
        __DRV_SFR_PageIndex(THR5_Page);                   \
        THR5 = HIBYTE(__RELOAD__);                        \
        TLR5 = LOBYTE(__RELOAD__) & 0xff;                 \
        __DRV_SFR_PageIndex(0);)

/**
*******************************************************************************
* @brief        TIMER6 16 Bit Reload Register
* @details      Set THR6 TLR6
* @param[in]    __RELOAD__ :
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER6_Set16Bit_AutoReloadValue(__RELOAD__)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER6_Set16Bit_AutoReloadValue(__RELOAD__) \
    MWT(                                                  \
        __DRV_SFR_PageIndex(THR6_Page);                   \
        THR6 = HIBYTE(__RELOAD__);                        \
        TLR6 = LOBYTE(__RELOAD__) & 0xff;                 \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER2 T2EXI input signal inversion control.
* @details      Set T2EIP
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER2_Input_Signal_Inversion(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER2_Input_Signal_Inversion(__STATE__)                                         \
    MWT(                                                                                       \
        __DRV_SFR_PageIndex(T2MOD_Page);                                                       \
        __STATE__ == 0 ? (T2MOD = T2MOD & (~TL2X12_T2EIP)) : (T2MOD = T2MOD | (TL2X12_T2EIP)); \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER3 T3EXI input signal inversion control.
* @details      Set T3EIP
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER3_Input_Signal_Inversion(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER3_Input_Signal_Inversion(__STATE__)                                         \
    MWT(                                                                                       \
        __DRV_SFR_PageIndex(T3MOD_Page);                                                       \
        __STATE__ == 0 ? (T3MOD = T3MOD & (~TL3X12_T3EIP)) : (T3MOD = T3MOD | (TL3X12_T3EIP)); \
        __DRV_SFR_PageIndex(0);)

/**
*****************************************************************************
* @brief        TIMER4 T4EXI input signal inversion control.
* @details      Set T4EIP
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_TIMER4_Input_Signal_Inversion(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_TIMER4_Input_Signal_Inversion(__STATE__)                                         \
    MWT(                                                                                       \
        __DRV_SFR_PageIndex(T4MOD_Page);                                                       \
        __STATE__ == 0 ? (T4MOD = T4MOD & (~TL4X12_T4EIP)) : (T4MOD = T4MOD | (TL4X12_T4EIP)); \
        __DRV_SFR_PageIndex(0);)

bool DRV_TIMER0_GetTF0(void);
bool DRV_TIMER1_GetTF1(void);
bool DRV_TIMER2_GetTF2(void);
bool DRV_TIMER3_GetTF3(void);
bool DRV_TIMER4_GetTF4(void);
bool DRV_TIMER5_GetTF5(void);
bool DRV_TIMER6_GetTF6(void);

bool DRV_TIMER2_GetTF2L(void);
bool DRV_TIMER3_GetTF3L(void);
bool DRV_TIMER4_GetTF4L(void);

bool DRV_TIMER2_GetEXF2(void);
bool DRV_TIMER3_GetEXF3(void);
bool DRV_TIMER4_GetEXF4(void);

void DRV_TIMER0_ClearTF0(void);
void DRV_TIMER1_ClearTF1(void);
void DRV_TIMER2_ClearTF2(void);
void DRV_TIMER3_ClearTF3(void);
void DRV_TIMER4_ClearTF4(void);
void DRV_TIMER5_ClearTF5(void);
void DRV_TIMER6_ClearTF6(void);

void DRV_TIMER2_ClearTF2L(void);
void DRV_TIMER3_ClearTF3L(void);
void DRV_TIMER4_ClearTF4L(void);

void DRV_TIMER2_ClearEXF2(void);
void DRV_TIMER3_ClearEXF3(void);
void DRV_TIMER4_ClearEXF4(void);

void DRV_TIMER0_SetTF0(void);
void DRV_TIMER1_SetTF1(void);
void DRV_TIMER2_SetTF2(void);
void DRV_TIMER3_SetTF3(void);
void DRV_TIMER4_SetTF4(void);
void DRV_TIMER5_SetTF5(void);
void DRV_TIMER6_SetTF6(void);

void DRV_TIMER2_SetTF2L(void);
void DRV_TIMER3_SetTF3L(void);
void DRV_TIMER4_SetTF4L(void);

void DRV_TIMER2_SetEXF2(void);
void DRV_TIMER3_SetEXF3(void);
void DRV_TIMER4_SetEXF4(void);

uint8_t DRV_TIMER0_GetTH0(void);
uint8_t DRV_TIMER0_GetTL0(void);
uint8_t DRV_TIMER1_GetTH1(void);
uint8_t DRV_TIMER1_GetTL1(void);
uint8_t DRV_TIMER2_GetTH2(void);
uint8_t DRV_TIMER2_GetTL2(void);
uint8_t DRV_TIMER3_GetTH3(void);
uint8_t DRV_TIMER3_GetTL3(void);
uint8_t DRV_TIMER4_GetTH4(void);
uint8_t DRV_TIMER4_GetTL4(void);
uint8_t DRV_TIMER5_GetTH5(void);
uint8_t DRV_TIMER5_GetTL5(void);
uint8_t DRV_TIMER6_GetTH6(void);
uint8_t DRV_TIMER6_GetTL6(void);

uint8_t DRV_TIMER2_GetRCAP2H(void);
uint8_t DRV_TIMER3_GetRCAP3H(void);
uint8_t DRV_TIMER4_GetRCAP4H(void);

uint8_t DRV_TIMER2_GetRCAP2L(void);
uint8_t DRV_TIMER3_GetRCAP3L(void);
uint8_t DRV_TIMER4_GetRCAP4L(void);

#endif
