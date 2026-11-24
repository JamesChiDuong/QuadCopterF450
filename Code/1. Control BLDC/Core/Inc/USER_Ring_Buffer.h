/*
 * Ring_Buffer.h
 *
 *  Created on: Mar 1, 2025
 *      Author: PC
 */
/*
 * OVERVIEW
 *   Byte ring buffer used for UART reception (explanation of head/tail and
 *   the index arithmetic in USER_Ring_Buffer.c).
 *   RingBuffTypeDef fields:
 *     u8ptrBuff : storage (allocated by RingBuf_Init)
 *     u16Size   : array size in bytes (usable capacity = u16Size - 1)
 *     u16Tail   : index of the oldest unread byte (moved by the reader)
 *     u16Head   : index where the next byte is written (moved by the writer)
 *     bFlag     : RING_BUFFER_INITIALIZED after a successful Init
 */

#ifndef INC_USER_RING_BUFFER_H_
#define INC_USER_RING_BUFFER_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
typedef enum RingBuffState
{
	RING_BUFFER_INITIALIZED,
	RING_BUFFER_UNINITIALIZED,
	RING_BUFFER_FULL,
	RING_BUFFER_NO_SUFFICIENT_SPACE,
}RingBuffState;
typedef struct RingBuffTypeDef
{
	uint8_t* u8ptrBuff;

	uint16_t u16Size;

	uint16_t u16Tail;

	uint16_t u16Head;

	bool bFlag;
}RingBuffTypeDef;

void RingBuf_Init(RingBuffTypeDef *ptrBuff);

void RingBuf_DeInit(RingBuffTypeDef *ptrBuff);

void RingBuf_Reset(RingBuffTypeDef *ptrBuff);

uint16_t RingBuf_GetFreeSpace(RingBuffTypeDef *ptrBuff);

uint16_t RingBuf_GetDataLength(RingBuffTypeDef *ptrBuff);

uint16_t RingBuf_GetDataLenPos(RingBuffTypeDef *ptrBuff, uint16_t u16Pos);

uint16_t RingBuf_ReadPos(RingBuffTypeDef *ptrBuff, uint16_t u16Pos, uint8_t* ptru8Data, uint16_t u16DatLen);

uint16_t RingBuf_Read(RingBuffTypeDef *ptrBuff, uint8_t* ptru8Data, uint16_t u16DatLen);

uint16_t RingBuf_Write(RingBuffTypeDef *ptrBuff, uint8_t* ptru8Data, uint16_t u16DatLen);

bool RingBuf_FindPos(RingBuffTypeDef *ptrBuff, char* pcSeq, uint16_t *pu16Pos, uint16_t u16StartPos);

bool RingBuf_Find(RingBuffTypeDef *ptrBuff, char* pcSeq, uint16_t *pu16Pos);

uint16_t RingBuf_Peek(RingBuffTypeDef *ptrBuff, uint16_t u16Pos, uint8_t* pu8Data, uint16_t u16DataLen);

void RingBuf_DataShift(RingBuffTypeDef *ptrBuff, uint16_t u16Pos,uint16_t u16DatLen, uint16_t u16ShiftNum);
#endif /* INC_USER_RING_BUFFER_H_ */
