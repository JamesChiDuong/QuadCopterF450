#ifndef _GPS_DRIVER_H_
#define _GPS_DRIVER_H_


//============================================================================//
// User Includes
//============================================================================//

#include <time.h>

#include "stm32fxxx_hal_user.h"
#include "Config.h"
#include "DataType.h"


//============================================================================//
// User Defines
//============================================================================//

// Available modules for GPS
#define GPS_MODULE_QTEC21         1U

#define GPS_MODULE_SIM68          2U

// GPS hardware type
#if ENABLE_GPS_GNSS
#define GPS_HARDWARE_TYPE         GPS_MODULE_QTEC21
#else
#define GPS_HARDWARE_TYPE         GPS_MODULE_SIM68
#endif

#define GPS_MODULE_TYPE_DEFAULT   GPS_MODULE_TYPE_SIM68

#define GNSS_ENABLE_GET_LOC       1U

#if !ENABLE_MG82
#undef GNSS_ENABLE_GET_LOC
#define GNSS_ENABLE_GET_LOC       1U
#endif

// NMEA message types
#define NMEA_GGA                  "GGA"

#define	NMEA_GLL                  "GLL"

#define	NMEA_GSA                  "GSA"

#define	NMEA_GSV                  "GSV"

#define	NMEA_RMC                  "RMC"

#define	NMEA_VTG                  "VTG"

#define	NMEA_ZDA                  "ZDA"

#define NMEA_HEAD_GPGGA			      "$GPGGA,"

#define NMEA_HEAD_GNGGA			      "$GNGGA,"

#define NMEA_HEAD_GNGNS			      "$GNGNS,"

#define NMEA_HEAD_GPGSA			      "$GPGSA,"

#define NMEA_HEAD_GPRMC			      "$GPRMC,"

#define NMEA_HEAD_GNRMC			      "$GNRMC,"

#define NMEA_HEAD_GPGSV			      "$GPGSV,"

#define NMEA_HEAD_GLGSV			      "$GLGSV,"

#define NMEA_HEAD_GPVTG			      "$GPVTG,"

#define NMEA_HEAD_GNGSA			      "$GNGSA,"

#define NMEA_HEAS_BD_SYSTEM		    "$BD"

#define GPS_SERVICE_TYPE_DEF      GPS_GLONASS
//#define GPS_SERVICE_TYPE_DEF      GPS_GPS

#define	MAXSATELLITE			        20

//#define GPS_POS_FILTER_DIST		    100

#define GPS_STATE_LOCK_CNT           100U

#define GPS_INT_TIMEOUT              60U // 1 Min

#define GPS_RESET_NUM_MAX            10U

#define GPS_RESP_TIMEOUT             10U

#define GPS_EPO_DIR                  "EPO" // EPO file directory

#define GPS_EPO_FILE_NAME            "EPO_" // EPO file name, is followed by date YYMMDD

#define GPS_EPO_FILE_NAME_EXT        ".DAT" // EPO file extension

#define GPS_EPO_SAT_DATA_SIZE        72U // Size of 1 EPO Satellites data in bytes

#define GPS_EPO_SET_SIZE             (GPS_EPO_SAT_DATA_SIZE * 32) // Size of 1 EPO Set in bytes, 2304

#define GPS_EPO_PKT_LEN              0xE3 // EPO packet length

#define GPS_EPO_PACK_LEN             12U // EPO packets pack len 

#define GPS_EPO_SIZE_MAX             70000U // EPO file size limit

#define GPS_EPO_RESP_TIMEOUT         10U // GPS EPO wait response timeout in seconds 

#define GPS_EPO_RETRY_MAX            10U // Maximum EPO retry

#define GPS_EPO_RETRY_MAX_2          3U // Maximum EPO retry

#define GPS_RESPONSE_TIMEOUT         1U // GPS response time out in seconds

#define GPS_XTRA_VALID_DUR_MIN       HOUR_2_SECOND(12U) // Minimum GPS XTRA file valid duration

#define GPS_XTRA_SIZE_MAX            100000U // GPS XTRA file size limit


//============================================================================//
// User Typedef Variables
//============================================================================//
#pragma pack(1)
// SIM68 Type
typedef enum
{
	GPS_MODULE_TYPE_SIM68 = 0U,
	
	GPS_MODULE_TYPE_SIM65
	
} GPS_SIM_TypeDef;

typedef enum
{
	GPS_GPS = 0U,
	
	GPS_GLONASS,
	
	GPS_GALILEO,
	
	GPS_BD
	
} GPS_ServiceTypeDef;

// GPS speed state
typedef enum
{
  GPS_SPEED_NORMAL = 0,
	GPS_SPEED_PEAK_BEGIN, // 1
	GPS_SPEED_PEAK_GOING, // 2
	GPS_SPEED_PEAK_END, // 3
	GPS_SPEED_NOTCH_BEGIN, // 4
	GPS_SPEED_NOTCH_GOING, // 5
	GPS_SPEED_NOTCH_END, // 6
	GPS_SPEED_OVER_TH // 7
	
} GPS_SpeedStateTypeDef;

typedef enum
{
	GPS_POS_SAVE = 0U,
	
	GPS_POS_LOAD,
	
	GPS_POS_MAX_ITEM
	
} GpsModeTypeDef;

// GPS time
typedef struct
{
	uint16_t u16Year;

	uint8_t u8Month;

	uint8_t u8Day;

	uint8_t u8Hour;

	uint8_t u8Min;

	uint8_t u8Sec;

} UTC_TypeDef;

// GPS Longitude
typedef struct
{
	uint8_t u8ddd;

	uint8_t u8mm;

	uint16_t u16mmmm;

	uint8_t u8ss;

	char cIndicator;

} GPS_LonTypeDef;

// GPS Latitude
typedef struct
{
	uint8_t u8dd;

	uint8_t u8mm;

	uint16_t u16mmmm;

	uint8_t u8ss;

	char cIndicator;

} GPS_LatTypeDef;

// GPS number of satellites used
typedef struct
{
  uint16_t u16ddd;

	uint8_t u8mm;
	
} GPS_CourseTypeDef;

// GPS number of satellites used
typedef struct
{
	uint8_t u8Num;

	uint8_t u8Count;

} GPS_StlNumTypeDef;

// GPS Location
typedef struct
{
	double dLatitude;

	double dLongitude;

} GpsCoordTypeDef;

// GPS data struct
typedef struct
{
	UTC_TypeDef utcTime; // UTC time

	bool bStatus; // GPS data status

	GPS_LonTypeDef gpsLon; // GPS Longitude

	GPS_LatTypeDef gpsLat; // GPS Latitude

	GpsCoordTypeDef gpsLocation; // GPS Coordinate 

	GPS_CourseTypeDef gpsCourse; // GPS course

	uint16_t u16Course; // Rounded GPS course

	uint32_t u32Speed; // GPS speed

	uint8_t u8PosFix; // Position fix indicator

	uint8_t u8StlNum; // Number of satellites used

	float fPDOP; // Dilution of Precision

	float fHDOP; // Horizontal Dilution of Precision

	float fVDOP; // Vertical Dilution of Precision

	float fAltitude; // GPS Altitude

} GPS_DataTypeDef;

#if GPS_HARDWARE_TYPE == GPS_MODULE_SIM68 
typedef enum
{
	GPS_CHECK_STEP_NULL = 0U,
	
	GPS_CHECK_STEP_INIT,
	
	GPS_CHECK_STEP_SIM65,
	
	GPS_CHECK_STEP_SIM65_WAIT,
	
	GPS_CHECK_STEP_SIM68,
	
	GPS_CHECK_STEP_SIM68_WAIT,
	
	GPS_CHECK_STEP_FINISH
	
} GPS_CheckVersionTypeDef;

typedef enum
{
	EPO_SIM65_STEP_NULL,
	
	EPO_SIM65_STEP_INIT,
	
	EPO_SIM65_STEP_ERASE,
	
	EPO_SIM65_STEP_ERASE_WAIT,
	
	EPO_SIM65_STEP_ERASE_RETRY,
	
	EPO_SIM65_STEP_SEND_BEGIN,
	
	EPO_SIM65_STEP_SEND_BEGIN_WAIT,
	
	EPO_SIM65_STEP_BEGIN_TRANSFER,
	
	EPO_SIM65_STEP_SEND_PKT,
	
	EPO_SIM65_STEP_GET_DATA,
	
	EPO_SIM65_STEP_WAIT_RESP,
	
	EPO_SIM65_STEP_SEND_FINAL,
	
	EPO_SIM65_STEP_SEND_FINAL_WAIT,
	
	EPO_SIM65_STEP_SEND_PKT_RETRY,
	
	EPO_SIM65_STEP_VERIFY,
	
	EPO_SIM65_STEP_VERIFY_WAIT,
	
	EPO_SIM65_STEP_RETRY_VERIFY,
	
	EPO_SIM65_STEP_FINISH

} EPO_SIM65_StepTypeDef;

typedef enum
{
	EPO_STEP_NULL = 0U, // 0
	
	EPO_STEP_INIT, 
	
	EPO_STEP_CHANGE_UART_MODE,
	
	EPO_STEP_CHANGE_UART_MODE_WAIT,
	
	EPO_STEP_RETRY_CHANGE_UART_MODE,
	
	EPO_STEP_BEGIN_TRANSFER,
	
	EPO_STEP_SEND_PKT,
	
	EPO_STEP_GET_DATA,
	
	EPO_STEP_WAIT_RESP,
	
	EPO_STEP_SEND_FINAL_PKT,
	
	EPO_STEP_WAIT_FINAL_RESP,
	
	EPO_STEP_RETRY_TRANSFER,
	
	EPO_STEP_CHANGE_PMTK_MODE,
	
	EPO_STEP_CHANGE_PMTK_MODE_WAIT,
	
	EPO_STEP_RETRY_CHANGE_PMTK_MODE,
	
	EPO_STEP_RESET_UART_MODE,
	
	EPO_STEP_RESET_UART_MODE_WAIT,
	
	EPO_STEP_RETRY_RESET_UART_MODE,
	
	EPO_STEP_VERIFY,
	
	EPO_STEP_VERIFY_WAIT,
	
	EPO_STEP_RETRY_VERIFY,
	
	EPO_STEP_FINISH
	
} EPO_StepTypeDef;

typedef struct
{
	bool bRequest;
	
	char cFilePath[128];
	
	uint32_t u32FileSize;
	
} EPO_UpdateTypeDef;

typedef struct 
{
	uint8_t u8Preamble[2];
	
	uint8_t u8CmdID[2];
	
	uint8_t u8Length[2];
	
	uint8_t u8Dat;
	
	uint8_t u8ChkSum;
	
	uint8_t u8EndWord[2];

} EPO_SIM65_PktTypeDef;

typedef struct 
{
	uint8_t u8Preamble[2];
	
	uint8_t u8CmdID[2];
	
	uint8_t u8Length[2];
	
	uint8_t u8Dat[72];
	
	uint8_t u8ChkSum;
	
	uint8_t u8EndWord[2];

} EPO_SIM65_DataPktTypeDef;

typedef struct 
{
	uint8_t u8Preamble[2];
	
	uint8_t u8CmdID[2];
	
	uint8_t u8Length[2];
	
	uint8_t u8Dat[4];
	
	uint8_t u8ChkSum;
	
	uint8_t u8EndWord[2];

} EPO_SIM65_RespPktTypeDef;

typedef struct 
{
	uint8_t u8Preamble[2];
	
	uint8_t u8Length[2];
	
	uint8_t u8CmdID[2];
	
	uint8_t u8Seq[2];
	
	uint8_t u8Dat[3][72];
	
	uint8_t u8ChkSum;
	
	uint8_t u8EndWord[2];
	
} EPO_PktTypeDef;

typedef struct 
{
	uint8_t u8Preamble[2];
	
	uint8_t u8Length[2];
	
	uint8_t u8CmdID[2];
	
	uint8_t u8Protocol;
	
	uint32_t u32BaudRate;
	
	uint8_t u8ChkSum;
	
	uint8_t u8EndWord[2];
	
} MTK_Type253_TypeDef;

typedef struct 
{
	uint8_t u8Preamble[2];
	
	uint8_t u8Length[2];
	
	uint8_t u8CmdID[2];
	
	uint8_t u8Seq[2];
	
	uint8_t u8ResCode;
	
	uint8_t u8ChkSum;
	
	uint8_t u8EndWord[2];
	
	uint8_t u8Reserve[100];
	
} EPO_PktRespTypeDef;

typedef struct
{
	char cHeader[10]; // Packet header
	GPS_ServiceTypeDef tService;
	uint32_t u32SetNum; // Number of EPO set
	uint32_t u32FirstWN; // First available week number
	uint32_t u32FirstTOW; // First available TOW
	uint32_t u32LastWN; // Last available week number
	uint32_t u32LastTOW; // Last available TOW
	uint32_t u32FirstCurrWN; // First current week number
	uint32_t u32FirstCurrTOW; // First current TOW
	uint32_t u32LastCurrWN; // Last current week number
	uint32_t u32LastCurrTOW; // Last current TOW
	
} EPO_InfoTypeDef;
#endif
#pragma pack()


//============================================================================//
// User Externs
//============================================================================//

//extern GPS_DataTypeDef tGPS_Data;

extern bool bGPS_ON;

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21
extern bool bGPS_InitFlag;

extern bool bGPS_GetDataEn;
#endif

extern bool bGPS_DataFixed;

extern bool bMCU_RTC_Updated;

extern uint8_t u8GPS_RetryCnt;

extern bool bGPS_SystembootTestFlag;

extern bool bGPS_FirstDetection;

extern bool bGPS_GetInt;

extern uint8_t u8GPS_ResetCnt;


//============================================================================//
// User Function Prototypes
//============================================================================//

#if GPS_HARDWARE_TYPE == GPS_MODULE_SIM68
void GPS_SendBuffer(uint8_t *pu8Buf, uint8_t u8Len);
void GPS_PowerOn(void);
void GPS_PowerOff(void);
void GPS_CheckType(void);
void GPS_CheckTypeProc(void);
#elif GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21
bool GPS_ConfigOutport(void);
bool GPS_ConfigNMEA_Src(void);
bool GPS_ConfigGPS_NMEA_Type(void);
bool GPS_ConfigGLONASS_NMEA_Type(void);
bool GPS_ConfigGalileo_NMEA_Type(void);
bool GPS_ConfigBeidou_NMEA_Type(void);
bool GPS_ConfigGSV_Ext(void);
bool GPS_ConfigGNSS(void);
bool GPS_ConfigAutoGPS(void);
bool GPS_DelMsg(void);
bool GPS_TurnOn(void);
bool GPS_TurnOff(void);
bool GPS_GetLoc(char *pcBuf);
bool GPS_GetNMEA(char *pcBuf, char *pcType);
bool GPS_ConfigXTRA(bool bEnable);
bool GPS_XTRA_Time(char *pcTimeStamp);
bool GPS_XTRA_Data(char *pcFileName);
bool GPS_XTRA_DataQuery(uint16_t *pu16DurTime, time_t *pttValidTime);
void GPS_Config(void);
void GPS_EnableSendMsg(void);
void GPS_EnableRecvMsg(void);
void GPS_Start(void);
void GPS_Finish(void);
bool GPS_GNSS_ParseLoc(char *pcBuf);
#endif
void GPS_Reset(uint8_t u8Delay);
bool GPS_NMEA_ChkSum(char *cMsg);
bool GPS_NMEA_GGA_Parse(char *cMsg);
bool GPS_NMEA_GLL_Parse(char *cMsg);
bool GPS_NMEA_GSA_Parse(char *cMsg);
bool GPS_NMEA_GSV_Parse(char *cMsg);
bool GPS_NMEA_RMC_Parse(char *cMsg);
bool GPS_NMEA_VTG_Parse(char *cMsg);
bool GPS_NMEA_ZDA_Parse(char *cMsg);
bool GPS_NMEA_Parse(void);
void GPS_RX_Proc(void);
#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21 && ENABLE_GPS_XTRA
void GPS_XTRA_RequestDownload(void);
#endif
#if GPS_HARDWARE_TYPE == GPS_MODULE_SIM68 && ENABLE_GPS_EPO
void GPS_EPO_Check(void);
void GPS_EPO_RequestDownload(void);
void GPS_EPO_RequestUpdate(char *pcFilePath, uint32_t u32Size);
void GPS_EPO_UpdateInit(void);
void GPS_EPO_SIM65_UpdateProc(void);
void GPS_EPO_UpdateProc(void);
#endif
bool GPS_GetFixed(void);
bool GPS_GetUTC(time_t *pttUTC);
bool GPS_GetLongitude(GPS_LonTypeDef *p_gpsLongitude);
bool GPS_GetLatitude(GPS_LatTypeDef *p_gpsLatitude);
void GPS_GetCoordinate(GpsCoordTypeDef *p_gpsLocation);
uint32_t GPS_GetSpeed(void);
uint16_t GPS_GetCourse(void);
double GPS_CalcDist_2D(GpsCoordTypeDef *p_gpsLocat1, GpsCoordTypeDef *p_gpsLocat2);
double GPS_LatitudeDMMtoDD(GPS_LatTypeDef *p_gpsLatitude_DMM);
double GPS_LongitudeDMMtoDD(GPS_LonTypeDef *p_gpsLongitude_DMM);
void GPS_LatitudeDDtoDMM(double dLatitude_DD, GPS_LatTypeDef *p_gpsLatitude_DMM);
void GPS_LongitudeDDtoDMM(double dLongitude_DD, GPS_LonTypeDef *p_gpsLongitude_DMM);
void GPS_LastCoordinateProcess(GpsModeTypeDef u8Mode);


//============================================================================//
#endif /* _GPS_DRIVER_H_ */
