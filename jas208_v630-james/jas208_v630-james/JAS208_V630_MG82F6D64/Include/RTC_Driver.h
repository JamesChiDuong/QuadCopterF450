#ifndef _RTC_DRIVER_H
#define _RTC_DRIVER_H

//============================================================================//
// Include
//============================================================================//
#include "Type.h"
#include "Utility.h"

//============================================================================//
// Define
//============================================================================//
#define RTC_WKUP_COUNTER_MIN       1U // Minimum RTC alarm counter in seconds
#define RTC_WKUP_COUNTER_MAX       DAY_2_SECOND(2U) // Maximum RTC alarm counter in seconds, 172800U

//============================================================================//
// Typedef
//============================================================================//
typedef enum
{
	RTC_WKUP_NONE = 0,
	RTC_WKUP_ALM,
	RTC_WKUP_PERIODIC
} RTC_WKUP_TypeDef;
	
typedef struct DATE_TIME
{
	u8 u8Year;
	u8 u8Mon;
	u8 u8Day;
	u8 u8Hour;
	u8 u8Min;
	u8 u8Sec;
	//u8 u8WeekDay;	//1(Monday),2,3,4,5,6,7(Sunday) // Might be not needed
} DATE_TIME;

typedef struct RTC_ALM
{
	u8 u8Enable;
	u8 u8Repeat;
	DATE_TIME tAlarmTime;
} RTC_ALM_TypeDef;

typedef struct RTC_PED_WKUP
{
	u8 u8Enable;
	u32 u32Timeout;
	u32 u32Counter;
} RTC_PED_WKUP_TypeDef;

//============================================================================//
// Extern
//============================================================================//
extern bit bRTC_SetTimeFlag;
extern xdata DATE_TIME DateTime, DateTimeTemp;

//============================================================================//
// Function
//============================================================================//
void RTC_Init(void);
#if !EN_RTC_WKUP
void RTC_DeInit(void);
#endif
void InitDateTime(void);
void IncDateTime(void);
#if EN_RTC_WKUP
void RTC_Wkup_Handle(void);
#endif

#endif // _RTC_DRIVER_H