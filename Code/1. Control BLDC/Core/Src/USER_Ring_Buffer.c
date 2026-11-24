/*
 * Ring_Buffer.c
 *
 *  Created on: Mar 1, 2025
 *      Author: PC
 */
/*
 * OVERVIEW
 *   A ring (circular) buffer of bytes: a fixed array used as a queue, where
 *   the indices wrap around to 0 at the end instead of moving data.
 *     u16Head : where the NEXT byte will be written (producer = UART RX ISR)
 *     u16Tail : the OLDEST unread byte (consumer = UARTTask)
 *     empty   : head == tail
 *     full    : head is one step behind tail -> one slot is always left
 *               unused so "full" and "empty" can be told apart. Capacity is
 *               therefore u16Size - 1 bytes.
 *
 *   Index arithmetic (always modulo u16Size; adding u16Size first keeps the
 *   unsigned subtraction from going negative):
 *     data length = (size + head - tail) % size
 *     free space  = (size + tail - head - 1) % size
 *   Example, size 8: tail = 6, head = 2 (data wrapped: slots 6,7,0,1)
 *     length = (8 + 2 - 6) % 8 = 4,  free = (8 + 6 - 2 - 1) % 8 = 3.
 *
 *   Besides plain Read/Write, this buffer supports searching a text sequence
 *   anywhere between tail and head (RingBuf_Find/FindPos) and reading out a
 *   frame that starts at an arbitrary position (RingBuf_ReadPos), which is
 *   what the terminal uses to extract "@QC+...\r\n".
 *
 *   Concurrency: the ISR only moves head, the task only moves tail, so simple
 *   single-producer/single-consumer use needs no lock. (RingBuf_Reset, called
 *   from the task, is the exception: it also sets head.)
 *   Memory: the array and the temporary search buffers come from RTOSmalloc().
 */
#include <USER_Ring_Buffer.h>
#include <USER_CommFunc.h>

/* Allocate u16Size bytes (u16Size must be set by the caller first) and
 * start empty. Does nothing for size 0 or if malloc fails. */
void RingBuf_Init(RingBuffTypeDef *ptrBuff)
{
	if(ptrBuff->u16Size == 0)
	{
		return;
	}

	ptrBuff->u8ptrBuff = (uint8_t*)RTOSmalloc(sizeof(uint8_t) * ptrBuff->u16Size);

	if(ptrBuff->u8ptrBuff == NULL)
	{
		return;
	}

	memset(ptrBuff->u8ptrBuff, 0x00, ptrBuff->u16Size);

	ptrBuff->u16Head = ptrBuff->u16Tail = 0;

	ptrBuff->bFlag = RING_BUFFER_INITIALIZED;
}

void RingBuf_DeInit(RingBuffTypeDef *ptrBuff)
{
	if(!ptrBuff)
	{
		return;
	}

	free(ptrBuff->u8ptrBuff);

	ptrBuff->u16Size = 0;

	ptrBuff->u16Head = ptrBuff->u16Tail = 0;

	ptrBuff->bFlag = RING_BUFFER_UNINITIALIZED;
}

/* Discard everything (head = tail = 0) */
void RingBuf_Reset(RingBuffTypeDef *ptrBuff)
{
	if(!ptrBuff)
	{
		return;
	}

	memset(ptrBuff->u8ptrBuff, 0x00, ptrBuff->u16Size);

	ptrBuff->u16Head = ptrBuff->u16Tail = 0;
}

uint16_t RingBuf_GetFreeSpace(RingBuffTypeDef *ptrBuff)
{
	if(!ptrBuff)
	{
		return 0;
	}

	if(ptrBuff->u8ptrBuff == NULL)
		return 0;

	return ((ptrBuff->u16Size + ptrBuff->u16Tail - ptrBuff->u16Head - 1) % ptrBuff->u16Size);
}

uint16_t RingBuf_GetDataLength(RingBuffTypeDef *ptrBuff)
{
	if(!ptrBuff)
	{
		return 0;
	}

	if(ptrBuff->u8ptrBuff == NULL)
		return 0;

	return ((ptrBuff->u16Size + ptrBuff->u16Head - ptrBuff->u16Tail) % ptrBuff->u16Size);
}

/* Number of bytes from index u16Pos up to head, or 0 if u16Pos is not inside
 * the stored data. "Inside" test: distance tail -> pos must not exceed the
 * data length. Example size 8, tail 6, head 2: pos 7 -> distance 1 <= 4 ok,
 * returns (8 + 2 - 7) % 8 = 3; pos 4 -> distance 6 > 4 -> 0. */
uint16_t RingBuf_GetDataLenPos(RingBuffTypeDef *ptrBuff, uint16_t u16Pos)
{
	if(!ptrBuff)
	{
		return 0;
	}

	if(ptrBuff->u8ptrBuff == NULL)
		return 0;

	// check if position between Tail and Head
	if(((ptrBuff->u16Size + u16Pos - ptrBuff->u16Tail) % ptrBuff->u16Size) > RingBuf_GetDataLength(ptrBuff))
	{
		return 0;
	}
	else
	{
		return ((ptrBuff->u16Size + ptrBuff->u16Head - u16Pos) % ptrBuff->u16Size);
	}
}

/* Move u16DatLen bytes starting at u16Pos forward by u16ShiftNum positions
 * (with wrap-around), through a temporary copy. Used by RingBuf_ReadPos to
 * keep the bytes that were in front of an extracted frame. */
void RingBuf_DataShift(RingBuffTypeDef *ptrBuff, uint16_t u16Pos,uint16_t u16DatLen, uint16_t u16ShiftNum)
{
	uint8_t* u8Temp;
	if(!ptrBuff)
	{
		return;
	}

	if(ptrBuff->u8ptrBuff == NULL)
		return;

	if(u16DatLen == 0)
		return;

	u8Temp = (uint8_t*)RTOSmalloc(sizeof(uint8_t)*u16DatLen);

	if(u8Temp == NULL)
	{
		return;
	}

	// Coppy data from original positions to temp buffer
	for(int i = 0; i < u16DatLen; i++)
		u8Temp[i] = ptrBuff->u8ptrBuff[(u16Pos + 1) % ptrBuff->u16Size];


	for(int i = 0; i < u16DatLen; i++)
	{
		uint16_t u16NewPos = (u16Pos + i + u16ShiftNum) % ptrBuff->u16Size;
		ptrBuff->u8ptrBuff[u16NewPos] = u8Temp[i];
	}
	free(u8Temp);
	u8Temp = NULL;
}
/* Read up to u16DatLen bytes starting at index u16Pos (somewhere between tail
 * and head) and remove them from the buffer. Idea: copy the frame out, shift
 * the bytes that were before it (tail .. u16Pos) forward by the frame length
 * so they close the gap, then advance tail by the frame length.
 *   before: [tail] x x @ Q C + ... \r \n [head]
 *   after :         ........ x x [head]   (frame removed, "x x" kept)
 * Returns the number of bytes copied. */
uint16_t RingBuf_ReadPos(RingBuffTypeDef *ptrBuff, uint16_t u16Pos, uint8_t* ptru8Data, uint16_t u16DatLen)
{
	uint16_t u16Cnt = 0;
	uint16_t u16ByteNum;
	if(!ptrBuff)
	{
		return 0;
	}

	if(ptrBuff->u8ptrBuff == NULL)
		return 0;

	if(RingBuf_GetDataLenPos(ptrBuff, u16Pos) == 0)
	{
		return 0;
	}

	u16ByteNum = (ptrBuff->u16Size + u16Pos - ptrBuff->u16Size) % ptrBuff->u16Size;

	while((u16Pos != ptrBuff->u16Head) && (u16Cnt < u16DatLen))
	{
		ptru8Data[u16Cnt++] = ptrBuff->u8ptrBuff[u16Pos];
		u16Pos = (u16Pos + 1) % ptrBuff->u16Size;
	}

	RingBuf_DataShift(ptrBuff, ptrBuff->u16Tail, u16ByteNum, u16Cnt);

	ptrBuff->u16Tail = (ptrBuff->u16Tail + u16Cnt) % ptrBuff->u16Size;

	return u16Cnt;

}
/* Plain FIFO read: take up to u16DatLen bytes from the tail. */
uint16_t RingBuf_Read(RingBuffTypeDef *ptrBuff, uint8_t* ptru8Data, uint16_t u16DatLen)
{
	uint16_t u16Cnt;

	if(!ptrBuff)
		return 0;

	if(ptrBuff->u8ptrBuff == NULL)
		return 0;

	while((ptrBuff->u16Tail != ptrBuff->u16Head) && (u16Cnt < u16DatLen))
	{
		ptru8Data[u16Cnt++] = ptrBuff->u8ptrBuff[ptrBuff->u16Tail];
		ptrBuff->u16Tail = (ptrBuff->u16Tail + 1) % ptrBuff->u16Size;

	}
	return u16Cnt;
}


/* Append u16DatLen bytes at the head (all or nothing). Called from the UART
 * RX interrupt. Returns the number of bytes written, or the
 * RING_BUFFER_FULL / RING_BUFFER_NO_SUFFICIENT_SPACE codes. */
uint16_t RingBuf_Write(RingBuffTypeDef *ptrBuff, uint8_t* ptru8Data, uint16_t u16DatLen)
{
	uint16_t u16Cnt = 0;
	uint16_t u16FreeSpace = 0;

	if(!ptrBuff)
		return 0;

	if(ptrBuff->u8ptrBuff == NULL)
		return 0;

	u16FreeSpace =RingBuf_GetFreeSpace(ptrBuff);

	if(u16FreeSpace == 0)
	{
		return RING_BUFFER_FULL;
	}
	else if(u16FreeSpace < u16DatLen)
	{
		return RING_BUFFER_NO_SUFFICIENT_SPACE;
	}

	while(u16Cnt < u16DatLen)
	{
		ptrBuff->u8ptrBuff[ptrBuff->u16Head] = ptru8Data[u16Cnt++];

		ptrBuff->u16Head = (ptrBuff->u16Head + 1) % ptrBuff->u16Size;

	}
	return u16Cnt;
}
/* Copy up to u16DataLen bytes starting at u16Pos WITHOUT removing them
 * (tail does not move). Used by the search functions. */
uint16_t RingBuf_Peek(RingBuffTypeDef *ptrBuff, uint16_t u16Pos, uint8_t* pu8Data, uint16_t u16DataLen)
{
	uint16_t u16Cnt = 0;

	if(RingBuf_GetDataLenPos(ptrBuff,u16Pos) == 0)
	{
		return 0;
	}

	while((u16Pos != ptrBuff->u16Head) && (u16Cnt < u16DataLen))
	{
		pu8Data[u16Cnt++] = ptrBuff->u8ptrBuff[u16Pos];
		u16Pos = (u16Pos + 1) % ptrBuff->u16Size;
	}
	return u16Cnt;
}

/* Search the text pcSeq (e.g. "@QC+") in the stored data, starting at tail.
 * Slides a window one byte at a time: at each position, Peek strlen(pcSeq)
 * bytes and compare. The last start position tried is head - strlen + 1
 * (the sequence must fit before head). On success *pu16Pos = index of the
 * first character and returns true. Nothing is removed. */
bool RingBuf_Find(RingBuffTypeDef *ptrBuff, char* pcSeq, uint16_t *pu16Pos)
{
	uint8_t *pu8BuffStr;
	uint16_t u16SeqLen;
	if(!ptrBuff)
		return 0;

	if(ptrBuff->u8ptrBuff == NULL)
		return 0;

	u16SeqLen = strlen(pcSeq);
	if((u16SeqLen == 0) || (u16SeqLen > 1024))
		return 0;

	// Check if the length of the character seuqence is smaller than the legnth of the data
	if(RingBuf_GetDataLength(ptrBuff) < u16SeqLen)
	{
		return 0;
	}

	pu8BuffStr = RTOSmalloc(sizeof(char) * u16SeqLen);
	if(pu8BuffStr == NULL)
	{
		return 0;
	}

	*pu16Pos = ptrBuff->u16Tail;

	while(((*pu16Pos) != ((ptrBuff->u16Size + ptrBuff->u16Head - u16SeqLen + 1) % ptrBuff ->u16Size)) &&((*pu16Pos) != (ptrBuff->u16Head)))
	{
		RingBuf_Peek(ptrBuff, (*pu16Pos), pu8BuffStr, u16SeqLen);
		if(strncmp((char*) pu8BuffStr, pcSeq, u16SeqLen) == 0)
		{
			free(pu8BuffStr);
			pu8BuffStr = NULL;
			return 1;
		}

		(*pu16Pos) = ((*pu16Pos) + 1) % ptrBuff->u16Size;
	}
	free(pu8BuffStr);
	pu8BuffStr = NULL;
	return 0;
	// CHeck if
}

/* Same as RingBuf_Find but starts searching at u16StartPos instead of tail
 * (used to look for "\r\n" only after the "@QC+" that was found). */
bool RingBuf_FindPos(RingBuffTypeDef *ptrBuff, char* pcSeq, uint16_t *pu16Pos, uint16_t u16StartPos)
{
	uint8_t *pu8BuffStr;
	uint16_t u16SeqLen;
	if(!ptrBuff)
		return 0;

	if(ptrBuff->u8ptrBuff == NULL)
		return 0;

	u16SeqLen = strlen(pcSeq);
	if((u16SeqLen == 0) || (u16SeqLen > 1024))
		return 0;

	// Check if the length of the character seuqence is smaller than the legnth of the data
	if(RingBuf_GetDataLenPos(ptrBuff,u16StartPos) < u16SeqLen)
	{
		return 0;
	}

	pu8BuffStr = (uint8_t*)RTOSmalloc(sizeof(uint8_t) * u16SeqLen);
	if(pu8BuffStr == NULL)
	{
		return 0;
	}

	*pu16Pos = u16StartPos;

	while(((*pu16Pos) != ((ptrBuff->u16Size + ptrBuff->u16Head - u16SeqLen + 1) % ptrBuff ->u16Size)) &&((*pu16Pos) != (ptrBuff->u16Head)))
	{
		RingBuf_Peek(ptrBuff, (*pu16Pos), pu8BuffStr, u16SeqLen);
		if(strncmp((char*) pu8BuffStr, pcSeq, u16SeqLen) == 0)
		{
			free(pu8BuffStr);
			pu8BuffStr = NULL;
			return 1;
		}

		(*pu16Pos) = ((*pu16Pos) + 1) % ptrBuff->u16Size;
	}
	free(pu8BuffStr);
	pu8BuffStr = NULL;
	return 0;
}
