/**
 ******************************************************************************
 *
 * @file        MG82F6D64_I2C1_DRV.c
 *
 * @brief       This is the C code format driver head file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     V1.00
 * @date        2020/07/10
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
* @brief       Get I2C1 state event flag
* @details     0: There is no I2C1 event happened. It is only cleared by software.
               1: There is a I2C1 event happened.
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
               if(DRV_I2C1_GetEventFlag())
                  .... //Flag=1
               else
                  .... //Flag=0
* @endcode
*******************************************************************************
*/
bool DRV_I2C1_GetEventFlag(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(SI1CON_Page);
    if((SI1CON & SI1)==SI1)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    _pop_(SFRPI);
    return FALSE;
}



/**
 *******************************************************************************
 * @brief       Get I2C1 state event code
 * @details
 * @return      8bit state from I2C1 SI1STA
 * @note
 * @par         Example
 * @code
 *    8bitReg(user define) = DRV_I2C1_GetEventCode();
 * @endcode
 *******************************************************************************
 */
uint8_t DRV_I2C1_GetEventCode(void)
{
    uint8_t dta;
    _push_(SFRPI);
    __DRV_SFR_PageIndex(SI1STA_Page);
    dta = SI1STA;
    _pop_(SFRPI);
    return dta;
}





/**
 *******************************************************************************
 * @brief       Get the I2C1 data buffer
 * @details
 * @return      8bit data from I2C1 SI1DAT
 * @note
 * @par         Example
 * @code
 *    8bitReg(user define) = DRV_I2C1_GetBufData();
 * @endcode
 *******************************************************************************
 */
uint8_t DRV_I2C1_GetBufData(void)
{
    uint8_t dta;
    _push_(SFRPI);
    __DRV_SFR_PageIndex(SI1DAT_Page);
    dta = SI1DAT;
    _pop_(SFRPI);
    return dta;
}




