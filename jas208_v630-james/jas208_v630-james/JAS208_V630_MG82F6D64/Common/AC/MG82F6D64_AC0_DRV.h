/**
 ******************************************************************************
 *
 * @file        MG82F6D64_AC0_DRV.h
 *
 * @brief       This is the C code format driver head file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     V1.01
 * @date        2022/06/09
 * @author      Megawin Software Center
 * @copyright   Copyright (c) 2019 Megawin Technology Co., Ltd.
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

#ifndef _MG82F6D64_AC0_DRV_H
#define _MG82F6D64_AC0_DRV_H

/// @cond Parameter_definition

//!@{
//! defgroup AC0 Positive pin
#define AC_AC0PI0 0x00     /*!< PI select P0.2 */
#define AC_AC0PI1 0x01     /*!< PI select P0.3  */
#define AC_AC0PI2 0x02     /*!< PI select P0.4  */
#define AC_AC0PI3 0x03     /*!< PI select P0.5  */
//!@}

//!@{
//! defgroup selec AC0 Negative pin
#define AC_AC0NI 0x00  /*!< NI select P0.1 */
#define AC_9_OF_32VREF 0x10  /*!< 9/32 of Vdd voltage (R-ladder) */
#define AC_10_OF_32VREF 0x20 /*!< 10/32 of Vdd voltage (R-ladder) */
#define AC_11_OF_32VREF 0x30 /*!< 11/32 of Vdd voltage (R-ladder) */
#define AC_12_OF_32VREF 0x40 /*!< 12/32 of Vdd voltage (R-ladder) */
#define AC_13_OF_32VREF 0x50 /*!< 13/32 of Vdd voltage (R-ladder) */
#define AC_14_OF_32VREF 0x60 /*!< 14/32 of Vdd voltage (R-ladder) */
#define AC_15_OF_32VREF 0x70 /*!< 15/32 of Vdd voltage (R-ladder) */
#define AC_16_OF_32VREF 0x80 /*!< 16/32 of Vdd voltage (R-ladder) */
#define AC_17_OF_32VREF 0x90 /*!< 17/32 of Vdd voltage (R-ladder) */
#define AC_18_OF_32VREF 0xA0 /*!< 18/32 of Vdd voltage (R-ladder) */
#define AC_19_OF_32VREF 0xB0 /*!< 19/32 of Vdd voltage (R-ladder) */
#define AC_20_OF_32VREF 0xC0 /*!< 20/32 of Vdd voltage (R-ladder) */
#define AC_21_OF_32VREF 0xD0 /*!< 21/32 of Vdd voltage (R-ladder) */
#define AC_22_OF_32VREF 0xE0 /*!< 22/32 of Vdd voltage (R-ladder) */
#define AC_23_OF_32VREF 0xF0 /*!< 23/32 of Vdd voltage (R-ladder) */
 
#define AC_AC0_INTVREF 0x08  /*!< NI select Int. Vref 1.4V  */
#define AC_1_OF_24VREF 0x18  /*!< 1/24 of Vdd voltage (R-ladder) */
#define AC_2_OF_24VREF 0x28  /*!< 2/24 of Vdd voltage (R-ladder) */
#define AC_3_OF_24VREF 0x38  /*!< 3/24 of Vdd voltage (R-ladder) */
#define AC_4_OF_24VREF 0x48  /*!< 4/24 of Vdd voltage (R-ladder) */
#define AC_5_OF_24VREF 0x58  /*!< 5/24 of Vdd voltage (R-ladder) */
#define AC_6_OF_24VREF 0x68  /*!< 6/24 of Vdd voltage (R-ladder) */
#define AC_7_OF_24VREF 0x78  /*!< 7/24 of Vdd voltage (R-ladder) */
#define AC_8_OF_24VREF 0x88  /*!< 8/24 of Vdd voltage (R-ladder) */
#define AC_9_OF_24VREF 0x98  /*!< 9/24 of Vdd voltage (R-ladder) */
#define AC_10_OF_24VREF 0xA8 /*!< 18/24 of Vdd voltage (R-ladder) */
#define AC_11_OF_24VREF 0xB8 /*!< 11/24 of Vdd voltage (R-ladder) */
#define AC_12_OF_24VREF 0xC8 /*!< 12/24 of Vdd voltage (R-ladder) */
#define AC_13_OF_24VREF 0xD8 /*!< 13/24 of Vdd voltage (R-ladder) */
#define AC_14_OF_24VREF 0xE8 /*!< 14/24 of Vdd voltage (R-ladder) */
#define AC_15_OF_24VREF 0xF8 /*!< 15/24 of Vdd voltage (R-ladder) */
//!@}

//!@{
//! defgroup AC Filter Mode Select
#define AC_FILTER_DISABLE          0x0000
#define AC_FILTER_SYSCLK_X3        0x0004
#define AC_FILTER_SYSCLK_DIV6X3    0x0500
#define AC_FILTER_T3OF_X3          0x0504
//!@}

//!@{
//! defgroup AC Interrupt Mode Select
#define AC_NO_TRIGGER       0x00
#define AC_FALLING_TRIGGER  0x01
#define AC_RISING_TRIGGER   0x02
#define AC_DUAL_TRIGGER     0x03
//!@}

//!@{
//! defgroup Power Mode
#define AC_NORMAL_POWER     0x00
#define AC_LOW_POWER        0x01
//!@}

/// @endcond

    

/**
 *******************************************************************************
 * @brief       Enable AC0 block
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_AC0_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC0_Enable()     AC0CON |= AC0EN

/**
 *******************************************************************************
 * @brief       Disable AC0 block
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_AC0_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC0_Disable()    AC0CON &= ~AC0EN

/**
 *******************************************************************************
 * @brief	    Enable/Disable AC0 Function State
 * @details
 * @param[in] __STATE__ : Configure AC0 Function
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC0_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC0_Cmd(__STATE__)\
        __STATE__==MW_ENABLE?(AC0CON |= AC0EN):(AC0CON &= ~AC0EN)\
         
/**
 *******************************************************************************
 * @brief        Clear AC0 interrupt flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_AC0_ClearFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC0_ClearFlag()  AC0CON &= ~AC0F

/**
 *******************************************************************************
 * @brief	 Configure AC0 power mode.
 * @details
 * @param[in]   __SELECT__ : AC0 power mode.
 *  @arg\b	    AC_NORMAL_POWER : Fast response.
 *	@arg\b	    AC_LOW_POWER : Slow response.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC0_PowerMode_Select(AC_LOW_POWER);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC0_PowerMode_Select(__SELECT__)\
        __SELECT__==AC_LOW_POWER?(AC0CON |= AC0LP):(AC0CON &= ~AC0LP)\
   
/**
 *******************************************************************************
 * @brief	   Enable/Disable  AC0 to continue its function during PD mode.
 * @details
 * @param[in]  __STATE__ : Configure AC0 Operation in power down mode.
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC0_OperationInPD(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC0_OperationInPD(__STATE__)\
        __STATE__==MW_ENABLE?(AC0CON |= AC0PDX):(AC0CON &= ~AC0PDX)\

/**
 *******************************************************************************
 * @brief	   Enable/Disable AC0 output on P00.
 * @details
 * @param[in]  __STATE__ : Configure AC0 output.
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC0_Output_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC0_Output_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR4_Page);\
        __STATE__==MW_ENABLE?(AUXR4 |= (AC0OE)):(AUXR4 &= ~(AC0OE));\
        __DRV_SFR_PageIndex(0);\
        )  

/**
 *******************************************************************************
 * @brief	   Enable/Disable AC0 output inversion.
 * @details
 * @param[in]  __STATE__ : Configure AC0 output inversion.
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC0_OutputInverse_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC0_OutputInverse_Cmd(__STATE__)\
        __STATE__==MW_ENABLE?(AC0CON |= AC0INV):(AC0CON &= ~AC0INV)\

/**
 *******************************************************************************
 * @brief	   Set AC0 interrupt trigger type
 * @details
 * @param[in]  __SELECT__ : Set AC0 interrupt trigger.
 *  @arg\b	    AC_NO_TRIGGER : AC0 does not trigger interrupt.
 *	@arg\b	    AC_FALLING_TRIGGER : AC0 trigger interrupt on the negative edge.
 *	@arg\b	    AC_RISING_TRIGGER : AC0 trigger interrupt on the positive edge
 *	@arg\b	    AC_DUAL_TRIGGER : AC0 trigger interrupt on positive and negative edges
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC0_TriggerType_Select(AC_FALLING_TRIGGER);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC0_TriggerType_Select(__SELECT__)\
    MWT(\
        AC0CON &= ~(AC0M1 | AC0M0);\
        AC0CON |= __SELECT__&(AC0M1 | AC0M0);\
        )  
/**
 *******************************************************************************
 * @brief	    Select AC0 positive input path
 * @param[in] 	__SELECT__: config positive pin 
 *  @arg\b	    AC_AC0PI0 : P02
 *  @arg\b	    AC_AC0PI1 : P03
 *  @arg\b	    AC_AC0PI2 : P04
 *  @arg\b	    AC_AC0PI3 : P05
 * @return 	    none
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC0_PositivePinMux_Select(AC_AC0PI0);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC0_PositivePinMux_Select(__SELECT__)\
      MWT(\
          AC0MOD &= ~(AC0PIS1 | AC0PIS0);\
          AC0MOD |= __SELECT__&(AC0PIS1 | AC0PIS0);\
         )
		     
/**
 *******************************************************************************
 * @brief	    Select AC0 negative input path
 * @param[in] 	__SELECT__: config negative pin 
 *  @arg\b	    AC_AC0NI : P01
 *  @arg\b      AC_9_OF_32VREF : 9/32 of Vdd voltage (R-ladder)
 *  @arg\b      AC_10_OF_32VREF : 10/32 of Vdd voltage (R-ladder)
 *  @arg\b      AC_11_OF_32VREF : 11/32 of Vdd voltage (R-ladder)
 *  @arg\b      AC_12_OF_32VREF : 12/32 of Vdd voltage (R-ladder)
 *  @arg\b      AC_13_OF_32VREF : 13/32 of Vdd voltage (R-ladder)
 *  @arg\b      AC_14_OF_32VREF : 14/32 of Vdd voltage (R-ladder)
 *  @arg\b      AC_15_OF_32VREF : 15/32 of Vdd voltage (R-ladder)
 *  @arg\b      AC_16_OF_32VREF : 16/32 of Vdd voltage (R-ladder)
 *  @arg\b      AC_17_OF_32VREF : 17/32 of Vdd voltage (R-ladder)
 *  @arg\b      AC_18_OF_32VREF : 18/32 of Vdd voltage (R-ladder)
 *  @arg\b      AC_19_OF_32VREF : 19/32 of Vdd voltage (R-ladder)
 *  @arg\b      AC_20_OF_32VREF : 20/32 of Vdd voltage (R-ladder)
 *  @arg\b      AC_21_OF_32VREF : 21/32 of Vdd voltage (R-ladder)
 *  @arg\b      AC_22_OF_32VREF : 22/32 of Vdd voltage (R-ladder)
 *  @arg\b      AC_23_OF_32VREF : 23/32 of Vdd voltage (R-ladder)
 *  @arg\b	    AC_AC0_INTVREF : Internal Vref (1.4V)
 *  @arg\b      AC_1_OF_24VREF : 1/24 of Vdd voltage (R-ladder)
 *  @arg\b      AC_2_OF_24VREF : 2/24 of Vdd voltage (R-ladder)
 *  @arg\b      AC_3_OF_24VREF : 3/24 of Vdd voltage (R-ladder)
 *  @arg\b      AC_4_OF_24VREF : 4/24 of Vdd voltage (R-ladder)
 *  @arg\b      AC_5_OF_24VREF : 5/24 of Vdd voltage (R-ladder)
 *  @arg\b      AC_6_OF_24VREF : 6/24 of Vdd voltage (R-ladder)
 *  @arg\b      AC_7_OF_24VREF : 7/24 of Vdd voltage (R-ladder)
 *  @arg\b      AC_8_OF_24VREF : 8/24 of Vdd voltage (R-ladder)
 *  @arg\b      AC_9_OF_24VREF : 9/24 of Vdd voltage (R-ladder)
 *  @arg\b      AC_10_OF_24VREF : 10/24 of Vdd voltage (R-ladder)
 *  @arg\b      AC_11_OF_24VREF : 11/24 of Vdd voltage (R-ladder)
 *  @arg\b      AC_12_OF_24VREF : 12/24 of Vdd voltage (R-ladder)
 *  @arg\b      AC_13_OF_24VREF : 13/24 of Vdd voltage (R-ladder)
 *  @arg\b      AC_14_OF_24VREF : 14/24 of Vdd voltage (R-ladder)
 *  @arg\b      AC_15_OF_24VREF : 15/24 of Vdd voltage (R-ladder)
 * @return 	    none
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC0_NegativePinMux_Select(AC_AC0NI);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC0_NegativePinMux_Select(__SELECT__)\
      MWT(\
          AC0MOD &= ~(NVRS3 | NVRS2 | NVRS1 | NVRS0 | NVRL);\
          AC0MOD |= __SELECT__&(NVRS3 | NVRS2 | NVRS1 | NVRS0 | NVRL);\
         )

/**
 *******************************************************************************
 * @brief       Set AC0 filter mode type
 * @details
 * @param[in]   __SELECT__ : Set AC0 filter mode type
 *  @arg\b      AC_FILTER_DISABLE: Disable AC0 filter mode type (default)
 *  @arg\b      AC_FILTER_SYSCLK_X3: Set AC0 filter mode type to SYSCLKx3
 *  @arg\b      AC_FILTER_SYSCLK_DIV6X3: Set AC0 filter mode type to SYSCLK/6x3
 *  @arg\b      AC_FILTER_T3OF_X3: Set AC0 filter mode type to T3OFx3
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_AC0_FilterMode_Select(AC_FILTER_SYSCLK_X3);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC0_FilterMode_Select(__SELECT__)\
    MWT(\
        AC0MOD &= ~AC0FLT;\
        AC0MOD |= __SELECT__ & AC0FLT;\
        __DRV_SFR_PageIndex(AUXR4_Page);\
        AUXR4 &= ~AC0FLT1;\
        AUXR4 |= HIBYTE(__SELECT__) & AC0FLT1;\
        __DRV_SFR_PageIndex(0);\
       )

/**
 *******************************************************************************
 * @brief	   Enable/Disable  AC0 Hysteresis.
 * @details
 * @param[in]  __STATE__ : Configure AC0 Hysteresis.
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC0_Hysteresis_Cmd(MW_DISABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC0_Hysteresis_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR10_Page);\
        __STATE__==MW_ENABLE?(AUXR10 |= AC0HC0):(AUXR10 &= ~AC0HC0);\
        __DRV_SFR_PageIndex(0);\
       )

/**
 *******************************************************************************
 * @brief	    MW_ENABLE/MW_DISABLE AC0 interrupts.
 * @details
 * @param[in] __STATE__ : configure AC0 interrupts
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC0_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define   __DRV_AC0_IT_Cmd(__STATE__)\
      (__STATE__==MW_ENABLE?(EIE1 |= EAC0):(EIE1 &= ~(EAC0)));\
            
///@cond
bool DRV_AC0_GetFlag(void);
bool DRV_AC0_GetOutput(void);
///@endcond 

#endif
