
/*==========================================================*/
/* Title		:	Battery.cpp								*/
/* Description	: 	Class_ChargeManage realization 			*/
/* Date			:											*/
/*==========================================================*/

//#include "Obj_EEPROM.h"
#include "Publics.h"


_QueueDataDef  g_st_aSaveBuff[256];//存储数据缓冲区 原来30,现在：62*3 = 186
struct	Seq_Queue SaveQueue(g_st_aSaveBuff,sizeof(g_st_aSaveBuff)/sizeof(*g_st_aSaveBuff));

/***************************************************************************
函数名:Class_EEPROM::WriteEpoTime
功能描述:写EPO错误时间
输入参数：无
返回值：无
使用说明：
****************************************************************************/

void	Class_EEPROM::WriteEpoTime(void)
{
	//ReadRTCTime();//读取当前系统时间
	//objEEPROM.App_WriteNumBytes(EPOERRORADDR, (uint8_t*)(&Calendar),sizeof(_CALENDAR));//EPO错误写入EEROM
	BSP_I2C_EepromPageWrite((uint8_t*)(&objSystemRTC.s_SysCalendar), EPOERRORADDR,sizeof(_CALENDAR),0);


}




/***************************************************************************
函数名:Class_EEPROM::Dat_Init
功能描述:队列初始化
输入参数：无
返回值：无
使用说明：
****************************************************************************/
void	Class_EEPROM::Dat_Init(void)
{
//	int16_t i;
	
	//对队列进行初始化
	pSaveQueue = &SaveQueue;//指向要存储数据的指针
	pSaveQueue->u16size = sizeof(g_st_aSaveBuff)/sizeof(*g_st_aSaveBuff);//队列最大数目初始化
	pSaveQueue->head = 0;//队头
	pSaveQueue->end = 0;//队尾，起始时队头和队尾都是0
	pSaveQueue->fence = 0;//队列隔离
 	pSaveQueue->pQueueArray = g_st_aSaveBuff;//队列指向的数组初始化
 	
	//---------------存储器分块---------------------
	/*
	for (i = 0; i < EEP_BIOCKS; i++)
	{
		m_aBlock[i].pBlockMem = &m_u16_TotalDataMemory[i*BIOCK_SIZE];
		m_aBlock[i].sem = 0;
	}
	*/
	
	/*//存储保存数据
	_QueueDataDef	data;
	_DataBlockDef	*pMemBlock = Dat_GetFreeBlock();
	
	pMemBlock->pBlockMem[0] = 0x1111;
	pMemBlock->pBlockMem[1] = 0x2222;
	data.u16Addr = SerialNumbLAddr;
	data.u16Len = 2;//按照字节给
	data.pDataBlock = pMemBlock;
	Dat_enQueueSeq(pSaveQueue,data);
	*/
}




/************************************************************************************
函数名称：	Dat_FreeBlock()
功能描述：	
************************************************************************************/
/*
void Class_EEPROM::Dat_FreeBlock(_DataBlockDef* block)
{
	block->sem = 0;
	block->pBlockMem = 0;
}
*/
/************************************************************************************
函数名称：	MemBlockFree()
功能描述：	数据存储在已经定义的内存块中，每个内存块最大可以存储64个字(128bytes)
************************************************************************************/
/*
_DataBlockDef* Class_EEPROM::Dat_GetFreeBlock()
{
  int16_t i;
  _DataBlockDef* pmb = 0;
	
	for (i = 0; i < EEP_BIOCKS; i++)
	{
		if (m_aBlock[i].sem == 0)
		{
			m_aBlock[i].sem = 1;
			pmb = &(m_aBlock[i]);
			break;
		}
	}
	return pmb;
}
*/
/***************************************************************************
函数名:Class_EEPROM::App_WriteWord
功能描述:以u16_Addr为首地址，每次写入2字节数据到队列
输入参数：uint16_t u16_Addr,uint16_t u16_Data
返回值：无
使用说明：
****************************************************************************/

void	Class_EEPROM::App_WriteWord(uint16_t u16_Addr,uint16_t u16_Data)	
{
	_QueueDataDef	data;
	//_DataBlockDef	*pMemBlock = Dat_GetFreeBlock();
	//m_u16_Debug[0] = u16_Data;

	//pMemBlock->pBlockMem[0] = u16_Data;
	data.u16Addr = u16_Addr;
	data.u16Len = 1;//按照字节给
	data.i16data = u16_Data;
	//data.pDataBlock = pMemBlock;
	Dat_enQueueSeq(objEEPROM.pSaveQueue,data);	
}


/***************************************************************************
函数名:Class_EEPROM::App_WriteBuff
功能描述:以u16_Addr为首地址，每次写入n字节数据到队列
输入参数：uint16_t u16_Addr,uint8_t* pData, uint16_t NumByteToWrite
返回值：无
使用说明：
****************************************************************************/

void	Class_EEPROM::App_WriteNumBytes(uint16_t u16_Addr,uint8_t* pData, uint16_t NumByteToWrite)	
{
	_QueueDataDef	data;
	uint8_t i;
	data.u16Len = 1;//按照字节给
	for (i = 0; i < NumByteToWrite; i++)
	{
		data.u16Addr = u16_Addr + i;	//地址偏移2个字节
		data.i16data = pData[i];
		Dat_enQueueSeq(objEEPROM.pSaveQueue,data);
	}
		
}

/***************************************************************************
函数名:Class_EEPROM::App_ReadWord
功能描述:以u16_Addr为首地址,每次读1个字节
输入参数：uint16_t u16_Addr
返回值：读到的内容
使用说明：
****************************************************************************/
uint16_t Class_EEPROM::App_ReadSingleByte(uint16_t u16_Addr)
{
	uint16_t data;
	__disable_irq();
	BSP_I2C_EepromBufferRead((uint8_t*)&data, u16_Addr, 1,0);	//按单字节读
    __enable_irq();    
	return data;
}


/***************************************************************************
函数名:Class_EEPROM::App_ReadWord
功能描述:以u16_Addr为首地址,每次读2个字节
输入参数：uint16_t u16_Addr
返回值：读到的内容
使用说明：
****************************************************************************/
uint16_t Class_EEPROM::App_ReadWord(uint16_t u16_Addr)
{
	uint16_t data;	
	__disable_irq();
	BSP_I2C_EepromBufferRead((uint8_t*)&data, u16_Addr, 2,0);	//每次读2个字节,原来为2
  	__enable_irq();
	return data;
}


/***************************************************************************
函数名:Class_EEPROM::App_ReadBuff
功能描述:以u16_Addr为首地址,每次读n个字节
输入参数：uint16_t u16_Addr
返回值：读到的内容
使用说明：
****************************************************************************/
uint8_t* 	Class_EEPROM::App_ReadBuff(uint16_t u16_Addr, uint8_t* pData, uint16_t NumBytes)
{
//	uint8_t temp[20];
	

	BSP_I2C_EepromBufferRead(pData, u16_Addr, NumBytes,0);
	
	return pData;			
}


/***************************************************************************
函数名:Class_EEPROM::App_SaveManage
功能描述:对队列中的数据按照FIFO进行逐个存储,直到队列中的数据为空	
输入参数：无
返回值：无
使用说明：存储之间间隔8ms，以保证EEPROM正常存储
****************************************************************************/
void	Class_EEPROM::App_SaveManage(void)
{
	_QueueDataDef  tempData;
	
	if (g_st_TimerFlag.b1ms == 1)
	{
		++m_i16_SaveDly8msCnt;
	}
	if (m_i16_SaveDly8msCnt >= 8)
	{

		m_i16_SaveDly8msCnt = 0;

//		Delay_10us(800);

		if (EMPTY_QUEUE != Dat_deQueueSeq(pSaveQueue, &tempData))
		{
			//如果队列不为空时，继续存储
			//BSP_I2C_EepromPageWrite((uint8_t*)(tempData.pDataBlock->pBlockMem), tempData.u16Addr,2*tempData.u16Len,0);
			__disable_irq();
			BSP_I2C_EepromPageWrite((uint8_t*)(&(tempData.i16data)), tempData.u16Addr,2*tempData.u16Len,0);
			__enable_irq();
			//Dat_FreeBlock(tempData.pDataBlock);
		}
	}
}


/***************************************************************************
函数名:Class_EEPROM::App_ReadSysVersion
功能描述:以字节为单位，从EEPROM读n个字节
输入参数：uint16_t u16_Addr, uint8_t* pData, uint16_t NumBytes
返回值：无
使用说明：
****************************************************************************/
void	Class_EEPROM::App_ReadNumBytes(uint16_t u16_Addr, uint8_t* pData, uint16_t NumBytes)
{			
	uint8_t i;
	for (i = 0; i < NumBytes; i++)
		{
			*pData = App_ReadWord(u16_Addr + i);	// 每次偏移2字节
			pData++;
		}
}


/***************************************************************************
函数名:Class_EEPROM::EEP_BuffCompare
功能描述:以字节为单位，比较EEPROM两块存储区内容是否相同
输入参数：
返回值：比较结果
使用说明：
****************************************************************************/
uint8_t*	Class_EEPROM::EEP_BuffCompare()
{
	uint8_t i;
	int8_t ret1, ret2, ret3;
	uint8_t *ptr1, *ptr2, *ptr3, *ptr4;
//	ptr1 = objPcsConfigManage.TestDataRead.TestArry;
//	ptr2 = objPcsConfigManage.TestDataRead.TestBackArry1;
//	ptr3 = objPcsConfigManage.TestDataRead.TestBackArry2;
	ptr1 = objPcsConfigManage.PcsConfig1;
	ptr2 = objPcsConfigManage.PcsConfig2;
	ptr3 = objPcsConfigManage.PcsConfig3;
	ptr4 = NULL;
//	for (i = 0; i < sizeof(objPcsConfigManage.PcsConfig1)/sizeof(objPcsConfigManage.PcsConfig1[0]); i++)
		for (i = 0; i < 64; i++)
		{
			if((ptr1[i] == 0xFF) || (ptr2[i] == 0xFF) || (ptr3[i] == 0xFF))
			{
				//读出来有0xff，说明有的值未写成功，@todo重写还是直接赋默认值
			//	memset(PcsConfig1, 0, sizeof(PcsConfig1)/sizeof(PcsConfig1[0]);
			//	memset(PcsConfig2, 0, sizeof(PcsConfig2)/sizeof(PcsConfig2[0]);
			//	memset(PcsConfig3, 0, sizeof(PcsConfig3)/sizeof(PcsConfig3[0]);
				
				return ptr4;
			}
		}
	
	
	ret1 = memcmp(ptr1, ptr2, sizeof(_ConfigManageDef));
	
	if (ret1)	//如果ret1不为0，继续比较另外两块
	{
		ret2 = memcmp(ptr1, ptr3, sizeof(_ConfigManageDef));
		if (ret2)	//如果ret2不为0，继续比较另外两块
		{
			ret3 = memcmp(ptr2, ptr3, sizeof(_ConfigManageDef));
			if (ret3)
			{
				//@todo 读出来3份都不相等，重新写
				objPcsConfigManage.WriteConfigInfo();
				//@todo 写完该干嘛，程序跳转到哪里
				
			}
			else
			{
				return ptr2;
			}
		}
		else
		{
			return ptr1;
		}
	}
	else
	{
		//相等，则解析第一块存储区域
		return ptr1;
	}

//	return OTHERS;
}





//===========================================================================
// End of file.
//===========================================================================


