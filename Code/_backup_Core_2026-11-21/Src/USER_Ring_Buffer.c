/*
 * Ring_Buffer.c
 *
 *  Created on: Mar 1, 2025
 *      Author: PC
 */
#include <USER_Ring_Buffer.h>

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
