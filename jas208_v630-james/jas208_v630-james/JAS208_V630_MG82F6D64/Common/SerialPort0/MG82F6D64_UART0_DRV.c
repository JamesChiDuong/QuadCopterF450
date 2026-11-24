/**
 ******************************************************************************
 *
 * @file        MG82F6D64_UART0_DRV.c
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
* @brief       UART0 read TX interrupt flag
* @details     Read TI0
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_URT0_GetTI0();
* @endcode
*******************************************************************************
*/
bool DRV_URT0_GetTI0(void)
{
    _push_(SFRPI);
    SFRPI=0;
    if(TI0==1)
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
* @brief       UART0 read RX interrupt flag
* @details    Read RI0
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_URT0_GetRI0();
* @endcode
*******************************************************************************
*/
bool DRV_URT0_GetRI0(void)
{
    _push_(SFRPI);
    SFRPI=0;
    if(RI0==1)
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
* @brief       UART0 clear TX interrupt flag
* @details     Clear TI0
* @return      None
* @note        None
* @par         Example
* @code
                DRV_URT0_ClearTI0();
* @endcode
*******************************************************************************
*/
void DRV_URT0_ClearTI0(void)
{
    _push_(SFRPI);
    SFRPI=0;
    TI0=0;
    _pop_(SFRPI);
}
/**
*******************************************************************************
* @brief       UART0 clear interrupt flag
* @details    Clear RI0
* @return      None
* @note        None
* @par         Example
* @code
                DRV_URT0_ClearRI0();
* @endcode
*******************************************************************************
*/

void DRV_URT0_ClearRI0(void)
{
    _push_(SFRPI);
    SFRPI=0;
    RI0=0;
    _pop_(SFRPI);
}
/**
*******************************************************************************
* @brief       UART0 Disable SM20 function
* @details     Clear SM20
* @return      None
* @note        None
* @par         Example
* @code
                DRV_URT0_ClearSM20();
* @endcode
*******************************************************************************
*/
void DRV_URT0_ClearSM20(void)
{
    _push_(SFRPI);
    SFRPI=0;
    SM20=0;
    _pop_(SFRPI);
}
/**
*******************************************************************************
* @brief       UART0 Enable SM20 function
* @details     Set SM20
* @return      None
* @note        Enable the automatic address recognition feature in Modes 2 and 3.
* @par         Example
* @code
                DRV_URT0_SetSM20();
* @endcode
*******************************************************************************
*/
void DRV_URT0_SetSM20(void)
{
    _push_(SFRPI);
    SFRPI=0;
    SM20=1;
    _pop_(SFRPI);
}
/**
*******************************************************************************
* @brief       UART0 Set SADDR
* @details     Set SADDR
* @param[in]   Address :
*  @arg\b       userdata(0~255)
* @return      None
* @note        Enable the automatic address recognition feature in Modes 2 and 3.
* @par         Example
* @code
                DRV_URT0_SetSADDR(0);
* @endcode
*******************************************************************************
*/
void DRV_URT0_SetSADDR(uint8_t Address )
{
    _push_(SFRPI);
    SADDR=Address;
    _pop_(SFRPI);
}
/**
*******************************************************************************
* @brief       UART0 Set SADEN(Mask)
* @details     Set SADEN
* @param[in]   Address :
*  @arg\b       userdata(0~255)
* @return      None
* @note        Enable the automatic address recognition feature in Modes 2 and 3.
* @par         Example
* @code
                DRV_URT0_SetSADEN(0);
* @endcode
*******************************************************************************
*/
void DRV_URT0_SetSADEN(uint8_t Address )
{
    _push_(SFRPI);
    SFRPI=0;
    S0CFG=S0CFG&(~SMOD3);
    SADEN=Address;
    _pop_(SFRPI);
}
/**
*******************************************************************************
* @brief       UART0 transfer data
* @details    Set TB80
* @return      None
* @note        None
* @par         Example
* @code
                DRV_URT0_SetTXData9th();
* @endcode
*******************************************************************************
*/
void DRV_URT0_SetTXData9th(void)
{
    _push_(SFRPI);
    SFRPI=0;
    TB80=1;
    _pop_(SFRPI);
}
/**
*******************************************************************************
* @brief       UART0 transfer data
* @details    Clear TB80
* @return      None
* @note        None
* @par         Example
* @code
                DRV_URT0_ClearTXData9th();
* @endcode
*******************************************************************************
*/
void DRV_URT0_ClearTXData9th(void)
{
    _push_(SFRPI);
    SFRPI=0;
    TB80=0;
    _pop_(SFRPI);
}
/**
*******************************************************************************
* @brief       UART0 receive data
* @details    Read RB80
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                usertmp=DRV_URT0_GetRXData9th();
* @endcode
*******************************************************************************
*/
bool DRV_URT0_GetRXData9th(void)
{
    _push_(SFRPI);
    SFRPI=0;
    if(RB80==1)
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
* @brief       UART0 transfer data
* @details    Send S0BUF
* @param[in]   TXData :
*  @arg\b       userdata(0~255)
* @return      None
* @note        None
* @par         Example
* @code
                DRV_URT0_SendTXData(255);
* @endcode
*******************************************************************************
*/
void DRV_URT0_SendTXData(uint8_t TXData)
{
    _push_(SFRPI);
    SFRPI=0;
    S0BUF=TXData;
    _pop_(SFRPI);
}
/**
*******************************************************************************
* @brief       UART0 receive data
* @details    Read S0BUF
* @return     S0BUF
* @note        None
* @par         Example
* @code
                usertmp=DRV_URT0_ReceiveRXData();
* @endcode
*******************************************************************************
*/
uint8_t DRV_URT0_ReceiveRXData(void)
{
    uint8_t tmp;
    _push_(SFRPI);
    SFRPI=0;
    tmp=S0BUF;
    _pop_(SFRPI);
    return(tmp);
}

/**
*******************************************************************************
* @brief       UART0 lin set sync-break-flag
* @details    Set SBF0 and SBF0=0
* @return
* @note        None
* @par         Example
* @code
                DRV_URT0_SetSyncBreakFlag();
* @endcode
*******************************************************************************
*/

void DRV_URT0_SetSyncBreakFlag(void)
{
    _push_(SFRPI);
    SFRPI=0;
    S0CFG1=S0CFG1|SBF0;
    _pop_(SFRPI);
}
/**
*******************************************************************************
* @brief       UART0 lin sync-break-flag
* @details    Read SBF0
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_URT0_GetSyncBreakFlag();
* @endcode
*******************************************************************************
*/
bool DRV_URT0_GetSyncBreakFlag(void)
{
    _push_(SFRPI);
    SFRPI=0;
    if((S0CFG1&SBF0)==SBF0)
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
* @brief       UART0 lin sync-break
* @details    Set SYNC0
* @return
* @note        None
* @par         Example
* @code
                DRV_URT0_SetSyncBreak();
* @endcode
*******************************************************************************
*/
void DRV_URT0_SetSyncBreak(void)
{
    _push_(SFRPI);
    SFRPI=0;
    S0CFG1=S0CFG1|SYNC0;
    _pop_(SFRPI);
}
/**
*******************************************************************************
* @brief       UART0 lin sync-break
* @details    Read SYNC0
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_URT0_GetSyncBreak();
* @endcode
*******************************************************************************
*/
bool DRV_URT0_GetSyncBreak(void)
{
    _push_(SFRPI);
    SFRPI=0;
    if((S0CFG1&SYNC0)==SYNC0)
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
* @brief       UART0 lin sync-break
* @details    Set ATBR0
* @return
* @note        None
* @par         Example
* @code
                DRV_URT0_SetAutoBaudRate();
* @endcode
*******************************************************************************
*/
void DRV_URT0_SetAutoBaudRate(void)
{
    _push_(SFRPI);
    SFRPI=0;
    S0CFG1=S0CFG1|ATBR0;
    _pop_(SFRPI);
}
/**
*******************************************************************************
* @brief       UART0 get baud rate
* @details    Read S0BRT
* @return     S0BRT
* @note        None
* @par         Example
* @code
                usertmp=DRV_URT0_GetBaudRate();
* @endcode
*******************************************************************************
*/
uint8_t DRV_URT0_GetBaudRate(void)
{
    uint8_t tmp;
    _push_(SFRPI);
    SFRPI=0;
    tmp=S0BRT;
    _pop_(SFRPI);
    return(tmp);
}
/**
*******************************************************************************
* @brief       UART0 lin transmit-error-flag
* @details    Set TXER0 and TXER0=0
* @return
* @note        None
* @par         Example
* @code
                DRV_URT0_SetTransmitErrorFlag();
* @endcode
*******************************************************************************
*/
void DRV_URT0_SetTransmitErrorFlag(void)
{
    _push_(SFRPI);
    SFRPI=0;
    S0CFG1=S0CFG1|TXER0;
    _pop_(SFRPI);
}
/**
*******************************************************************************
* @brief       UART0 lin transmit-error-flag
* @details    Read
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_URT0_GetTransmitErrorFlag();
* @endcode
*******************************************************************************
*/
bool DRV_URT0_GetTransmitErrorFlag(void)
{
    _push_(SFRPI);
    SFRPI=0;
    if((S0CFG1&TXER0)==TXER0)
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





