#ifndef _RINGBUF_H_
#define _RINGBUF_H_
//============================================================================//


//============================================================================//
// User Includes
//============================================================================//

#include <stdint.h>

#include "DataType.h"


//============================================================================//
// User Defines
//============================================================================//

#define RING_BUFFER_UNINITIALIZED   FALSE

#define RING_BUFFER_INITIALIZED     TRUE


//============================================================================//
// User Typedef Variables
//============================================================================//

// Struct of ring buffer state
typedef enum
{
	RING_BUFFER_OK = 0x0,

	RING_BUFFER_FULL,

	RING_BUFFER_NO_SUFFICIENT_SPACE,

	RING_BUFFER_ERROR
	
} RingBufStatTypeDef;

// Struct for handling a ring buffer
typedef struct
{
	// Pointer to the beginning of the ring buffer
	uint8_t *pu8Buf;

	// Size of the ring buffer
	uint16_t u16Size;

	// u16Head: Pointer to the last written position
	// u16Tail: Pointer to the last read position
	uint16_t u16Head, u16Tail;

	bool rbsFlag;

} RingBufTypeDef;


//============================================================================//
// User Function Prototypes
//============================================================================//

void RingBuf_Init(RingBufTypeDef *prbBuf);
void RingBuf_DeInit(RingBufTypeDef *prbBuf);
void RingBuf_Reset(RingBufTypeDef *prbBuf);
uint16_t RingBuf_GetFreeSpace(RingBufTypeDef *prbBuf);
uint16_t RingBuf_GetDataLength(RingBufTypeDef *prbBuf);
uint16_t RingBuf_GetDataLenPos(RingBufTypeDef *prbBuf, uint16_t u16Pos);
void RingBuf_DataShift(RingBufTypeDef *prbBuf, uint16_t u16Pos, uint16_t u16DatLen,
                       uint16_t u16ShiftNum);
uint16_t RingBuf_Peek(RingBufTypeDef *prbBuf, uint16_t u16Pos, uint8_t *pu8Dat,
                      uint16_t u16DatLen);
uint16_t RingBuf_ReadPos(RingBufTypeDef *prbBuf, uint16_t u16Pos, uint8_t *pu8Dat,
                         uint16_t u16DatLen);
uint16_t RingBuf_Read(RingBufTypeDef *prbBuf, uint8_t *pu8Dat, uint16_t u16DatLen);
RingBufStatTypeDef RingBuf_Write(RingBufTypeDef *prbBuf, uint8_t *pu8Dat,
                                 uint16_t u16DatLen);
bool RingBuf_Find(RingBufTypeDef *prbBuf, char *pcSeq, uint16_t *pu16Pos);
bool RingBuf_FindPos(RingBufTypeDef *prbBuf, char* pcSeq, uint16_t* pu16Pos,
                     uint16_t u16StartPos);


//============================================================================//
#endif //_RINGBUF_H_
