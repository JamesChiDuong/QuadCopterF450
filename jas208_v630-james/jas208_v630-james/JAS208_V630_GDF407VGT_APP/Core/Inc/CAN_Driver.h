#ifndef _CAN_DRIVER_H_
#define _CAN_DRIVER_H_
//============================================================================//

//============================================================================//
// User Includes
//============================================================================//
#include "stm32fxxx_hal_user.h"
#include "Config.h"
#include "DataType.h"


//============================================================================//
// User Defines
//============================================================================//
#define CAN_UNDEF				          (0)

#define CAN_SAE_J1939			        (1)

#define CAN_ISO_15765			        (2)

#define CAN_CUSTOM_NORMAL  			  (0)
#define CAN_CUSTOM_DAEWOO  			  (1)
#define CAN_CUSTOM_SUNWIN		  	  (2)
#define CAN_CUSTOM_DAF            (3)
#define CAN_CUSTOM_KINGLONG			  (4)
#define CAN_CUSTOM_GDBUS          (5)
#define CAN_CUSTOM_MBEYE          (6)

#define CAN_CUSTOM				        CAN_CUSTOM_NORMAL

#if CUSTOMER_TYPE == CUSTOMER_NORMAL || CUSTOMER_TYPE == CUSTOMER_TESTB
#undef CAN_CUSTOM
#define CAN_CUSTOM				        CAN_CUSTOM_NORMAL
#elif CUSTOMER_TYPE == CUSTOMER_MASTER
#undef CAN_CUSTOM
#define CAN_CUSTOM				        CAN_CUSTOM_DAEWOO
#elif CUSTOMER_TYPE == CUSTOMER_TANGENG
#undef CAN_CUSTOM
#define CAN_CUSTOM				        CAN_CUSTOM_SUNWIN
#elif CUSTOMER_TYPE == CUSTOMER_DAF
#undef CAN_CUSTOM
#define CAN_CUSTOM				        CAN_CUSTOM_DAF
#elif CUSTOMER_TYPE == CUSTOMER_KINGLONG
#undef CAN_CUSTOM
#define CAN_CUSTOM				        CAN_CUSTOM_KINGLONG
#elif CUSTOMER_TYPE == CUSTOMER_GDBUS
#undef CAN_CUSTOM
#define CAN_CUSTOM				        CAN_CUSTOM_GDBUS
#elif CUSTOMER_TYPE == CUSTOMER_FORMOSA
#undef CAN_CUSTOM
#define CAN_CUSTOM				        CAN_CUSTOM_MBEYE
#elif CUSTOMER_TYPE == CUSTOMER_FCFC
#undef CAN_CUSTOM
#define CAN_CUSTOM				        CAN_CUSTOM_MBEYE
#endif

#define	COUNT_RELINK			        4

#define	COUNT_SEND_MSG			      1

#define CAN_FILTER_ID(ID, IDE, RTR) \
								      (((uint32_t)ID << 3) | IDE | RTR)

#define CAN_EXT_ID(P, DP, PGN, SA) \
								   (((uint32_t)P << 26) \
								   | ((uint32_t)DP << 24) \
								   | ((uint32_t)PGN << 8) | SA)

#define GET_EXT_PGN(EXT_ID)		((uint32_t)EXT_ID >> 8 & 0xFFFF)

#if \
    CAN_CUSTOM == CAN_CUSTOM_NORMAL || \
		CAN_CUSTOM == CAN_CUSTOM_DAEWOO || \
		CAN_CUSTOM == CAN_CUSTOM_SUNWIN || \
		CAN_CUSTOM == CAN_CUSTOM_DAF || \
    CAN_CUSTOM == CAN_CUSTOM_KINGLONG || \
    CAN_CUSTOM == CAN_CUSTOM_GDBUS		
// PGN61440 - Electronic Retarder Controller 1 - ERC1 -
#define CAN_J1939_PGN61440           0xF000

// PGN61441 - Electronic Brake Controller 1 - EBC1
#define CAN_J1939_PGN61441           0xF001

// PGN61443 - Electronic Engine Controller 2 - EEC2 - 
#define CAN_J1939_PGN61443           0xF003

// PGN61444 - Electronic Engine Controller 1 - EEC1 -
#define CAN_J1939_PGN61444		       0xF004

// PGN61445 - Electronic Transmission Controller 2 - ETC2 -
#define CAN_J1939_PGN61445           0xF005

// PGN61449 - Vehicle Dynamic Stability Control 2 - VDC2 -
#define CAN_J1939_PGN61449           0xF009

// PGN64695 - Vehicle Electrical Power #4 - VEP4 -
#define CAN_J1939_PGN64695           0xFCB7

// PGN64777 - High Resolution Fuel Consumption (Liquid) - HRLFC -
#define CAN_J1939_PGN64777           0xFD09

// PGN64893 - FMS Tell Tale Status - FMS1 -
#define CAN_J1939_PGN64893           0xFD7D

// PGN64932 - PTO Drive Engagement - PTODE -
#define CAN_J1939_PGN64932           0xFDA4

// PGN64933 - Door Control 2 - DC2 -
#define CAN_J1939_PGN64933           0xFDA5

// PGN64962 - Electronic Engine Controller 14 - EEC14 -
#define CAN_J1939_PGN64962           0xFDC2

// PGN64977 - FMS standard Interface Identity / Capabilities - FMS -
#define CAN_J1939_PGN64977           0xFDD1

// PGN65102 - Door Control 1 - DC1 -
#define CAN_J1939_PGN65102           0xFE4E

// PGN65110 - Aftertreatment 1 Diesel Exhaust Fluid Tank 1 Information - AT1T1I -
#define CAN_J1939_PGN65110           0xFE56

// PGN65112 - Air Suspension Control 4 - ASC4 - 
#define CAN_J1939_PGN65112           0xFE58

// PGN65131 - Driver Identification - DI -
#define CAN_J1939_PGN65131           0xFE6B

// PGN65132 - Tachograph - TCO1 -
#define CAN_J1939_PGN65132		       0xFE6C

// PGN65136 - Combination Vehicle Weight - CVW -
#define CAN_J1939_PGN65136           0xFE70

// PGN65198 - Air Supply Pressure - AIR1
#define CAN_J1939_PGN65198           0xFEAE

// PGN65199 - Fuel Consumption (Gaseous) - GFC -
#define CAN_J1939_PGN65199           0xFEAF

// PGN65216 - Service Information - SERV -
#define CAN_J1939_PGN65216           0xFEC0

// PGN65217 - High Resolution Vehicle Distance - VDHR -
#define CAN_J1939_PGN65217		       0xFEC1

// PGN65237 - Alternator Speed - AS -
#define CAN_J1939_PGN65237           0xFED5

// PGN65248 - Vehicle Distance - VD -
#define CAN_J1939_PGN65248		       0xFEE0

// PGN65253 - Engine Hours/Revolution - HOURS - 
#define CAN_J1939_PGN65253           0xFEE5

// PGN65254 - Time/Date - TD -            
#define CAN_J1939_PGN65254           0xFEE6

// PGN65257 - Fuel Consumption - LFC -
#define CAN_J1939_PGN65257           0xFEE9

// PGN65258 - Vehicle Weight - VW -
#define CAN_J1939_PGN65258           0xFEEA

// PGN65260 - Vehicle Identification - VI -
#define CAN_J1939_PGN65260           0xFEEC

// PGN65262 - Engine Temperature 1 - ET1 -
#define CAN_J1939_PGN65262           0xFEEE 

// PGN65265 - Cruise Control/Vehicle Speed - CCVS -
#define CAN_J1939_PGN65265		       0xFEF1

// PGN65266 - Fuel Economy - LFE -
#define CAN_J1939_PGN65266           0xFEF2

// PGN65269 - Ambient Conditions - AMB -
#define CAN_J1939_PGN65269           0xFEF5

// PGN65276 - Dash Display 1 - DD1 -
#define CAN_J1939_PGN65276           0xFEFC

//PGN65514 - GDBUS Command 1 -
#define CAN_J1939_PGN65514		       0xFFEA

//PGN65515 - GDBUS Command 2 -
#define CAN_J1939_PGN65515		       0xFFEB

//PGN65534 - GDBUS Command 3 -
#define CAN_J1939_PGN65534		       0xFFFE
#endif

#if CAN_CUSTOM == CAN_CUSTOM_MBEYE
#define CAN_MBEYE_700                0x700

#define CAN_MBEYE_720                0x720

#define CAN_MBEYE_727                0x727

#define CAN_MBEYE_760                0x760

#define LENS_MASK_TIME			         S_2_mS(900)
#endif

#define CAN_RESET_TIMEOUT            60U // Seconds


//============================================================================//
// User Typedef Variables
//============================================================================//
typedef enum
{
	LINK_BEGIN = 0U,		//
	LINK_J1939,		// SAE J1939-71, 250Kbps, CAN-ID: 29bit
	LINK_J1979_1,	// ISO15765-4, 250Kbps, CAN-ID: 11bit, ISO15031-5
	LINK_J1979_2,	// ISO15765-4, 250Kbps, CAN-ID: 29bit, ISO15031-5
	LINK_J1979_3,	// ISO15765-4, 500Kbps, CAN-ID: 11bit, ISO15031-5
	LINK_J1979_4,	// ISO15765-4, 500Kbps, CAN-ID: 29bit, ISO15031-5
	LINK_END,		//
} CAN_LinkTypeDef;

typedef enum
{
	PID_DEF_00_1,

	PID_DEF_00_2,

	PID_DEF_00_3,

	PID_DEF_END

} PID_DEF_t;

typedef enum
{
	PID_DATA_0C,

	PID_DATA_0D,

	PID_DATA_END

} PID_DATA_t;

#pragma pack(1)
#if CAN_CUSTOM == CAN_CUSTOM_DAF
typedef union
{
	uint16_t byte;

	struct
	{
		unsigned DOOR_1   :1;	
		unsigned DOOR_2   :1;
		unsigned DOOR_3   :1;
		unsigned DOOR_4   :1;
		unsigned DOOR_5   :1;
		unsigned DOOR_6   :1;
		unsigned DOOR_7   :1;
		unsigned DOOR_8   :1;
		unsigned DOOR_9   :1;	
		unsigned DOOR_10  :1;
		unsigned DOOR_11  :1;
		unsigned DOOR_12  :1;
		unsigned DOOR_13  :1;
		unsigned DOOR_14  :1;
		unsigned DOOR_15  :1;
		unsigned DOOR_16  :1;

	} bits;

} CAN_DoorStateTypeDef;

typedef struct 
{
  bool bBrakeSysErr; // 0: Brake system OK, 1: Brake system failure, malfunction
	
	bool bBrakeState;
	
	uint8_t u8BrakePos;
	
	uint8_t u8AEBS_State;
	
} CAN_BrakeStateTypeDef;

typedef struct 
{
  uint8_t u8AllDoorStat;
	
	uint8_t u8DoorStat[10];
	
	CAN_DoorStateTypeDef tDoorState;
	
} CAN_DoorControlTypeDef;

typedef struct
{
  uint8_t u8HighHeadBeam;
	
	uint8_t u8LowHeadBeam;
	
	uint8_t u8TurnSignal;
	
} CAN_LightStateTypeDef;
#endif

#if CAN_CUSTOM == CAN_CUSTOM_GDBUS
typedef struct
{
  uint16_t u16MOT_SPEED;	// Motor Speed.
	uint8_t u8BAT_CHG; // Charging State.
	
	uint16_t u16BAT_VOLTAGE;	// Motor Speed.
	int16_t i16BAT_CURRENT; // Motor Speed.
	uint16_t u16BAT_SOC; // Battery soc;

	uint8_t u8CELL_VOL_MAX;	// Battery cell voltage max
	uint8_t u8CELL_VOL_MIN;	// Battery cell voltage min
	uint8_t u8CELL_TMP_MAX;	// Battery cell temperature max.
	uint8_t u8CELL_TMP_MIN;	// Battery cell temperature min.
	
} CAN_PowerStateTypeDef;
#endif

#if CAN_CUSTOM == CAN_CUSTOM_MBEYE
typedef struct
{
	uint8_t u8HeadwayMeasurement;
	
	bool bLeft_LDW;
	
	bool bRight_LDW;
	
	bool bFCW_On;
	
	uint8_t u8HeadwayWarningLevel;
	
	bool bTamperAlert;
	
}CAN_MBEYE_DataTypeDef;
#endif

typedef struct
{
	uint16_t u16RPM;

	uint32_t u32Speed; // M/H

	float fDIST;
	
	#if CAN_CUSTOM == CAN_CUSTOM_DAF
	CAN_BrakeStateTypeDef tBrakeState;
	
	CAN_DoorControlTypeDef tDoorControl;
	
	CAN_LightStateTypeDef tLightState;
	
	bool bLDWS_En;
	
	uint8_t u8LDWS_Inc;
	#endif
	
	#if CAN_CUSTOM == CAN_CUSTOM_GDBUS
	CAN_PowerStateTypeDef tPowerState;
	#endif
	
	#if CAN_CUSTOM == CAN_CUSTOM_MBEYE
	CAN_MBEYE_DataTypeDef tCAN_MBEYE;
	#endif

} CAN_DATA_t;

typedef uint16_t CAN_PGN_t;

typedef uint32_t CAN_ID_t;
#pragma pack()


//============================================================================//
// User Externs
//============================================================================//
extern CAN_RxHeaderTypeDef CAN_RxHeaderHandle;

extern uint8_t u8CAN_RxMsg[8];

extern CAN_DATA_t tCAN_DATA;

extern bool bCAN_GetRX;

extern bool bCAN_GetDist;

extern bool bCAN_Ready;

extern uint8_t CAN_LinkCount;


//============================================================================//
// User Functions
//============================================================================//
bool CAN_CheckEnable(void);
void CAN_Config(void);
void CAN_LinkOBD(void);
void CAN_Proc(void);
void CAN_ParseMsg(void);
uint32_t CAN_GetSpeed(void);
uint16_t CAN_GetRPM(void);
double CAN_GetDist(void);
bool CAN_ResDetect(void);
#if CAN_CUSTOM == CAN_CUSTOM_GDBUS
uint16_t CAN_GetMotSpeed(void);
uint8_t CAN_GetBatChg(void);
uint16_t CAN_GetBatVoltage(void);
int16_t CAN_GetBatCurrent(void);
uint8_t CAN_GetSOC(void);
uint8_t CAN_GetCellVolMax(void);
uint8_t CAN_GetCellVolMin(void);
uint8_t CAN_GetCellTmpMax(void);
uint8_t CAN_GetCellTmpMin(void);
#endif
#if CAN_CUSTOM == CAN_CUSTOM_MBEYE
bool MBEYE_LeftLDW(void);
bool MBEYE_RightLDW(void);
bool MBEYE_FCWOn(void);
uint8_t MBEYE_HeadwayWarningLevel(void);
#endif


//============================================================================//
#endif /* _CAN_DRIVER_H_ */
