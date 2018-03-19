/*==========================================================*/
/*Copyright (C), 2015-2017, XIAN TGOOD Tech. Co., Ltd.		*/
/* Title		:	Obj_StoredEnergy.cpp					*/
/* Description	: 	Module Control							*/
// Author		:	zhangpeiqi								*/
/* Date			:	2017.11.14								*/
/*==========================================================*/

#include "Publics.h"

/*===========================================================================*
 *  函数名称：StoredEnergy::Dat_Init(void)
 *  功能描述：初始化模块参数
 *  输入参数：none
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/

void Class_StoredEnergy::Dat_Init(void)
{
	//i，j分别为ACDC,DCDC地址计数、acmajor,dcmajor分别为主模块地址计数
	uint8_t i,j,acmajor,dcmajor;
	//波特率系数，指每秒可以传送的字节数，用来算查询数据的入队时间
	BaudrateFactor = USART_BAUDRATE1 / 9600;
	//级数一级情况下，只读实时数据的帧数为1帧，读设置量和实时数据的帧数为2帧
	SetDataFrameNum = 2;
	RealDataFrameNum = 1;
	//获取配置管理并赋值给结构体
	_GetConfigParameterDef pcsConfig;
	memset(&pcsConfig, 0, sizeof(_GetConfigParameterDef));
	memset(&m_st_ModuleInfo, 0, sizeof(_ModuleInfoDef));
	objPcsConfigManage.GetPcsConfigInfo(&pcsConfig);
	m_st_ModuleInfo.ModuleLevel = pcsConfig.ModuleLevel;
	m_st_ModuleInfo.ACModuleNum = pcsConfig.ACModuleNum;
	m_st_ModuleInfo.ACMajorNum = pcsConfig.ACMajorNum;
	//数据入队延迟时间为数据长度加模块处理时间乘帧数
	RealDataDelayTime = (sizeof(_ACDCReadDataDef) / BaudrateFactor) + (MODULEACKTIME * RealDataFrameNum);
	SetInsqueueDelayTime = ((sizeof(_ACDCReadDataDef) + sizeof(_ACDCSettingDataDef_1)) / BaudrateFactor) + (MODULEACKTIME * SetDataFrameNum);
	for(i = 0;i < m_st_ModuleInfo.ACModuleNum;i++)
	{
		m_st_ModuleInfo.ACModeAddr[i] = pcsConfig.ACModeAddr[i];
		//初始化时读取所有模块当前设置量
		App_TransReqData(m_st_ModuleInfo.ACModeAddr[i],DCAC_CMD_REQ,WORKMODE,sizeof(_ACDCSettingDataDef_1)/2);
	}
	for(acmajor = 0;acmajor < m_st_ModuleInfo.ACMajorNum;acmajor++)
	{
		m_st_ModuleInfo.ACMajorAddr[acmajor] = pcsConfig.ACMajorAddr[acmajor];
	}
	//模块级数为2级的情况下，获取DCDC地址，计算入队延迟时间
	if(m_st_ModuleInfo.ModuleLevel == 2)
	{
		SetDataFrameNum = 4;
		RealDataFrameNum = 2;
		RealDataDelayTime = ((sizeof(_ACDCReadDataDef) + sizeof(_DCDCDataDef_1)) / BaudrateFactor) + (MODULEACKTIME * RealDataFrameNum);
		SetInsqueueDelayTime = ((sizeof(_ACDCReadDataDef)  + sizeof(_ACDCSettingDataDef_1) + sizeof(_DCDCDataDef_1) 
					   + sizeof(_DCDCSettingDataDef)) / BaudrateFactor) + (MODULEACKTIME * SetDataFrameNum);
		m_st_ModuleInfo.DCModuleNum = pcsConfig.DCModuleNum;
		m_st_ModuleInfo.DCMajorNum = pcsConfig.DCMajorNum;
		for(j = 0;j < m_st_ModuleInfo.DCModuleNum;j++)
		{
			m_st_ModuleInfo.DCModeAddr[j] = pcsConfig.DCModeAddr[j];
			App_TransReqData(m_st_ModuleInfo.DCModeAddr[j],DCAC_CMD_REQ,WORKMODE,sizeof(_DCDCSettingDataDef)/2);
		}
		for(dcmajor = 0;dcmajor < m_st_ModuleInfo.DCMajorNum;dcmajor++)
		{
			m_st_ModuleInfo.DCMajorAddr[dcmajor] = pcsConfig.DCMajorAddr[dcmajor];
		}
	}
	ACDCSetChangeFlag = 0;
}

/*===========================================================================*
 *  函数名称：StoredEnergy::App_Manager(void)
 *  功能描述：模块功能管理
 *  输入参数：none
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::App_Manager(void)
{
	App_DataScheduling();	//模块数据读取
	//App_SetAllModulesOnOff();	//模块开关机
	//App_SetACDCPara();		//ACDC参数设置
	//App_SetDCDCPara();		//DCDC参数设置
	App_ModuleOffLineCrl();	//模块离线处理
}

/*===========================================================================*
 *  函数名称：StoredEnergy::App_DataScheduling(void)
 *  功能描述：模块数据读取
 *  输入参数：none
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
  void Class_StoredEnergy::App_DataScheduling(void)
{
	static uint16_t index1,index2 = 0;		//记读取时间（200ms/帧）
	static uint8_t acdcnumber = 0;		//读实时数据ACDC模块计数
	static uint8_t dcdcnumber = 0;		//读实时数据DCDC模块计数
	static uint8_t acdcnumber2 = 0;		//读实时数据+设置量ACDC模块计数
	static uint8_t dcdcnumber2 = 0;		//读实时数据+设置量DCDC模块计数
	//模块设置量没有改变，只读实时数据
	if(ACDCSetChangeFlag == 0)
	{
		if (g_st_TimerFlag.b10ms == 1)
		{
			index1++;
			//每帧的延迟时间为40ms
			if (index1 % ((RealDataDelayTime / 10) + 5 * RealDataFrameNum) != 0)
			{
	           return;
			}
			index1 = 0;
			if(m_st_ModuleInfo.ModuleLevel == 1)
			{
				App_TransReqData(m_st_ModuleInfo.ACModeAddr[acdcnumber],DCAC_CMD_REQ,DCAC_U_VOLT,sizeof(_ACDCReadDataDef)/2);
				acdcnumber++;
				if (acdcnumber >= m_st_ModuleInfo.ACModuleNum)
				{
					acdcnumber = 0;
				}
			}
			else
			{
				App_TransReqData(m_st_ModuleInfo.ACModeAddr[acdcnumber],DCAC_CMD_REQ,DCAC_U_VOLT,sizeof(_ACDCReadDataDef)/2);
				App_TransReqData(m_st_ModuleInfo.DCModeAddr[dcdcnumber],DCAC_CMD_REQ,DCDC_P_Voltage,sizeof(_DCDCReadDataDef)/2);
				//App_TransReqData(m_st_ModuleInfo.DCModeAddr[dcdcnumber],DCAC_CMD_REQ,DCDC_IGBTTEMP,sizeof(_DCDCDataDef_2)/2);
				acdcnumber++;
				dcdcnumber++;
				if (acdcnumber >= m_st_ModuleInfo.ACModuleNum)
				{
					acdcnumber = 0;
				}
				if (dcdcnumber >= m_st_ModuleInfo.DCModuleNum)
				{
					dcdcnumber = 0;
				}
			}
		}
	}
	//如果模块设置量改变了，读取模块设置量，设置的数据和读取的相同时将标志位置0，不再读取设置量
	else if(ACDCSetChangeFlag == 1)
	{
		if (g_st_TimerFlag.b10ms == 1)
		{
			index2++;
			if (index2 % ((SetInsqueueDelayTime / 10) + 7 * SetDataFrameNum) != 0)
			{
	           return;
			}
			index2 = 0;
			if(m_st_ModuleInfo.ModuleLevel == 1)
			{
				App_TransReqData(m_st_ModuleInfo.ACModeAddr[acdcnumber2],DCAC_CMD_REQ,DCAC_U_VOLT,sizeof(_ACDCReadDataDef)/2);
				App_TransReqData(m_st_ModuleInfo.ACModeAddr[acdcnumber2],DCAC_CMD_REQ,WORKMODE,sizeof(_ACDCSettingDataDef_1)/2);
				acdcnumber2++;
				if (acdcnumber2 >= m_st_ModuleInfo.ACModuleNum)
				{
					acdcnumber2 = 0;
				}
			}
			else
			{
				App_TransReqData(m_st_ModuleInfo.ACModeAddr[acdcnumber2],DCAC_CMD_REQ,DCAC_U_VOLT,sizeof(_ACDCReadDataDef)/2);
				App_TransReqData(m_st_ModuleInfo.ACModeAddr[acdcnumber2],DCAC_CMD_REQ,WORKMODE,sizeof(_ACDCSettingDataDef_1)/2);
				App_TransReqData(m_st_ModuleInfo.DCModeAddr[dcdcnumber2],DCAC_CMD_REQ,DCDC_P_Voltage,sizeof(_DCDCReadDataDef)/2);
				//App_TransReqData(m_st_ModuleInfo.DCModeAddr[dcdcnumber2],DCAC_CMD_REQ,DCDC_IGBTTEMP,sizeof(_DCDCDataDef_2)/2);
				App_TransReqData(m_st_ModuleInfo.DCModeAddr[dcdcnumber2],DCAC_CMD_REQ,WORKMODE,sizeof(_DCDCSettingDataDef)/2);
				acdcnumber2++;
				dcdcnumber2++;
				if (acdcnumber2 >= m_st_ModuleInfo.ACModuleNum)
				{
					acdcnumber2 = 0;
				}
				if (dcdcnumber2 >= m_st_ModuleInfo.DCModuleNum)
				{
					dcdcnumber2 = 0;
				}
			}
		}
	}
}
/*===========================================================================*
 *  函数名称：StoredEnergy::App_TransReqData(uint8_t macID,uint8_t cmd,uint16_t dataAdd,uint16_t dataLen)
 *  功能描述：发送modbus命令
 *  输入参数：macID:modbus地址		cmd:功能码 dataAdd:起始地址	dataLen:数据长度
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::App_TransReqData(uint8_t macID,uint8_t cmd,uint16_t dataAdd,uint16_t dataLen)
{
	_ModBusTxDef tempMsg;

	if (macID > DCDCMAXADDR || macID < ACDCMINADDR)
	{
		return;
	}

	tempMsg.u8MacId = macID;
	tempMsg.u8CmdType = cmd;
	tempMsg.u16DataStartAdd = dataAdd;
	tempMsg.u16DataLen = dataLen;
	objRS485App.App_SendMsg(tempMsg);
}

/*===========================================================================*
 *  函数名称：StoredEnergy::App_ModuleOffLineCrl(void)
 *  功能描述：模块离线处理
 *  输入参数：none
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::App_ModuleOffLineCrl(void)
{
  	int16_t i,j,max;
	//max = MAX(m_st_ModuleInfo.DCModuleNum, m_st_ModuleInfo.ACModuleNum);
	if (g_st_TimerFlag.b10ms == 1)
	{
		for (i = 0; i < m_st_ModuleInfo.ACModuleNum; i++)
		{
			m_st_aACDCData[i].u16OfflineCnt++;
			if (m_st_aACDCData[i].u16OfflineCnt >= 1000)
			{
				m_st_aACDCData[i].u16OfflineCnt = 1000;
				m_st_aACDCData[i].Online = NO;
			}
		}
		for (j = 0; j < m_st_ModuleInfo.DCModuleNum; j++)
		{
			m_st_bDCDCData[i].u16OfflineCnt++;
			if (m_st_bDCDCData[i].u16OfflineCnt >= 1000)
			{
				m_st_bDCDCData[i].u16OfflineCnt = 1000;
				m_st_bDCDCData[i].Online = NO;
			}
		}
	}
}

/*===========================================================================*
 *  函数名称：StoredEnergy::App_RecvACDCMsgDispatch(IN _ModBusRxDef* pApdu)
 *  功能描述：ACDC数据接收解析
 *  输入参数：ACDC数据地址
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::App_RecvACDCMsgDispatch(IN _ModBusRxDef* pApdu)
{
	int16_t i,j,index;
	uint8_t* pData;
	for(j = 0;j < m_st_ModuleInfo.ACModuleNum;j++)
	{
		if((pApdu->u8MacId) == m_st_ModuleInfo.ACModeAddr[j])
		{
			index = j;
			//objRS485App.m_u16_ACDCRefreshID = j;
			m_st_aACDCData[index].ModID = m_st_ModuleInfo.ACModeAddr[j];
			break;
		}
	}
	if (E_QUEUE_FULL != objModBusProtocol.m_ACIDRecvQue.FlagQueue())
	{
		objModBusProtocol.m_ACIDRecvQue.InsQueue(index);
	}
	switch(pApdu->u8CmdType)
	{
		case 0x03://实时数据
			if(pApdu->u8DataLen == sizeof(_ACDCReadDataDef))//实时数据
			{
				pData = (uint8_t*)(&(m_st_aACDCData[index].stMeter.U_Voltage));
				for (i = 0;i < pApdu->u8DataLen/2;i++)
				{
					*((int16_t*)&(pData[i*2])) =BIG_LITTLE_TRANVERSE16(S2U16(pApdu->u8Data[i*2]));
				}
			}
			else if(pApdu->u8DataLen == sizeof(m_st_aACDCData[index].stConfig1))//设置参数
			{
				pData = (uint8_t*)(&(m_st_aACDCData[index].stConfig1.Mode));
				for (i = 0;i < pApdu->u8DataLen/2;i++)
				{
					*((int16_t*)&(pData[i*2])) =BIG_LITTLE_TRANVERSE16(S2U16(pApdu->u8Data[i*2]));
				}
			}
			break;
		case 0x04://设置量数据
			pData = (uint8_t*)(&(m_st_aACDCData[index].stConfig1));
			for (i = 0;i < pApdu->u8DataLen/2;i++)
			{
				*((int16_t*)&(pData[i*2])) = BIG_LITTLE_TRANVERSE16(S2U16(pApdu->u8Data[i*2]));
			}
			break;
		case DCAC_CMD_SET:

			break;
		default: 
			break;
	}
	m_st_aACDCData[index].u16OfflineCnt = 0;
	m_st_aACDCData[index].Online = YES;
}

/*===========================================================================*
 *  函数名称：StoredEnergy::App_RecvDCDCMsgDispatch(IN _ModBusRxDef* pApdu)
 *  功能描述：DCDC数据接收解析
 *  输入参数：DCDC数据地址
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::App_RecvDCDCMsgDispatch(IN _ModBusRxDef* pApdu)
{
	int16_t i,j,index;
	uint8_t* pData;
	for(j = 0;j < m_st_ModuleInfo.DCModuleNum;j++)
	{
		if((pApdu->u8MacId) == m_st_ModuleInfo.DCModeAddr[j])
		{
			index = j;
			objRS485App.m_u16_DCDCRefreshID = j;
			m_st_bDCDCData[index].ModID = m_st_ModuleInfo.DCModeAddr[j];
			break;
		}
	}
	//index = pApdu->u8MacId - MAC_ID_BATT_DCDC0;
	if (E_QUEUE_FULL != objModBusProtocol.m_DCIDRecvQue.FlagQueue())
	{
		objModBusProtocol.m_DCIDRecvQue.InsQueue(index);
	}
	switch(pApdu->u8CmdType)
	{
		case 0x03://实时数据
		if(pApdu->u8DataLen == sizeof(_DCDCReadDataDef))//第1帧
		{
			pData = (uint8_t*)(&(m_st_bDCDCData[index].stMeter.P_Voltage));
			for (i = 0;i < pApdu->u8DataLen/2;i++)
			{
				*((int16_t*)&(pData[i*2])) =BIG_LITTLE_TRANVERSE16(S2U16(pApdu->u8Data[i*2]));
			}
		}
		/*if(pApdu->u8DataLen == sizeof(m_st_bDCDCData[index].stMeter2))//第2帧
		{
			pData = (uint8_t*)(&(m_st_bDCDCData[index].stMeter2.IGBT1_Temp));
			for (i = 0;i < pApdu->u8DataLen/2;i++)
			{
				*((int16_t*)&(pData[i*2])) =BIG_LITTLE_TRANVERSE16(S2U16(pApdu->u8Data[i*2]));
			}
		}*/
		if(pApdu->u8DataLen == sizeof(m_st_bDCDCData[index].stConfig))//第3帧
		{
			pData = (uint8_t*)(&(m_st_bDCDCData[index].stConfig.WorkMode));
			for (i = 0;i < pApdu->u8DataLen/2;i++)
			{
				*((int16_t*)&(pData[i*2])) =BIG_LITTLE_TRANVERSE16(S2U16(pApdu->u8Data[i*2]));
			}
		}
		break;

		case 0x04://设置量数据
			pData = (uint8_t*)(&(m_st_bDCDCData[index].stConfig));
			for (i = 0;i < pApdu->u8DataLen/2;i++)
			{
				*((int16_t*)&(pData[i*2])) = BIG_LITTLE_TRANVERSE16(S2U16(pApdu->u8Data[i*2]));
			}
			
		break;
		
		case 0x06://设置量返回数据
			pData = (uint8_t*)(&(m_st_bDCDCData[index].stConfig));
			for (i = 0;i < pApdu->u8DataLen/2;i++)
			{
				*((int16_t*)&(pData[i*2])) = BIG_LITTLE_TRANVERSE16(S2U16(pApdu->u8Data[i*2]));
			}
			
		break;


		default: 

			break;
	}

	m_st_bDCDCData[index].u16OfflineCnt = 0;
	m_st_bDCDCData[index].Online = YES;

}

/*===========================================================================*
 *  函数名称：StoredEnergy::App_SetModuleOnOff(uint8_t dcacID, uint16_t  u16DCACState)
 *  功能描述：设置模块开关机
 *  输入参数：dcacID:模块modbus地址		u16DCACState:1为开/0为关
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::App_SetModuleOnOff(uint8_t dcacID, uint16_t  u16DCACState)
{
	_ModBusTxDef tempMsg;

	if (dcacID > DCDCMAXADDR || dcacID < ACDCMINADDR)
	{
		return;
	}

	tempMsg.u8MacId = dcacID;
	tempMsg.u8CmdType = DCAC_CMD_SET;
	tempMsg.u16DataStartAdd = DCAC_CMD_ONOFF;
	
	if (u16DCACState == ACDC_ON)
	{
		tempMsg.u16DataLen = 1;
	}
	else if (u16DCACState == ACDC_OFF)
	{
		tempMsg.u16DataLen = 0;
	}

	objRS485App.App_SendMsg(tempMsg);
}

/*===========================================================================*
 *  函数名称：App_SetAllModulesOnOff(void)
 *  功能描述：设置所有模块开关机
 *  输入参数：none
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
 /*
void Class_StoredEnergy::App_SetAllModulesOnOff(void)
{	
		if(g_st_TimerFlag.b1s)
		{
			if(objCanDataPool.m_i16TurnonoffMoudle !=0 )
			{
				App_SetModuleOnOff(objCanDataPool.m_i16_TurnOnOffAddress,objCanDataPool.m_i16TurnonoffMoudle);
			}
		}	
}
*/
/*===========================================================================*
 *  函数名称：StoredEnergy::App_SetACDCParaTo485(uint8_t dcacID,uint16_t setID, uint16_t  u16para)
 *  功能描述：发送设置ACDC参数命令
 *  输入参数：dcacID:ACDCmodbus地址		setID:寄存器地址		u16para:设置量
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::App_SetACDCParaTo485(uint8_t dcacID,uint16_t setID, uint16_t  u16para)
{
	_ModBusTxDef tempMsg;

	if (dcacID > ACDCMAXADDR || dcacID < ACDCMINADDR)
	{
		return;
	}
	tempMsg.u8MacId = dcacID;
	tempMsg.u8CmdType = DCAC_CMD_WRITE;
	tempMsg.u16DataStartAdd = setID;
	tempMsg.u16DataLen = u16para;
	objRS485App.App_SendMsg(tempMsg);
}

/*===========================================================================*
 *  函数名称：StoredEnergy::App_SetACDCPara(void)
 *  功能描述：设置ACDC参数
 *  输入参数：none
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::App_SetACDCPara(void)
{
	if(objCanDataPool.m_i16_SetACDCFlag != 0)
	{
		App_SetACDCParaTo485(objCanDataPool.m_i16_ACDCID,objCanDataPool.m_i16_ACDCSetID,objCanDataPool.m_i16_ACDCSetData);
	}
}

/*===========================================================================*
 *  函数名称：StoredEnergy::App_SetDCDCParaTo485(uint8_t dcdcID,uint16_t setID, uint16_t  u16para)
 *  功能描述：发送设置DCDC参数命令
 *  输入参数：dcdcID:DCDCmodbus地址		setID:寄存器地址		u16para:设置量
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::App_SetDCDCParaTo485(uint8_t dcdcID,uint16_t setID, uint16_t  u16para)
{
	_ModBusTxDef tempMsg;

	if (dcdcID > DCDCMAXADDR || dcdcID < DCDCMINADDR)
	{
		return;
	}
	tempMsg.u8MacId = dcdcID;
	tempMsg.u8CmdType = DCAC_CMD_WRITE;
	tempMsg.u16DataStartAdd = setID;
	tempMsg.u16DataLen = u16para;
	objRS485App.App_SendMsg(tempMsg);
}

/*===========================================================================*
 *  函数名称：StoredEnergy::App_SetDCDCPara(void)
 *  功能描述：设置DCDC参数
 *  输入参数：none
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::App_SetDCDCPara(void)
{
	if(g_st_TimerFlag.b1s)
	{
		if(objCanDataPool.m_i16_SetDCDCFlag !=0 )
		{
			App_SetDCDCParaTo485(objCanDataPool.m_i16_DCDCID,objCanDataPool.m_i16_DCDCSetID,objCanDataPool.m_i16_DCDCSetData);
		}
	}
}

/*===========================================================================*
 *  函数名称：StoredEnergy::App_GetModuleState(uint16_t moduleID)
 *  功能描述：查询模块运行状态
 *  输入参数：moduleID:0~2为ACDC3~5为DCDC
 *  返回值：1模块告警/2模块故障
 *  使用说明： 
 *==========================================================================*/
Module_Run_State Class_StoredEnergy::App_GetModuleState(uint16_t moduleID)
{	
	uint16_t acdcwarn,acdcfault,Contravrun,Rectifrun,j,index;	//ACDC状态标记,index为模块地址对应的数组序号
	uint16_t dcdcwarn,dcdcfault,dcdcrun;	//DCDC状态标记
	Module_Run_State ret;
	if (moduleID >= ACDCMINADDR && moduleID <= ACDCMAXADDR )
	{
		if(m_st_ModuleInfo.ACModuleNum > 0)
		{
			for(j = 0;j < m_st_ModuleInfo.ACModuleNum;j++)
			{
				if((moduleID) == m_st_ModuleInfo.ACModeAddr[j])
				{
					index = j;
					break;
				}
			}
			acdcwarn = objStoredEnergy.m_st_aACDCData[index].stMeter.WorkState.WarmFlag;
			acdcfault = objStoredEnergy.m_st_aACDCData[index].stMeter.FaultState;
			Contravrun = objStoredEnergy.m_st_aACDCData[index].stMeter.WorkState.Contrav_Run;
			Rectifrun = objStoredEnergy.m_st_aACDCData[index].stMeter.WorkState.Rectif_Run;
			if (acdcfault != 0)
				ret = MODULE_STATE_FAULT;		//返回2
			
			if (acdcwarn != 0)
				ret = MODULE_STATE_WARN;		//返回1
			
			if ((Contravrun == 1) || (Rectifrun == 1))
				ret = MODULE_STATE_RUN;		//返回0
		}
	}
	else if (moduleID >= DCDCMINADDR && moduleID <= DCDCMAXADDR) 
	{
		if(m_st_ModuleInfo.DCModuleNum > 0)
		{
			for(j = 0;j < m_st_ModuleInfo.DCModuleNum;j++)
			{
				if((moduleID) == m_st_ModuleInfo.DCModeAddr[j])
				{
					index = j;
					break;
				}
			}
			dcdcwarn = objStoredEnergy.m_st_bDCDCData[index].stMeter.DevStatus.WarmFlag;
			dcdcfault = objStoredEnergy.m_st_bDCDCData[index].stMeter.DevStatus.FaultFlag;
			dcdcrun = objStoredEnergy.m_st_bDCDCData[index].stMeter.DevStatus.RunFlag;
			if (dcdcfault == 1)
				ret = MODULE_STATE_FAULT;
			
			if (dcdcwarn == 1)
				ret = MODULE_STATE_WARN;
			
			if (dcdcrun == 1)
				ret = MODULE_STATE_RUN;
		}
	}
	return ret;
}

/*===========================================================================*
 *  函数名称：StoredEnergy::Dat_RefreshModuleWorkState(uint16_t index)
 *  功能描述：更新模块工作状态
 *  输入参数：index:模块计数
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::Dat_RefreshModuleWorkState(uint16_t index)
{
	uint16_t DCDCindex;	 //查询DCDC数据时需改变值
	m_st_ModuleWorkState.i16_ModID=index+1;
	if (index >= 0 && index <= 2 )
	{
		m_st_ModuleWorkState.stACDCData.WorkState = objStoredEnergy.m_st_aACDCData[index].stMeter.WorkState; 	//ACDC工作状态
		m_st_ModuleWorkState.stACDCData.WarnState = objStoredEnergy.m_st_aACDCData[index].stMeter.WarnState;	//ACDC告警状态
		m_st_ModuleWorkState.stACDCData.FaultState = objStoredEnergy.m_st_aACDCData[index].stMeter.FaultState;	//ACDC故障状态
		m_st_ModuleWorkCurrrent.stACDCWorkCurrent.U_Current = objStoredEnergy.m_st_aACDCData[index].stMeter.U_Current;	//ACDCU相电流
		m_st_ModuleWorkCurrrent.stACDCWorkCurrent.V_Current = objStoredEnergy.m_st_aACDCData[index].stMeter.V_Current;	//ACDCV相电流
		m_st_ModuleWorkCurrrent.stACDCWorkCurrent.W_Current = objStoredEnergy.m_st_aACDCData[index].stMeter.W_Current;	//ACDCW相电流
	}
	else if (index >= 3 && index <= 5 )
	{
		DCDCindex = index-3;
		m_st_ModuleWorkState.stDCDCWorkState = objStoredEnergy.m_st_bDCDCData[DCDCindex].stMeter.DevStatus;		//DCDC工作状态
		m_st_ModuleWorkCurrrent.stDCDCWorkCurrent.U_Current = objStoredEnergy.m_st_bDCDCData[DCDCindex].stMeter.U_Current;	//DCDCU相电流
		m_st_ModuleWorkCurrrent.stDCDCWorkCurrent.V_Current = objStoredEnergy.m_st_bDCDCData[DCDCindex].stMeter.V_Current;	//DCDCV相电流	
		m_st_ModuleWorkCurrrent.stDCDCWorkCurrent.W_Current = objStoredEnergy.m_st_bDCDCData[DCDCindex].stMeter.W_Current;	//DCDCW相电流	
	}
}

/*===========================================================================*
 *  函数名称：StoredEnergy::Dat_RefreshACDCSetData(uint16_t acdcID)
 *  功能描述：更新模块设置量
 *  输入参数：index:模块计数
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::Dat_RefreshACDCSetData(uint16_t acdcID)
{
	uint8_t	index,j;
	for(j = 0;j < m_st_ModuleInfo.ACMajorNum;j++)
	{
		if((acdcID) == m_st_ModuleInfo.ACMajorAddr[j])
		{
			index = j;
			break;
		}
	}
	m_st_ModuleSettingdata.stACDCSettingData1 = objStoredEnergy.m_st_aACDCData[index].stConfig1;
	//m_st_ModuleSettingdata.stACDCSettingData2 = objStoredEnergy.m_st_aACDCData[index].stConfig2;
}
/*===========================================================================*
 *  函数名称：StoredEnergy::Dat_RefreshDCDCSetData(uint16_t dcdcID)
 *  功能描述：更新模块设置量
 *  输入参数：index:模块计数
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::Dat_RefreshDCDCSetData(uint16_t dcdcID)
{
	uint8_t	index,j;
	for(j = 0;j < m_st_ModuleInfo.DCMajorNum;j++)
	{
		if((dcdcID) == m_st_ModuleInfo.DCMajorAddr[j])
		{
			index = j;
			break;
		}
	}
		m_st_ModuleSettingdata.stDCDCSettingData = objStoredEnergy.m_st_bDCDCData[index].stConfig;
}
/*===========================================================================*
 *  函数名称：Dat_GetModuleRunState(uint16_t moduleID)
 *  功能描述：查询模块是否在运行
 *  输入参数：moduleID:0~2为ACDC3~5为DCDC
 *  返回值：1模块在运行/0模块没有运行
 *  使用说明： 
 *==========================================================================*/
uint8_t Class_StoredEnergy::Dat_GetModuleRunState(uint16_t moduleID)
{
	uint16_t Contravrun,Rectifrun,acdccur,index = 0,j = 0;	//ACDC状态标记,ACDC电流
	uint16_t dcdcrun,dcdccur;	//DCDC状态标记,DCDC电流	
    uint8_t ret = 0;
	if (moduleID >= ACDCMINADDR && moduleID <= ACDCMAXADDR )
	{
        if(m_st_ModuleInfo.ACModuleNum > 0)
        {
          for(j = 0;j < m_st_ModuleInfo.ACModuleNum;j++)
          {
              if((moduleID) == m_st_ModuleInfo.ACModeAddr[j])
              {
                      index = j;
                      break;
              }
          }
          Contravrun = m_st_aACDCData[index].stMeter.WorkState.Contrav_Run;
          Rectifrun = m_st_aACDCData[index].stMeter.WorkState.Rectif_Run;
          acdccur = MAX(m_st_aACDCData[index].stMeter.U_Current, m_st_aACDCData[index].stMeter.V_Current);
          acdccur = MAX(acdccur, m_st_aACDCData[index].stMeter.W_Current);
          if((1 == Contravrun) || (1 == Rectifrun) || (1 < (acdccur)))
          {
                  ret = 1;
          }
          else					//返回状态为0并且模块电流最大的小于0.1A
          {
                  ret = 0;
          }
        }
	}
	else if (moduleID >= DCDCMINADDR && moduleID <= DCDCMAXADDR) 
	{
		if(m_st_ModuleInfo.DCModuleNum > 0)
		{
			for(j = 0;j < m_st_ModuleInfo.DCModuleNum;j++)
			{
				if((moduleID) == m_st_ModuleInfo.DCModeAddr[j])
				{
					index = j;
					break;
				}
			}
			dcdcrun = m_st_bDCDCData[index].stMeter.DevStatus.RunFlag;
			dcdccur = MAX(m_st_bDCDCData[index].stMeter.U_Current, m_st_bDCDCData[index].stMeter.V_Current);
			dcdccur = MAX(dcdccur, m_st_bDCDCData[index].stMeter.W_Current);
			if((1 == dcdcrun) || (1 < (dcdccur)))
			{
				ret = 1;
			}
			else	//返回状态为0并且模块电流最大的小于0.1A
			{
				ret = 0;
			}
		}
	}	
	return ret;
}
/*===========================================================================*
 *  函数名称：StoredEnergy::App_SetACDCWorkMode(uint8_t dcacID, uint16_t  u16mode)
 *  功能描述：设置ACDC工作模式
 *  输入参数：acdcID:ACDCmodbus地址		u16para:工作模式(1为恒压/2为恒功率)
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::APP_SetACDCWorkMode(uint8_t acdcID,uint16_t u16para)
{
	_ModBusTxDef tempMsg;
	if (acdcID > ACDCMAXADDR || acdcID < ACDCMINADDR)
	{
		return;
	}
	tempMsg.u8MacId = acdcID;
	tempMsg.u8CmdType = DCAC_CMD_WRITE;
	tempMsg.u16DataStartAdd = WORKMODE;
	tempMsg.u16DataLen = u16para;
	objRS485App.App_SendMsg(tempMsg);
}
/*===========================================================================*
 *  函数名称：StoredEnergy::APP_SetACDCOutVolt(uint8_t acdcID,uint16_t u16para)
 *  功能描述：设置ACDC输出电压
 *  输入参数：acdcID:ACDCmodbus地址		u16para:设置参数
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::APP_SetACDCOutVolt(uint8_t acdcID,uint16_t u16para)
{
	_ModBusTxDef tempMsg;
	if (acdcID > ACDCMAXADDR || acdcID < ACDCMINADDR)
	{
		return;
	}
	tempMsg.u8MacId = acdcID;
	tempMsg.u8CmdType = DCAC_CMD_WRITE;
	tempMsg.u16DataStartAdd = VOLTSETTING;
	tempMsg.u16DataLen = u16para;
	objRS485App.App_SendMsg(tempMsg);
}
/*===========================================================================*
 *  函数名称：StoredEnergy::APP_SetACDCMaxPow(uint8_t acdcID,uint16_t u16para)
 *  功能描述：设置ACDC最大功率
 *  输入参数：acdcID:ACDCmodbus地址		u16para:设置参数
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
/*void Class_StoredEnergy::APP_SetACDCMaxPow(uint8_t acdcID,uint16_t u16para)
{
	_ModBusTxDef tempMsg;
	if (acdcID > ACDCMAXADDR || acdcID < ACDCMINADDR)
	{
		return;
	}
	tempMsg.u8MacId = acdcID;
	tempMsg.u8CmdType = DCAC_CMD_WRITE;
	tempMsg.u16DataStartAdd = POWLIMIT;
	tempMsg.u16DataLen = u16para;
	objRS485App.App_SendMsg(tempMsg);
}
*/
/*===========================================================================*
 *  函数名称：StoredEnergy::APP_SetACDCEnergy(uint8_t acdcID,int16_t para)
 *  功能描述：ACDC有功设定
 *  输入参数：acdcID:ACDCmodbus地址		u16para:设置参数
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::APP_SetACDCEnergy(uint8_t acdcID,int16_t para)
{
	_ModBusTxDef tempMsg;
	if (acdcID > ACDCMAXADDR || acdcID < ACDCMINADDR)
	{
		return;
	}
	tempMsg.u8MacId = acdcID;
	tempMsg.u8CmdType = DCAC_CMD_WRITE;
	tempMsg.u16DataStartAdd = ENERGY_P;
	tempMsg.u16DataLen = para;
	objRS485App.App_SendMsg(tempMsg);
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_SetACDCReactPow(uint8_t acdcID,int16_t para)
 *  功能描述：ACDC无功设定
 *  输入参数：acdcID:ACDCmodbus地址		u16para:设置参数
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::APP_SetACDCReactPow(uint8_t acdcID,int16_t para)
{
	_ModBusTxDef tempMsg;
	if (acdcID > ACDCMAXADDR || acdcID < ACDCMINADDR)
	{
		return;
	}
	tempMsg.u8MacId = acdcID;
	tempMsg.u8CmdType = DCAC_CMD_WRITE;
	tempMsg.u16DataStartAdd = REACTPOWSET;
	tempMsg.u16DataLen = para;
	objRS485App.App_SendMsg(tempMsg);
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_SetACDCReactPowFact(uint8_t acdcID,int16_t para)
 *  功能描述：ACDC无功功率因数设定
 *  输入参数：acdcID:ACDCmodbus地址		u16para:设置参数
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::APP_SetACDCReactPowFact(uint8_t acdcID,int16_t para)
{
	_ModBusTxDef tempMsg;
	if (acdcID > ACDCMAXADDR || acdcID < ACDCMINADDR)
	{
		return;
	}
	tempMsg.u8MacId = acdcID;
	tempMsg.u8CmdType = DCAC_CMD_WRITE;
	tempMsg.u16DataStartAdd = REACTPOWFACT;
	tempMsg.u16DataLen = para;
	objRS485App.App_SendMsg(tempMsg);
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_SetDCDCWorkMode(uint8_t acdcID,uint16_t u16para)
 *  功能描述：设置DCDC工作模式
 *  输入参数：dcdcID:DCDCmodbus地址		u16para:设置参数
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::APP_SetDCDCWorkMode(uint8_t dcdcID,uint16_t u16para)
{
	_ModBusTxDef tempMsg;

	if (dcdcID > DCDCMAXADDR || dcdcID < DCDCMINADDR)
	{
		return;
	}
	tempMsg.u8MacId = dcdcID;
	tempMsg.u8CmdType = DCAC_CMD_WRITE;
	tempMsg.u16DataStartAdd = WORKMODE;
	tempMsg.u16DataLen = u16para;
	objRS485App.App_SendMsg(tempMsg);
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_SetDCDCOutVolt(uint8_t acdcID,uint16_t u16para)
 *  功能描述：设置DCDC直流输出电压
 *  输入参数：dcdcID:DCDCmodbus地址		u16para:设置参数
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::APP_SetDCDCOutVolt(uint8_t dcdcID,uint16_t u16para)
{
	_ModBusTxDef tempMsg;

	if (dcdcID > DCDCMAXADDR || dcdcID < DCDCMINADDR)
	{
		return;
	}
	tempMsg.u8MacId = dcdcID;
	tempMsg.u8CmdType = DCAC_CMD_WRITE;
	tempMsg.u16DataStartAdd = VOLTSETTING;
	tempMsg.u16DataLen = u16para;
	objRS485App.App_SendMsg(tempMsg);
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_SetDCDCMaxPow(uint8_t acdcID,uint16_t u16para)
 *  功能描述：设置DCDC最大功率
 *  输入参数：dcdcID:DCDCmodbus地址		u16para:设置参数
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::APP_SetDCDCMaxPow(uint8_t dcdcID,uint16_t u16para)
{
	_ModBusTxDef tempMsg;

	if (dcdcID > DCDCMAXADDR || dcdcID < DCDCMINADDR)
	{
		return;
	}
	tempMsg.u8MacId = dcdcID;
	tempMsg.u8CmdType = DCAC_CMD_WRITE;
	tempMsg.u16DataStartAdd = POWLIMIT;
	tempMsg.u16DataLen = u16para;
	objRS485App.App_SendMsg(tempMsg);
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_SetDCDCCurrent(uint8_t acdcID,int16_t 16para)
 *  功能描述：设置DCDC电流
 *  输入参数：dcdcID:DCDCmodbus地址		u16para:设置参数
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::APP_SetDCDCCurrent(uint8_t dcdcID,int16_t para)
{
	_ModBusTxDef tempMsg;

	if (dcdcID > DCDCMAXADDR || dcdcID < DCDCMINADDR)
	{
		return;
	}
	tempMsg.u8MacId = dcdcID;
	tempMsg.u8CmdType = DCAC_CMD_WRITE;
	tempMsg.u16DataStartAdd = ENERGY_P; //1003
	tempMsg.u16DataLen = para;
	objRS485App.App_SendMsg(tempMsg);
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_GetACDCWorkMode(uint8_t acdcID,uint16_t u16para)
 *  功能描述：读ACDC工作模式
 *  输入参数：acdcID:ACDCmodbus地址		u16para:参数
 *  返回值：1为模块数据与输入参数相同/0为模块数据与输入参数不同
 *  使用说明： 
 *==========================================================================*/
uint8_t Class_StoredEnergy::APP_GetACDCWorkMode(uint8_t acdcID,uint16_t u16para)
{	
	Dat_RefreshACDCSetData(acdcID);
	if ((m_st_ModuleSettingdata.stACDCSettingData1.Mode) == (u16para))
		return 1;
	else 
		return 0;
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_GetACDCOutVolt(uint8_t acdcID,uint16_t u16para)
 *  功能描述：读ACDC直流输出电压
 *  输入参数：acdcID:ACDCmodbus地址		u16para:参数
 *  返回值：1为模块数据与输入参数相同/0为模块数据与输入参数不同
 *  使用说明： 
 *==========================================================================*/
uint8_t Class_StoredEnergy::APP_GetACDCOutVolt(uint8_t acdcID,uint16_t u16para)
{	
	Dat_RefreshACDCSetData(acdcID);
	if ((m_st_ModuleSettingdata.stACDCSettingData1.VOLT_SET) == (u16para))
		return 1;
	else 
		return 0;
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_GetACDCMaxPow(uint8_t acdcID,uint16_t u16para)
 *  功能描述：读ACDC最大功率
 *  输入参数：acdcID:ACDCmodbus地址		u16para:参数
 *  返回值：1为模块数据与输入参数相同/0为模块数据与输入参数不同
 *  使用说明： 
 *==========================================================================*/
/*uint8_t Class_StoredEnergy::APP_GetACDCMaxPow(uint8_t acdcID,uint16_t u16para)
{	
	Dat_RefreshACDCSetData(acdcID);
	if ((m_st_ModuleSettingdata.stACDCSettingData1.Power_P) == (u16para)) 
		return 1;
	else 
		return 0;
}
*/
/*===========================================================================*
 *  函数名称：StoredEnergy::APP_GetACDCEnergy(uint8_t acdcID,int16_t para)
 *  功能描述：读ACDC有功
 *  输入参数：acdcID:ACDCmodbus地址		u16para:参数
 *  返回值：1为模块数据与输入参数相同/0为模块数据与输入参数不同
 *  使用说明： 
 *==========================================================================*/
uint8_t Class_StoredEnergy::APP_GetACDCEnergy(uint8_t acdcID,int16_t para)
{	
	Dat_RefreshACDCSetData(acdcID);
	if ((m_st_ModuleSettingdata.stACDCSettingData1.Power_N) == (para)) 
		return 1;
	else 
		return 0;
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_GetACDCReactPow(uint8_t acdcID,int16_t para)
 *  功能描述：读ACDC无功
 *  输入参数：acdcID:ACDCmodbus地址		u16para:参数
 *  返回值：1为模块数据与输入参数相同/0为模块数据与输入参数不同
 *  使用说明： 
 *==========================================================================*/
uint8_t Class_StoredEnergy::APP_GetACDCReactPow(uint8_t acdcID,int16_t para)
{	
	Dat_RefreshACDCSetData(acdcID);
	if ((m_st_ModuleSettingdata.stACDCSettingData1.ReactPow) == (para))
		return 1;
	else 
		return 0;
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_GetACDCReactPowFact(uint8_t acdcID,int16_t para)
 *  功能描述：读ACDC无功功率因数
 *  输入参数：acdcID:ACDCmodbus地址		u16para:参数
 *  返回值：1为模块数据与输入参数相同/0为模块数据与输入参数不同
 *  使用说明： 
 *==========================================================================*/
uint8_t Class_StoredEnergy::APP_GetACDCReactPowFact(uint8_t acdcID,int16_t para)
{	
	Dat_RefreshACDCSetData(acdcID);
	if ((m_st_ModuleSettingdata.stACDCSettingData1.PowFact) == (para))
		return 1;
	else 
		return 0;
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_GetDCDCWorkMode(uint8_t acdcID,uint16_t u16para)
 *  功能描述：读DCDC工作模式
 *  输入参数：dcdcID:DCDCmodbus地址		u16para:参数
 *  返回值：1为模块数据与输入参数相同/0为模块数据与输入参数不同
 *  使用说明： 
 *==========================================================================*/
uint8_t Class_StoredEnergy::APP_GetDCDCWorkMode(uint8_t dcdcID,uint16_t u16para)
{	
	Dat_RefreshDCDCSetData(dcdcID);
	if ((m_st_ModuleSettingdata.stDCDCSettingData.WorkMode) == (u16para))
		return 1;
	else 
		return 0;
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_GetDCDCOutVolt(uint8_t acdcID,uint16_t u16para)
 *  功能描述：读DCDC直流输出电压
 *  输入参数：dcdcID:DCDCmodbus地址		u16para:参数
 *  返回值：1为模块数据与输入参数相同/0为模块数据与输入参数不同
 *  使用说明： 
 *==========================================================================*/
uint8_t Class_StoredEnergy::APP_GetDCDCOutVolt(uint8_t dcdcID,uint16_t u16para)
{	
	Dat_RefreshDCDCSetData(dcdcID);
	if ((m_st_ModuleSettingdata.stDCDCSettingData.VoltSet) == (u16para))
		return 1;
	else 
		return 0;
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_GetDCDCMaxPow(uint8_t acdcID,uint16_t u16para)
 *  功能描述：读DCDC最大功率
 *  输入参数：dcdcID:DCDCmodbus地址		u16para:参数
 *  返回值：1为模块数据与输入参数相同/0为模块数据与输入参数不同
 *  使用说明： 
 *==========================================================================*/
uint8_t Class_StoredEnergy::APP_GetDCDCMaxPow(uint8_t dcdcID,uint16_t u16para)
{	
	Dat_RefreshDCDCSetData(dcdcID);
	if ((m_st_ModuleSettingdata.stDCDCSettingData.MaxPow) == (u16para))
		return 1;
	else 
		return 0;
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_GetDCDCCurrent(uint8_t dcdcID,int16_t para)
 *  功能描述：读DCDC电流
 *  输入参数：dcdcID:DCDCmodbus地址		u16para:参数
 *  返回值：1为模块数据与输入参数相同/0为模块数据与输入参数不同
 *  使用说明： 
 *==========================================================================*/
uint8_t Class_StoredEnergy::APP_GetDCDCCurrent(uint8_t dcdcID,int16_t para)
{	
	Dat_RefreshDCDCSetData(dcdcID);
	if ((m_st_ModuleSettingdata.stDCDCSettingData.CurrentSet) == (para))
		return 1;
	else 
		return 0;
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_ReportModuleLinkStatus(uint8_t moduleID)
 *  功能描述：上报模块离线状态
 *  输入参数：dcdcID:DCDCmodbus地址		u16para:参数
 *  返回值：1为模块离线/0为模块未离线
 *  使用说明： 
 *==========================================================================*/
 
Module_offline_state Class_StoredEnergy::APP_ReportModuleLinkStatus(uint8_t moduleID)
{
	uint8_t index,j;
	
	//ACDC数据存放数据从0开始
	if(moduleID >=ACDCMINADDR && moduleID <= ACDCMAXADDR)
	{	
		for(j = 0;j < m_st_ModuleInfo.ACModuleNum;j++)
		{
			if((moduleID) == m_st_ModuleInfo.ACModeAddr[j])
			{
				index = j;
				break;
			}
		}
		if(m_st_aACDCData[index].Online == NO)
			return MODULE_LINK_ABNORMAL;
		else 
			return MODULE_LINK_NORMAL;
	}
	//DCDC数据存放数据从0开始
	if(m_st_ModuleInfo.ModuleLevel == 2)
	{
		if(moduleID >= DCDCMINADDR&& moduleID <= DCDCMAXADDR)
		{
			for(j = 0;j < m_st_ModuleInfo.DCModuleNum;j++)
			{
				if((moduleID) == m_st_ModuleInfo.DCModeAddr[j])
				{
					index = j;
					break;
				}
			}
			if(m_st_bDCDCData[index].Online == NO)
				return MODULE_LINK_ABNORMAL;
			else
				return MODULE_LINK_NORMAL;
		}
	}
	return UNKNOWN_STATE;
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_GetMaxTemp(void)
 *  功能描述：获取模块最大进风口温度
 *  输入参数：none
 *  返回值：模块最大进风口温度
 *  使用说明： 
 *==========================================================================*/
 
int16_t Class_StoredEnergy::APP_GetMaxTemp(void)
{
	uint8_t index;
	int16_t maxtemp = 0;
	for(index = 0;index < m_st_ModuleInfo.ACModuleNum;index++)
	{
		maxtemp = MAX(maxtemp,m_st_aACDCData[index].stMeter2.Import_Temp);
	}
	if(m_st_ModuleInfo.ModuleLevel == 2)
	{
		for(index = 0;index < m_st_ModuleInfo.DCModuleNum;index++)
		{
			maxtemp = MAX(maxtemp,m_st_bDCDCData[index].stMeter2.Import_Temp);
		}
	}
	return maxtemp;
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_GetACDCFaultstate(void)
 *  功能描述：获取ACDC具体故障状态
 *  输入参数：故障代码
 *  返回值：	  0：故障不存在 1：故障存在
 *  使用说明： 故障代码定义在 ACDC_Fault_Word
 *==========================================================================*/
 
uint8_t Class_StoredEnergy::APP_GetACDCFaultstate(uint16_t faultname)
{
	uint8_t state = 0,index;
	for(index = 0;index < m_st_ModuleInfo.ACModuleNum;index++)
	{
		if(m_st_aACDCData[index].stMeter.FaultState == faultname)
		{
			state = 1;
			break;
		}
	}
	return state;
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_SetACDCFunctionMode(uint8_t acdcID,int16_t 16para)
 *  功能描述：设置ACDC功能模式
 *  输入参数：dcdcID:DCDCmodbus地址		u16para:设置参数
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::APP_SetACDCFunctionMode(uint8_t acdcID,int16_t para)
{
	_ModBusTxDef tempMsg;

	if (acdcID > ACDCMAXADDR || acdcID < ACDCMINADDR)
	{
		return;
	}
	tempMsg.u8MacId = acdcID;
	tempMsg.u8CmdType = DCAC_CMD_WRITE;
	tempMsg.u16DataStartAdd = FUNCTIONMODE;
	tempMsg.u16DataLen = para;
	objRS485App.App_SendMsg(tempMsg);
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_GetACDCFunctionMode(uint8_t acdcID,int16_t para)
 *  功能描述：读ACDC功能模式
 *  输入参数：acdcID:ACDCmodbus地址		u16para:参数
 *  返回值：1为模块数据与输入参数相同/0为模块数据与输入参数不同
 *  使用说明： 
 *==========================================================================*/
uint8_t Class_StoredEnergy::APP_GetACDCFunctionMode(uint8_t acdcID,int16_t para)
{	
	Dat_RefreshACDCSetData(acdcID);
	if ((m_st_ModuleSettingdata.stACDCSettingData1.FunctionMode) == (para))
		return 1;
	else 
		return 0;
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_SetACDCConstantCurr(uint8_t acdcID,int16_t para)
 *  功能描述：设置ACDC恒流模式电流值
 *  输入参数：acdcID:设置的ACDC的modbus地址，para:设置量
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::APP_SetACDCConstantCurr(uint8_t acdcID,int16_t para)
{
	_ModBusTxDef tempMsg;
	if (acdcID > ACDCMAXADDR || acdcID < ACDCMINADDR)
	{
		return;
	}
	tempMsg.u8MacId = acdcID;
	tempMsg.u8CmdType = DCAC_CMD_WRITE;
	tempMsg.u16DataStartAdd = CONSTANTCURR;
	tempMsg.u16DataLen = para;
	objRS485App.App_SendMsg(tempMsg);
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_GetACDCConstantCurr(uint8_t acdcID,int16_t para)
 *  功能描述：读ACDC恒流模式电流值
 *  输入参数：acdcID:ACDCmodbus地址		u16para:参数
 *  返回值：1为模块数据与输入参数相同/0为模块数据与输入参数不同
 *  使用说明： 
 *==========================================================================*/
uint8_t Class_StoredEnergy::APP_GetACDCConstantCurr(uint8_t acdcID,int16_t para)
{	
	Dat_RefreshACDCSetData(acdcID);
	if ((m_st_ModuleSettingdata.stACDCSettingData1.ConstantCurr) == (para))
		return 1;
	else 
		return 0;
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_SetACDCUnsignedPara(uint8_t acdcID,_ModuleWriteAddrDef writeaddr,uint16_t para)
 *  功能描述：设置ACDC无符号参数
 *  输入参数：acdcID:设置的ACDC的modbus地址，writeaddr：设置的寄存器地址，para:设置量
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::APP_SetACDCUnsignedPara(uint8_t acdcID,_ModuleWriteAddrDef_U writeaddr,uint16_t para)
{
	_ModBusTxDef tempMsg;
	if (acdcID > ACDCMAXADDR || acdcID < ACDCMINADDR)
	{
		return;
	}
	tempMsg.u8MacId = acdcID;
	tempMsg.u8CmdType = DCAC_CMD_WRITE;
	tempMsg.u16DataStartAdd = writeaddr;
	tempMsg.u16DataLen = para;
	objRS485App.App_SendMsg(tempMsg);
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_GetACDCUnsignedPara(uint8_t acdcID,_ModuleWriteAddrDef_U writeaddr,uint16_t para)
 *  功能描述：读ACDC无符号参数
 *  输入参数：acdcID:读取的ACDCmodbus地址,writeaddr：读取的寄存器地址,u16para:参数
 *  返回值：1为模块数据与输入参数相同/0为模块数据与输入参数不同
 *  使用说明： 
 *==========================================================================*/
uint8_t Class_StoredEnergy::APP_GetACDCUnsignedPara(uint8_t acdcID,_ModuleWriteAddrDef_U writeaddr,uint16_t para)
{	
	uint16_t *modulepara;
	uint8_t  addr;
	Dat_RefreshACDCSetData(acdcID);
	addr = writeaddr & 0x00ff;
	modulepara = &m_st_ModuleSettingdata.stACDCSettingData1.Mode + addr;
	if ((*modulepara) == (para))
		return 1;
	else 
		return 0;
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_SetACDCUnsignedPara(uint8_t acdcID,_ModuleWriteAddrDef writeaddr,uint16_t para)
 *  功能描述：设置ACDC有符号参数
 *  输入参数：acdcID:设置的ACDC的modbus地址，writeaddr：设置的寄存器地址，para:设置量
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::APP_SetACDCSignedPara(uint8_t acdcID,_ModuleWriteAddrDef_S writeaddr,int16_t para)
{
	_ModBusTxDef tempMsg;
	if (acdcID > ACDCMAXADDR || acdcID < ACDCMINADDR)
	{
		return;
	}
	tempMsg.u8MacId = acdcID;
	tempMsg.u8CmdType = DCAC_CMD_WRITE;
	tempMsg.u16DataStartAdd = writeaddr;
	tempMsg.u16DataLen = para;
	objRS485App.App_SendMsg(tempMsg);
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_GetACDCUnsignedPara(uint8_t acdcID,int16_t para)
 *  功能描述：读ACDC无符号参数
 *  输入参数：acdcID:读取的ACDCmodbus地址,writeaddr：读取的寄存器地址,u16para:参数
 *  返回值：1为模块数据与输入参数相同/0为模块数据与输入参数不同
 *  使用说明： 
 *==========================================================================*/
uint8_t Class_StoredEnergy::APP_GetACDCSignedPara(uint8_t acdcID,_ModuleWriteAddrDef_S writeaddr,int16_t para)
{	
	uint16_t *modulepara;
	uint8_t  addr;
	Dat_RefreshACDCSetData(acdcID);
	addr = writeaddr & 0x00ff;
	modulepara = &m_st_ModuleSettingdata.stACDCSettingData1.Mode + addr;
	if ((*modulepara) == (para))
		return 1;
	else 
		return 0;
}


/*===========================================================================*
 *  函数名称：StoredEnergy::APP_SetDCPara(uint8_t dcdcID,_DC_ModuleWriteAddrDef writeaddr,uint16_t para)
 *  功能描述：设置DCDC参数
 *  输入参数：dcdcID:设置的DCDC的modbus地址，writeaddr：设置的寄存器地址，para:设置量
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_StoredEnergy::APP_SetDCPara(uint8_t dcdcID,_DC_ModuleWriteAddrDef writeaddr,uint16_t para)
{
	_ModBusTxDef tempMsg;
	if (dcdcID > DCDCMAXADDR || dcdcID < DCDCMINADDR)
	{
		return;
	}
	tempMsg.u8MacId = dcdcID;
	tempMsg.u8CmdType = DCAC_CMD_WRITE;
	tempMsg.u16DataStartAdd = writeaddr;
	tempMsg.u16DataLen = para;
	objRS485App.App_SendMsg(tempMsg);
}

/*===========================================================================*
 *  函数名称：StoredEnergy::APP_GetDCPara(uint8_t dcdcID,_DC_ModuleWriteAddrDef writeaddr,uint16_t para)
 *  功能描述：读DCDC无符号参数
 *  输入参数：dcdcID:读取的DCDC的modbus地址,writeaddr：读取的寄存器地址,u16para:参数
 *  返回值：1为模块数据与输入参数相同/0为模块数据与输入参数不同
 *  使用说明： 
 *==========================================================================*/
uint8_t Class_StoredEnergy::APP_GetDCPara(uint8_t dcdcID,_DC_ModuleWriteAddrDef writeaddr,uint16_t para)
{	
	uint16_t *modulepara;
	uint8_t  addr;
	Dat_RefreshACDCSetData(dcdcID);
	addr = writeaddr & 0x00ff;
	modulepara = &m_st_ModuleSettingdata.stACDCSettingData1.Mode + addr;
	if ((*modulepara) == (para))
		return 1;
	else 
		return 0;
}


