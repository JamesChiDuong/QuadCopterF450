/**
 ******************************************************************************
 *
 * @file        MG82F6D64_AC1_DRV.h
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

#ifndef _MG82F6D64_AC1_DRV_H
#define _MG82F6D64_AC1_DRV_H

/// @cond Parameter_definition

//!@{
//! defgroup AC1 Negative pin
#define AC_AC1NI 0x00              /*!< NI select P0.6 */
#define AC_AC1_INTVREF 0x08        /*!< NI select Int. Vref 1.4V  */
#define AC_AC1_FROM_ACNI0 0x10     /*!< NI select AC0 Negative Input  */
//!@}

/// @endcond


/**
 *******************************************************************************
 * @brief       Enable AC1 block
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_AC1_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC1_Enable()\
    MWT(\
        __DRV_SFR_PageIndex(AC1CON_Page);\
        AC1CON |= AC1EN;\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief       Disable AC1 block
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_AC1_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC1_Disable()\
    MWT(\
        __DRV_SFR_PageIndex(AC1CON_Page);\
         AC1CON &= ~AC1EN;\
        __DRV_SFR_PageIndex(0);\
       )
       
/**
 *******************************************************************************
 * @brief	    Enable/Disable AC1 Function State
 * @details
 * @param[in] __STATE__ : Configure AC1 Function
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC1_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC1_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(AC1CON_Page);\
        __STATE__==MW_ENABLE?(AC1CON |= AC1EN):(AC1CON &= ~AC1EN);\
        __DRV_SFR_PageIndex(0);\
       )
         
/**
 *******************************************************************************
 * @brief        Clear AC1 interrupt flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_AC1_ClearFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC1_ClearFlag()\
    MWT(\
        __DRV_SFR_PageIndex(AC1CON_Page);\
        AC1CON &= ~AC1F;\
        __DRV_SFR_PageIndex(0);\
       )

/**
 *******************************************************************************
 * @brief	 Configure AC1 power mode.
 * @details
 * @param[in]   __SELECT__ : AC1 power mode.
 *  @arg\b	    AC_NORMAL_POWER : Fast response.
 *	@arg\b	    AC_LOW_POWER : Slow response.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC1_PowerMode_Select(AC_LOW_POWER);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC1_PowerMode_Select(__SELECT__)\
     MWT(\
        __DRV_SFR_PageIndex(AC1CON_Page);\
        __SELECT__==AC_LOW_POWER?(AC1CON |= AC1LP):(AC1CON &= ~AC1LP);\
        __DRV_SFR_PageIndex(0);\ 
        )   
       
/**
 *******************************************************************************
 * @brief	   Enable/Disable AC1 to continue its function during PD mode.
 * @details
 * @param[in]  __STATE__ : Configure AC1 Operation in power down mode.
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC1_OperationInPD(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC1_OperationInPD(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(AC1CON_Page);\
        __STATE__==MW_ENABLE?(AC1CON |= AC1PDX):(AC1CON &= ~AC1PDX);\
        __DRV_SFR_PageIndex(0);\
       )    

/**
 *******************************************************************************
 * @brief	   Enable/Disable AC1 output on P72.
 * @details
 * @param[in]  __STATE__ : Configure AC1 output.
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC1_Output_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC1_Output_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR4_Page);\
        __STATE__==MW_ENABLE?(AUXR4 |= (AC1OE)):(AUXR4 &= ~(AC1OE));\
        __DRV_SFR_PageIndex(0);\
        )  

/**
 *******************************************************************************
 * @brief	   Enable/Disable AC1 output inversion.
 * @details
 * @param[in]  __STATE__ : Configure AC1 output inversion.
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC1_OutputInverse_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC1_OutputInverse_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(AC1CON_Page);\
        __STATE__==MW_ENABLE?(AC1CON |= AC1INV):(AC1CON &= ~AC1INV);\
        __DRV_SFR_PageIndex(0);\
       ) 

/**
 *******************************************************************************
 * @brief	   Set AC1 interrupt trigger type
 * @details
 * @param[in]  __SELECT__ : Set AC1 interrupt trigger.
 *  @arg\b	    AC_NO_TRIGGER : AC1 does not trigger interrupt.
 *	@arg\b	    AC_FALLING_TRIGGER : AC1 trigger interrupt on the negative edge.
 *	@arg\b	    AC_RISING_TRIGGER : AC1 trigger interrupt on the positive edge
 *	@arg\b	    AC_DUAL_TRIGGER : AC1 trigger interrupt on positive and negative edges
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC1_TriggerType_Select(AC_FALLING_TRIGGER);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC1_TriggerType_Select(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(AC1CON_Page);\
        AC1CON &= ~(AC1M1 | AC1M0);\
        AC1CON |= __SELECT__&(AC1M1 | AC1M0);\
        __DRV_SFR_PageIndex(0);\
        )  
		     
/**
 *******************************************************************************
 * @brief	    Select AC1 negative input path
 * @param[in] 	__SELECT__: config negative pin 
 *  @arg\b	    AC_AC1NI : P06
 *  @arg\b      AC_AC1_INTVREF : Internal Vref 1.4V
 *  @arg\b      AC_AC1_FROM_ACNI0 : AC0 Negative Input 
 * @return 	    none
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC1_NegativePinMux_Select(AC_AC1NI);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC1_NegativePinMux_Select(__SELECT__)\
      MWT(\
          __DRV_SFR_PageIndex(AC1MOD_Page);\  
          AC1MOD &= ~(AC1NIS | NVRL1);\
          AC1MOD |= __SELECT__&(AC1NIS | NVRL1);\
          __DRV_SFR_PageIndex(0);\
         )

/**
 *******************************************************************************
 * @brief       Set AC1 filter mode type
 * @details
 * @param[in]   __SELECT__ : Set AC1 filter mode type
 *  @arg\b      AC_FILTER_DISABLE: Disable AC1 filter mode type (default)
 *  @arg\b      AC_FILTER_SYSCLK_X3: Set AC1 filter mode type to SYSCLKx3
 *  @arg\b      AC_FILTER_SYSCLK_DIV6X3: Set AC1 filter mode type to SYSCLK/6x3
 *  @arg\b      AC_FILTER_T3OF_X3: Set AC1 filter mode type to T3OFx3
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_AC1_FilterMode_Select(AC_FILTER_SYSCLK_X3);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC1_FilterMode_Select(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(AC1MOD_Page);\
        AC1MOD &= ~AC1FLT;\
        AC1MOD |= __SELECT__ & AC1FLT;\
        AUXR4 &= ~AC1FLT1;\
        AUXR4 |= HIBYTE(__SELECT__) & AC1FLT1;\
        __DRV_SFR_PageIndex(0);\
       )

/**
 *******************************************************************************
 * @brief	   Enable/Disable  AC1 Hysteresis.
 * @details
 * @param[in]  __STATE__ : Configure AC1 Hysteresis.
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC1_Hysteresis_Cmd(MW_DISABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC1_Hysteresis_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR10_Page);\
        __STATE__==MW_ENABLE?(AUXR10 |= AC1HC0):(AUXR10 &= ~AC1HC0);\
        __DRV_SFR_PageIndex(0);\
       )     

/**
 *******************************************************************************
 * @brief	    MW_ENABLE/MW_DISABLE AC1 interrupts.
 * @details
 * @param[in] __STATE__ : configure AC1 interrupts
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC1_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define   __DRV_AC1_IT_Cmd(__STATE__)\
      (__STATE__==MW_ENABLE?(EIE2 |= EAC1):(EIE2 &= ~(EAC1)));\
            
///@cond
bool DRV_AC1_GetFlag(void);
bool DRV_AC1_GetOutput(void);
///@endcond   

#endif
