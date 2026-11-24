/**
 ******************************************************************************
 *
 * @file        MG82F6D64_PCA_DRV.c
 *
 * @brief       This is the C code format driver head file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     V1.00
 * @date        2020/10/06
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

#include "MG82F6D64_CONFIG.h"

/**
*******************************************************************************
* @brief       Read PCA Module0 flag.
* @details     Set by hardware when a match or capture occurs.
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
               if(DRV_PCA_GetModule0Flag())
                  .... //Flag=1
               else
                  .... //Flag=0
* @endcode
*******************************************************************************
*/
bool DRV_PCA_GetModule0Flag(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(0);
    if(CCF0==1)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    _pop_(SFRPI);
    return FALSE;
}
/**
*******************************************************************************
* @brief       Read PCA Module1 flag.
* @details     Set by hardware when a match or capture occurs.
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
               if(DRV_PCA_GetModule1Flag())
                  .... //Flag=1
               else
                  .... //Flag=0
* @endcode
*******************************************************************************
*/
bool DRV_PCA_GetModule1Flag(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(0);
    if(CCF1==1)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    _pop_(SFRPI);
    return FALSE;
}
/**
*******************************************************************************
* @brief       Read PCA Module2 flag.
* @details     Set by hardware when a match or capture occurs.
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
               if(DRV_PCA_GetModule2Flag())
                  .... //Flag=1
               else
                  .... //Flag=0
* @endcode
*******************************************************************************
*/
bool DRV_PCA_GetModule2Flag(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(0);
    if(CCF2==1)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    _pop_(SFRPI);
    return FALSE;
}
/**
*******************************************************************************
* @brief       Read PCA Module3 flag.
* @details     Set by hardware when a match or capture occurs.
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
               if(DRV_PCA_GetModule3Flag())
                  .... //Flag=1
               else
                  .... //Flag=0
* @endcode
*******************************************************************************
*/
bool DRV_PCA_GetModule3Flag(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(0);
    if(CCF3==1)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    else
    _pop_(SFRPI);
    return FALSE;
}
/**
*******************************************************************************
* @brief       Read PCA Module4 flag.
* @details     Set by hardware when a match or capture occurs.
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
               if(DRV_PCA_GetModule4Flag())
                  .... //Flag=1
               else
                  .... //Flag=0
* @endcode
*******************************************************************************
*/
bool DRV_PCA_GetModule4Flag(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(0);
    if(CCF4==1)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    _pop_(SFRPI);
    return FALSE;
}
/**
*******************************************************************************
* @brief       Read PCA Module5 flag.
* @details     Set by hardware when a match or capture occurs.
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
               if(DRV_PCA_GetModule5Flag())
                  .... //Flag=1
               else
                  .... //Flag=0
* @endcode
*******************************************************************************
*/
bool DRV_PCA_GetModule5Flag(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(0);
    if(CCF5==1)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    else
    _pop_(SFRPI);
    return FALSE;
}
/**
*******************************************************************************
* @brief       Read PCA Module6 flag.
* @details     Set by hardware when a match or capture occurs.
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
               if(DRV_PCA_GetModule6Flag())
                  .... //Flag=1
               else
                  .... //Flag=0
* @endcode
*******************************************************************************
*/
bool DRV_PCA_GetModule6Flag(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(1);
    if((PCAPWM6&CCF6)==CCF6)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    _pop_(SFRPI);
    return FALSE;
}
/**
*******************************************************************************
* @brief       Read PCA Module7 flag.
* @details     Set by hardware when a match or capture occurs.
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
               if(DRV_PCA_GetModule7Flag())
                  .... //Flag=1
               else
                  .... //Flag=0
* @endcode
*******************************************************************************
*/
bool DRV_PCA_GetModule7Flag(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(1);
    if((PCAPWM7&CCF7)==CCF7)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    _pop_(SFRPI);
    return FALSE;
}
/**
*******************************************************************************
* @brief       Read PCA flag.
* @details     Set by hardware when the counter rolls over.
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
               if(DRV_PCA_GetOverflowFlag())
                  .... //Flag=1
               else
                  .... //Flag=0
* @endcode
*******************************************************************************
*/
bool DRV_PCA_GetOverflowFlag(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(0);
    if(CF==1)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    _pop_(SFRPI);
    return FALSE;
}
/**
*******************************************************************************
* @brief       Read PWM break flag.
* @details     0: There is no PWM Break event happened. It is only cleared by software.	
               1: There is a PWM Break event happened or software triggers a PWM Break.
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
               if(DRV_PCA_GetBreakFlag())
                  .... //Flag=1
               else
                  .... //Flag=0
* @endcode
*******************************************************************************
*/
bool DRV_PCA_GetBreakFlag(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(0);
    if((AUXR0&PBKF)==PBKF)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    _pop_(SFRPI);
    return FALSE;
}
/**
 *******************************************************************************
 * @brief       Get the PCA PWM channel 0 duty
 * @details
 * @return      16bit data from PWM 0 Duty
 * @note
 * @par         Example
 * @code
 *    16bitReg(user define) = __DRV_PCA_GetPWM0Duty();
 * @endcode
 *******************************************************************************
 */
uint16_t DRV_PCA_GetPWM0DutyValue(void)
{
    return CCAP0H << 8 | CCAP0L;
}
/**
 *******************************************************************************
 * @brief       Get the PCA PWM channel 1 duty
 * @details
 * @return      16bit data from PWM 1 Duty
 * @note
 * @par         Example
 * @code
 *    16bitReg(user define) = __DRV_PCA_GetPWM1Duty();
 * @endcode
 *******************************************************************************
 */
uint16_t DRV_PCA_GetPWM1DutyValue(void)
{
    return CCAP1H << 8 | CCAP1L;
}
/**
 *******************************************************************************
 * @brief       Get the PCA PWM channel 2 duty
 * @details
 * @return      16bit data from PWM 2 Duty
 * @note
 * @par         Example
 * @code
 *    16bitReg(user define) = __DRV_PCA_GetPWM2Duty();
 * @endcode
 *******************************************************************************
 */
uint16_t DRV_PCA_GetPWM2DutyValue(void)
{
    return CCAP2H << 8 | CCAP2L;
}
/**
 *******************************************************************************
 * @brief       Get the PCA PWM channel 3 duty
 * @details
 * @return      16bit data from PWM 3 Duty
 * @note
 * @par         Example
 * @code
 *    16bitReg(user define) = __DRV_PCA_GetPWM3Duty();
 * @endcode
 *******************************************************************************
 */
uint16_t DRV_PCA_GetPWM3DutyValue(void)
{
    return CCAP3H << 8 | CCAP3L;
}
/**
 *******************************************************************************
 * @brief       Get the PCA PWM channel 4 duty
 * @details
 * @return      16bit data from PWM 4 Duty
 * @note
 * @par         Example
 * @code
 *    16bitReg(user define) = __DRV_PCA_GetPWM4Duty();
 * @endcode
 *******************************************************************************
 */
uint16_t DRV_PCA_GetPWM4DutyValue(void)
{
    return CCAP4H << 8 | CCAP4L;
}
/**
 *******************************************************************************
 * @brief       Get the PCA PWM channel 5 duty
 * @details
 * @return      16bit data from PWM 5 Duty
 * @note
 * @par         Example
 * @code
 *    16bitReg(user define) = __DRV_PCA_GetPWM5Duty();
 * @endcode
 *******************************************************************************
 */
uint16_t DRV_PCA_GetPWM5DutyValue(void)
{
    return CCAP5H << 8 | CCAP5L;
}
/**
 *******************************************************************************
 * @brief       Get the PCA PWM channel 6 duty
 * @details
 * @return      16bit data from PWM 6 Duty
 * @note
 * @par         Example
 * @code
 *    16bitReg(user define) = __DRV_PCA_GetPWM6Duty();
 * @endcode
 *******************************************************************************
 */
uint16_t DRV_PCA_GetPWM6DutyValue(void)
{
    uint16_t dta;
    _push_(SFRPI);
    __DRV_SFR_PageIndex(1);
    dta = (CCAP6H << 8 | CCAP6L);
    _pop_(SFRPI);
    return dta;
}
/**
 *******************************************************************************
 * @brief       Get the PCA PWM channel 7 duty
 * @details
 * @return      16bit data from PWM 7 Duty
 * @note
 * @par         Example
 * @code
 *    16bitReg(user define) = __DRV_PCA_GetPWM7Duty();
 * @endcode
 *******************************************************************************
 */
uint16_t DRV_PCA_GetPWM7DutyValue(void)
{
    uint16_t dta;
    _push_(SFRPI);
    __DRV_SFR_PageIndex(1);
    dta = (CCAP7H << 8 | CCAP7L);
    _pop_(SFRPI);
    return dta;
}



