#ifndef _PRN_DRIVER_H_
#define _PRN_DRIVER_H_
//============================================================================//

//============================================================================//
// User Includes
//============================================================================//

#include "stm32fxxx_hal_user.h"
#include "DataType.h"
#include "MX25L_Driver.h"


//============================================================================//
// User Defines
//============================================================================//
#define DRAW_LCD		               1
#define DRAW_PRN     	             2

#define BYTES_PER_ROW              18

#define PRINTSPEED                 100

#define DRAW_COLUMN_SIZE_MAX       50 //300

//#define DRAW_COLUMN_SIZE         (DRAW_COLUMN_STEP * 59)

#define CHART_SPEED_AXIS_MAX       140U

#define PRINT_DATA_TEMP_FILE       "PRINT_DATA_TEMP.tmp"

#define MAX_PRINTING_ITEM          5U

//#define PRINT_TEMP_DATA_SIZE       3072

#define SPEED_RECORD_SIZE          1444U

#define MAX_PRINT_RETRY            3U   

#define MOTOR_DLY					         25000U


//============================================================================//
// User Typedef Variables
//============================================================================//
typedef enum
{
	PRT_IDLE = 0U,
	PRT_INIT,
	PRT_GETDATA,
	PRT_SENDDATA,
	PRT_HEAT1_OPEN,
	PRT_HEAT1_CLOSE,
	PRT_HEAT2_OPEN,
	PRT_HEAT2_CLOSE,
	PRT_MOTOR1,
	PRT_MOTOR2,
	PRT_MOTOR3,
	PRT_MOTOR4,
	PRT_STOP,
	PRRT_MAX,
	PRN_PRINT_ONE_LINE, //[Nancy add]
	PRT_MOTOR_SET,
	PRT_HEAT
} PrintStepTypeDef;

#pragma pack(1)
typedef struct
{
  uint8_t u8PrintID;
	
	bool bRePrint;
	
	char cDataBuf[70][40];
	
	uint8_t u8ReprintCnt;

} PrintDataTypeDef;
#pragma pack()


//============================================================================//
// User Externs
//============================================================================//
extern PrintStepTypeDef u8PStep;

extern uint16_t u16PrintRequestNum;

extern bool bPRN_Run;

extern bool bGetSpeedRecord;

extern PrintStepTypeDef step;


//============================================================================//
// User Function Prototypes
//============================================================================//
void PRN_Init(void);
void PRN_Printing(uint8_t u8Item);
void PRN_SetHourRange(uint8_t u8Hours);
void PRN_Proc(void);
bool PRN_PaperStatus(void);
bool PRN_LoadSpeedRecord(void);
bool PRN_UpdateSpeedRecord(uint8_t u8SpeedVal);
void PRN_GetSpeedRecord(void);
void PRN_Delay(short cnt);

//============================================================================//
#endif /* _PRN_DRIVER_H_ */
