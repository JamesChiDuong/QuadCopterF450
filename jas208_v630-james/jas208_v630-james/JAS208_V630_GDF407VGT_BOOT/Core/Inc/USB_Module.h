#ifndef _USB_MODULE_H_
#define _USB_MODULE_H_
//============================================================================//


//============================================================================//
// User Includes
//============================================================================//
#include "stm32fxxx_hal_user.h"
#include "CommFunc.h"
#include "DataType.h"
#include "USBH_Usr.h"


//============================================================================//
// User Defines
//============================================================================//

#define MSG_USB_DRIVE_INVALID		         "\r\n>  USB drive invalid."
#define MSG_USB_OPEN_DIR_ERROR		       "\r\n>  USB drive open dir error."

#define USB_FILE_TRANSFER_MAX_RETRY      (5U)

//#define USB_DOWNLOAD_STEP_AMOUNT        (8192U)
#define USB_DOWNLOAD_STEP_AMOUNT        (204800U)

#define USB_DOWNLOAD_TEMP_FILE          "USB_DOWNLOAD_TEMP.tmp"


//============================================================================//
// User TypeDef
//============================================================================//

#pragma pack(1)

typedef enum
{
	APP_AUTH_ROOT,
	
	APP_AUTH_FACTORY,
	
	APP_AUTH_ADMIN,
	
	APP_AUTH_USER,
	
}APP_AUTH_t;

typedef struct
{
	APP_AUTH_t AUTH; 

	uint8_t LOG_CNT;
	
}APP_CONFIG_t;

typedef struct
{
  char cDownloadFile[128];
	
	uint8_t u8ErrCnt;
	
}USB_DownloadTypeDef;

#pragma pack()

//============================================================================//
// User Externs
//============================================================================//
extern bool bUSB_Mounted;

extern uint8_t u8PublicDataBuf[8192]; //__attribute__ ((section("ccm_var")));

extern ProcStepTypeDef procUSB_TransStep;

//extern bool bUSB_TransFinish;

extern bool bUSB_PrepData;

extern uint16_t u16USB_DownloadNum;

//extern uint8_t u8USB_FileTransRetry;

extern bool bUSB_RunOne;

extern APP_CONFIG_t gAppCfg;

extern uint8_t u8USB_ReportEvent;

//============================================================================//
// User Function Prototypes
//============================================================================//
bool USB_Mount(void);
bool USB_UnMount(void);
bool USB_Check(void);
bool USB_FS_Init(void);
bool USB_FS_DeInit(void);
static bool USB_CheckFileByName(char *pcFileName, char *pcFilter);
static bool USB_CheckFileByExten(char *pcFileName, char *pcFilter);
bool USB_CheckFileExist(char *pcFileName);
bool USB_FindFileName(char *pcFindName, char *pcReturnName);
uint32_t USB_GetFileSize(char *pcFileName);
bool USB_FileList(void);
bool USB_RemoveFile(char *pcFilenames);
bool USB_Rename(char *pcOldName, char *pcNewName);
bool USB_AddBuffToFile(char *pcFileName, uint8_t *pu8Buf, uint16_t u16BufLen);
bool USB_AddStrToFile(char *pcFileName, char *pcString);
bool USB_ReadAddrToBuff(char *pcFileName, uint8_t *pu8Buf,
                        uint16_t u16BufLen, uint32_t u32Addr);
bool USB_ReadFileToBuff(char *pcFileName, uint8_t *pu8Buf, uint16_t u16BufLen);
bool USB_WriteBuffToAddr(char *pcFileName, uint8_t *pu8Buf,
                         uint16_t u16BufLen, uint32_t u32Addr);
bool USB_WriteBuffToFile(char *pcFileName, uint8_t *pu8Buf, uint16_t u16BufLen);
//void USB_Demo(void);


//============================================================================//
#endif /* _USB_MODULE_H_ */
