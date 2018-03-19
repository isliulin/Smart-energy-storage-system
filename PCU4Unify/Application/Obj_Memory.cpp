
/*==========================================================*/
/* Title		:	Battery.cpp								*/
/* Description	: 	Class_ChargeManage realization 			*/
/* Date			:											*/
/*==========================================================*/

#include "Publics.h"


/************************************************************************************
函数名称：	Dat_Init()
功能描述：	
************************************************************************************/
void	Class_Memory::Dat_Init(void)
{
	int16_t i;
	
	//---------------存储器分块64字节---------------------
	for (i = 0; i < BIOCKS_64B; i++)
	{
		m_aBlock_64B[i].pBlockMem = &m_u8_aDataMemory_64B[i*BIOCK_64B_SIZE];
		m_aBlock_64B[i].sem = 0;
	}

	//---------------存储器分块128字节---------------------
	for (i = 0; i < BIOCKS_128B; i++)
	{
		m_aBlock_128B[i].pBlockMem = &m_u8_aDataMemory_128B[i*BIOCK_128B_SIZE];
		m_aBlock_128B[i].sem = 0;
	}

	//---------------存储器分块1408字节---------------------
	for (i = 0; i < BIOCKS_1408B; i++)
	{
		m_aBlock_1408B[i].pBlockMem = &m_u8_aDataMemory_1408B[i*BIOCK_1408B_SIZE];
		m_aBlock_1408B[i].sem = 0;
	}
}

//void Class_Memory::Dat_MemCreate (void *addr, uint32_t nblks, int32_t blksize, int8_t *err)
//{
	
//}


/************************************************************************************
函数名称：	Dat_GetMemBlock()
功能描述：	数据存储在已经定义的内存块中，
分三种内存块:64bytes,128bytes,1408bytes。根据给定的大小分配对应的block
************************************************************************************/
void Class_Memory::Dat_GetMemBlock(_DataBlockDef **pMem,uint16_t size,int16_t *err)
{
  int16_t i;
  _DataBlockDef* pmb = 0;
  *err = E_MEM_NULL;

	if(size <= BIOCK_64B_SIZE)
	{
		for (i = 0; i < BIOCKS_64B; i++)
		{
			if (m_aBlock_64B[i].sem == 0)
			{
				m_aBlock_64B[i].sem = 1;
				pmb = &(m_aBlock_64B[i]);
				*err = E_NO_ERR;
				break;
			}
		}
	}
	else if(size <= BIOCK_128B_SIZE)
	{
		for (i = 0; i < BIOCKS_128B; i++)
		{
			if (m_aBlock_128B[i].sem == 0)
			{
				m_aBlock_128B[i].sem = 1;
				pmb = &(m_aBlock_128B[i]);
				*err = E_NO_ERR;
				break;
			}
		}
	}
	else if(size <= BIOCK_1408B_SIZE)
	{
		for (i = 0; i < BIOCKS_1408B; i++)
		{
			if (m_aBlock_1408B[i].sem == 0)
			{
				m_aBlock_1408B[i].sem = 1;
				pmb = &(m_aBlock_1408B[i]);
				*err = E_NO_ERR;
				break;
			}
		}
	}
	else
	{
		*err = E_MEM_NULL;
	}
	
	*pMem = pmb;
}


/************************************************************************************
函数名称：	Dat_PutMemBlock()
功能描述：	
************************************************************************************/
void Class_Memory::Dat_PutMemBlock(_DataBlockDef* block)
{
	block->sem = 0;
}
/*===========================================================================*
 *  函数名称：Class_Memory::Dat_CleanMemBlock(void)
 *  功能描述：清除内存块标志位
 *  输入参数：none
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/

void Class_Memory::Dat_CleanMemBlock(void)
{
	int16_t i;
	for (i = 0; i < BIOCKS_64B; i++)
	{
		m_aBlock_64B[i].pBlockMem = &m_u8_aDataMemory_64B[i*BIOCK_64B_SIZE];
		m_aBlock_64B[i].sem = 0;
	}

	//---------------存储器分块128字节---------------------
	for (i = 0; i < BIOCKS_128B; i++)
	{
		m_aBlock_128B[i].pBlockMem = &m_u8_aDataMemory_128B[i*BIOCK_128B_SIZE];
		m_aBlock_128B[i].sem = 0;
	}

	//---------------存储器分块1408字节---------------------
	for (i = 0; i < BIOCKS_1408B; i++)
	{
		m_aBlock_1408B[i].pBlockMem = &m_u8_aDataMemory_1408B[i*BIOCK_1408B_SIZE];
		m_aBlock_1408B[i].sem = 0;
	}

}


//==========================================================================
// End =of file.
//===========================================================================


