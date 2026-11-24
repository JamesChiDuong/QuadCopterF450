/**
 ******************************************************************************
 *
 * @file        MG82F6D64_KBI_DRV.c
 *
 * @brief       This is the C code format driver head file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     V1.00
 * @date        2021/10/08
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
* @brief       Read KBI interrupt flag.
* @details     Set by hardware when a event trigger.
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
               if(DRV_KBI_GetFlag())
                  .... //Flag=1
               else
                  .... //Flag=0
* @endcode
*******************************************************************************
*/
bool DRV_KBI_GetFlag(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(KBCON_Page);
    if((KBCON&KBIF)==KBIF)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    _pop_(SFRPI);
    return FALSE;
}
