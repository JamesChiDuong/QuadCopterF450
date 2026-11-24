/**
 ******************************************************************************
 *
 * @file        MG82F6D64_AC2_DRV.h
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

#ifndef _MG82F6D64_AC2_DRV_H
#define _MG82F6D64_AC2_DRV_H

/// @cond Parameter_definition

//!@{
//! defgroup AC2 Negative pin
#define AC_AC2NI 0x00               /*!< NI select P6.2 */
#define AC_AC2_INTVREF 0x08         /*!< NI select Int. Vref 1.4V  */
#define AC_AC2_FROM_ACNI0 0x10      /*!< NI select AC0 Negative Input  */
//!@}

/// @endcond


/**
 *******************************************************************************
 * @brief       Enable AC2 block
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_AC2_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC2_Enable()\
    MWT(\
        __DRV_SFR_PageIndex(AC2CON_Page);\
        AC2CON |= AC2EN;\
        __DRV_SFR_PageIndex(0);\
        )

/**
 *******************************************************************************
 * @brief       Disable AC2 block
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_AC2_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC2_Disable()\
    MWT(\
        __DRV_SFR_PageIndex(AC2CON_Page);\
         AC2CON &= ~AC2EN;\
        __DRV_SFR_PageIndex(0);\
       )
/**
 *******************************************************************************
 * @brief	    Enable/Disable AC2 Function State
 * @details
 * @param[in] __STATE__ : Configure AC2 Function
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC2_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC2_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(AC2CON_Page);\
        __STATE__==MW_ENABLE?(AC2CON |= AC2EN):(AC2CON &= ~AC2EN);\
        __DRV_SFR_PageIndex(0);\
       )
         
/**
 *******************************************************************************
 * @brief        Clear AC2 interrupt flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_AC2_ClearFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC2_ClearFlag()\
    MWT(\
        __DRV_SFR_PageIndex(AC2CON_Page);\
        AC2CON &= ~AC2F;\
        __DRV_SFR_PageIndex(0);\
       )

/**
 *******************************************************************************
 * @brief	 Configure AC2 power mode.
 * @details
 * @param[in]   __SELECT__ : AC2 power mode.
 *  @arg\b	    AC_NORMAL_POWER : Fast response.
 *	@arg\b	    AC_LOW_POWER : Slow response.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC2_PowerMode_Select(AC_LOW_POWER);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC2_PowerMode_Select(__SELECT__)\
     MWT(\
        __DRV_SFR_PageIndex(AC2CON_Page);\
        __SELECT__==AC_LOW_POWER?(AC2CON |= AC2LP):(AC2CON &= ~AC2LP);\
        __DRV_SFR_PageIndex(0);\
        )   

/**
 *******************************************************************************
 * @brief	   Enable/Disable AC2 to continue its function during PD mode.
 * @details
 * @param[in]  __STATE__ : Configure AC2 Operation in power down mode.
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC2_OperationInPD(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC2_OperationInPD(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(AC2CON_Page);\
        __STATE__==MW_ENABLE?(AC2CON |= AC2PDX):(AC2CON &= ~AC2PDX);\
        __DRV_SFR_PageIndex(0);\
       )    

/**
 *******************************************************************************
 * @brief	   Enable/Disable AC2 output on P64.
 * @details
 * @param[in]  __STATE__ : Configure AC2 output.
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC2_Output_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC2_Output_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR14_Page);\
        __STATE__==MW_ENABLE?(AUXR14 |= (AC2OE)):(AUXR14 &= ~(AC2OE));\
        __DRV_SFR_PageIndex(0);\
        )  

/**
 *******************************************************************************
 * @brief	   Enable/Disable AC2 output inversion.
 * @details
 * @param[in]  __STATE__ : Configure AC2 output inversion.
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC2_Output_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC2_OutputInverse_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(AC2CON_Page);\
        __STATE__==MW_ENABLE?(AC2CON |= AC2INV):(AC2CON &= ~AC2INV);\
        __DRV_SFR_PageIndex(0);\
       ) 

/**
 *******************************************************************************
 * @brief	   Set AC2 interrupt trigger type
 * @details
 * @param[in]  __SELECT__ : Set AC2 interrupt trigger.
 *  @arg\b	    AC_NO_TRIGGER : AC2 does not trigger interrupt.
 *	@arg\b	    AC_FALLING_TRIGGER : AC2 trigger interrupt on the negative edge.
 *	@arg\b	    AC_RISING_TRIGGER : AC2 trigger interrupt on the positive edge
 *	@arg\b	    AC_DUAL_TRIGGER : AC2 trigger interrupt on positive and negative edges
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC2_TriggerType_Select(AC_FALLING_TRIGGER);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC2_TriggerType_Select(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(AC2CON_Page);\
        AC2CON &= ~(AC2M1 | AC2M0);\
        AC2CON |= __SELECT__&(AC2M1 | AC2M0);\
        __DRV_SFR_PageIndex(0);\
        )  
		     
/**
 *******************************************************************************
 * @brief	    Select AC2 negative input path
 * @param[in] 	__SELECT__: config negative pin 
 *  @arg\b	    AC_AC2NI : P62
 *  @arg\b      AC_AC2_INTVREF : Internal Vref 1.4V
 *  @arg\b      AC_AC2_FROM_ACNI0 : AC0 Negative Input 
 * @return 	    none
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC2_NegativePinMux_Select(AC_AC2NI0);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC2_NegativePinMux_Select(__SELECT__)\
      MWT(\
          __DRV_SFR_PageIndex(AC2MOD_Page);\
          AC2MOD &= ~(AC2NIS | NVRL2);\
          AC2MOD |= __SELECT__&(AC2NIS | NVRL2);\
          __DRV_SFR_PageIndex(0);\
         )

/**
 *******************************************************************************
 * @brief       Set AC2 filter mode type
 * @details
 * @param[in]   __SELECT__ : Set AC2 filter mode type
 *  @arg\b      AC_FILTER_DISABLE: Disable AC2 filter mode type (default)
 *  @arg\b      AC_FILTER_SYSCLK_X3: Set AC2 filter mode type to SYSCLKx3
 *  @arg\b      AC_FILTER_SYSCLK_DIV6X3: Set AC2 filter mode type to SYSCLK/6x3
 *  @arg\b      AC_FILTER_T3OF_X3: Set AC2 filter mode type to T3OFx3
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_AC2_FilterMode_Select(AC_FILTER_SYSCLK_X3);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC2_FilterMode_Select(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(AC2MOD_Page);\
        AC2MOD &= ~AC2FLT;\
        AC2MOD |= __SELECT__ & AC2FLT;\
        __DRV_SFR_PageIndex(AUXR14_Page);\
        AUXR14 &= ~AC2FLT1;\
        AUXR14 |= HIBYTE(__SELECT__) & AC2FLT1;\
        __DRV_SFR_PageIndex(0);\
       )

/**
 *******************************************************************************
 * @brief	   Enable/Disable  AC2 Hysteresis.
 * @details
 * @param[in]  __STATE__ : Configure AC2 Hysteresis.
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC2_Hysteresis_Cmd(MW_DISABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_AC2_Hysteresis_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR14_Page);\
        __STATE__==MW_ENABLE?(AUXR14 |= AC2HC0):(AUXR14 &= ~AC2HC0);\
        __DRV_SFR_PageIndex(0);\
       )     
       
/**
 *******************************************************************************
 * @brief	    MW_ENABLE/MW_DISABLE AC2 interrupts.
 * @details
 * @param[in] __STATE__ : configure AC2 interrupts
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_AC2_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define   __DRV_AC2_IT_Cmd(__STATE__)\
      (__STATE__==MW_ENABLE?(EIE2 |= EAC2):(EIE2 &= ~(EAC2)));\
            
            
///@cond
bool DRV_AC2_GetFlag(void);
bool DRV_AC2_GetOutput(void);
///@endcond 
      
#endif
