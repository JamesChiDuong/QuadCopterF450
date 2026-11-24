/**
 ******************************************************************************
 *
 * @file        MG82F6D64_IAP_DRV.h
 *
 * @brief       This is the C code format driver head file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     V1.01
 * @date        2022/06/09
 * @author      Megawin Software Center
 * @copyright   Copyright (c) 2019 MegaWin Technology Co., Ltd.
 *              All rights reserved.
 *
 ******************************************************************************
 * @par 		Disclaimer
 *		The Demo software is provided "AS IS" without any warranty, either
 *		expressed or implied, including, but not limited to, the implied warranties
 *		of merchantability and fitness for a particular purpose.  The author will
 *		not be liable for any special, incidental, consequential or indirect
 *		damages due to loss of data or any other reason.
 *		These statements agree with the worldwide and local dictated laws about
 *		authorship and violence against these laws.
 ******************************************************************************
 ******************************************************************************
 */


#ifndef _MG82F6D64_IAP_DRV_H
#define _MG82F6D64_IAP_DRV_H
/*--------------------------------------------------------------
IAP Function :
    1.Boundary
        |----Initializing IAP boundary(Only for wizard) : DRV_IAP_Boundary_Wizard_Init();
        
    2.UID
        |----UID reading : __DRV_IAP_ReadUID(__UID__);
        __UID__ : 
            A length 16 uint8_t array to store UID
            
    3.IAP FlashRead
        |----Reading one byte from flash : DRV_IAP_FlashRead(uint8_t MODE, uint16_t ADDR);
        |----Reading one byte from flash without interrupts : DRV_IAP_FlashReadWithoutIT(uint8_t MODE, uint16_t ADDR);
        MODE :                                          ADDR :
            IFMT_IAP_READ/IFMT_IAP_READ_INC_IFADR           specifies the flash address, 16-bit, to be read.
            
    4.IAP ByteProgram
        |----Writing one byte data to flash : DRV_IAP_ByteProgram(uint8_t MODE, uint16_t ADDR, uint8_t DATA);
        |----Writing one byte data to flash without interrupts : DRV_IAP_ByteProgramWithoutIT(uint8_t MODE, uint16_t ADDR, uint8_t DATA);
        MODE :                                          ADDR :                                                      DATA :
            IFMT_IAP_READ/IFMT_IAP_READ_INC_IFADR           Specifies the flash address, 16-bit, to be read.            The data which will be written onto the specified flash address.
            
    5.IAP PageErase
        |----Erasing a full page of flash data : DRV_IAP_PageErase(uint8_t ADDR);
        |----Erasing a full page of flash data without interrupts  : DRV_IAP_PageEraseWithoutIT(uint8_t ADDR);
        ADDR :
            Specifies the page start address which should be even
            
    6.GetStatus(CFAIL)
        |----Checking if ISP/IAP operations fail : DRV_IAP_GetStatus(void);
--------------------------------------------------------------*/
/// @cond Parameter_definition

//!@{
//! defgroup IFMT Commands
//__IFMT Commands
#define IFMT_STANDBY                        0x00
#define IFMT_IAP_READ                       0x01
#define IFMT_IAP_PROGRAM                    0x02
#define IFMT_IAP_PAGE_ERASE                 0x03
#define IFMT_FLASH_READ_CRC                 0x80
#define IFMT_FLASH_READ_INC_IFADR           0x81
#define IFMT_FLASH_PROGRAM_INC_IFADR        0x82
//!@}
/// @endcond

/**
 *******************************************************************************
 * @brief       Checking if ISP/IAP operations fail.
 * @details
 * @return      If it fails, it returns 1; otherwise, it returns 0 if it successes.
 * @note
 * @par Example
 * @code
 *  DRV_IAP_GetStatus();
 * @endcode
 *******************************************************************************
 */
uint8_t DRV_IAP_GetStatus(void);

/**
 *******************************************************************************
 * @brief       Erasing a full page of flash data without interrupts, a full page stands for 512 bytes. This should be executed first before start byte programming. (Caution: This function MUST be operated in the suitable code area.)
 * @details
 * @param[in]   ADDR : specifies the page start address which should be even.
 * @return      If it fails, it returns 1; otherwise, it returns 0 if it successes.
 * @note
 * @par Example
 * @code
 *  DRV_IAP_PageEraseWithoutIT(0x36);
 * @endcode
 *******************************************************************************
 */
uint8_t DRV_IAP_PageEraseWithoutIT(uint8_t ADDR);

/**
 *******************************************************************************
 * @brief       Erasing a full page of flash data, a full page stands for 512 bytes. (Caution: This function MUST be operated in the suitable code area.)
 * @details
 * @param[in]   ADDR : specifies the page start address which should be even.
 * @return      If it fails, it returns 1; otherwise, it returns 0 if it successes.
 * @note
 * @par Example
 * @code
 *  DRV_IAP_PageErase(0x36);
 * @endcode
 * @note
 * @par Example
 *******************************************************************************
 */
uint8_t DRV_IAP_PageErase(uint8_t ADDR);

/**
 *******************************************************************************
 * @brief       Writing one byte data to flash without interrupts. (Caution: This function MUST be operated in the suitable code area.)
 * @details
 * @param[in]   MODE : supports byte program with/without  increasing address.
 *  @arg\b      IFMT_IAP_PROGRAM : writes one byte without increasing address.
 *  @arg\b      IFMT_FLASH_PROGRAM_INC_IFADR : writes one byte with increasing address.
 * @param[in]   ADDR : specifies the flash address, 16-bit, to be written.
 * @param[in]   DATA : the data which will be written onto the specified flash address.
 * @return      If it fails, it returns 1; otherwise, it returns 0 if it successes.
 * @note
 * @par Example
 * @code
 *  DRV_IAP_ByteProgramWithoutIT(IFMT_FLASH_PROGRAM, 0x3600, 0x52);
 * @endcode
 *******************************************************************************
 */
uint8_t DRV_IAP_ByteProgramWithoutIT(uint8_t MODE, uint16_t ADDR, uint8_t DATA);

/**
 *******************************************************************************
 * @brief       Writing one byte data to flash. (Caution: This function MUST be operated in the suitable code area.)
 * @details
 * @param[in]   MODE : supports byte program with/without increasing address.
 *  @arg\b      IFMT_IAP_PROGRAM : writes one byte without increasing address.
 *  @arg\b      IFMT_FLASH_PROGRAM_INC_IFADR : writes one byte with increasing address.
 * @param[in]   ADDR : specifies the flash address, 16-bit, to be written.
 * @param[in]   DATA : the data which will be written onto the specified flash address.
 * @return      If it fails, it returns 1; otherwise, it returns 0 if it successes.
 * @note
 * @par Example
 * @code
 *  DRV_IAP_ByteProgram(IFMT_FLASH_PROGRAM, 0x3600, 0x52);
 * @endcode
 *******************************************************************************
 */
uint8_t DRV_IAP_ByteProgram(uint8_t MODE, uint16_t ADDR, uint8_t DATA);

/**
 *******************************************************************************
 * @brief       Reading one byte from flash without interrupts. (Caution: This function MUST be operated in the suitable code area.)
 * @details
 * @param[in]   MODE : supports flash read with/without increasing address.
 *  @arg\b      IFMT_IAP_READ : reads one byte without increasing address.
 *  @arg\b      IFMT_IAP_READ_INC_IFADR : reads one byte with increasing address.
 * @param[in]   ADDR : specifies the flash address, 16-bit, to be read.
 * @return      Data stored in address ADDR.
 * @note
 * @par Example
 * @code
 *  DRV_IAP_FlashReadWithoutIT(IFMT_IAP_READ, 0x3600);
 * @endcode
 *******************************************************************************
 */
uint8_t DRV_IAP_FlashReadWithoutIT(uint8_t MODE, uint16_t ADDR);

/**
 *******************************************************************************
 * @brief       Reading one byte from flash. (Caution: This function MUST be operated in the suitable code area.)
 * @details
 * @param[in]   MODE : supports flash read with/without increasing address.
 *  @arg\b      IFMT_IAP_READ : reads one byte without increasing address.
 *  @arg\b      IFMT_IAP_READ_INC_IFADR : reads one byte with increasing address.
 * @param[in]   ADDR : specifies the flash address, 16-bit, to be read.
 * @return      Data stored in address ADDR.
 * @note
 * @par Example
 * @code
 *  DRV_IAP_FlashRead(IFMT_IAP_READ, 0x3600);
 * @endcode
 *******************************************************************************
 */
uint8_t DRV_IAP_FlashRead(uint8_t MODE, uint16_t ADDR);


/**
 *******************************************************************************
 * @brief       UID reading.
 * @details
 * @param[in]   __UID__ : a length 16 uint8_t array to store UID.
 * @return      UID.
 * @note   UID is stored in the address IFADRH = 0, IFADRL = 0xF0.
 * @par Example
 * @code
 *   __DRV_IAP_ReadUID(UID);
 * @endcode
 *******************************************************************************
 */
#define __DRV_IAP_ReadUID(__UID__)\
    MWT(\
        uint8_t i;\
        ISPCR |= ISPEN;\
        BOREV = 0x22;\
        IFMT = 0x06;\
        IFADRH = 0x00;\
        IFADRL = 0xF0;\
        for(i = 0;i < 16;i++){\
            SCMD = 0x46;\
            SCMD = 0xB9;\
            __UID__[i] = IFD;\
            IFADRL++;\
        }\
        BOREV = 0;\
        IFMT = 0;\
        ISPCR &= ~ISPEN;\
    )

#endif
