
/*==========================================================*/
/* Title		:	EEPROM.h									*/
/* Description	: 	Class_ChargerMamager definition					*/
/* Date			:	NOV.2010
// Author		: 								*/
/*==========================================================*/
#ifndef _MEMORY_H
#define _MEMORY_H

#include "Publics.h"

#define	BIOCKS_64B		    ((int16_t)30)
#define	BIOCK_64B_SIZE	    ((int16_t)64)
#define	BIOCKS_128B			((int16_t)10)
#define	BIOCK_128B_SIZE		((int16_t)128)
#define	BIOCKS_1408B	 	((int16_t)2)
#define	BIOCK_1408B_SIZE	((int16_t)1408)


enum
{
	E_MEM_NULL = 0,
	E_NO_ERR = 1,
};

typedef struct
{
	uint8_t *pBlockMem;
	int16_t	sem;//信号，标示该快是否被使用
}_DataBlockDef;

class Class_Memory
{
	private:
		int16_t m_i16_SaveDly8msCnt;
		
		_DataBlockDef	m_aBlock_64B[BIOCKS_64B];
		uint8_t	m_u8_aDataMemory_64B[BIOCKS_64B * BIOCK_64B_SIZE];
		_DataBlockDef	m_aBlock_128B[BIOCKS_128B];
		uint8_t	m_u8_aDataMemory_128B[BIOCKS_128B * BIOCK_128B_SIZE];
		_DataBlockDef	 m_aBlock_1408B[BIOCKS_1408B];
		uint8_t	m_u8_aDataMemory_1408B[BIOCKS_1408B * BIOCK_1408B_SIZE];

	public:


	private:


	public:
		void	Dat_Init(void);	//初始化(void)

		void   Dat_PutMemBlock(_DataBlockDef* block);
		void Dat_GetMemBlock(_DataBlockDef **pMem,uint16_t size,int16_t *err);
		void Dat_CleanMemBlock(void);
};


#endif
//============================================================
// End of file.
//============================================================



