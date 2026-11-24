#ifndef _COMM_FUNC_H_
#define _COMM_FUNC_H_
//============================================================================//


//============================================================================//
// User Includes
//============================================================================//

#include "stm32fxxx_hal_user.h"
#include "DataType.h"


//============================================================================//
// User Externs
//============================================================================//

extern const char * const cWeekDay[];

extern const char * const cMonth[];


//============================================================================//
// User Variables
//============================================================================//

static const uint32_t u32CRC32_TABLE[] =
{
	0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
	0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
	0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
	0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
	0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
	0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
	0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
	0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
	0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
	0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
	0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
	0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
	0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
	0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
	0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
	0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,

	0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
	0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
	0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
	0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
	0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
	0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
	0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
	0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
	0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
	0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
	0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
	0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
	0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
	0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
	0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
	0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,

	0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
	0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
	0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
	0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
	0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
	0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
	0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
	0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
	0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
	0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
	0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
	0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
	0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
	0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
	0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
	0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,

	0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
	0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
	0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
	0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
	0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
	0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
	0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
	0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
	0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
	0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
	0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
	0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
	0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
	0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
	0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
	0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D,
};


//============================================================================//
// User Defines
//============================================================================//

#define PI						              3.14159265358979323e0

// radius(®|«×) = PI/180
#define radius					            1.74532925199432957e-002

// radian(©·«× or Ñy«×) = 180/PI
#define radian					            5.72957795130823208e1

#define EarthRadius				          6378137ul

#define mS_2_uS(mS)				          ((mS) * 1000)

#define mS_2_S(mS)				          ((mS) / 1000)

#define mS_2_Hz(mS)				          ((float)1000 / (mS))

#define S_2_mS(S)				            ((S) * 1000)

#define S_2_uS(S)				            ((S) * 1000000)

#define S_2_MIN(S)				          ((S) / 60)

#define S_2_HOUR(S)				          ((S) / 3600)
 
#define S_2_DAY(S)				          ((S) / 86400)

#define MINUTE_2_SECOND(MIN)	      ((MIN) * 60)

#define HOUR_2_SECOND(HOUR)		      ((HOUR) * 3600)

#define DAY_2_SECOND(DAY)		        ((DAY) * 86400)

#define KM_2_MILES(KM)			        ((KM) * 0.621371)

#define KM_2_M(KM)				          ((KM) * 1000)

#define KM_2_CM(KM)				          ((KM) * 100000)

#define KM_2_MM(KM)				          ((KM) * 1000000)

#define MILES_2_KM(MI)			        ((MI) * 1.609344)

#define KNOT_2_M(KNOT)			        ((KNOT) * 1852)

#define M_2_KNOT(M)				          ((M) * 0.00053995)

#define M_2_100M(M)				          ((M) / 100)

#define M_2_FT(M)				            ((M) * 3.280840)

#define M_2_10M(M)				          ((M) / 10)

#define M_2_HM(M)				            ((M) / 100)	

#define M_4_100M(M)				          ((M) * 100)

#define HM_2_M(HM)				          ((HM) * 100)

#define M_2_KM(M)				            ((M) / 1000)

#define M_2_CM(M)				            ((M) * 100)

#define M_2_MM(M)				            ((M) * 1000)

#define MM_2_M(MM)				          ((MM) / 1000)

#define CRC8_RESET_VAL              (0x00)

#define CRC16_RESET_VAL             (0x0000)

#define CRC32_RESET_VAL             (0x00000000)

#define CALC_CRC32(CRC32, INPUT) \
        CRC32 = ((CRC32) >> 8) ^ u32CRC32_TABLE[(INPUT) ^ ((CRC32) & 0x000000FF)]

// User typedef 
typedef enum
{
	PROC_STEP_NULL = 0,
	PROC_STEP_1,
	PROC_STEP_2,
	PROC_STEP_3,
	PROC_STEP_4,
	PROC_STEP_5,
	PROC_STEP_6,
	PROC_STEP_7,
	PROC_STEP_8,
	PROC_STEP_9,
	PROC_STEP_10,
	PROC_STEP_11,
	PROC_STEP_12,
	PROC_STEP_13,
	PROC_STEP_14,
	PROC_STEP_15,
	PROC_STEP_16,
	PROC_STEP_17,
	PROC_STEP_18,
	PROC_STEP_19,
	PROC_STEP_20,
	PROC_STEP_21,
	PROC_STEP_22,
	PROC_STEP_23,
	PROC_STEP_24,
	PROC_STEP_25,
	PROC_STEP_26,
	PROC_STEP_27,
	PROC_STEP_28,
	PROC_STEP_29,
	PROC_STEP_30,
	PROC_STEP_31,
	PROC_STEP_32,
	PROC_STEP_33,
	PROC_STEP_34,
	PROC_STEP_35,
	PROC_STEP_36,
	PROC_STEP_37,
	PROC_STEP_38,
	PROC_STEP_39,
	PROC_STEP_40,
	PROC_STEP_41,
	PROC_STEP_42,
	PROC_STEP_43,
	PROC_STEP_44,
	PROC_STEP_45,
	PROC_STEP_46,
	PROC_STEP_47,
	PROC_STEP_48,
	PROC_STEP_49,
	PROC_STEP_50

} ProcStepTypeDef;

//============================================================================//
// User Externs
//============================================================================//

extern const uint8_t ASCII_WORD[];

extern const uint8_t TWord[4][32];
	

//============================================================================//
// User Function Prototypes
//============================================================================//

void Delay_uS(const uint32_t u32_uSec);
uint8_t Bin_2_BCD(uint8_t u8BinVal);
uint8_t BCD_2_Bin(uint8_t u8BCD_Val);
uint8_t LowBCDToASCII(uint8_t u8BCDByte);
uint8_t HIGHBCDToASCII(uint8_t u8BCDByte);
uint8_t LowBCDToNum(uint8_t u8BCDByte);
uint8_t HIGHBCDToNum(uint8_t u8BCDByte);
uint8_t Bin_2_BCD2(uint8_t u8BinValue);
uint8_t BCD2_2_Bin(uint8_t u8BCDValue);
void StopChar(char *pcStr, char cEnd);
void ChangeChar(char *pcStr, char cSource, char cTarget);
uint8_t CountChar(char *pcStr, char cChar);
bool FindSequence(char* pcDat, char* pcSeq, uint16_t* pu16Pos);
void CRC8_Reset(uint8_t *pu8CRC_Value);
uint8_t CRC8_Calc(uint8_t u8CRC8_Value, uint8_t u8Dat);
void CRC16_Reset(uint16_t *pu16CRC16_Value);
uint16_t CRC16_Calc(uint16_t u16CRC16_Value, uint8_t u8Dat);
void CRC32_Reset(uint32_t *pu32CRC32_Value);
uint32_t CRC32_Calc(uint32_t u32CRC32_Value, uint8_t u8Dat);
uint8_t CalCheckSum(uint8_t *pu8Dat, uint32_t u32DatLen);


//============================================================================//
#endif //_COMM_FUNC_H_
