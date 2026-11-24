/**
 ******************************************************************************
 *
 * @file        MG82F6D64_TIMER_DRV.c
 *
 * @brief       This is the C code format driver head file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     V1.02
 * @date        2021/10/18
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

#include "MG82F6D64_CONFIG.h"

/**
*******************************************************************************
* @brief       TIMER0 Read overflow flag.
* @details     Read TF0
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_TIMER0_GetTF0();
* @endcode
*******************************************************************************
*/
bool DRV_TIMER0_GetTF0(void)
{
    if(TF0==1)
    {
        return TRUE;
    }
    return FALSE;
}


/**
*******************************************************************************
* @brief       TIMER1 Read overflow flag.
* @details     Read TF1
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_TIMER1_GetTF1();
* @endcode
*******************************************************************************
*/
bool DRV_TIMER1_GetTF1(void)
{
    if(TF1==1)
    {
        return TRUE;
    }
    return FALSE;
}


/**
*******************************************************************************
* @brief       TIMER2 Read overflow flag.
* @details     Read TF2
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_TIMER2_GetTF2();
* @endcode
*******************************************************************************
*/
bool DRV_TIMER2_GetTF2(void)
{
    if(TF2==1)
    {
        return TRUE;
    }
    return FALSE;
}


/**
*******************************************************************************
* @brief       TIMER3 Read overflow flag.
* @details     Read TF3
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_TIMER3_GetTF3();
* @endcode
*******************************************************************************
*/
bool DRV_TIMER3_GetTF3(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(T3CON_Page);
    if(TF3==1)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    _pop_(SFRPI);
    return FALSE;
}


/**
*******************************************************************************
* @brief       TIMER4 Read overflow flag.
* @details     Read TF4
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_TIMER4_GetTF4();
* @endcode
*******************************************************************************
*/
bool DRV_TIMER4_GetTF4(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(T4CON_Page);
    if(TF4==1)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    _pop_(SFRPI);
    return FALSE;
}


/**
*******************************************************************************
* @brief       TIMER5 Read overflow flag.
* @details     Read TF5
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_TIMER5_GetTF5();
* @endcode
*******************************************************************************
*/
bool DRV_TIMER5_GetTF5(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(T5CON_Page);
    if(TF5==1)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    _pop_(SFRPI);
    return FALSE;
}


/**
*******************************************************************************
* @brief       TIMER6 Read overflow flag.
* @details     Read TF6
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_TIMER6_GetTF6();
* @endcode
*******************************************************************************
*/
bool DRV_TIMER6_GetTF6(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(T6CON_Page);
    if(TF6==1)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    _pop_(SFRPI);
    return FALSE;
}


/**
*******************************************************************************
* @brief       TIMER0 Clear overflow flag.
* @details     Clear TF0
* @return      None
* @note        None
* @par         Example
* @code
                DRV_TIMER0_ClearTF0();
* @endcode
*******************************************************************************
*/
void DRV_TIMER0_ClearTF0(void)
{
    TF0=0;
}


/**
*******************************************************************************
* @brief       TIMER1 Clear overflow flag.
* @details     Clear TF1
* @return      None
* @note        None
* @par         Example
* @code
                DRV_TIMER1_ClearTF1();
* @endcode
*******************************************************************************
*/
void DRV_TIMER1_ClearTF1(void)
{
    TF1=0;
}


/**
*******************************************************************************
* @brief       TIMER2 Clear overflow flag.
* @details     Clear TF2
* @return      None
* @note        None
* @par         Example
* @code
                DRV_TIMER2_ClearTF2();
* @endcode
*******************************************************************************
*/
void DRV_TIMER2_ClearTF2(void)
{
    TF2=0;
}


/**
*******************************************************************************
* @brief       TIMER3 Clear overflow flag.
* @details     Clear TF1
* @return      None
* @note        None
* @par         Example
* @code
                DRV_TIMER3_ClearTF3();
* @endcode
*******************************************************************************
*/
void DRV_TIMER3_ClearTF3(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(T3CON_Page);
    TF3=0;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       TIMER4 Clear overflow flag.
* @details     Clear TF4
* @return      None
* @note        None
* @par         Example
* @code
                DRV_TIMER4_ClearTF4();
* @endcode
*******************************************************************************
*/
void DRV_TIMER4_ClearTF4(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(T4CON_Page);
    TF4=0;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       TIMER5 Clear overflow flag.
* @details     Clear TF5
* @return      None
* @note        None
* @par         Example
* @code
                DRV_TIMER5_ClearTF5();
* @endcode
*******************************************************************************
*/
void DRV_TIMER5_ClearTF5(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(T5CON_Page);
    TF5=0;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       TIMER6 Clear overflow flag.
* @details     Clear TF6
* @return      None
* @note        None
* @par         Example
* @code
                DRV_TIMER6_ClearTF6();
* @endcode
*******************************************************************************
*/
void DRV_TIMER6_ClearTF6(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(T6CON_Page);
    TF6=0;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       TIMER0 Set overflow flag.
* @details     Set TF0
* @return      None
* @note        None
* @par         Example
* @code
                DRV_TIMER0_SetTF0();
* @endcode
*******************************************************************************
*/
void DRV_TIMER0_SetTF0(void)
{
    TF0=1;
}


/**
*******************************************************************************
* @brief       TIMER1 Set overflow flag.
* @details     Set TF1
* @return      None
* @note        None
* @par         Example
* @code
                DRV_TIMER1_SetTF1();
* @endcode
*******************************************************************************
*/
void DRV_TIMER1_SetTF1(void)
{
    TF1=1;
}


/**
*******************************************************************************
* @brief       TIMER2 Set overflow flag.
* @details     Set TF2
* @return      None
* @note        None
* @par         Example
* @code
                DRV_TIMER2_SetTF2();
* @endcode
*******************************************************************************
*/
void DRV_TIMER2_SetTF2(void)
{
    TF2=1;
}


/**
*******************************************************************************
* @brief       TIMER3 Set overflow flag.
* @details     Set TF3
* @return      None
* @note        None
* @par         Example
* @code
                DRV_TIMER3_SetTF3();
* @endcode
*******************************************************************************
*/
void DRV_TIMER3_SetTF3(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(T3CON_Page);
    TF3=1;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       TIMER4 Set overflow flag.
* @details     Set TF4
* @return      None
* @note        None
* @par         Example
* @code
                DRV_TIMER4_SetTF4();
* @endcode
*******************************************************************************
*/
void DRV_TIMER4_SetTF4(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(T4CON_Page);
    TF4=1;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       TIMER5 Set overflow flag.
* @details     Set TF5
* @return      None
* @note        None
* @par         Example
* @code
                DRV_TIMER5_SetTF5();
* @endcode
*******************************************************************************
*/
void DRV_TIMER5_SetTF5(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(T5CON_Page);
    TF5=1;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       TIMER6 Set overflow flag.
* @details     Set TF6
* @return      None
* @note        None
* @par         Example
* @code
                DRV_TIMER6_SetTF6();
* @endcode
*******************************************************************************
*/
void DRV_TIMER6_SetTF6(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(T6CON_Page);
    TF6=1;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       TIMER2 Read External flag.
* @details     Read EXF2
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_TIMER2_GetEXF2();
* @endcode
*******************************************************************************
*/
bool DRV_TIMER2_GetEXF2(void)
{
    if(EXF2==1)
    {
        return TRUE;
    }
    return FALSE;
}


/**
*******************************************************************************
* @brief       TIMER2 Clear External flag.
* @details     Clear EXF2
* @return      None
* @note        None
* @par         Example
* @code
                DRV_TIMER2_ClearEXF2();
* @endcode
*******************************************************************************
*/
void DRV_TIMER2_ClearEXF2(void)
{
    EXF2=0;
}


/**
*******************************************************************************
* @brief       TIMER2 Set External flag.
* @details     Set EXF2
* @return      None
* @note        None
* @par         Example
* @code
                DRV_TIMER2_SetEXF2();
* @endcode
*******************************************************************************
*/
void DRV_TIMER2_SetEXF2(void)
{
    EXF2=1;
}


/**
*******************************************************************************
* @brief       TIMER2 Read Capture High Byte.
* @details     Read RCAP2H
* @return      None
* @note        None
* @par         Example
* @code
                tmp=DRV_TIMER2_GetRCAP2H();
* @endcode
*******************************************************************************
*/
uint8_t DRV_TIMER2_GetRCAP2H(void)
{
    return RCAP2H;
}


/**
*******************************************************************************
* @brief       TIMER2 Read Capture Low Byte.
* @details     Read RCAP2L
* @return      None
* @note        None
* @par         Example
* @code
                tmp=DRV_TIMER2_GetRCAP2L();
* @endcode
*******************************************************************************
*/
uint8_t DRV_TIMER2_GetRCAP2L(void)
{
    return RCAP2L;
}


/**
*******************************************************************************
* @brief       TIMER2 TL2 Set overflow flag.
* @details     Set TF2L
* @return      None
* @note        Before set RCLK_TF2L,please ensure TL2IS=1(__DRV_TIMER2_Access_TF2L_TL2IE_Function_Cmd(MW_ENABLE)).
* @par         Example
* @code
                DRV_TIMER2_SetTF2L();
* @endcode
*******************************************************************************
*/
void DRV_TIMER2_SetTF2L(void)
{
    RCLK_TF2L=1;
}


/**
*******************************************************************************
* @brief       TIMER2 TL2 Clear overflow flag.
* @details     Set TF2L
* @return      None
* @note        Before clear RCLK_TF2L,please ensure TL2IS=1(__DRV_TIMER2_Access_TF2L_TL2IE_Function_Cmd(MW_ENABLE)).
* @par         Example
* @code
                DRV_TIMER2_ClearTF2L();
* @endcode
*******************************************************************************
*/
void DRV_TIMER2_ClearTF2L(void)
{
    RCLK_TF2L=0;
}


/**
*******************************************************************************
* @brief       TIMER2 TL2 Read overflow flag.
* @details     Set TF2L
* @return      None
* @note        Before read RCLK_TF2L,please ensure TL2IS=1(__DRV_TIMER2_Access_TF2L_TL2IE_Function_Cmd(MW_ENABLE))and TL2IE=1(__DRV_TIMER2_TF2L_IT_Cmd(MW_ENABLE)).
* @par         Example
* @code
                DRV_TIMER2_GetTF2L();
* @endcode
*******************************************************************************
*/
bool DRV_TIMER2_GetTF2L(void)
{
    if(RCLK_TF2L)
    {
        return TRUE;
    }
    return FALSE;
}


/**
*******************************************************************************
* @brief       TIMER3 Read External flag.
* @details     Read EXF3
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_TIMER3_GetEXF3();
* @endcode
*******************************************************************************
*/
bool DRV_TIMER3_GetEXF3(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(T3CON_Page);
    if(EXF3==1)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    _pop_(SFRPI);
    return FALSE;
}


/**
*******************************************************************************
* @brief       TIMER3 Clear External flag.
* @details     Clear EXF3
* @return      None
* @note        None
* @par         Example
* @code
                DRV_TIMER3_ClearEXF3();
* @endcode
*******************************************************************************
*/
void DRV_TIMER3_ClearEXF3(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(T3CON_Page);
    EXF3=0;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       TIMER3 Set External flag.
* @details     Set EXF3
* @return      None
* @note        None
* @par         Example
* @code
                DRV_TIMER3_SetEXF3();
* @endcode
*******************************************************************************
*/
void DRV_TIMER3_SetEXF3(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(T3CON_Page);
    EXF3=1;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       TIMER3 Read Capture High Byte.
* @details     Read RCAP3H
* @return      None
* @note        None
* @par         Example
* @code
                tmp=DRV_TIMER3_GetRCAP3H();
* @endcode
*******************************************************************************
*/
uint8_t DRV_TIMER3_GetRCAP3H(void)
{
    uint8_t tmp;

    _push_(SFRPI);
    __DRV_SFR_PageIndex(RCAP3H_Page);
    tmp=RCAP3H;
    _pop_(SFRPI);
    return(tmp);
}


/**
*******************************************************************************
* @brief       TIMER3 Read Capture Low Byte.
* @details     Read RCAP3L
* @return      None
* @note        None
* @par         Example
* @code
                tmp=DRV_TIMER3_GetRCAP3L();
* @endcode
*******************************************************************************
*/
uint8_t DRV_TIMER3_GetRCAP3L(void)
{
    uint8_t tmp;

    _push_(SFRPI);
    __DRV_SFR_PageIndex(RCAP3L_Page);
    tmp=RCAP3L;
    _pop_(SFRPI);
    return(tmp);
}


/**
*******************************************************************************
* @brief       TIMER3 TL3 Set overflow flag.
* @details     Set TF3L
* @return      None
* @note        None
* @par         Example
* @code
                DRV_TIMER3_SetTF3L();
* @endcode
*******************************************************************************
*/
void DRV_TIMER3_SetTF3L(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(T3CON_Page);
    TF3L=1;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       TIMER3 TL3 Clear overflow flag.
* @details     Set TF3L
* @return      None
* @note        None
* @par         Example
* @code
                DRV_TIMER3_ClearTF3L();
* @endcode
*******************************************************************************
*/
void DRV_TIMER3_ClearTF3L(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(T3CON_Page);
    TF3L=0;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       TIMER3 TL3 Read overflow flag.
* @details     Set TF3L
* @return      None
* @note        None
* @par         Example
* @code
                DRV_TIMER3_GetTF3L();
* @endcode
*******************************************************************************
*/
bool DRV_TIMER3_GetTF3L(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(T3CON_Page);
    if(TF3L)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    _pop_(SFRPI);
    return FALSE;
}


/**
*******************************************************************************
* @brief       TIMER4 Read External flag.
* @details     Read EXF4
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_TIMER4_GetEXF4();
* @endcode
*******************************************************************************
*/
bool DRV_TIMER4_GetEXF4(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(T4CON_Page);
    if(EXF4==1)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    _pop_(SFRPI);
    return FALSE;
}


/**
*******************************************************************************
* @brief       TIMER4 Clear External flag.
* @details     Clear EXF4
* @return      None
* @note        None
* @par         Example
* @code
                DRV_TIMER4_ClearEXF4();
* @endcode
*******************************************************************************
*/
void DRV_TIMER4_ClearEXF4(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(T4CON_Page);
    EXF4=0;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       TIMER4 Set External flag.
* @details     Set EXF4
* @return      None
* @note        None
* @par         Example
* @code
                DRV_TIMER4_SetEXF4();
* @endcode
*******************************************************************************
*/
void DRV_TIMER4_SetEXF4(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(T4CON_Page);
    EXF4=1;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       TIMER4 Read Capture High Byte.
* @details     Read RCAP4H
* @return      None
* @note        None
* @par         Example
* @code
                tmp=DRV_TIMER4_GetRCAP4H();
* @endcode
*******************************************************************************
*/
uint8_t DRV_TIMER4_GetRCAP4H(void)
{
    uint8_t tmp;

    _push_(SFRPI);
    __DRV_SFR_PageIndex(RCAP4H_Page);
    tmp=RCAP4H;
    _pop_(SFRPI);
    return(tmp);
}


/**
*******************************************************************************
* @brief       TIMER4 Read Capture Low Byte.
* @details     Read RCAP4L
* @return      None
* @note        None
* @par         Example
* @code
                tmp=DRV_TIMER4_GetRCAP4L();
* @endcode
*******************************************************************************
*/
uint8_t DRV_TIMER4_GetRCAP4L(void)
{
    uint8_t tmp;

    _push_(SFRPI);
    __DRV_SFR_PageIndex(RCAP4L_Page);
    tmp=RCAP4L;
    _pop_(SFRPI);
    return(tmp);
}


/**
*******************************************************************************
* @brief       TIMER4 TL4 Set overflow flag.
* @details     Set TF4L
* @return      None
* @note        Before set TF4L,please ensure TL4IS=1(__DRV_TIMER4_Access_TF4L_TL4IE_Function_Cmd(MW_ENABLE)).
* @par         Example
* @code
                DRV_TIMER4_SetTF2L();
* @endcode
*******************************************************************************
*/
void DRV_TIMER4_SetTF4L(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(T4CON_Page);
    TF4L=1;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       TIMER4 TL4 Clear overflow flag.
* @details     Set TF4L
* @return      None
* @note        Before clear TF4L,please ensure TL4IS=1(__DRV_TIMER4_Access_TF4L_TL4IE_Function_Cmd(MW_ENABLE)).
* @par         Example
* @code
                DRV_TIMER4_ClearTF4L();
* @endcode
*******************************************************************************
*/
void DRV_TIMER4_ClearTF4L(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(T4CON_Page);
    TF4L=0;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       TIMER4 TL4 Read overflow flag.
* @details     Set TF4L
* @return      None
* @note        Before read TF4L,please ensure TL4IS=1(__DRV_TIMER4_Access_TF4L_TL4IE_Function_Cmd(MW_ENABLE))and TL4IE=1(__DRV_TIMER4_TF4L_IT_Cmd(MW_ENABLE)).
* @par         Example
* @code
                DRV_TIMER4_GetTF4L();
* @endcode
*******************************************************************************
*/
bool DRV_TIMER4_GetTF4L(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(T4CON_Page);
    if(TF4L)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    _pop_(SFRPI);
    return FALSE;
}


/**
*******************************************************************************
* @brief       TIMER0 Read TH0.
* @details     Read TH0
* @return      None
* @note        None
* @par         Example
* @code
                tmp=DRV_TIMER0_GetTH0();
* @endcode
*******************************************************************************
*/
uint8_t DRV_TIMER0_GetTH0(void)
{
    uint8_t tmp;

    _push_(SFRPI);
    __DRV_SFR_PageIndex(TH0_Page);
    tmp=TH0;
    _pop_(SFRPI);
    return(tmp);
}


/**
*******************************************************************************
* @brief       TIMER0 Read TL0.
* @details     Read TL0
* @return      None
* @note        None
* @par         Example
* @code
                tmp=DRV_TIMER0_GetTL0();
* @endcode
*******************************************************************************
*/
uint8_t DRV_TIMER0_GetTL0(void)
{
    uint8_t tmp;

    _push_(SFRPI);
    __DRV_SFR_PageIndex(TL0_Page);
    tmp=TL0;
    _pop_(SFRPI);
    return(tmp);
}


/**
*******************************************************************************
* @brief       TIMER1 Read TH1.
* @details     Read TH1
* @return      None
* @note        None
* @par         Example
* @code
                tmp=DRV_TIMER1_GetTH1();
* @endcode
*******************************************************************************
*/
uint8_t DRV_TIMER1_GetTH1(void)
{
    uint8_t tmp;

    _push_(SFRPI);
    __DRV_SFR_PageIndex(TH1_Page);
    tmp=TH1;
    _pop_(SFRPI);
    return(tmp);
}


/**
*******************************************************************************
* @brief       TIMER1 Read TL1.
* @details     Read TL1
* @return      None
* @note        None
* @par         Example
* @code
                tmp=DRV_TIMER1_GetTL1();
* @endcode
*******************************************************************************
*/
uint8_t DRV_TIMER1_GetTL1(void)
{
    uint8_t tmp;

    _push_(SFRPI);
    __DRV_SFR_PageIndex(TL1_Page);
    tmp=TL1;
    _pop_(SFRPI);
    return(tmp);
}


/**
*******************************************************************************
* @brief       TIMER2 Read TH2.
* @details     Read TH2
* @return      None
* @note        None
* @par         Example
* @code
                tmp=DRV_TIMER2_GetTH2();
* @endcode
*******************************************************************************
*/
uint8_t DRV_TIMER2_GetTH2(void)
{
    uint8_t tmp;

    _push_(SFRPI);
    __DRV_SFR_PageIndex(TH2_Page);
    tmp=TH2;
    _pop_(SFRPI);
    return(tmp);
}


/**
*******************************************************************************
* @brief       TIMER2 Read TL2.
* @details     Read TL2
* @return      None
* @note        None
* @par         Example
* @code
                tmp=DRV_TIMER2_GetTL2();
* @endcode
*******************************************************************************
*/
uint8_t DRV_TIMER2_GetTL2(void)
{
    uint8_t tmp;

    _push_(SFRPI);
    __DRV_SFR_PageIndex(TL2_Page);
    tmp=TL2;
    _pop_(SFRPI);
    return(tmp);
}


/**
*******************************************************************************
* @brief       TIMER3 Read TH3.
* @details     Read TH3
* @return      None
* @note        None
* @par         Example
* @code
                tmp=DRV_TIMER3_GetTH3();
* @endcode
*******************************************************************************
*/
uint8_t DRV_TIMER3_GetTH3(void)
{
    uint8_t tmp;

    _push_(SFRPI);
    __DRV_SFR_PageIndex(TH3_Page);
    tmp=TH3;
    _pop_(SFRPI);
    return(tmp);
}


/**
*******************************************************************************
* @brief       TIMER3 Read TL3.
* @details     Read TL3
* @return      None
* @note        None
* @par         Example
* @code
                tmp=DRV_TIMER3_GetTL3();
* @endcode
*******************************************************************************
*/
uint8_t DRV_TIMER3_GetTL3(void)
{
    uint8_t tmp;

    _push_(SFRPI);
    __DRV_SFR_PageIndex(TL3_Page);
    tmp=TL3;
    _pop_(SFRPI);
    return(tmp);
}


/**
*******************************************************************************
* @brief       TIMER4 Read TH4.
* @details     Read TH4
* @return      None
* @note        None
* @par         Example
* @code
                tmp=DRV_TIMER4_GetTH4();
* @endcode
*******************************************************************************
*/
uint8_t DRV_TIMER4_GetTH4(void)
{
    uint8_t tmp;
    _push_(SFRPI);
    __DRV_SFR_PageIndex(TH4_Page);
    tmp=TH4;
    _pop_(SFRPI);
    return(tmp);
}


/**
*******************************************************************************
* @brief       TIMER4 Read TL4.
* @details     Read TL4
* @return      None
* @note        None
* @par         Example
* @code
                tmp=DRV_TIMER4_GetTL4();
* @endcode
*******************************************************************************
*/
uint8_t DRV_TIMER4_GetTL4(void)
{
    uint8_t tmp;

    _push_(SFRPI);
    __DRV_SFR_PageIndex(TL4_Page);
    tmp=TL4;
    _pop_(SFRPI);
    return(tmp);
}


/**
*******************************************************************************
* @brief       TIMER5 Read TH5.
* @details     Read TH5
* @return      None
* @note        None
* @par         Example
* @code
                tmp=DRV_TIMER5_GetTH5();
* @endcode
*******************************************************************************
*/
uint8_t DRV_TIMER5_GetTH5(void)
{
    uint8_t tmp;

    _push_(SFRPI);
    __DRV_SFR_PageIndex(TH5_Page);
    tmp=TH5;
    _pop_(SFRPI);
    return(tmp);
}


/**
*******************************************************************************
* @brief       TIMER5 Read TL5.
* @details     Read TL5
* @return      None
* @note        None
* @par         Example
* @code
                tmp=DRV_TIMER5_GetTL5();
* @endcode
*******************************************************************************
*/
uint8_t DRV_TIMER5_GetTL5(void)
{
    uint8_t tmp;

    _push_(SFRPI);
    __DRV_SFR_PageIndex(TL5_Page);
    tmp=TL5;
    _pop_(SFRPI);
    return(tmp);
}


/**
*******************************************************************************
* @brief       TIMER6 Read TH6.
* @details     Read TH6
* @return      None
* @note        None
* @par         Example
* @code
                tmp=DRV_TIMER6_GetTH6();
* @endcode
*******************************************************************************
*/
uint8_t DRV_TIMER6_GetTH6(void)
{
    uint8_t tmp;

    _push_(SFRPI);
    __DRV_SFR_PageIndex(TH6_Page);
    tmp=TH6;
    _pop_(SFRPI);
    return(tmp);
}


/**
*******************************************************************************
* @brief       TIMER6 Read TL6.
* @details     Read TL6
* @return      None
* @note        None
* @par         Example
* @code
                tmp=DRV_TIMER6_GetTL6();
* @endcode
*******************************************************************************
*/
uint8_t DRV_TIMER6_GetTL6(void)
{
    uint8_t tmp;

    _push_(SFRPI);
    __DRV_SFR_PageIndex(TL6_Page);
    tmp=TL6;
    _pop_(SFRPI);
    return(tmp);
}

