#ifndef _SYSTEM_H_
#define _SYSTEM_H_
//============================================================================//

//============================================================================//
// User Includes
//============================================================================//
#include <stdint.h>
#include <time.h>

#include "CAN_Driver.h"
#include "Config.h"
#include "cmsis_os.h"
#include "Default.h"
#include "GPS_Driver.h"
#include "GSM_Driver.h"
#include "Protocol.h"
#include "UART_Driver.h"

//============================================================================//
// User Defines
//============================================================================//

// Offset address of product information
// #define	PRODUCT_INFO_OFFSET		           0x2000

// Actual address of product information
// #define	PRODUCT_INFO_ADDR		             (APPLICATION_ADDR + PRODUCT_INFO_OFFSET)

// Actual address of system setup
// #define	SETUP_BACKUP_ADDR		             (FLASH_END_ADDR - SECTOR_128K_SIZE)
// Number of active tasks
#define TASK_QUANTITY 10

// Temporary files folder
#define DIRNAME_TEMP "TEMP" // Folder to store files for temporary packets

// Setting files folder
#define DIRNAME_SETTING "SETTING" // Folder to store system settings and backup settings

#define FILENAME_G_SETTING "G_SETTING.SYS" // File to store g_Setting

#define FILENAME_BACKUP_SETTING "BACKUP_SETTING.SYS" // File to store backup setting

#define FILENAME_BACKUP_SETTING_TEMP "BACKUP_SETTING_TEMP.SYS" // Temp file to store backup setting

#define BACKUP_SETTING_FTP_EXT "SETTING" // Extension of backup setting file on FTP server

// File system log name
#define FILENAME_SYSLOG "SYSLOG" // File name for system log

// File data log name
// #define	FILENAME_DATALOG_DTR	           "DATALOG.DTR"

// File photo name
// #define	FILENAME_PHOTO_JPG  	           "PHOTO.JPG"

// DFU file name
#define FILENAME_UPGRADE_DFU "UPGRADE.DFU"

// Upgrade file name
// #define FILENAME_UPGRADE_BIN	           "UPGRADE.BIN"

#define UPGRADE_FILE_EXT "DF2" // Upgrade file name extension

// DFU file name after finish upgrade
// #define FILENAME_UPGRADE_FINISH	         "UPGRADE"

#define TEMP_FILE_EXT "TMP"

#define CHINESE_FONT_FILE "STDFONT.16" // Name of Chinese font file for LCD display

#define FILENAME_SETUP_TXT "SETUP.TXT"

#define FILENAME_CONFIG_SYS "CONFIG.SYS"

#define STR_AUTH_ROOT "ROOT"

#define STR_AUTH_FACTORY "FACTORY"

#define STR_AUTH_ADMIN "ADMIN"

#define STR_AUTH_USER "USER"

#define GET_DATALOG_ADMIN_DEF 7

#define GET_DATALOG_USER_DEF 2

#define GET_DATALOG_ROOT_DEF 1

#define GET_DATALOG_ADMIN_MAX 30

#define GET_DATALOG_USER_MAX 7

#define GET_DATALOG_ROOT_MAX DATALOG_FILE_MAX

#define SYSTICK_COUNT_DEF 1000000U

// System tick timebase default is 1mS
#define SYSTICK_TIMEBASE_DEF 1000U

#define SYSTICK_CLOCK (SYSTICK_COUNT_DEF / SYSTICK_TIMEBASE_DEF)

#define SysTick_4_mS (SYSTICK_CLOCK / 1000) // Systick-milliseconds equivalence

#define mS_2_SysTick(mS) ((mS) * SysTick_4_mS) // Convert milliseconds to Systick

#define SysTick_2_mS(SysTick) ((SysTick) / SysTick_4_mS) // Convert Systick to milliseconds

#define SETUP_VERSION 0x02

#define NOT_SETUP_VERSION 0xFD

#define BACKUP_SETUP_VERSION 0xAB

#define DEFAULT_STRING "------"

#define DRIVER_INFO_MAX 10U

#define INVALID_VALUE_DEF 0xFFFFFFFF

#define SPEED_LIMIT_BANK_MAX 3U

#define DISTANCE_MAX 2U // Heinz

#define PHONE_BANK_MAX 10U

#define DRV_QUANTITY_MAX 2U

#define RESET_DELAY_NULL 0U

#define RESET_DELAY_SHORT 1U

#define RESET_DELAY_NORMAL 3U

#define RESET_DELAY_LONG 5U

#define FUNC_KEY_SPEED_LIMIT_GROUP 3U

#define FUNC_KEY_PHONE_BANK_MAX 3U

#define UPSPEED_HOW_LONG_SEC 30U

#define SAVE_RPM_TIME 30U

#define STR_SYSTEM_LOG "\r\n> [SYS]"

// System state code list
#define SYS_STATE_SYSTEM_INIT_OK 0x3030     // 0x0000
#define SYS_STATE_SYSTEM_UPGRADE_OK 0x3130  // 0x0001
#define SYS_STATE_SETUP_RECOVERY 0x3230     // 0x0002
#define SYS_STATE_DATALOG_SAVE_ERROR 0x3330 // 0x0003
#define SYS_STATE_SYS_DEFAULT 0x3430        // 0x0004
#define SYS_STATE_SYS_INIT 0x3530           // 0x0005
#define SYS_STATE_SYS_RENAME1 0x3730        // 0x0007
#define SYS_STATE_SYS_RENAME2 0x3830        // 0x0007
#define SYS_STATE_SD_WRITE_ERR 0x3930       // 0x0009

#define SYS_STATE_MOTOR_POWER_ON 0x3031   // 0x0010
#define SYS_STATE_MOTOR_POWER_DOWN 0x3131 // 0x0011
#define SYS_STATE_KEY_ON 0x3231           // 0x0012
#define SYS_STATE_KEY_OFF 0x3431          // 0x0014
#define SYS_STATE_RTC_SYNC_OK 0x3531      // 0x0015

#define SYS_STATE_GPS_POWER_ON 0x3032       // 0x0020
#define SYS_STATE_GPS_POWER_OFF 0x3132      // 0x0021
#define SYS_STATE_GPS_FIX 0x3232            // 0x0022
#define SYS_STATE_GPS_NOT_FIX 0x3332        // 0x0023
#define SYS_STATE_GPS_PACKAGE_ERROR 0x3432  // 0x0024
#define SYS_STATE_GPS_POSITION_ERROR 0x3532 // 0x0025
#define SYS_STATE_GPS_NMEA_ERROR 0x3632     // 0x0026
#define SYS_STATE_GPS_NMEA_QUALITY 0x3732   // 0x0027

#define SYS_STATE_GSM_POWER_ON 0x3033     // 0x0030
#define SYS_STATE_GSM_POWER_OFF 0x3133    // 0x0031
#define SYS_STATE_GSM_NET_RELEASE 0x3233  // 0X0032
#define SYS_STATE_GSM_NET_CONNECT 0x3333  // 0x0033
#define SYS_STATE_GSM_NET_DISCONN 0x3433  // 0x0034
#define SYS_STATE_GSM_AT_CMD_ERROR 0x3533 // 0x0035
#define SYS_STATE_GSM_AT_CMD_RESET 0x3633 // 0x0036

#define SYS_STATE_USB_DL_DAY 0x3034       // 0x0040
#define SYS_STATE_USB_DL_HOUR 0x3134      // 0x0041
#define SYS_STATE_UART_DL_DAY 0x3234      // 0X0042
#define SYS_STATE_UART_DL_HOUR 0x3334     // 0x0043
#define SYS_STATE_CHANG_BUS_ID 0X3434     // 0x0044
#define SYS_STATE_CHANG_SPEED_GAIN 0x3534 // 0x0045
#define SYS_STATE_CHANG_RPM_GAIN 0x3634   // 0x0046
#define SYS_STATE_CHANG_DRV_ID 0x3734     // 0x0047
#define SYS_STATE_CHANG_DISTANCE 0x3834   // 0x0048

#define SYS_STATE_GPS_AUTO_TEST 0x3035    // 0x0050
#define SYS_STATE_GPS_AUTO_TEST_OK 0x3135 // 0x0051
#define SYS_STATE_GPS_NG 0x3235           // 0x0052
#define SYS_STATE_PTR_AUTO_TEST 0x3335    // 0x0053
#define SYS_STATE_PTR_AUTO_TEST_OK 0x3435 // 0x0054
#define SYS_STATE_PTR_NG 0x3535           // 0x0055
#define SYS_STATE_LCM_AUTO_TEST 0x3635    // 0x0056
#define SYS_STATE_LCM_AUTO_TEST_OK 0x3735 // 0x0057
#define SYS_STATE_LCM_NG 0x3835           // 0x0058
#define SYS_STATE_CALIBRATION_ON 0x3935   // 0x0059
#define SYS_STATE_CALIBRATION_OFF 0x4135  // 0x005A
#define SYS_STATE_BATTERY_ON 0x4235       // 0x005B
#define SYS_STATE_BATTERY_OFF 0x4335      // 0x005C
#define SYS_STATE_NON_AUTH 0x4435         // 0x005D
#define SYS_STATE_NON_CAL 0x4535          // 0x005E

#define SYS_STATE_POWER_OFF_200MS 0x3036 // 0x0060
#define SYS_STATE_ADMIN_ON 0x3136        // 0x0061
#define SYS_STATE_ADMIN_OFF 0x3236       // 0x0062

#define SYS_STATE_AUTO_GAIN_STAR 0x3039 // 0x0090
#define SYS_STATE_AUTO_GAIN_END 0x3139  // 0x0091
#define SYS_STATE_AUTO_GAIN_OK 0x3239   // 0x0092
#define SYS_STATE_DMD_ERROR 0x3939      // 0x0099

#define SYS_STATE_MAKE_VDR_START 0x3041
#define SYS_STATE_MAKE_VDR_STOP 0x3141
#define SYS_STATE_RENAME_DAY_FILE 0x3241
#define SYS_STATE_RECREATE_DAY_FILE 0x3341
#define SYS_STATE_CHECK_RECORD_FILE 0x3441
#define SYS_STATE_PCF8563T_SYNC_OK 0x3541
#define SYS_STATE_DATALOG_HEAD_NG 0x3641
#define SYS_STATE_DATALOG_READ_NG 0x3741
#define SYS_STATE_DATALOG_FILE_NG 0x3841

// #define SYS_STATE_LOG_IN	        	     0x45303032 //002
// #define SYS_STATE_LOG_OUT	        	     0x45303033 //003

// Upgrade file name
#define FILENAME_UPGRADE_BIN "UPGRADE.BIN"

#define FLASH_CRC_SIZE 4U

#define ENABLE_CRC_CHECK 1U

#define FLASH_HEADER_SIZE 24U

// JASSLIN EVENT DEFINE
#define STR_EVENT_DRIVER_ID "Drv_ID"
#define STR_EVENT_DRIVER_ID0 "Drv_ID0"
#define STR_EVENT_DRIVER_ID1 "Drv_ID1"
#define STR_EVENT_TRAILER_ID "Tra_ID"
#define STR_EVENT_G_IMM "G_Imm"
#define STR_EVENT_IMM_ENABLE "Imm_enable"
#define STR_EVENT_IMM_DISABLE "Imm_disable"
#define STR_EVENT_GEO_ENABLE "Geo_enable"
#define STR_EVENT_GEO_DISABLE "Geo_disable"
#define STR_EVENT_THERMO "Tmp"
#define STR_EVENT_MSG "MSG"
#define STR_EVENT_BARCODE "BARCODE"
#define STR_EVENT_PING "PING"
#define STR_EVENT_PWR_DOWN "PWR_DOWN"
#define STR_EVENT_FIRST_PWR_ON "FIRST_PWR_ON"
#define STR_EVENT_PWR_ON "PWR_ON"
#define STR_EVENT_KEY_ON "KEY_ON"
#define STR_EVENT_KEY_OFF "KEY_OFF"
#define STR_EVENT_PARK_ALM_EN "Park_Alarm_enable"
#define STR_EVENT_PARK_ALM_DIS "Park_Alarm_disable"
#define STR_EVENT_SOS "SOS"
#define STR_EVENT_KEYIN "KEYIN"
#define STR_EVENT_UPGRADE_OK "UPGRADE OK"
#define STR_EVENT_SD_ERROR "SD ERROR"
#define STR_EVENT_BATTERY_ERROR "BAT ERROR"
#define STR_EVENT_UPGRADE_START "UPGRADE ERROR"
#define STR_EVENT_DELETE_FILE "DEL FILE ERROR"
#define STR_EVENT_SD_FORMAT "SD FORMAT"
#define STR_EVENT_ARGUMENT_SMS "SMS"
#define STR_EVENT_SPEED_LINE_OK "SPEED LINE OK" // Heinz

#if CUSTOMER_TYPE == CUSTOMER_SKYEYE
// �Ѳ��ìP��޸�Ʃw�q
#define SKYEYE_EVENT_NORMAL "000"        // �w�ɦ^�Ǫ��A
#define SKYEYE_EVENT_LOGIN "001"         // �r�p�n�J
#define SKYEYE_EVENT_LOGOUT "002"        // �r�p�n�X
#define SKYEYE_EVENT_SOS "003"           // SOS �����s
#define SKYEYE_EVENT_UPGRADE_OK "004"    // �t�Ϊ�����s����
#define SKYEYE_EVENT_PING "005"          //
#define SKYEYE_EVENT_RS232_PD "006"      // RS232�P��AT$PD�R�O
#define SKYEYE_EVENT_RS232_CODE "007"    // RS232�P��AT$CODE�R�O
#define SKYEYE_EVENT_BATTERY_ERROR "008" // �����q�����`
#define SKYEYE_EVENT_UPGRADE_START "009" // �}�l��s�y�{
#define SKYEYE_EVENT_DELETE_FILE "010"   // �R���ɮ�
#define SKYEYE_EVENT_SD_FORMAT "011"     // SD�d�榡��
#define SKYEYE_EVENT_KEY_ON "100"        //
#define SKYEYE_EVENT_KEY_OFF "100"       //
#define SKYEYE_EVENT_IO "100"            //
#define SKYEYE_EVENT_IO1 "101"           //
#define SKYEYE_EVENT_IO2 "102"           //
#define SKYEYE_EVENT_IO3 "103"           //
#define SKYEYE_EVENT_IO4 "104"           //
#define SKYEYE_EVENT_IO5 "105"           //
#define SKYEYE_EVENT_IO6 "106"           //
#define SKYEYE_EVENT_IO7 "105"           //
#define SKYEYE_EVENT_IO8 "106"           //
#define SKYEYE_EVENT_SPEED "300"         //
#endif

#if CUSTOMER_TYPE == CUSTOMER_FPTC
// �x��f�B�ƥ�w�q
#define FPTC_EVENT_NORMAL "000"   // �w�ɦ^�Ǫ��A
#define FPTC_EVENT_SOS "065"      // SOS button
#define FPTC_EVENT_ENG_ON "066"   // �����_��
#define FPTC_EVENT_PWR_ON "067"   // �����}��(�}�l�ǰe�w�ɦ^��)
#define FPTC_EVENT_ENG_OFF "068"  // ��������
#define FPTC_EVENT_PWR_DOWN "079" // ��������(����������@�p�ɫ�ʧ@�ä��A�ǰe�w�ɦ^��)
#define FPTC_EVENT_PWR_LOST "500" // �L�~���q���A�Y�q���ѹq

#define FPTC_EVENT_100 "100" // �����֧@��
#define FPTC_EVENT_101 "101" // ���L�������O���` : ���ި�
#define FPTC_EVENT_102 "102" // ���L�����u�����` : ���ި�
#define FPTC_EVENT_110 "110" // ���L�����ūײ��` : ���ި�
#define FPTC_EVENT_201 "201" // ���L�������O���` : ����
#define FPTC_EVENT_202 "202" // ���L�����u�����` : ����
#define FPTC_EVENT_210 "210" // ���L�����ūײ��` : ����
#define FPTC_EVENT_700 "700" // �����}��
#define FPTC_EVENT_701 "701" // ��������
#define FPTC_EVENT_702 "702" // �����s��
#define FPTC_EVENT_703 "703" // ����������
#define FPTC_EVENT_704 "704" // ����p�}��
#define FPTC_EVENT_705 "705" // ����p����
#define FPTC_EVENT_710 "710" // �w�ɦ^��ĵ���Ұ�
#define FPTC_EVENT_711 "711" // �w�ɦ^��ĵ���Ѱ�
#define FPTC_EVENT_800 "800" // ���楼��U
#define FPTC_EVENT_801 "801" // ����w��U

#define MBEYE_EVENT_900 "900" // �I��ĵ�i
#define MBEYE_EVENT_910 "910" // �k�����D
#define MBEYE_EVENT_911 "911" // �������D
#define MBEYE_EVENT_912 "912" // �Y�����D����
#define MBEYE_EVENT_913 "913" // ���Y�B��ĵ�i
#define MBEYE_EVENT_914 "914" // ���Y����B��ĵ�i
#define MBEYE_EVENT_920 "920" // ���Zĵ�iLV1
#define MBEYE_EVENT_921 "921" // ���Zĵ�iLV2
#define MBEYE_EVENT_922 "922" // �M�I��
#endif

#if CUSTOMER_TYPE == CUSTOMER_CPTI
// CPTI  �����ȹB+�x�_�ȹB
#define CPTI_EVENT_LDWS "LDWS" // �����t��
#define CPTI_EVENT_TPMS "TPMS" // �L���L��

// �H�U�n�Ωw�ɦ^�Ǯ榡�^��
#define CPTI_EVENT_PWR_DOWN "PWR_DOWN"  // PWR_DOWN(500)�G�~���q�������ƥ�.-ok
#define CPTI_EVENT_PWR_DOWN_VALUE "500" // PWR_DOWN(500)�G�~���q�������ƥ�.-ok

#define CPTI_EVENT_SPEED_L3 "OVER_SPEED" // OVER_RPM(74)�G�W�L�]�w��t�W��15��.
#define CPTI_EVENT_SPEED_L3_VALUE "71"   // OVER_RPM(74)�G�W�L�]�w��t�W��15��.

#define CPTI_EVENT_RPM_OVER "OVER_RPM" // OVER_SPEED(71)�G�W�L��3�q�t��ĵ��15��.
#define CPTI_EVENT_RPM_OVER_VALUE "74" // OVER_SPEED(71)�G�W�L��3�q�t��ĵ��15��.
#endif

#if ENABLE_GSM_NET
// System events that need to be reported to server through GSM

// 1.System
#define EVENT_KEY_OFF "100"
#define EVENT_KEY_ON "101"

#define EVENT_USB_DISCON "110"
#define EVENT_USB_CON "111"

#define EVENT_SYSTEM_OFF "120"
#define EVENT_SYSTEM_ON "121"
#define EVENT_SYSTEM_RESET "122"

#define EVENT_BRAKE_OFF "140"
#define EVENT_BRAKE_ON "141"

#define EVENT_LEFT_SIGNAL_OFF "150"
#define EVENT_LEFT_SIGNAL_ON "151"

#define EVENT_RIGHT_SIGNAL_OFF "160"
#define EVENT_RIGHT_SIGNAL_ON "161"

#define EVENT_FDOOR_OFF "170"
#define EVENT_FDOOR_ON "171"

#define EVENT_BDOOR_OFF "180"
#define EVENT_BDOOR_ON "181"

#define EVENT_HEADLIGHT_OFF "190"
#define EVENT_HEADLIGHT_ON "191"

// 2. IOs
#define EVENT_IO1_OFF "200"
#define EVENT_IO1_ON "201"

#define EVENT_IO2_OFF "210"
#define EVENT_IO2_ON "211"

#define EVENT_IO3_OFF "220"
#define EVENT_IO3_ON "221"

#define EVENT_IO4_OFF "230"
#define EVENT_IO4_ON "231"

#define EVENT_IO7_OFF "240"
#define EVENT_IO7_ON "241"

// 3. Speed
#define EVENT_OVER_SPEED_END "300"
#define EVENT_OVER_SPEED_START "301"

#define EVENT_OVER_ACCEL_START "311"
#define EVENT_OVER_DECEL_START "321"

#define EVENT_OVER_RPM_END "340"
#define EVENT_OVER_RPM_START "341"

#define EVENT_RPM_IDLE_END "350"
#define EVENT_RPM_IDLE_START "351"

#define EVENT_AUTOGAIN_ON "360"
#define EVENT_AUTOGAIN_LOOP1 "361"
#define EVENT_AUTOGAIN_LOOP2 "362"
#define EVENT_AUTOGAIN_OK "363"
#define EVENT_AUTOGAIN_NG "364"
#define EVENT_AUTOGAIN_OFF "365"
#define EVENT_AUTOGAIN_2Y "366"
#define EVENT_CHKGAIN_OK "367"

// 4. Sensors
#define EVENT_LDWS_ON "400"

#define EVENT_TIRE_TEMP_HIGH "410"
#define EVENT_TIRE_PRESS_HIGH "411"
#define EVENT_TIRE_PRESS_LOW "412"
#define EVENT_TIRE_SENSOR_POWER_LOW "413"
#define EVENT_TIRE_NO_SIGNAL "414"

// 5. Peripherals
#define EVENT_MDVR_VID_LOSS "500"
#define EVENT_MDVR_DISK_ERR "501"
#define EVENT_MDVR_GPS_ERR "502"

#define EVENT_SD_FAIL "511"
#define EVENT_MX25L_FAIL "512"
#define EVENT_I2C2_FAIL "513"
#define EVENT_GPS_MODULE_FAIL "514"
#define EVENT_GPS_NOT_VALID "515"
#define EVENT_GPS_NO_NMEA "516"
#define EVENT_GPS_CHANGE_BR "517"

#define EVENT_USB_DOWNLOAD_SUCCESS "520"
#define EVENT_USB_DOWNLOAD_FAIL "521"
#define EVENT_USB_DOWNLOAD_DISCONN "522"
#define EVENT_USB_HOLD_ON "523"
#define EVENT_USB_HOLD_OFF "524"

#define EVENT_EXTERNAL_POWER_ISSUE "531"

#define EVENT_SPEED_INIT "560"

#define EVENT_GSM_NO_PWR "591"
#define EVENT_GSM_NO_READY "592"

// 6.
#define EVENT_REALSPEED_LOSE "601"
#define EVENT_SPEED_ABNORMAL "602"

#define EVENT_SMS_RESPONSE "610"

#define EVENT_VDR_REQUEST "620"
#define EVENT_VDR_INIT "621"
#define EVENT_VDR_TIME_FAIL "622"
#define EVENT_VDR_MAKE_FAIL "623"
#define EVENT_VDR_NOW_REQUEST "624"
#define EVENT_VDR_NOW_INIT "625"
#define EVENT_VDR_NOW_FAIL "626"

#define EVENT_FOTA_FAIL "631"
#define EVENT_FTP_DOWNLOAD_OK "632"
#define EVENT_FTP_DOWNLOAD_FAIL "633"
#define EVENT_FTP_UPLOAD_BEGIN "634"
#define EVENT_FTP_UPLOAD_OK "635"
#define EVENT_FTP_UPLOAD_FAIL "636"
#define EVENT_UPLOAD_CHK_BEGIN "637"
#define EVENT_UPLOAD_CHK_OK "638"
#define EVENT_UPLOAD_CHK_FAIL "639"

#define EVENT_TCP_DNS_ERR "641"
#define EVENT_TCP_PARAM_ERR "642"
#define EVENT_FTP_DNS_ERR "643"
#define EVENT_FTP_PARAM_ERR "644"
#define EVENT_UPLOAD_CHK_FULL "645"
#define EVENT_UPLOAD_CHK_INIT_ERR "646"
#define EVENT_GPS_XTRA_FAIL "647"
#define EVENT_SMS_PKG_FULL "648"
#define EVENT_VDR_PKG_FULL "649"

#define EVENT_RESTORE_SETTING_OK "650"
#define EVENT_RESTORE_SETTING_FAIL "651"

#define EVENT_LOG_TIME_ERR "661"
#define EVENT_LOG_TIME_ERR_2 "662"

#define EVENT_KGAIN_CHANGE "663"

// Gsensor event
#define EVENT_G_SENSOR_WAKEUP "700"
#define EVENT_G_SENSOR_TILT_CHANGE "701"
#endif

#define STR_MSG_ON "ON"
#define STR_MSG_OFF "OFF"

#define HARDFAULT_EVENT_CODE 0xA1
#define TASK_HANG_EVENT_CODE 0xA2
#define SD_ERROR_EVENT_CODE 0xA3

//============================================================================//
// User Typedef Variables
//============================================================================//
#pragma pack(1)
typedef enum
{
    SYSTEM_MODE_NORMAL = 0U,

    SYSTEM_MODE_STEALTH

} SystemModeTypeDef;

typedef enum
{
    PWR_DOWN_STANDBY = 0U,

    PWR_DOWN_SHUTDOWN

} PwrDownModeTypeDef;

typedef enum
{
    TASK_LOG_PROC = 0,

    TASK_EXTIO_PROC,

    TASK_GPS_PROC,

    TASK_GSM_PROC,

    TASK_LCD_KEY,

    TASK_RS232_PROC,

    TASK_HIGH_LOAD,

    TASK_SD_PROC,

    TASK_GSM_RX,

    TASK_GSM_TX

} TaskTagTypeDef;

typedef enum
{
    LANG_CHN = 0U,

    LANG_ENG

} SelectLangTypeDef;

typedef enum
{
    VAR_DISTANCE = 0U,
    VAR_BUSID,
    VAR_VIN,
    VAR_SPEED_GAIN,
    VAR_RPM_DIV,
    VAR_CAN_LINK,
    VAR_CAN_SPEED,
    VAR_CAN_RPM,
    VAR_GPRS_SERVER_IP,
    VAR_GPRS_SERVER_PORT,
    VAR_DRIVER_ID1,
    VAR_DRIVER_ID2,
    VAR_SPEED_LIMIT1,
    VAR_SPEED_LIMIT2,
    VAR_SPEED_LIMIT3,
    VAR_CAL_TIME,
    VAR_GPRS_APN,
    VAR_GPRS_DNS,
    VAR_GPRS_SERVER_IP_2,
    VAR_GPRS_SERVER_PORT_2,
    VAR_GPRS_DNS_2,
    VAR_GPRS_FTP_ADDR,
    VAR_GPRS_FTP_PORT,
    VAR_GPRS_FTP_USERNAME,
    VAR_GPRS_FTP_PASSWORD,
    VAR_GPRS_FTP_ADDR_2,
    VAR_GPRS_FTP_PORT_2,
    VAR_GPRS_FTP_USERNAME_2,
    VAR_GPRS_FTP_PASSWORD_2,
    VAR_FTP_VDR_PATH,
    VAR_FTP_VDR_DATE_EN,
    VAR_LCD_SAVER,
    VAR_VDR_NOW,
    VAR_DB9_FUNC,
    VAR_XTRA_URL,
    VAR_EPO_URL,
    VAR_SOCKET_EXT,
    VAR_4G_WKUP,
    VAR_EX_ALM,
    VAR_EX_PED_WKUP,
    VAR_PWR_DOWN,
    VAR_KGAIN,
    VAR_GAIN_CHK_DIS,
    VAR_ALL_ITEMS

} SettingItemTypeDef;

typedef struct
{
    bool bTaskState;

    osThreadId taskID;

    osThreadDef_t tTaskDef;

    char cTaskName[20];

    uint32_t u32TaskTimeout;

    uint32_t u32LastTick;

    uint16_t u16TaskStep;

    bool bTaskDelay;

    // bool bTaskCheck;

} TaskInfoTypeDef;

typedef struct
{
    char cBus_ID[11]; // Device ID

    char cBus_SN[11]; // Device serial number

    char cVIN[21]; // Vehicle VIN number

    CarTypeTypeDef tCarType; // Vehicle type

} BusSetTypeDef;

// #if ENABLE_GSM_NET
//  Phonebook
typedef struct
{
    char cLabel[16]; // Phone name

    char cNumber[16]; // Phone number

} PhoneBankTypeDef;

// Internet information
typedef struct
{
    // APN name, default:internet
    char cAPN[31];

    // User name, default:(blank)
    char cAPN_UserName[21];

    // Password, default:(blank)
    char cAPN_Password[21];

    // ��ƶǿ鶡�j�ɶ�, default: 60 seconds
    // uint16_t u16SendNormal;

    // ��v���A��ƶǿ鶡�j�ɶ�, default: 600(��)
    uint16_t u16SendStandby;

    uint8_t u8GSM_CSQ; /// Heinz

    uint8_t u8Reserve[8];

} NetSetTypeDef;

// GSM module information
typedef struct
{
    // International Mobile Subscriber Identity, ��ڲ���ñ�p�̼���
    char cIMSI[16];

    // International Mobile Equipment Identity, ��ڲ��ʳ]�Ƽ���
    char cIMEI[16];

} MobileInfoTypeDef;

// Server IP
typedef struct
{
    // IP address, XXX.XXX.XXX.XXX
    char cIP_Addr[20]; // IP address in string

    MEM_U32_t u32IP_Addr; // IP address in 4 bytes format

    // ���A�� address
    char cDomainName[31]; // Server domain name

    // Socket port, XXXXX
    uint16_t u16Port; // Destination port

    bool bEnableChk;
    bool bEnableLog;
    bool bEnableEvent;

    char cHeaderChk[10];
    char cHeaderLog[10];
    char cHeaderEvent[10];

    uint32_t u32ChkType;

    uint8_t u8LogPeriod;

    uint8_t u8EventType;

    uint8_t u8Reserve[8];

} ServerInfo_TypeDef;

// FTP setup
typedef struct
{
    // ���A�� address
    char cAddr[31]; // FTP server domain name or IP address

    // Socket port, XXXXX
    uint16_t u16Port; // Destination port

    // �ɮצ��A���ϥΪ̦W��
    char cUserName[21]; // FTP username

    // �ɮצ��A���ϥΪ̱K�X
    char cPassword[21]; // FTP user password

    uint8_t u8Reserve[8];

} FTP_SetTypeDef;

typedef struct
{
    bool bGSM_ModuleErr; // Is GSM module having issue?

    uint8_t u8Function_ErrCnt; // GSM module error count //
    uint8_t u8SIM_ErrCnt;      // GSM PS unregister number count //
    uint8_t u8PDP_ErrCnt;      // GSM PDP context activation related error count //
    uint8_t u8TCP_ErrCnt;      // TCP related error count
    uint8_t u8FTP_ErrCnt;      // FTP related error count
    uint8_t u8SMS_ErrCnt;      // Temporary message error count
    uint8_t u8GPS_ErrCnt;      // GPS function related error count
    uint8_t u8PkgErrCnt;       // Temporary packet error count

    uint8_t u8Reserve[8];

} GSM_ErrTypeDef;

// GSM setup
typedef struct
{
    // Sim PIN number
    char cSIM_PIN[5]; // SIM PIN // 5,

    char cSIM_PUK[9]; // SIM PUK 8 digits

    // ��X
    // char cCountryCode[5]; // Country code

    // Mobile info
    MobileInfoTypeDef tMobileInfo; // Mobile info

    // �w�]�q�ܸ��X
    // PhoneBankTypeDef tPhoneBank[PHONE_BANK_MAX]; // Phone number bank

    NetSetTypeDef tNetSet; // Internet setup

    FTP_SetTypeDef tFTP_Set[2]; // FTP setup

    ServerInfo_TypeDef tServerInfo[2]; // Set of server IP

    // ServerNameTypeDef	tServerName[2]; // Set of server name

    // ���J���W�q��, default: 10
    // uint8_t u8LevelMicGain; // Mic gain

    // �q���n���W�q��, default: 90
    // uint8_t u8LevelSpkGain; // Speaker gain

    // �ӹq�a�n�W�q��, default: 100
    // uint8_t u8LevelRingSound; // Ring sound level

    GSM_ErrTypeDef tGSM_Err;

    bool bDisableSocketEXT;

    bool b4G_WakeUp;

    uint32_t u32DelayTimeOut;

    uint8_t u8Reserve[11];

} GSM_SetTypeDef;
// #endif

// Speed determination setup
typedef struct
{
    // �t�׼W�q��, default: 0 (Disable)
    // �N���@�Ӯɳt�T���Ҧ樫���Z��, ��쬰����
    float fGain;

    // SPEED trigger level, default: 100 (= 1.00V)
    uint16_t u16Trig;

    // ���tĵ�ܳ]�w
    uint32_t u32Limit[SPEED_LIMIT_BANK_MAX]; // Set of speed limit, uint: m/h

    uint8_t u8Offset; // Heinz // Speed offset, uint: m/h

    uint16_t u16Star; // Heinz 32 // speed star, uint: m/h

    uint8_t u8Auto_Gain_Count; // Heinz // AUto gain count

    float fAutoGain; // Heinz // Autogain in float.

    uint8_t u8MergeSpeed; // Merge GPS Speed Select

    uint16_t u16SPEED_LIMIT; // SPEED LIMIT value, uint: 100m/h

    uint16_t u16OVER_SPEED_END_CNT_MAX; // Maximum over speed event count

    uint16_t u16ACCEL_LIMIT; // Acceleration speed limit, uint: m/h

    uint16_t u16DECEL_LIMIT; // Deceleration speed limit, uint: m/h

    uint8_t u8ACCEL_LIM_DURA; // Acceleration Limit Duration

    time_t ttAgain_Time; // Need ReCalibration Auto Gain time

    time_t ttAutoGainTimeOut; // Auto Gain Time Out

    uint8_t u8AutoGainInit;

    uint8_t u8AutoGainSrc;

    uint32_t u32KGAIN_Val;

    bool bGAIN_ChkDisable;

    uint8_t u8Reserve[5];

} SpeedSetTypeDef;

// RPM determination setup
typedef struct
{
    // ��t���έ�, default: 1
    uint16_t u16DIV;

    // RPM trigger level, default: 100 (= 1.00V)
    uint16_t u16Trig;

    // ��t����, default: 0
    uint16_t u16Limit; // RPM limit, uint: RPM

    uint16_t u16RPM_OVER_ON_CNT_MAX; // Maximum count for over RPM event start

    uint16_t u16RPM_OVER_OFF_CNT_MAX; // Maximum count for over RPM event end

    uint16_t u16RPM_IDLE_CNT_MAX; // Maximum count for RPM idle event start

    uint8_t u8Reserve[16];

} RPM_SetTypeDef;

typedef struct
{
    // uint8_t u8UpSpeed[UPSPEED_HOW_LONG_SEC + 1]; // Speed data to upload

    float fRPM; // RPM in float, uint: RPM

    uint16_t u16RPM_Hz; // RPM frequency, uint: times/s

    uint16_t u16Speed_Hz; // Speed frequency, uint: times/s

    uint32_t u32SpeedCount; // Speed pulse count

    uint32_t u32RealSpeed; // Real speed, uint: m/h

    float fAccelRate; // Acceleration rate

    bool bIsOverSpeed; // Is the vehicle currently running over speed limit?

    bool bIsRPM_Idle; // Is the engine currently idle?

    bool bIsOver_RPM; // Is the RPM currently over limit?

    bool bIsOverAccel; // Is the acceleration too much?

    bool bIsOverDecel; // Is the deceleration too much?

    uint8_t u8Reserve[16];

} SpeedInfoTypeDef;

// Functions enable control
typedef union
{
    uint16_t u16Byte;

    struct
    {
        unsigned DRV_ID : 1;    // �r�p�ѧO�\��, 0:���� | 1:�ҥ�
        unsigned EXP_ID : 1;    // �X�R�ѧO�\��, 0:���� | 1:�ҥ�
        unsigned ImmoCtrl : 1;  // ���s���]�w, 0:���� | 1:�ҥ�
        unsigned ASCII_UNI : 1; // ²�T�r�X�]�w, 0:ASCII | 1:UNI-CODE
        unsigned ADDR_MAP : 1;  // �ثe��m�^��, 0:Address | 1:Map
        unsigned CAN_DIST : 1;  // CAN BUS ���{, 0:���� | 1:�ҥ�; �Y���Ϋh�ĥ� sensor ��T�֭p
        unsigned CAN_SPEED : 1; // CAN BUS �ɳt, 0:���� | 1:�ҥ�; �Y���Ϋh�ĥ� (SPEED GAIN = 0): GPS | (SPEED GAIN > 0): sensor ��T
        unsigned CAN_RPM : 1;   // CAN BUS ��t, 0:���� | 1:�ҥ�; �Y���Ϋh�ĥ� sensor ��T
        unsigned CAN_OTHER : 1; // CAN BUS other data, 0: Disable | 1: Enable
        unsigned ENG_MODE : 1;  // Data in English
        unsigned CORRECT_MODE : 1;

    } bits;

} EnableSetTypeDef;
/*
// KF5096 setup
typedef struct
{
    // KF-5096 out hi-duty, default: 1000 (uS)
    uint16_t HiDuty;

}KF5096_SetTypeDef;
*/

// I/O States
typedef union
{
    uint16_t word;

    struct
    {
        unsigned IN_PWR : 1;
        unsigned IN_CHG : 1;
        unsigned IN_ACC : 1;
        unsigned IN_SOS : 1;
        unsigned IN_1 : 1;
        unsigned IN_2 : 1;
        unsigned IN_3 : 1;
        unsigned IN_4 : 1;
        unsigned IN_5 : 1;
        unsigned IN_6 : 1;
        unsigned IN_7 : 1;
        unsigned IN_8 : 1;
        unsigned IN_9 : 1;
        unsigned IN_10 : 1;
        unsigned IN_11 : 1;
        unsigned RES : 1;
    } bits;

} EXTIO_StateTypeDef;

// Vehicle state struct
typedef struct
{
    bool bPWR_ON; // Power state

    bool bPWR_CHK; // Power check state

    bool bACC_ON; // ACC state

    bool bACC_CHK; // ACC check state

    uint16_t u16RPM; // RPM, uint: RPM

    uint32_t u32Speed; // Speed, Unit: m/s

    // GPS Speed
    uint32_t u32Speed_GPS; // GPS speed, unit: m/s

    // �ثe���{, default: 0
    double dDistance; // Unit: m

    // double dDistance1; // Heinz
    // double dDistance2; // Heinz

    // uint32_t u32DistCount1; // Distance count 1
    // uint32_t u32DistCount2; // Distance count 2

    double dDistance_GPS; // GPS distance, unit: m

    double dDistance_CAN; // CAN distance, unit: m

    // �ե����{, default: 0
    // uint32_t u32AdjDist; // Adjust distance

    double dDistBase; // Distance base, unit: m

    double dDistBase_GPS; // GPS distance base, unit: m

    double dDistBase_CAN; // CAN distance base, unit: m

    // ���{�p��, default: 0
    uint32_t u32DistCount; // Distance pulse count

    uint32_t u32DistCount_AGain; // DistCount for AutoGain

    // uint16_t u16EVENT;

    uint8_t u8Reserve[10];

} VehicleStateTypeDef;

// Driver information
/*
typedef struct
{
    // ACC OFF �ɶ��H�������, ACC ON �k�s
    uint32_t u32TimeEngineOff;

    // �r�p�O�ɹH�W�O�I����
    uint8_t	u8CntOverWork;

    // �r�p�O�ɹH�W�ֿn�ɶ�, �H�������
    uint32_t u32AccOverWork;

    // �r�p�ɶ��H�������, �����𮧳W�w���k�s	(��uint32_t�~���|����)
    uint32_t u32WorkTime;

    // �r�p�`�u�@�ɶ��O�ɹH�W�O�I����
    uint8_t	u8CntOverAccum;

    // �`�u�@�ɶ��H�������, �r�p���󴫮��k�s
    uint32_t u32AccumTime;

    // �W�t����
    uint16_t u16CntOverSpeed;

    // �}������
    uint16_t u16CntDoorOpen;

    // ��������
    uint16_t u16CntDoorClose;

    // ACC ON ����
    uint16_t u16CntEngineOn;

    // ACC OFF ����
    uint16_t u16CntEngineOff;

    // �̪�@���s��r�p�ɶ�
    // (Lately Continuous Driving Time)
    uint16_t u16LCDT;

    // �̪�@���`�u�@�ɶ�
    // (Lately Total Working Time)
    uint16_t u16LTWT;

    // �r�p�}�l�u�@���ɶ�
    time_t ttStartUCT;

    // �r�p�}�l�g�׶}�l�n��
    GPS_DataTypeDef tGPS_Data;

    bool bResponse;

    uint8_t u8Reserve[16];

} DrivingInfoTypedef;
*/
typedef struct
{
    time_t ttDrvWorkStartTime; // Start working time

    // uint16_t u16DrvResponse; // Driver response

    uint8_t u8DriveThreshold[2]; // Driving time threshold
    uint8_t u8RestThreshold[2];  // Rest time threshold

    uint8_t u8Reserve[8];

} DRV_SetTypeDef;

// Driver record
typedef struct
{
    uint32_t u32WorkTime; // Driver work time

    uint32_t u32PauseTime; // Driver pause time

    uint32_t u32StandByTime; // Driver standby time

    uint32_t u32RestTime; // Driver rest time

    uint8_t u8Reserve[8];

} DrvRecTypeDef;

// Driver Info
typedef struct
{
    uint32_t u32DrvIdNum;

    uint8_t u8DrvStatus;

    uint32_t u32DrWorkTime;

    uint32_t u32DrRestTime;

    DrvRecTypeDef stDrvRecDay[14];

    double dDriverDistance;

    uint8_t u8Reserve[16];

} DRV_InfoTypeDef;

// Restart log
/*
typedef struct
{
    uint8_t	u8RestartCnt; // Count number of restart

    uint32_t u32SysTickBuf; // SysTick buffer

    uint8_t u8Reserve[8];

} RestartLogTypeDef;
*/
/*
typedef struct
{
    uint16_t u16DataLogPeriod; // Datalog writing period

  RestartLogTypeDef stRestartLog; // Restart log setting

} LogSetTypeDef;
*/

// SD card state
typedef struct
{
    bool bSD_State; // SD card permanent state, OK or Error

    bool bSD_FormatFlag; // SD card format flag

    uint8_t u8SD_ResetCnt; // Number of SD card reset

    uint8_t u8Reserve[8];

} SD_StateTypeDef;

// Upgrade control
typedef struct
{
    bool bFOTA_Flag; // FOTA request flag

    bool bUpgradeFlag; // Upgrade flag

    char cLastFW_Ver[64]; // Last FW version

    time_t ttLastUpgrade; // Last upgrade time

    time_t ttLastInstall; // Last install time

    bool bUpgrade_SMS; // Upgrade through SMS command flag

    bool bUpgrade_TCP; // Upgrade through TCP command flag

    char cUpgrade_PhoneNbr[20]; // Phone number from which the SMS was sent

    uint8_t u8Upgrade_Socket; // Socket ID from which the TCP command was received

    uint8_t u8FOTA_RetryCnt;

    bool bUpgradeNow;

    uint8_t u8Reserve[7];

} UpgradeTypeDef;

typedef struct
{
    PwrDownModeTypeDef tMode;

    uint16_t u16Delay;

    uint8_t u8WkupSource;
} PwrDownTypeDef;

typedef struct
{
    bool bEnableAlm;

    bool bRequest;

    bool bRepeat;

    RTC_DateTypeDef tAlmDate;

    RTC_TimeTypeDef tAlmTime;

    uint32_t u32Mask;

} ExternalAlarmTypeDef;

typedef struct
{
    bool bEnableWkup;

    bool bRequest;

    uint32_t u32Timeout;

} ExternalPedWkupTypeDef;

#if CAN_CUSTOM == CAN_CUSTOM_MBEYE
typedef struct
{
    uint8_t LDWCnt;

    uint8_t LDWTime;

    uint8_t HWTime;

    uint8_t u8Reserve[4];

} MBEYE_SetTypeDef;
#endif

typedef struct
{
    uint8_t u8PID; // Product ID

    uint8_t u8CustID; // Customer ID

    uint8_t u8HwID; // Hardware ID

    uint16_t u16VerID; // Version ID

} ProductInfoTypeDef;

#ifdef ENABLE_LSM6DSL
typedef struct
{
    float fScale;
    float fThreSold;
    uint8_t u8Axis;
    char cAxis_Str[5];
    uint8_t u8ThresoldAngle;
    uint32_t u32DelayTimeOut;
} GS_Setting;
#endif
// Device setting struct
typedef struct
{
    // g_Setting control //
    uint8_t u8SetVer; // Version verification key 1

    uint32_t u32SetSize; //

    // g_Setting info and setup data //
    char cJAS_Ver[10]; // Version based on GSM function availability //

    char cFW_Version[20]; // FW version

    int32_t i32TimeZoneOffset; // Timezone offset in hours //

    PwrDownTypeDef tPwrDown; // Power down setting //

    time_t ttAccOffTime; // ACC off time //

    uint8_t u8CurrDrvID; // Driver index 0 or 1 //

    uint32_t u32DebugMode; // Debug mode setting //

    BusSetTypeDef tBusInfo; // BUS_ID, BUS_SN, VIN //

    SD_StateTypeDef tSD_State; // SD card state //

    // LogSetTypeDef tLogSet; // Datalog setting //

    GSM_SetTypeDef tGSM_SET; // GSM setting //

    DRV_SetTypeDef tDRV_SET; // Driver setting and general info //

    DRV_InfoTypeDef tDrvInfo[DRV_QUANTITY_MAX]; // Individual drivers' info //

    PowerModeTypeDef tPowerMode; // Power mode setting //

    SpeedSetTypeDef tSpeedSet; // Speed setting //

    RPM_SetTypeDef tRPM_Set; // RPM setting //

    CAN_LinkTypeDef tCanLinkType; // CAN Link type //

    EnableSetTypeDef tENABLE_SET; // Enable setting //

    EXTIO_StateTypeDef tReverse; // Reserve EXTIO //

    UpgradeTypeDef tUpgrade; // Upgrade control

    time_t ttLastSaveTime; // Last save time

    time_t ttCalibrationTime; // Calibration time

    bool bCAN_RelinkAll; // CAN relink mode

    bool bIOT_SIM_En; // Enable IOT SIM mode

    char cFTP_VDR_Path[64]; // VDR file path

    uint8_t u8FTP_VDR_Mode; // Optional classification for VDR file path

    time_t ttLastVDR_Time; // Last time making VDR file

    uint32_t u32RecordMode; // Record mode setting //

    bool bLCDSaver; // LCD Saver Flag

    bool bVdrNow; // VDR Now Download Flag

    uint8_t u8DB9Func; // DB9 Function Select

    uint16_t u16AG_Dist_Limit; // AutoGain Distance Limit, uint: m

    uint32_t u32GPS_Baudrate; // GPS UART Baudrate

    char cXTRA_URL[80]; // GPS XTRA file URL

    char cEPO_URL[100]; // GPS EPO file URL

    GPS_LonTypeDef gpsLonBK;

    GPS_LatTypeDef gpsLatBK;

    ExternalAlarmTypeDef tExAlarm; // External alarm setting

    ExternalPedWkupTypeDef tExPedWkup; // External periodic wake up setting

    uint16_t u16AG_Dist_Limit_Temp; // AutoGain Distance Limit temp, uint: m

    uint8_t u8Reserve[298];

#if ENABLE_CAN == 1 && CAN_CUSTOM == CAN_CUSTOM_MBEYE
    MBEYE_SetTypeDef tMBEYE_SET;
#endif

    uint8_t u8SetVerChk; // Version verification key 2
#ifdef ENABLE_LSM6DSL
    GS_Setting tGS_Setting;
#endif

} gSettingTypeDef;

// System info struct
typedef struct
{
    // Current time of the system in seconds
    time_t ttNow; // Current system local time

    uint16_t u16GSensorMeter; // G sensor value

    // uint8_t u8VdrInfo; // Heinz

    // DrivingInfoTypedef tDrivingInfo; // Driver info

    GPS_DataTypeDef tGPS_Data; // GPS data

    SpeedInfoTypeDef tSpeedInfo; // Speed data

    EXTIO_StateTypeDef tEXTIO; // EXTIO data

    VehicleStateTypeDef tVehicleState; // Vehicle state data

    bool bBatChgState;

} gSysInfoTypeDef;

typedef struct
{
    uint8_t u8Ver; // Backup data version verification

    uint32_t u32Size; // Backup data size

    time_t ttLastSaveTime; // Backup data last save time

    double dDistance; // Distance

    char cBus_ID[11]; // Bus ID

    char cVIN[21]; // VIN

    float fGain; // Speed GAIN

    uint16_t u16DIV; // RPM DIV

    CAN_LinkTypeDef tCanLink; // CAN link type

    uint8_t u8CanSpeed; // CAN speed

    uint8_t u8CanRpm; // CAN RPM

    MEM_U32_t u32IP_Addr; // Server IP address

    uint16_t u16Port; // Server port

    uint32_t u32DrvIdNum1; // Driver 1 ID

    uint32_t u32DrvIdNum2; // Driver 2 ID

    uint32_t u32Limit1; // Speed limit level 1

    uint32_t u32Limit2; // Speed limit level 2

    uint32_t u32Limit3; // Speed limit level 3

    time_t ttCalibrationTime; // Auto GAIN calibration time

    char cAPN[31]; // Server APN

    char cDomainName[31]; // Server domain name

    MEM_U32_t u32IP_Addr2; // Extended server IP address

    uint16_t u16Port2; // Extended server port

    char cDomainName2[31]; // Extended server domain name

    char cFTP_Addr[31]; // FTP server 1 address

    uint16_t u16FTP_Port; // FTP server 1 address

    char cFTP_UserName[21]; // FTP server 1 username

    char cFTP_Password[21]; // FTP server 1 password

    char cFTP_Addr2[31]; // FTP server 2 address

    uint16_t u16FTP_Port2; // FTP server 2 port

    char cFTP_UserName2[21]; // FTP server 2 username

    char cFTP_Password2[21]; // FTP server 2 password

    char cFTP_VDR_Path[64]; // VDR file path

    uint8_t u8FTP_VDR_Mode; // Optional classification for VDR file path

    bool bLCDSaver; // LCD Saver Flag

    bool bVdrNow; // VDR Now Download Flag

    uint8_t u8DB9Func; // DB9 Function Select

    char cXTRA_URL[80]; // GPS XTRA file URL;

    char cEPO_URL[100]; // GPS EPO file URL;

    bool bDisableSocketEXT; // Enable/ disable 4G EXT socket

    bool b4G_WakeUp; // Enable/ disable 4G Wakeup

    ExternalAlarmTypeDef tExAlarm; // External alarm setting

    ExternalPedWkupTypeDef tExPedWkup; // External periodic wake up setting

    PwrDownTypeDef tPwrDown; // Power down setting //

    uint32_t u32KGAIN_Val; // KGAIN value

    bool bGAIN_ChkDisable; // Disable/enable GAIN check function

} BackupSettingTypeDef;

#pragma pack()

//============================================================================//
// User Externs
//============================================================================//
extern bool bSystemMode;

extern TaskInfoTypeDef taskInfo[TASK_QUANTITY];

// Public Setting_t instant g_Setting
extern gSettingTypeDef g_Setting;

extern gSysInfoTypeDef g_SysInfo;

// extern bool g_bUSBDownloadFlg;

extern bool g_bSystemBootFlag;

// extern bool g_bDoFormatFlag;

extern const ProductInfoTypeDef PRODUCT_INFO;

extern bool bSysLogEn;

extern uint8_t g_u8HwInfo;

extern bool g_bPTestMode;

extern bool bSystemStop;

extern bool bSystemTimeReady;

extern uint8_t u8ResetFlagStatus;

extern bool bHaveResetFlag;

extern uint8_t u8DrvPreStatus[2];
// extern uint8_t u8PublicDataBuf[8192];

extern float fAutoGainValue;

#if ENABLE_GSM_NET == 1 &&      \
    ENABLE_FTP_DOWNLOAD == 1 && \
    ENABLE_FTP_BACKUP == 1
extern bool bRestoreSettingFlag;
#endif

//============================================================================//
// User Functions Prototypes
//============================================================================//
void System_TaskInfoInit(void);
void System_TaskCheck(void);
void System_TaskWdgRefresh(TaskTagTypeDef taskTag);
void System_TaskCheckWaterMark(void);
void System_IO_3V3_Enable(void);
void System_IO_3V3_Disable(void);
void System_CheckTempFile(void);
void System_CheckSettingFile(void);
void System_SetupDefault(void);
void System_Init(void);
void System_DeInit(void);
void System_Reset(void);
time_t System_GetTime(void);
bool System_SaveBackupSetting(void);
bool System_SaveBackupSettingFinal(void);
bool System_SaveBackupSettingAll(SettingItemTypeDef tItem);
bool System_RestoreBackupSetting(SettingItemTypeDef tItem);
void System_DelBackupSetting(void);
bool System_SaveSetting(void);
bool System_SaveSettingFinal(void);
bool System_SaveSettingAll(void);
bool System_GetSetting(void);
#if ENABLE_GSM_NET == 1 &&      \
    ENABLE_FTP_DOWNLOAD == 1 && \
    ENABLE_FTP_BACKUP == 1
bool System_UploadBackupSettingToFTP(time_t ttBackupTime);
void System_RestoreSettingFromFTP(void);
#endif
void System_EnterStandbyMode(void);
void System_Stop(void);
bool SystemFLASH_Erase(void);
bool System_CheckRDP(void);
bool System_SetRDP(void);
bool System_ClrRDP(void);
bool System_CheckWRP(void);
bool System_EnableWRP(void);
bool System_DisableWRP(void);
void System_ChkHardfault(void);
void System_SetExAlmSetting(ExternalAlarmTypeDef *ptSetting);
void System_CheckExAlmSetting(void);
void System_SetWakeupTimer(uint32_t u32Timeout);
bool System_DisableResetAfterStandby(void);

//============================================================================//
#endif //_SYSTEM_H_
