/**
 ******************************************************************************
 *
 * @file        MG82F6D64_GPIO_DRV.c
 *
 * @brief       This is the C code format driver head file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     V1.02
 * @date        2021/10/28
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
 * @brief       Get Port 6 Pin 0 Status
 * @details
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = DRV_GPIO_GetPin60();
 * @endcode
 *******************************************************************************
 */
bool DRV_GPIO_GetPin60(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(P6_Page);
    if (P6 & P6Pin0)
    {
        _pop_(SFRPI);
        return 1;
    }
    _pop_(SFRPI);
    return 0;
}

/**
 *******************************************************************************
 * @brief       Get Port 6 Pin 1 Status
 * @details
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = DRV_GPIO_GetPin61();
 * @endcode
 *******************************************************************************
 */
bool DRV_GPIO_GetPin61(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(P6_Page);
    if (P6 & P6Pin1)
    {
        _pop_(SFRPI);
        return 1;
    }
    _pop_(SFRPI);
    return 0;
}

/**
 *******************************************************************************
 * @brief       Get Port 6 Pin 2 Status
 * @details
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = DRV_GPIO_GetPin62();
 * @endcode
 *******************************************************************************
 */
bool DRV_GPIO_GetPin62(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(P6_Page);
    if (P6 & P6Pin2)
    {
        _pop_(SFRPI);
        return 1;
    }
    _pop_(SFRPI);
    return 0;
}

/**
 *******************************************************************************
 * @brief       Get Port 6 Pin 3 Status
 * @details
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = DRV_GPIO_GetPin63();
 * @endcode
 *******************************************************************************
 */
bool DRV_GPIO_GetPin63(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(P6_Page);
    if (P6 & P6Pin3)
    {
        _pop_(SFRPI);
        return 1;
    }
    _pop_(SFRPI);
    return 0;
}

/**
 *******************************************************************************
 * @brief       Get Port 6 Pin 4 Status
 * @details
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = DRV_GPIO_GetPin64();
 * @endcode
 *******************************************************************************
 */
bool DRV_GPIO_GetPin64(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(P6_Page);
    if (P6 & P6Pin4)
    {
        _pop_(SFRPI);
        return 1;
    }
    _pop_(SFRPI);
    return 0;
}

/**
 *******************************************************************************
 * @brief       Get Port 6 Pin 5 Status
 * @details
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = DRV_GPIO_GetPin65();
 * @endcode
 *******************************************************************************
 */
bool DRV_GPIO_GetPin65(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(P6_Page);
    if (P6 & P6Pin5)
    {
        _pop_(SFRPI);
        return 1;
    }
    _pop_(SFRPI);
    return 0;
}

/**
 *******************************************************************************
 * @brief       Get Port 6 Pin 6 Status
 * @details
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = DRV_GPIO_GetPin66();
 * @endcode
 *******************************************************************************
 */
bool DRV_GPIO_GetPin66(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(P6_Page);
    if (P6 & P6Pin6)
    {
        _pop_(SFRPI);
        return 1;
    }
    _pop_(SFRPI);
    return 0;
}

/**
 *******************************************************************************
 * @brief       Get Port 6 Pin 7 Status
 * @details
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = DRV_GPIO_GetPin67();
 * @endcode
 *******************************************************************************
 */
bool DRV_GPIO_GetPin67(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(P6_Page);
    if (P6 & P6Pin7)
    {
        _pop_(SFRPI);
        return 1;
    }
    _pop_(SFRPI);
    return 0;
}

/**
 *******************************************************************************
 * @brief       Get Port 7 Pin 0 Status
 * @details
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = DRV_GPIO_GetPin70();
 * @endcode
 *******************************************************************************
 */
bool DRV_GPIO_GetPin70(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(P7_Page);
    if (P7 & P7Pin0)
    {
        _pop_(SFRPI);
        return 1;
    }
    _pop_(SFRPI);
    return 0;
}

/**
 *******************************************************************************
 * @brief       Get Port 7 Pin 1 Status
 * @details
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = DRV_GPIO_GetPin71();
 * @endcode
 *******************************************************************************
 */
bool DRV_GPIO_GetPin71(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(P7_Page);
    if (P7 & P7Pin1)
    {
        _pop_(SFRPI);
        return 1;
    }
    _pop_(SFRPI);
    return 0;
}

/**
 *******************************************************************************
 * @brief       Get Port 7 Pin 2 Status
 * @details
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = DRV_GPIO_GetPin72();
 * @endcode
 *******************************************************************************
 */
bool DRV_GPIO_GetPin72(void)
{
    _push_(SFRPI);
    __DRV_SFR_PageIndex(P7_Page);
    if (P7 & P7Pin2)
    {
        _pop_(SFRPI);
        return 1;
    }
    _pop_(SFRPI);
    return 0;
}
