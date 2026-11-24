/**
 ******************************************************************************
 *
 * @file        MG82F6D64_DMA_DRV.H
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

 
#ifndef _MG82F6D64_DMA_DRV_H
#define _MG82F6D64_DMA_DRV_H



/**
 *******************************************************************************
 * @brief       Enable DMA Hardware
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_DMA_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_DMA_Enable()        DMACR0 |= DMAE0


/**
 *******************************************************************************
 * @brief       Disable DMA Hardware
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_DMA_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_DMA_Disable()        DMACR0 &= ~DMAE0



/**
 *******************************************************************************
 * @brief       Enable/Disable DMA Hardware
 * @details
 * @param[in]   __STATE__ : config DMA hardware control bit
 *  @arg\b      MW_DISABLE : Set DMA hardware disable (default)
 *  @arg\b      MW_ENABLE : Set DMA hardware enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_DMA_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_DMA_Cmd(__STATE__)\
    MWT(\
        __STATE__==MW_ENABLE?(DMACR0 |= DMAE0):(DMACR0 &= ~DMAE0);\
    )


/**
 *******************************************************************************
 * @brief       Enable DMA Hardware Interrupt
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_DMA_IT_Enable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_DMA_IT_Enable()        EDMA = MW_ENABLE



/**
 *******************************************************************************
 * @brief       Disable DMA Hardware Interrupt
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_DMA_IT_Disable();
 * @endcode
 *******************************************************************************
 */
#define __DRV_DMA_IT_Disable()        EDMA = MW_DISABLE



/**
 *******************************************************************************
 * @brief       Enable/Disable DMA Hardware Interrupt
 * @details
 * @param[in]   __STATE__ : config DMA hardware interrupt control bit
 *  @arg\b      MW_DISABLE : Set DMA hardware interrupt disable (default)
 *  @arg\b      MW_ENABLE : Set DMA hardware interrupt enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_DMA_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_DMA_IT_Cmd(__STATE__)        EDMA =__STATE__





/**
 *******************************************************************************
 * @brief       Enable/Disable DMA DCF0 Interrupt
 * @details
 * @param[in]   __STATE__ : config DMA DCF0 hardware interrupt control bit
 *  @arg\b      MW_DISABLE : Set DMA DCF0 hardware interrupt disable (default)
 *  @arg\b      MW_ENABLE : Set DMA DCF0 hardware interrupt enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_DMA_DCF0_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_DMA_DCF0_IT_Cmd(__STATE__)\
    MWT(\
        __STATE__==MW_ENABLE?(DMACR0 |= DIE0):(DMACR0 &= ~DIE0);\
    )




/**
 *******************************************************************************
 * @brief       Enable/Disable DMA TF5 Interrupt
 * @details
 * @param[in]   __STATE__ : config DMA TF5 hardware interrupt control bit
 *  @arg\b      MW_DISABLE : Set DMA TF5 hardware interrupt disable (default)
 *  @arg\b      MW_ENABLE : Set DMA TF5 hardware interrupt enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_DMA_TF5_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_DMA_TF5_IT_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(T5CON_Page);\
        T5IE =__STATE__;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Enable/Disable DMA TF6 Interrupt
 * @details
 * @param[in]   __STATE__ : config DMA TF6 hardware interrupt control bit
 *  @arg\b      MW_DISABLE : Set DMA TF6 hardware interrupt disable (default)
 *  @arg\b      MW_ENABLE : Set DMA TF6 hardware interrupt enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_DMA_TF6_IT_Cmd(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_DMA_TF6_IT_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(T6CON_Page);\
        T6IE =__STATE__;\
        __DRV_SFR_PageIndex(0);\
    )



/**
 *******************************************************************************
 * @brief       Set DMA Transfer Start
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_DMA_SetTransferStart();
 * @endcode
 *******************************************************************************
 */
#define __DRV_DMA_SetTransferStart()        DMACR0 |= DMAS0


/**
 *******************************************************************************
 * @brief       Set DMA Transfer Suspend
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_DMA_SetTransferSuspend();
 * @endcode
 *******************************************************************************
 */
#define __DRV_DMA_SetTransferSuspend()        DMACR0 &= ~DMAS0


/**
 *******************************************************************************
 * @brief       Get DMA flag status
 * @details
 * @return      return DCF0 status
 * @note
 * @par         Example
 * @code
 *    __DRV_DMA_GetFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_DMA_GetFlag()        DMACR0 & DCF0




/**
 *******************************************************************************
 * @brief       Clear DMA Complete Flag
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_DMA_ClearCompleteFlag();
 * @endcode
 *******************************************************************************
 */
#define __DRV_DMA_ClearCompleteFlag()        DMACR0 &= ~DCF0




/**
 *******************************************************************************
 * @brief       Polling for DMA transfer complete.
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_DMA_PollForTransferComplete();
 * @endcode
 *******************************************************************************
 */
#define   __DRV_DMA_PollForTransferComplete()\
    MWT(\
          while((DMACR0 & DCF0) != DCF0);\
    )



/**
 *******************************************************************************
 * @brief       Select DMA loop mode operation
 * @details
 * @param[in]   __SELECT__ : Programmable DMA loop mode operation
 *  @arg\b      MW_DISABLE:Set DMA loop mode disable
 *  @arg\b      MW_ENABLE:Set DMA loop mode enable
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_DMA_LoopMode_Select(MW_DISABLE);
 * @endcode
 *******************************************************************************
*/
#define __DRV_DMA_LoopMode_Select(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(DMACG0_Page);\
        DMACG0 = (DMACG0 & ~LOOP0) | __SELECT__;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Select DMA data copy to CRC16 
 * @details
 * @param[in]   __SELECT__ : Programmable DMA data is copied to CRC16 concurrnetly
 *  @arg\b      DMA_DataToCRC16_DISABLE: Disable the DMA data is copied to CRC16
 *  @arg\b      DMA_DataToCRC16_ENABLE: Enable the DMA data is copied to CRC16
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_DMA_CRC16_Write_Select(DMA_DataToCRC16_DISABLE);
 * @endcode
 *******************************************************************************
*/
/// @cond __DRV_DMA_CRC16_Write_Select
#define DMA_DataToCRC16_DISABLE       0x00
#define DMA_DataToCRC16_ENABLE        CRCW0
/// @endcond

#define __DRV_DMA_CRC16_Write_Select(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(DMACG0_Page);\
        DMACG0 = (DMACG0 & ~CRCW0) | __SELECT__;\
        __DRV_SFR_PageIndex(0);\
    )

/**
 *******************************************************************************
 * @brief       Enable DMA data copy to CRC16 
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_DMA_CRC16_Write_Enable();
 * @endcode
 *******************************************************************************
*/
#define __DRV_DMA_CRC16_Write_Enable()\
    MWT(\
        __DRV_SFR_PageIndex(DMACG0_Page);\
        DMACG0 |= CRCW0;\
        __DRV_SFR_PageIndex(0);\
    )
    
    
/**
 *******************************************************************************
 * @brief       Disable DMA data copy to CRC16 
 * @details
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_DMA_CRC16_Write_Disable();
 * @endcode
 *******************************************************************************
*/
#define __DRV_DMA_CRC16_Write_Disable()\
    MWT(\
        __DRV_SFR_PageIndex(DMACG0_Page);\
        DMACG0 &= ~CRCW0;\
        __DRV_SFR_PageIndex(0);\
    )




/**
 *******************************************************************************
 * @brief       Select DMA External Trigger Source
 * @details
 * @param[in]   __SELECT__ : Programmable DMA external trigger source
 *  @arg\b      DMA_EXTERNAL_TRIGGER_SOURCE_DISABLED : Set DMA external trigger source to disabled (Default)
 *  @arg\b      DMA_EXTERNAL_TRIGGER_SOURCE_INT2ET : Set DMA external trigger source to INT2ET
 *  @arg\b      DMA_EXTERNAL_TRIGGER_SOURCE_KBIET : Set DMA external trigger source to KBIET
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_DMA_ExternalTriggerSource_Select(DMA_EXTERNAL_TRIGGER_SOURCE_INT2ET);
 * @endcode
 *******************************************************************************
*/
/// @cond __DRV_DMA_ExternalTriggerSource_Select
#define DMA_EXTERNAL_TRIGGER_SOURCE_DISABLED     0x00
#define DMA_EXTERNAL_TRIGGER_SOURCE_INT2ET       EXTS00
#define DMA_EXTERNAL_TRIGGER_SOURCE_KBIET        (EXTS10 | EXTS00)
/// @endcond
#define __DRV_DMA_ExternalTriggerSource_Select(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(DMACG0_Page);\
        DMACG0 = (DMACG0 & ~DMA_EXTERNAL_TRIGGER_SOURCE_KBIET) | __SELECT__;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Select DMA Data Source
 * @details
 * @param[in]   __SELECT__ : Programmable DMA data is copied to CRC16 concurrnetly
 *  @arg\b      DMA_DATA_SOURCE_DISABLED : Set DMA data source to disabled (Default)
 *  @arg\b      DMA_DATA_SOURCE_S0_RX : Set DMA data source to S0 RX
 *  @arg\b      DMA_DATA_SOURCE_S1_RX : Set DMA data source to S1 RX
 *  @arg\b      DMA_DATA_SOURCE_S2_RX : Set DMA data source to S2 RX
 *  @arg\b      DMA_DATA_SOURCE_S3_RX : Set DMA data source to S3 RX 
 *  @arg\b      DMA_DATA_SOURCE_I2C0_RX : Set DMA data source to I2C0 RX
 *  @arg\b      DMA_DATA_SOURCE_I2C1_RX : Set DMA data source to I2C1 RX 
 *  @arg\b      DMA_DATA_SOURCE_SPI0_RX : Set DMA data source to SPI0 RX
 *  @arg\b      DMA_DATA_SOURCE_ADC0 : Set DMA data source to ADC0
 *  @arg\b      DMA_DATA_SOURCE_EMB : Set DMA data source to EMB 
 *  @arg\b      DMA_DATA_SOURCE_XRAM : Set DMA data source to XRAM
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_DMA_Source_Select(DMA_DATA_SOURCE_XRAM);
 * @endcode
 *******************************************************************************
*/
/// @cond __DRV_DMA_Source_Select
#define DMA_DATA_SOURCE_DISABLED     0x00
#define DMA_DATA_SOURCE_S0_RX        DSS00
#define DMA_DATA_SOURCE_S1_RX        DSS10
#define DMA_DATA_SOURCE_S2_RX       (DSS10 | DSS00)
#define DMA_DATA_SOURCE_S3_RX        DSS20
#define DMA_DATA_SOURCE_I2C0_RX     (DSS20 | DSS00)
#define DMA_DATA_SOURCE_I2C1_RX     (DSS20 | DSS10)
#define DMA_DATA_SOURCE_SPI0_RX     (DSS20 | DSS10 | DSS00)
#define DMA_DATA_SOURCE_ADC0        (DSS30 | DSS00)
#define DMA_DATA_SOURCE_EMB         (DSS30 | DSS10 | DSS00)
#define DMA_DATA_SOURCE_XRAM        (DSS30 | DSS20 | DSS10 | DSS00)
/// @endcond
#define __DRV_DMA_Source_Select(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(DMADS0_Page);\
        DMADS0 = (DMADS0 & ~DMA_DATA_SOURCE_XRAM) | __SELECT__;\
        __DRV_SFR_PageIndex(0);\
    )


/**
 *******************************************************************************
 * @brief       Select DMA Data Destination
 * @details
 * @param[in]   __SELECT__ : Programmable DMA data is copied to CRC16 concurrnetly
 *  @arg\b      DMA_DATA_DESTINATION_DISABLED : Set DMA data destination to disabled (Default)
 *  @arg\b      DMA_DATA_DESTINATION_S0_TX : Set DMA data destination to S0 TX
 *  @arg\b      DMA_DATA_DESTINATION_S1_TX : Set DMA data destination to S1 TX
 *  @arg\b      DMA_DATA_DESTINATION_S2_TX : Set DMA data destination to S2 TX
 *  @arg\b      DMA_DATA_DESTINATION_S3_TX : Set DMA data destination to S3 TX 
 *  @arg\b      DMA_DATA_DESTINATION_I2C0_TX : Set DMA data destination to I2C0 TX
 *  @arg\b      DMA_DATA_DESTINATION_I2C1_TX : Set DMA data destination to I2C1 TX 
 *  @arg\b      DMA_DATA_DESTINATION_SPI0_TX : Set DMA data destination to SPI0 TX
 *  @arg\b      DMA_DATA_DESTINATION_EMB : Set DMA data destination to EMB 
 *  @arg\b      DMA_DATA_DESTINATION_CRC : Set DMA data destination to CRC
 *  @arg\b      DMA_DATA_DESTINATION_XRAM : Set DMA data destination to XRAM
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_DMA_Destination_Select(DMA_DATA_DESTINATION_I2C0_TX);
 * @endcode
 *******************************************************************************
*/
/// @cond __DRV_DMA_Destination_Select
#define DMA_DATA_DESTINATION_DISABLED     0x00
#define DMA_DATA_DESTINATION_S0_TX        DDS00
#define DMA_DATA_DESTINATION_S1_TX        DDS10
#define DMA_DATA_DESTINATION_S2_TX       (DDS10 | DDS00)
#define DMA_DATA_DESTINATION_S3_TX        DDS20
#define DMA_DATA_DESTINATION_I2C0_TX     (DDS20 | DDS00)
#define DMA_DATA_DESTINATION_I2C1_TX     (DDS20 | DDS10)
#define DMA_DATA_DESTINATION_SPI0_TX     (DDS20 | DDS10 | DDS00)
#define DMA_DATA_DESTINATION_EMB         (DDS30 | DDS10 | DDS00)
#define DMA_DATA_DESTINATION_CRC         (DDS30 | DDS20 | DDS00)
#define DMA_DATA_DESTINATION_XRAM        (DDS30 | DDS20 | DDS10 | DDS00)
/// @endcond
#define __DRV_DMA_Destination_Select(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(DMADS0_Page);\
        DMADS0 = (DMADS0 & ~DMA_DATA_DESTINATION_XRAM) | __SELECT__;\
        __DRV_SFR_PageIndex(0);\
    )




/**
 *******************************************************************************
 * @brief       Set Timer5 DMA to Data Length Counter 
 * @details
 * @param[in]   __SELECT__ : Programmable DMA data length counter value
 *  @arg\b      0x0000~0xFFFF : Set DMA to XRAM data length counter value
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_DMA_SetDataLength(0x00FF);
 * @endcode
 *******************************************************************************
*/
#define __DRV_DMA_SetDataLength(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(TL5_Page);\
        TL5 = TLR5 = ~LOBYTE(__SELECT__);\
        TH5 = THR5 = ~HIBYTE(__SELECT__);\
        __DRV_SFR_PageIndex(0);\
    )




/**
 *******************************************************************************
 * @brief       Set Timer6 DMA to XRAM Address Pointer Counter 
 * @details
 * @param[in]   __SELECT__ : Programmable DMA to XRAM Address Pointer counter value
 *  @arg\b      0x0000~0x0EFF : Set DMA to XRAM Address Pointer counter value
 * @return      None
 * @note
 * @par         Example
 * @code
 *    __DRV_DMA_SetXRAMAddressPointer(0x00FF);
 * @endcode
 *******************************************************************************
*/

#define __DRV_DMA_SetXRAMAddressPointer(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(TL6_Page);\
        TL6 = TLR6 = LOBYTE(__SELECT__);\
        TH6 = THR6 = HIBYTE(__SELECT__);\
        __DRV_SFR_PageIndex(0);\
    )


bool DRV_DMA_GetFlag(void);
#endif  //_MG82F6D64_DMA_DRV_H
