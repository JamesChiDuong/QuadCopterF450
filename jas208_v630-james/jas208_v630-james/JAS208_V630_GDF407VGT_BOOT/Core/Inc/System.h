#ifndef _SYSTEM_H_
#define _SYSTEM_H_
//============================================================================//


//============================================================================//
// User Includes
//============================================================================//
#include <ff.h>
#include <stdint.h>
#include <time.h>

#include "Config.h"
#include "Default.h"


//============================================================================//
// User Defines
//============================================================================//
// Upgrade file name
#define FILENAME_UPGRADE_BIN	       "UPGRADE.BIN"

#define UPGRADE_FILE_EXT             "DF2"

// Chinese character font file
#define CHINESE_FONT_FILE            "STDFONT.16"

#define FLASH_HEADER_SIZE            24U

#define ENABLE_HEADER_CHECK          1U


//============================================================================//
// User Functions Prototypes
//============================================================================//

void System_IO_3V3_Enable(void);
void System_IO_3V3_Disable(void);
void System_SetupDefault(void);
void System_Init(void);
void System_DeInit(void);
void System_Reset(void);
bool IsMainRS232_SET(void);


//============================================================================//
#endif //_SYSTEM_H_
