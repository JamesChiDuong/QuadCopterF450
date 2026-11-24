/**
 ******************************************************************************
 *
 * @file        MG82F6D64_ACx_DRV.c
 *
 * @brief       This is the C code format driver head file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     V1.00
 * @date        2020/10/30
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
* @brief       Read AC0 interrupt flag.
* @details     Set by hardware when a edge trigger.
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
               if(DRV_AC0_GetFlag())
                  .... //Flag=1
               else
                  .... //Flag=0
* @endcode
*******************************************************************************
*/
bool DRV_AC0_GetFlag(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(AC0CON_Page);
    if((AC0CON&AC0F)==AC0F)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    _pop_(SFRPI);
    return FALSE;
}
/**
*******************************************************************************
* @brief       Read AC1 interrupt flag.
* @details     Set by hardware when a edge trigger.
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
               if(DRV_AC1_GetFlag())
                  .... //Flag=1
               else
                  .... //Flag=0
* @endcode
*******************************************************************************
*/
bool DRV_AC1_GetFlag(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(AC1CON_Page);
    if((AC1CON&AC1F)==AC1F)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    _pop_(SFRPI);
    return FALSE;
}
/**
*******************************************************************************
* @brief       Read AC2 interrupt flag.
* @details     Set by hardware when a edge trigger.
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
               if(DRV_AC2_GetFlag())
                  .... //Flag=1
               else
                  .... //Flag=0
* @endcode
*******************************************************************************
*/
bool DRV_AC2_GetFlag(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(AC2CON_Page);
    if((AC2CON&AC2F)==AC2F)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    _pop_(SFRPI);
    return FALSE;
}

/**
*******************************************************************************
* @brief       Read AC0 output state.
* @details     Read AC0 comparator output.
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
               if(DRV_AC0_GetOutput())
                  .... //Flag=1
               else
                  .... //Flag=0
* @endcode
*******************************************************************************
*/
bool DRV_AC0_GetOutput(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(AC0CON_Page);
    if((AC0CON&AC0OUT)==AC0OUT)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    _pop_(SFRPI);
    return FALSE;
}
/**
*******************************************************************************
* @brief       Read AC1 output state.
* @details     Read AC1 comparator output.
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
               if(DRV_AC1_GetOutput())
                  .... //Flag=1
               else
                  .... //Flag=0
* @endcode
*******************************************************************************
*/
bool DRV_AC1_GetOutput(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(AC1CON_Page);
    if((AC1CON&AC1OUT)==AC1OUT)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    _pop_(SFRPI);
    return FALSE;
}
/**
*******************************************************************************
* @brief       Read AC2 output state.
* @details     Read AC2 comparator output.
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
               if(DRV_AC2_GetOutput())
                  .... //Flag=1
               else
                  .... //Flag=0
* @endcode
*******************************************************************************
*/
bool DRV_AC2_GetOutput(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(AC2CON_Page);
    if((AC2CON&AC2OUT)==AC2OUT)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    _pop_(SFRPI);
    return FALSE;
}
