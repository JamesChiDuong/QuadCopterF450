
#ifndef _RULEV161_H_
#define _RULEV161_H_
//============================================================================//

//============================================================================//
// User Includes
//============================================================================//

#include "stm32fxxx_hal_user.h"
#include "Config.h"
#include "DataType.h"
#include "time.h"
#include "CommFunc.h"


//============================================================================//
// User Defines
//============================================================================//
// Table5
#define FILENAME_BREAKDOWN_TMP			           "Breakdown.tmp"
#define FILENAME_EVENT_TMP				             "Event.tmp"

// Table6
#define	FILENAME_DETAILSPEED_TMP		           "DetailSpeed.tmp"
//#define	FILENAME_DETAILSPEED_DAY_TMP 	         "DetailSpeed_Day.tmp"
//#define	FILENAME_DETAILSPEED_DAY_2_TMP 	       "DetailSpeed_Day_2.tmp"

// Table7
#define FILENAME_TECHNICAL_TMP		             "Technical.tmp"
#define FILENAME_CALIBRATION_TMP		           "Calibration.tmp"
#define FILENAME_ADJUST_TMP				             "Adjust.tmp"
#define FILENAME_DRIVING_TMP		             	 "Driving.tmp"
#define FILENAME_REST_TMP				               "Rest.tmp"

// Table8
#define FILENAME_DRIVE_TMP				             "Drive.tmp"
//#define FILENAME_DRIVE_DAY_TMP			           "Drive_Day.tmp"
//#define FILENAME_DRIVE_DAY_2_TMP		           "Drive_Day_2.tmp"
//#define FILENAME_DRIVE_BUF_TMP			           "Drive_Buf.tmp"

// Table9
#define FILENAME_POSITION_TMP			             "Position.tmp"
//#define FILENAME_POSITION_DAY_TMP		           "Position_Day.tmp"
//#define FILENAME_POSITION_DAY_2_TMP		         "Position_Day_2.tmp"

// Table10
#define FILENAME_EXTERNAL_TMP			             "External.tmp"
//#define FILENAME_EXTERNAL_DAY_TMP		           "External_Day.tmp"
//#define FILENAME_EXTERNAL_DAY_2_TMP		         "External_Day_2.tmp"

// New VDR File Name
#define FILENAME_MIN_VDR_TMP									 "MinVDRBuf.tmp"

// Round Trip temp
#define	FILENAME_VDR_NOW_TMP								  "RoundTrip.tmp"

#define FOLDERNAME_VDR                         "VDR"
#define FOLDERNAME_VDR_2                       "VDR"
#define FOLDERNAME_DTR												 "DTR"

#define HEADLENS				                2
#define SHORTHEAD				                1
#define DATELENS				                127
#define POSITIONLENS			              11
#define BREAKDOWNLENS			              15
#define EVENTLENS				                16
#define DRIVINGLENS				              26
#define CALIBRATIONLENS			            58
#define ADJUSTLENS				              14
#define DRIVERTIMELENS			            18
#define RESTLENS			        	        18
#define POSLENS					                667
#define EXTLENS									        12
#define BCDDATE					                7
#define CheckSumMax				              128
#define BYTEMAX					                255
#define	WORDMAX					                65535
#define DSPEEDMAX				                43200
#define POSITIONMAX                     1440
#define CALMAX					                365
#define POSMAX					                8760
#define RPMLENS													9

#define EVENT_POWER_200MS				        1
#define EVENT_NON_AUTH					        2

#define BROKEN_GPS_MODULE				        1
#define BROKEN_PRINTER_MODULE		        2
#define BROKEN_LCD_MODULE				        3

#define MAX_INDEXED_FILE_NUM            10

//#define SD_DETAIL_SPEED_QUEUE_MAX       10
//#define SD_POSITION_SHORT_QUEUE_MAX     10
//#define SD_POSITION_LONG_QUEUE_MAX      2
#define SD_1MIN_QUEUE_MAX               30
#define SD_RECORD_DRIVE_QUEUE_MAX       60
#define SD_RECORD_EXTEND_QUEUE_MAX      50

#define SD_1MIN_TASK_RETRY_MAX          3U
#define SD_REC_EXT_TASK_RETRY_MAX       3U


//============================================================================//
// User Typedef Variables
//============================================================================//
typedef enum
{
	INITAL_STEP,
	INITAL_STEP2,
	MAKE_TABLE5_STEP,
	MAKE_TABLE5_1_STEP,
	MAKE_TABLE5_2_STEP,
	TABLE6_INITAL_STEP,
	MAKE_TABLE6_STEP,
	MAKE_TABLE6_1_STEP,
	MAKE_TABLE6_2_STEP,
	MAKE_TABLE6_3_STEP,
	MAKE_TABLE7_STEP,
	MAKE_TABLE7_1_STEP,
	MAKE_TABLE7_2_STEP,
	MAKE_TABLE7_3_STEP,
	MAKE_TABLE7_4_STEP,
	MAKE_TABLE7_5_STEP,
	TABLE8_INITAL_STEP,
	MAKE_TABLE8_STEP,
	TABLE9_INITAL_STEP,
	MAKE_TABLE9_STEP,
	CHECK_CODE_INITAL_STEP,
	MAKE_CHECK_CODE_STEP,
	CLOSE_INITAL,
	CLOSE_INITAL2,
	CLOSE_STEP,
	MAX_STEP,

} MakeVDR_StepTypeDef;

typedef enum
{
	STEP_1,
	STEP_2,
	STEP_3,
	STEP_4,
	STEP_5,
	STEP_6,
	STEP_7,
	STEP_8,
	STEP_9,
	STEP_10,
	STEP_11,
	STEP_12,
	STEP_13,
	STEP_14,
	STEP_15,
	STEP_16,
	STEP_17,
	STEP_18,
	STEP_19,
	STEP_20,
	STEP_21,
	STEP_22,
	STEP_23,
	STEP_24,
	STEP_25,
	STEP_26,
	STEP_27,
	STEP_28,
	STEP_29,
	STEP_30,
	STEP_31,
	STEP_32,
	STEP_33,
	STEP_34,
	STEP_OVER,
	STEP_MAX

} NormalStepTypeDef;

#pragma pack(1)
// Table5
typedef	struct
{
	uint8_t u8BlockIndex;
	uint8_t u8Code;
	uint8_t u8DateStartBCD[7];
	uint8_t u8DateStopBCD[7];

} BreakdownDataTypeDef;

typedef	struct
{
	uint8_t u8BlockIndex;
	uint8_t u8Code;
	uint8_t u8DateStartBCD[7];
	uint8_t u8DateStopBCD[7];
	uint8_t u8EventCnt;

} EventDataTypeDef;

// Table 6
typedef	struct
{
	uint16_t u16BlockIndex;
	uint8_t u8Index;
	uint8_t u8DateBCD[7];
	uint8_t u8SpeedData[120];

} DetailSpeedTypeDef;

// Table 7
typedef	struct
{
	uint8_t u8Company[36];
	uint8_t u8Address[36];
	uint8_t u8DeviceNum[8];
	uint8_t u8FwNum[4];
	uint8_t u8FwDateBCD[7];
	uint8_t u8MakeYear[4];
	uint8_t u8CertNum[8];
	uint8_t u8BusID[17];

} TechnicalDataTypeDef;

typedef	struct
{
	uint16_t u16BlockIndex;
	uint8_t u8NameID[36];
	uint8_t u8BeforeDistBCD[4];
	uint8_t u8AfterDistBCD[4];
	uint8_t u8BeforeDateBCD[7];
	uint8_t u8AfterDateBCD[7];

} CalibrationDataTypeDef;

typedef	struct
{
	uint8_t u8BlockIndex;
	uint8_t u8BeforeDateBCD[7];
	uint8_t u8AfterDateBCD[7];

} AdjustDataTypeDef;

typedef	struct
{
	uint8_t u8BlockIndex;
	uint8_t u8BeforeDateBCD[7];
	uint8_t u8AfterDateBCD[7];
	uint8_t u8BeforeAdjustBCD[2];
	uint8_t u8AfterAdjustBCD[2];

} DrivingTimeTypeDef;

typedef	struct
{
	uint8_t u8BlockIndex;
	uint8_t u8BeforeDateBCD[7];
	uint8_t u8AfterDateBCD[7];
	uint8_t u8BeforeRestBCD[2];
	uint8_t u8AfterRestBCD[2];

} RestDataTypeDef;

// Table8
typedef	struct
{
	uint16_t u16BlockIndex;
	uint8_t u8DriverName[18];
	uint8_t u8DriveCode;
	uint8_t u8DriveDateBCD[7];

} DriveDataTypeDef;

// Table9
typedef	struct
{
	uint16_t u16BlockIndex;
	uint8_t u8Index;
	uint8_t u8DateBCD[7];
	uint8_t u8Position[660];
} PositionDataTypeDef;

//Table10
typedef	struct
{
	uint16_t u16BlockIndex;
	uint8_t u8DateBCD[7];
	uint32_t u32Distance;
	uint8_t u8IOStatus;
} ExternalDataTypeDef;

typedef	struct
{
	uint8_t u8Code;
	uint8_t u8Name[18];
	uint32_t u32Lens;
} DataBlockTypeDef;

//Table12
typedef	struct
{
	uint32_t u32UTCTime;
	uint16_t u16Rpm;
	uint8_t  u8GpsSpeed;
	MEM_U16_t u16IOStatus;
} RpmTypeDef;

/*
typedef struct
{
  bool bRequest[SD_DETAIL_SPEED_QUEUE_MAX];

	char cDstFileName[2][50];

	uint32_t u32Addr[SD_DETAIL_SPEED_QUEUE_MAX][2];

	uint32_t u32DatLen;

	uint8_t u8Data[SD_DETAIL_SPEED_QUEUE_MAX][128];

	uint16_t u16Idx[2];

}SD_DetailSpeedTypeDef;

typedef struct
{
  bool bRequest[SD_POSITION_SHORT_QUEUE_MAX];

	char cDstFileName[2][50];

	uint32_t u32Addr[SD_POSITION_SHORT_QUEUE_MAX][2];

	uint32_t u32DatLen;

	uint8_t u8Data[SD_POSITION_SHORT_QUEUE_MAX][12];

}SD_PositionShortTypeDef;

typedef struct
{
  bool bRequest[SD_POSITION_LONG_QUEUE_MAX];

	char cDstFileName[2][50];

	uint32_t u32Addr[SD_POSITION_LONG_QUEUE_MAX][2];

	uint32_t u32DatLen;

	uint8_t u8Data[SD_POSITION_LONG_QUEUE_MAX][670];

	uint16_t u16Idx[2];

}SD_PositionLongTypeDef;
*/

typedef struct
{
	bool bRequest[SD_1MIN_QUEUE_MAX];

	bool b1HourRequest;

	bool b_u16Index2Save;

	uint32_t u32DetailSpeedLen;

	uint32_t u32DetailSpeedDayLen;

	uint32_t u32Position1HourLen;

	uint32_t u32PositionDay1HourLen;

	uint32_t u32PositionLen;

	uint32_t u32PositionDayLen;

	uint32_t u32DetailSpeedAddr[SD_1MIN_QUEUE_MAX];

	uint32_t u32DetailSpeedDayAddr[SD_1MIN_QUEUE_MAX];

	uint32_t u32Position1HourAddr;

	uint32_t u32PositionDay1HourAddr;

	uint32_t u32PositionAddr[SD_1MIN_QUEUE_MAX];

	uint32_t u32PositionDayAddr[SD_1MIN_QUEUE_MAX];

	uint8_t u8DetailSpeedData[SD_1MIN_QUEUE_MAX][DATELENS + 1];

	//uint8_t u8DetailSpeedDayData[SD_1MIN_QUEUE_MAX][DATELENS + 1];

	uint8_t u8Position1HourData[POSLENS + 1];

	//uint8_t u8PositionDay1HourData[POSLENS + 1];

	uint8_t u8PositionData[SD_1MIN_QUEUE_MAX][POSITIONLENS + 1];

	//uint8_t u8PositionDayData[SD_1MIN_QUEUE_MAX][12];

	uint16_t u16DSBlockIndex;

	uint16_t u16PosBlockIndex;

	uint16_t u16Index, u16Index2;

	uint8_t u8Index;

} SD_1MinTaskTypeDef;

typedef struct
{
	bool bRequest[SD_RECORD_DRIVE_QUEUE_MAX];

	uint32_t u32DriveAddr[SD_RECORD_DRIVE_QUEUE_MAX];

	uint32_t u32DriveDayAddr[SD_RECORD_DRIVE_QUEUE_MAX];

	uint32_t u32DatLen;

	uint8_t u8Data[SD_RECORD_DRIVE_QUEUE_MAX][DRIVINGLENS + 1];

	uint16_t u16DrvBlockIndex;

	uint16_t u16DriveActCntDay;

} SD_RecordDriveTypeDef;

typedef struct
{
	bool bRequest[SD_RECORD_EXTEND_QUEUE_MAX];

	uint32_t u32RecExtAddr[SD_RECORD_EXTEND_QUEUE_MAX];

	uint32_t u32RecExtDayAddr[SD_RECORD_EXTEND_QUEUE_MAX];

	uint32_t u32DatLen;

	uint8_t u8Data[SD_RECORD_EXTEND_QUEUE_MAX][EXTLENS + 1];

	uint16_t u16RecExtBlockIndex;

	uint16_t u16Idx;

} SD_RecordExtendTypeDef;

// Table Index 00
typedef	struct
{
	uint8_t u8TableCode;
	
	uint8_t u8Name[18];
	
	uint32_t u32Lens;
	
	BreakdownDataTypeDef stBK_DATA;
	
} VdrBreakdownDataTypeDef;

// Table Index 01
typedef	struct
{
	uint8_t u8TableCode;
	
	uint8_t u8Name[18];
	
	uint32_t u32Lens;
	
	EventDataTypeDef stEvent;
	
} VdrEventDataTypeDef;

// Table Index 02
typedef	struct
{
	uint8_t u8TableCode;
	
	uint8_t u8Name[18];
	
	uint32_t u32Lens;
	
	uint16_t u16BlockIndex;

	uint8_t u8DateBCD[7];
	
	uint8_t u8SpeedData[120];
	
} VdrDetailSpeedTypeDef;

// Table Index 03
/*
typedef	struct
{
	uint8_t u8TableCode;
	
	uint8_t u8Name[18];
	
	uint32_t u32Lens;
	
  TechnicalDataTypeDef stTechData;
	
} VdrTechnicalDataTypeDef;
*/
typedef	struct
{
	uint8_t u8TableCode;
	
	uint8_t u8Name[18];
	
	uint32_t u32Lens;
	
  TechnicalDataTypeDef stTechData;
	
	uint8_t u8CheckSum;
	
} VdrTechnicalDataTypeDef;

// Table Index 04
typedef	struct
{
	uint8_t u8TableCode;
	
	uint8_t u8Name[18];
	
	uint32_t u32Lens;
	
	CalibrationDataTypeDef stCalData;
	
} VdrCalibrationDataTypeDef;

// Table Index 05
typedef	struct
{
	uint8_t u8TableCode;
	
	uint8_t u8Name[18];
	
	uint32_t u32Lens;
	
	AdjustDataTypeDef stAdjust;
	
} VdrAdjustDataTypeDef;

// Table Index 06
typedef	struct
{
	uint8_t u8TableCode;
	
	uint8_t u8Name[18];
	
	uint32_t u32Lens;
	
	DrivingTimeTypeDef stDrvTime;
	
} VdrDrivingTimeTypeDef;

// Table Index 07
typedef	struct
{
	uint8_t u8TableCode;
	
	uint8_t u8Name[18];
	
	uint32_t u32Lens;
	
	RestDataTypeDef stRestTime;
	
} VdrRestDataTypeDef;

// Table Index 08
typedef	struct
{
	uint8_t u8TableCode;
	
	uint8_t u8Name[18];
	
	uint32_t u32Lens;
	
	uint16_t u16BlockIndex;
	
	uint8_t u8DriverName[18];
	
	uint8_t u8DriveCode;
	
	uint8_t u8DriveDateBCD[7];
	
	uint8_t u8DriverName2[18];
	
	uint8_t u8DriveCode2;
	
	uint8_t u8DriveDateBCD2[7];
	
} VdrDriveDataTypeDef;

// Table Index 09
typedef	struct
{
	uint8_t u8TableCode;
	
	uint8_t u8Name[18];
	
	uint32_t u32Lens;
	
	uint16_t u16BlockIndex;
	
	uint8_t u8DateBCD[7];
	
	uint8_t u8Position[11];
	
} VdrPositionDataTypeDef;

// Table Index 10
typedef	struct
{
	uint8_t u8TableCode;
	
	uint8_t u8Name[18];
	
	uint32_t u32Lens;
	
	ExternalDataTypeDef stExtData;
	
} VdrExternalDataTypeDef;

// Table Index 11
typedef	struct
{
	uint8_t u8TableCode;
	
	uint8_t u8Name[18];
	
	uint32_t u32Lens;
	
	uint16_t u16DataLens;
	
	uint8_t u8TimeBCD[7];
	
	uint16_t u16EventCode;
	
} VdrDatalogEventDataTypeDef;

// Table Index 12
typedef	struct
{
	uint8_t u8TableCode;
	
	uint8_t u8Name[18];
	
	uint32_t u32Lens;
	
	uint32_t u32DataLens;
	
	RpmTypeDef stRpmBuf[30];
	
} VdrRpmDataTypeDef;

typedef struct
{
	bool bIsShowFlag;
	
	uint8_t u8EventTime[3];
	
} PowerOffEventTypeDef;

typedef struct
{
	bool bIsHappenFlag;
	
	uint8_t u8BCDBuf[7];
	
} PrinterEventTypeDef;

#pragma pack()


//============================================================================//
// User Externs
//============================================================================//

extern char cPC_HourFileName[50];

extern bool bDoneHourVDRFile;

extern uint8_t u8SpeedBufTemp[60][2], u8SpeedBuf[60][2];

extern uint8_t u8SpeedDateBCD[7];

extern DetailSpeedTypeDef u8DetailSpeed;

extern PositionDataTypeDef PositionData;

//extern uint8_t u8OneMinAvgSpeed;

extern uint32_t u32TickBuf;

extern bool bSD_SaveDatRequest;

extern uint8_t u8HourDataCnt;

extern bool bHourDataFlag;

extern bool bDLRS232Flag;

//extern bool bOneSecFlag;

extern bool bHalfSecFlag;

//extern uint8_t u8HalfSecSpeed[2];

//extern time_t ttTimeTag;

extern bool bDoVDR_HourFlag, bDoVDR_DayFlag;

extern bool bDoHourVDRFlag;

extern bool bPreWoringFlag;

extern bool bIsDelVDRFileFlag;

extern bool bRpmSecSaveFlag;

extern bool bCheckVDR_TmpExist;

extern PowerOffEventTypeDef stPwrOffEvent;

extern time_t ttLastLogTime;

extern ProcStepTypeDef procCreateHourData;

//extern uint8_t u8VDR_Buf[2048];

extern uint8_t u8RpmBufIndex;

extern VdrRpmDataTypeDef stVdrRpm, stVdrRpmTemp;
//extern VdrRpmDataTypeDef stVdrRpm;

extern uint8_t u8SecBk;

#if ENABLE_INDEX_BUFFER
extern char cIndexedFileName[MAX_INDEXED_FILE_NUM][30];
//extern bool bFileIdxIsSaved;
#endif


//============================================================================//
// User Functions
//============================================================================//
void V161_1MinLogProc(void);
void V161_CheckRecordFile(void);
void V161_DetailSpeedInit(void);
void V161_PositionInit(void);
#if 0
bool V161_MoveDataProc(void);
void V161_RenameDayFile(void);
void V161_RecreateDayFile(void);
void V161_CopyDayFile(void);
#endif
void V161_CreateHourDataProc(uint8_t u8GetHour);
void V161_CopyFile(char *pcScrFileName, char *pcDstFileName);
bool V161_SaveEvent(uint8_t EventIndex);
void V161_WriteTechnicalToFile(char *pcFileName);
void V161_RecordBreakdown(BreakdownDataTypeDef stBreakDown);
void V161_RecordEvent(EventDataTypeDef stEvent);
void V161_RecordDriving(DriveDataTypeDef stDriving);
void V161_RecordTechnical(uint8_t u8Item);
void V161_RecordCalibration(CalibrationDataTypeDef stCalibration);
void V161_RecordAdjust(AdjustDataTypeDef stAdjust);
void V161_RecordDrivingTime(DrivingTimeTypeDef stDrivingTime);
void V161_RecordRestTime(RestDataTypeDef stRestTime);
void V161_RecordExternslData(void);
bool V161_CheckVDRHeader(void);

#if ENABLE_SD_BUFFER
bool V161_SDBuf_IsHighLoading(void);
void V161_SDBuf_SaveDataInit(void);
void V161_SDBuf_SaveDataProc(void);
#endif

#if ENABLE_INDEX_BUFFER
void V161_IndexFileNameInit(void);
void V161_SaveFileIdxAll(void);
void V161_LoadFileIdxAll(void);
bool V161_SaveFileIdx(char *pcFileName, uint16_t u16Idx);
bool V161_GetFileIdx(char *pcFileName, uint16_t *pu16Idx);
#endif

void V161_WriteBreakdownToVDR(BreakdownDataTypeDef stBreakDown);
void V161_WriteEventToVDR(EventDataTypeDef stEvent);
void V161_WriteDetailSpeedToVDR(DetailSpeedTypeDef stDSpeed);
void V161_WriteTechnicalToVDR(void);
void V161_WriteCalibrationToVDR(CalibrationDataTypeDef stCalData);
void V161_WriteAdjustToVDR(AdjustDataTypeDef stAdjust);
void V161_WriteDrivingToVDR(DrivingTimeTypeDef stDrivingTime);
void V161_WriteRestToVDR(RestDataTypeDef stRestTime);
void V161_WriteDrvStatusToVDR(uint8_t *pu8BCDTime);
void V161_WritePositionToVDR(VdrPositionDataTypeDef stVdrPosition);
void V161_WriteExternalToVDR(ExternalDataTypeDef stExtData);
void V161_WriteDatalogEventToVDR(uint8_t *u8BCDTime, uint16_t u16EventCode);
void V161_MakeDailyVDR(void);
void V161_MakeHourVDR(void);
void V161_WriteRpmToVDR(VdrRpmDataTypeDef stVdrRpm);
void V161_WriteTechnicalToUSB(char *pcFileName);
void V161_MakeVDRNowFile(void);
void V161_CreateVDRNowFile(void);
void V161_SaveCalibration(void);
void V161_LastMinLogProc(void);


//============================================================================//
#endif //_RULEV161_H_

