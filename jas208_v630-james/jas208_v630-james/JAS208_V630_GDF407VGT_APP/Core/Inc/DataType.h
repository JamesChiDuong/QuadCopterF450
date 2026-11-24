#ifndef _DATA_TYPE_H_
#define _DATA_TYPE_H_


//============================================================================//
// User Includes
//============================================================================//

#include <stdbool.h>


//============================================================================//
// User TypeDefs
//============================================================================//

typedef	unsigned char	BYTE;
typedef unsigned short 	WORD;
typedef unsigned long	DWORD;
typedef	float			FLOAT;
typedef bool			BOOL;

#ifndef FALSE
#define FALSE false
#define TRUE true
#endif

#pragma pack(1)

typedef union
{
	BYTE _U8;

	struct
	{
		unsigned _0 : 1;
		unsigned _1 : 1;
		unsigned _2 : 1;
		unsigned _3 : 1;
		unsigned _4 : 1;
		unsigned _5 : 1;
		unsigned _6 : 1;
		unsigned _7 : 1;
	} bit;
} MEM_U8_t;

typedef union
{
	WORD	_U16;
	BYTE	_U8[2];

	struct
	{
		unsigned _00 : 1;
		unsigned _01 : 1;
		unsigned _02 : 1;
		unsigned _03 : 1;
		unsigned _04 : 1;
		unsigned _05 : 1;
		unsigned _06 : 1;
		unsigned _07 : 1;
		unsigned _08 : 1;
		unsigned _09 : 1;
		unsigned _10 : 1;
		unsigned _11 : 1;
		unsigned _12 : 1;
		unsigned _13 : 1;
		unsigned _14 : 1;
		unsigned _15 : 1;
	} bit;
} MEM_U16_t;

typedef union
{
	DWORD	_U32;
	WORD	_U16[2];
	BYTE	_U8[4];

	struct
	{
		unsigned _00 : 1;
		unsigned _01 : 1;
		unsigned _02 : 1;
		unsigned _03 : 1;
		unsigned _04 : 1;
		unsigned _05 : 1;
		unsigned _06 : 1;
		unsigned _07 : 1;
		unsigned _08 : 1;
		unsigned _09 : 1;
		unsigned _10 : 1;
		unsigned _11 : 1;
		unsigned _12 : 1;
		unsigned _13 : 1;
		unsigned _14 : 1;
		unsigned _15 : 1;
		unsigned _16 : 1;
		unsigned _17 : 1;
		unsigned _18 : 1;
		unsigned _19 : 1;
		unsigned _20 : 1;
		unsigned _21 : 1;
		unsigned _22 : 1;
		unsigned _23 : 1;
		unsigned _24 : 1;
		unsigned _25 : 1;
		unsigned _26 : 1;
		unsigned _27 : 1;
		unsigned _28 : 1;
		unsigned _29 : 1;
		unsigned _30 : 1;
		unsigned _31 : 1;
	} bit;
} MEM_U32_t;

typedef struct
{
	BYTE	CENTURY;
	BYTE	YEAR;
	BYTE	MONTH;
	BYTE	DAY;
	BYTE	HOUR;
	BYTE	MINUTE;
	BYTE	SECOND;
} DataTime_t;

#pragma pack()


//============================================================================//
// User Defines
//============================================================================//

//#ifndef NULL
//  #define NULL  (void*)0
//#endif

#define STR_NULL				     ""

#define GET_MSB(X)				   (BYTE)((X & 0xFF00) >> 8)

#define GET_LSB(X)				   (BYTE)(X & 0xFF)

#define OFFSET_SIZE(s, m)		 sizeof(((s *)0)->m)

#define TO_CONN(x,y)			   x##y


//============================================================================//
#endif //_DATA_TYPE_H_

