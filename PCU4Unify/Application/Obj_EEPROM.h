
/*==========================================================*/
/* Title		:	EEPROM.h									*/
/* Description	: 	Class_ChargerMamager definition					*/
/* Date			:	NOV.2010
// Author		: 								*/
/*==========================================================*/
#ifndef DC_EEPROM_H
#define DC_EEPROM_H

#include "Publics.h"

#define	EEP_BIOCKS		((int16_t)16)
#define	BIOCK_SIZE		((int16_t)64)


#define	SETDATA			0x55			//校准数据是否初始化数据

#define	   FirstPowerAddr		0x0200			//EEPROM第一次上电标志
//#define	   ParamAddrBase		0x0400			//EEPROM中校正参数的首地址(1k~2k)

enum MEMCMP_RESULT 		//返回比较结果
{
	EQUAL 		   = 0, 
	UN_EQUAL	   = -1,
	OTHERS         = 1
};


class Class_EEPROM
{
	private:
		int16_t m_i16_SaveDly8msCnt;
		//_DataBlockDef	m_aBlock[EEP_BIOCKS];
		//int16_t	m_u16_TotalDataMemory[EEP_BIOCKS * BIOCK_SIZE];


	public:
		struct	Seq_Queue*	pSaveQueue;

		uint16_t	m_u16_Debug[4];


	private:


	public:
		void	WriteEpoTime(void);
		void	Dat_Init(void);	//初始化(void);
		
		void	App_ReadNumBytes(uint16_t u16_Addr,       uint8_t* pData, uint16_t NumBytes);	//从EEPROM读多个字节
		void	App_WriteNumBytes(uint16_t u16_Addr, uint8_t* pData, uint16_t NumByteToWrite);
		void    App_SaveManage(void);	//EEPROM处理
		void 	App_WriteWord(uint16_t u16_Addr, uint16_t u16_Data);	
		uint16_t App_ReadSingleByte(uint16_t u16_Addr);	//从EEPROM读单个字节
		uint16_t App_ReadWord(uint16_t u16_Addr);		//从EEPROM读2个字节
		uint8_t* App_ReadBuff(uint16_t u16_Addr, uint8_t* pData, uint16_t NumBytes);		//目前未用
		uint8_t*	EEP_BuffCompare();		//EEPROM备份配置管理存储内容比较
		//void   Dat_FreeBlock(_DataBlockDef* block);
	//	_DataBlockDef* Dat_GetFreeBlock();

};


#endif
//============================================================
// End of file.
//============================================================



