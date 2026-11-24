#ifndef _GSM_QTEC21_H_
#define _GSM_QTEC21_H_
//============================================================================//

//============================================================================//
// User Includes
//============================================================================//
#include "stm32fxxx_hal_user.h"
#include "Config.h"
#include "DataType.h"
#include "GPS_Driver.h"
#include "UART_Driver.h"


//============================================================================//
// User Defines
//============================================================================//
#if ENABLE_GSM_QTEC21
#define	SOCKET_BANK_DEF			                     0U

#define	SOCKET_BANK_EXT			                     1U

#if ENABLE_MULTI_SERVER
#define	MAX_SOCKET_BANK			                     2U
#else
#define	MAX_SOCKET_BANK			                     1U
#endif

// AT command components //
#define CMD_BEGIN                                "AT"

#define CMD_END                                  "\r"

#define RESP_BEGIN                               "\r\n"

#define RESP_END                                 "\r\n"


//#define SEND_MSG_CMD                             0x1A // Ctrl+Z
#define SEND_MSG_CMD                             (char *)"\x1A" // Ctrl+Z
	
	
// Hardware control commands
#define CMD_QPOWD                                "+QPOWD" // Turn off module


// QTEC21 general commands //
#define CMD_CGMI                                 "+CGMI" // Request manufacturer Identification

#define CMD_CGMM                                 "+CGMM" // Request TA model identification

#define CMD_CGMR                                 "+CGMR" // Request TA Revision Identification of Software Release

#define CMD_CGSN                                 "+CGSN" // Request IMEI

#define CMD_CFUN                                 "+CFUN" // Set phone functionality

#define CMD_ATW                                  "&W" // Save current parameters to user defined profile

#define CMD_ATZ                                  "Z" // Set all current parameters to user defined profile

#define CMD_ATE                                  "E" // Set command echo mode

#define CMD_ATX                                  "X" // Set connect result code format

#define CMD_CMEE                                 "+CMEE" // Set error code format

#define CMD_CSCS                                 "+CSCS" // Select TE character set

#define CMD_QURCCFG                              "+QURCCFG" // Config URC outport           


// Serial interface functions //
#define CMD_AT_AND_D                             "+AT&D" // Set DTR function mode

#define CMD_IPR                                  "+IPR" // Set local baud rate


// Status control functions //
#define CMD_QCFG                                 "+QCFG" // Extended configuration settings


// SIM functions //
#define CMD_SIM_CIMI                             "+CIMI" // Request IMSI

#define CMD_SIM_CPIN                             "+CPIN" // Query USIM status

#define CMD_SIM_CPWD                             "+CPWD" // Change SIM password


// Network functions //
#define CMD_NET_CREG                             "+CREG?" // Query network registration on CS domain status 

#define CMD_NET_CSQ                              "+CSQ" // Query network service quality

#define CMD_NET_QLTS                             "+QLTS" // Obtain the latest time synchronized through network


// Data mode to command mode function //
#define CMD_DAT_2_CMD                            "+++" // Switch from data mode to command mode


// GNSS module configuration commands //
#define CMD_GNSS_CONFIG_QUERY                    "+QGPSCFG=?" // Query GPS module current status

#define CMD_GNSS_CONFIG_OUTPORT                  "+QGPSCFG=\"outport\"" // NMEA messages output 
                                                                                      // via UART debug port

#define CMD_GNSS_CONFIG_NMEASRC                  "+QGPSCFG=\"nmeasrc\",1" // Enable NMEA messages acquisition
                                                                          // via commands

#define CMD_GNSS_CONFIG_GPSNMEATYPE              "+QGPSCFG=\"gpsnmeatype\",11" // Send 3 types of messages
                                                                               // for GPS (GGA, GSA, RMC)

#define CMD_GNSS_CONFIG_GLONASSNMEATYPE          "+QGPSCFG=\"glonassnmeatype\"" // Config message types for GLONASS

#define CMD_GNSS_CONFIG_GALILEONMEATYPE          "+QGPSCFG=\"galileonmeatype\",0" // Send 0 type of message 
                                                                                  // for Galileo

#define CMD_GNSS_CONFIG_BEIDOUNMEATYPE           "+QGPSCFG=\"beidounmeatype\",1" // Send GSA messages
                                                                                 // for Beidou

#define CMD_GNSS_CONFIG_GSVEXTNMEATYPE           "+QGPSCFG=\"gsvextnmeatype\",1" // Enable extended GSV

#define CMD_GNSS_CONFIG_GNSSCONFIG               "+QGPSCFG=\"gnssconfig\"" // Modules selection

#define CMD_GNSS_CONFIG_AUTOGPS                  "+QGPSCFG=\"autogps\",0" // Disable automatic GNSS

#define CMD_GNSS_QGPSDEL                         "+QGPSDEL=0" // Delete all assistance data except gpsOneXTRA
                                                              // when turn off the module

#define CMD_GNSS_QGPS                            "+QGPS=1,30,50,0,1" // Turn on GPS module
// Stand-alone mode
// Maximum positioning time 30s
// Accuracy threshold 50m
// Continuous positioning
// 1s between 1st and 2nd positioning

#define CMD_GNSS_QGPSEND                         "+QGPSEND" // Turn off GNSS module if operating in 
// continuously positioning mode

#define CMD_GNSS_QGPSLOC                         "+QGPSLOC=0" // Acquire positioning information in format 0

#define CMD_GNSS_QGPSGNMEA                       "+QGPSGNMEA=" // Acquire NMEA message

#define CMD_GNSS_QGPSGNMEA_GGA                   "+QGPSGNMEA=\"GGA\"" // Acquire NMEA GGA message

#define CMD_GNSS_QGPSGNMEA_RMC                   "+QGPSGNMEA=\"RMC\"" // Acquire NMEA RMC message

#define CMD_GNSS_QGPSGNMEA_GSV                   "+QGPSGNMEA=\"GSV\"" // Acquire NMEA GSV message

#define CMD_GNSS_QGPSGNMEA_GSA                   "+QGPSGNMEA=\"GSA\"" // Acquire NMEA GSA message

#define CMD_GNSS_QGPSGNMEA_VTG                   "+QGPSGNMEA=\"VTG\"" // Acquire NMEA VTG message

#define CMD_GNSS_QGPSGNMEA_GNS                   "+QGPSGNMEA=\"GNS\"" // Acquire NMEA GNS message

#define CMD_GNSS_QGPSXTRA                        "+QGPSXTRA=" // Enable/ Disable gpsOneXTRA Assistance

#define CMD_GNSS_QGPSXTRATIME                    "+QGPSXTRATIME=" // Inject XTRA time

#define CMD_GNSS_QGPSXTRADATA                    "+QGPSXTRADATA=" // Inject XTRA data file

#define CMD_GNSS_QGPSXTRADATA_QUERY              "+QGPSXTRADATA?" // Query XTRA data file


// GSM SMS commands //
#define CMD_SMS_CSMS                             "+CSMS" // Select message service

#define CMD_SMS_CMGF                             "+CMGF" // Message format

#define CMD_SMS_CSCA                             "+CSCA" // Service center address

#define CMD_SMS_CPMS                             "+CPMS" // Select preferred message storages

#define CMD_SMS_CMGD                             "+CMGD" // Delete message

#define CMD_SMS_CMGR                             "+CMGR" // Read message

#define CMD_SMS_CMGS                             "+CMGS" // Send message

#define CMD_SMS_CMGL                             "+CMGL" // List message

#define CMD_SMS_CNMI                             "+CNMI" // SMS event reporting configuration

#define CMD_SMS_CSDH                             "+CSDH" // Set SMS text mode parameters


// GSM call commands //
#define CMD_CALL_ATA                             "A" // Answering incoming call

#define CMD_CALL_ATD                             "D" // Set up outgoing call

#define CMD_CALL_ATH                             "H0" // Disconnect an existing connection

#define CMD_CALL_CHUP                            "+CHUP" // Disconnect voice call

#define CMD_CALL_CMD_2_DAT                       "O0" // Switch from command mode to data mode

#define CMD_CALL_ATS7                            "S7" // Set time to wait for connection completion

#define CMD_CALL_CLCC                            "+CLCC" // List of current calls of ME


// Packet domain functions //
#define CMD_PD_CGATT                             "+CGATT?" // Query PS state

#define CMD_PD_CGDCONT                           "+CGDCONT" // Config a PDP context

#define CMD_PD_CGQREQ                            "+CGQREQ" // Quality of service profile (Requested)

#define CMD_PD_CGQMIN                            "+CGQMIN" // Quality of service profile (Minimum acceptable)

#define CMD_PD_CGACT                             "+CGACT" // Activate of deactivate PDP context

#define CMD_PD_CGPADDR                           "+CGPADDR" // Show PDP context IP address

#define CMD_PD_CGREG                             "+CGREG?" // Query network registration on PS domain status


// Hardware commands
#define CMD_HW_CCLK                              "+CCLK?" // Query 4G module RTC 


// TCP/IP module commands //
#define CMD_TCP_QICSGP                           "+QICSGP" // Config a PDP context

#define CMD_TCP_QIACT                            "+QIACT" // Activate a PDP context

#define CMD_TCP_QIDEACT                          "+QIDEACT" // Deactivate a PDP context

#define CMD_TCP_QIOPEN                           "+QIOPEN" // Open a connection

#define CMD_TCP_QICLOSE                          "+QICLOSE" // Close a connection

#define CMD_TCP_QISTATE                          "+QISTATE" // Query connection status

#define CMD_TCP_QISEND                           "+QISEND" // Send data to server via connection

#define CMD_TCP_QIRD                             "+QIRD" // Retrieve the received TCP IP data

#define CMD_TCP_QISWTMD                          "+QISWTMD" // Switch data access mode

#define CMD_TCP_QPING                            "+QPING" // Ping a remote server

#define CMD_TCP_QIDNSGIP                         "+QIDNSGIP" // Get IP address by domain name

#define CMD_TCP_QICFG                            "+QICFG" // Configure optional parameters

#define CMD_TCP_QIGETERROR                       "+QIGETERROR" // Query the last error code


// FTP commands //
#define CMD_FTP_QFTPCFG_PDP_ID                   "+QFTPCFG= \"contextid\"" // Configure FTP context ID 

#define CMD_FTP_QFTPCFG_ACC                      "+QFTPCFG= \"account\"" // Configure FTP account

#define CMD_FTP_QFTPCFG_FILE_TYPE                "+QFTPCFG= \"filetype\",0" // Configure FTP file type to binary type

#define CMD_FTP_QFTPCFG_TRANS_MODE               "+QFTPCFG= \"transmode\",1" // Configure FTP transfer mode to passive mode

#define CMD_FTP_QFTPCFG_TIMEOUT                  "+QFTPCFG= \"rsptimeout\"" // Configure FTP response timeout

#define CMD_FTP_QFTPCFG_SSL_TYPE                 "+QFTPCFG= \"ssltype\"" // Configure FTP SSL type

#define CMD_FTP_QFTPCFG_SSL_ID                   "+QFTPCFG= \"sslctxid\"" // Configure FTP SSL context ID

#define CMD_FTP_QSSLCFG_VER                      "+QSSLCFG= \"sslversion\"" // Configure SSL version

#define CMD_FTP_QSSLCFG_CIPHER                   "+QSSLCFG= \"ciphersuite\"" // Configure SSL cipher suite

#define CMD_FTP_QSSLCFG_CACERT                   "+QSSLCFG= \"cacert\"" // Configure SSL CA certification

#define CMD_FTP_QSSLCFG_SEC_LV                   "+QSSLCFG= \"seclevel\"" // Configure SSL security level

#define CMD_FTP_QFTPOPEN                         "+QFTPOPEN" // Login to FTP server

#define CMD_FTP_QFTPCWD                          "+QFTPCWD" // Set working directory on FTP server

#define CMD_FTP_QFTPMKDIR                        "+QFTPMKDIR" // Create 1 or multiple directories on FTP server

#define CMD_FTP_QFTPPUT                          "+QFTPPUT" // Upload file from QFTP server

#define CMD_FTP_QFTPGET                          "+QFTPGET" // Download file from QFTP server

#define CMD_FTP_QFTPSIZE                         "+QFTPSIZE" // Get the size of a file on the FTP server 

#define CMD_FTP_QFTPMDTM                         "+QFTPMDTM" // Get the last modification time of a file on the FTP server

#define CMD_FTP_QFTPLEN                          "+QFTPLEN" // Get the length of data transferred

#define CMD_FTP_QFTPDEL                          "+QFTPDEL" // Delete a file in the FTP server

#define CMD_FTP_QFTPCLOSE                        "+QFTPCLOSE" // Close connection to FTP server

// HTTP command
#define CMD_HTTP_CFG_PDP_ID                      "+QHTTPCFG= \"contextid\"" // Configure HTTP context ID 

#define CMD_HTTP_CFG_RESP_HEADER                 "+QHTTPCFG= \"responseheader\"" // Configure HTTP response header

#define CMD_HTTP_URL                             "+QHTTPURL=" // Set URL of HTTP server

#define CMD_HTTP_GET                             "+QHTTPGET=" // Send GET request to HTTP server

#define CMD_HTTP_READ_FILE                       "+QHTTPREADFILE=" // Read response from HTTP server via file

#define CMD_HTTP_STOP                            "+QHTTPSTOP" // Cancel HTTP request

// File operations //
#define CMD_FILE_LIST                            "+QFLST" // List the information of a file of all files in a storage

#define CMD_FILE_UPLOAD                          "+QFUPL" // Upload a file to a storage in the QTEC21 module

#define CMD_FILE_OPEN                            "+QFOPEN" // Open a file stored in the QTEC21 module

#define CMD_FILE_SEEK                            "+QFSEEK" // Set the pointer position in a file in the 

#define CMD_FILE_READ                            "+QFREAD" // Read a file stored in the QTEC21 module

#define CMD_FILE_WRITE                           "+QFWRITE" // Write to a file stored in the QTEC21 module

#define CMD_FILE_CLOSE                           "+QFCLOSE" // Close a file stored in the QTEC21 module

#define CMD_FILE_DELETE                          "+QFDEL" // Delete a file stored in the QTEC21 module

#define CMD_HW_SLEEP_EN                          "+QSCLK" // Enable sleep mode


// GSM strings //
#define	STR_MODULE_START	                       "\r\nRDY\r\n"

#define STR_ME_FUNC_READY                        "\r\n+CFUN: 1\r\n"

#define STR_SIM_READY                            "\r\n+CPIN: READY\r\n"

#define STR_SIM_NOT_READY                        "\r\n+CPIN: NOT READY\r\n"

#define STR_SIM_NOT_INSERTED                     "\r\n+CPIN: NOT INSERTED\r\n"

#define STR_QUSIM_OK                             "\r\n+QUSIM: 1\r\n"

#define STR_SMS_READY                            "\r\n+QIND: SMS DONE\r\n"

#define STR_PHONEBOOK_READY                      "\r\n+QIND: PB DONE\r\n"

#define STR_NORMAL_POWER_DOWN                    "\r\nNORMAL POWER DOWN\r\n"

#define STR_POWERED_DOWN                         "\r\nPOWERED DOWN\r\n"

#define	STR_RESPONSE_OK			                     "\r\nOK\r\n"

#define	STR_RESPONSE_ERROR		                   "\r\nERROR\r\n"

#define STR_CMS_ERROR                            "\r\n+CMS ERROR:"

#define STR_CME_ERROR                            "\r\n+CME ERROR:"

#define STR_RING		                             "\r\nRING\r\n"

#define STR_RESPONSE_BUSY		                     "\r\nBUSY\r\n"

#define STR_RESPONSE_NO_CARRIER	                 "\r\nNO CARRIER\r\n"

#define STR_RESPONSE_NO_ANSWER	                 "\r\nNO ANSWER\r\n"

#define STR_RESPONSE_CONNECT                     "\r\nCONNECT\r\n"

#define STR_RESPONSE_GMI                         "\r\nQuectel\r\nOK\r\n"

#define STR_RESPONSE_SEND                        "\r\n+CMGS:"

#define STR_RESPONSE_QIACT                       "\r\n+QIACT:"

#define STR_RESPONSE_OPEN                        "+QIOPEN: "

#define STR_RESPONSE_OPEN_2                      "\r\n+QIOPEN: "

#define STR_CONNECT_OK			                     "\r\n+QIOPEN: 0,0"

#define STR_CONNECT_FAIL		                     "\r\n+QIOPEN: 0,566"

#define STR_SEND_OK				                       "\r\nSEND OK\r\n"

#define STR_QISEND_OK		                         "\r\n+QISEND:"	

#define STR_SEND_FAIL			                       "\r\nSEND FAIL\r\n" 

#define STR_RECEIVE_PACKET                       "\r\n+QIURC: \"recv\","

#define STR_CONN_CLOSED                          "\r\n+QIURC: \"closed\","

#define STR_MSG_PDP_DEACT                        "\r\n+QIURC: \"pdpdeact\","

#define STR_URC_GPS_XTRA                         "\r\n+QGPSURC:"

#define	STR_CMTI_MSG				                     "\r\n+CMTI: \"SM\","

#define	STR_QCMGR				                         "\r\n+CMGR:"

#define	STR_CSQ					                         "\r\n+CSQ:"

#define	STR_COPS				                         "\r\n+COPS:"

#define STR_FTP_OPEN			                       "\r\n+QFTPOPEN:"

#define STR_FTP_CWD                              "\r\n+QFTPCWD:"

#define STR_FTP_MKDIR                            "\r\n+QFTPMKDIR:"

#define STR_FTP_PUT				                       "\r\n+QFTPPUT:"

#define STR_FTP_GET				                       "\r\n+QFTPGET:"

#define STR_FTP_SIZE                             "\r\n+QFTPSIZE:"

#define STR_FTP_DATE                             "\r\n+QFTPMDTM:"

#define STR_FTP_LEN                              "\r\n+QFTPLEN:"

#define STR_FTP_DEL                              "\r\n+QFTPDEL:"

#define STR_FTP_CLOSE                            "\r\n+QFTPCLOSE:"

#define STR_HTTP_GET                             "\r\n+QHTTPGET:"

#define STR_HTTP_READFILE                        "\r\n+QHTTPREADFILE:"

#define STR_FILE_OPEN			                       "\r\n+QFOPEN:"

#define STR_STORAGE_DEL_ALL                      "*"

#define GSM_OUTPORT_USBAT                        "usbat"

#define GSM_OUTPORT_USBMODEM                     "usbmodem"

#define GSM_OUTPORT_UART1                        "uart1"

#define GSM_OUTPORT_UART2                        "uart2"

// TCP/IP module configs //

// PDP maximum number
#define TCP_PDP_NUM_MAX                          7U // Maximum number of available TCP/IP PDP contexts

// Socket maximum number
#define TCP_SOCKET_NUM_MAX                       12U // Maximum number of available TCP sockets

// Server 0 socket range
#define TCP_SERVER_0_SOCKET_MIN                  0U // Server 0 socket min
#define TCP_SERVER_0_SOCKET_MAX                  5U // Server 0 socket max

// Server 1 socket range
#define TCP_SERVER_1_SOCKET_MIN                  6U // Server 1 socket min
#define TCP_SERVER_1_SOCKET_MAX                  11U // Server 1 socket max

// PDP context type
#define TCP_PDP_TYPE                             1U // Option for TCP/IP PDP context type

// TCP service type
#define TCP_SERVICE_TYPE                         "TCP" // TCP/IP PDP context type

// PDP Authentication type
#define TCP_PDP_AUTH                             2U // Option for TCP/IP PDP authentication type

#define FTP_LOCAL_STORAGE                        "UFS:" // Module local storage UFS - primary

#define FTP_LOCAL_STORAGE_2                      "RAM:" // Module local storage RAM - secondary

#define FTP_LOCAL_STORAGE_FILE                   "TEMP" // FTP temp file name

#define FTP_LOCAL_STORAGE_FILE_2                 "TEMP2" // FTP temp file name 2 

#define FTP_FILE_UPLOAD_BUF_SIZE                 8192U // FTP upload buffer size

#define FTP_FILE_UPLOAD_SIZE_MAX                 (uint32_t)(20 * 1024 * 1024) // Maximum file size for FTP upload - 20MB

#define FTP_SSL_ID_DEF                           0U // Default SSL context ID

#define FTP_SSL_VERSION_DEF                      4U // Default SSL version setting - Support all versions

#define FTP_SSL_CIPHER_SUIT_DEF                  "0XFFFF" // Default SSL cipher suit setting - Support all suits

#define FTP_SSL_CA_CERT_SD                       "CA Certificate/cacert.cer" // Path of CA certificate in SD card

#define FTP_SSL_CA_DEF                           "RAM:cacert.cer" // SSL CA certificate path

#define FTP_SSL_SEC_LEVEL_DEF                    1U // Default SSL security level

// Timeout in seconds
#define GSM_READY_TIMEOUT                        60U // Maximum wait time for module QTEC21 being ready

#define GSM_NO_RESPONSE_CNT_MAX                  3U // Maximum count for GSM function no response

#define QTEC21_COMM_RESPONSE_TIMEOUT             400U // QTEC21 common command response timeout in ms

#define QTEC21_GPS_RESPONSE_TIMEOUT              500U // QTEC21 GPS command response timeout in mS

#define GSM_USIM_QUERY_TIMEOUT                   6U // GSM USIM query timeout
#define GSM_CREG_QUERY_TIMEOUT                   90U // GSM CS service query timeout
#define GSM_CGREG_QUERY_TIMEOUT                  60U // GSM PS service query timeout

#define GSM_SMS_SEND_TIMEOUT                     60U // GSM SMS send timeout

#define GSM_CALL_ANS_TIMEOUT                     60U // GSM call answer timeout
#define GSM_CALL_DIAL_TIMEOUT                    5U // GSM call dial timeout
#define GSM_CALL_DISCONN_TIMEOUT                 60U // GSM call disconnect timeout

#define TCP_PDP_ATTACH_TIMEOUT                   60U // TCP/IP attach or detachment of PS
#define TCP_PDP_ACTIV_TIMEOUT                    60U // TCP/IP PDP context activate timeout
#define TCP_PDP_DEACTIV_TIMEOUT                  60U // TCP/IP PDP context deactivate timeout
#define TCP_OPEN_CONN_TIMEOUT                    60U // TCP/IP open connection timeout
#define TCP_CLOSE_CONN_TIMEOUT                   10U // TCP/IP close connection timeout
#define TCP_SEND_ACK_TIMEOUT                     30U // TCP/IP send ACK timeout

#define FTP_ACC_CONFIG_TIMEOUT                   5U // FTP account configure timeout

#define FTP_FUNCTION_TIMEOUT                     120U // FTP normal function timeout 

#define HTTP_FUNCTION_TIMEOUT                    120U // HTTP normal function timeout

#define FILE_READ_TIMEOUT                        5U // FTP read function timeout

#define FILE_WRITE_TIMEOUT                       5U // FTP write function timeout
#endif


//============================================================================//
// User Typedefs
//============================================================================//
#if ENABLE_GSM_QTEC21
// Module type
typedef enum
{
  GSM_MODULE_EC21 = 0U,

	GSM_MODULE_EC25,
	
	GSM_MODULE_EC200
	
} GSM_ModuleTypeDef;

typedef enum
{
	// No command
	QTEC21_BLOCK_NULL = 0U, // 0

	
	// Hardware control functions
	QTEC21_PWR_ON, // Turn on power
	
	QTEC21_PWR_OFF, // Turn off power
	
	QTEC21_SWITCH_ON, // Switch on module
	
	QTEC21_SWITCH_OFF, // Switch off module
	
	QTEC21_TURNOFF, // AT+QPOWD
	
	QTEC21_RESET, // RESET
	
	QTEC21_RESET_CHECK_FTP, // RESET WITH CHECK FTP
	

	// General functions
	QTEC21_BASE_AT_TEST, // AT
	
	QTEC21_BASE_READ_CGMI, // AT+CGMI
	
	QTEC21_BASE_READ_CGMM, // AT+CGMM

	QTEC21_BASE_READ_CGMR, // AT+CGMR

	QTEC21_BASE_GET_IMEI, // AT+CGSN
	
	QTEC21_BASE_SET_FUNC, // AT+CFUN

	QTEC21_BASE_SAVE_PARA, // AT&W

	QTEC21_BASE_SET_PARA, // ATZ

	QTEC21_BASE_ECHO_CONFIG, // ATE

	QTEC21_BASE_RESULT_FORMAT, // ATX

	QTEC21_BASE_ERR_FORMAT, // AT+CMEE

	QTEC21_BASE_SEL_TESET, // AT+CSCS

	QTEC21_BASE_URC_OUTPUT, // AT+QURCCFG
	

	// Serial Interface functions
	QTEC21_COM_DTR_FUNCTION, // AT&D

	QTEC21_COM_SET_BAUDRATE, // AT+IPR
	

	// Status control functions
	QTEC21_STAT_CONFIG_RI, // AT+QCFG="urc/ri/ring"


	// SIM functions
	QTEC21_SIM_GET_IMSI, // AT+CIMI

	QTEC21_SIM_QUERY_USIM, // AT+CPIN?
	
	QTEC21_SIM_SET_PIN, // AT+CPWD= 
	

	// Network functions
	QTEC21_NET_QUERY_CS, // AT+CREG?

	QTEC21_NET_GET_CSQ, // AT+CSQ
	
	QTEC21_NET_GET_TIME, // AT+QLTS
	

	// Data mode to command mode
	QTEC21_DAT_2_CMD, // +++
	

	// GPS functions
	QTEC21_GPS_OUTPORT, // AT+QGPSCFG=outport?

	QTEC21_GPS_NMEASRC, // AT+QGPSCFG="nmeasrc"

	QTEC21_GPS_GPSNMEATYPE, // AT+QGPSCFG=gpsnmeatype?

	QTEC21_GPS_GLONASSNMEATYPE, // AT+QGPSCFG=glonassnmeatype?

	QTEC21_GPS_GALILEONMEATYPE, // AT+QGPSCFG=galileonmeatype?

	QTEC21_GPS_BEIDOUNMEATYPE, // AT+QGPSCFG=beidounmeatype?

	QTEC21_GPS_GSVEXTNMEATYPE, // AT+QGPSCFG=gsvextnmeatype?

	QTEC21_GPS_GNSSCONFIG, // AT+QGPSCFG=gnssconfig?

	QTEC21_GPS_AUTOGPS, // AT+QGPSCFG=autogps?

	QTEC21_GPS_DELETE, // AT+QGPSDEL

	QTEC21_GPS_ON, // AT+QGPS

	QTEC21_GPS_OFF, // AT+QGPSEND

	QTEC21_GPS_LOCATION, // AT+QGPSLOC

	QTEC21_GPS_GETNMEA, // AT+QGPSGNMEA

	QTEC21_GPS_CONFIGXTRA, // AT+QGPSXTRA
	
	QTEC21_GPS_XTRATIME, // AT+QGPSXTRATIME
	
	QTEC21_GPS_XTRADATA, // AT+QGPSXTRADATA=
	
	QTEC21_GPS_XTRADATA_QUERY, // AT+QGPSXTRADATA?
	

	// SMS functions
	QTEC21_SMS_SEL_SERVICE, // AT+CSMS

	QTEC21_SMS_SEL_FORMAT, // AT+CMGF

	QTEC21_SMS_SERV_CENTER, // AT+CSCA

	QTEC21_SMS_SEL_STORAGE, // AT+CPMS

	QTEC21_SMS_DELETE, // AT+CMGD

	QTEC21_SMS_GET_LIST, // AT+CMGL

	QTEC21_SMS_READ, // AT+CMGR

	QTEC21_SMS_EVENT_REPORT, // AT+CNMI

	QTEC21_SMS_SEL_TEXTMODE, // AT+CSDH
	

	// Call functions
	QTEC21_CALL_DIAL, // ATD

	QTEC21_CALL_CMD_2_DAT, // ATO

	QTEC21_CALL_CONN_TIMEOUT, // ATS7

	QTEC21_CALL_GET_LIST, // AT+CLCC
	

	// Packet domain functions
	QTEC21_PD_PDP_CFG, // AT+CGDCONT

	QTEC21_PD_CGQ_REQ, // AT+CGQREQ

	QTEC21_PD_CGQ_MIN, // AT+CGQMIN

	QTEC21_PD_PDP_ADDR, // AT+CGPADDR

	QTEC21_PD_QUERY_PS, // AT+CGREG
	
	
	// Hardware functions
	QTEC21_HW_GET_RTC, // AT+CCLK 
	

	// TCP/IP functions
	QTEC21_TCP_PDP_CONFIG, // AT+QICSGP 
	
	QTEC21_TCP_PDP_ACT, // AT+QIACT 
	
	QTEC21_TCP_GET_IP, // AT+QIACT 
	
	QTEC21_TCP_PDP_DEACT, // AT+QIDEACT

	QTEC21_TCP_CONN_CLOSE, // AT+QICLOSE  

	QTEC21_TCP_CONN_STATE, // AT+QISTATE

	QTEC21_TCP_SEND, // AT+QISEND 

	QTEC21_TCP_SEND_STATUS, // AT+QISEND 

	QTEC21_TCP_RECV_CHECK, // AT+QIRD 

	QTEC21_TCP_READ, // AT+QIRD

	QTEC21_TCP_CMD_2_DAT, // AT+QISWTMD


	// FTP functions
	QTEC21_FTP_CONFIG_PDP_ID, // AT+QFTPCFG="contextid"

	QTEC21_FTP_CONFIG_ACC, // AT+QFTPCFG="account"

	QTEC21_FTP_CONFIG_FILETYPE, // AT+QFTPCFG="filetype"

	QTEC21_FTP_CONFIG_TRANSMODE, // AT+QFTPCFG="transmode"

	QTEC21_FTP_CONFIG_TIMEOUT, // AT+QFTPCFG="rsptimeout"
	
	QTEC21_FTP_CONFIG_SSL_TYPE, // AT+QFTPCFG="ssltype"
	
	QTEC21_FTP_CONFIG_SSL_ID, // AT+QFTPCFG="sslctxid"
	
	QTEC21_FTP_SSL_VERSION, // AT+QSSLCFG="sslversion"
	
	QTEC21_FTP_SSL_CIPHER, // AT+QSSLCFG="ciphersuite"
	
	QTEC21_FTP_SSL_CACERT, // AT+QSSLCFG="cacert"
	
	QTEC21_FTP_SSL_SECLV, // AT+QSSLCFG="seclevel"
	
	
	// HTTP functions
	QTEC21_HTTP_CFG_PDP_ID, // AT+QHTTPCFG="contextid"
	
	QTEC21_HTTP_CFG_RESP_HEADER, // AT+QHTTPCFG="responseheader"
	
	QTEC21_HTTP_URL, // AT+QHTTPURL 
	
	QTEC21_HTTP_STOP, // AT+QHTTPSTOP
	

	// File operation functions
	QTEC21_FIL_LIST, // AT+QFLST
	
	QTEC21_FIL_UPLOAD, // AT+QFUPL
	
	QTEC21_FIL_OPEN, // AT+QFOPEN

	QTEC21_FIL_SEEK, // AT+QFSEEK

	QTEC21_FIL_READ, // AT+QFREAD
	
	QTEC21_FIL_WRITE, // AT+QFWRITE

	QTEC21_FIL_CLOSE, // AT+QFCLOSE

	QTEC21_FIL_DELETE, // AT+QFDEL
	
	QTEC21_HW_SLEEP_EN // AT+QSCLK

} QTEC21_BlockCMD_TypeDef;

typedef enum
{
  QTEC21_NONBLOCK_NULL = 0U,
	
	
	// SMS functions
	QTEC21_SMS_SEND, // AT+CMGS  NON-BLOCK
	
	
	// Call functions
	QTEC21_CALL_ANS, // ATA  NON-BLOCK
	
	QTEC21_CALL_DISCONN, // ATH / CHUP  NON-BLOCK
	
	
	// Packet domain functions
	QTEC21_PD_PS_STATE, // AT+CGATT  NON-BLOCK
	
	QTEC21_PD_PDP_ACT, // AT+CGACT  NON-BLOCK
	
	
	// TCP/IP functions
	QTEC21_TCP_CONN_OPEN, // AT+QIOPEN  NON-BLOCK
	
	
	// FTP functions
	QTEC21_FTP_OPEN, // AT+QFTPOPEN  NON-BLOCK

	QTEC21_FTP_SET_DIR, // AT+QFTPCWD  NON-BLOCK
	
	QTEC21_FTP_MAKE_DIR, // AT+QFTPMKDIR  NON-BLOCK
	
	QTEC21_FTP_UPLOAD, // AT+QFTPPUT NON-BLOCK
	
	QTEC21_FTP_UPLOAD_DIRECT, // AT+QFTPPUT NON-BLOCK

	QTEC21_FTP_DOWNLOAD, // AT+QFTPGET  NON-BLOCK
	
	QTEC21_FTP_FILE_SIZE, // AT+QFTPSIZE  NON-BLOCK

	QTEC21_FTP_FILE_DATE, // AT+QFTPMDTM  NON-BLOCK

	QTEC21_FTP_TRANS_LEN, // AT+QFTPLEN  NON-BLOCK
	
	QTEC21_FTP_FILE_DEL, // AT+QFTPDEL  NON-BLOCK

	QTEC21_FTP_CLOSE, // AT+QFTPCLOSE  NON-BLOCK
	
	
	// HTTP functions
	QTEC21_HTTP_GET, // AT+QHTTPGET  NON-BLOCK
	
	QTEC21_HTTP_READ_FILE, // AT+QHTTPREADFILE  NON-BLOCK
	
} QTEC21_NonBlockCMD_TypeDef;

// SIM Status
typedef enum
{
	GSM_SIM_NULL = 0U, //
	
	GSM_SIM_OK, // 1 SIM is working properly
	
	GSM_SIM_NOT_INSERTED, //21 SIM is not inserted
	
	GSM_SIM_PIN_REQ, // 3 SIM PIN is required
	
	GSM_SIM_PUK_REQ, // 4 SIM PIN is required
	
	GSM_SIM_PIN_REJECTED, // 5 current SIM PIN is incorrect
	
	GSM_SIM_PUK_REJECTED, // 6 current SIM PUK is incorrect
	
	GSM_SIM_NOT_RESP, // 7 Query SIM status no response
	
	GSM_SIM_UNCLEAR_ERR, // 8 SIM uncleared error
	
} GSM_SIM_StatTypedef;

typedef enum
{
	GSM_NULL = 0U, 
	
	GSM_OK, // 1 Connection is established successfully

	GSM_NO_CARRIER, // 2 Connection cannot be established

	GSM_BUSY, // 3 The receiver is busy

	GSM_NO_DIALTONE, // 4 No dialtone when calling

	GSM_ERROR// 5 GSM-related operation fail

} GSM_StatusTypeDef;

typedef enum
{
	SEND_NULL = 0U,
	
	SEND_OK, // 1

	SEND_FAIL, // 2

	SEND_ERROR // 3

} MsgSendStatusTypeDef;

typedef enum
{
	TCP_NULL = 0U,
	
	TCP_OK, // 

	TCP_NO_PDP, //

	TCP_NO_SOCKET, // 

	TCP_NO_USIM, // 

	TCP_NO_CS, // 

	TCP_NO_PS, // 

	TCP_PDP_CONFIG_ERR, // 

	TCP_PDP_ACTV_ERR, // 

	TCP_PDP_DEACTV_ERR, // 

	TCP_PDP_IP_QUERY_ERR, // 

	TCP_OPEN_ERR, // 

	TCP_CLOSE_ERR, // 

	TCP_CONN_QUERY_ERR, // 

	TCP_SEND_ERR, // 

	TCP_SEND_QUERY_ERR, // 

	TCP_SEND_NOT_FINISH, // 

	TCP_RECV_QUERY_ERR, // 

	TCP_RECV_ERR,	// 

	TCP_FTP_PDP_CONFIG_ERR, // 

	TCP_FTP_ACC_CONFIG_ERR, // 

	TCP_FTP_FILETYPE_CONFIG_ERR, // 

	TCP_FTP_TRANSMODE_CONFIG_ERR, // 

	TCP_FTP_TIMEOUT_CONFIG_ERR, // 
	
	TCP_FTP_SSL_TYPE_ERR, // 
	
	TCP_FTP_SSL_ID_ERR, // 
	
	TCP_FTP_SSL_VER_ERR, // 
	
	TCP_FTP_SSL_CIPHER_ERR, // 
	
	TCP_FTP_SSL_CACERT_ERR, // 
	
	TCP_FTP_SSL_SECLV_ERR, // 

	TCP_FTP_LOGIN_ERR, // 

	TCP_FTP_SETDIR_ERR, // 
	
	TCP_FTP_MAKEDIR_ERR, //
	
	TCP_FTP_UPLOAD_ERR, // 

	TCP_FTP_DOWNLOAD_ERR, // 

	TCP_FTP_DOWNLOAD_WAIT, // 

	TCP_FTP_FILESIZE_ERR, // 

	TCP_FTP_FILEDATE_ERR, // 

	TCP_FTP_LOGOUT_ERR, // 

	TCP_FTP_DF2FILEDEL_ERR, // 
	
	TCP_FTP_FILEUPLOAD_ERR, // 

	TCP_FTP_FILEOPEN_ERR, // 

	TCP_FTP_FILESEEK_ERR, // 

	TCP_FTP_FILEREAD_ERR, // 
	
	TCP_FTP_FILEWRITE_ERR, // 

	TCP_FTP_FILECLOSE_ERR, // 

	TCP_FTP_FILEDEL_ERR, // 

	TCP_FTP_SD_OPEN_ERR, // 
	
	TCP_FTP_CHECKFW_ERR, // 
	
	TCP_FTP_SAVEFILE_ERR, // 

	TCP_FTP_CRC_ERR, // 
	
	TCP_FTP_NO_SRC_FILE, // 
	
	TCP_FTP_OTHER_ERR, // 
	
	TCP_HTTP_CFG_PDP_ERR, //
	
	TCP_HTTP_CFG_RESP_ERR, //
	
	TCP_HTTP_URL_ERR, //
	
	TCP_HTTP_GET_ERR, //
	
	TCP_HTTP_READ_FILE_ERR, //
	
	TCP_HTTP_STOP_ERR, //
	
	TCP_HTTP_OTHER_ERR //

} TCP_StatusTypeDef;

typedef enum
{
  CHAR_SET_IRA = 0U,
	
	CHAR_SET_GSM
	
} GSM_CharSetTypeDef;

#pragma pack(1)
typedef struct
{
	bool bInUse;

	//bool bBusy;

	QTEC21_BlockCMD_TypeDef qtec21Cmd;

	uint32_t u32Arg[5];

	uint8_t *pu8RetVal;

} QtEC21_BlockCmdBuildTypeDef;

typedef struct
{
	bool bInUse;

	//bool bBusy;

	QTEC21_NonBlockCMD_TypeDef qtec21Cmd;

	uint32_t u32Arg[5];

	uint8_t *pu8RetVal;

} QtEC21_NonBlockCmdBuildTypeDef;

typedef struct
{
	bool bActiv;

	bool bInUse;

	char cIP_Addr[20];
	
	uint8_t u8Precedence;
	
	uint8_t u8Delay;
	
	uint8_t u8Reliability;
	
	uint8_t u8Peak;
	
	uint8_t u8Mean;

} TCP_PDP_ContextStruct;

typedef struct
{
	bool bConnect;

	TCP_PDP_ContextStruct *ptcp_pdpHandle;

} TCP_SocketStruct;

typedef struct
{
	char *pcPhoneNum;

	char *pcData;

	uint16_t u16DataLen;

} GSM_MsgDataTypeDef;

typedef struct
{
	uint8_t SocketNum;

	char *pcData;

	uint16_t u16DataLen;

} GSM_SocketDataTypeDef;

typedef struct
{
	char cServType[20];

	char cRemoteAddr[31];

	uint16_t u16RemotePort;

	uint16_t u16LocalPort;

	uint8_t u8ConnMode;

} GSM_SocketConnTypeDef;
#pragma pack()
#endif


//============================================================================//
// User Externs
//============================================================================//
#if ENABLE_GSM_QTEC21
extern GSM_ModuleTypeDef tGSM_ModuleType;

extern bool bQtEC21_State;

extern bool bQtEC21_Ready;

extern bool bGPS_Ready;

extern bool bQtEC21_InitFinish;

extern bool bGSMTimeOut;

extern uint32_t u32GSM_ResetLastTick;

extern uint8_t u8GSM_NoRespCnt;

extern bool bGSM_PwrOnFlag;
extern bool bGSM_PwrOffFlag;

extern bool bGSM_ResetFlag;
extern bool bGSM_ResetCheckFTP_Flag;

extern TCP_PDP_ContextStruct tcp_pdpContext[TCP_PDP_NUM_MAX + 1];

extern TCP_SocketStruct tcpSocket[TCP_SOCKET_NUM_MAX + 1];

extern GSM_SocketConnTypeDef tcpConnParamHandle;

extern const uint8_t u8CurrPDP_ID;

extern uint8_t u8CurrTCP_Socket[MAX_SOCKET_BANK];

extern char cGSM_CurrIP[20];

extern bool bTCPIP_Connect[MAX_SOCKET_BANK];

extern bool bFTP_SetupFinish;

extern bool bFTP_Connect;

extern uint8_t u8TCP_RetryCnt[MAX_SOCKET_BANK];

extern uint8_t u8TCP_ConnectRetryCnt[MAX_SOCKET_BANK];

extern uint8_t u8FTP_RetryCnt;

extern uint8_t u8FTP_AccessRetryCnt;

extern uint8_t u8HTTP_RetryCnt;

//extern uint8_t u8QtEC21_Buf[FTP_FILE_UPLOAD_BUF_SIZE];

extern uint8_t u8SMS_List[GSM_BUFFER_SIZE];

extern bool bQtEC21_Pwr;

extern bool bQtEC21_Switch;

extern GSM_CharSetTypeDef tQtEC21_CharSet;
#endif


//============================================================================//
// User Function Prototypes
//============================================================================//
// Software control functions //
#if ENABLE_GSM_QTEC21
void QtEC21_PwrEnable(void);
void QtEC21_PwrDisable(void);
bool QtEC21_SendBuffer(uint8_t *pu8Buf, uint16_t u16Len);
bool QtEC21_SendCMD(char* pcCmd);
void QtEC21_RetryCheck(void);
void QtEC21_Init(void);
void QtEC21_CheckRedoFTP(void);

// Hardware controls function
void QtEC21_TurnOnPwrKey(void);
void QtEC21_TurnOffPwrKey(void);
void QtEC21_TurnOffCmd(void);
void QtEC21_SetSleep(void);
void QtEC21_WakeUp(void);
void QtEC21_Reset(void);
void QtEC21_ResetWithCheckFTP(void);

// General functions
bool QtEC21_AT_Test(void);
bool QtEC21_ReadCGMI(char *pcCGMI);
bool QtEC21_ReadCGMM(char *pcCGMM);
bool QtEC21_ReadCGMR(char *pcCGMR);
bool QtEC21_GetIMEI(char *pcIMEI);
bool QtEC21_SetFunc(uint8_t u8Func, bool bRst);
bool QtEC21_SaveConfigProf(uint8_t u8ConfigID);
bool QtEC21_SetConfigProf(uint8_t u8ConfigID);
bool QtEC21_ConfigEcho(bool bOption);
bool QtEC21_ConnCodeFormat(uint8_t u8Option);
bool QtEC21_TECharSet(char* pcCharSet);
bool QtEC21_SetErrFormat(uint8_t u8Option);
bool QtEC21_URC_Outport(char *pcOutPort);

// Serial interface functions
bool QtEC21_COM_SetDTR(uint8_t u8Option);
bool QtEC21_COM_SetBaudRate(uint32_t u32BaudRate);

// Status control functions
bool QtEC21_Stat_ConfigRI(char *pcOption);

// SIM functions
bool QtEC21_GetIMSI(char *pcIMSI);
bool QtEC21_SIM_QueryUSIM(GSM_SIM_StatTypedef *simRetCode);
bool QtEC21_SIM_SetPIN(char *pcOldPIN, char *pcNewPIN);

// Network functions
bool QtEC21_Net_QueryCS(void);
bool QtEC21_Net_QueryCS2(void);
bool QtEC21_Net_GetCSQ(uint8_t *pu8RSSI, uint8_t *pu8Ber);
bool QtEC21_Net_GetTime(uint8_t u8Option, char *pcBuf);

// Data mode to command mode function
bool QtEC21_Dat_2_Cmd(void);

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21
// GPS functions //
bool QtEC21_GPS_ConfigOutport(void);
bool QtEC21_GPS_ConfigNMEA_Src(void);
bool QtEC21_GPS_ConfigGPS_NMEA_Type(void);
bool QtEC21_GPS_ConfigGLONASS_NMEA_Type(void);
bool QtEC21_GPS_ConfigGalileo_NMEA_Type(void);
bool QtEC21_GPS_ConfigBeidou_NMEA_Type(void);
bool QtEC21_GPS_ConfigGSV_Ext(void);
bool QtEC21_GPS_ConfigGNSS(void);
bool QtEC21_GPS_ConfigAutoGPS(void);
bool QtEC21_GPS_DelMsg(void);
bool QtEC21_GPS_TurnOn(void);
bool QtEC21_GPS_TurnOff(void);
bool QtEC21_GPS_GetLoc(char *pcBuf);
bool QtEC21_GPS_GetNMEA(char *pcBuf, char *pcType);
bool QtEC21_GPS_ConfigXTRA(bool bEnable);
bool QtEC21_GPS_XTRA_Time(char *pcTimeStamp);
bool QtEC21_GPS_XTRA_Data(char *pcFileName);
bool QtEC21_GPS_XTRA_DataQuery(uint16_t *pu16DurTime, time_t *pttValidTime);
void QtEC21_GPS_Config(void);
void QtEC21_GPS_EnableSendMsg(void);
void QtEC21_GPS_EnableRecvMsg(void);
void QtEC21_GPS_On(void);
void QtEC21_GPS_Off(void);
#endif

// SMS functions //
bool QtEC21_SMS_SelectServ(uint8_t u8Type);
bool QtEC21_SMS_SetFormat(uint8_t u8Type);
bool QtEC21_SMS_SCA(char* pcSCA, uint16_t u16Type);
bool QtEC21_SMS_SetStorage(char *pcMem1, char *pcMem2, char *pcMem3);
bool QtEC21_SMS_Del(uint16_t u16Idx, uint8_t u8DelFlag);
bool QtEC21_SMS_GetList(char* pcStat, uint8_t* pu8Buf);
bool QtEC21_SMS_Read(uint16_t u16Idx, uint8_t* pu8Buf);
bool QtEC21_SMS_Send(GSM_MsgDataTypeDef *pgsmMsgData);
bool QtEC21_SMS_Send2(GSM_MsgDataTypeDef *pgsmMsgData);
bool QtEC21_SMS_ReportConfig(uint8_t u8Mode, uint8_t u8Mt, uint8_t u8Bm,
                             uint8_t u8Ds, uint8_t u8Bfr);
bool QtEC21_SMS_SetParam(uint8_t u8Mode);
bool QtEC21_SMS_Parse(char* pcMsg, char* pcPhoneNbr, char* pcTime, char* pcContent);
bool QtEC21_SMS_GetUnread(uint16_t *pu16MsgIdx, char* pcPhoneNbr, 
	                        char* pcTime, char* pcContent);
bool QtEC21_SMS_GetRead(uint16_t *pu16MsgIdx, char* pcPhoneNbr, 
	                      char* pcTime, char* pcContent);

// Phone call functions //
bool QtEC21_Call_Ans(void);
GSM_StatusTypeDef QtEC21_Call_Dial(char* pcPhoneNbr);
bool QtEC21_Call_Disconn(void);
bool QtEC21_Call_Cmd_2_Dat(void);
bool QtEC21_Call_ConnWaitTime(uint8_t u8Time);
bool QtEC21_Call_GetList(uint8_t* pu8Buf);
bool QtEC21_Call_CheckRing(void);
bool QtEC21_Call_CheckIncoming(void);

// Packet domain functions
bool QtEC21_PD_Attach(bool bOption);
bool QtEC21_PD_ConfigPDP(uint8_t u8ID, char *pcAPN, char *pcAddr);
bool QtEC21_PD_QualityReq(uint8_t u8ID, TCP_PDP_ContextStruct *pdpInstance);
bool QtEC21_PD_QualityMin(uint8_t u8ID, TCP_PDP_ContextStruct *pdpInstance);
bool QtEC21_PD_Activ(uint8_t u8ID, bool bOption);
bool QtEC21_PD_GetAddr(uint8_t u8ID, char *pcAddr);
bool QtEC21_PD_QueryPS(void);
bool QtEC21_PD_QueryPS2(void);

// HW functions
bool QtEC21_HW_GetRTC(char *pcBuf);

// TCP/IP functions //
void QtEC21_TCP_Init(void);
bool QtEC21_TCP_ConfigPDP(uint8_t u8PDP_ID, char *pcAPN, char *pcUser_1_Name, char *pcPwd);
bool QtEC21_TCP_ConfigQoS(void);
bool QtEC21_TCP_ActivPDP(uint8_t u8PDP_ID);
bool QtEC21_TCP_GetIP(uint8_t u8PDP_ID, char* pcIP_Addr);
bool QtEC21_TCP_DeactivPDP(uint8_t u8PDP_ID);
bool QtEC21_TCP_OpenConn(uint8_t u8PDP_ID, uint8_t u8ConnID, GSM_SocketConnTypeDef *pgsmConnHandle);
bool QtEC21_TCP_OpenConn2(uint8_t u8PDP_ID, uint8_t u8ConnID, GSM_SocketConnTypeDef gsmConnHandle);
void QtEC21_TCP_CloseConn2(uint8_t u8SocketID, uint16_t u16TimeOut);
bool QtEC21_TCP_CloseConn(uint8_t u8SocketID, uint16_t u16TimeOut);
bool QtEC21_TCP_IsConnect(bool bQueryType, uint8_t u8ID, bool* bIsConnect);
MsgSendStatusTypeDef QtEC21_TCP_Send(GSM_SocketDataTypeDef *pgsmSocketData);
bool QtEC21_TCP_SendingStatus(uint8_t u8SocketID, uint16_t* pu16NACK_Bytes);
bool QtEC21_TCP_RecvDataLen(uint8_t u8SocketID, uint16_t* pu16RecvBytesNum);
bool QtEC21_TCP_GetRecvData(uint8_t u8SocketID, uint8_t* pu8Buf, uint16_t u16Len,
                            uint16_t* u16ReadLen);
bool QtEC21_TCP_Cmd_2_Dat(uint8_t u8SocketID);

// FTP functions //
bool QtEC21_FTP_CfgPDP(uint8_t u8PDP_ID);
bool QtEC21_FTP_CfgAcc(char *pcUser_1_Name, char *pcPwd);
bool QtEC21_FTP_CfgFileType(void);
bool QtEC21_FTP_CfgTransMode(void);
bool QtEC21_FTP_CfgTimeout(uint16_t u16TimeOut);
bool QtEC21_FTP_CfgSSL_Type(bool bSSL_Type);
bool QtEC21_FTP_CfgSSL_ID(uint8_t u8SSL_ID);
bool QtEC21_FTP_SSL_Ver(uint8_t u8SSL_ID, uint8_t u8SSL_Ver);
bool QtEC21_FTP_SSL_Cipher(uint8_t u8SSL_ID, char *pcCipher);
bool QtEC21_FTP_SSL_CA(uint8_t u8SSL_ID, char *pcSSL_CA_Path);
bool QtEC21_FTP_SSL_SecLV(uint8_t u8SSL_ID, uint8_t u8SSL_SecLV);
bool QtEC21_FTP_Login(char *pcHostName, uint16_t u16Port);
bool QtEC21_FTP_SetDir(char *pcDirPath);
bool QtEC21_FTP_MakeDir(char *pcDirPath);
bool QtEC21_FTP_UploadFile(char *pcLocalName, char *pcFileName);
bool QtEC21_FTP_UploadDirect(uint8_t *pu8DatBuf, char *pcUploadFile, uint32_t u32UploadPos, 
	                           uint16_t u16UploadLen, bool bUploadFinish);
bool QtEC21_FTP_DownloadFile(char *pcFileName, char *pcLocalDest, uint32_t u32FilePtr);
bool QtEC21_FTP_FileSize(char *pcFileName);
bool QtEC21_FTP_FileDate(char *pcFileName);
bool QtEC21_FTP_TransLen(void);
bool QtEC21_FTP_FileDel(char *pcFileName);
bool QtEC21_FTP_Logout(void);

// HTTP functions
bool QtEC21_HTTP_CfgPDP(uint8_t u8PDP_ID);
bool QtEC21_HTTP_RespHeader(bool bEnable);
bool QtEC21_HTTP_SetURL(char *pcURL);
bool QtEC21_HTTP_Get(void);
bool QtEC21_HTTP_ReadFile(char *pcFileName);
bool QtEC21_HTTP_Cancel(void);

// File operation functions //
bool QtEC21_FileList(char *pcPattern, uint8_t *pu8Resp, uint16_t *pu16RetLen);
bool QtEC21_FileUpload(char *pcFileName, char *pcLocalName);
bool QtEC21_FileOpen(char *pcFileName, uint32_t *pu32FileHandle, uint8_t u8Mode);
bool QtEC21_FileSeek(uint32_t u32FileHandle, uint32_t u32FilePtr);
bool QtEC21_FileRead(uint32_t u32FileHandle, uint32_t u32ReadLen, uint32_t *pu32ActualLen,
                     char *pcReadContent);
bool QtEC21_FileWrite(uint32_t u32FileHandle, uint8_t *pu8DatBuf, uint16_t u16WriteLen);
bool QtEC21_FileClose(uint32_t u32FileHandle);
bool QtEC21_FileDelete(char *cFileName);

// Hardware related functions
bool QtEC21_HW_SleepEn(bool bOption);
#endif // #if ENABLE_GSM_QTEC21

//============================================================================//
#endif /* _GSM_QTEC21_H_ */
