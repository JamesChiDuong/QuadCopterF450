#ifndef _BUZZER_H_
#define _BUZZER_H_
//============================================================================//

//============================================================================//
// User Includes
//============================================================================//
#include "stm32fxxx_hal_user.h"
#include "DataType.h"


//============================================================================//
// User Defines
//============================================================================//
#define BUZZER_ITEM_MAX			         10

#define BUZZER_INTERVAL_SHORT	       (500 / 100)

#define BUZZER_INTERVAL_LONG	       (S_2_mS(1) / 100)

#define BUZZER_INVALID_ID   	       0x80

#define BUZZER_BEEP_CONTINUE	       0xFF


//============================================================================//
// User Typedefs
//============================================================================//
typedef enum
{
	BEEP_SHORT = 0,

	BEEP_LONG

} BUZZER_INTERVAL_t;

#pragma pack(1)
typedef struct
{
	bool Enable;

	uint8_t u8Interval;

	uint8_t u8CountIntv;

	uint8_t u8CountBeep;

} BuzzerItem_t;
#pragma pack()


//============================================================================//
// User Externs
//============================================================================//
extern bool bBzEnable;

extern uint8_t u8BzItem;


//============================================================================//
// User Function Prototypes
//============================================================================//
extern bool Buzzer_Handle_Proc(void);
extern bool Buzzer_Handle_Proc2(void);
void Buzzer_SetMute(FlagStatus MUTE);
bool Buzzer_SetRing(uint8_t *pu8ITEM, BUZZER_INTERVAL_t tINTERVAL, uint8_t u8COUNT);
bool Buzzer_ClrRing(uint8_t *pu8ITEM);
void Buzzer_SetRing2(uint8_t u8Item);
void Buzzer_ClrRing2(void);
bool Buzzer_GetState(uint8_t *pu8BzState);
void Buzzer_Beep(BUZZER_INTERVAL_t tINTERVAL, uint8_t u8COUNT);


//============================================================================//
#endif /* _BUZZER_H_ */
