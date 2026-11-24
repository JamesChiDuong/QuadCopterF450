#ifndef  __base_typedef_h__
#define  __base_typedef_h__

#ifndef NULL
#define NULL    ((void *) 0)
#endif

typedef  unsigned char       u_8;
typedef  unsigned short int  u_16;
typedef  unsigned long       u_32;
typedef  signed char         s_8;
typedef  signed short int    s_16;
typedef  signed long         s_32;

typedef  unsigned char       uchar;
typedef  unsigned short int  uint;
typedef  unsigned long       ulong;
typedef  signed char         schar;
typedef  signed short int    sint;
typedef  signed long         slong;

/*
typedef  unsigned char       u8;
typedef  unsigned short int  u16;
typedef  unsigned long       u32;
typedef  signed char         s8;
typedef  signed short int    s16;
typedef  signed long         s32;
*/

typedef  unsigned char       U8;
typedef  unsigned short int  U16;
typedef  unsigned long       U32;
typedef  signed char         S8;
typedef  signed short int    S16;
typedef  signed long         S32;

typedef enum
{
	_false_ = 0,
	_true_  = 1
}
BIT_ENUM;    /* 位狀態 */

#define   SETBIT(d_byte,d_bit)    (d_byte|=(1<<d_bit))         /* 置位 */
#define   CLRBIT(d_byte,d_bit)    (d_byte&=(~(1<<d_bit)))      /* 清零 */
#define   TESTBIT(d_byte,d_bit)   (d_byte&(1<<d_bit))          /* 測試 */
#define   CPLBIT(d_byte,d_bit)    (d_byte^=(1<<d_bit))         /* 取反 */

#define   SUB_U8(dat, sub)      ((u_8)(dat-sub))
#define   SUB_U16(dat, sub)     ((u_16)(dat-sub))
#define   SUB_U32(dat, sub)     ((u_32)(dat-sub))              /* 求差值 */

#define   BIT_MOVE(n)           (((u_32)0x01)<<(n))            /* 移位定義 */

#endif
