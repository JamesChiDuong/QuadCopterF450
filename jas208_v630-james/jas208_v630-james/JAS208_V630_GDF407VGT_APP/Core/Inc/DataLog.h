#ifndef _DATALOG_H_
#define _DATALOG_H_
//============================================================================//

//============================================================================//
// User Includes
//============================================================================//
#include <time.h>

#include "stm32fxxx_hal_user.h"
#include "CommFunc.h"
#include "Config.h"
#include "DataType.h"
#include "GSM_QtEC21.h"


//============================================================================//
// Public define
//============================================================================//
#define	FILENAME_DATALOG_BIN	     "DATALOG.BIN"

#define	FILENAME_DATALOG_EVENT_BIN "DATALOG_EVENT.BIN"

#define	DATALOG_FILE_MAX		       90

// Datalog 資料格式版本
#define DATALOG_VERSION_DEF		     (0)

#define	DATALOG_MAX_ERR_CNT			   5

//#define DATALOG_FAST_LOAD	         1

//#define	CMD_DATALOG_HEAD		       0xD0

//#define	CMD_DATALOG_GPS			       0xD1

//#define	CMD_DATALOG_INFO		       0xD2

//#define	CMD_DATALOG_EXT	           0xD3

//#define	CMD_DATALOG_LOG_IN_OUT	   0xD4

//#define	CMD_DATALOG_EXT2	         0xD5

#define	CMD_DATALOG_SIMPLE		     0xD7

#define	CMD_DATALOG_BATTERY	       0xDA // Heinz 

//#define	CMD_SIMPLE_INFO	           0xDB // Heinz EDR Info

#define	CMD_DATALOG_EVENT		       0xDC

#define	DATALOG_SIMPLE_SIZE_MAX	   32U

#define DATALOG_UPLOAD_MAX_LIMIT   60U

#define	DATALOG_UPLOAD_MAX_60      60U
#define	DATALOG_UPLOAD_MAX_30		   30U
#define DATALOG_UPLOAD_MAX_20      20U
#define	DATALOG_UPLOAD_MAX_15		   15U
#define	DATALOG_UPLOAD_MAX_10		   10U

#define FILENAME_SPEED_INT_FILE		      "SpeedInt.tmp"
#define FILENAME_CAN_DEBUG_MSG_FILE     "CanDebug.tmp"
#define FILENAME_PULSE_CNT_FILE         "PulseCount.tmp"
#define FILENAME_GPS_SPEED_FILE         "GPS_Speed.tmp"
#define FILENAME_GPRS_LOG_FILE          "GPRS_Log.tmp"
#define FILENAME_AGAIN_LOG_FILE					"AGAIN_Log.tmp"

#define RECORD_DATA_SIZE                30U // Don't change this parameter's value

#define YEAR_2_DAYS_MAX             		366U // Maximum number of days in a year
#define MONTH_2_DAYS_MAX                31U // Maximum number of days in a month

#define VDR_RETAIN_PERIOD               (1 * YEAR_2_DAYS_MAX) // VDR files smallest retain time in days
#define DTR_RETAIN_PERIOD               (3 * MONTH_2_DAYS_MAX) // DTR files smallest retain time in days
#define LOG_RETAIN_PERIOD               (3 * MONTH_2_DAYS_MAX) // Unimportant log files retain time in days

#define VDR_CLEAR_FILES_MAX             100U //  Maximum number of VDR files to clear each run 
#define DTR_CLEAR_FILES_MAX             40U // Maximum number of DTR files to clear each run
#define LOG_CLEAR_FILES_MAX             40U // Maximum number of other log files to clear each run


//============================================================================//
// User Typedef Variables
//============================================================================//
#pragma pack(1)
typedef enum
{
  CLEAR_FILE_NONE = 0U,
	
	CLEAR_FILE_VDR,
	
	CLEAR_FILE_DTR,
	
	CLEAR_FILE_CAN_DEBUG,
	
	CLEAR_FILE_SPEED_INT,
	
	CLEAR_FILE_PULSE_CNT,
	
	CLEAR_FILE_GPS_SPEED,
	
	CLEAR_FILE_GPRS_LOG,
	
	CLEAR_FILE_AGAIN_LOG
	
} ClearFileTypedef;

// Datalog 資料結構, JASSLIN 基本格式主機資訊, Command: 0xD0
/*
typedef struct
{
	uint8_t CMD;

	// Datalog 資料格式版本
	uint16_t u16Version;

	// 系統內部世界標準時間
	time_t UTC;

	// 資料擷取間隔
	uint16_t INTERVAL;

	// 車號 or 車機編號
	char BUS_ID[11];

	// 車輛識別碼 (引擎號碼 or 車身號碼)
	char VIN[21];

} DatalogHeadTypeDef;
*/
// Datalog 資料結構, JASSLIN 基本格式 GPS 資訊, Command: 0xD1
/*
typedef struct
{
	uint8_t CMD;

	// GPS 緯度資料, 為 10 進制格式再乘 10^6 後只取整數
	int32_t i32Latitude;

	// GPS 經度資料, 為 10 進制格式再乘 10^6 後只取整數
	int32_t i32Longitude;

	// 是否定位
	uint8_t u8Fixed;

	// 航向 0 ~ 359度
	uint16_t u16Course;

} DatalogGPS_TypeDef;
*/
// Datalog 資料結構, JASSLIN 基本格式行車資訊, Command: 0xD2
/*
typedef struct
{
	uint8_t CMD;

	// 使用者(駕駛)識別碼
	uint32_t u32DRV_IN;

	// 數入訊號狀態
	MEM_U8_t EXTIO;

	// 時速單位為公里
	uint8_t u8Speed;

	// 引擎轉速
	uint16_t u16RPM;

	// 總里程單位為百公尺, default: 00000000
	uint32_t dwhmDistance;

} DatalogInfoTypeDef;
*/
// Datalog 資料結構, JASSLIN 基本格式延伸資訊, Command: 0xD3
/*
typedef struct
{
	uint8_t CMD;

	// 駕駛行為分析儀(DMD) 送出的資料
	MEM_U8_t DMD_STATE;

	MEM_U8_t RES_1;

	MEM_U8_t RES_2;

	MEM_U8_t RES_3;

} DatalogExtTypeDef;
*/
// Datalog 資料結構, JASSLIN 基本格式延伸資訊, Command: 0xD5
/*
typedef struct
{
	uint8_t CMD;

	// Ext IO
	MEM_U8_t EXT2_IO;

} DatalogExt2_TypeDef;
*/
// 上傳起始資料結構
typedef struct
{
	uint8_t CMD;

	// Datalog 資料格式版本
	uint16_t u16Version;

	// 系統內部世界標準時間
	time_t UTC;

	// 總里程單位為公尺, default: 00000000
	uint32_t dwDistance;

	// 使用者(駕駛)識別碼
	uint32_t u32DRV_IN;

	// 行車紀錄資料筆數
	uint8_t u8DataCnt;

} UploadHeadTypeDef;

// 上傳紀錄資料結構
typedef struct
{
	// GPS 經度資料, 為 10 進制格式再乘 10^6 後只取整數
	int32_t i32Longitude;

	// GPS 緯度資料, 為 10 進制格式再乘 10^6 後只取整數
	int32_t i32Latitude;

	// 航向 0 ~ 359度, 單位 10 度
	uint8_t u8Course;

	// 時速, 單位為 KM/H
	uint8_t u8Speed;

	// 引擎轉速, 單位為 50RPM
	uint8_t b50RPM;

	// 數入訊號狀態
  MEM_U8_t EXTIO;

	// GPS時速, 單位為 KM/H
	uint8_t u8Speed_GPS;
	
	MEM_U8_t u8Status;

} UploadDataTypeDef;

// Datalog 資料結構, 上傳封包簡易格式, Command: 0xD7
typedef struct
{
	UploadHeadTypeDef HEAD;

	UploadDataTypeDef DATA[DATALOG_UPLOAD_MAX_LIMIT];

} DatalogUploadTypeDef;

typedef struct
{
	UploadHeadTypeDef HEAD;

	UploadDataTypeDef DATA;

} DatalogReUploadTypeDef;

// Datalog 資料結構, 上傳封包簡易格式, Command: 0xD7
typedef struct
{
	UploadHeadTypeDef HEAD;

	UploadDataTypeDef DATA;

	uint8_t RES[DATALOG_SIMPLE_SIZE_MAX - (sizeof(UploadHeadTypeDef) + sizeof(UploadDataTypeDef))];

} DatalogSimpleTypeDef;

// Datalog 資料結構, 上傳封包簡易格式訊息, Command: 0xDB
/*
typedef struct
{
	uint8_t CMD; // Header.

	char Version[39];

	float SPD_GAIN;

	uint16_t SPD_DIV;

	uint8_t CAN_SPD;

	uint8_t CAN_RPM;

	uint32_t SPD_LIMIT1;

	uint32_t SPD_LIMIT2;

	uint32_t SPD_LIMIT3;

	char szDNS[30];

	uint8_t IP_ADD[4];

	uint16_t IP_PORT[1];

} DatalogSimpleInfoTypeDef;
*/
// Datalog data structure, DTR record event Command: 0xDC
typedef struct
{
	uint8_t CMD;
	
	time_t UTC;
	
	uint16_t u16EventCode;
	
	uint8_t u8Speed;
	
	int32_t i32Longitude;
	
	int32_t i32Latitude;
	
	uint32_t dwDistance;
	
	uint32_t u32DRV_IN;
	
	uint16_t u16RPM;
	
	uint16_t u16Course;
	
	uint8_t u8Fixed;
	
	uint8_t u8STNum;
	
	uint8_t u8CSQ;

	MEM_U8_t EXTIO;
	
}DatalogEventlogTypeDef;
/*
typedef struct
{
	uint8_t CMD;

	time_t UTC;

	uint32_t dwDistance;

	uint8_t u8Res[7];

} DatalogV161_TypeDef;
*/
#if ENABLE_CAN == 1 && CAN_CUSTOM == CAN_CUSTOM_GDBUS
// Datalog Battery info	Command: 0xDA
typedef struct
{
	uint8_t CMD; // Header.

	int16_t MOT_SPEED; // Motor Speed.

	uint8_t CHG_STATE; // Battery charging state.

	uint16_t BAT_VOL;	// Battery Voltage.

	uint16_t BAT_CUR;	// Battery Current.

	uint16_t BAT_SOC;	// Battery SOC.

	uint16_t BAT_CELL_VMAX;	// Battery Cell Voltage Max.

	uint16_t BAT_CELL_VMIN;	// Battery Cell Voltage Min

	uint16_t BAT_CELL_TMAX;	// Battery Cell Temperature Max.

	uint16_t BAT_CELL_TMIN;	// Battery Cell Temperature Min.

} Datalog_BAT_InfoTypeDef;
#endif

#if ENABLE_SPEED_INT_REC
typedef struct
{
  uint8_t u8BCDTime[4];

  uint32_t u32SpeedIntDebug;
	
} SpeedIntTypeDef;
#endif

#if ENABLE_PULSE_COUNT_REC
typedef struct
{
  uint8_t u8BCDTime[4];
	
	uint8_t u8PulseCnt[RECORD_DATA_SIZE];
	
} PulseCountTypeDef;
#endif

#if ENABLE_GPS_SPEED_REC
typedef struct
{
  uint8_t u8BCDTime[4];
	
	uint8_t u8Speed[RECORD_DATA_SIZE];
	
} GPS_SpeedRecTypeDef;
#endif
#pragma pack()


//============================================================================//
// User Externs
//============================================================================//
extern bool bDatalogTickFlag;

#if ENABLE_GSM_NET
extern DatalogUploadTypeDef datalogHandle[MAX_SOCKET_BANK], 
	                          datalogHandleTemp[MAX_SOCKET_BANK];

extern uint8_t u8DATALOG_UPLOAD_MAX[MAX_SOCKET_BANK];
#endif

#if ENABLE_SPEED_INT_REC
extern SpeedIntTypeDef tSpeedInt, tSpeedIntTemp;
extern bool bRecordSpeedIntFlag;
extern time_t ttRecordSpeedIntTime;
extern const uint32_t u32MaskBit[32];
#endif

#if ENABLE_PULSE_COUNT_REC
extern PulseCountTypeDef tPulseCnt, tPulseCntTemp;
extern bool bRecordPulseCntFlag;
extern time_t ttRecordPulseCntTime;
#endif

#if ENABLE_GPS_SPEED_REC
extern GPS_SpeedRecTypeDef tGPS_Speed, gpsSpdRecTemp;
extern bool bRecordGPS_SpeedFlag;
extern time_t ttRecordGPS_SpeedTime;
#endif

extern const char * RecordModeFile[];

extern ProcStepTypeDef procClearFileStep;


//============================================================================//
// User Functions
//============================================================================//
void Datalog_Init(void);
void Datalog_Reset(void);
bool Datalog_Save_Proc(void);
bool Datalog_ChkData(void);
bool Datalog_ResendLog(time_t ttStart, time_t ttStop, uint8_t u8SocketBank);
bool Datalog_ResendEvent(time_t ttStart, time_t ttStop, uint8_t u8SocketBank);
bool Datalog_IsRecordMode(RecordModeTypeDef tRecordMode);
#if ENABLE_SPEED_INT_REC
bool Datalog_RecordSpeedInt(void);
#endif
#if ENABLE_PULSE_COUNT_REC
bool Datalog_RecordPulseCount(void);
#endif
#if ENABLE_GPS_SPEED_REC
bool Datalog_RecordGPS_Speed(void);
#endif
void Datalog_ClearFileProc(void);


//============================================================================//
#endif /* _DATALOG_H_ */
