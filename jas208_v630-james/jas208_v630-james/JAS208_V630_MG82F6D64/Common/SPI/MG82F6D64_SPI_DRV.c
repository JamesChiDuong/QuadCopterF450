/**
 ******************************************************************************
 *
 * @file        MG82F6D64_SPI_DRV.c
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
* @brief       SPI transfer completion flag
* @details     Read SPIF
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_SPI_GetSPIF();
* @endcode
*******************************************************************************
*/
bool DRV_SPI_GetSPIF(void)
{
    _push_(SFRPI);
    if((SPSTAT&SPIF)==SPIF)
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
* @brief       SPI transfer completion flag
* @details     Clear SPIF
* @return      None
* @note        The SPIF is cleared in software by writing “1” to this bit.
* @par         Example
* @code
                DRV_SPI_ClearSPIF();
* @endcode
*******************************************************************************
*/
void DRV_SPI_ClearSPIF(void)
{
    _push_(SFRPI);
    SPSTAT=SPSTAT|SPIF;
    _pop_(SFRPI);
}
/**
*******************************************************************************
* @brief       SPI write collision flag
* @details    Read WCOL
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_SPI_GetWCOL();
* @endcode
*******************************************************************************
*/
bool DRV_SPI_GetWCOL(void)
{
    _push_(SFRPI);
    if((SPSTAT&WCOL)==WCOL)
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
* @brief       SPI write collision flag
* @details     Clear WCOL
* @return      None
* @note        The WCOL flag is cleared in software by writing “1” to this bit.
* @par         Example
* @code
                DRV_SPI_ClearWCOL();
* @endcode
*******************************************************************************
*/
void DRV_SPI_ClearWCOL(void)
{
    _push_(SFRPI);
    SPSTAT=SPSTAT|WCOL;
    _pop_(SFRPI);
}
/**
*******************************************************************************
* @brief       Transmit Holding Register (THR) Full flag
* @details    Read THRF
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_SPI_GetTHRF();
* @endcode
*******************************************************************************
*/
bool DRV_SPI_GetTHRF(void)
{
    _push_(SFRPI);
    if((SPSTAT&THRF)==THRF)
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
* @brief       SPI Busy flag
* @details    Read SPIBSY
* @return      TRUE or FALSE
* @note        None
* @par         Example
* @code
                DRV_SPI_GetSPIBSY();
* @endcode
*******************************************************************************
*/
bool DRV_SPI_GetSPIBSY(void)
{
    _push_(SFRPI);
    if((SPSTAT&SPIBSY)==SPIBSY)
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
* @brief       SPI Mode Fault Flag
* @details    Read MODF
* @return      TRUE or FALSE
* @note        This bit is not automatically cleared by hardware, and must be cleared by software writing “1”.
* @par         Example
* @code
                DRV_SPI_GetMODF();
* @endcode
*******************************************************************************
*/
bool DRV_SPI_GetMODF(void)
{
    _push_(SFRPI);
    if((SPSTAT&MODF)==MODF)
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
* @brief       SPI Mode Fault Flag
* @details     Clear MODF
* @return      None
* @note        This bit is not automatically cleared by hardware, and must be cleared by software writing “1”.
* @par         Example
* @code
                DRV_SPI_ClearMODF();
* @endcode
*******************************************************************************
*/
void DRV_SPI_ClearMODF(void)
{
    _push_(SFRPI);
    SPSTAT=SPSTAT|MODF;
    _pop_(SFRPI);
}
/**
*******************************************************************************
* @brief       SPI transfer data
* @details    Send SPDAT
* @param[in]   TXData :
*  @arg\b       userdata(0~255)
* @return      None
* @note        None
* @par         Example
* @code
                DRV_SPI_SendSPDAT(0);
* @endcode
*******************************************************************************
*/
void DRV_SPI_SendSPDAT(uint8_t TXData)
{
    _push_(SFRPI);
    SPDAT=TXData;
    _pop_(SFRPI);
}
/**
*******************************************************************************
* @brief       SPI receive data
* @details    Read SPDAT
* @return     SPDAT
* @note        None
* @par         Example
* @code
                usertmp=DRV_SPI_ReceiveSPDAT();
* @endcode
*******************************************************************************
*/
uint8_t DRV_SPI_ReceiveSPDAT(void)
{
	uint8_t tmp;
    _push_(SFRPI);
    tmp=SPDAT;
    _pop_(SFRPI);
    return(tmp);
}


