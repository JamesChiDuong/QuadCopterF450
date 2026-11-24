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
#define CUSTOMER_NORMAL			          (0) // STANDARD
#define CUSTOMER_JASS_EPA		          (1)	// 捷世林環保車機
#define CUSTOMER_JASS_DEMO		        (2)	// 捷世林展示機

#define CUSTOMER_ARTC			            (10) // 國內認證 : ARTC
#define CUSTOMER_TTIA			            (11) // 國內認證 : TTIA
#define CUSTOMER_EPA			            (12) // 國內認證 : 環保署監控車機

#define CUSTOMER_MASTER			          (30) // 國內客戶(車體廠) : 成運汽車
#define CUSTOMER_TANGENG		          (31) // 國內客戶(車體廠) : 唐榮車輛科技
#define CUSTOMER_DAF                  (32) // 國內客戶(車體廠) : DAF
#define CUSTOMER_DAF_2                (33) // 國內客戶(車體廠) : DAF-2
#define CUSTOMER_MAN                  (34) // 國內客戶(車體廠) : MAN
#define CUSTOMER_SYE									(35) // 國內客戶(車體廠) : 順益汽車

#define CUSTOMER_KINGLONG		          (40) // 國外客戶(車體廠) : 金龍汽車

#define CUSTOMER_EUPFIN 		          (50) // 國內客戶(DEALER) : 弋揚科技
#define CUSTOMER_SKYEYE			          (51) // 國內客戶(DEALER) : 天眼衛星科技		 
#define CUSTOMER_EUPFIN_B 		        (52) // 國內客戶(DEALER) : 弋揚科技_V2

#define CUSTOMER_FORMOSA		          (60) // 國內客戶(END USER) : 台塑貨運	 
#define CUSTOMER_ARTC_DMD		          (61) // 國內客戶(END USER) : ARTC - DEVICE , 架駛行為分析儀 
#define CUSTOMER_CPTI			            (62) // 國內客戶(END USER) : 首都客運+台北客運
#define CUSTOMER_FPTC			            (63) // 國內客戶(END USER) : 台塑貨運精簡版

#define CUSTOMER_FCFC			            (65) // 國內客戶(END USER) : FCFC

#define CUSTOMER_GDBUS                (70)

#define CUSTOMER_TST			            (110)	// 國外客戶(END USER) : 越南 TST

#define CUSTOMER_ECCOMM			          (200) // SAMPLE TEST : 易通通訊		 

#define CUSTOMER_TESTB                (99)

// Setup for CUSTOMER
#define CUSTOMER_TYPE		  CUSTOMER_NORMAL
//#define CUSTOMER_TYPE			CUSTOMER_JASS_EPA
//#define CUSTOMER_TYPE			CUSTOMER_JASS_DEMO

//#define CUSTOMER_TYPE			CUSTOMER_ARTC
//#define CUSTOMER_TYPE			CUSTOMER_TTIA
//#define CUSTOMER_TYPE			CUSTOMER_EPA

//#define CUSTOMER_TYPE			CUSTOMER_MASTER
//#define CUSTOMER_TYPE			CUSTOMER_TANGENG
//#define CUSTOMER_TYPE     CUSTOMER_DAF
//#define CUSTOMER_TYPE     CUSTOMER_DAF_2
//#define CUSTOMER_TYPE     CUSTOMER_MAN
//#define CUSTOMER_TYPE			CUSTOMER_SYE

//#define CUSTOMER_TYPE			CUSTOMER_KINGLONG

//#define CUSTOMER_TYPE			CUSTOMER_EUPFIN
//#define CUSTOMER_TYPE			CUSTOMER_EUPFIN_B
//#define CUSTOMER_TYPE			CUSTOMER_SKYEYE

//#define CUSTOMER_TYPE			CUSTOMER_FORMOSA
//#define CUSTOMER_TYPE			CUSTOMER_FCFC

//#define CUSTOMER_TYPE     CUSTOMER_GDBUS

//#define CUSTOMER_TYPE			CUSTOMER_ARTC_DMD
//#define CUSTOMER_TYPE			CUSTOMER_CPTI
//#define CUSTOMER_TYPE			CUSTOMER_FPTC

//#define CUSTOMER_TYPE			CUSTOMER_TST

//#define CUSTOMER_TYPE			CUSTOMER_ECCOMM

//#define CUSTOMER_TYPE     CUSTOMER_TESTB

// Main server types
//#define MAIN_SERVER_JASSLIN         (0) // Main server is Jasslin company server
//#define MAIN_SERVER_GOVERNMENT      (1) // Main server is government server

// Setup for parameters //
#define STR_CUST                    " " // Customer name

#define TIME_ZONE_OFFSET_DEF	      0U // Local timezone offset

#define GSM_SERVER_JASS             1U
#define GSM_SERVER_VDRS             2U
#define GSM_SERVER_GOV              3U

#define GSM_SERVER_DEF              GSM_SERVER_VDRS
#define GSM_SERVER_EXT              GSM_SERVER_JASS

//#define MAIN_SERVER                 MAIN_SERVER_JASSLIN // SOCKET_BANK_DEF will be MAIN_SERVER

// Setup for function enable //

// Only for development process !!! //
// In testing version and official release function please set this option to 0 !!! //
#define ENABLE_MULTI_DEBUG          (0) // Enable multiple debug modes 

#define ENABLE_CAN				          (0) // Enable CAN Bus functions

#define ENABLE_DEBUG_MSG		        (0) // Enable print all debug messages

#define ENABLE_DVR                  (0) // Enable DVR function

#define ENABLE_FTPS                 (0) // Enable access to FTPS server

#define ENABLE_GPS_MODULE		        (0) // Enable GPS module

#define ENABLE_GS_MC3210		        (0) // Enable G-Sensor

#define ENABLE_GSM_NET			        (0) // Enable GSM function

#define ENABLE_GSM_QTEC21		        (0) // Enable QtEC21 module as GSM module

#define ENABLE_IAP			            (0) // Enable IAP

#define ENABLE_INDEX_BUFFER         (0) // Enable handling SD file index using buffer

#define ENABLE_MULTI_SERVER			    (0) // Enable multiple servers

#define ENABLE_PC_TRANS             (0) // Enable PC file transfer

#define ENABLE_POWER_CHECK_VOLT		  (0) // Enable checking power by measuring voltage

#define ENABLE_RELAY_CONTROL_BOARD	(0)	// Enable Relay control board

#define ENABLE_SD_BUFFER            (0) // Do SD 1 min task using buffer

#define ENABLE_SYSLOG               (0) // Enable write SYSLOG
 
#define ENABLE_UPLOAD_LOG		        (0) // Enable upload datalog

#define ENABLE_USER_ALARM		        (0) // Enable user warning using command 


// Define for particular customers //

// Test B
#if CUSTOMER_TYPE == CUSTOMER_TESTB
#undef STR_CUST
#define STR_CUST				            "TESTB"

#undef TIME_ZONE_OFFSET_DEF
#define TIME_ZONE_OFFSET_DEF	      HOUR_2_SECOND(8)

//#undef MAIN_SERVER
//#define MAIN_SERVER                 MAIN_SERVER_JASSLIN
#undef GSM_SERVER_DEF
#define GSM_SERVER_DEF              GSM_SERVER_VDRS

#undef ENABLE_CAN
#define ENABLE_CAN					        (0)

#undef ENABLE_DEBUG_MSG
#define ENABLE_DEBUG_MSG      	    (1)

#undef ENABLE_DVR
#define ENABLE_DVR      	          (0)

#undef ENABLE_FTPS
#define ENABLE_FTPS      	          (0)

#undef ENABLE_GSM_NET
#define ENABLE_GSM_NET	            (0)

#undef ENABLE_IAP
#define ENABLE_IAP                  (1)

#undef ENABLE_MULTI_DEBUG
#define ENABLE_MULTI_DEBUG          (0)

#undef ENABLE_MULTI_SERVER   
#define ENABLE_MULTI_SERVER         (0) 

#undef ENABLE_PC_TRANS
#define ENABLE_PC_TRANS             (0) 

#undef ENABLE_POWER_CHECK_VOLT
#define ENABLE_POWER_CHECK_VOLT     (0)

#undef ENABLE_RELAY_CONTROL_BOARD
#define ENABLE_RELAY_CONTROL_BOARD  (0)

#undef ENABLE_UPLOAD_LOG
#define ENABLE_UPLOAD_LOG  	        (0)
#endif

// Standard customer
#if CUSTOMER_TYPE == CUSTOMER_NORMAL
#undef STR_CUST
#define STR_CUST				            "STD"

#undef TIME_ZONE_OFFSET_DEF
#define TIME_ZONE_OFFSET_DEF	      HOUR_2_SECOND(8)

//#undef MAIN_SERVER
//#define MAIN_SERVER                 MAIN_SERVER_JASSLIN
#undef GSM_SERVER_DEF
#define GSM_SERVER_DEF              GSM_SERVER_VDRS

#undef ENABLE_CAN
#define ENABLE_CAN					        (0)

#undef ENABLE_DEBUG_MSG
#define ENABLE_DEBUG_MSG      	    (0)

#undef ENABLE_DVR
#define ENABLE_DVR      	          (0)

#undef ENABLE_FTPS
#define ENABLE_FTPS      	          (0)

#undef ENABLE_GSM_NET
#define ENABLE_GSM_NET	            (0)

#undef ENABLE_IAP
#define ENABLE_IAP                  (1)

#undef ENABLE_MULTI_DEBUG
#define ENABLE_MULTI_DEBUG          (0)

#undef ENABLE_MULTI_SERVER   
#define ENABLE_MULTI_SERVER         (0) 

#undef ENABLE_PC_TRANS
#define ENABLE_PC_TRANS             (0) 

#undef ENABLE_POWER_CHECK_VOLT
#define ENABLE_POWER_CHECK_VOLT     (0)

#undef ENABLE_RELAY_CONTROL_BOARD
#define ENABLE_RELAY_CONTROL_BOARD  (0)

#undef ENABLE_UPLOAD_LOG
#define ENABLE_UPLOAD_LOG  	        (0)
#endif


// General define //

// Enable GPS module 
#if HW_TYPE == HW_JAS106 || \
    HW_TYPE == HW_JAS208 || \
		HW_TYPE == HW_JAS306
  #undef ENABLE_GPS_MODULE
  #define ENABLE_GPS_MODULE	          (1)
#endif

// Enable hardware module for GSM functions
#if ENABLE_GSM_NET == 1
  #undef ENABLE_GSM_QTEC21
  #define ENABLE_GSM_QTEC21	          (1)
#endif

#if GSM_SERVER_DEF == GSM_SERVER_EXT
  #undef GSM_SERVER_EXT
	#define GSM_SERVER_EXT              (0)
#endif


//============================================================================//
#endif //_CONFIG_H_
