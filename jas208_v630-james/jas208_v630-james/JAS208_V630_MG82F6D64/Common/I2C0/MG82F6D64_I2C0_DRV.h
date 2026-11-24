/**
 ******************************************************************************
 *
 * @file        MG82F6D64_I2C0_DRV.H
 *
 * @brief       This is the C code format driver head file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     v1.03
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

 
#ifndef _MG82F6D64_I2C0_DRV_H
#define _MG82F6D64_I2C0_DRV_H


/**
 *******************************************************************************
 * @brief       Enable I2C0 Hardware
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_Enable()        SICON |= ENSI


/**
 *******************************************************************************
 * @brief       Disable I2C0 Hardware
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_Disable()        SICON &= ~ENSI


/**
 *******************************************************************************
 * @brief       Enable/Disable I2C0 Hardware
 * @details
 * @param[in]   __STATE__ : config I2C0 hardware control bit
 *  @arg\b      MW_DISABLE : Set I2C0 hardware disable (Default)
 *  @arg\b      MW_ENABLE : Set I2C0 hardware enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_Cmd(__STATE__)\
    MWT(\
        __STATE__==MW_ENABLE?(SICON |= ENSI):(SICON &= ~ENSI);\
    )



/**
 *******************************************************************************
 * @brief       Enable I2C0 Hardware Interrupt
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_IT_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_IT_Enable()        EIE1 |= ETWI0



/**
 *******************************************************************************
 * @brief       Disable I2C0 Hardware Interrupt
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_IT_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_IT_Disable()        EIE1 &= ~ETWI0



/**
 *******************************************************************************
 * @brief       Enable/Disable I2C0 Hardware Interrupt
 * @details
 * @param[in]   __STATE__ : config I2C0 hardware interrupt control bit
 *  @arg\b      MW_DISABLE : Set I2C0 hardware interrupt disable (Default)
 *  @arg\b      MW_ENABLE : Set I2C0 hardware interrupt enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_IT_Cmd(__STATE__)\
    MWT(\
        __STATE__==MW_ENABLE?(EIE1 |= ETWI0):(EIE1 &= ~ETWI0);\
    )


/**
 *******************************************************************************
 * @brief       Set I2C0 START flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_SetSTART();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_SetSTART()        SICON |= STA


/**
 *******************************************************************************
 * @brief       Clear I2C0 START flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_ClearSTART();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_ClearSTART()        SICON &= ~STA


/**
 *******************************************************************************
 * @brief       Set I2C0 STOP flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_SetSTOP();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_SetSTOP()        SICON |= STO


/**
 *******************************************************************************
 * @brief       Polling for I2C0 stop event conversion complete.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_PollForStopComplete();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_PollForStopComplete()\
    MWT(\
        while((SICON & STO) == STO);\
    )



/**
 *******************************************************************************
 * @brief        Set I2C0 state event flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_SetEventFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_SetEventFlag()        SICON |= SI


/**
 *******************************************************************************
 * @brief       Clear I2C0 state event flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_ClearEventFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_ClearEventFlag()        SICON &= ~SI


/**
 *******************************************************************************
 * @brief       Get I2C0 state event flag
 * @details
 * @return      SI : return SI bit status
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_GetEventFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_GetEventFlag()        SICON & SI



/**
 *******************************************************************************
 * @brief       Polling for I2C0 state event conversion complete.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_PollForStateComplete();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_PollForStateComplete()\
    MWT(\
        while((SICON & SI) != SI);\
    )


/**
 *******************************************************************************
 * @brief       Set I2C0 ACK flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_SetACK();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_SetACK()        SICON |= AA


/**
 *******************************************************************************
 * @brief       Clear I2C0 ACK flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_SetACK();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_ClearACK()        SICON &= ~AA


/**
 *******************************************************************************
 * @brief       Set I2C0 STA STO AA 000
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_Set_STA_STO_AA_000();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_Set_STA_STO_AA_000()\
    MWT(\
        SICON &= ~(STA | STO | AA);\
    )


/**
 *******************************************************************************
 * @brief       Set I2C0 STA STO AA 001
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_Set_STA_STO_AA_001();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_Set_STA_STO_AA_001()\
    MWT(\
        SICON = (SICON & ~(STA | STO | AA)) | AA;\
    )


/**
 *******************************************************************************
 * @brief       Set I2C0 STA STO AA 010
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_Set_STA_STO_AA_010();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_Set_STA_STO_AA_010()\
    MWT(\
        SICON = (SICON & ~(STA | STO | AA)) | STO;\
    )


/**
 *******************************************************************************
 * @brief       Set I2C0 STA STO AA 100
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_Set_STA_STO_AA_100();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_Set_STA_STO_AA_100()\
    MWT(\
        SICON = (SICON & ~(STA | STO | AA)) | STA;\
    )


/**
 *******************************************************************************
 * @brief       Set I2C0 STA STO AA 101
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_Set_STA_STO_AA_101();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_Set_STA_STO_AA_101()\
    MWT(\
        SICON = (SICON & ~(STA | STO | AA)) | (STA | AA);\
    )


/**
 *******************************************************************************
 * @brief       Set I2C0 STA STO AA 110
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_Set_STA_STO_AA_110();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_Set_STA_STO_AA_110()\
    MWT(\
        SICON = (SICON & ~(STA | STO | AA)) | (STA | STO);\
    )


/**
 *******************************************************************************
 * @brief       Enable I2C0 GC Function
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_GC_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_GC_Enable()        SIADR |= GC



/**
 *******************************************************************************
 * @brief       Disable I2C0 GC Function
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_GC_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_GC_Disable()        SIADR &= ~GC



/**
 *******************************************************************************
 * @brief       Enable/Disable I2C0 GC Function
 * @details
 * @param[in]   __STATE__ : config I2C0 GC functiont control bit
 *  @arg\b      MW_DISABLE : Set I2C0 GC functiont disable (default)
 *  @arg\b      MW_ENABLE : Set I2C0 GC functiont enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_GC_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_GC_Cmd(__STATE__)\
    MWT(\
        __STATE__==MW_ENABLE?(SIADR |= GC):(SIADR &= ~GC);\
    )



/**
 *******************************************************************************
 * @brief       Get I2C0 state event code
 * @details
 * @return      SISTA : return SISTA status
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_GetEventCode();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_GetEventCode()        SISTA



/**
 *******************************************************************************
 * @brief       Set I2C0 slave device address 
 * @details
 * @param[in]   __SELECT__ : Programmable I2C0 slave address
 *  @arg\b      0x00~0x7F : Set the slave device address to SIADR
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_SetSlaveAddress(0x00~0x7F);
 * @endcode
 *******************************************************************************
*/
#define __DRV_I2C0_SetSlaveAddress(__SELECT__)\
    MWT(\
        SIADR &= GC;\
        SIADR |= (__SELECT__<<1)&~GC;\
    )
    


/**
 *******************************************************************************
 * @brief       Set I2C0 slave device address write
 * @details
 * @param[in]   __SELECT__ : Programmable I2C0 slave address write
 *  @arg\b      0x00~0x7F : Set the slave device address to SIDAT
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_SetAddressWrite(0x00~0x7F);
 * @endcode
 *******************************************************************************
*/
#define __DRV_I2C0_SetAddressWrite(__SELECT__)\
    MWT(\
        SIDAT = (__SELECT__<<1)&0xFE;\
    )



/**
 *******************************************************************************
 * @brief       Set I2C0 slave device address read
 * @details
 * @param[in]   __SELECT__ : Programmable I2C0 slave address read
 *  @arg\b      0x00~0x7F : Set the slave device address to SIDAT
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_SetAddressRead(0x00~0x7F);
 * @endcode
 *******************************************************************************
*/
#define __DRV_I2C0_SetAddressRead(__SELECT__)\
    MWT(\
        SIDAT = (__SELECT__<<1)|0x01;\
    )



/**
 *******************************************************************************
 * @brief       Set I2C0 data
 * @details
 * @param[in]   __SELECT__ : Programmable I2C0 buffer data
 *  @arg\b      0x00~0xFF : Set the data to SIDAT 
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_SetBufData(0x00~0xFF);
 * @endcode
 *******************************************************************************
*/
#define __DRV_I2C0_SetBufData(__SELECT__)        SIDAT = __SELECT__


/**
 *******************************************************************************
 * @brief       Get I2C0 data
 * @details
 * @return      SIDAT : return I2C0 data
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_GetBufData();
 * @endcode
 *******************************************************************************
*/
#define __DRV_I2C0_GetBufData()        SIDAT




/**
 *******************************************************************************
 * @brief       Check I2C0 Bus Idle State
 * @details 
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_State_BusIdle();
 * @endcode
 *******************************************************************************
*/
#define __DRV_I2C0_State_BusIdle()\
    MWT(\
        while(SISTA != 0xF8);\
    )


/**
 *******************************************************************************
 * @brief       Select I2C0 clock rate divide from SYSCLK
 * @details
 * @param[in]   __SELECT__ : Programmable I2C0 clock rate selection.
 *  @arg\b      I2C0_SCL_SYSCLK_DIV_8 : I2C0_SCL = SYSCLK / 8 (Default)
 *  @arg\b      I2C0_SCL_SYSCLK_DIV_16 : I2C0_SCL = SYSCLK / 16
 *  @arg\b      I2C0_SCL_SYSCLK_DIV_32 : I2C0_SCL = SYSCLK / 32
 *  @arg\b      I2C0_SCL_SYSCLK_DIV_64 : I2C0_SCL = SYSCLK / 64
 *  @arg\b      I2C0_SCL_SYSCLK_DIV_128 : I2C0_SCL = SYSCLK / 128
 *  @arg\b      I2C0_SCL_SYSCLK_DIV_256 : I2C0_SCL = SYSCLK / 256
 *  @arg\b      I2C0_SCL_S0TOF_DIV_6 : I2C0_SCL = S0TOF / 6
 *  @arg\b      I2C0_SCL_T0OF_DIV_6 : I2C0_SCL = T0OF / 6
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_ClockRate_Select(I2C0_SCL_SYSCLK_DIV_16);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_I2C0_ClockRate_Select
#define I2C0_SCL_SYSCLK_DIV_8          0x00                        //I2C0_SCL = SYSCLK / 8 (Default)
#define I2C0_SCL_SYSCLK_DIV_16         CR0                         //I2C0_SCL = SYSCLK / 16
#define I2C0_SCL_SYSCLK_DIV_32         CR1                         //I2C0_SCL = SYSCLK / 32
#define I2C0_SCL_SYSCLK_DIV_64         (CR1 | CR0)                 //I2C0_SCL = SYSCLK / 64
#define I2C0_SCL_SYSCLK_DIV_128        CR2                         //I2C0_SCL = SYSCLK / 128
#define I2C0_SCL_SYSCLK_DIV_256        (CR2 | CR0)                 //I2C0_SCL = SYSCLK / 256
#define I2C0_SCL_S0TOF_DIV_6           (CR2 | CR1)                 //I2C0_SCL = S0TOF / 6
#define I2C0_SCL_T0OF_DIV_6            (CR2 | CR1 | CR0)           //I2C0_SCL = T0OF / 6
/// @endcond
#define __DRV_I2C0_ClockRate_Select(__SELECT__)\
    MWT(\
        SICON = (SICON & ~I2C0_SCL_T0OF_DIV_6) | __SELECT__;\
    )




/**
 *******************************************************************************
 * @brief       Set I2C0 input pin source
 * @details
 * @param[in]   __SELECT__ : Set external I2C0 input pin source
 *  @arg\b      I2C0_INPUT_PIN_SELECT_P40_P41 : Set I2C0 input pin source to SCL=P40, SDA=P41 (Default)
 *  @arg\b      I2C0_INPUT_PIN_SELECT_P60_P61 : Set I2C0 input pin source to SCL=P60, SDA=P61
 *  @arg\b      I2C0_INPUT_PIN_SELECT_P31_P30 : Set I2C0 input pin source to SCL=P31, SDA=P30
 *  @arg\b      I2C0_INPUT_PIN_SELECT_P25_P23 : Set I2C0 input pin source to SCL=P25, SDA=P23
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_PinMux_Select(I2C0_INPUT_PIN_SELECT_P31_P30);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_I2C0_PinMux_Select
#define I2C0_INPUT_PIN_SELECT_P40_P41     0x00
#define I2C0_INPUT_PIN_SELECT_P60_P61     TWIPS0
#define I2C0_INPUT_PIN_SELECT_P31_P30     TWIPS1
#define I2C0_INPUT_PIN_SELECT_P25_P23     (TWIPS1 | TWIPS0)
#define I2C0_SCL_P40_SDA_P41              0x00
#define I2C0_SCL_P60_SDA_P61              TWIPS0
#define I2C0_SCL_P31_SDA_P30              TWIPS1
#define I2C0_SCL_P25_SDA_P23              (TWIPS1 | TWIPS0)
/// @endcond
#define __DRV_I2C0_PinMux_Select(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR3_Page);\
        AUXR3 = (AUXR3 & ~I2C0_SCL_P25_SDA_P23) | __SELECT__;\
        __DRV_SFR_PageIndex(0);\
    )



/**
 *******************************************************************************
 * @brief       Enable I2C0 Pre-Assert Acknowledge for DMA Application
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_PAA_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_PAA_Enable()\
    MWT(\
        __DRV_SFR_PageIndex(AUXR10_Page);\
        AUXR10 |= PAA;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Disable I2C0 Pre-Assert Acknowledge for DMA Application
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_PAA_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_PAA_Disable()\
    MWT(\
        __DRV_SFR_PageIndex(AUXR10_Page);\
        AUXR10 &= ~PAA;\
        __DRV_SFR_PageIndex(0);\
    )



/**
 *******************************************************************************
 * @brief       Enable/Disable I2C0 Pre-Assert Acknowledge for DMA Application
 * @details
 * @param[in]   __STATE__ : config I2C0 Pre-Assert Acknowledge control bit
 *  @arg\b      MW_DISABLE : Set I2C0 Pre-Assert Acknowledge disable
 *  @arg\b      MW_ENABLE : Set I2C0 Pre-Assert Acknowledge enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_PAA_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_PAA_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR10_Page);\
        __STATE__==MW_ENABLE?(AUXR10 |= PAA):(AUXR10 &= ~PAA);\
        __DRV_SFR_PageIndex(0);\
    )



/**
 *******************************************************************************
 * @brief       Enable I2C0 TWICF function
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_TWICF_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_TWICF_Enable()\
    MWT(\
        __DRV_SFR_PageIndex(AUXR10_Page);\
        AUXR10 |= TWICF;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Disable I2C0 TWICF function
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_TWICF_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_TWICF_Disable()\
    MWT(\
        __DRV_SFR_PageIndex(AUXR10_Page);\
        AUXR10 &= ~TWICF;\
        __DRV_SFR_PageIndex(0);\
    )



/**
 *******************************************************************************
 * @brief       Enable/Disable I2C0 TWICF function
 * @details
 * @param[in]   __STATE__ : config I2C0 TWICF function control bit
 *  @arg\b      MW_DISABLE : Set I2C0 TWICF function disable
 *  @arg\b      MW_ENABLE : Set I2C0 TWICF function enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_TWICF_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_TWICF_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR10_Page);\
        __STATE__==MW_ENABLE?(AUXR10 |= TWICF):(AUXR10 &= ~TWICF);\
        __DRV_SFR_PageIndex(0);\
    )
    
    
/**
 *******************************************************************************
 * @brief       Enable I2C0 Second Address 
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_SecondAddress_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_SecondAddress_Enable()\
    MWT(\
        __DRV_SFR_PageIndex(SIA2_Page);\
        SIA2 |= A2E;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Disable I2C0 Second Address
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_SecondAddress_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_SecondAddress_Disable()\
    MWT(\
        __DRV_SFR_PageIndex(SIA2_Page);\
        SIA2 &= ~A2E;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Enable/Disable I2C0 Second Address
 * @details
 * @param[in]   __STATE__ : config I2C0 Second Address
 *  @arg\b      MW_DISABLE : Set I2C0 Second Address disable (Default)
 *  @arg\b      MW_ENABLE : Set I2C0 Second Address enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_SecondAddress_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C0_SecondAddress_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(SIA2_Page);\
        __STATE__==MW_ENABLE?(SIA2 |= A2E):(SIA2 &= ~A2E);\
        __DRV_SFR_PageIndex(0);\
    )




/**
 *******************************************************************************
 * @brief       Set I2C0 Second Address
 * @details
 * @param[in]   __SELECT__ : Programmable I2C0 second address
 *  @arg\b      0x00~0xFF : Set the data to SIA2 
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_SetSecondAddress(0x00~0x7F);
 * @endcode
 *******************************************************************************
*/
#define __DRV_I2C0_SetSecondAddress(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(SIA2_Page);\
        SIA2 |= (__SELECT__<<1)&~A2E;\
        __DRV_SFR_PageIndex(0);\
    )
    
    
    
/**
 *******************************************************************************
 * @brief       Set I2C0 Second Address Bit Mask
 * @details
 * @param[in]   __SELECT__ : Programmable I2C0 second address bit mask
 *  @arg\b      0x00~0xFF : Set the data to SIA2M 
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C0_SetSecondAddressMask(0x00~0x7F);
 * @endcode
 *******************************************************************************
*/
#define __DRV_I2C0_SetSecondAddressMask(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(SIA2M_Page);\
        SIA2M |= (__SELECT__<<1)&~A2E;\
        __DRV_SFR_PageIndex(0);\
    )



void Sample_I2C0_ISR_Handle(void);
void Sample_I2C0_Master_Byte_Write(uint8_t I2C0_Address, uint8_t I2C0_Data);
uint8_t Sample_I2C0_Master_Byte_Read(uint8_t I2C0_Address);


#endif  //_MG82F6D64_I2C0_DRV_H
