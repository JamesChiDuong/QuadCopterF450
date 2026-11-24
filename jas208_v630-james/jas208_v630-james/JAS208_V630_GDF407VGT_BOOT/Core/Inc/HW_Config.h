#ifndef _HW_CONFIG_H_
#define _HW_CONFIG_H_
//============================================================================//


//============================================================================//
// User Includes
//============================================================================//

#include <stdio.h>
#include <stdbool.h>

#include "stm32fxxx_hal_user.h"


//============================================================================//
// User Defines
//============================================================================//

// Define for device type
#define TYPE_JAS106                "JAS106"
#define TYPE_JAS208                "JAS208"
#define TYPE_JAS306                "JAS306"

// Define for HW Type
#define HW_JAS106                  1U
#define HW_JAS208                  2U
#define HW_JAS306                  3U

// Define for HW Version
#define HW_JAS201_V400			       20U 
#define HW_JAS121_V100			       30U  
#define HW_JAS121_V110			       31U 

#define HW_JAS211_V100			       40U            
#define HW_JAS211_V110			       41U // Heinz
#define HW_JAS211_V200			       41U // Heinz

#define HW_JAS106_V100			       50U
#define HW_JAS106_V200			       51U

#define HW_JAS208_V100			       60U
#define HW_JAS208_V200			       61U
#define HW_JAS208_V300			       62U
#define HW_JAS208_V340			       64U
#define HW_JAS208_V360			       66U
#define HW_JAS208_V610			       67U
#define HW_JAS208_V630			       65U
#define HW_JAS208_V700			       63U

#define HW_JAS306_V100             70U

// Define for HW MCU
#define HW_STM32F105VC             1U
#define HW_STM32F205VE             2U
#define HW_STM32F405ZG             3U
#define HW_STM32F407VE             4U
#define HW_STM32F407ZE             5U
#define HW_STM32F407VG             6U
#define HW_STM32F407ZG             7U

// Define for SD card interface
#define SD_SPI                     1U
#define SD_SDIO                    2U

#define SD_INTERFACE               SD_SDIO

// Define size for HW-FLASH
#define FLASH_SIZE_64K			       (0x10000)
#define FLASH_SIZE_128K			       (0x20000)
#define FLASH_SIZE_256K			       (0x40000)
#define FLASH_SIZE_512K			       (0x80000)
#define FLASH_SIZE_1024K		       (0x100000)

// Define size for HW-BOOT
#define BOOT_SIZE_12K			         (0x3000)
#define BOOT_SIZE_16K			         (0x4000)
#define BOOT_SIZE_32K			         (0x8000)
#define BOOT_SIZE_40K			         (0xA000)
#define BOOT_SIZE_48K			         (0xC000)

// Flash Start address
#define FLASH_START_ADDR		       ((uint32_t)0x08000000)

// Defines for the specific model
#define DEVICE_TYPE                TYPE_JAS208

#define HW_TYPE					           HW_JAS208

#define HW_VER					           HW_JAS208_V630

#define HW_MCU			               HW_STM32F407VG

#define BOOT_SIZE				           BOOT_SIZE_48K

#if HW_MCU == HW_STM32F407VG
#define FLASH_SIZE			           FLASH_SIZE_1024K

#define PROGRAM_SIZE               (FLASH_SIZE - BOOT_SIZE)

#define SECTOR_NUMBER              12U

#define SECTOR_16K_SIZE            0x4000

#define SECTOR_64K_SIZE            0x10000

#define SECTOR_128K_SIZE           0x20000

#define FLASH_SECTOR_0_ADDRESS     FLASH_START_ADDR
#define FLASH_SECTOR_0_SIZE        SECTOR_16K_SIZE

#define FLASH_SECTOR_1_ADDRESS     (FLASH_SECTOR_0_ADDRESS + FLASH_SECTOR_0_SIZE)
#define FLASH_SECTOR_1_SIZE        SECTOR_16K_SIZE

#define FLASH_SECTOR_2_ADDRESS     (FLASH_SECTOR_1_ADDRESS + FLASH_SECTOR_1_SIZE)
#define FLASH_SECTOR_2_SIZE        SECTOR_16K_SIZE

#define FLASH_SECTOR_3_ADDRESS     (FLASH_SECTOR_2_ADDRESS + FLASH_SECTOR_2_SIZE)
#define FLASH_SECTOR_3_SIZE        SECTOR_16K_SIZE

#define FLASH_SECTOR_4_ADDRESS     (FLASH_SECTOR_3_ADDRESS + FLASH_SECTOR_3_SIZE)
#define FLASH_SECTOR_4_SIZE        SECTOR_64K_SIZE

#define FLASH_SECTOR_5_ADDRESS     (FLASH_SECTOR_4_ADDRESS + FLASH_SECTOR_4_SIZE)
#define FLASH_SECTOR_5_SIZE        SECTOR_128K_SIZE

#define FLASH_SECTOR_6_ADDRESS     (FLASH_SECTOR_5_ADDRESS + FLASH_SECTOR_5_SIZE)
#define FLASH_SECTOR_6_SIZE        SECTOR_128K_SIZE

#define FLASH_SECTOR_7_ADDRESS     (FLASH_SECTOR_6_ADDRESS + FLASH_SECTOR_6_SIZE)
#define FLASH_SECTOR_7_SIZE        SECTOR_128K_SIZE

#define FLASH_SECTOR_8_ADDRESS     (FLASH_SECTOR_7_ADDRESS + FLASH_SECTOR_7_SIZE)
#define FLASH_SECTOR_8_SIZE        SECTOR_128K_SIZE

#define FLASH_SECTOR_9_ADDRESS     (FLASH_SECTOR_8_ADDRESS + FLASH_SECTOR_8_SIZE)
#define FLASH_SECTOR_9_SIZE        SECTOR_128K_SIZE

#define FLASH_SECTOR_10_ADDRESS    (FLASH_SECTOR_9_ADDRESS + FLASH_SECTOR_9_SIZE)
#define FLASH_SECTOR_10_SIZE       SECTOR_128K_SIZE

#define FLASH_SECTOR_11_ADDRESS    (FLASH_SECTOR_10_ADDRESS + FLASH_SECTOR_10_SIZE)
#define FLASH_SECTOR_11_SIZE       SECTOR_128K_SIZE

// User start code address
#if BOOT_SIZE == BOOT_SIZE_12K || BOOT_SIZE == BOOT_SIZE_16K
#define APPLICATION_ADDR		       FLASH_SECTOR_1_ADDRESS

#undef APP_FIRST_UNIT
#define APP_FIRST_UNIT             FLASH_SECTOR_1
#elif BOOT_SIZE == BOOT_SIZE_32K
#define APPLICATION_ADDR		       FLASH_SECTOR_2_ADDRESS

#undef APP_FIRST_UNIT
#define APP_FIRST_UNIT             FLASH_SECTOR_2
#else

#define APPLICATION_ADDR		       FLASH_SECTOR_3_ADDRESS

#undef APP_FIRST_UNIT
#define APP_FIRST_UNIT             FLASH_SECTOR_3
#endif
#endif

#undef APP_UNIT_NUMBER
#define APP_UNIT_NUMBER            (uint32_t)(SECTOR_NUMBER - APP_FIRST_UNIT)

// Flash end address
#define FLASH_END_ADDR		         (FLASH_START_ADDR + FLASH_SIZE - 1)    

// Backup register reset value
#define BKP_RST_VALUE			         ((uint32_t)0x00)

// Backup register check load firmware image file from ExFlash IC
#define BKP_LOAD_VALUE             ((uint32_t)0xC3C3)

// Backup register check jump to app value 
#define BKP_JUMP_VALUE		         ((uint32_t)0xA5A5)

// Backup register value to check enter standby mode
#define BKP_SLEEP_VALUE            ((uint32_t)0xB4B4)

// Backup register for RTC init information
#define BKP_RTC_INIT			         0x00000001U

// Backup register for DFU check value
#define BKP_DFU_CHK				         0x00000002U

// Backup register power check value
#define BKP_PWR_CHK				         0x00000003U

// Backup register storing firmware size
#define BKP_FW_SIZE                0x00000004U

// Backup register storing check value to enter standby mode
#define BKP_SLEEP_CHK              0x00000005U

// Backup register storing MX25L error count and error flag
#define MX25L_ERROR_CHK            0x00000006U

// Backup register storing MX25L error count and error flag
#define PCF_ERROR_CHK              0x00000007U

// Backup registers storing file indices start address
#define BKP_FILE_INDEX_START       0x0000000AU


//============================================================================//
// User Typedef Variables
//============================================================================//

#if HW_TYPE == HW_JAS106
typedef enum 
{
	EN_3V3,
	
	MX25L_SPI_CS,
	
	SD_SPI_DETECT,
	SD_SPI_CS,
	
	LED5,
	LED6,
	LED7
}GPIO_PosTypeDef;
#elif HW_TYPE == HW_JAS208 && SD_INTERFACE == SD_SPI
typedef enum 
{
	EN_MOTOR_PWR,

	EN_3V3,
	
	ACC_IN,
	
	MX25L_SPI_CS,
	
	SD_PWR_EN,
	SD_SPI_DETECT,
	SD_SPI_CS,
	
	LCD_CS,
	LCD_RST,
	LCD_A0,
	LCD_SCL,
	LCD_SIO,
	LCD_BL
}GPIO_PosTypeDef;
#elif HW_TYPE == HW_JAS208 && SD_INTERFACE == SD_SDIO
typedef enum 
{
	EN_MOTOR_PWR,
	
	EN_3V3,
	
	N_ACC_IN,
	
	MX25L_SPI_CS,
	
	SD_PWR_EN,
	SD_SPI_DETECT,
	
	WDT_EN,
	
	LCD_CS,
	LCD_RST,
	LCD_A0,
	LCD_SCL,
	LCD_SIO,
	LCD_BL
}GPIO_PosTypeDef;
#endif 


//============================================================================//
// User Function Prototypes
//============================================================================//
void HW_3V3EN_GPIO_Init(void);
void HW_PBInit(GPIO_PosTypeDef gpioDef, bool bEnEXTI);
void HW_PBDeInit(GPIO_PosTypeDef gpioDef, bool bEnEXTI);
uint8_t HW_PBGetState(GPIO_PosTypeDef gpioDef);
void HW_PBSet(GPIO_PosTypeDef gpioDef);
void HW_PBClr(GPIO_PosTypeDef gpioDef);
void HW_WriteBackupRegister(uint32_t u32BKP_Reg, uint32_t u32Dat);
uint32_t HW_ReadBackupRegister(uint32_t u32BKP_Reg);


//============================================================================//
#endif //_HW_CONFIG_H_
