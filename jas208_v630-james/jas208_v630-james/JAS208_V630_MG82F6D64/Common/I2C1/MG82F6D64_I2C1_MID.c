/**
 ******************************************************************************
 *
 * @file        MG82F6D64_I2C1_MID.C
 *
 * @brief       This is the C code format driver source file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     v1.00
 * @date        2020/07/10
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
 * @brief       I2C1 Master Byte Write 
 * @details     
 * @return      No
 * @note        
 * @par         Example
 * @code        
                Sample_I2C1_Master_Byte_Write(uint8_t Address, uint8_t Data);
 * @endcode     
 *******************************************************************************
 */

void Sample_I2C1_Master_Byte_Write(uint8_t I2C1_Address, uint8_t I2C1_Data)
{
    __DRV_I2C1_SetSTART();
    __DRV_I2C1_ClearEventFlag();
    __DRV_I2C1_PollForStateComplete();
    __DRV_I2C1_ClearSTART();
    
    __DRV_I2C1_SetAddressWrite(I2C1_Address);
    __DRV_I2C1_ClearEventFlag();
    __DRV_I2C1_PollForStateComplete();
    
    __DRV_I2C1_SetBufData(I2C1_Data);
    __DRV_I2C1_ClearEventFlag();
    __DRV_I2C1_PollForStateComplete();

    __DRV_I2C1_SetSTOP();
    __DRV_I2C1_ClearEventFlag();
    __DRV_I2C1_PollForStopComplete();
}


/**
 *******************************************************************************
 * @brief       I2C1 Master Byte Read 
 * @details     
 * @return      Data_Temp
 * @note        
 * @par         Example
 * @code        
                Sample_I2C1_Master_Byte_Read(uint8_t Address);
 * @endcode     
 *******************************************************************************
 */

uint8_t Sample_I2C1_Master_Byte_Read(uint8_t I2C1_Address)
{
    uint8_t Data_Temp;

    __DRV_I2C1_SetSTART();
    __DRV_I2C1_ClearEventFlag();
    __DRV_I2C1_PollForStateComplete();
    __DRV_I2C1_ClearSTART();
    
    __DRV_I2C1_SetAddressWrite(I2C1_Address);
    __DRV_I2C1_ClearEventFlag();
    __DRV_I2C1_PollForStateComplete();

    __DRV_I2C1_SetSTART();
    __DRV_I2C1_ClearEventFlag();
    __DRV_I2C1_PollForStateComplete();
    __DRV_I2C1_ClearSTART();

    __DRV_I2C1_SetAddressRead(I2C1_Address);
    __DRV_I2C1_ClearEventFlag();
    __DRV_I2C1_PollForStateComplete();

    __DRV_I2C1_ClearEventFlag();
    __DRV_I2C1_PollForStateComplete();
    Data_Temp = DRV_I2C1_GetBufData();

    __DRV_I2C1_SetSTOP();
    __DRV_I2C1_ClearEventFlag();
    __DRV_I2C1_PollForStopComplete();


    return Data_Temp;
}
