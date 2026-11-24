/**
 ******************************************************************************
 *
 * @file        MG82F6D64_ADC_DRV.h
 *
 * @brief       This is the C code format driver head file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     V1.02
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

#ifndef _MG82F6D64_ADC_DRV_H
#define _MG82F6D64_ADC_DRV_H

/// @cond Parameter_definition

//!@{
//! defgroup ADC Channel input
#define AIN0     0x00
#define AIN1     0x01 
#define AIN2     0x00
#define AIN3     0x03
#define AIN4     0x04
#define AIN5     0x05
#define AIN6     0x06
#define AIN7     0x07
#define AIN8     0x20
#define AIN9     0x21
#define AIN10    0x22
#define AIN11    0x23
#define AIN12    0x24
#define AIN13    0x25
#define AIN14    0x26
#define AIN15    0x27
#define INTERNAL_VERF     0x08
#define VSS     0x09
//!@}

//!@{
//! defgroup Conversion Clock
#define ADC_SYSCLK_DIV_1     0x00<<5
#define ADC_SYSCLK_DIV_2     0x01<<5
#define ADC_SYSCLK_DIV_4     0x02<<5
#define ADC_SYSCLK_DIV_8     0x03<<5
#define ADC_SYSCLK_DIV_16    0x04<<5
#define ADC_SYSCLK_DIV_32    0x05<<5
#define ADC_S0TOF_DIV_2      0x06<<5
#define ADC_T2OF_DIV_2       0x07<<5
//!@}

//!@{
//! defgroup ADC conversion data format
#define	  ADC_LEFT_JUSTIFIED  0x00
#define	  ADC_RIGHT_JUSTIFIED  0x10
//!@}

//!@{
//! defgroup ADC start control source select
#define	  ADC_START   0x0         
#define   ADC_TM0OF   0x1          
#define   ADC_FREE_RUN 0x2        
#define   ADC_S0OF    0x3
#define   ADC_KBIET   0x10        
#define   ADC_INT1ET  0x11
#define   ADC_INT2ET  0x12
#define   ADC_INT3ET  0x13
#define   ADC_T2EXES  0x20
#define   ADC_AC0ES   0x21
#define   ADC_T3EXES  0x22
#define   ADC_AC1ES   0x23
#define   ADC_C0TOF   0x30
#define   ADC_C0CMP6  0x31
#define   ADC_C0MP6_OR_C0MP7 0x32
#define   ADC_BM0FES 0x33
//!@}

//!@{
//! defgroup ADC scan channel select
#define SAIN15       0x8000
#define SAIN14       0x4000
#define SAIN13       0x2000
#define SAIN12       0x1000
#define SAIN11       0x0800
#define SAIN10       0x0400
#define SAIN9        0x0200
#define SAIN8        0x0100
#define SAIN7        0x0080
#define SAIN6        0x0040
#define SAIN5        0x0020
#define SAIN4        0x0010
#define SAIN3        0x0008
#define SAIN2        0x0004
#define SAIN1        0x0002
#define SAIN0        0x0001
//!@}

//!@{
//! defgroup ADC window detect mode select
#define ADC_WD_INSIDE   0x00
#define ADC_WD_OUTSIDE  0x40
//!@}

//!@{
//! defgroup ADC Resolution select
#define ADC_8BIT   0x00
#define ADC_10BIT  0x01
#define ADC_12BIT  0x02
//!@}

//!@{
//! defgroup ADC Data Order transfer by DMA
#define H_FIRST   0x00
#define L_FIRST   0x01
//!@}

//!@{
//! defgroup ADC Data Byte transfer by DMA.
#define DRV_1BYTE 0x00
#define DRV_2BYTE 0x01
//!@}
/// @endcond


/**
 *******************************************************************************
 * @brief       Enable ADC block
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_ADC_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_ADC_Enable()     ADCON0 |= ADCEN

/**
 *******************************************************************************
 * @brief       Disable ADC block
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_ADC_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_ADC_Disable()    ADCON0 &= ~ADCEN

/**
 *******************************************************************************
 * @brief       ADC Start of conversion
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_ADC_Start();
 * @endcode
 *******************************************************************************
 */
#define __DRV_ADC_Start()      ADCON0 |= ADCS		

/**
 *******************************************************************************
 * @brief        Clear ADC interrupt flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_ADC_ClearFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_ADC_ClearFlag()  ADCON0 &= ~ADCI

/**
 *******************************************************************************
 * @brief        Clear ADC window compare interrupt flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_ADC_ClearWindowFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_ADC_ClearWindowFlag()  ADCON0 &= ~ADCWI

/**
 *******************************************************************************
 * @brief        Clear ADC channel sample & hold flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_ADC_ClearSampleFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_ADC_ClearSampleFlag()  ADCFG0 &= ~SMPF

/**
 *******************************************************************************
 * @brief       configure ADC input.
 * @details
 * @param[in]   __INPUTDEF__ : ADC Channel input pin.
 *  @arg\b	    AIN0 : select P10
 *	@arg\b	    AIN1 : select P11
 *	@arg\b	    AIN2 : select P12
 *	@arg\b	    AIN3 : select P13
 *	@arg\b	    AIN4 : select P14
 *	@arg\b	    AIN5 : select P15
 *	@arg\b	    AIN6 : select P16
 *	@arg\b	    AIN7 : select P17
 *  @arg\b	    AIN8 : select P20
 *	@arg\b	    AIN9 : select P21
 *	@arg\b	    AIN10 : select P52
 *	@arg\b	    AIN11 : select P53
 *	@arg\b	    AIN12 : select P54
 *	@arg\b	    AIN13 : select P55
 *	@arg\b	    AIN14 : select P56
 *	@arg\b	    AIN15 : select P51
 *  @arg\b      INTERNAL_VERF : select Internal Vref 1.4V
 *  @arg\b      VSS : select Internal GND
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_ADC_PinMux_Select(AIN0);
 * @endcode
 *******************************************************************************
 */
#define   __DRV_ADC_PinMux_Select(__INPUTDEF__)\
      MWT(\
          ADCON0 &= ~(CHS3 | CHS2 | CHS1 | CHS0);\
          ADCFG0 &= ~ACHS;\
          ADCON0 |= __INPUTDEF__ & (CHS3 | CHS2 | CHS1 | CHS0);\
          ADCFG0 |= __INPUTDEF__ & ACHS;\
		  )
/**
 *******************************************************************************
 * @brief       Polling for regular conversion complete.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_ADC_PollForConversion();
 * @endcode
 *******************************************************************************
 */
#define   __DRV_ADC_PollForConversion()\
          while((ADCON0 & ADCI) != ADCI)\

/**
 *******************************************************************************
 * @brief       configure ADC Conversion Clock.
 * @details         
 * @param[in]   __CLOCKDEF__ :  specifies the ADC Conversion Clock.
 *  @arg\b      ADC_SYSCLK_DIV_1: ADC clock source by system clock with divider 1  
 *  @arg\b      ADC_SYSCLK_DIV_2: ADC clock source by system clock with divider 2   
 *  @arg\b      ADC_SYSCLK_DIV_4: ADC clock source by system clock with divider 4 
 *  @arg\b      ADC_SYSCLK_DIV_8: ADC clock source by system clock with divider 8  
 *  @arg\b      ADC_SYSCLK_DIV_16: ADC clock source by system clock with divider 16 
 *  @arg\b      ADC_SYSCLK_DIV_32: ADC clock source by system clock with divider 32   
 *  @arg\b      ADC_S0TOF_DIV_2: ADC clock source by S0 overflow with divider 2 
 *  @arg\b      ADC_T2OF_DIV_2: ADC clock source by Timer2 overflow with divider 2 
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_ADC_ClockSource_Select(ADC_SYSCLK_DIV_8);
 * @endcode
 *******************************************************************************
 */	
#define   __DRV_ADC_ClockSource_Select(__CLOCKDEF__)\
     MWT(\
          ADCFG0 &= 0x1F;\
          ADCFG0 |= __CLOCKDEF__;\
         )  
          
/**
 *******************************************************************************
 * @brief	    configure ADC conversion data Alignment mode
 * @details
 * @param[in] 	__ALIGNMODE__ : data alignment mode select (Right/Left)
 *	@arg\b	    ADC_LEFT_JUSTIFIED : ADC result Left-Justified
 *  @arg\b	    ADC_RIGHT_JUSTIFIED : ADC result Right-Justified
 * @return 	    None
 * @note
 * @par         Example
 * @code
 *    __DRV_ADC_DataAlignment_Select(ADC_RIGHT_JUSTIFIED);
 * @endcode
 *******************************************************************************
 */
#define   __DRV_ADC_DataAlignment_Select(__ALIGNMODE__)\
          __ALIGNMODE__==ADC_RIGHT_JUSTIFIED?(ADCFG0 |= ADRJ):(ADCFG0 &= ~ADRJ);\


/**
 *******************************************************************************
 * @brief	    MW_ENABLE/MW_DISABLE ADC Function State
 * @details
 * @param[in] __STATE__ : configure ADC Function
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_ADC_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define   __DRV_ADC_Cmd(__STATE__)\
          __STATE__==MW_ENABLE?(ADCON0 |= ADCEN):(ADCON0 &= ~ADCEN)\
				
/**
 *******************************************************************************
 * @brief	    configure ADC trigger conversion source
 * @details
 * @param[in] 	__ADCTRGSEL__ : select conversion trigger source
 *	@arg\b	    ADC_START : ADC trigger source by software (ADCS)       
 *	@arg\b	    ADC_TM0OF : ADC trigger source by Timer0 overflow        
 *	@arg\b	    ADC_FREE_RUN : ADC free run        
 *	@arg\b	    ADC_S0OF : ADC trigger source by S0BRG overflow          
 *	@arg\b	    ADC_KBIET : ADC trigger source by KBI        
 *	@arg\b	    ADC_INT1ET : ADC trigger source by INT1
 *	@arg\b	    ADC_INT2ET : ADC trigger source by INT2
 *	@arg\b	    ADC_INT3ET : ADC trigger source by INT3
 *	@arg\b	    ADC_T2EXES : ADC trigger source by T2EXES
 *	@arg\b	    ADC_AC0ES : ADC trigger source by AC0ES 
 *	@arg\b	    ADC_T3EXES : ADC trigger source by T3EXES
 *	@arg\b	    ADC_AC1ES : ADC trigger source by AC1ES  
 *	@arg\b	    ADC_C0TOF : ADC trigger source by PCA0 overflow
 *	@arg\b	    ADC_C0CMP6 : ADC trigger source by PCA0 CH6 compare
 *	@arg\b	    ADC_C0MP6_OR_C0MP7 : ADC trigger source by PCA0 CH6/CH7 compare
 *	@arg\b	    ADC_BM0FES : ADC trigger source by OBM0
 * @return 	    None
 * @note
 * @par         Example 
 * @code
 *    __DRV_ADC_TriggerSource_Select(ADC_START);
 * @endcode
 *******************************************************************************
 */
#define   __DRV_ADC_TriggerSource_Select(__ADCTRGSEL__)\
      MWT(\
          ADCFG0 &= ~(ADTM1 | ADTM0);\
          ADCFG0 |= __ADCTRGSEL__ & (ADTM1 | ADTM0);\
          __DRV_SFR_PageIndex(ADCFG4_Page);\
          ADCFG4 &= ~(ADTM3 | ADTM2);\
          ADCFG4 |= __ADCTRGSEL__ & (ADTM3 | ADTM2);\
          __DRV_SFR_PageIndex(0);\
         ) 
				
/**
 *******************************************************************************
 * @brief	    set Conversion Time
 * @details
 * @param[in] __SAMVAL__ : configure conversion extend time.(0~255)
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_ADC_SetExtendSampling(10);
 * @endcode
 *******************************************************************************
 */
#define   __DRV_ADC_SetExtendSampling(__SAMVAL__)\
      MWT(\
          __DRV_SFR_PageIndex(ADCFG2_Page);\
          ADCFG2 = __SAMVAL__;\
          __DRV_SFR_PageIndex(0);\
         )    

/**
 *******************************************************************************
 * @brief	    get ADC conversion data
 * @details
 * @return 	  ADCDH,ADCDL
 * @note
 * @par         Example 
 * @code
 *    16bitReg(user define) = __DRV_ADC_GetConversionData();
 * @endcode
 *******************************************************************************
 */
#define   __DRV_ADC_GetConversionData() (ADCDH << 8 | ADCDL)

/**
 *******************************************************************************
 * @brief	    MW_ENABLE/MW_DISABLE IVREF Function State
 * @details
 * @param[in] __STATE__ : configure IVREF Function
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_IVREF_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define   __DRV_IVREF_Cmd(__STATE__)\
      MWT(\
          DRV_PageP_Read(PCON3_P);\
          __STATE__==MW_ENABLE?(IFD |= IVREN_P):(IFD &= ~IVREN_P);\
          DRV_PageP_Write(PCON3_P,IFD);\
          )
/**
 *******************************************************************************
 * @brief	    MW_ENABLE/MW_DISABLE channel in scan/loop mode
 * @details
 * @param[in] 	__SCANCHANNELX__ : SAIN0, SAIN1 ... SAIN15
 *  @arg\b	    SAIN0 : select P10
 *	@arg\b	    SAIN1 : select P11
 *	@arg\b	    SAIN2 : select P12
 *	@arg\b	    SAIN3 : select P13
 *	@arg\b	    SAIN4 : select P14
 *	@arg\b	    SAIN5 : select P15
 *	@arg\b	    SAIN6 : select P16
 *	@arg\b	    SAIN7 : select P17
 *  @arg\b	    SAIN8 : select P20
 *	@arg\b	    SAIN9 : select P21
 *	@arg\b	    SAIN10 : select P52
 *	@arg\b	    SAIN11 : select P53
 *	@arg\b	    SAIN12 : select P54
 *	@arg\b	    SAIN13 : select P55
 *	@arg\b	    SAIN14 : select P56
 *	@arg\b	    SAIN15 : select P51 
 * @param[in] 	__STATE__ : configure auto-scan control bit
 *      	      This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	    None
 * @note
 * @par         Example 
 * @code
 *    __DRV_ScanLoopChannel_Config(SAIN0|SAIN2|SAIN7,MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define   __DRV_ScanLoopChannel_Config(__SCANCHANNELX__,__STATE__)\
      MWT(\
          __DRV_SFR_PageIndex(ADCFG5_Page);\
          __STATE__==MW_ENABLE?(ADCFG5 |= LOBYTE(__SCANCHANNELX__)):(ADCFG5 &= ~(LOBYTE(__SCANCHANNELX__)));\
          __DRV_SFR_PageIndex(ADCFG6_Page);\
          __STATE__==MW_ENABLE?(ADCFG6 |= HIBYTE(__SCANCHANNELX__)):(ADCFG6 &= ~(HIBYTE(__SCANCHANNELX__)));\  
          __DRV_SFR_PageIndex(0);\
          )

/**
 *******************************************************************************
 * @brief	    set Window Detect High Boundary value.
 * @details
 * @param[in] __HTHRESHOLD__ : configure High Boundary value 1~4095.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_ADC_SetHigherThreshold(4000);
 * @endcode
 *******************************************************************************
 */
#define   __DRV_ADC_SetHigherThreshold(__HTHRESHOLD__)\
      MWT(\
          __DRV_SFR_PageIndex(ADCFG11_Page);\
          ADCFG11 = LOBYTE( __HTHRESHOLD__ << 4);\
          __DRV_SFR_PageIndex(ADCFG12_Page);\
          ADCFG12 = HIBYTE( __HTHRESHOLD__ << 4);\
          __DRV_SFR_PageIndex(0);\
         )     

/**
 *******************************************************************************
 * @brief	    set Window Detect Low Boundary value.
 * @details
 * @param[in] __LTHRESHOLD__ : configure Low Boundary value 0~4094.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_ADC_SetLowerThreshold(40);
 * @endcode
 *******************************************************************************
 */
#define   __DRV_ADC_SetLowerThreshold(__LTHRESHOLD__)\
      MWT(\
          __DRV_SFR_PageIndex(ADCFG13_Page);\
          ADCFG13 = LOBYTE( __LTHRESHOLD__ << 4);\
          __DRV_SFR_PageIndex(ADCFG14_Page);\
          ADCFG14 = HIBYTE( __LTHRESHOLD__ << 4);\
          __DRV_SFR_PageIndex(0);\
         )     

/**
 *******************************************************************************
 * @brief	    MW_ENABLE/MW_DISABLE ADC window detect interrupt
 * @details
 * @param[in] __STATE__ :  configure Window Detect interrupt
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_ADC_WindowDetect_IT(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define   __DRV_ADC_WindowDetect_IT(__STATE__)\
      MWT(\
          __DRV_ADC_ClearWindowFlag();\
          __DRV_SFR_PageIndex(ADCFG1_Page);\
          (__STATE__==MW_ENABLE?(ADCFG1 |= EADCWI):(ADCFG1 &= ~EADCWI));\
          __DRV_SFR_PageIndex(0);\
          )
    
/**
 *******************************************************************************
 * @brief	    Select ADC window detect mode select
 * @details
 * @param[in] __ADC_WDSEL__ : configure window detect mode select
 *	@arg\b	  ADC_WD_INSIDE         
 *	@arg\b	  ADC_WD_OUTSIDE          
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_ADC_WindowDetectMode_Select(ADC_WD_INSIDE);
 * @endcode
 *******************************************************************************
 */
#define   __DRV_ADC_WindowDetectMode_Select(__ADC_WDSEL__)\
      MWT(\
          __DRV_SFR_PageIndex(ADCFG4_Page);\
          (__ADC_WDSEL__==ADC_WD_INSIDE?(ADCFG4 &= ~ADWM0):(ADCFG4 |= ADWM0));\
          __DRV_SFR_PageIndex(0);\
          )

/**
 *******************************************************************************
 * @brief	    MW_DISABLE ADCI interrupt.
 * @details
 * @param[in] __STATE__ : configure Ignore ADCI flag 
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_ADC_IgnoreADCI_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define   __DRV_ADC_IgnoreADCI_Cmd(__STATE__)\
      MWT(\
          __DRV_SFR_PageIndex(ADCFG1_Page);\
          (__STATE__==MW_ENABLE?(ADCFG1 |= IGADCI):(ADCFG1 &= ~IGADCI));\
          __DRV_SFR_PageIndex(0);\
         )
         
/**
 *******************************************************************************
 * @brief       ADC mode select,Resolution 8bit or 10bit and 12bit
 * @details
 * @param[in]   __RESOLUTIONDATA__ : configure ADC Resolution
 *  @arg\b	    ADC_8BIT        
 *	@arg\b	    ADC_10BIT
 *	@arg\b	    ADC_12BIT 
 * @return      None
 * @note
 * @par         Example 
 * @code
 *    __DRV_ADC_DataResolution_Select(ADC_10BIT);
 * @endcode
 *******************************************************************************
 */
#define   __DRV_ADC_DataResolution_Select(__RESOLUTIONDATA__)\
      MWT(\
          __DRV_SFR_PageIndex(ADCFG3_Page);\
          ADCFG3 &= ~ ( ARES1 | ARES0 );\
          (__RESOLUTIONDATA__==ADC_8BIT?(ADCFG3 |= ARES1):\
          __RESOLUTIONDATA__== ADC_10BIT? (ADCFG3 |= ARES0): (_nop_()));\
          __DRV_SFR_PageIndex(0);\
          )    

/**
 *******************************************************************************
 * @brief	    ADC Data Order transfer by DMA.
 * @details
 * @param[in] __STATE__ : configure ADC Data Order transfer by DMA
 *  @arg\b	  H_FIRST : DMA transfer ADCDH first
 *  @arg\b	  L_FIRST : DMA transfer ADCDL first
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_ADC_DMATransferOrder(H_FIRST);
 * @endcode
 *******************************************************************************
 */
#define   __DRV_ADC_DMATransferOrder(__STATE__)\
      MWT(\
        __DRV_SFR_PageIndex(ADCFG4_Page);\
        (__STATE__==H_FIRST?(ADCFG4 |= DOSD):(ADCFG4 &= ~DOSD));\
        __DRV_SFR_PageIndex(0);\
        )
     
/**
 *******************************************************************************
 * @brief	    ADC Data Byte transfer by DMA.
 * @details
 * @param[in] __DMADATASIZE__ : configure ADC Data Byte transfer 1BYTE or 2BYTE by DMA.
 *  @arg\b	  DRV_1BYTE : DMA transfer 1 Byte Data
 *  @arg\b	  DRV_2BYTE : DMA transfer 2 Byte Data
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_ADC_DMADataSize_Select(DRV_1BYTE);
 * @endcode
 *******************************************************************************
 */
#define   __DRV_ADC_DMADataSize_Select(__DMADATASIZE__)\
      MWT(\
          __DRV_SFR_PageIndex(ADCFG4_Page);\
          (__DMADATASIZE__==DRV_1BYTE?(ADCFG4 |= DBSD):(ADCFG4 &= ~DBSD));\
          __DRV_SFR_PageIndex(0);\
         )
    
/**
 *******************************************************************************
 * @brief	    MW_ENABLE/MW_DISABLE ADC interrupts.
 * @details
 * @param[in] __STATE__ : configure ADC interrupts
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_ADC_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
    #define   __DRV_ADC_IT_Cmd(__STATE__)\
          (__STATE__==MW_ENABLE?(EIE1 |= EADC):(EIE1 &= ~(EADC)));\

/**
 *******************************************************************************
 * @brief	    MW_ENABLE/MW_DISABLE ADC sample flag interrupt
 * @details
 * @param[in] __STATE__ :  configure sample flag interrupt
 *      	    This parameter can be: MW_ENABLE or MW_DISABLE.
 * @return 	  None
 * @note
 * @par         Example 
 * @code
 *    __DRV_ADC_SampleFlag_IT(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define   __DRV_ADC_SampleFlag_IT(__STATE__)\
      MWT(\
          __DRV_ADC_ClearSampleFlag();\
          __DRV_SFR_PageIndex(ADCFG1_Page);\
          (__STATE__==MW_ENABLE?(ADCFG1 |= SMPFIE):(ADCFG1 &= ~SMPFIE));\
          __DRV_SFR_PageIndex(0);\
          )

#endif
