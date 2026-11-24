#ifndef _IAP_FUNC_H_
#define _IAP_FUNC_H_
//============================================================================//


//============================================================================//
// User Includes
//============================================================================//

#include "Default.h"
#include "ff.h"
#include "MX25L_Driver.h"


//============================================================================//
// User Externs
//============================================================================//
extern bool bInDownload;
extern MEM_U32_t mem32DatBuf[8192];
#if HW_TYPE == HW_JAS106
extern GPIO_PosTypeDef gpioLED_Curr;
#elif HW_TYPE == HW_JAS208
extern int iLoadingPercent;
#endif


//============================================================================//
// User Function Prototypes
//============================================================================//

bool IAP_Process(FATFS* drv, char* cDrvPath, uint8_t u8Drv);
bool IAP_LoadFWImage(void);
void IAP_StartNotify(void);
#if HW_TYPE == HW_JAS106
void IAP_StopNotify(void)
#elif HW_TYPE == HW_JAS208
void IAP_StopNotify(bool bRes);
#endif
void IAP_CompleteNotify(bool bUseUSB);
bool IAP_FLASH_Erase(void);
bool IAP_CheckRDP(void);
bool IAP_SetRDP(void);
bool IAP_CheckWRP(void);
bool IAP_EnableWRP(void);
bool IAP_DisableWRP(void);
bool IAP_SaveChineseFont(FATFS* fatfsHandle, char* cDrvPath, uint8_t u8Drv);


//============================================================================//
#endif //_IAP_FUNC_
