#ifndef _EXTIO_H_
#define _EXTIO_H_
//============================================================================//

//============================================================================//
// User Includes
//============================================================================//

#include "stm32fxxx_hal_user.h"
#include "DataType.h"
#include "SD_Module.h"

//============================================================================//
// User Defines
//============================================================================//
#define ADC_VALUE_1V5 1758U

#define ADC_VALUE_1V3 1510U

#define ADC_VALUE_1V1 1252U

// #define ADC_VALUE_DOWN_LEVEL						1700U
#define ADC_VALUE_DOWN_LEVEL 1500U

#define ADC_BAT_DETECT_CHANNEL 14U

#define ADC_MOTOR_PWR_CHANNEL 9U

// #define POWER_VOLT_CONV(VOLT)	          (VOLT * 40960 / 363)
#define POWER_VOLT_CONV(VOLT) (VOLT * 4096 / 363)

#define MOTOR_POWER_VOLT_DEF POWER_VOLT_CONV(8)

#define TEMP_V_SENSE (float)3.3 / 4095

#define TEMP_V_25 (float)0.76

#define TEMP_SLOPE_AVG (float)0.0025

#define SPEED_DIFF_THRESHOLD 7000U // 7 Km/h

#define RPM_LIMIT 30000U

#define ACCEL_SAMPLE_RATE 4

#define ACCEL_LIMIT (float)3.0

#define DECEL_LIMIT (float)-5.0

#define SPD_FILTER_DELAY_NUM 3

//============================================================================//
// User Typedef Variables
//============================================================================//
typedef enum
{
	PWR_DOWN_STEP_NONE,
	PWR_DOWN_STEP_1,
	PWR_DOWN_STEP_2,
	PWR_DOWN_STEP_3

} PwrDownStepTypeDef;

typedef enum
{
	STATE_INIT,
	STATE_ON,
	STATE_OFF,

} StateDetectTypeDef;

typedef enum
{
	INPUT_INIT,
	INPUT_HIGH,
	INPUT_LOW,
	INPUT_CLK,

} InputDetectTypeDef;

// Real speed state
typedef enum
{
	AUTOGAIN_SOURCE_GPS = 0U,

	AUTOGAIN_SOURCE_CAN

} AutoGainSrcTypeDef;

// Real speed state
typedef enum
{
	REAL_SPEED_NORMAL = 0,
	REAL_SPEED_PEAK_BEGIN, // 1
	REAL_SPEED_PEAK_GOING, // 2
	REAL_SPEED_PEAK_END,   // 3
	REAL_SPEED_OVER_TH	   // 4

} Real_SpeedStateTypeDef;

//============================================================================//
// User Variables
//============================================================================//
// static uint16_t Air_Interval_Count = 0;

//============================================================================//
// User Externs
//============================================================================//
extern uint32_t u32PwrDownDelayTick;

extern bool bBackupPwrStable;

extern uint16_t u16PoweADCValue;

extern uint16_t uiSPD_TickIdx;

extern uint16_t uiRPM_TickIdx;

extern uint32_t u32RealSpeedTemp;

extern bool bWaitingEnterStandby;

extern bool bIsSaveSettingFlag;

extern bool bIsCheckDistanceFlag;

extern bool bRS_FirstDetection;

extern uint32_t u32ErrorPageTick;

extern bool bIsShowAGMsgFlag;

extern time_t ttAccOnTime;

extern uint8_t u8SpdFilDelayCnt;

extern bool bIsBacklightFlag;

extern bool bChkGAIN_Step1;
extern bool bChkGAIN_Step2;

//============================================================================//
// User Function Prototypes
//============================================================================//
bool EXTIO_IsMainPowerNormal(void);
bool EXTIO_IsBkpPowerNormal(void);
bool EXTIO_IsPowerNormal(void);
void EXTIO_Init(void);
void EXTIO_Proc(void);
void EXTIO_CallBackHandle(void);
void EXTIO_BatteryEnable(void);
void EXTIO_BatteryDisable(void);
void EXTIO_DetectPWR(void);
void EXTIO_DetectACC_2(void);
void EXTIO_DetectCHG(void);
double EXTIO_GetDistGPS(void);
void EXTIO_AutoGainProc(void);
void EXTIO_AGAIN_DebugLog(char *pcLog, uint16_t u16Len);
uint16_t EXTIO_GetADC1(uint8_t u8Channel);
//============================================================================//
#endif /* _EXTIO_H_ */
