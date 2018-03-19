/*==========================================================*/
/*Copyright (C), 2015-2017, XIAN TGOOD Tech. Co., Ltd.		*/
/* Title		:	Obj_PcsConfigManage.cpp					*/
/* Description	:	PCS ConfigManage 						*/
/* Author		:	Hexianguo								*/
/* Date			:	2017.12.14								*/
/*==========================================================*/

#include "Publics.h"



/***************************************************************************
函数名:Class_PCSConfigManage::PCU_Version_DatInit
功能描述:PCU版本信息初始化
输入参数：无
返回值：无
使用说明：
****************************************************************************/
void	Class_PCSConfigManage::PCU_Version_DatInit(void)
{
	m_VersionManage.HardVersion = 0x02;
	m_VersionManage.SoftVersion[0] = 0x01;
	m_VersionManage.SoftVersion[1] = 0x00;
	m_VersionManage.SoftVersion[2] = 0x01;
}

/***************************************************************************
函数名:Class_PCSConfigManage::ConfigManageDat_Init
功能描述:PCS柜配置管理初始化，本次测试使用
输入参数：无
返回值：无
使用说明：
****************************************************************************/

void	Class_PCSConfigManage::ConfigManageDat_Init(void)
{
	m_st_ConfigManage.PCUAddr = 0x64;
	m_st_ConfigManage.Frequency = 0x32;			//50Hz
	m_st_ConfigManage.CabinetType = 0x01;		//1表示单柜模式
	m_st_ConfigManage.AltitudeRange = 0x01;		//4000m以下

	m_st_ConfigManage.ACMeter = 0x01;
	m_st_ConfigManage.DCMeter = 0x00;		//1表示有，0表示没有
	m_st_ConfigManage.ACMeterBaudRate = 0x05;	//波特率0-300、1-600、2-1200、3-2400、4-4800、5-9600、6-19200、7-38400、8-43000、9-56000、10-57600、11-115200
	m_st_ConfigManage.DCMeterBaudRate = 0x05;	//波特率9600
	m_st_ConfigManage.BaudRateUart1   = 0x05;
	m_st_ConfigManage.BaudRateUart2   = 0x05;

	
	m_st_ConfigManage.ACModuleNum  = 0x04;
	m_st_ConfigManage.DCModuleNum  = 0x04;
	m_st_ConfigManage.ModeGroupNum = 0x01;
	m_st_ConfigManage.ModuleLevel  = 0x02;		//模块级数：2级

	m_st_ConfigManage.ACModeAddrAttribute[0] = 0x1114;	//AC模块地址：20~39
	m_st_ConfigManage.ACModeAddrAttribute[1] = 0x0015;
	m_st_ConfigManage.ACModeAddrAttribute[2] = 0x0016;
	m_st_ConfigManage.ACModeAddrAttribute[3] = 0x0017;
	m_st_ConfigManage.ACModeAddrAttribute[4] = 0x0018;
	m_st_ConfigManage.ACModeAddrAttribute[5] = 0x0019;
	m_st_ConfigManage.ACModeAddrAttribute[6] = 0x001A;
    m_st_ConfigManage.ACModeAddrAttribute[7] = 0x001B;

	m_st_ConfigManage.DCModeAddrAttribute[0] = 0x1128;	//DC模块地址：40~59
	m_st_ConfigManage.DCModeAddrAttribute[1] = 0x0029;
	m_st_ConfigManage.DCModeAddrAttribute[2] = 0x002A;
	m_st_ConfigManage.DCModeAddrAttribute[3] = 0x002B;
	m_st_ConfigManage.DCModeAddrAttribute[4] = 0x002C;
	m_st_ConfigManage.DCModeAddrAttribute[5] = 0x002D;
	m_st_ConfigManage.DCModeAddrAttribute[6] = 0x002E;
    m_st_ConfigManage.DCModeAddrAttribute[7] = 0x002F;
	
	m_st_ConfigManage.BMSAddrAttribute[0] = 0x1001;


	m_st_ConfigManage.VoltOut = 0x017C;			//380v
	m_st_ConfigManage.ACMeterComPara = 0x00C2;		//194 (11000010)
	m_st_ConfigManage.DCMeterComPara = 0x00C3;		//

	
	m_st_ConfigManage.SoftVersion[0] = 0x01;
	m_st_ConfigManage.SoftVersion[1] = 0x00;
	m_st_ConfigManage.SoftVersion[2] = 0x01;
	m_st_ConfigManage.HardVersion	 = 0x02;

}


/***************************************************************************
函数名:Class_PCSConfigManage::ConfigManageDat_Init
功能描述:PCS柜配置管理出厂默认设置
输入参数：无
返回值：无
使用说明：
****************************************************************************/

void	Class_PCSConfigManage::DefaultConfigDat_Init(void)
{
	m_st_GetConfigPara.PCUAddr = 0x64;
	m_st_GetConfigPara.Frequency = 0x32;			//50Hz
	m_st_GetConfigPara.CabinetType = 0x01;			//1表示单柜模式
	m_st_GetConfigPara.AltitudeRange = 0x01;		//4000m以下

	
	m_st_GetConfigPara.ACMeter = 0x01;
	m_st_GetConfigPara.DCMeter = 0x00;		//1表示有，0表示没有
	m_st_GetConfigPara.ACMeterBaudRate = 0x05;	//波特率0-300、1-600、2-1200、3-2400、4-4800、5-9600、6-19200、7-38400、8-43000、9-56000、10-57600、11-115200
	m_st_GetConfigPara.DCMeterBaudRate = 0x05;	//波特率9600
	m_st_GetConfigPara.BaudRateUart1   = 0x05;
	m_st_GetConfigPara.BaudRateUart2   = 0x05;
	
	m_st_GetConfigPara.ACModuleNum = 0x04;
	m_st_GetConfigPara.DCModuleNum = 0x04;
	m_st_GetConfigPara.ModeGroupNum = 0x01;
	m_st_GetConfigPara.ModuleLevel = 0x02;		//模块级数：2级

	m_st_GetConfigPara.ACMajorNum = 1;
	m_st_GetConfigPara.DCMajorNum = 1;
	
	m_st_GetConfigPara.ACModeAddr[0] = 0x14;
	m_st_GetConfigPara.ACModeAddr[1] = 0x15;
	m_st_GetConfigPara.ACModeAddr[2] = 0x16;
	m_st_GetConfigPara.ACModeAddr[3] = 0x17;
	m_st_GetConfigPara.ACModeAddr[4] = 0x18;
	m_st_GetConfigPara.ACModeAddr[5] = 0x19;
	m_st_GetConfigPara.ACModeAddr[6] = 0x1A;
	m_st_GetConfigPara.ACModeAddr[7] = 0x1B;

	m_st_GetConfigPara.ACMajorAddr[0] = 0x14;

	m_st_GetConfigPara.DCModeAddr[0] = 0x28;
	m_st_GetConfigPara.DCModeAddr[1] = 0x29;
	m_st_GetConfigPara.DCModeAddr[2] = 0x2A;
	m_st_GetConfigPara.DCModeAddr[3] = 0x2B;
	m_st_GetConfigPara.DCModeAddr[4] = 0x2C;
	m_st_GetConfigPara.DCModeAddr[5] = 0x2D;
	m_st_GetConfigPara.DCModeAddr[6] = 0x2E;
	m_st_GetConfigPara.DCModeAddr[7] = 0x2F;

	m_st_GetConfigPara.DCMajorAddr[0] = 0x28;


	m_st_GetConfigPara.BMSAddr[0] = 0x01;
//	m_st_GetConfigPara.BMSAddr[1] = 0x02;
//	m_st_GetConfigPara.BMSAddr[2] = 0x03;
//	m_st_GetConfigPara.BMSAddr[3] = 0x04;

	m_st_GetConfigPara.VoltOut = 0x017C;			//380v
	m_st_GetConfigPara.ACMeterComPara = 0x00C2; 	//194 (11000010)
	m_st_GetConfigPara.DCMeterComPara = 0x00C3; 	//


	m_st_GetConfigPara.SoftVersion[0] = 0x01;
	m_st_GetConfigPara.SoftVersion[1] = 0x00;
	m_st_GetConfigPara.SoftVersion[2] = 0x01;
	m_st_GetConfigPara.HardVersion = 0x02;


}


/***************************************************************************
函数名:Class_PCSConfigManage::WriteConfigInfo
功能描述:解析集控下发PCS柜配置管理信息并写入EEPROM
输入参数：_CanMstMonAppMsgDef *pApdu
返回值：无
使用说明：高字节在前data[0] 低字节在后data[1] 
****************************************************************************/
void Class_PCSConfigManage::RecvMstMonMsgDispatch(_CanMstMonAppMsgDef *pApdu)
{
	uint16_t ScuCmd;
//	int16_t newMsg = E_NON_MSG_XMIT;
//    uint8_t data[8];

//	_CanMstMonAppMsgDef tempData;
//	tempData.pDataPtr = data;
//	tempData = *pApdu;
				
				ScuCmd =  ((uint16_t)pApdu->pDataPtr[0] << 8) | pApdu->pDataPtr[1];
				switch(ScuCmd)
				{
					case PCS_ConfigPara1:

						m_st_ConfigManage.AltitudeRange	  = pApdu->pDataPtr[2];	//海拔范围 01：<4000米 02：>4000
						m_st_ConfigManage.VoltOut		  = ((uint16_t)pApdu->pDataPtr[3] << 8) | pApdu->pDataPtr[4];	//输出电压
						m_st_ConfigManage.Frequency		  = pApdu->pDataPtr[5];
						m_st_ConfigManage.ACMeterBaudRate = pApdu->pDataPtr[6];
						m_st_ConfigManage.DCMeterBaudRate = pApdu->pDataPtr[7];
						break;
						
					case PCS_ConfigPara2:
						m_st_ConfigManage.ACMeter		 = pApdu->pDataPtr[2];
						m_st_ConfigManage.DCMeter		 = pApdu->pDataPtr[3];
						m_st_ConfigManage.ACMeterComPara = ((uint16_t)pApdu->pDataPtr[4] << 8) | pApdu->pDataPtr[5]; //AC电表通讯参数
						m_st_ConfigManage.DCMeterComPara = ((uint16_t)pApdu->pDataPtr[6] << 8) | pApdu->pDataPtr[7]; //DC电表通讯参数
						break;
						 
					case PCS_ConfigPara3:
						m_st_ConfigManage.ModuleLevel	 = pApdu->pDataPtr[2]; //模块级数
						m_st_ConfigManage.CabinetType	 = pApdu->pDataPtr[3]; //工作模式：单并柜						
						m_st_ConfigManage.PCUAddr	 	 = pApdu->pDataPtr[4]; //pcu地址
						m_st_ConfigManage.ACModuleNum	 = pApdu->pDataPtr[5]; //AC模块个数
						m_st_ConfigManage.DCModuleNum	 = pApdu->pDataPtr[6]; //DC模块个数
						m_st_ConfigManage.ModeGroupNum	 = pApdu->pDataPtr[7]; //模块组数
						break;
											
					case PCS_ConfigPara4:						
						m_st_ConfigManage.ACModeAddrAttribute[0] = ((uint16_t)pApdu->pDataPtr[2] << 8) | pApdu->pDataPtr[3];
						m_st_ConfigManage.ACModeAddrAttribute[1] = ((uint16_t)pApdu->pDataPtr[4] << 8) | pApdu->pDataPtr[5];
						m_st_ConfigManage.ACModeAddrAttribute[2] = ((uint16_t)pApdu->pDataPtr[6] << 8) | pApdu->pDataPtr[7];
						break;

					case PCS_ConfigPara5:						
						m_st_ConfigManage.ACModeAddrAttribute[3] = ((uint16_t)pApdu->pDataPtr[2] << 8) | pApdu->pDataPtr[3];
						m_st_ConfigManage.ACModeAddrAttribute[4] = ((uint16_t)pApdu->pDataPtr[4] << 8) | pApdu->pDataPtr[5];
						m_st_ConfigManage.ACModeAddrAttribute[5] = ((uint16_t)pApdu->pDataPtr[6] << 8) | pApdu->pDataPtr[7];
						break;

					case PCS_ConfigPara6:						
						m_st_ConfigManage.ACModeAddrAttribute[6] = ((uint16_t)pApdu->pDataPtr[2] << 8) | pApdu->pDataPtr[3];
						m_st_ConfigManage.ACModeAddrAttribute[7] = ((uint16_t)pApdu->pDataPtr[4] << 8) | pApdu->pDataPtr[5];
						break;

					case PCS_ConfigPara7:						
						m_st_ConfigManage.DCModeAddrAttribute[0] = ((uint16_t)pApdu->pDataPtr[2] << 8) | pApdu->pDataPtr[3];
						m_st_ConfigManage.DCModeAddrAttribute[1] = ((uint16_t)pApdu->pDataPtr[4] << 8) | pApdu->pDataPtr[5];
						m_st_ConfigManage.DCModeAddrAttribute[2] = ((uint16_t)pApdu->pDataPtr[6] << 8) | pApdu->pDataPtr[7];
						break;

					case PCS_ConfigPara8:						
						m_st_ConfigManage.DCModeAddrAttribute[3] = ((uint16_t)pApdu->pDataPtr[2] << 8) | pApdu->pDataPtr[3];
						m_st_ConfigManage.DCModeAddrAttribute[4] = ((uint16_t)pApdu->pDataPtr[4] << 8) | pApdu->pDataPtr[5];
						m_st_ConfigManage.DCModeAddrAttribute[5] = ((uint16_t)pApdu->pDataPtr[6] << 8) | pApdu->pDataPtr[7];
						break;

					case PCS_ConfigPara9:						
						m_st_ConfigManage.DCModeAddrAttribute[6] = ((uint16_t)pApdu->pDataPtr[2] << 8) | pApdu->pDataPtr[3];
						m_st_ConfigManage.DCModeAddrAttribute[7] = ((uint16_t)pApdu->pDataPtr[4] << 8) | pApdu->pDataPtr[5];
						break;

					case PCS_ConfigPara10:						
						m_st_ConfigManage.BMSAddrAttribute[0]	 = ((uint16_t)pApdu->pDataPtr[2] << 8) | pApdu->pDataPtr[3];
						m_st_ConfigManage.BMSAddrAttribute[1]	 = ((uint16_t)pApdu->pDataPtr[4] << 8) | pApdu->pDataPtr[5];
						m_st_ConfigManage.BMSAddrAttribute[2]	 = ((uint16_t)pApdu->pDataPtr[6] << 8) | pApdu->pDataPtr[7];
						break;

					case PCS_ConfigPara11:						
						m_st_ConfigManage.BMSAddrAttribute[3]	 = ((uint16_t)pApdu->pDataPtr[2] << 8) | pApdu->pDataPtr[3];
						
						break;
					
					default:
					
						break;
				
				}

		
		objEEPROM.App_WriteNumBytes(PCU_ADDR, (uint8_t*)&m_st_ConfigManage, sizeof(_ConfigManageDef));
}


/***************************************************************************
函数名:Class_PCSConfigManage::WriteConfigInfo
功能描述:写PCS柜配置管理信息到存储队列
输入参数：无
返回值：无
使用说明：
****************************************************************************/

void	Class_PCSConfigManage::WriteConfigInfo(void)
{

	objEEPROM.App_WriteNumBytes(PCU_ADDR, (uint8_t*)&m_st_ConfigManage, sizeof(_ConfigManageDef));//配置管理信息
	objEEPROM.App_WriteNumBytes(PCU_ADDR1, (uint8_t*)&m_st_ConfigManage, sizeof(_ConfigManageDef));//备份1
	objEEPROM.App_WriteNumBytes(PCU_ADDR2, (uint8_t*)&m_st_ConfigManage, sizeof(_ConfigManageDef));//备份2
	
}



/***************************************************************************
函数名:Class_PCSConfigManage::ReadConfigInfo
功能描述:从EEPROM读PCS柜配置信息
输入参数：无
返回值：无
使用说明：
****************************************************************************/

void	Class_PCSConfigManage::ReadConfigInfo(void)
{
	objEEPROM.App_ReadNumBytes(PCU_ADDR, PcsConfig1, sizeof(_ConfigManageDef));
	
	Delay_10us(500);	//延时5ms，再读下一片区域
	objEEPROM.App_ReadNumBytes(PCU_ADDR1, PcsConfig2, sizeof(_ConfigManageDef));
	
	Delay_10us(500);
	objEEPROM.App_ReadNumBytes(PCU_ADDR2, PcsConfig3, sizeof(_ConfigManageDef));

	
//	Delay_10us(50000);	//延时0.5S

}

/***************************************************************************
函数名:Class_PCSConfigManage::GetDefaultPcsInfo
功能描述:默认PCS柜配置管理参数，防止EEPROM故障，备用
输入参数：_GetConfigParameterDef *pData
返回值：无
使用说明：
****************************************************************************/
/*
void	Class_PCSConfigManage::GetDefaultPcsInfo(_GetConfigParameterDef *pData)
{	
	uint8_t j;
	pData->PCUAddr 			 = m_st_GetConfigPara.PCUAddr;
	pData->Frequency		 = m_st_GetConfigPara.Frequency;
	pData->CabinetType 		 = m_st_GetConfigPara.CabinetType;
	pData->AltitudeRange	 = m_st_GetConfigPara.AltitudeRange;

	pData->ACMeter			 = m_st_GetConfigPara.ACMeter;	
	pData->DCMeter			 = m_st_GetConfigPara.DCMeter;
	pData->ACMeterBaudRate	 = m_st_GetConfigPara.ACMeterBaudRate;
	pData->DCMeterBaudRate	 = m_st_GetConfigPara.DCMeterBaudRate;
	
	pData->ACModuleNum		 = m_st_GetConfigPara.ACModuleNum;
	pData->DCModuleNum		 = m_st_GetConfigPara.DCModuleNum;
	pData->ModeGroupNum		 = m_st_GetConfigPara.ModeGroupNum;
	pData->ModuleLevel		 = m_st_GetConfigPara.ModuleLevel;

	pData->ACMajorNum		 = m_st_GetConfigPara.ACMajorNum;
	pData->DCMajorNum		 = m_st_GetConfigPara.DCMajorNum;

	for (j = 0; j < pData->ACModuleNum; j++)
		{
		
			pData->ACModeAddr[j] = m_st_GetConfigPara.ACModeAddr[j];	//ACModeAddr数组依次存入AC模块地址
					
		}

	
	for (j = 0; j < pData->ACMajorNum; j++)
		{
		
			pData->ACMajorAddr[j] = m_st_GetConfigPara.ACMajorAddr[j];	//ACMajorAddr数组依次存入AC主模块地址
			
		}


	for (j = 0; j < pData->ModeGroupNum; j++)	//模块路数=电池路数
		{
			
			pData->BMSAddr[j] = m_st_GetConfigPara.BMSAddr[j]; //BMSAddr数组存入对应序号的BMS地址
			
		}

	pData->VoltOut		  = m_st_GetConfigPara.VoltOut ;
	pData->ACMeterComPara = m_st_GetConfigPara.ACMeterComPara ;
	pData->DCMeterComPara = m_st_GetConfigPara.DCMeterComPara ;
	
	pData->SoftVersion[0] =	m_st_GetConfigPara.SoftVersion[0];
	pData->SoftVersion[1] =	m_st_GetConfigPara.SoftVersion[1];
	pData->SoftVersion[2] = m_st_GetConfigPara.SoftVersion[2];
	pData->HardVersion	  = m_st_GetConfigPara.HardVersion	

}

*/

/***************************************************************************
函数名:Class_PCSConfigManage::GetPcsConfigInfo
功能描述:解析从EEPROM读取的PCS柜配置管理参数，供外部使用
输入参数：_GetConfigParameterDef *pData
返回值：无
使用说明：
****************************************************************************/
	
void	Class_PCSConfigManage::GetPcsConfigInfo(_GetConfigParameterDef *pData)
{
	uint8_t j, ACSequenceNum, DCSequenceNum, BMSSequenceNum;
	uint8_t* TempPtr;
	
	uint8_t ACMajorNumCnt = 0, DCMajorNumCnt = 0;	
	uint8_t ACModuleAddrStart = ACMODULE_ADDR_OFFSET; 	//AC模块地址偏移
	uint8_t DCModuleAddrStart = DCMODULE_ADDR_OFFSET;	//DC模块地址偏移
	uint8_t BMSAddrStart	  = BMS_ADDR_OFFSET;		//BMS地址偏移
	TempPtr = objEEPROM.EEP_BuffCompare();	//配置管理信息提供给外部接口时，先比较几份内容是否相等
	if (TempPtr == NULL)
	{
			
			uint8_t j;
			pData->PCUAddr			 = m_st_GetConfigPara.PCUAddr;
			pData->Frequency		 = m_st_GetConfigPara.Frequency;
			pData->CabinetType		 = m_st_GetConfigPara.CabinetType;
			pData->AltitudeRange	 = m_st_GetConfigPara.AltitudeRange;
		
			pData->ACMeter			 = m_st_GetConfigPara.ACMeter;	
			pData->DCMeter			 = m_st_GetConfigPara.DCMeter;
			pData->ACMeterBaudRate	 = m_st_GetConfigPara.ACMeterBaudRate;
			pData->DCMeterBaudRate	 = m_st_GetConfigPara.DCMeterBaudRate;
			
			pData->ACModuleNum		 = m_st_GetConfigPara.ACModuleNum;
			pData->DCModuleNum		 = m_st_GetConfigPara.DCModuleNum;
			pData->ModeGroupNum 	 = m_st_GetConfigPara.ModeGroupNum;
			pData->ModuleLevel		 = m_st_GetConfigPara.ModuleLevel;
		
			pData->ACMajorNum		 = m_st_GetConfigPara.ACMajorNum;
			pData->DCMajorNum		 = m_st_GetConfigPara.DCMajorNum;
		
			for (j = 0; j < pData->ACModuleNum; j++)
				{
				
					pData->ACModeAddr[j] = m_st_GetConfigPara.ACModeAddr[j];	//ACModeAddr数组依次存入AC模块地址
							
				}
		
			
			for (j = 0; j < pData->ACMajorNum; j++)
				{
				
					pData->ACMajorAddr[j] = m_st_GetConfigPara.ACMajorAddr[j];	//ACMajorAddr数组依次存入AC主模块地址
					
				}
		
		
			for (j = 0; j < pData->ModeGroupNum; j++)	//模块路数=电池路数
				{
					
					pData->BMSAddr[j] = m_st_GetConfigPara.BMSAddr[j]; //BMSAddr数组存入对应序号的BMS地址
					
				}
		
			pData->VoltOut		  = m_st_GetConfigPara.VoltOut ;
			pData->ACMeterComPara = m_st_GetConfigPara.ACMeterComPara ;
			pData->DCMeterComPara = m_st_GetConfigPara.DCMeterComPara ;
			
			pData->SoftVersion[0] = m_st_GetConfigPara.SoftVersion[0];
			pData->SoftVersion[1] = m_st_GetConfigPara.SoftVersion[1];
			pData->SoftVersion[2] = m_st_GetConfigPara.SoftVersion[2];
			pData->HardVersion	  = m_st_GetConfigPara.HardVersion;	
			pData->BaudRateUart1  = m_st_GetConfigPara.BaudRateUart1; 
			pData->BaudRateUart2  = m_st_GetConfigPara.BaudRateUart2; 		
	}
	else
	{
	
			pData->PCUAddr 			 = TempPtr[0];
			pData->Frequency		 = TempPtr[1];
			pData->CabinetType 		 = TempPtr[2];
			pData->AltitudeRange	 = TempPtr[3];

			pData->ACMeter			 = TempPtr[4];	//@todo 如果所需，按位解析
			pData->DCMeter			 = TempPtr[5];
			pData->ACMeterBaudRate	 = TempPtr[6];
			pData->DCMeterBaudRate	 = TempPtr[7];
			
			pData->ACModuleNum		 = TempPtr[8];
			pData->DCModuleNum		 = TempPtr[9];
			pData->ModeGroupNum		 = TempPtr[10];
			pData->ModuleLevel		 = TempPtr[11];


			for (j = 0; j < pData->ACModuleNum; j++)
				{
				
					pData->ACModeAddr[j] = TempPtr[ACModuleAddrStart];	//ACModeAddrAttribute数组依次存入AC模块地址
					
					if (((TempPtr[ACModuleAddrStart + 1] & 0xF0) >> 4) == 1)		//AC主模块
						{
							ACMajorNumCnt++;	
				//			ACSequenceNum = TempPtr[ACModuleAddrStart + 1] & 0x0F;	//取出AC主模块的组序号
							pData->ACMajorAddr[j] = TempPtr[ACModuleAddrStart];	//将AC主模块地址存入数组对应的位置中（eg:第5组存入arry[4]）
						}
							ACModuleAddrStart += 2;
				}
				pData->ACMajorNum = ACMajorNumCnt;



			for (j = 0; j < pData->DCModuleNum; j++)
				{
				
					pData->DCModeAddr[j] = TempPtr[DCModuleAddrStart];	//DCModeAddrAttribute数组依次存入DC模块地址
					
					if (((TempPtr[DCModuleAddrStart + 1] & 0xF0) >> 4) == 1)		//DC主模块
						{
							DCMajorNumCnt++;
				//			DCSequenceNum = TempPtr[DCModuleAddrStart + 1] & 0x0F;	//取出DC主模块的组序号
							pData->DCMajorAddr[j] = TempPtr[DCModuleAddrStart];	//将DC主模块地址存入数组对应的位置中（eg:第5组存入arry[4]）
						}
							DCModuleAddrStart += 2;
				}
				pData->DCMajorNum = DCMajorNumCnt;


			for (j = 0; j < pData->ModeGroupNum; j++)	//模块路数=电池路数
				{
				//		BMSSequenceNum = TempPtr[BMSAddrStart + 1];	//取出每个BMS的组序号
						pData->BMSAddr[j] = TempPtr[BMSAddrStart];	//BMSAddr数组存入对应序号的BMS地址
						
						BMSAddrStart += 2;
				}
			
			
			pData->VoltOut		  =  (uint16_t)TempPtr[ALTITUDE_ADDR_OFFSET + 1] << 8 | TempPtr[ALTITUDE_ADDR_OFFSET + 0];
			pData->ACMeterComPara =  (uint16_t)TempPtr[ALTITUDE_ADDR_OFFSET + 3] << 8 | TempPtr[ALTITUDE_ADDR_OFFSET + 2];
			pData->DCMeterComPara =  (uint16_t)TempPtr[ALTITUDE_ADDR_OFFSET + 5] << 8 | TempPtr[ALTITUDE_ADDR_OFFSET + 4];
			
			pData->SoftVersion[0] =  TempPtr[ALTITUDE_ADDR_OFFSET + 6];
			pData->SoftVersion[1] =  TempPtr[ALTITUDE_ADDR_OFFSET + 7];
			pData->SoftVersion[2] =  TempPtr[ALTITUDE_ADDR_OFFSET + 8];
			pData->HardVersion	  =  TempPtr[ALTITUDE_ADDR_OFFSET + 9];
			pData->BaudRateUart1  =  TempPtr[ALTITUDE_ADDR_OFFSET + 10];
			pData->BaudRateUart2  =  TempPtr[ALTITUDE_ADDR_OFFSET + 11];

	}
}



/***************************************************************************
函数名:Class_PCSConfigManage::PCSConfigSchedule
功能描述:PCS柜配置管理任务
输入参数：无
返回值：无
使用说明：
****************************************************************************/
void	Class_PCSConfigManage::PCSConfigSchedule(void)
{

//写eeprom之前，关闭所有中断
//	__disable_irq();


	WriteConfigInfo();	//写3份配置管理数据到队列
	
//读完eeprom之后，打开所有中断
//	__enable_irq();

	
	GetPcsConfigInfo(&objPcsConfigManage.m_st_GetConfigPara);	//for test
	
}




	

/***************************************************************************
函数名:Class_PCSConfigManage::WriteConfigInfo
功能描述:写PCS柜配置管理信息到存储队列
输入参数：无
返回值：无
使用说明：
****************************************************************************/

//void	Class_PCSConfigManage::TestPCSConfig(void)
//{

//	objEEPROM.App_WriteNumBytes(PCU_ADDR, (uint8_t*)&m_st_ConfigManage, sizeof(_ConfigManageDef));//for test
//	objEEPROM.App_WriteNumBytes(PCU_ADDR1, (uint8_t*)&m_st_ConfigManage, sizeof(_ConfigManageDef));//for test
//	objEEPROM.App_WriteNumBytes(PCU_ADDR2, (uint8_t*)&m_st_ConfigManage, sizeof(_ConfigManageDef));//for test
//}


