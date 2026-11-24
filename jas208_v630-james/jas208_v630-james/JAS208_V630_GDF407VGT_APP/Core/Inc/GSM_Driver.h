#ifndef _GSM_DRIVER_H_
#define _GSM_DRIVER_H_
//============================================================================//

//============================================================================//
// User Includes
//============================================================================//
#include <time.h>

#include "stm32fxxx_hal_user.h"
#include "CommFunc.h"
#include "Config.h"
#include "DataLog.h"
#include "DataType.h"
//#include "GSM_QtEC21.h"
#if ENABLE_GSM_SIM908
#include "GSM_SIM908.h"
#elif ENABLE_GSM_SIM968
#include "GSM_SIM908.h"
#elif ENABLE_GSM_SIM5320
#include "GSM_SIM5320.h"
#elif ENABLE_GSM_QTEC21
#include "GSM_QtEC21.h"
#endif
#include "HW_Config.h"


//============================================================================//
// User Defines
//============================================================================//
#if ENABLE_GSM_NET
#define	STR_PASS_ADMIN			            "5948" // Network pass admin

#define	STR_PASS_USER			              "1111" // Network pass user

// Server Jasslin information
#define SERVER_JASS_IP_ADDR             "210.65.89.237"

#define SERVER_JASS_IP_ADDR_BYTE3       210U
#define SERVER_JASS_IP_ADDR_BYTE2       65U
#define SERVER_JASS_IP_ADDR_BYTE1       89U
#define SERVER_JASS_IP_ADDR_BYTE0       237U

#define SERVER_JASS_DOMAIN_NAME         "NONE"

#define SERVER_JASS_PORT                6666U

#define SERVER_JASS_ENABLE_CHK          1U
#define SERVER_JASS_ENABLE_LOG          1U
#define SERVER_JASS_ENABLE_EVENT        1U

#define SERVER_JASS_HEADER_CHK          "$CHK"
#define SERVER_JASS_HEADER_LOG          "$LOG"
#define SERVER_JASS_HEADER_EVENT        "$DATA"

//#define SERVER_JASS_SEND_NORMAL         DATALOG_UPLOAD_MAX_10
//#define SERVER_JASS_SEND_NORMAL         DATALOG_UPLOAD_MAX_15
//#define SERVER_JASS_SEND_NORMAL         DATALOG_UPLOAD_MAX_20
#define SERVER_JASS_SEND_NORMAL         DATALOG_UPLOAD_MAX_30
//#define SERVER_JASS_SEND_NORMAL         DATALOG_UPLOAD_MAX_60

// Server VDRS information
#define SERVER_VDRS_IP_ADDR             "60.251.157.47"

#define SERVER_VDRS_IP_ADDR_BYTE3       60U
#define SERVER_VDRS_IP_ADDR_BYTE2       251U
#define SERVER_VDRS_IP_ADDR_BYTE1       157U
#define SERVER_VDRS_IP_ADDR_BYTE0       47U

#define SERVER_VDRS_DOMAIN_NAME         "vdrs-jasslin.com"

#define SERVER_VDRS_PORT                8208U

#define SERVER_VDRS_ENABLE_CHK          1U
#define SERVER_VDRS_ENABLE_LOG          1U
#define SERVER_VDRS_ENABLE_EVENT        1U

#define SERVER_VDRS_HEADER_CHK          "$CHK1"
#define SERVER_VDRS_HEADER_LOG          "$LOG40"
#define SERVER_VDRS_HEADER_EVENT        "$DATA1"

//#define SERVER_VDRS_SEND_NORMAL         DATALOG_UPLOAD_MAX_10
//#define SERVER_VDRS_SEND_NORMAL         DATALOG_UPLOAD_MAX_15
//#define SERVER_VDRS_SEND_NORMAL         DATALOG_UPLOAD_MAX_20
#define SERVER_VDRS_SEND_NORMAL         DATALOG_UPLOAD_MAX_30
//#define SERVER_VDRS_SEND_NORMAL         DATALOG_UPLOAD_MAX_60

// Server VDRS information
#define SERVER_VDRS_2_IP_ADDR           "60.251.157.48"

#define SERVER_VDRS_2_IP_ADDR_BYTE3     60U
#define SERVER_VDRS_2_IP_ADDR_BYTE2     251U
#define SERVER_VDRS_2_IP_ADDR_BYTE1     157U
#define SERVER_VDRS_2_IP_ADDR_BYTE0     48U

#define SERVER_VDRS_2_DOMAIN_NAME       "vdrs-jasslin.com"

#define SERVER_VDRS_2_PORT              9208U

#define SERVER_VDRS_2_ENABLE_CHK        1U
#define SERVER_VDRS_2_ENABLE_LOG        1U
#define SERVER_VDRS_2_ENABLE_EVENT      1U

#define SERVER_VDRS_2_HEADER_CHK          "$CHK1"
#define SERVER_VDRS_2_HEADER_LOG          "$LOG40"
#define SERVER_VDRS_2_HEADER_EVENT        "$DATA1"

//#define SERVER_VDRS_2_SEND_NORMAL       DATALOG_UPLOAD_MAX_10
//#define SERVER_VDRS_2_SEND_NORMAL       DATALOG_UPLOAD_MAX_15
//#define SERVER_VDRS_2_SEND_NORMAL       DATALOG_UPLOAD_MAX_20
#define SERVER_VDRS_2_SEND_NORMAL       DATALOG_UPLOAD_MAX_30
//#define SERVER_VDRS_2_SEND_NORMAL       DATALOG_UPLOAD_MAX_60

// Server Government information
#define SERVER_GOV_IP_ADDR              "223.200.80.120" 

#define SERVER_GOV_IP_ADDR_BYTE3        223U
#define SERVER_GOV_IP_ADDR_BYTE2        200U
#define SERVER_GOV_IP_ADDR_BYTE1        80U
#define SERVER_GOV_IP_ADDR_BYTE0        120U

#define SERVER_GOV_DOMAIN_NAME          "NONE"

#define SERVER_GOV_PORT                 9030U

#define SERVER_GOV_ENABLE_CHK           0U
#define SERVER_GOV_ENABLE_LOG           1U
#define SERVER_GOV_ENABLE_EVENT         0U

#define SERVER_GOV_HEADER_CHK           "NONE"
#define SERVER_GOV_HEADER_LOG           "$LOG"
#define SERVER_GOV_HEADER_EVENT         "NONE"

//#define SERVER_GOV_SEND_NORMAL          DATALOG_UPLOAD_MAX_10
//#define SERVER_GOV_SEND_NORMAL          DATALOG_UPLOAD_MAX_15
//#define SERVER_GOV_SEND_NORMAL          DATALOG_UPLOAD_MAX_20
#define SERVER_GOV_SEND_NORMAL          DATALOG_UPLOAD_MAX_30
//#define SERVER_GOV_SEND_NORMAL          DATALOG_UPLOAD_MAX_60

// Default server information
#if GSM_SERVER_DEF == GSM_SERVER_JASS
#define SERVER_1_IP_ADDR                SERVER_JASS_IP_ADDR

#define SERVER_1_IP_ADDR_BYTE3          SERVER_JASS_IP_ADDR_BYTE3
#define SERVER_1_IP_ADDR_BYTE2          SERVER_JASS_IP_ADDR_BYTE2
#define SERVER_1_IP_ADDR_BYTE1          SERVER_JASS_IP_ADDR_BYTE1
#define SERVER_1_IP_ADDR_BYTE0          SERVER_JASS_IP_ADDR_BYTE0

#define SERVER_1_DOMAIN_NAME            SERVER_JASS_DOMAIN_NAME

#define SERVER_1_PORT                   SERVER_JASS_PORT

#define SERVER_1_ENABLE_CHK             SERVER_JASS_ENABLE_CHK
#define SERVER_1_ENABLE_LOG             SERVER_JASS_ENABLE_LOG
#define SERVER_1_ENABLE_EVENT           SERVER_JASS_ENABLE_EVENT

#define SERVER_1_HEADER_CHK             SERVER_JASS_HEADER_CHK
#define SERVER_1_HEADER_LOG             SERVER_JASS_HEADER_LOG
#define SERVER_1_HEADER_EVENT           SERVER_JASS_HEADER_EVENT

#define SERVER_1_SEND_NORMAL            SERVER_JASS_SEND_NORMAL
#elif GSM_SERVER_DEF == GSM_SERVER_VDRS
#define SERVER_1_IP_ADDR                SERVER_VDRS_IP_ADDR

#define SERVER_1_IP_ADDR_BYTE3          SERVER_VDRS_IP_ADDR_BYTE3
#define SERVER_1_IP_ADDR_BYTE2          SERVER_VDRS_IP_ADDR_BYTE2
#define SERVER_1_IP_ADDR_BYTE1          SERVER_VDRS_IP_ADDR_BYTE1
#define SERVER_1_IP_ADDR_BYTE0          SERVER_VDRS_IP_ADDR_BYTE0

#define SERVER_1_DOMAIN_NAME            SERVER_VDRS_DOMAIN_NAME

#define SERVER_1_PORT                   SERVER_VDRS_PORT

#define SERVER_1_ENABLE_CHK             SERVER_VDRS_ENABLE_CHK
#define SERVER_1_ENABLE_LOG             SERVER_VDRS_ENABLE_LOG
#define SERVER_1_ENABLE_EVENT           SERVER_VDRS_ENABLE_EVENT

#define SERVER_1_HEADER_CHK             SERVER_VDRS_HEADER_CHK
#define SERVER_1_HEADER_LOG             SERVER_VDRS_HEADER_LOG
#define SERVER_1_HEADER_EVENT           SERVER_VDRS_HEADER_EVENT

#define SERVER_1_SEND_NORMAL            SERVER_VDRS_SEND_NORMAL
#elif GSM_SERVER_DEF == GSM_SERVER_VDRS_2
#define SERVER_1_IP_ADDR                SERVER_VDRS_2_IP_ADDR

#define SERVER_1_IP_ADDR_BYTE3          SERVER_VDRS_2_IP_ADDR_BYTE3
#define SERVER_1_IP_ADDR_BYTE2          SERVER_VDRS_2_IP_ADDR_BYTE2
#define SERVER_1_IP_ADDR_BYTE1          SERVER_VDRS_2_IP_ADDR_BYTE1
#define SERVER_1_IP_ADDR_BYTE0          SERVER_VDRS_2_IP_ADDR_BYTE0

#define SERVER_1_DOMAIN_NAME            SERVER_VDRS_2_DOMAIN_NAME

#define SERVER_1_PORT                   SERVER_VDRS_2_PORT

#define SERVER_1_ENABLE_CHK             SERVER_VDRS_2_ENABLE_CHK
#define SERVER_1_ENABLE_LOG             SERVER_VDRS_2_ENABLE_LOG
#define SERVER_1_ENABLE_EVENT           SERVER_VDRS_2_ENABLE_EVENT

#define SERVER_1_HEADER_CHK             SERVER_VDRS_2_HEADER_CHK
#define SERVER_1_HEADER_LOG             SERVER_VDRS_2_HEADER_LOG
#define SERVER_1_HEADER_EVENT           SERVER_VDRS_2_HEADER_EVENT

#define SERVER_1_SEND_NORMAL            SERVER_VDRS_2_SEND_NORMAL
#elif GSM_SERVER_DEF == GSM_SERVER_GOV
#define SERVER_1_IP_ADDR                SERVER_GOV_IP_ADDR

#define SERVER_1_IP_ADDR_BYTE3          SERVER_GOV_IP_ADDR_BYTE3
#define SERVER_1_IP_ADDR_BYTE2          SERVER_GOV_IP_ADDR_BYTE2
#define SERVER_1_IP_ADDR_BYTE1          SERVER_GOV_IP_ADDR_BYTE1
#define SERVER_1_IP_ADDR_BYTE0          SERVER_GOV_IP_ADDR_BYTE0

#define SERVER_1_DOMAIN_NAME            SERVER_GOV_DOMAIN_NAME

#define SERVER_1_PORT                   SERVER_GOV_PORT

#define SERVER_1_ENABLE_CHK             SERVER_GOV_ENABLE_CHK
#define SERVER_1_ENABLE_LOG             SERVER_GOV_ENABLE_LOG
#define SERVER_1_ENABLE_EVENT           SERVER_GOV_ENABLE_EVENT

#define SERVER_1_HEADER_CHK             SERVER_GOV_HEADER_CHK
#define SERVER_1_HEADER_LOG             SERVER_GOV_HEADER_LOG
#define SERVER_1_HEADER_EVENT           SERVER_GOV_HEADER_EVENT

#define SERVER_1_SEND_NORMAL            SERVER_GOV_SEND_NORMAL
#else
#define SERVER_1_IP_ADDR                ""

#define SERVER_1_IP_ADDR_BYTE3          0U
#define SERVER_1_IP_ADDR_BYTE2          0U
#define SERVER_1_IP_ADDR_BYTE1          0U
#define SERVER_1_IP_ADDR_BYTE0          0U

#define SERVER_1_DOMAIN_NAME            ""

#define SERVER_1_PORT                   0U

#define SERVER_1_ENABLE_CHK             0U
#define SERVER_1_ENABLE_LOG             0U
#define SERVER_1_ENABLE_EVENT           0U

#define SERVER_1_HEADER_CHK             ""
#define SERVER_1_HEADER_LOG             ""
#define SERVER_1_HEADER_EVENT           ""

#define SERVER_1_SEND_NORMAL            DATALOG_UPLOAD_MAX_30
#endif

//#if ENABLE_MULTI_SERVER
// Extended server information
#if GSM_SERVER_EXT == GSM_SERVER_JASS
#define SERVER_2_IP_ADDR                SERVER_JASS_IP_ADDR

#define SERVER_2_IP_ADDR_BYTE3          SERVER_JASS_IP_ADDR_BYTE3
#define SERVER_2_IP_ADDR_BYTE2          SERVER_JASS_IP_ADDR_BYTE2
#define SERVER_2_IP_ADDR_BYTE1          SERVER_JASS_IP_ADDR_BYTE1
#define SERVER_2_IP_ADDR_BYTE0          SERVER_JASS_IP_ADDR_BYTE0

#define SERVER_2_DOMAIN_NAME            SERVER_JASS_DOMAIN_NAME

#define SERVER_2_PORT                   SERVER_JASS_PORT

#define SERVER_2_ENABLE_CHK             SERVER_JASS_ENABLE_CHK
#define SERVER_2_ENABLE_LOG             SERVER_JASS_ENABLE_LOG
#define SERVER_2_ENABLE_EVENT           SERVER_JASS_ENABLE_EVENT

#define SERVER_2_HEADER_CHK             SERVER_JASS_HEADER_CHK
#define SERVER_2_HEADER_LOG             SERVER_JASS_HEADER_LOG
#define SERVER_2_HEADER_EVENT           SERVER_JASS_HEADER_EVENT

#define SERVER_2_SEND_NORMAL            SERVER_JASS_SEND_NORMAL
#elif GSM_SERVER_EXT == GSM_SERVER_VDRS
#define SERVER_2_IP_ADDR                SERVER_VDRS_IP_ADDR

#define SERVER_2_IP_ADDR_BYTE3          SERVER_VDRS_IP_ADDR_BYTE3
#define SERVER_2_IP_ADDR_BYTE2          SERVER_VDRS_IP_ADDR_BYTE2
#define SERVER_2_IP_ADDR_BYTE1          SERVER_VDRS_IP_ADDR_BYTE1
#define SERVER_2_IP_ADDR_BYTE0          SERVER_VDRS_IP_ADDR_BYTE0

#define SERVER_2_DOMAIN_NAME            SERVER_VDRS_DOMAIN_NAME

#define SERVER_2_PORT                   SERVER_VDRS_PORT

#define SERVER_2_ENABLE_CHK             SERVER_VDRS_ENABLE_CHK
#define SERVER_2_ENABLE_LOG             SERVER_VDRS_ENABLE_LOG
#define SERVER_2_ENABLE_EVENT           SERVER_VDRS_ENABLE_EVENT

#define SERVER_2_HEADER_CHK             SERVER_VDRS_HEADER_CHK
#define SERVER_2_HEADER_LOG             SERVER_VDRS_HEADER_LOG
#define SERVER_2_HEADER_EVENT           SERVER_VDRS_HEADER_EVENT

#define SERVER_2_SEND_NORMAL            SERVER_VDRS_SEND_NORMAL
#elif GSM_SERVER_EXT == GSM_SERVER_VDRS_2
#define SERVER_2_IP_ADDR                SERVER_VDRS_2_IP_ADDR

#define SERVER_2_IP_ADDR_BYTE3          SERVER_VDRS_2_IP_ADDR_BYTE3
#define SERVER_2_IP_ADDR_BYTE2          SERVER_VDRS_2_IP_ADDR_BYTE2
#define SERVER_2_IP_ADDR_BYTE1          SERVER_VDRS_2_IP_ADDR_BYTE1
#define SERVER_2_IP_ADDR_BYTE0          SERVER_VDRS_2_IP_ADDR_BYTE0

#define SERVER_2_DOMAIN_NAME            SERVER_VDRS_2_DOMAIN_NAME

#define SERVER_2_PORT                   SERVER_VDRS_2_PORT

#define SERVER_2_ENABLE_CHK             SERVER_VDRS_2_ENABLE_CHK
#define SERVER_2_ENABLE_LOG             SERVER_VDRS_2_ENABLE_LOG
#define SERVER_2_ENABLE_EVENT           SERVER_VDRS_2_ENABLE_EVENT

#define SERVER_2_HEADER_CHK             SERVER_VDRS_2_HEADER_CHK
#define SERVER_2_HEADER_LOG             SERVER_VDRS_2_HEADER_LOG
#define SERVER_2_HEADER_EVENT           SERVER_VDRS_2_HEADER_EVENT

#define SERVER_2_SEND_NORMAL            SERVER_VDRS_2_SEND_NORMAL
#elif GSM_SERVER_EXT == GSM_SERVER_GOV
#define SERVER_2_IP_ADDR                SERVER_GOV_IP_ADDR

#define SERVER_2_IP_ADDR_BYTE3          SERVER_GOV_IP_ADDR_BYTE3
#define SERVER_2_IP_ADDR_BYTE2          SERVER_GOV_IP_ADDR_BYTE2
#define SERVER_2_IP_ADDR_BYTE1          SERVER_GOV_IP_ADDR_BYTE1
#define SERVER_2_IP_ADDR_BYTE0          SERVER_GOV_IP_ADDR_BYTE0

#define SERVER_2_DOMAIN_NAME            SERVER_GOV_DOMAIN_NAME

#define SERVER_2_PORT                   SERVER_GOV_PORT

#define SERVER_2_ENABLE_CHK             SERVER_GOV_ENABLE_CHK
#define SERVER_2_ENABLE_LOG             SERVER_GOV_ENABLE_LOG
#define SERVER_2_ENABLE_EVENT           SERVER_GOV_ENABLE_EVENT

#define SERVER_2_HEADER_CHK             SERVER_GOV_HEADER_CHK
#define SERVER_2_HEADER_LOG             SERVER_GOV_HEADER_LOG
#define SERVER_2_HEADER_EVENT           SERVER_GOV_HEADER_EVENT

#define SERVER_2_SEND_NORMAL            SERVER_GOV_SEND_NORMAL
#else
#define SERVER_2_IP_ADDR                ""

#define SERVER_2_IP_ADDR_BYTE3          0U
#define SERVER_2_IP_ADDR_BYTE2          0U
#define SERVER_2_IP_ADDR_BYTE1          0U
#define SERVER_2_IP_ADDR_BYTE0          0U

#define SERVER_2_DOMAIN_NAME            ""

#define SERVER_2_PORT                   0U

#define SERVER_2_ENABLE_CHK             0U
#define SERVER_2_ENABLE_LOG             0U
#define SERVER_2_ENABLE_EVENT           0U

#define SERVER_2_HEADER_CHK             ""
#define SERVER_2_HEADER_LOG             ""
#define SERVER_2_HEADER_EVENT           ""

#define SERVER_2_SEND_NORMAL            DATALOG_UPLOAD_MAX_30
#endif
//#endif

// SIM PW
#define GSM_SIM_PIN_DEF                 ""
#define GSM_SIM_PUK_DEF                 ""

// CSQ
#define CSQ_GOOD_THRESHOLD              18U

#define CSQ_POOR_THRESHOLD              16U             

// Local port
#define TCP_LOCAL_PORT                  0U

// APN
//#define TCP_APN_NAME                    "internet"
// APN Compatible for EC200
#define TCP_APN_NAME                    ""

#define TCP_USER_NAME                   ""

#define TCP_USER_PW                     ""

// Server default DNS
#define TCP_DNS_NAME_DEF                "vdrs-jasslin.com"

#define TCP_DNS_PORT_DEF                8208U

#define TCP_DNS_NAME_DISABLE            "NONE"

// FTP functions config
#define FTP_SERVER_DOMAIN_NAME          "ftp.jasslin.com" // FTP server domain name
//#define FTP_SERVER_DOMAIN_NAME          "ftp.9284.com.tw" // FTP server domain name

#define FTP_SERVER_IP_ADDR              "60.251.157.49" // FTP server IP address

#define FTP_SERVER_PORT                 21 // FTP server port

#define FTP_SERVER_DIR                  "/OTA/" // Firmware upgrade directory

#define FTP_ACC_USERNAME                "user" // FTP user account name
//#define FTP_ACC_USERNAME                "jasslin" // FTP user account name
//#define FTP_ACC_USERNAME                "jasslin" // FTP user account name

#define FTP_ACC_PASSWORD                "user" // FTP user account password
//#define FTP_ACC_PASSWORD                "jasslin" // FTP user account password
//#define FTP_ACC_PASSWORD                "FDDBNHAyytdz" // FTP user account password

#define FTP_ACC_USERNAME_ADMIN          "jasslin" // FTP admin account name

#define FTP_ACC_PASSWORD_ADMIN          "jasslin" // FTP admin account password

// FTP functions config
#define FTP_SERVER_DOMAIN_NAME_EXT      "ftp.jasslin.com" // FTP server domain name

#define FTP_SERVER_IP_ADDR_EXT          "60.251.157.49" // FTP server IP address

#define FTP_SERVER_PORT_EXT             21 // FTP server port

#define FTP_SERVER_DIR_EXT              "VDR" // VDR file directory

#define FTP_ACC_USERNAME_EXT            "user" // FTP user account name

#define FTP_ACC_PASSWORD_EXT            "user" // FTP user account password

//#define FTP_UPGRADE_FILE                "JAS106U_051_052_107.dfu"

#define PHONE_NUMBER_DEFAULT            "0978046944" // Default phone number

#define GSM_SEND_LOG_RETRY_MAX          2U // Maximum send log retry

#define SMS_PKG_TEMP_FILE               "SMS_PKG_TEMP.tmp" // Temporary file to store SMS packages

#define SEND_PKG_TEMP_FILE              "SEND_PKG_TEMP.tmp" // Temporary file to store TCP packets

#define SEND_PKG_EXT_TEMP_FILE          "SEND_PKG_EXT_TEMP.tmp" // Temporary file to store TCP packets for EXT server

#if ENABLE_FTP_DOWNLOAD
#define FTP_DOWNLOAD_TEMP_FILE          "FTP_DOWNLOAD_TEMP.tmp" // Temporary file to store FTP download request
#endif

#define SEND_FILE_TEMP_FILE             "SEND_FILE_TEMP.tmp" // Temporary file to store FTP upload request

#if ENABLE_UPLOAD_CHK
//#define UPLOAD_CHK_TEMP_FILE            "UPLOAD_CHECK_TEMP.tmp" // Temporary file to store the uploaded file checking request
#define UPLOAD_CHK_TEMP_FILE            "UPLOAD_CHK_TEMP.tmp" // Temporary file to store the uploaded file checking request

#define UPLOAD_CHK_NUM_MAX              20U // Maximum number of upload check requests
#endif

#define TEMP_PKG_SIZE                   1024U // Size of a temporary packet

#define GSM_SMS_NUM_MAX                 100U // Store maximum 100 SMS 

#define SMS_PKG_CHK_1                   0xA5
#define SMS_PKG_CHK_2                   0xC3

//#define GSM_PACKET_NUM_MAX              1500U // Store maximum 1500 files for resend
#define GSM_PACKET_NUM_MAX              2000U // Store maximum 2000 files for resend

#define GSM_FTP_UPLOAD_CHK_SCAN_MAX     20U // Upload check - maximum number of upload requests to scan

#define GSM_FUNCTION_RETRY_MAX          3U // Maximum normal GSM function retry

#define GSM_SIM_QUERY_MAX               3U // Maximum retry to query USIM status

#define GSM_PDP_RETRY_MAX               2U // Maximum retry for a step in PDP context activate process

#define GSM_PDP_ACTIV_RETRY_MAX         5U // Maximum PDP context activate process retry

#define STOP_PDP_ACT_TIMEOUT            60U // Waiting time to retry to activate the PDP context again

//#define PDP_ACT_STOP_MAX                15U // Maximum PDP activation stop

#define TCP_OPEN_CONN_RETRY_MAX         3U // Maximum TCP/IP opening connection retry

#define TCP_CONNECT_RETRY_MAX           3U // Maximum TCP/IP connecting process retry 

#define STOP_TCP_CONNECT_TIMEOUT        60U // Waiting time to retry to connect to TCP server again

#define TCP_CONNECT_STOP_MAX            30U // Maximum TCP/IP connecting stop

#define FTP_UPLOAD_TIMEOUT              500U // Timeout for FTP file upload function

#define FTP_DOWNLOAD_TIMEOUT            90U // Timeout for FTP file download function

#define FOTA_RETRY_MAX                  3U // Maximum FOTA retry

#define FOTA_STOP_TIMEOUT               30U // Waiting time to retry a FOTA step 

#define FILE_DOWNLOAD_RETRY_MAX         3U // Maximum file download retry

#define FILE_DOWNLOAD_STOP_TIMEOUT      30U // Waiting time to retry downloading a file

#define FILE_UPLOAD_RETRY_MAX           3U // Maximum FTP file upload retry

#define FILE_UPLOAD_STOP_TIMEOUT        30U // Waiting time to retry a step to upload file to FTP server

#define FTP_ACCESS_RETRY_MAX            3U // Maximum FTP server access retry

#define STOP_FTP_ACCESS_TIMEOUT         120U // Waiting time to retry an operation that need to access FTP server

#define STOP_UPLOAD_VDR_TIMEOUT         1800U // Waiting time to retry upload a VDR file in case unlimit reset

#define FTP_ACCESS_STOP_MAX             2U // Maximum FTP access stop 

#define HTTP_EPO_GPS_FILE_URL           "http://wepodownload.mediatek.com/EPO_GPS_3_1.DAT" // URL for GPS only EPO file
#define HTTP_EPO_GR_FILE_URL            "http://wepodownload.mediatek.com/EPO_GR_3_1.DAT" // Path for GPS+GLONASS EPO file

#define HTTP_DOWNLOAD_STOP_TIMEOUT      30U // Waiting time to retry download file from HTTP link

#define GPS_EPO_FILE_NAME_RAM           "UFS:EPO.bin"

#define GPS_XTRA_FILE_URL               "http://xtrapath4.izatcloud.net/xtra2.bin"

#define GPS_XTRA_FILE_NAME              "UFS:xtra2.bin" // GPS XTRA file name in RAM

#define XTRA_DOWNLOAD_STOP_TIMEOUT      30U // Waiting time to retry downloading XTRA file

// Backup setting directory on FTP server
#if ENABLE_FTP_BACKUP
#define DIRNAME_FTP_BACKUP_SETTING      "SETTING"
#endif
#endif

#define CMD_ID_MAX                      90U // Highest ID of commands


//============================================================================//
// User Typedef Variables
//============================================================================//
#if ENABLE_GSM_NET
// GSM events
typedef enum
{
	GSM_EVENT_NULL = 0,

	GSM_EVENT_INIT,

	GSM_EVENT_PROCESS,
	

	GSM_EVENT_SIM_READY,

	GSM_EVENT_SIM_NOT_READY,
	
	GSM_EVENT_SIM_NOT_INSERTED,

	GSM_EVENT_SIM_PIN_REQUIRED,
	
	GSM_EVENT_SIM_PUK_REQUIRED,

	GSM_EVENT_QUSIM_OK,
	
	GSM_EVENT_ME_FUNC_READY,

	GSM_EVENT_SMS_READY,

	GSM_EVENT_PHONEBOOK_READY,
	
	GSM_EVENT_POWERED_DOWN,


	GSM_EVENT_RECV_MSG,

	GSM_EVENT_RECV_CALL,

	GSM_EVENT_RECV_PACKET,

	GSM_EVENT_CONN_CLOSED,

	GSM_EVENT_PDP_DEACT,

	GSM_EVENT_NO_CARRIER,
	
	GSM_EVENT_NO_DIALTONE,
	
	GSM_EVENT_NO_ANSWER,
	
	GSM_EVENT_BUSY,


	GSM_EVENT_SMS_SEND_SUCCESS,

	GSM_EVENT_SMS_SEND_ERROR,

	GSM_EVENT_SMS_SEND_NO_RESPONSE,
	
	
	GSM_EVENT_CALL_ANS_SUCCESS,
	
	GSM_EVENT_CALL_ANS_ERROR,
	
	GSM_EVENT_CALL_ANS_NO_RESPONSE,
	
	
	GSM_EVENT_CALL_DISCONN_SUCCESS,
	
	GSM_EVENT_CALL_DISCONN_ERROR,
	
	GSM_EVENT_CALL_DISCONN_NO_RESPONSE,
	
	
	GSM_EVENT_PD_PSSTATE_SUCCESS,
	
	GSM_EVENT_PD_PSSTATE_ERROR,
	
	GSM_EVENT_PD_PSSTATE_NO_RESPONSE,
	
	
	GSM_EVENT_PD_PDPACT_SUCCESS,
	
	GSM_EVENT_PD_PDPACT_ERROR,
	
	GSM_EVENT_PD_PDPACT_NO_RESPONSE,


	GSM_EVENT_TCP_OPEN_SUCCESS,

	GSM_EVENT_TCP_OPEN_ERROR,

	GSM_EVENT_TCP_OPEN_NO_RESPONSE,


	GSM_EVENT_FTP_OPEN_SUCCESS,

	GSM_EVENT_FTP_OPEN_ERROR,

	GSM_EVENT_FTP_OPEN_NO_RESPONSE,


	GSM_EVENT_FTP_SETDIR_SUCCESS,

	GSM_EVENT_FTP_SETDIR_ERROR,

	GSM_EVENT_FTP_SETDIR_NO_RESPONSE,
	
	
	GSM_EVENT_FTP_MAKEDIR_SUCCESS,

	GSM_EVENT_FTP_MAKEDIR_ERROR,

	GSM_EVENT_FTP_MAKEDIR_NO_RESPONSE,


  GSM_EVENT_FTP_UPLOAD_SUCCESS,
	
	GSM_EVENT_FTP_UPLOAD_ERROR,
	
	GSM_EVENT_FTP_UPLOAD_NO_RESPONSE,
	

	GSM_EVENT_FTP_DOWNLOAD_CONNECT,

	GSM_EVENT_FTP_DOWNLOAD_SUCCESS,

	GSM_EVENT_FTP_DOWNLOAD_ERROR,

	GSM_EVENT_FTP_DOWNLOAD_NO_RESPONSE,


	GSM_EVENT_FTP_GETSIZE_SUCCESS,

	GSM_EVENT_FTP_GETSIZE_ERROR,

	GSM_EVENT_FTP_GETSIZE_NO_RESPONSE,


	GSM_EVENT_FTP_GETDATE_SUCCESS,

	GSM_EVENT_FTP_GETDATE_ERROR,

	GSM_EVENT_FTP_GETDATE_NO_RESPONSE,


	GSM_EVENT_FTP_TRANSLEN_SUCCESS,

	GSM_EVENT_FTP_TRANSLEN_ERROR,

	GSM_EVENT_FTP_TRANSLEN_NO_RESPONSE,
	
	
	GSM_EVENT_FTP_FILEDEL_SUCCESS,
  
	GSM_EVENT_FTP_FILEDEL_ERROR,

	GSM_EVENT_FTP_FILEDEL_NO_RESPONSE,
	

	GSM_EVENT_FTP_CLOSE_SUCCESS,

	GSM_EVENT_FTP_CLOSE_ERROR,

	GSM_EVENT_FTP_CLOSE_NO_RESPONSE,
	
	
	GSM_EVENT_HTTP_GET_SUCCESS,

	GSM_EVENT_HTTP_GET_ERROR,

	GSM_EVENT_HTTP_GET_NO_RESPONSE,
	
	
	GSM_EVENT_HTTP_READFILE_SUCCESS,

	GSM_EVENT_HTTP_READFILE_ERROR,

	GSM_EVENT_HTTP_READFILE_NO_RESPONSE,

} GSM_EventTypeDef;

// Non-block command execution result
typedef enum
{
	GSM_NONBLOCK_RES_NULL = 0U,
	
	GSM_NONBLOCK_RES_SUCCESS,
	
	GSM_NONBLOCK_RES_ERROR,
	
	GSM_NONBLOCK_RES_NO_RESPONSE,

} GSM_NonBlockCMD_Res_Typedef;

// GSM Non-Block functions results
typedef enum
{
	GSMRES_NULL = 0U,
	
	GSMRES_CMD_SENT, // 1, Command sent, next step is waiting for result
	
  GSMRES_SUCCESS, // 2, Command is successful
	
	GSMRES_FAIL_NO_REDO, // 3, Command is failed, no need to redo
	
	GSMRES_FAIL_REDO_LIMIT, // 4, Command is failed, can redo within a limit
	
	GSMRES_FAIL_REDO_LIMIT_RST, // 5, Command is failed, can redo within a limit and the GSM module is reset each time
	
	GSMRES_FAIL_REDO_LIMIT_DELAY, // 6, Command is failed, can redo within a limit, have delay between each time
	
	GSMRES_FAIL_REDO_LIMIT_DELAY_RST, // 7, Command is failed, can redo within a limit, have delay and the GSM module is reset each time 
	
	GSMRES_FAIL_REDO_UNLIMIT_DELAY, // 8, Command is failed, can redo with no limitation and have delay between each time
	
	GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST // 9, Command is failed, can redo with no limitation, have delay and the GSM module is reset each time
	
} GSM_NonBlockResp_ResTypeDef;

// GSM command packet source
typedef enum
{
	GSM_CMD_SRC_NULL = 0U,

	GSM_CMD_SRC_SMS,

	GSM_CMD_SRC_TCP_1,

  #if ENABLE_MULTI_SERVER
	GSM_CMD_SRC_TCP_2
  #endif

} GSM_CmdSrcTypeDef;

typedef enum
{
	PDP_STEP_NULL,
	
	PDP_STEP_QUERY_USIM,
	
	PDP_STEP_QUERY_CS,
	
	PDP_STEP_QUERY_PS,
	
	PDP_STEP_CONFIG,
	
	PDP_STEP_ACT,
	
	PDP_STEP_DEACT
	
} PDP_ActStepTypeDef;

typedef enum
{
	CONN_STEP_NULL = 0U,
	
  CONN_STEP_SOCKET_SEL,
	
	CONN_STEP_OPEN,
	
	CONN_STEP_OPEN_WAIT,
	
	CONN_STEP_SEND_CHK
	
} TCP_ConnectStepTypeDef;

typedef enum
{
	FOTA_STEP_NULL = 0U, // 0
	
  FOTA_STEP_CFG_PDP, // 1
	
	FOTA_STEP_CFG_ACC, // 2
	
	FOTA_STEP_CFG_FILE_TYPE, // 3
	
	FOTA_STEP_CFG_TRANS_MODE, // 4
	
	FOTA_STEP_CFG_TIMEOUT, // 5
	
	FOTA_STEP_OPEN, // 6
	
	FOTA_STEP_OPEN_WAIT, // 7
	
	FOTA_STEP_SET_DIR, // 8
	
	FOTA_STEP_SET_DIR_WAIT, // 9
	
	FOTA_STEP_GET_SIZE, // 10
	
	FOTA_STEP_GET_SIZE_WAIT, // 11
	
	FOTA_STEP_RAM_DEL, // 12
	
	FOTA_STEP_DOWNLOAD, // 13
	
	FOTA_STEP_DOWNLOAD_WAIT, // 14
	
	FOTA_STEP_CLOSE, // 15
	
	FOTA_STEP_CLOSE_WAIT, // 16
	
	FOTA_STEP_RAM_OPEN, // 17
	
	FOTA_STEP_CHECK_INFO, // 18
	
	FOTA_STEP_SAVE_FILE_INIT, // 19
	
	FOTA_STEP_SET_POINTER, // 20
	
	FOTA_STEP_SAVE_DATA, // 21
	
	FOTA_STEP_RAM_CLOSE, // 22
	
	FOTA_STEP_RESET_UPGRADE, //23
	
	FOTA_STEP_REDO // 24
	
} FOTA_StepTypeDef;

#if ENABLE_FTP_DOWNLOAD
typedef enum
{
	DOWNLOAD_STEP_NULL = 0U, // 0
	
	DOWNLOAD_STEP_INIT, // 1
	
  DOWNLOAD_STEP_CFG_PDP, // 2
	
	DOWNLOAD_STEP_CFG_ACC, // 3
	
	DOWNLOAD_STEP_CFG_FILE_TYPE, // 4
	
	DOWNLOAD_STEP_CFG_TRANS_MODE, // 5
	
	DOWNLOAD_STEP_CFG_TIMEOUT, // 6
	
	DOWNLOAD_STEP_OPEN, // 7
	
	DOWNLOAD_STEP_OPEN_WAIT, // 8
	
	DOWNLOAD_STEP_SET_DIR, // 9
	
	DOWNLOAD_STEP_SET_DIR_WAIT, // 10
	
	DOWNLOAD_STEP_GET_SIZE, // 11
	
	DOWNLOAD_STEP_GET_SIZE_WAIT, // 12
	
	DOWNLOAD_STEP_RAM_DEL, // 13
	
	DOWNLOAD_STEP_DOWNLOAD, // 14
	
	DOWNLOAD_STEP_DOWNLOAD_WAIT, // 15
	
	DOWNLOAD_STEP_CLOSE, // 16
	
	DOWNLOAD_STEP_CLOSE_WAIT, // 17
	
	DOWNLOAD_STEP_RAM_OPEN, // 18
	
	DOWNLOAD_STEP_SAVE_FILE_INIT, // 19
	
	DOWNLOAD_STEP_SAVE_FILE, // 20
	
	DOWNLOAD_STEP_RAM_CLOSE, // 21
	
	DOWNLOAD_STEP_REDO // 22
	
} Download_StepTypeDef;
#endif

typedef enum
{ 
	UPLOAD_STEP_NULL = 0U, // 0
	
  UPLOAD_STEP_INIT, // 1
	
	UPLOAD_STEP_RAM_DEL, // 2
	
	UPLOAD_STEP_RAM_OPEN, // 3
	
	UPLOAD_STEP_SET_POINTER, // 4
	
	UPLOAD_STEP_LOAD_FILE, // 5
	
	UPLOAD_STEP_RAM_CLOSE, // 6
	 
	UPLOAD_STEP_CFG_PDP, // 7
	
	UPLOAD_STEP_CFG_ACC, // 8
	
	UPLOAD_STEP_CFG_FILE_TYPE, // 9
	
	UPLOAD_STEP_CFG_TRANS_MODE, // 10
	
	UPLOAD_STEP_CFG_TIMEOUT, // 11
	
	UPLOAD_STEP_SSL_MODE, // 12
	
	UPLOAD_STEP_SSL_PDP, // 13
	
	UPLOAD_STEP_SSL_VER, // 14
	
	UPLOAD_STEP_SSL_CIPHER, // 15
	
	UPLOAD_STEP_SSL_CA, // 16
	
	UPLOAD_STEP_SSL_SEC, // 17
	
	UPLOAD_STEP_OPEN, // 18
	
	UPLOAD_STEP_OPEN_WAIT, // 19
	
	UPLOAD_STEP_SET_DIR, // 20
	
	UPLOAD_STEP_SET_DIR_WAIT, // 21
	
	UPLOAD_STEP_MAKE_DIR, // 22
	
	UPLOAD_STEP_MAKE_DIR_WAIT, // 23
	
	UPLOAD_STEP_GET_SIZE, // 24
	
	UPLOAD_STEP_GET_SIZE_WAIT, // 25
	
	UPLOAD_STEP_FILE_DEL, // 26
	
	UPLOAD_STEP_FILE_DEL_WAIT, // 27
	
	UPLOAD_STEP_UPLOAD, // 28
	
	UPLOAD_STEP_UPLOAD_WAIT, // 29
	
	UPLOAD_STEP_CHECK_SIZE, // 30
	
	UPLOAD_STEP_CHECK_SIZE_WAIT, // 31
	
	UPLOAD_STEP_CLOSE, // 32
	
	UPLOAD_STEP_CLOSE_WAIT, // 33
	
	UPLOAD_STEP_REDO // 34
	
} FTP_UploadStepTypeDef;

#if ENABLE_UPLOAD_CHK
typedef enum
{ 
	UPLOAD_CHK_STEP_NULL = 0U, // 0
	
  UPLOAD_CHK_STEP_INIT, // 1

	UPLOAD_CHK_STEP_CFG_PDP, // 2
	
	UPLOAD_CHK_STEP_CFG_ACC, // 3
	
	UPLOAD_CHK_STEP_CFG_TIMEOUT, // 4
	
	UPLOAD_CHK_STEP_OPEN, // 5
	
	UPLOAD_CHK_STEP_OPEN_WAIT, // 6
	
	UPLOAD_CHK_STEP_SET_DIR, // 7
	
	UPLOAD_CHK_STEP_SET_DIR_WAIT, // 8
	
	UPLOAD_CHK_STEP_GET_SIZE, // 9
	
	UPLOAD_CHK_STEP_GET_SIZE_WAIT, // 10
	
	UPLOAD_CHK_STEP_CLOSE, // 11
	
	UPLOAD_CHK_STEP_CLOSE_WAIT, // 12
	
	UPLOAD_CHK_STEP_FINISH // 13
	
} FTP_UploadChkStepTypeDef;

typedef enum
{
	HTTP_DOWNLOAD_STEP_NULL = 0U, // 0
	
	HTTP_DOWNLOAD_STEP_INIT, // 1
	
  HTTP_DOWNLOAD_STEP_CFG_PDP, // 2
	
	HTTP_DOWNLOAD_STEP_CFG_RESP, // 3
	
	HTTP_DOWNLOAD_STEP_SET_URL, // 4
	
	HTTP_DOWNLOAD_STEP_GET, // 5
	
	HTTP_DOWNLOAD_STEP_GET_WAIT, // 6
	
	HTTP_DOWNLOAD_STEP_READ, // 7
	
	HTTP_DOWNLOAD_STEP_READ_WAIT, // 8
	
	HTTP_DOWNLOAD_STEP_STOP, // 9
	
	HTTP_DOWNLOAD_STEP_GET_SIZE, // 10
	
	HTTP_DOWNLOAD_STEP_RAM_OPEN, // 11
	
	HTTP_DOWNLOAD_STEP_SAVE_FILE_INIT, // 12
	
	HTTP_DOWNLOAD_STEP_SAVE_FILE, // 13
	
	HTTP_DOWNLOAD_STEP_RAM_CLOSE, // 14
	
	HTTP_DOWNLOAD_STEP_FINISH // 15
	
} HTTP_DownloadStepTypeDef;

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21 && ENABLE_GPS_XTRA
#if 0
typedef enum
{
	XTRA_STEP_NULL = 0U, // 0
	
  XTRA_STEP_HTTP_CFG_PDP, // 1
	
	XTRA_STEP_HTTP_CFG_RESP, // 2
	
	XTRA_STEP_HTTP_URL, // 3
	
	XTRA_STEP_HTTP_GET, // 4
	
	XTRA_STEP_HTTP_GET_WAIT, // 5
	
	XTRA_STEP_HTTP_READ, // 6
	
	XTRA_STEP_HTTP_READ_WAIT, // 7
	
	XTRA_STEP_HTTP_STOP, // 8
	
	XTRA_STEP_SET_TIME, // 9
	
	XTRA_STEP_ACT_DATA, // 10
	
	XTRA_STEP_VERIFY, // 11
	
	XTRA_STEP_FINISH // 12
	
} XTRA_DownloadStepTypeDef;
#endif

typedef enum
{
	XTRA_UPDATE_STEP_NULL = 0U, // 0
	
	XTRA_UPDATE_STEP_INIT, // 1
	
	XTRA_UPDATE_STEP_SET_TIME, // 2
	
	XTRA_UPDATE_STEP_ACT_DATA, // 3
	
	XTRA_UPDATE_STEP_VERIFY, // 4
	
	XTRA_UPDATE_STEP_FINISH // 5
	
} XTRA_UpdateStepTypeDef;
#endif

typedef enum
{
  FTP_DOWNLOAD_NORMAL = 0U,
	
	FTP_DOWNLOAD_FOTA,
	
	FTP_DOWNLOAD_SETTING
	
} FTP_DownloadTypeDef;

typedef enum
{
  CURR_SERVER_NONE = 0,
	
	CURR_SERVER_TCP,
	
	CURR_SERVER_TCP_EXT,
	
	CURR_SERVER_FTP,
	
	CURR_SERVER_FTP_EXT
	
} GSM_CurrServTypeDef;
#endif

// Message command components
#pragma pack(1)
typedef struct
{
	char *pcShort;

	char *pcSET;

} MsgCmdTypeDef;

typedef struct
{
	uint8_t u8Chk1;
	
	char cPhoneNum[15];

	char cSMS[256];

	uint16_t u16Len;
	
	uint8_t u8RetryCnt;
	
	uint8_t u8Chk2;
	
} SMS_PkgTypeDef;

typedef struct
{
	bool bInUse;

	bool bInRequest;

	char cEventCode[10];

	char cEventMsg[128];

} GSM_EventDataRequestTypeDef;

#if ENABLE_FTP_DOWNLOAD
typedef struct
{
  char cSrcFile[64];
	
	char cDstFile[64];
	
	char cSrcDir[64];
	
	char cDstDir[64];
	
	uint8_t u8ServerID;
	
	uint8_t u8ReDownloadCnt;
	
	FTP_DownloadTypeDef dlType;
	
} GSM_DownloadRequestTypeDef;
#endif

typedef struct
{
  char cSrcFile[64];
	
	char cDstFile[64];
	
	char cSrcDir[64];
	
	char cDstDir[64];
	
	uint32_t u32UploadSize;
	
	uint8_t u8ServerID;
	
	uint8_t u8ReUploadCnt;
	
} GSM_UploadRequestTypeDef;

#if ENABLE_UPLOAD_CHK
typedef struct
{
  //bool bRequireChk;
	
	char cSrcFile[64];
	
	char cDstFile[64];
	
	char cSrcDir[64];
	
	char cDstDir[64];
	
	uint8_t u8ServerID;
	
	uint8_t u8ReChkCnt;
	
} GSM_UploadChkTypeDef;
#endif

typedef struct
{
	char cIP_Addr[31];
	
	uint16_t u16Port;
	
} IP_InvalidTypeDef;

typedef struct 
{
  uint16_t u16Err;
	
	uint16_t u16HTTP_Err;
	
	uint32_t u32ContentLen;
	
} HTTP_GET_TypeDef;

typedef void (*HTTP_CpltCallback)(char *, uint32_t);

typedef struct
{
	bool bRequest;
	
	uint8_t u8Type;
	
	char cSrcPath[128];
	
	char cDstDir[64];
	
	char cSrcFileName[64];
	
	char cDstFileName[64];
	
	uint8_t u8RetryCnt;
	
	HTTP_CpltCallback callback;
	
} HTTP_DownloadRequestTypeDef;

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21 && ENABLE_GPS_XTRA
#if 0
typedef struct 
{
  char cGPS_XTRA_File[64];
	
	uint32_t u32FileSize;
	
	bool bGPS_XTRA_Enable;
	
	bool bGPS_XTRA_Valid;
	
	bool bGPS_XTRA_Update;
	
	uint8_t u8GPS_XTRA_RetryCnt;
	
	HTTP_GET_TypeDef tGET_Resp;
	
	uint16_t u16ReadFileErr;
	
} GPS_XTRA_TypeDef;
#endif

typedef struct
{
	bool bRequest;
	
	char cFilePath[128];
	
	uint32_t u32FileSize;
	
} XTRA_RequestTypeDef;
#endif
#pragma pack()
#endif


//============================================================================//
// User Externs
//============================================================================//
#if ENABLE_GSM_NET
extern QtEC21_BlockCmdBuildTypeDef qtec21_BlockCmdInstance;
	
extern QtEC21_NonBlockCmdBuildTypeDef qtec21_NonBlockCmdInstance;

extern QTEC21_NonBlockCMD_TypeDef gsmCurrNonBlockCmd;

extern GSM_CmdSrcTypeDef gsmCmdSrc;

extern FIFO_IndexTypeDef tSMS_Index;

extern char cCmdPhoneNum[20];

extern bool bPDP_DeactFlag, bTCP_CloseFlag[MAX_SOCKET_BANK];

extern bool bFTP_ResetFlag;

extern TCP_ConnectStepTypeDef procGSM_TCP_Step[MAX_SOCKET_BANK];

extern uint8_t u8PDP_ActRetryCnt;

extern FOTA_StepTypeDef procGSM_FOTA_Step;

#if ENABLE_FTP_DOWNLOAD
extern GSM_DownloadRequestTypeDef gsmDownloadRequest;

extern uint16_t u16DownloadFileNum;

extern Download_StepTypeDef procGSM_Download_Step;
#endif

extern FTP_UploadStepTypeDef procGSM_Upload_Step;

extern GSM_UploadRequestTypeDef gsmUploadRequest;

extern HTTP_DownloadRequestTypeDef tHTTP_DlRequest;

extern HTTP_DownloadStepTypeDef procHTTP_Download_Step;

extern uint16_t u16UnsendPkgNum[MAX_SOCKET_BANK];

extern uint16_t u16UnsendFileNum;

#if ENABLE_UPLOAD_CHK
extern bool bDoUploadChk;

extern FIFO_IndexTypeDef tUploadChkHandle;
#endif

extern GSM_CurrServTypeDef gsmCurrServerType;

//extern GSM_MsgDataTypeDef gsmMsgInstance;

extern GSM_SocketDataTypeDef gsmTCP_CHK[MAX_SOCKET_BANK];
extern GSM_SocketDataTypeDef gsmTCP_DATA[MAX_SOCKET_BANK];
extern GSM_SocketDataTypeDef gsmTCP_LOG[MAX_SOCKET_BANK];

//extern uint8_t u8FileTransRetryCnt;

//extern uint32_t u32GetFileSize;

extern bool bIsIMEIReady;

extern bool bGSM_SetSleep;
extern bool bGSM_WakeUp;

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21 && ENABLE_GPS_XTRA
//extern GPS_XTRA_TypeDef tGPS_Xtra;

//extern XTRA_DownloadStepTypeDef procXTRA_Download_Step;

extern XTRA_RequestTypeDef tXTRA_Update;

extern XTRA_UpdateStepTypeDef procXTRA_Update_Step;
#endif

extern bool bStealthFOTA_En;
extern bool bStealthDownloadEn;
extern bool bStealthUploadEn;
#endif


//============================================================================//
// User Function Prototypes
//============================================================================//
#if ENABLE_GSM_NET
// GSM handling functions
void GSM_Task_Proc(void);
void GSM_RX_Proc(void);
void GSM_TX_Proc(void);
void GSM_LOG_Proc(uint8_t u8Dst);
void GSM_BlockCallBackHandle(void);
void GSM_NonBlockCallBackHandle(void);
void GSM_BlockCmdInit(void);
void GSM_NonBlockCmdInit(void);
void GSM_BuildBlockCmd(QTEC21_BlockCMD_TypeDef qtec21Cmd, uint8_t *pu8RetVal, uint8_t u8ArgNum,...);
void GSM_Handle_Block_osEventTimeout(void);

// Send data to server functions
TCP_StatusTypeDef GSM_PDP_ActProc(void);
TCP_StatusTypeDef GSM_TCP_Connect_Proc(uint8_t u8ConnDst);
void GSM_CheckTempFile(void);
void GSM_SMS_CheckUnread(void);
bool GSM_SaveTempPkg(uint8_t *pu8Buf, uint16_t u16Len, uint8_t u8Dst);
bool GSM_GetTempPkg(uint8_t *pu8Buf, uint16_t *pu16ReadLen, uint8_t u8Dst);
MsgSendStatusTypeDef GSM_TCP_SendCHK(uint8_t u8Dst);
bool GSM_TCP_SendEVENT_Direct(char *pcEventCode, char *pcEventMsg, uint8_t u8SocketID);
void GSM_TCP_SendLOG(DatalogUploadTypeDef *pUPLOAD, uint8_t u8SocketBank);
void GSM_TCP_ReportEvent(char *pcEventCode, char *pcEventMsg);
void GSM_TCP_ReSendLOG(DatalogUploadTypeDef *pUPLOAD, uint8_t u8SocketBank);
void GSM_TCP_ReSendEVENT(DatalogEventlogTypeDef stMyDatalogEvent, uint8_t u8SocketBank);

// Download firmware file on the FTP server
TCP_StatusTypeDef GSM_FOTA_Proc(void);

// Download file from FTP server
#if ENABLE_FTP_DOWNLOAD
bool GSM_FTP_FileDownloadRequest(GSM_DownloadRequestTypeDef *pDlHandle);
bool GSM_FTP_FileDownloadInit(void);
void GSM_FTP_IncDownloadIdx(void);
void GSM_FTP_DecDownloadIdx(void);
TCP_StatusTypeDef GSM_FTP_Download_Proc(void);
#endif

// Upload file to FTP server
bool GSM_FTP_FileUploadRequest(char *pcSrcFile, char *pcDstFile, char *pcSrcDir, 
	                             char *pcDstDir, uint8_t u8ServerID, uint8_t u8RetryCnt);
bool GSM_FTP_FileUploadInit(void);
bool GSM_FTP_FileUploadPeek(uint16_t u16Index, GSM_UploadRequestTypeDef *pRequest);
void GSM_FTP_IncUploadIdx(void);
void GSM_FTP_DecUploadIdx(void);
TCP_StatusTypeDef GSM_FTP_Upload_Proc(void);

// Check upload file on FTP server
#if ENABLE_UPLOAD_CHK
bool GSM_FTP_SaveUploadChk(GSM_UploadChkTypeDef *pgsmUploadChk);
bool GSM_FTP_InitUploadChk(void);
void GSM_FTP_UploadChkIncTail(void);
void GSM_FTP_UploadChkIncHead(void);
//void GSM_FTP_UploadChkHandle(void);
TCP_StatusTypeDef GSM_FTP_UploadChk_Proc(void);
#endif

bool GSM_HTTP_DownloadRequest(HTTP_DownloadRequestTypeDef *ptRequest);
void GSM_HTTP_DownloadInit(void);
TCP_StatusTypeDef GSM_HTTP_Download_Proc(void);

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21 && ENABLE_GPS_XTRA
//TCP_StatusTypeDef GSM_XTRA_Download_Proc(void);
void GSM_XTRA_RequestUpdate(char *pcFilePath, uint32_t u32Size);
void GSM_XTRA_UpdateInit(void);
TCP_StatusTypeDef GSM_XTRA_Update_Proc(void);
#endif

// Hardware control functions
void GSM_PwrEnable(void);
void GSM_PwrDisable(void);
void GSM_TurnOnPwrKey(void);
void GSM_TurnOffPwrKey(void);
void GSM_TurnOffCmd(void);
void GSM_Reset(void);
void GSM_ResetWithCheckFTP(void);

// Basic functions
bool GSM_AT_Test(void);
bool GSM_ReadCGMI(char *pcCGMI);
bool GSM_ReadCGMM(char *pcCGMM);
bool GSM_ReadCGMR(char *pcCGMR);
bool GSM_GetIMEI(char *pcIMEI);
bool GSM_SetFunc(uint8_t u8Func, bool bRst);
bool GSM_SaveConfigProf(uint8_t u8ConfigID);
bool GSM_SetConfigProf(uint8_t u8ConfigID);
bool GSM_ConfigEcho(bool bOption);
bool GSM_ConnCodeFormat(uint8_t u8Option);
bool GSM_SetErrFormat(uint8_t u8Option);
bool GSM_TECharSet(char* pcCharSet);
bool GSM_URC_Outport(char *pcOutPort);

// Serial interface function
bool GSM_COM_SetDTR(uint8_t u8Option);
bool GSM_COM_SetBaudRate(uint32_t u32BaudRate);

// Status control function
bool GSM_Stat_ConfigRI(char *pcOption);

// SIM functions
bool GSM_GetIMSI(char *pcIMSI);
bool GSM_SIM_QueryUSIM(GSM_SIM_StatTypedef *simRetCode);
bool GSM_SIM_SetPIN(char *pcOldPIN, char *pcNewPIN);

// Network functions
bool GSM_Net_QueryCS(void);
bool GSM_Net_GetCSQ(uint8_t *pu8RSSI, uint8_t *pu8Ber);
bool GSM_Net_GetTime(uint8_t u8Option, char *pcBuf);

// Data mode to command mode
bool GSM_Dat_2_Cmd(void);

// Messaging functions
bool GSM_SMS_SelectServ(uint8_t u8Type);
bool GSM_SMS_SetFormat(uint8_t u8Type);
bool GSM_SMS_SCA(char* pcSCA, uint16_t u16Type);
bool GSM_SMS_SetStorage(char *pcMem1, char *pcMem2, char *pcMem3);
bool GSM_SMS_Del(uint16_t u16Idx, uint8_t u8DelFlag);
bool GSM_SMS_GetList(char* pcStat, uint8_t* pu8Buf);
bool GSM_SMS_Read(uint16_t u16Idx, uint8_t* pu8Buf);
bool GSM_SMS_Send(GSM_MsgDataTypeDef *pgsmMsgData);
bool GSM_SMS_ReportConfig(uint8_t u8Mode, uint8_t u8Mt, uint8_t u8Bm,
                          uint8_t u8Ds, uint8_t u8Bfr);
bool GSM_SMS_SetParam(uint8_t u8Mode);
bool GSM_SMS_SavePkg(GSM_MsgDataTypeDef *pSMS_Data);
void GSM_SMS_PkgIncTail(void);
void GSM_SMS_PkgIncHead(void);

// Call functions
bool GSM_Call_Ans(void);
GSM_StatusTypeDef GSM_Call_Dial(char* pcPhoneNbr);
bool GSM_Call_Disconn(void);
bool GSM_Call_Cmd_2_Dat(void);
bool GSM_Call_ConnWaitTime(uint8_t u8Time);
bool GSM_Call_GetList(uint8_t* pu8Buf);

// Packet domain functions
bool GSM_PD_Attach(bool bOption);
bool GSM_PD_ConfigPDP(uint8_t u8ID, char *pcAPN, char *pcAddr);
bool GSM_PD_QualityReq(uint8_t u8ID, TCP_PDP_ContextStruct *pdpInstance);
bool GSM_PD_QualityMin(uint8_t u8ID, TCP_PDP_ContextStruct *pdpInstance);
bool GSM_PD_Activ(uint8_t u8ID, bool bOption);
bool GSM_PD_GetAddr(uint8_t u8ID, char *pcAddr);
bool GSM_PD_QueryPS(void);

// HW functions
bool GSM_HW_GetRTC(char *pcBuf);

// TCP/IP functions
bool GSM_TCP_ConfigPDP(uint8_t u8PDP_ID, char *pcAPN, char *pcUser_1_Name, char *pcPwd);
bool GSM_TCP_ActivPDP(uint8_t u8PDP_ID);
bool GSM_TCP_GetIP(uint8_t u8PDP_ID, char* pcIP_Addr);
bool GSM_TCP_DeactivPDP(uint8_t u8PDP_ID);
bool GSM_TCP_OpenConn(uint8_t u8PDP_ID, uint8_t u8ConnID, GSM_SocketConnTypeDef *pgsmConnHandle);
bool GSM_TCP_CloseConn(uint8_t u8SocketID, uint16_t u16TimeOut);
bool GSM_TCP_IsConnect(bool bQueryType, uint8_t u8ID, bool* bIsConnect);
MsgSendStatusTypeDef GSM_TCP_Send(GSM_SocketDataTypeDef *pgsmSocketData);
bool GSM_TCP_SendingStatus(uint8_t u8SocketID, uint16_t* pu16NACK_Bytes);
bool GSM_TCP_RecvDataLen(uint8_t u8SocketID, uint16_t* pu16RecvBytesNum);
bool GSM_TCP_GetRecvData(uint8_t u8SocketID, uint8_t * pu8Buf, uint16_t u16Len,
                         uint16_t * u16ReadLen);
bool GSM_TCP_Cmd_2_Dat(uint8_t u8SocketID);

// FTP functions
bool GSM_FTP_CfgPDP(uint8_t u8PDP_ID);
bool GSM_FTP_CfgAcc(char *pcUser_1_Name, char *pcPwd);
bool GSM_FTP_CfgFileType(void);
bool GSM_FTP_CfgTransMode(void);
bool GSM_FTP_CfgTimeout(uint16_t u16TimeOut);
bool GSM_FTP_CfgSSL_Type(bool bSSL_Type);
bool GSM_FTP_CfgSSL_ID(uint8_t u8SSL_ID);
bool GSM_FTP_SSL_Ver(uint8_t u8SSL_ID, uint8_t u8SSL_Ver);
bool GSM_FTP_SSL_Cipher(uint8_t u8SSL_ID, char *pcCipher);
bool GSM_FTP_SSL_CA(uint8_t u8SSL_ID, char *pcSSL_CA_Path);
bool GSM_FTP_SSL_SecLV(uint8_t u8SSL_ID, uint8_t u8SSL_SecLV);
bool GSM_FTP_Login(char *pcHostName, uint16_t u16Port);
bool GSM_FTP_SetDir(char *pcDirPath);
bool GSM_FTP_UploadFile(char *pcLocalName, char *pcFileName);
bool GSM_FTP_UploadDirect(uint8_t *pu8DatBuf, char *pcUploadFile, uint32_t u32UploadPos, 
	                        uint16_t u16UploadLen, bool bUploadFinish);
bool GSM_FTP_DownloadFile(char *pcFileName, char *pcLocalDest, uint32_t u32FilePtr);
bool GSM_FTP_FileSize(char *pcFileName);
bool GSM_FTP_FileDate(char *pcFileName);
bool GSM_FTP_TransLen(void);
bool GSM_FTP_FileDel(char *pcFileName);
bool GSM_FTP_Logout(void);

// HTTP functions
bool GSM_HTTP_CfgPDP(uint8_t u8PDP_ID);
bool GSM_HTTP_RespHeader(bool bEnable);
bool GSM_HTTP_SetURL(char *pcURL);
bool GSM_HTTP_Get(void);
bool GSM_HTTP_ReadFile(char *pcFileName);
bool GSM_HTTP_Cancel(void);

// File operation functions
bool GSM_FileList(char *pcPattern, uint8_t *pu8Resp, uint16_t *pu16RetLen);
bool GSM_FileUpload(char *pcFileName, char *pcLocalName);
bool GSM_FileOpen(char *pcFileName, uint32_t *pu32FileHandle, uint8_t u8Mode);
bool GSM_FileSeek(uint32_t u32FileHandle, uint32_t u32FilePtr);
bool GSM_FileRead(uint32_t u32FileHandle, uint32_t u32ReadLen, uint32_t *pu32ActualLen,
                  char *pcReadContent);
bool GSM_FileWrite(uint32_t u32FileHandle, uint8_t *pu8DatBuf, uint16_t u16WriteLen);
bool GSM_FileClose(uint32_t u32FileHandle);
bool GSM_FileDelete(char *cFileName);

void GPRS_DebugLog(char *pcLog, uint16_t u16Len);

// Hardware related functions
bool GSM_HW_SleepEn(bool bOption);
#endif // #if ENABLE_GSM_NET

//============================================================================//
#endif /* _GSM_DRIVER_H_ */
