#ifndef _CONFIG_H_
#define _CONFIG_H_
//============================================================================//

//============================================================================//
// User Includes
//============================================================================//
#include "Version.h"

//============================================================================//
// User Defines
//============================================================================//
// Define for customer
#define CUSTOMER_NORMAL (0)    // STANDARD
#define CUSTOMER_JASS_EPA (1)  // ���@�L���O����
#define CUSTOMER_JASS_DEMO (2) // ���@�L�i�ܾ�

#define CUSTOMER_ARTC (10) // �ꤺ�{�� : ARTC
#define CUSTOMER_TTIA (11) // �ꤺ�{�� : TTIA
#define CUSTOMER_EPA (12)  // �ꤺ�{�� : ���O�p�ʱ�����

#define CUSTOMER_MASTER (30)  // �ꤺ�Ȥ�(����t) : ���B�T��
#define CUSTOMER_TANGENG (31) // �ꤺ�Ȥ�(����t) : ��a�������
#define CUSTOMER_DAF (32)     // �ꤺ�Ȥ�(����t) : DAF
#define CUSTOMER_DAF_2 (33)   // �ꤺ�Ȥ�(����t) : DAF-2
#define CUSTOMER_MAN (34)     // �ꤺ�Ȥ�(����t) : MAN
#define CUSTOMER_SYE (35)     // �ꤺ�Ȥ�(����t) : ���q�T��
#define CUSTOMER_SCANIA (36)  // SCANIA

#define CUSTOMER_KINGLONG (40) // ��~�Ȥ�(����t) : ���s�T��

#define CUSTOMER_EUPFIN (50)   // �ꤺ�Ȥ�(DEALER) : �|�����
#define CUSTOMER_SKYEYE (51)   // �ꤺ�Ȥ�(DEALER) : �Ѳ��ìP���
#define CUSTOMER_EUPFIN_B (52) // �ꤺ�Ȥ�(DEALER) : �|�����_V2

#define CUSTOMER_FORMOSA (60)  // �ꤺ�Ȥ�(END USER) : �x��f�B
#define CUSTOMER_ARTC_DMD (61) // �ꤺ�Ȥ�(END USER) : ARTC - DEVICE , �[�p�欰���R��
#define CUSTOMER_CPTI (62)     // �ꤺ�Ȥ�(END USER) : �����ȹB+�x�_�ȹB
#define CUSTOMER_FPTC (63)     // �ꤺ�Ȥ�(END USER) : �x��f�B��²��

#define CUSTOMER_FCFC (65) // �ꤺ�Ȥ�(END USER) : FCFC

#define CUSTOMER_GDBUS (70)
#define CUSTOMER_CHT (71)     // ���عq�H�򶩤����B�M��(FTP�خL)
#define CUSTOMER_CHT_STD (72) // ���عq�HSTD(FTP���@�L)

#define CUSTOMER_TST (110) // ��~�Ȥ�(END USER) : �V�n TST

#define CUSTOMER_ECCOMM (200) // SAMPLE TEST : ���q�q�T

#define CUSTOMER_TESTB (99)

// Setup for CUSTOMER
#define CUSTOMER_TYPE CUSTOMER_NORMAL
// #define CUSTOMER_TYPE			CUSTOMER_JASS_EPA
// #define CUSTOMER_TYPE			CUSTOMER_JASS_DEMO

// #define CUSTOMER_TYPE			CUSTOMER_ARTC
// #define CUSTOMER_TYPE			CUSTOMER_TTIA
// #define CUSTOMER_TYPE			CUSTOMER_EPA

// #define CUSTOMER_TYPE			CUSTOMER_MASTER
// #define CUSTOMER_TYPE			CUSTOMER_TANGENG
// #define CUSTOMER_TYPE     CUSTOMER_DAF
// #define CUSTOMER_TYPE     CUSTOMER_DAF_2
// #define CUSTOMER_TYPE     CUSTOMER_MAN
// #define CUSTOMER_TYPE			CUSTOMER_SYE
// #define CUSTOMER_TYPE     CUSTOMER_SCANIA

// #define CUSTOMER_TYPE			CUSTOMER_KINGLONG

// #define CUSTOMER_TYPE			CUSTOMER_EUPFIN
// #define CUSTOMER_TYPE			CUSTOMER_EUPFIN_B
// #define CUSTOMER_TYPE			CUSTOMER_SKYEYE

// #define CUSTOMER_TYPE			CUSTOMER_FORMOSA
// #define CUSTOMER_TYPE			CUSTOMER_FCFC

// #define CUSTOMER_TYPE     CUSTOMER_GDBUS

// #define CUSTOMER_TYPE			CUSTOMER_ARTC_DMD
// #define CUSTOMER_TYPE			CUSTOMER_CPTI
// #define CUSTOMER_TYPE			CUSTOMER_FPTC

// #define CUSTOMER_TYPE			CUSTOMER_TST

// #define CUSTOMER_TYPE			CUSTOMER_ECCOMM

// #define CUSTOMER_TYPE     CUSTOMER_TESTB

// Main server types
// #define MAIN_SERVER_JASSLIN         (0) // Main server is Jasslin company server
// #define MAIN_SERVER_GOVERNMENT      (1) // Main server is government server

// Setup for parameters //
#define STR_CUST " " // Customer name

#define TIME_ZONE_OFFSET_DEF 0U // Local timezone offset

#define GSM_SERVER_JASS 1U
#define GSM_SERVER_VDRS 2U
#define GSM_SERVER_VDRS_2 3U
#define GSM_SERVER_GOV 4U

#define GSM_SERVER_DEF GSM_SERVER_VDRS
#define GSM_SERVER_EXT GSM_SERVER_VDRS

#define RTC_WKUP_SRC_MCU 0U
#define RTC_WKUP_SRC_MG82 1U

#define RTC_WKUP_SRC RTC_WKUP_SRC_MCU
// #define RTC_WKUP_SRC                RTC_WKUP_SRC_MG82

#define CAN_RX_MODE_IT 0U
#define CAN_RX_MODE_POLLING 1U

#define CAN_RX_MODE CAN_RX_MODE_POLLING

// #define MAIN_SERVER                 MAIN_SERVER_JASSLIN // SOCKET_BANK_DEF will be MAIN_SERVER

// Setup for function enable //

// Only for development process !!! //
// In testing version and official release function please set this option to 0 !!! //
#define ENABLE_MULTI_DEBUG (0) // Enable multiple debug modes

#define ENABLE_ADJUST_OFF_TIME (0) // Enable turn off wait time longer than 20 sec

#define ENABLE_CAN (0) // Enable CAN Bus functions

#define ENABLE_DEBUG_CAN_REC (0) // Enable debug CAN function

#define ENABLE_DEBUG_MSG (0) // Enable print all debug messages

#define ENABLE_DVR (0) // Enable DVR function

#define ENABLE_EX_WKUP (0) // Enable external wake up function

#define ENABLE_FOTA_ACC_OFF (0) // Enable doing FOTA when ACC is off

#define ENABLE_FTP_BACKUP (0) // Enable backup g_Setting important variables to FTP server

#define ENABLE_FTP_DOWNLOAD (0) // Enable FTP file download function

#define ENABLE_FTPS (0) // Enable access to FTPS server

#define ENABLE_GPS_CHECK_INT (0) // Enable checking GPS interrupt

#define ENABLE_GPS_EPO (0) // Enable GPS EPO

#define ENABLE_GPS_MODULE (0) // Enable GPS module

#define ENABLE_GPS_GNSS (0) // Enable GPS from GNSS on QTEC21/25

#define ENABLE_GPS_SPEED_REC (0) // Enable GPS speed record

#define ENABLE_GPS_XTRA (0) // Enable GPS XTRA service

#define ENABLE_GS_MC3210 (0) // Enable G-Sensor

#define ENABLE_GSM_NET (0) // Enable GSM function

#define ENABLE_GSM_QTEC21 (0) // Enable QtEC21 module as GSM module

#define ENABLE_INDEX_BUFFER (0) // Enable handling SD file index using buffer

#define ENABLE_LSM6DSL (0) // Enable G-Sensor LSM6DSL

#define ENABLE_MG82 (0) // Enable MG82 IC functions

#define ENABLE_MG82_RTC (0) // Enbble RTC function of MG82 RTC

#define ENABLE_MULTI_FTP_SERVER (0) // Enable multiple FTP servers

#define ENABLE_MULTI_SERVER (0) // Enable multiple servers

#define ENABLE_PC_TRANS (0) // Enable PC file transfer

#define ENABLE_POWER_CHECK_VOLT (0) // Enable checking power by measuring voltage

#define ENABLE_PULSE_COUNT_REC (0) // Enable TIMER 2 counter

#define ENABLE_RX8130 (0) // Enable IC RX8130

#define ENABLE_SD_BUFFER (0) // Enable doing SD 1 min task using buffer

#define ENABLE_SMS_EVENT (0) // Enable SMS response through 4G EVENT

#define ENABLE_SPEED_INT_REC (0) // Enable speed interrupt recording

#define ENABLE_SYSLOG (0) // Enable write SYSLOG

#define ENABLE_UPLOAD_CHK (0) // Enable re-check uploaded VDR files

#define ENABLE_USER_ALARM (0) // Enable user warning using command

// Define for particular customers //

// Test B
#if CUSTOMER_TYPE == CUSTOMER_TESTB
#undef STR_CUST
#define STR_CUST "TESTB"

#undef TIME_ZONE_OFFSET_DEF
#define TIME_ZONE_OFFSET_DEF HOUR_2_SECOND(8)

#undef GSM_SERVER_DEF
#define GSM_SERVER_DEF GSM_SERVER_VDRS

#undef GSM_SERVER_EXT
#define GSM_SERVER_EXT GSM_SERVER_VDRS

#undef ENABLE_ADJUST_OFF_TIME
#define ENABLE_ADJUST_OFF_TIME (1)

#undef ENABLE_CAN
#define ENABLE_CAN (1)

#undef ENABLE_DEBUG_CAN_REC
#define ENABLE_DEBUG_CAN_REC (1)

#undef ENABLE_DEBUG_MSG
#define ENABLE_DEBUG_MSG (0)

#undef ENABLE_DVR
#define ENABLE_DVR (1)

#undef ENABLE_EX_WKUP
#define ENABLE_EX_WKUP (1)

#undef ENABLE_FOTA_ACC_OFF
#define ENABLE_FOTA_ACC_OFF (1)

#undef ENABLE_FTP_BACKUP
#define ENABLE_FTP_BACKUP (1)

#undef ENABLE_FTP_DOWNLOAD
#define ENABLE_FTP_DOWNLOAD (1)

#undef ENABLE_FTPS
#define ENABLE_FTPS (0)

#undef ENABLE_GPS_CHECK_INT
#define ENABLE_GPS_CHECK_INT (1)

#undef ENABLE_GPS_EPO
#define ENABLE_GPS_EPO (0)

#undef ENABLE_GPS_GNSS
#define ENABLE_GPS_GNSS (0)

#undef ENABLE_GPS_SPEED_REC
#define ENABLE_GPS_SPEED_REC (1)

#undef ENABLE_GPS_XTRA
#define ENABLE_GPS_XTRA (1)

#undef ENABLE_GSM_NET
#define ENABLE_GSM_NET (1)

#undef ENABLE_MG82
#define ENABLE_MG82 (1)

#undef ENABLE_MG82_RTC
#define ENABLE_MG82_RTC (1)

#undef ENABLE_MULTI_DEBUG
#define ENABLE_MULTI_DEBUG (1)

#undef ENABLE_MULTI_FTP_SERVER
#define ENABLE_MULTI_FTP_SERVER (1)

#undef ENABLE_MULTI_SERVER
#define ENABLE_MULTI_SERVER (1)

#undef ENABLE_PC_TRANS
#define ENABLE_PC_TRANS (1)

#undef ENABLE_POWER_CHECK_VOLT
#define ENABLE_POWER_CHECK_VOLT (1)

#undef ENABLE_PULSE_COUNT_REC
#define ENABLE_PULSE_COUNT_REC (0)

#undef ENABLE_RX8130
#define ENABLE_RX8130 (1)

#undef ENABLE_SMS_EVENT
#define ENABLE_SMS_EVENT (1)

#undef ENABLE_SPEED_INT_REC
#define ENABLE_SPEED_INT_REC (1)

#undef ENABLE_UPLOAD_CHK
#define ENABLE_UPLOAD_CHK (1)
#endif

// Standard customer
#if CUSTOMER_TYPE == CUSTOMER_NORMAL
#undef STR_CUST
#define STR_CUST "STD"

#undef TIME_ZONE_OFFSET_DEF
#define TIME_ZONE_OFFSET_DEF HOUR_2_SECOND(8)

#undef GSM_SERVER_DEF
#define GSM_SERVER_DEF GSM_SERVER_VDRS

#undef GSM_SERVER_EXT
#define GSM_SERVER_EXT GSM_SERVER_VDRS

#undef ENABLE_ADJUST_OFF_TIME
#define ENABLE_ADJUST_OFF_TIME (1)

#undef ENABLE_CAN
#define ENABLE_CAN (1)

#undef ENABLE_DEBUG_CAN_REC
#define ENABLE_DEBUG_CAN_REC (1)

#undef ENABLE_DEBUG_MSG
#define ENABLE_DEBUG_MSG (0)

#undef ENABLE_DVR
#define ENABLE_DVR (1)

#undef ENABLE_EX_WKUP
#define ENABLE_EX_WKUP (1)

#undef ENABLE_FOTA_ACC_OFF
#define ENABLE_FOTA_ACC_OFF (1)

#undef ENABLE_FTP_BACKUP
#define ENABLE_FTP_BACKUP (1)

#undef ENABLE_FTP_DOWNLOAD
#define ENABLE_FTP_DOWNLOAD (1)

#undef ENABLE_FTPS
#define ENABLE_FTPS (0)

#undef ENABLE_GPS_CHECK_INT
#define ENABLE_GPS_CHECK_INT (1)

#undef ENABLE_GPS_EPO
#define ENABLE_GPS_EPO (1)

#undef ENABLE_GPS_GNSS
#define ENABLE_GPS_GNSS (0)

#undef ENABLE_GPS_SPEED_REC
#define ENABLE_GPS_SPEED_REC (1)

#undef ENABLE_GPS_XTRA
#define ENABLE_GPS_XTRA (1)

#undef ENABLE_GSM_NET
#define ENABLE_GSM_NET (1)

#undef ENABLE_LSM6DSL
#define ENABLE_LSM6DSL (0)

#undef ENABLE_MG82
#define ENABLE_MG82 (1)

#undef ENABLE_MG82_RTC
#define ENABLE_MG82_RTC (0)

#undef ENABLE_MULTI_DEBUG
#define ENABLE_MULTI_DEBUG (0)

#undef ENABLE_MULTI_FTP_SERVER
#define ENABLE_MULTI_FTP_SERVER (1)

#undef ENABLE_MULTI_SERVER
#define ENABLE_MULTI_SERVER (1)

#undef ENABLE_PC_TRANS
#define ENABLE_PC_TRANS (1)

#undef ENABLE_POWER_CHECK_VOLT
#define ENABLE_POWER_CHECK_VOLT (1)

#undef ENABLE_PULSE_COUNT_REC
#define ENABLE_PULSE_COUNT_REC (0)

#undef ENABLE_RX8130
#define ENABLE_RX8130 (1)

#undef ENABLE_SMS_EVENT
#define ENABLE_SMS_EVENT (1)

#undef ENABLE_SPEED_INT_REC
#define ENABLE_SPEED_INT_REC (1)

#undef ENABLE_UPLOAD_CHK
#define ENABLE_UPLOAD_CHK (1)

#endif

// General define //

// Enable GPS module
#if HW_TYPE == HW_JAS106 || \
    HW_TYPE == HW_JAS208 || \
    HW_TYPE == HW_JAS306
#undef ENABLE_GPS_MODULE
#define ENABLE_GPS_MODULE (1)
#endif

// Enable hardware module for GSM functions
#if ENABLE_GSM_NET == 1
#undef ENABLE_GSM_QTEC21
#define ENABLE_GSM_QTEC21 (1)
#endif

#if !ENABLE_MG82
#undef RTC_WKUP_SRC
#define RTC_WKUP_SRC RTC_WKUP_SRC_MCU

#undef ENABLE_MG82_RTC
#define ENABLE_MG82_RTC (1)
#endif

//============================================================================//
#endif //_CONFIG_H_
