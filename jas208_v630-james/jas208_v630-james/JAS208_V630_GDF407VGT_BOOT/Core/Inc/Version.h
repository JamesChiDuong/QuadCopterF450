#ifndef _VERSION_H_
#define _VERSION_H_
//============================================================================//
// User Defines
//============================================================================//
#include "HW_Config.h"


//============================================================================//
// User Defines
//============================================================================//
#define STR_TITLE						  "Jasslin Technology Co., Ltd."

#define HARDWARE_VERSION      "HW6.3D"
#define FIRMWARE_VERSION      "1.001"
#define VERSION_BUILD         (1001)
#define FW_VERSION_CHK			  ""
#define FIRMWARE_DATE				  "20240709"
#define FW_TEST_VERSION				""

// Define for product model name
#define MODEL_UNDEFINE		(0)		

#define JAS208S           (1)

#define JAS208_NO_GSM     "JAS208S"

#define JAS208_GSM        "JAS208U"

#define MODEL_JAS102R			(10)	
#define MODEL_JAS102S			(11)	
#define MODEL_JAS102G			(12)	
#define MODEL_JAS102M			(13)	 

#define MODEL_JAS201R			(20)	 
#define MODEL_JAS201S			(21)	 
#define MODEL_JAS201G			(22)	 
#define MODEL_JAS201M			(23) 
#define MODEL_JAS201D			(24)	 

#define MODEL_JAS121R			(30)	
#define MODEL_JAS121S			(31)	
#define MODEL_JAS121G			(32)	 
#define MODEL_JAS121W			(33)	 
#define MODEL_JAS121M			(34)	
#define MODEL_JAS121U			(35)	 

#define MODEL_JAS211R			(40)	 
#define MODEL_JAS211S			(41)	
#define MODEL_JAS211G			(42)	
#define MODEL_JAS211W			(43)	
#define MODEL_JAS211M			(44)
#define MODEL_JAS211U			(45)	

#define MODEL_JAS106R			(50)	 
#define MODEL_JAS106S			(51)	 
#define MODEL_JAS106G			(52)
#define MODEL_JAS106W			(53)	
#define MODEL_JAS106M			(54)	
#define MODEL_JAS106U			(55)	 

#define MODEL_JAS208S			(60)	 
#define MODEL_JAS208U			(61)	 

#define MODEL_NAME				MODEL_UNDEFINE

#if defined(JAS201R)
#undef MODEL_NAME
#define MODEL_NAME				MODEL_JAS201R
#endif

#if defined(JAS201S)
#undef MODEL_NAME
#define MODEL_NAME				MODEL_JAS201S
#endif

#if defined(JAS201G)
#undef MODEL_NAME
#define MODEL_NAME				MODEL_JAS201G
#endif

#if defined(JAS201M)
#undef MODEL_NAME
#define MODEL_NAME				MODEL_JAS201M
#endif

#if defined(JAS201D)
#undef MODEL_NAME
#define MODEL_NAME				MODEL_JAS201D
#endif

#if defined(JAS121R)
#undef MODEL_NAME
#define MODEL_NAME				MODEL_JAS121R
#endif

#if defined(JAS121S)
#undef MODEL_NAME
#define MODEL_NAME				MODEL_JAS121S
#endif

#if defined(JAS121G)
#undef MODEL_NAME
#define MODEL_NAME				MODEL_JAS121G
#endif

#if defined(JAS121W)
#undef MODEL_NAME
#define MODEL_NAME				MODEL_JAS121W
#endif

#if defined(JAS121M)
#undef MODEL_NAME
#define MODEL_NAME				MODEL_JAS121M
#endif

#if defined(JAS121U)
#undef MODEL_NAME
#define MODEL_NAME				MODEL_JAS121U
#endif

#if defined(JAS211R)
#undef MODEL_NAME
#define MODEL_NAME				MODEL_JAS211R
#endif

#if defined(JAS211S)
#undef MODEL_NAME
#define MODEL_NAME				MODEL_JAS211S
#endif

#if defined(JAS211G)
#undef MODEL_NAME
#define MODEL_NAME				MODEL_JAS211G
#endif

#if defined(JAS211W)
#undef MODEL_NAME
#define MODEL_NAME				MODEL_JAS211W
#endif

#if defined(JAS211M)
#undef MODEL_NAME
#define MODEL_NAME				MODEL_JAS211M
#endif

#if defined(JAS211U)
#undef MODEL_NAME
#define MODEL_NAME				MODEL_JAS211U
#endif

// Heinz
#if defined(JAS106S)
#undef MODEL_NAME
#define MODEL_NAME				MODEL_JAS106S
#endif

#if defined(JAS106U)
#undef MODEL_NAME
#define MODEL_NAME				MODEL_JAS106U
#endif

#if defined(JAS208S)
#undef MODEL_NAME
#define MODEL_NAME				MODEL_JAS208S
#endif

#if defined(JAS208U)
#undef MODEL_NAME
#define MODEL_NAME				MODEL_JAS208U
#endif

//============================================================================//
// Product information
//============================================================================//

// Product name //
#define STR_PRODUCT				"UNKNOW"

// Heinz
#if MODEL_NAME == MODEL_JAS106S
#undef STR_PRODUCT
#define STR_PRODUCT				"JAS106S"
#endif

#if MODEL_NAME == MODEL_JAS106U
#undef STR_PRODUCT
#define STR_PRODUCT				"JAS106U"
#endif

#if MODEL_NAME == MODEL_JAS208S
#undef STR_PRODUCT
#define STR_PRODUCT				"JAS208S"
#endif

#if MODEL_NAME == MODEL_JAS208U
#undef STR_PRODUCT
#define STR_PRODUCT				"JAS208U"
#endif

// Hardware version information //
#define STR_HW				""

#if HW_VER == HW_JAS201_V400

#undef STR_HW
#define STR_HW				"HW201V400"

#if FLASH_SIZE == FLASH_SIZE_256K
#undef STR_HW
#define STR_HW			  "HW201V401"
#endif
#endif

#if HW_VER == HW_JAS121_V100
#undef STR_HW
#define STR_HW				"HW121V100"
#endif

#if HW_VER == HW_JAS121_V110
#undef STR_HW
#define STR_HW				"HW121V110"
#endif

#if HW_VER == HW_JAS211_V100
#undef STR_HW
#define STR_HW				"HW211V100"
#endif

#if HW_VER == HW_JAS211_V110

#undef STR_HW
#define STR_HW				"HW211_V100"	//JAS211A
#endif

#if HW_VER == HW_JAS211_V200

#undef STR_HW
#define STR_HW				"HW211_V200" // AMBO
#endif

#if HW_VER == HW_JAS106_V200
#undef STR_HW
#define STR_HW				"HW106_V200" // JAS106
#endif

#if HW_VER == HW_JAS208_V100
#undef STR_HW
#define STR_HW				"HW208_V100" // JAS208 V1
#endif

#if HW_VER == HW_JAS208_V200
#undef STR_HW
#define STR_HW				"HW208_V200" // JAS208 V2
#endif

#if HW_VER == HW_JAS208_V300
#undef STR_HW
#define STR_HW				"HW208_V300" // JAS208 V3
#endif

#if HW_VER == HW_JAS208_V330
#undef STR_HW
#define STR_HW				"HW208_V330" // JAS208 V3.3
#endif

#if HW_VER == HW_JAS208_V340
#undef STR_HW
#define STR_HW				"HW208_V340" // JAS208 V3.4
#endif

#if HW_VER == HW_JAS208_V350
#undef STR_HW
#define STR_HW				"HW208_V350" // JAS208 V3.5
#endif

#if HW_VER == HW_JAS208_V360
#undef STR_HW
#define STR_HW				"HW208_V360" // JAS208 V3.6
#endif

#if HW_VER == HW_JAS208_V600
#undef STR_HW
#define STR_HW				"HW208_V600" // JAS208 V6.0
#endif

#if HW_VER == HW_JAS208_V610
#undef STR_HW
#define STR_HW				"HW208_V610" // JAS208 V6.1
#endif

#if HW_VER == HW_JAS208_V630
#undef STR_HW
#define STR_HW				"HW208_V630" // JAS208 V6.3
#endif

#if HW_VER == HW_JAS208_V700
#undef STR_HW
#define STR_HW				"HW208_V700" // JAS208 V7.0
#endif
//============================================================================//
#endif //_VERSION_H_
