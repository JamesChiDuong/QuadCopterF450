/**
 ******************************************************************************
 *
 * @file        MG82F6D64_UART3_DRV.c
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
* @brief       UART3 read interrupt flag
* @details     Read TI3
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_URT3_GetTI3();
* @endcode
*******************************************************************************
*/
bool DRV_URT3_GetTI3(void)
{
    _push_(SFRPI);
    SFRPI=4;
    if(TI3==1)
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
* @brief       UART3 read interrupt flag
* @details    Read RI3
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_URT3_GetRI3();
* @endcode
*******************************************************************************
*/
bool DRV_URT3_GetRI3(void)
{
    _push_(SFRPI);
    SFRPI=4;
    if(RI3==1)
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
* @brief       UART3 clear interrupt flag
* @details     Clear TI3
* @return      None
* @note        None
* @par         Example
* @code
                DRV_URT3_ClearTI3();
* @endcode
*******************************************************************************
*/
void DRV_URT3_ClearTI3(void)
{
    _push_(SFRPI);
    SFRPI=4;
    TI3=0;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       UART3 clear interrupt flag
* @details    Clear RI3
* @return      None
* @note        None
* @par         Example
* @code
                DRV_URT3_ClearRI3();
* @endcode
*******************************************************************************
*/
void DRV_URT3_ClearRI3(void)
{
    _push_(SFRPI);
    SFRPI=4;
    RI3=0;
    _pop_(SFRPI);
}

/**
*******************************************************************************
* @brief       UART3 transfer data
* @details    Set TB83
* @return      None
* @note        None
* @par         Example
* @code
                DRV_URT3_SetTXData9th();
* @endcode
*******************************************************************************
*/
void DRV_URT3_SetTXData9th(void)
{
    _push_(SFRPI);
    SFRPI=4;
    TB83=1;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       UART3 transfer data
* @details    Clear TB83
* @return      None
* @note        None
* @par         Example
* @code
DRV_URT3_ClearTXData9th();
* @endcode
*******************************************************************************
*/
void DRV_URT3_ClearTXData9th(void)
{
    _push_(SFRPI);
    SFRPI=4;
    TB83=0;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       UART3 read receive data
* @details    Read RB83
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                usertmp=DRV_URT3_GetRXData9th();
* @endcode
*******************************************************************************
*/
bool DRV_URT3_GetRXData9th(void)
{
    _push_(SFRPI);
    SFRPI=4;
    if(RB83==1)
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
* @brief       UART3 transfer data
* @details    Send S3BUF
* @param[in]   TXData:
*  @arg\b       userdata(0~255)
* @return      None
* @note        None
* @par         Example
* @code
                DRV_URT3_SendTXData(0x55);
* @endcode
*******************************************************************************
*/
void DRV_URT3_SendTXData(uint8_t TXData)
{
    _push_(SFRPI);
    SFRPI=4;
    S3BUF=TXData;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       UART3 read receive data
* @details    Read S3BUF
* @return      S3BUF
* @note        None
* @par         Example
* @code
usertmp=DRV_URT3_ReceiveRXData();
* @endcode
*******************************************************************************
*/
uint8_t DRV_URT3_ReceiveRXData(void)
{
    uint8_t tmp;
    _push_(SFRPI);
    SFRPI=4;
    tmp=S3BUF;
    _pop_(SFRPI);
    return(tmp);
}



