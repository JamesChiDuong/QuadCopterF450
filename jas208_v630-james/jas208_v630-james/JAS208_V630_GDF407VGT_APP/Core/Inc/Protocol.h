#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_
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
// �}�Y���O (for standard protocol)
#define CMD_HEAD_STD "$AS+"

// Device Information //
#define STR_AS_VERSION "$AS+VERSION="
#define STR_AS_BUS_ID "$AS+BUS ID="
#define STR_AS_BUS_SN "$AS+BUS SN="
#define STR_AS_VIN "$AS+VIN="
#define STR_AS_CAR_TYPE "$AS+CAR TYPE="
#define STR_AS_DF2_INFO "$AS+DF2 INFO="

// Time Setting //
#define STR_AS_TIME_ZONE "$AS+TIME ZONE="
#define STR_AS_RTC_CALIBRATION "$AS+RTC CAL="
#define STR_AS_RTC_TIME "$AS+RTC TIME="
#define STR_AS_TIME_ADJ "$AS+TIME ADJ="

// External Information Setting //
// Power
#define STR_AS_POWER_MODE "$AS+POWER MODE="
#define STR_AS_POWER_DOWN "$AS+POWER DOWN="
// Distance
#define STR_AS_DISTANCE "$AS+DISTANCE="
// Speed
#define STR_AS_SPEED_TRIG "$AS+SPEED TRIGGER=" // Reserved
#define STR_AS_SPEED_GAIN "$AS+SPEED GAIN="
#define STR_AS_SPEED_ADJ "$AS+SPEED ADJ="
#define STR_AS_SPEED_OFFSET "$AS+SPEED OFFSET="
#define STR_AS_AUTOGAIN_SPEED "$AS+AUTOGAIN SPEED="
#define STR_AS_AUTO_GAIN "$AS+AUTO GAIN="
#define STR_AS_SPEED_LIMIT "$AS+SPEED LIMIT="
#define STR_AS_OVER_SPEED "$AS+OVER SPEED="
#define STR_AS_ACCEL_LIMIT "$AS+ACCEL LIMIT="
#define STR_AS_AUTO_GAIN_TIME "$AS+AUTOGAIN TIME="
#define STR_AS_KGAIN "$AS+KGAIN="
#define STR_AS_GAIN_CHK_EN "$AS+GAIN CHK="
// RPM
#define STR_AS_RPM_TRIG "$AS+RPM TRIGGER=" // Reserved
#define STR_AS_RPM_DIV "$AS+RPM DIV="
#define STR_AS_RPM_LIMIT "$AS+RPM LIMIT="
#define STR_AS_RPM_IDLE "$AS+RPM IDLE="
// CAN Data
#define STR_AS_CAN_LINK "$AS+CAN LINK="
#define STR_AS_CAN_DIST "$AS+CAN DIST="
#define STR_AS_CAN_SPEED "$AS+CAN SPEED="
#define STR_AS_CAN_RPM "$AS+CAN RPM="
#define STR_AS_CAN_OTHER "$AS+CAN OTHER="
#define STR_AS_CAN_RELINK_ALL "$AS+CAN RELINK ALL="
// Mobile Eye CAN config commands
#if ENABLE_CAN == 1 && CAN_CUSTOM == CAN_CUSTOM_MBEYE
#define STR_AS_LDW_CNT "$AS+LDW CNT="
#define STR_AS_LDW_TIME "$AS+LDW TIME="
#define STR_AS_HW_TIME "$AS+HW TIME="
#endif
// Other External Information
#if HW_TYPE == HW_JAS211 || HW_TYPE == HW_JAS106 || HW_TYPE == HW_JAS208 || HW_TYPE == HW_JAS306
#define STR_AS_EXIN_REVERSE "$AS+EXIN REVERSE="
#endif
#define STR_AS_ALARM_BEEP "$AS+ALARM BEEP="
#define STR_AS_CORRECT_MODE "$AS+CORRECT MODE="
#define STR_AS_IMMOBILIZER "$AS+IMMO=" // Reserved
// Driver Information //
#define STR_AS_DRIVER_ID "$AS+DRIVER ID="
#define STR_AS_DRIVER_IN "$AS+DRIVER IN="
#define STR_AS_DRIVE_TIME_THRESHOLD "$AS+DRIVE TIME="
#define STR_AS_REST_TIME_THRESHOLD "$AS+REST TIME="
// DB9 Function Select
#define STR_AS_DB9_FUNCTION "$AS+DB9 FUNC="
// Merge GPS Speed Select
#define STR_AS_MERGE_SPEED "$AS+MERGE SPEED="
// AutoGain Distsnce Limit
#define STR_AS_AG_DIST_LIMIT "$AS+AUTOGAIN DIST="

#if ENABLE_GSM_NET
// GPRS Function //
#define STR_AS_SIM_PIN "$AS+SIM PIN=" // Reserved
#define STR_AS_GSM_IMEI "$AS+GPRS IMEI="
#define STR_AS_GSM_IMSI "$AS+GPRS IMSI="
#define STR_AS_GSM_CSQ "$AS+GPRS CSQ="
#define STR_AS_GSM_APN "$AS+GPRS APN="
#define STR_AS_GSM_MODE "$AS+GPRS MODE="
#define STR_AS_GSM_SERVER_IP "$AS+GPRS SERVER IP="
#define STR_AS_GSM_SERVER_NAME "$AS+GPRS SERVER NAME="
#define STR_AS_GSM_HEADER "$AS+GPRS HEADER="
// #define STR_AS_GSM_LOG                   "$AS+GPRS LOG="
// #define STR_AS_GSM_PKG_EN                "$AS+GPRS PKG EN="
// #define STR_AS_GSM_SEND_NORMAL		   	   "$AS+GPRS SEND NORMAL="
// #define STR_AS_GSM_SEND_STANDBY		   	   "$AS+GPRS SEND STANDBY="
// #define STR_AS_GSM_SERVER_INFO           "$AS+GPRS SERVER INFO="
#define STR_AS_GET_DATALOG "$AS+GET DATALOG="
#define STR_AS_GET_EVENT "$AS+GET EVENT="
#define STR_AS_GSM_MULTI_IP "$AS+GPRS MULTI IP="
#define STR_AS_GSM_MULTI_DNS "$AS+GPRS MULTI DNS="
#define STR_AS_GSM_MULTI_HEADER "$AS+GPRS MULTI HEADER="
#define STR_AS_GSM_SOCKET_EXT "$AS+GPRS EXT="
// #define STR_AS_GSM_MULTI_EN              "$AS+GPRS MULTI EN="
// #define STR_AS_GSM_MULTI_SEND 		   	   "$AS+GPRS MULTI SEND="
// #define STR_AS_GSM_MULTI_INFO            "$AS+GPRS MULTI INFO="
// #define STR_AS_GSM_LOG_EXT             	 "$AS+GPRS LOG EXT="
#define STR_AS_GET_DATALOG_EXT "$AS+GET DATALOG EXT="
#define STR_AS_GET_EVENT_EXT "$AS+GET EVENT EXT="
#define STR_AS_STATUS_CHECK "$AS+GPRS CHK="
#define STR_AS_GSM_FTP "$AS+GPRS FTP="
#define STR_AS_GSM_FTP_EXT "$AS+GPRS FTP EXT="
#define STR_AS_FTP_UPLOAD "$AS+FTP UPLOAD="
#define STR_AS_FTP_UPLOAD_EXT "$AS+FTP UPLOAD EXT="
#define STR_AS_FTP_DOWNLOAD "$AS+FTP DOWNLOAD="
#define STR_AS_FTP_DOWNLOAD_EXT "$AS+FTP DOWNLOAD EXT="
#define STR_AS_EPO_CHECK "$AS+EPO CHK="
#define STR_AS_EPO_DOWNLOAD "$AS+EPO DOWNLOAD="
#define STR_AS_EPO_UPDATE "$AS+EPO UPDATE="
#define STR_AS_XTRA_DOWNLOAD "$AS+XTRA DOWNLOAD="
#define STR_AS_XTRA_UPDATE "$AS+EPO UPDATE="
#define STR_AS_FTP_VDR_SUBDIR "$AS+VDR SUBDIR="
#define STR_AS_SMS_LIST "$AS+SMS LIST="
#define STR_AS_GSM_IOT_SIM "$AS+GPRS IOT SIM="
#define STR_AS_SOS "$AS+SOS=" // Reserved
#endif                        // #if ENABLE_GSM_NET

// System Control //
#define STR_AS_SYSTEM "$AS+SYS="
// #define STR_AS_INST_TIME			           "$AS+INST TIME="
#define STR_AS_RESET "$AS+RESET="
// #define STR_AS_REBOOT				             "$AS+REBOOT="
// #define STR_AS_REDEFAULT			           "$AS+RESET DEFAULT="

// External wakeup setting //
#define STR_AS_EX_ALM "$AS+EX ALARM="
#define STR_AS_EX_PED_WKUP "$AS+EX WAKEUP="
#define STR_AS_ALM "$AS+ALARM="
#define STR_AS_PED_WKUP "$AS+WAKEUP="

// Data File //
#define STR_AS_USB_DOWNLOAD "$AS+USB DOWNLOAD="
#define STR_AS_FIL_LIST "$AS+FILE LIST="
#define STR_AS_FORMAT "$AS+FORMAT="

// Debug //
#define STR_AS_DEBUG_MODE "$AS+DEBUG="

// Record //
#define STR_AS_RECORD_MODE "$AS+RECORD="

// USB Config //
#define STR_CONFIG_AUTH "$CONFIG+AUTH="
#define STR_CONFIG_GET_LOG "$CONFIG+GET LOG="
#define STR_CONFIG_DRIVER "$CONFIG+DRIVER="
#define STE_CONFIG_ENG_MODE "$CONFIG+ENG MODE="

// Multi-Parameter Set
#define STR_MULTI_SET "$AS+MULTI SET="

// Save Backup Setting
#define STR_AS_SAVE_SETTING "$AS+SAVE SETTING="

// Delete Backup Setting
#define STR_AS_BKP_SETTING_DEL "$AS+DEL SETTING="

// Save backup setting on FTP server
#define STR_AS_SETTING_FTP_UPLOAD "$AS+UPLOAD SETTING="

// Restore backup setting from FTP server
#define STR_AS_SETTING_FTP_DOWNLOAD "$AS+DOWNLOAD SETTING="

// Set RDP
#define STR_AS_RDP_MODE "$AS+RDP MODE="

// Clear RDP
#define STR_AS_CLEAR_RDP "$AS+CLR RDP="

// Set WRP
#define STR_AS_WRP_MODE "$AS+WRP MODE="

// Clear WRP
#define STR_AS_CLEAR_WRP "$AS+CLR WRP="

// LCD Saver
#define STR_AS_LCD_SAVER "$AS+LCD SAVER="

// BUS ID SETTING, DETAUL -> IMEI
#define STR_AS_BUSID_SET "$AS+BUS ID SET="

// VDR Now
#define STR_AS_VDR_NOW "$AS+VDR NOW="

// XTRA file URL
#define STR_AS_XTRA_URL "$AS+XTRA URL="

// EPO file URL
#define STR_AS_EPO_URL "$AS+EPO URL="

// Key reset enable
#define STR_AS_KEY_RST "$AS+KEY RST="

// Gary's Test Commands //
#define STR_AS_ALL_CMD "$AS+ALL CMD="
#define STR_AS_DRIVE_LIST "$AS+DRIVE LIST="
#define STR_AS_PARAMETER "$AS+PARA="
#define STR_AS_DRIVE_STATUS "$AS+DRIVE STATUS="
#define STR_AS_TEST_RS232 "$AS+TEST RS232="
#define STR_AS_TEST_BL "$AS+TEST BL="
#define STR_AS_TEST_MODE "$AS+TEST MODE="
#define STR_AS_GPS_CMD "$AS+GPS CMD="
#define STR_AS_CHECK_HEADER "$AS+CHECK HEADER="
#define STR_AS_AUTOGAIN_PARA "$AS+AG PARA="

// Brian's Test Commands //
#define STR_AS_RINGBUF_RESET "$AS+RINGBUF RESET="
#define STR_AS_UART_INT "$AS+UART INT="
#define STR_AS_PRINTF_EN "$AS+PRINTF="
#define STR_AS_FIL_RENAME "$AS+FILE RENAME="
#define STR_AS_FIL_SIZE "$AS+FILE SIZE="
#define STR_AS_FIND_FILE "$AS+FIND FILE="
#define STR_AS_FIL_DEL "$AS+FILE DEL="
#define STR_AS_DIR_DEL "$AS+DIR DEL="
#define STR_AS_VDR_DEL "$AS+VDR DEL="
#define STR_AS_DTR_DEL "$AS+DTR DEL="
#define STR_AS_VDR_MAKING "$AS+MAKE VDR="
#define STR_AS_EVENT "$AS+EVENT="
#define STR_AS_CALL "$AS+CALL="
#define STR_AS_SEND_SMS "$AS+SEND SMS="
#define STR_AS_AT_CMD "$AS+AT CMD="
#define STR_AS_TASK_RAM "$AS+TASK RAM="
#define STR_AS_TASK_STEP "$AS+TASK STEP="
#define STR_AS_TASK_DELAY "$AS+TASK DELAY="
#define STR_AS_USB_READ "$AS+USB READ="
#define STR_AS_SHOW_LCD "$AS+SHOW LCD="
#define STR_AS_4G_PWR "$AS+4G PWR="
#define STR_AS_4G_RST "$AS+4G RESET="
#define STR_AS_PRINT_ITEM "$AS+PRINT="
#define STR_AS_SLEEP "$AS+SLEEP="
#define STR_AS_8051_WDT_EN "$AS+MG82 WDT="
#define STR_AS_8051_WDT_TIME "$AS+MG82 WDT TIME="

// James CMD //
#define STR_AS_MCU_CRASH "$AS+MCU CRASH="
#define STR_AS_8051_USBPROTECT_SET "$AS+MCU USB PROTECT="

// G-sensor LSM6DSL function
// Wakup detection
#define STR_AS_LSM6DSL_GSENSOR_WKUP_SETTING "$AS+GSENSOR WKUP SETTING="
#define STR_AS_LSM6DSL_GSENSOR_AWTILT_SETTING "$AS+GSENSOR AWTILT SETTING="
#define STR_AS_LSM6DSL_GSENSOR_DISABLE "$AS+GSENSOR DISABLE="
#define STR_AS_LSM6DSL_GSENSOR_SET_CURRENT_ORIENTATION "$AS+GSENSOR SET CURRENT ORIENTATION="
//============================================================================//
// User Typedef Variables
//============================================================================//
typedef enum
{
    PWR_SAVE,
    PWR_IMMO,
    PWR_ON,
    PWR_OFF,
    PWR_END,
} PowerModeTypeDef;

typedef enum
{
    OTG_AUTO,
    OTG_HOST,
    OTG_DEVICE,
    OTG_END,
} OtgModeTypeDef;

typedef enum
{
    DBG_OFF = 0,
    DBG_TIME,
    DBG_DATA,
    DBG_LOG,
    DBG_GPS,
    DBG_GPS1,
    DBG_GSM,
    DBG_CAN,
    DBG_DB9,
    DBG_EXT,
    DBG_GS,
    DBG_ENG,
    DBG_RESERVED1,
    DBG_RESERVED2,
    DBG_RESERVED3,
    DBG_ALL,
} DebugModeTypeDef;

typedef enum
{
    REC_OFF = 0,
    REC_SPD_INT,
    REC_CAN_DBG,
    REC_PULSE_CNT,
    REC_GPS_SPD,
    REC_ALL
} RecordModeTypeDef;

typedef enum
{
    RST_SYS,
    RST_GPS,
    RST_GSM,
    RST_CAN,
    RST_LOG,
    RST_TEMP,
    RST_ALL,
    RST_DEFAULT,
    RST_UPGRADE,
    RST_SETTING,
    RST_END,
} ResetCmdTypeDef;

typedef enum
{
    SYS_UPGRADE_NOW,
    SYS_UPGRADE,
    SYS_BACKUP,
    SYS_PHOTO,
    SYS_INFO,
    SYS_END,
} SystemCmdTypeDef;

// Heinz
typedef enum
{
    CARTYPE_AUTO_GAIN = 0U,
    CARTYPE_DAWOO_2012,
    CARTYPE_DAWOO_2018,
    CARTYPE_HINO_500,
    CARTYPE_HINO_700,
    CARTYPE_HINO_SH1EESA,
    CARTYPE_HINO_COASTER,
    CARTYPE_FUSO_1841LSE_2015,
    CARTYPE_FUSO_2016,
    CARTYPE_FUSO_FM173,
    CARTYPE_FUSO_ROSA_BUS,
    CARTYPE_UD,
    CARTYPE_UD_PKC8YLS,
    CARTYPE_DAF_280,
    CARTYPE_DAF_360,
    CARTYPE_DAF_380_AUTO,
    CARTYPE_DAF_380_HAND,
    CARTYPE_DAF_410,
    CARTYPE_DAF_460,
    CARTYPE_SITRAK,
    CARTYPE_SCANIA_K380,
    CARTYPE_SCANIA_K400,
    CARTYPE_VOLVO_FM4B,
    CARTYPE_VOLVO_FM,
    CARTYPE_BENZ_O500,
    CARTYPE_BENZ_2046S,
    CARTYPE_BENZ_2545LS,
    CARTYPE_BENZ_2553LS,
    CARTYPE_BENZ_2653LS,
    CARTYPE_BENZ_430PS,
    CARTYPE_BENZ_1846LS,
    CARTYPE_BENZ_2645L,
    CARTYPE_BENZ_2651L,
    CARTYPE_BENZ_2545LS_6,
    CARTYPE_BENZ_2553LS_6,
    CARTYPE_BENZ_1840LS,
    CARTYPE_BENZ_2640L,
    CARTYPE_BENZ_2540L,
    CARTYPE_MAN_EURO5,
    CARTYPE_MAN_EURO6,
    CARTYPE_RAC,
    CARTYPE_YUTONG_2014,
    CARTYPE_DF_KL465,
    CARTYPE_DF_KL480,
    CARTYPE_SCANIA_K450,
    CARTYPE_SCANIA_K410,
    CARTYPE_SCANIA_P410,
    CARTYPE_SCANIA_G500,
    CARTYPE_SCANIA_P370,
    CARTYPE_SCANIA_P450,
    CARTYPE_SCANIA_P280,
    CARTYPE_SCANIA_P320,
    CARTYPE_SCANIA_R660,
    CARTYPE_VOLVO_2_85,
    CARTYPE_VOLVO_3_08,
    CARTYPE_ISUZU_series_N,
    CARTYPE_ISUZU_series_F,
    CARTYPE_DAF_430,
    CARTYPE_VOLVO_3_4,
    CARTYPE_VOLVO_4_13,
    CARTYPE_BENZ_OC500,
    CARTYPE_SHUCHI,
    CARTYPE_VOLVO_4_5,
    CARTYPE_VOLVO_5_14,
    CARTYPE_VOLVO_5_67,
    CARTYPE_END,
} CarTypeTypeDef;

#pragma pack(1)
typedef struct
{
    char *cCarTypeName;

    uint16_t u16CarTypeGain;

} SetCarTypeTypeDef;
#pragma pack()

//============================================================================//
// User Externs
//============================================================================//
extern const char *szResetCmd[];

extern const uint32_t DebugModeValue[];

extern const uint32_t RecordModeValue[];

extern const char *szCanLinkType[];

// extern const char* szCarType[];	// Heinz

// extern const uint16_t u16CarTypeGain[];

extern const uint8_t u8CarTypeIndex[];

extern const SetCarTypeTypeDef SetCarType[];

//============================================================================//
// Public functions
//============================================================================//
bool Protocl_ChkCmdPkg(char *pcCmdPkg, char *pcCmdMsg);
bool CMD_AllCmd(char *pStr);
bool Protocl_CommandCheck_Proc(void);
bool Protocl_IsRstGSM_Cmd(char *pcCmdStr, char *pcCmdArg);

//============================================================================//
#endif //_PROTOCOL_H_
