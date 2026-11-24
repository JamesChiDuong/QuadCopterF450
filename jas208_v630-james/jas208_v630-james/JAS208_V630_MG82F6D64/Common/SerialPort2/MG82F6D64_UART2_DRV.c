/**
 ******************************************************************************
 *
 * @file        MG82F6D64_UART2_DRV.c
 *
 * @brief       This is the C code format driver file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     V1.00
 * @date        2020/11/09
 * @author      Megawin Software Center
 * @copyright   Copyright (c) 2020 MegaWin Technology Co., Ltd.
 *              All rights reserved.
 *
 ******************************************************************************
 * @par 		Disclaimer
 *		The Demo software is provided "AS IS"  without any warranty, either
 *		expressed or implied, including, but not limited to, the implied warranties
 *		of merchantability and fitness for a particular purpose.  The author will
 *		not be liable for any special, incidental, consequential or indirect
 *		damages due to loss of data or any other reason.
 *		These statements agree with the world wide and local dictated laws about
 *		authorship and violence against these laws.
 ******************************************************************************
 ******************************************************************************
 */

#include "MG82F6D64_CONFIG.h"

/**
*******************************************************************************
* @brief       UART2 read interrupt flag
* @details     Read TI2
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_URT2_GetTI2();
* @endcode
*******************************************************************************
*/
bool DRV_URT2_GetTI2(void)
{
    _push_(SFRPI);
    SFRPI=3;
    if(TI2==1)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    //else
    //{
        _pop_(SFRPI);
        return FALSE;
    //}
}


/**
*******************************************************************************
* @brief       UART2 read interrupt flag
* @details    Read RI2
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_URT2_GetRI2();
* @endcode
*******************************************************************************
*/
bool DRV_URT2_GetRI2(void)
{
    _push_(SFRPI);
    SFRPI=3;
    if(RI2==1)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    //else
    //{
        _pop_(SFRPI);
        return FALSE;
    //}
}


/**
*******************************************************************************
* @brief       UART2 clear interrupt flag
* @details     Clear TI2
* @return      None
* @note        None
* @par         Example
* @code
                DRV_URT2_ClearTI2();
* @endcode
*******************************************************************************
*/
void DRV_URT2_ClearTI2(void)
{
    _push_(SFRPI);
    SFRPI=3;
    TI2=0;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       UART2 clear interrupt flag
* @details    Clear RI2
* @return      None
* @note        None
* @par         Example
* @code
                DRV_URT2_ClearRI2();
* @endcode
*******************************************************************************
*/
void DRV_URT2_ClearRI2(void)
{
    _push_(SFRPI);
    SFRPI=3;
    RI2=0;
    _pop_(SFRPI);
}

/**
*******************************************************************************
* @brief       UART2 transfer data
* @details    Set TB82
* @return      None
* @note        None
* @par         Example
* @code
                DRV_URT2_SetTXData9th();
* @endcode
*******************************************************************************
*/
void DRV_URT2_SetTXData9th(void)
{
    _push_(SFRPI);
    SFRPI=3;
    TB82=1;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       UART2 transfer data
* @details    Clear TB82
* @return      None
* @note        None
* @par         Example
* @code
DRV_URT2_ClearTXData9th();
* @endcode
*******************************************************************************
*/
void DRV_URT2_ClearTXData9th(void)
{
    _push_(SFRPI);
    SFRPI=3;
    TB82=0;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       UART2 read receive data
* @details    Read RB82
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                usertmp=DRV_URT2_GetRXData9th();
* @endcode
*******************************************************************************
*/
bool DRV_URT2_GetRXData9th(void)
{
    _push_(SFRPI);
    SFRPI=3;
    if(RB82==1)
    {
        _pop_(SFRPI);
        return TRUE;
    }
    //else
    //{
        _pop_(SFRPI);
        return FALSE;
    //}
}


/**
*******************************************************************************
* @brief       UART2 transfer data
* @details    Send S2BUF
* @param[in]   TXData:
*  @arg\b       userdata(0~255)
* @return      None
* @note        None
* @par         Example
* @code
                DRV_URT2_SendTXData(0x55);
* @endcode
*******************************************************************************
*/
void DRV_URT2_SendTXData(uint8_t TXData)
{
    _push_(SFRPI);
    SFRPI=3;
    S2BUF=TXData;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       UART2 read receive data
* @details    Read S2BUF
* @return      S2BUF
* @note        None
* @par         Example
* @code
usertmp=DRV_URT2_ReceiveRXData();
* @endcode
*******************************************************************************
*/
uint8_t DRV_URT2_ReceiveRXData(void)
{
    uint8_t tmp;
    _push_(SFRPI);
    SFRPI=3;
    tmp=S2BUF;
    _pop_(SFRPI);
    return(tmp);
}



