#ifndef _SD_MODULE_H_
#define _SD_MODULE_H_
//============================================================================//


//============================================================================//
// User Includes
//============================================================================//
#include "ff.h"
//#include "SD_Driver.h"


//============================================================================//
// User Defines
//============================================================================//
#define SD_BLOCK_SIZE           0x200

#define MSG_SD_DISK_INVALID		  "\r\n> SD card is INVALID."
#define MSG_SD_OPEN_DIR_ERROR		"\r\n> SD open dir ERROR."

#define SD_BUFFER_SIZE          2048

#define SD_ERROR_MAX_CNT        3

#define SD_RESET_MAX_CNT        3


//============================================================================//
// User Externs
//============================================================================//
//extern DIR dirSD_Handle;

//extern char cSD_FilePath[128];

//extern uint8_t u8SD_Buf[SD_BUFFER_SIZE];

//============================================================================//
// User Function Prototypes
//============================================================================//
void SD_PWR_Enable(void);
void SD_PWR_Disable(void);
void SD_ShowInfo(void);
bool SD_FS_Check(bool bInit);
bool SD_FS_GetStatus(void);
bool SD_Init(void);
bool SD_Deinit(void);
void SD_Reset(void);
bool SD_CheckDirExist(char *pcDirName);
bool SD_MakeDir(char *pcDirName);
bool SD_RemoveDir(char *pcDirName);
bool SD_CheckFileExist(char *pcFileName);
bool SD_FindFileName(char *pcFindName, char *pcReturnName);
uint32_t SD_GetFileSize(char *pcFileName);
bool SD_Format(void);
bool SD_RemoveFile(char *pcFilename);
bool SD_Rename(char *pcOldName, char *pcNewName);
bool SD_AddBuffToFile(char *pcFileName, uint8_t *pu8Buf, uint16_t u16BufLen);
bool SD_AddStrToFile(char *pcFileName, char *pcString);
bool SD_ReadAddrToBuff(char *pcFileName, uint8_t *pu8Buf,
                       uint16_t u16BufLen, uint32_t u32Addr);
bool SD_ReadFileToBuff(char *pcFileName, uint8_t *pu8Buf, uint16_t u16BufLen);
bool SD_WriteBuffToAddr(char *pcFileName, uint8_t *pu8Buf,
                        uint16_t u16BufLen, uint32_t u32Addr);
bool SD_WriteBuffToFile(char *pcFileName, uint8_t *pu8Buf, uint16_t u16BufLen);
bool SD_CreateFile(char *pcFileName, uint32_t u32Addr, bool bClearFlag);


//============================================================================//
#endif /* _SD_MODULE_H_ */
