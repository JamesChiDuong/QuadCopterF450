/**
 ******************************************************************************
 *
 * @file        MG82F6D64_SI2C_DRV.H
 *
 * @brief       This is the C code format driver head file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     v1.10
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


#ifndef _MG82F6D64_SI2C_DRV_H
#define _MG82F6D64_SI2C_DRV_H


/**
 *******************************************************************************
 * @brief       Enable SI2C0 Hardware Interrupt
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_SI2C_IT_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_SI2C_IT_Enable()        SFIE |= SIDFIE



/**
 *******************************************************************************
 * @brief       Disable SI2C Hardware Interrupt
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_SI2C_IT_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_SI2C_IT_Disable()        SFIE &= ~SIDFIE


/**
 *******************************************************************************
 * @brief       Enable/Disable SI2C Hardware Interrupt
 * @details
 * @param[in]   __STATE__ : config SI2C hardware interrupt control bit
 *  @arg\b      MW_DISABLE : Set SI2C hardware interrupt disable (Default)
 *  @arg\b      MW_ENABLE : Set SI2C hardware interrupt enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_SI2C_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_SI2C_IT_Cmd(__STATE__)\
    MWT(\
        __STATE__==MW_ENABLE?(SFIE |= SIDFIE):(SFIE &= ~SIDFIE);\
    )


/**
 *******************************************************************************
 * @brief       Get SI2C start flag
 * @details
 * @return      STAF : return STAF bit status
 * @note
 * @par         Example
 * @code
 *    __DRV_SI2C_GetStartFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_SI2C_GetStartFlag()        AUXR2 & STAF
#define __DRV_SI2C_Start_GetFlag()       AUXR2 & STAF

/**
 *******************************************************************************
 * @brief       Get SI2C stop flag
 * @details
 * @return      STOF : return STOF bit status
 * @note
 * @par         Example
 * @code
 *    __DRV_SI2C_GetStopFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_SI2C_GetStopFlag()        AUXR2 & STOF
#define __DRV_SI2C_Stop_GetFlag()       AUXR2 & STOF

/**
 *******************************************************************************
 * @brief       Clear SI2C start flag
 * @details
 * @return
 * @note
 * @par         Example
 * @code
 *    __DRV_SI2C_ClearStartFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_SI2C_ClearStartFlag()        AUXR2 &= ~STAF
#define __DRV_SI2C_Start_ClearFlag()       AUXR2 &= ~STAF


/**
 *******************************************************************************
 * @brief       Clear SI2C stop flag
 * @details
 * @return
 * @note
 * @par         Example
 * @code
 *    __DRV_SI2C_ClearStopFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_SI2C_ClearStopFlag()        AUXR2 &= ~STOF
#define __DRV_SI2C_Stop_ClearFlag()       AUXR2 &= ~STOF

/**
 *******************************************************************************
 * @brief       Set SI2C input pin source
 * @details
 * @param[in]   __SELECT__ : Set external SI2C input pin source
 *  @arg\b      SI2C_SCL_nINT1_SDA_S0MI : SI2C_SCL=nINT1, SI2C_SDA=S0MI
 *  @arg\b      SI2C_TWI0_SCL_SDA : SI2C_SCL=TWI0_SCL, SI2C_SDA=TWI0_SDA
 *  @arg\b      SI2C_TWI1_SCL_SDA : SI2C_SCL=TWI1_SCL, SI2C_SDA=TWI1_SDA
 *  @arg\b      SI2C_SCL_T2EXI_SDA_T3EXI : SI2C_SCL=T2EXI, SI2C_SDA=T3EXI
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_SI2C_PinMux_Select(SI2C_TWI0_SCL_SDA);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_SI2C_PinMux_Select
#define SI2C_SCL_nINT1_SDA_S0MI         0x00
#define SI2C_TWI0_SCL_SDA               SIDPS0
#define SI2C_TWI1_SCL_SDA               SIDPS1
#define SI2C_SCL_T2EXI_SDA_T3EXI        (SIDPS1 | SIDPS0)
/// @endcond
#define __DRV_SI2C_PinMux_Select(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR9_Page);\
        AUXR9 = (AUXR9 & ~SI2C_SCL_T2EXI_SDA_T3EXI) | __SELECT__;\
        __DRV_SFR_PageIndex(0);\
    )


#endif  //_MG82F6D64_SI2C_DRV_H
