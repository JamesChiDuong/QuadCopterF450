/**
 ******************************************************************************
 *
 * @file        MG82F6D64_I2C1_DRV.H
 *
 * @brief       This is the C code format driver head file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     v1.02
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

 
#ifndef _MG82F6D64_I2C1_DRV_H
#define _MG82F6D64_I2C1_DRV_H


/**
 *******************************************************************************
 * @brief       Enable I2C1 Hardware
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_Enable()\
    MWT(\
        __DRV_SFR_PageIndex(SI1CON_Page);\
        SI1CON |= ENSI1;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Disable I2C1 Hardware
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_Disable()\
    MWT(\
        __DRV_SFR_PageIndex(SI1CON_Page);\
        SI1CON &= ~ENSI1;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Enable/Disable I2C1 Hardware
 * @details
 * @param[in]   __STATE__ : config I2C1 hardware control bit
 *  @arg\b      MW_DISABLE : Set I2C1 hardware disable (Default)
 *  @arg\b      MW_ENABLE : Set I2C1 hardware enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(SI1CON_Page);\
        __STATE__==MW_ENABLE?(SI1CON |= ENSI1):(SI1CON &= ~ENSI1);\
        __DRV_SFR_PageIndex(0);\
    )



/**
 *******************************************************************************
 * @brief       Enable I2C1 Hardware Interrupt
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_IT_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_IT_Enable()        EIE2 |= ETWI1



/**
 *******************************************************************************
 * @brief       Disable I2C1 Hardware Interrupt
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_IT_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_IT_Disable()        EIE2 &= ~ETWI1



/**
 *******************************************************************************
 * @brief       Enable/Disable I2C1 Hardware Interrupt
 * @details
 * @param[in]   __STATE__ : config I2C1 hardware interrupt control bit
 *  @arg\b      MW_DISABLE : Set I2C1 hardware interrupt disable (Default)
 *  @arg\b      MW_ENABLE : Set I2C1 hardware interrupt enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_IT_Cmd(__STATE__)\
    MWT(\
        __STATE__==MW_ENABLE?(EIE2 |= ETWI1):(EIE2 &= ~ETWI1);\
    )


/**
 *******************************************************************************
 * @brief       Set I2C1 START flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_SetSTART();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_SetSTART()\
    MWT(\
        __DRV_SFR_PageIndex(SI1CON_Page);\
        SI1CON |= STA1;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Clear I2C1 START flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_ClearSTART();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_ClearSTART()\
    MWT(\
        __DRV_SFR_PageIndex(SI1CON_Page);\
        SI1CON &= ~STA1;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Set I2C1 STOP flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_SetSTOP();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_SetSTOP()\
    MWT(\
        __DRV_SFR_PageIndex(SI1CON_Page);\
        SI1CON |= STO1;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Polling for I2C1 stop event conversion complete.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_PollForStopComplete();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_PollForStopComplete()\
    MWT(\
        __DRV_SFR_PageIndex(SI1CON_Page);\
        while((SI1CON & STO1) == STO1);\
        __DRV_SFR_PageIndex(0);\
    )



/**
 *******************************************************************************
 * @brief        Set I2C1 state event flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_SetEventFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_SetEventFlag()\
    MWT(\
        __DRV_SFR_PageIndex(SI1CON_Page);\
        SI1CON |= SI1;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Clear I2C1 state event flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_ClearEventFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_ClearEventFlag()\
    MWT(\
        __DRV_SFR_PageIndex(SI1CON_Page);\
        SI1CON &= ~SI1;\
        __DRV_SFR_PageIndex(0);\
    )



/**
 *******************************************************************************
 * @brief       Polling for I2C1 state event conversion complete.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_PollForStateComplete();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_PollForStateComplete()\
    MWT(\
        __DRV_SFR_PageIndex(SI1CON_Page);\
        while((SI1CON & SI1) != SI1);\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Set I2C1 ACK flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_SetACK();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_SetACK()\
    MWT(\
        __DRV_SFR_PageIndex(SI1CON_Page);\
        SI1CON |= AA1;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Clear I2C1 ACK flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_SetACK();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_ClearACK()\
    MWT(\
        __DRV_SFR_PageIndex(SI1CON_Page);\
        SI1CON &= ~AA1;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Set I2C1 STA1 STO1 AA1 000
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_Set_STA1_STO1_AA1_000();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_Set_STA1_STO1_AA1_000()\
    MWT(\
        __DRV_SFR_PageIndex(SI1CON_Page);\
        SI1CON &= ~(STA1 | STO1 | AA1);\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Set I2C1 STA1 STO1 AA1 001
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_Set_STA1_STO1_AA1_001();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_Set_STA1_STO1_AA1_001()\
    MWT(\
        __DRV_SFR_PageIndex(SI1CON_Page);\
        SI1CON = (SI1CON & ~(STA1 | STO1 | AA1)) | AA1;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Set I2C1 STA1 STO1 AA1 010
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_Set_STA1_STO1_AA1_010();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_Set_STA1_STO1_AA1_010()\
    MWT(\
        __DRV_SFR_PageIndex(SI1CON_Page);\
        SI1CON = (SI1CON & ~(STA1 | STO1 | AA1)) | STO1;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Set I2C1 STA1 STO1 AA1 100
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_Set_STA1_STO1_AA1_100();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_Set_STA1_STO1_AA1_100()\
    MWT(\
        __DRV_SFR_PageIndex(SI1CON_Page);\
        SI1CON = (SI1CON & ~(STA1 | STO1 | AA1)) | STA1;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Set I2C1 STA1 STO1 AA1 101
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_Set_STA1_STO1_AA1_101();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_Set_STA1_STO1_AA1_101()\
    MWT(\
        __DRV_SFR_PageIndex(SI1CON_Page);\
        SI1CON = (SI1CON & ~(STA1 | STO1 | AA1)) | (STA1 | AA1);\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Set I2C1 STA1 STO1 AA1 110
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_Set_STA1_STO1_AA1_110();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_Set_STA1_STO1_AA1_110()\
    MWT(\
        __DRV_SFR_PageIndex(SI1CON_Page);\
        SI1CON = (SI1CON & ~(STA1 | STO1 | AA1)) | (STA1 | STO1);\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Enable I2C1 GC1 Function
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_GC1_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_GC1_Enable()\
    MWT(\
        __DRV_SFR_PageIndex(SI1ADR_Page);\
        SI1ADR |= GC1;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Disable I2C1 GC1 Function
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_GC1_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_GC1_Disable()\
    MWT(\
        __DRV_SFR_PageIndex(SI1ADR_Page);\
        SI1ADR &= ~GC1;\
        __DRV_SFR_PageIndex(0);\
    )



/**
 *******************************************************************************
 * @brief       Enable/Disable I2C1 GC1 Function
 * @details
 * @param[in]   __STATE__ : config I2C1 GC1 functiont control bit
 *  @arg\b      MW_DISABLE : Set I2C1 GC1 functiont disable (default)
 *  @arg\b      MW_ENABLE : Set I2C1 GC1 functiont enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_GC1_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_GC1_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(SI1ADR_Page);\
        __STATE__==MW_ENABLE?(SI1ADR |= GC1):(SI1ADR &= ~GC1);\
        __DRV_SFR_PageIndex(0);\
    )







/**
 *******************************************************************************
 * @brief       Set I2C1 slave device address 
 * @details
 * @param[in]   __SELECT__ : Programmable I2C1 slave address
 *  @arg\b      0x00~0xFF : Set the slave device address to SI1ADR (even)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_SetSlaveAddress(0x00~0x7F);
 * @endcode
 *******************************************************************************
*/
#define __DRV_I2C1_SetSlaveAddress(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(SI1ADR_Page);\
        SI1ADR &= GC1;\
        SI1ADR |= (__SELECT__<<1)&~GC1;\
        __DRV_SFR_PageIndex(0);\
    )
    


/**
 *******************************************************************************
 * @brief       Set I2C1 slave device address write
 * @details
 * @param[in]   __SELECT__ : Programmable I2C1 slave address write
 *  @arg\b      0x00~0xFF : Set the slave device address to SI1DAT (even)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_SetAddressWrite(0x00~0x7F);
 * @endcode
 *******************************************************************************
*/
#define __DRV_I2C1_SetAddressWrite(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(SI1DAT_Page);\
        SI1DAT = (__SELECT__<<1)&0xFE;\
        __DRV_SFR_PageIndex(0);\
    )



/**
 *******************************************************************************
 * @brief       Set I2C1 slave device address read
 * @details
 * @param[in]   __SELECT__ : Programmable I2C1 slave address read
 *  @arg\b      0x00~0xFF : Set the slave device address to SI1DAT (even)
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_SetAddressRead(0x00~0x7F);
 * @endcode
 *******************************************************************************
*/
#define __DRV_I2C1_SetAddressRead(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(SI1DAT_Page);\
        SI1DAT = (__SELECT__<<1)|0x01;\
        __DRV_SFR_PageIndex(0);\
    )



/**
 *******************************************************************************
 * @brief       Set I2C1 data
 * @details
 * @param[in]   __SELECT__ : Programmable I2C1 buffer data
 *  @arg\b      0x00~0xFF : Set the data to SI1DAT 
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_SetBufData(0x00~0xFF);
 * @endcode
 *******************************************************************************
*/
#define __DRV_I2C1_SetBufData(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(SI1DAT_Page);\
        SI1DAT = __SELECT__;\
        __DRV_SFR_PageIndex(0);\
    )






/**
 *******************************************************************************
 * @brief       Check I2C1 Bus Idle State
 * @details 
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_State_BusIdle();
 * @endcode
 *******************************************************************************
*/
#define __DRV_I2C1_State_BusIdle()\
    MWT(\
        __DRV_SFR_PageIndex(SI1STA_Page);\
        while(SI1STA != 0xF8);\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Select I2C1 clock rate divide from SYSCLK
 * @details
 * @param[in]   __SELECT__ : Programmable I2C1 clock rate selection.
 *  @arg\b      I2C1_SCL_SYSCLK_DIV_8 : I2C1_SCL = SYSCLK / 8 (Default)
 *  @arg\b      I2C1_SCL_SYSCLK_DIV_16 : I2C1_SCL = SYSCLK / 16
 *  @arg\b      I2C1_SCL_SYSCLK_DIV_32 : I2C1_SCL = SYSCLK / 32
 *  @arg\b      I2C1_SCL_SYSCLK_DIV_64 : I2C1_SCL = SYSCLK / 64
 *  @arg\b      I2C1_SCL_SYSCLK_DIV_128 : I2C1_SCL = SYSCLK / 128
 *  @arg\b      I2C1_SCL_SYSCLK_DIV_256 : I2C1_SCL = SYSCLK / 256
 *  @arg\b      I2C1_SCL_S0TOF_DIV_6 : I2C1_SCL = S0TOF / 6
 *  @arg\b      I2C1_SCL_T0OF_DIV_6 : I2C1_SCL = T0OF / 6
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_ClockRate_Select(I2C1_SCL_SYSCLK_DIV_16);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_I2C1_ClockRate_Select
#define I2C1_SCL_SYSCLK_DIV_8          0x00                        //I2C1_SCL = SYSCLK / 8 (Default)
#define I2C1_SCL_SYSCLK_DIV_16         CR01                         //I2C1_SCL = SYSCLK / 16
#define I2C1_SCL_SYSCLK_DIV_32         CR11                         //I2C1_SCL = SYSCLK / 32
#define I2C1_SCL_SYSCLK_DIV_64         (CR11 | CR01)                 //I2C1_SCL = SYSCLK / 64
#define I2C1_SCL_SYSCLK_DIV_128        CR21                         //I2C1_SCL = SYSCLK / 128
#define I2C1_SCL_SYSCLK_DIV_256        (CR21 | CR01)                 //I2C1_SCL = SYSCLK / 256
#define I2C1_SCL_S0TOF_DIV_6           (CR21 | CR11)                 //I2C1_SCL = S0TOF / 6
#define I2C1_SCL_T0OF_DIV_6            (CR21 | CR11 | CR01)           //I2C1_SCL = T0OF / 6
/// @endcond
#define __DRV_I2C1_ClockRate_Select(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(SI1CON_Page);\
        SI1CON = (SI1CON & ~I2C1_SCL_T0OF_DIV_6) | __SELECT__;\
        __DRV_SFR_PageIndex(0);\
    )




/**
 *******************************************************************************
 * @brief       Set I2C1 input pin source
 * @details
 * @param[in]   __SELECT__ : Set external I2C1 input pin source
 *  @arg\b      I2C1_INPUT_PIN_SELECT_P42_P43 : Set I2C1 input pin source to SCL=P42, SDA=P43 (Default)
 *  @arg\b      I2C1_INPUT_PIN_SELECT_P60_P61 : Set I2C1 input pin source to SCL=P60, SDA=P61
 *  @arg\b      I2C1_INPUT_PIN_SELECT_P65_P66 : Set I2C1 input pin source to SCL=P65, SDA=P66
 *  @arg\b      I2C1_INPUT_PIN_SELECT_P63_P62 : Set I2C1 input pin source to SCL=P63, SDA=P62
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_PinMux_Select(I2C1_INPUT_PIN_SELECT_P65_P66);
 * @endcode
 *******************************************************************************
 */
/// @cond __DRV_I2C1_PinMux_Select
#define I2C1_INPUT_PIN_SELECT_P42_P43     0x00
#define I2C1_INPUT_PIN_SELECT_P60_P61     I2C1PS0
#define I2C1_INPUT_PIN_SELECT_P65_P66     I2C1PS1
#define I2C1_INPUT_PIN_SELECT_P63_P62     (I2C1PS1 | I2C1PS0)
#define I2C1_SCL_P42_SDA_P43              0x00
#define I2C1_SCL_P60_SDA_P61              I2C1PS0
#define I2C1_SCL_P65_SDA_P66              I2C1PS1
#define I2C1_SCL_P63_SDA_P62              (I2C1PS1 | I2C1PS0)
/// @endcond
#define __DRV_I2C1_PinMux_Select(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR11_Page);\
        AUXR11 = (AUXR11 & ~I2C1_SCL_P63_SDA_P62) | __SELECT__;\
        __DRV_SFR_PageIndex(0);\
    )



/**
 *******************************************************************************
 * @brief       Enable I2C1 Pre-Assert Acknowledge for DMA Application
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_PAA1_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_PAA1_Enable()\
    MWT(\
        __DRV_SFR_PageIndex(AUXR11_Page);\
        AUXR11 |= PAA1;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Disable I2C1 Pre-Assert Acknowledge for DMA Application
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_PAA1_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_PAA1_Disable()\
    MWT(\
        __DRV_SFR_PageIndex(AUXR11_Page);\
        AUXR11 &= ~PAA1;\
        __DRV_SFR_PageIndex(0);\
    )



/**
 *******************************************************************************
 * @brief       Enable/Disable I2C1 Pre-Assert Acknowledge for DMA Application
 * @details
 * @param[in]   __STATE__ : config I2C1 Pre-Assert Acknowledge control bit
 *  @arg\b      MW_DISABLE : Set I2C1 Pre-Assert Acknowledge disable
 *  @arg\b      MW_ENABLE : Set I2C1 Pre-Assert Acknowledge enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_PAA1_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_PAA1_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR11_Page);\
        __STATE__==MW_ENABLE?(AUXR11 |= PAA1):(AUXR11 &= ~PAA1);\
        __DRV_SFR_PageIndex(0);\
    )



/**
 *******************************************************************************
 * @brief       Enable I2C1 I2C1CF function
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_I2C1CF_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_I2C1CF_Enable()\
    MWT(\
        __DRV_SFR_PageIndex(AUXR11_Page);\
        AUXR11 |= I2C1CF;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Disable I2C1 I2C1CF function
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_I2C1CF_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_I2C1CF_Disable()\
    MWT(\
        __DRV_SFR_PageIndex(AUXR11_Page);\
        AUXR11 &= ~I2C1CF;\
        __DRV_SFR_PageIndex(0);\
    )



/**
 *******************************************************************************
 * @brief       Enable/Disable I2C1 I2C1CF function
 * @details
 * @param[in]   __STATE__ : config I2C1 I2C1CF function control bit
 *  @arg\b      MW_DISABLE : Set I2C1 I2C1CF function disable
 *  @arg\b      MW_ENABLE : Set I2C1 I2C1CF function enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_I2C1CF_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_I2C1CF_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR11_Page);\
        __STATE__==MW_ENABLE?(AUXR11 |= I2C1CF):(AUXR11 &= ~I2C1CF);\
        __DRV_SFR_PageIndex(0);\
    )



/**
 *******************************************************************************
 * @brief       Enable I2C1 Second Address 
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_SecondAddress_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_SecondAddress_Enable()\
    MWT(\
        __DRV_SFR_PageIndex(SI1A2_Page);\
        SI1A2 |= A2E1;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Disable I2C1 Second Address
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_SecondAddress_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_SecondAddress_Disable()\
    MWT(\
        __DRV_SFR_PageIndex(SI1A2_Page);\
        SI1A2 &= ~A2E1;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Enable/Disable I2C1 Second Address
 * @details
 * @param[in]   __STATE__ : config I2C1 Second Address
 *  @arg\b      MW_DISABLE : Set I2C1 Second Address disable (Default)
 *  @arg\b      MW_ENABLE : Set I2C1 Second Address enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_SecondAddress_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_I2C1_SecondAddress_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(SI1A2_Page);\
        __STATE__==MW_ENABLE?(SI1A2 |= A2E1):(SI1A2 &= ~A2E1);\
        __DRV_SFR_PageIndex(0);\
    )




/**
 *******************************************************************************
 * @brief       Set I2C1 Second Address
 * @details
 * @param[in]   __SELECT__ : Programmable I2C1 second address
 *  @arg\b      0x00~0xFF : Set the data to SI1A2 
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_SetSecondAddress(0x00~0x7F);
 * @endcode
 *******************************************************************************
*/
#define __DRV_I2C1_SetSecondAddress(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(SI1A2_Page);\
        SI1A2 |= (__SELECT__<<1)&~A2E1;\
        __DRV_SFR_PageIndex(0);\
    )
    
    
    
/**
 *******************************************************************************
 * @brief       Set I2C1 Second Address Bit Mask
 * @details
 * @param[in]   __SELECT__ : Programmable I2C1 second address bit mask
 *  @arg\b      0x00~0xFF : Set the data to SI1A2M 
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_I2C1_SetSecondAddressMask(0x00~0x7F);
 * @endcode
 *******************************************************************************
*/
#define __DRV_I2C1_SetSecondAddressMask(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(SI1A2M_Page);\
        SI1A2M |= (__SELECT__<<1)&~A2E1;\
        __DRV_SFR_PageIndex(0);\
    )


bool DRV_I2C1_GetEventFlag(void);
uint8_t DRV_I2C1_GetEventCode(void);
uint8_t DRV_I2C1_GetBufData(void);

void Sample_I2C1_ISR_Handle(void);
void Sample_I2C1_Master_Byte_Write(uint8_t I2C1_Address, uint8_t I2C1_Data);
uint8_t Sample_I2C1_Master_Byte_Read(uint8_t I2C1_Address);


#endif  //_MG82F6D64_I2C1_DRV_H
