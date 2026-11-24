/**
 ******************************************************************************
 *
 * @file        MG82F6D64_RST_DRV.H
 *
 * @brief       This is the C code format driver head file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     V1.10
 * @date        2022/06/09
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

 
#ifndef _MG82F6D64_RST_DRV_H
#define _MG82F6D64_RST_DRV_H




/**
 *******************************************************************************
 * @brief       Enable Software Reset
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_RST_SWRST_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_RST_SWRST_Enable()        ISPCR |= SRST





/**
 *******************************************************************************
 * @brief       Software Reset Boot Selection
 * @details
 * @param[in]   __SELECT__ : Setting to the software reset boot from AP or ISP
 *  @arg\b      BOOT_FROM_AP : 0(Default)
 *  @arg\b      BOOT_FROM_ISP : SWBS
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_RST_SWRST_Boot(BOOT_FROM_ISP);
 * @endcode
 *******************************************************************************
 */
#define BOOT_FROM_AP    0
#define BOOT_FROM_ISP    SWBS
#define __DRV_RST_SWRST_Boot(__SELECT__)\
    MWT(\
        __SELECT__==BOOT_FROM_ISP?(ISPCR |= SWBS):(ISPCR &= ~SWBS);\
    )



/**
 *******************************************************************************
 * @brief       Stack Pointer Warning Reset enable/disable.
 * @details
 * @param[in]   __STATE__ : SPWF trigger reset selection.
 *  @arg\b      MW_ENABLE : Enable SPWF to trigger a MCU reset.
 *  @arg\b      MW_DISABLE : Disable SPWF to trigger a MCU reset.(Default)
 * @return      None
 * @note        
 * @par         Example
 * @code
 *    __DRV_RST_SPWRE_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_RST_SPWRE_Cmd(__STATE__)\
    MWT(\
        DRV_PageP_Read(PCON3_P);\
        __STATE__ == MW_ENABLE ? (IFD |= SPWRE_P) : (IFD &= ~SPWRE_P);\
        DRV_PageP_Write(PCON3_P, IFD);\
    )
    

/**
 *******************************************************************************
 * @brief       Get Software Reset flag
 * @details
 * @return      SWRF : return SWRF bit status
 * @note
 * @par         Example
 * @code
 *    __DRV_RST_SWRF_GetFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_RST_SWRF_GetFlag()        (PCON1 & SWRF)


/**
 *******************************************************************************
 * @brief       Clear Software Reset flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_RST_SWRF_ClearFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_RST_SWRF_ClearFlag()        PCON1 = SWRF



/**
 *******************************************************************************
 * @brief       Get External Reset flag
 * @details
 * @return      EXRF : return EXRF bit status
 * @note
 * @par         Example
 * @code
 *    __DRV_RST_EXRF_GetFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_RST_EXRF_GetFlag()        (PCON1 & EXRF)


/**
 *******************************************************************************
 * @brief       Clear External Reset flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_RST_EXRF_ClearFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_RST_EXRF_ClearFlag()        PCON1 = EXRF




#endif  //_MG82F6D64_RST_DRV_H
