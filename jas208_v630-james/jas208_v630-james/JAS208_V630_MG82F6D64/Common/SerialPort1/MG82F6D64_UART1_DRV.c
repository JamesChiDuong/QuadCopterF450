/**
 ******************************************************************************
 *
 * @file        MG82F6D64_UART1_DRV.c
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
* @brief       UART1 read interrupt flag
* @details     Read TI1
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_URT1_GetTI1();
* @endcode
*******************************************************************************
*/
bool DRV_URT1_GetTI1(void)
{
    _push_(SFRPI);
    SFRPI=1;
    if(TI1==1)
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
* @brief       UART1 read interrupt flag
* @details    Read RI1
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_URT1_GetRI1();
* @endcode
*******************************************************************************
*/
bool DRV_URT1_GetRI1(void)
{
    _push_(SFRPI);
    SFRPI=1;
    if(RI1==1)
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
* @brief       UART1 clear interrupt flag
* @details     Clear TI1
* @return      None
* @note        None
* @par         Example
* @code
                DRV_URT1_ClearTI1();
* @endcode
*******************************************************************************
*/
void DRV_URT1_ClearTI1(void)
{
    _push_(SFRPI);
    SFRPI=1;
    TI1=0;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       UART1 clear interrupt flag
* @details    Clear RI1
* @return      None
* @note        None
* @par         Example
* @code
                DRV_URT1_ClearRI1();
* @endcode
*******************************************************************************
*/
void DRV_URT1_ClearRI1(void)
{
    _push_(SFRPI);
    SFRPI=1;
    RI1=0;
    _pop_(SFRPI);
}

/**
*******************************************************************************
* @brief       UART1 transfer data
* @details    Set TB81
* @return      None
* @note        None
* @par         Example
* @code
                DRV_URT1_SetTXData9th();
* @endcode
*******************************************************************************
*/
void DRV_URT1_SetTXData9th(void)
{
    _push_(SFRPI);
    SFRPI=1;
    TB81=1;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       UART1 transfer data
* @details    Clear TB81
* @return      None
* @note        None
* @par         Example
* @code
DRV_URT1_ClearTXData9th();
* @endcode
*******************************************************************************
*/
void DRV_URT1_ClearTXData9th(void)
{
    _push_(SFRPI);
    SFRPI=1;
    TB81=0;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       UART1 read receive data
* @details    Read RB81
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                usertmp=DRV_URT1_GetRXData9th();
* @endcode
*******************************************************************************
*/
bool DRV_URT1_GetRXData9th(void)
{
    _push_(SFRPI);
    SFRPI=1;
    if(RB81==1)
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
* @brief       UART1 transfer data
* @details    Send S1BUF
* @param[in]   TXData:
*  @arg\b       userdata(0~255)
* @return      None
* @note        None
* @par         Example
* @code
                DRV_URT1_SendTXData(0x55);
* @endcode
*******************************************************************************
*/
void DRV_URT1_SendTXData(uint8_t TXData)
{
    _push_(SFRPI);
    SFRPI=1;
    S1BUF=TXData;
    _pop_(SFRPI);
}


/**
*******************************************************************************
* @brief       UART1 read receive data
* @details    Read S1BUF
* @return      S1BUF
* @note        None
* @par         Example
* @code
usertmp=DRV_URT1_ReceiveRXData();
* @endcode
*******************************************************************************
*/
uint8_t DRV_URT1_ReceiveRXData(void)
{
    uint8_t tmp;
    _push_(SFRPI);
    SFRPI=1;
    tmp=S1BUF;
    _pop_(SFRPI);
    return(tmp);
}



