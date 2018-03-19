/*==========================================================*/
/* Title		:	Obj_EMMonitor.c														*/
/* Description	        : 	        											*/
// Author		:	
/* Date			:	2017.11.14														*/
/*==========================================================*/

#include "Publics.h"
#include "CRC.h"

#include <math.h>
/***************************************************************************
函数名:Dat_Init
功能描述:
参数描述：
返回值：
****************************************************************************/
void Class_EMMonitor::Dat_Init(void)
{

	
}

/***************************************************************************
函数名:Dat_DataMainten
功能描述:
函数调用
作者:
日期:
****************************************************************************/
void	Class_EMMonitor::Dat_DataMainten(void)
{
	static int16_t s_XmitCnt;
	static int16_t index;
	static int16_t index2 = 0;//PCS柜信息2发送次数计数
	static int16_t	pcsdata2flag = 0;//PCS柜信息2传送标志,0为未传
	int16_t i16temp = objPcuModel.m_i16_SelfAddr;
//	objBMS.App_RespMaxChgCurr();
	if(i16temp >= MAC_ID_StoreEnergyBOX1 && i16temp <= MAC_ID_StoreEnergyBOX30)//储能一体柜
	{
		//如果集控未收到PCS信息2，标志位为0，向集控发送PCS柜信息2
		if (objCanDataPool.m_u8_PCSinfo2Flag == 0)
            {
			   objFailManager.App_UploadPCSData2();
            }
		//如果集控收到pcs信息2，标志位为1，向集控发送模块、bms、pcs信息1
		if(objCanDataPool.m_u8_PCSinfo2Flag == 1)
		{
			if (g_st_TimerFlag.b10ms == 1)
			{
				index++;
				if (index % 15 != 0)
				{
					return;
				}
				index = 0;
					if (s_XmitCnt < 8)
					{
						s_XmitCnt++;
						Dat_StoredEnergyBoxMsgXmit();
					}
					else if(s_XmitCnt < 9)
					{
						objBMS.App_UploadBMSInfo();	//上报BMS数据到集控
						s_XmitCnt++;
					}
					else if(s_XmitCnt < 10)
					{
						objFailManager.App_UploadPCSData1();
						s_XmitCnt = 0;
					}
					else
					{
					}
			}
		}
	}         
}

/*===========================================================================*
 *  函数名称：EMMonitor::Dat_RefreshDCDCData(int16_t index)
 *  功能描述：模块信息上传
 *  输入参数：DCDC数据结构体数组序号
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void	Class_EMMonitor::Dat_RefreshDCDCData(int16_t index)
{
	//遥测数据
	m_st_DCDCRemoteData.GroupID = objStoredEnergy.m_st_bDCDCData[index].ModID;  //上传模块ID
	//m_st_EBattRemoteData.GroupID = index + DCDC_MODID;  //上传模块ID
	m_st_DCDCRemoteData.meter1 = objStoredEnergy.m_st_bDCDCData[index].stMeter;
	m_st_DCDCRemoteData.stconfig = objStoredEnergy.m_st_bDCDCData[index].stConfig;
	m_st_DCDCRemoteData.Improt_Temp = objStoredEnergy.m_st_bDCDCData[index].stMeter2.Import_Temp;

}

//for bms data refresh test

void	Class_EMMonitor::Dat_RefreshBMSData()
{
/*
	static	uint8_t j;

	j++;
	
		if(j>255)
			{
				j= 0;
			}

				objBMS.BMS_Param.B2C_Status1.u8BattAllowedChgCurrMax_L 	= j;
				objBMS.BMS_Param.B2C_Status1.u8BattAllowedChgCurrMax_H 	= j;
				objBMS.BMS_Param.B2C_Status1.u8BattAllowedDisChgCurrMax_L	= j;
				objBMS.BMS_Param.B2C_Status1.u8BattAllowedDisChgCurrMax_H	= j;
				objBMS.BMS_Param.B2C_Status1.u8BattGroupVolt_L 			= j;
				objBMS.BMS_Param.B2C_Status1.u8BattGroupVolt_H 			= j;
				objBMS.BMS_Param.B2C_Status1.u8BattGroupCurr_L 			= j;
				objBMS.BMS_Param.B2C_Status1.u8BattGroupCurr_H 			= j;
				
	
		
				objBMS.BMS_Param.B2C_Status2.u8BattAllowedChgPowerMax_L		= j;
				objBMS.BMS_Param.B2C_Status2.u8BattAllowedChgPowerMax_H		= j;
				objBMS.BMS_Param.B2C_Status2.u8BattAllowedDisChgPowerMax_L 	= j;
				objBMS.BMS_Param.B2C_Status2.u8BattAllowedDisChgPowerMax_H 	= j;
				objBMS.BMS_Param.B2C_Status2.u8BattAllowedDisChgVoltMin_L		= j;
				objBMS.BMS_Param.B2C_Status2.u8BattAllowedDisChgVoltMin_H		= j;
				objBMS.BMS_Param.B2C_Status2.Group_SOC 						= j;
				objBMS.BMS_Param.B2C_Status2.Group_SOH 						= j;
		
				

				objBMS.BMS_Param.B2C_Status3.u8BattStateBits.u8Word1All	= j;
				objBMS.BMS_Param.B2C_Status3.Batt_type 					= j;
				objBMS.BMS_Param.B2C_Status3.Batt_Capacity_L				= j;
				objBMS.BMS_Param.B2C_Status3.Batt_Capacity_H				= j;
				objBMS.BMS_Param.B2C_Status3.Env_Temp						= j;
				objBMS.BMS_Param.B2C_Status3.Reserved						= j;
				objBMS.BMS_Param.B2C_Status3.Batt_ChgCnt_L 				= j;
				objBMS.BMS_Param.B2C_Status3.Batt_ChgCnt_H 				= j;
	
	
		
				objBMS.BMS_Param.B2C_Warn.Slight_Warn1.u8Word1All		= j;
				objBMS.BMS_Param.B2C_Warn.Slight_Warn2.u8Word1All		= j;
				objBMS.BMS_Param.B2C_Warn.Medium_Warn1.u8Word1All		= j;
				objBMS.BMS_Param.B2C_Warn.Medium_Warn2.u8Word1All		= j;
				objBMS.BMS_Param.B2C_Warn.Serious_Warn1.u8Word1All 	= j;
				objBMS.BMS_Param.B2C_Warn.Serious_Warn2.u8Word1All 	= j;
				objBMS.BMS_Param.B2C_Warn.Reserved1					= j;
				objBMS.BMS_Param.B2C_Warn.Reserved2					= j;
			
		
				objBMS.BMS_Param.B2C_VoltInfo.Cell_VoltMin_L	= j;
				objBMS.BMS_Param.B2C_VoltInfo.Cell_VoltMin_H	= j;
				objBMS.BMS_Param.B2C_VoltInfo.CellNum_VoltMin	= j;
				objBMS.BMS_Param.B2C_VoltInfo.Cell_VoltMax_L	= j;
				objBMS.BMS_Param.B2C_VoltInfo.Cell_VoltMax_H	= j;
				objBMS.BMS_Param.B2C_VoltInfo.CellNum_VoltMax	= j;
				objBMS.BMS_Param.B2C_VoltInfo.Reserved1		= j;
				objBMS.BMS_Param.B2C_VoltInfo.Reserved2		= j;
	
	
				objBMS.BMS_Param.B2C_Temp_SOC.Cell_TempMin 	= j;
				objBMS.BMS_Param.B2C_Temp_SOC.CellNum_TempMin	= j;
				objBMS.BMS_Param.B2C_Temp_SOC.Cell_TempMax 	= j;
				objBMS.BMS_Param.B2C_Temp_SOC.CellNum_TempMax	= j;
				objBMS.BMS_Param.B2C_Temp_SOC.Cell_SOC_Min 	= j;
				objBMS.BMS_Param.B2C_Temp_SOC.CellNum_SOC_Min	= j;
				objBMS.BMS_Param.B2C_Temp_SOC.Cell_SOC_Max 	= j;
				objBMS.BMS_Param.B2C_Temp_SOC.CellNum_SOC_Max	= j;
			
				
	
				objBMS.BMS_Param.B2C_Cell_Info1.Cell_DisChgVoltMin_L		= j;
				objBMS.BMS_Param.B2C_Cell_Info1.Cell_DisChgVoltMin_H		= j;
				objBMS.BMS_Param.B2C_Cell_Info1.Cell_ChgVoltMax_L			= j;
				objBMS.BMS_Param.B2C_Cell_Info1.Cell_ChgVoltMax_H			= j;
				objBMS.BMS_Param.B2C_Cell_Info1.Cell_DisChgTempMin 		= j;
				objBMS.BMS_Param.B2C_Cell_Info1.Cell_DisChgTempMax 		= j;
				objBMS.BMS_Param.B2C_Cell_Info1.Cell_ChgTempMin			= j;
				objBMS.BMS_Param.B2C_Cell_Info1.Cell_ChgTempMax			= j;
			
	
		
				objBMS.BMS_Param.B2C_Cell_Info2.CellNum_MidVoltLack		= j;
				objBMS.BMS_Param.B2C_Cell_Info2.CellNum_MidVoltOver		= j;
				objBMS.BMS_Param.B2C_Cell_Info2.CellNum_MidTempLow 		= j;
				objBMS.BMS_Param.B2C_Cell_Info2.CellNum_MidTempHigh		= j;
				objBMS.BMS_Param.B2C_Cell_Info2.Reserved1					= j;
				objBMS.BMS_Param.B2C_Cell_Info2.Reserved2					= j;
				objBMS.BMS_Param.B2C_Cell_Info2.Reserved3					= j;
				objBMS.BMS_Param.B2C_Cell_Info2.Reserved4					= j;
  */
		
}


/*===========================================================================*
 *  函数名称：EMMonitor::Dat_RefreshACDCData(int16_t index)
 *  功能描述：模块信息上传
 *  输入参数：ACDC数据结构体数组序号
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/

void	Class_EMMonitor::Dat_RefreshACDCData(int16_t index)
{
	//遥测数据
	m_st_ACDCRemoteData.i16_ModID =  objStoredEnergy.m_st_aACDCData[index].ModID;
	//m_st_ACDCRemotedata.i16_ModID=index + ACDC_MODID;	//上传模块ID
	m_st_ACDCRemoteData.stACDCData = objStoredEnergy.m_st_aACDCData[index].stMeter;
	m_st_ACDCRemoteData.stACDCSetData.Mode = objStoredEnergy.m_st_aACDCData[index].stConfig1.Mode;
	m_st_ACDCRemoteData.stACDCSetData.Sing_ParaConnect = objStoredEnergy.m_st_aACDCData[index].stConfig1.Sing_ParaConnect;
	m_st_ACDCRemoteData.stACDCSetData.MasterSlave = objStoredEnergy.m_st_aACDCData[index].stConfig1.MasterSlave;
	m_st_ACDCRemoteData.stACDCSetData.ConnectAddr = objStoredEnergy.m_st_aACDCData[index].stConfig1.ConnectAddr;
	m_st_ACDCRemoteData.stACDCSetData.ModbusAddr = objStoredEnergy.m_st_aACDCData[index].stConfig1.ModbusAddr;
	m_st_ACDCRemoteData.Import_Temp = objStoredEnergy.m_st_aACDCData[index].stMeter2.Import_Temp;
}

/*===========================================================================*
 *  函数名称：EMMonitor::Dat_StoredEnergyBoxMsgXmit(void)
 *  功能描述：模块信息上传
 *  输入参数：none
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/

void Class_EMMonitor::Dat_StoredEnergyBoxMsgXmit(void)
{
	static uint16_t index1 = 1;    //1发送ACDC请求，2发送DCDC请求
	uint8_t moduleID;
	if(objPcsConfigManage.m_st_GetConfigPara.ModuleLevel == 1)
	{
		/*if(objRS485App.m_u16_ACDCHaveMsgXmit == 1)
		{
			objRS485App.m_u16_ACDCHaveMsgXmit = 0;
			Dat_RefreshACDCData(objRS485App.m_u16_ACDCRefreshID);	//将modbus地址转成模块数据数组序号
			objRS485App.m_u16_ACDCRefreshID = 0;
			objCanMstMonLongFrame.App_TransData(objPcuModel.m_i16_SelfAddr,250,PGN_ACDC_DATADEF,
									sizeof(_ACDCRemoteDataDef),(uint8_t*)(&m_st_ACDCRemotedata));	
		}
		*/
		if(E_QUEUE_NULL != objModBusProtocol.m_ACIDRecvQue.FlagQueue())
		{
			moduleID = objModBusProtocol.m_ACIDRecvQue.GetOldestNode();
			objModBusProtocol.m_ACIDRecvQue.DelQueue();
			Dat_RefreshACDCData(moduleID);
			objCanMstMonLongFrame.App_TransData(objPcuModel.m_i16_SelfAddr,250,PGN_ACDC_DATADEF,
						sizeof(_ACDCRemoteDataDef),(uint8_t*)(&m_st_ACDCRemoteData));	
		}
	}
	else if(objPcsConfigManage.m_st_GetConfigPara.ModuleLevel == 2)
	{
		if (index1 == 1 )
		{
			if(E_QUEUE_NULL != objModBusProtocol.m_ACIDRecvQue.FlagQueue())
			{
				moduleID = objModBusProtocol.m_ACIDRecvQue.GetOldestNode();
				objModBusProtocol.m_ACIDRecvQue.DelQueue();
				Dat_RefreshACDCData(moduleID);
				objCanMstMonLongFrame.App_TransData(objPcuModel.m_i16_SelfAddr,250,PGN_ACDC_DATADEF,
							sizeof(_ACDCRemoteDataDef),(uint8_t*)(&m_st_ACDCRemoteData));	
			}
			index1 = 2;
		}
		else if(index1 == 2)
		{
			if(E_QUEUE_NULL != objModBusProtocol.m_DCIDRecvQue.FlagQueue())
			{
				moduleID = objModBusProtocol.m_DCIDRecvQue.GetOldestNode();
				objModBusProtocol.m_DCIDRecvQue.DelQueue();
				Dat_RefreshDCDCData(moduleID);
				objCanMstMonLongFrame.App_TransData(objPcuModel.m_i16_SelfAddr,250,PGN_ACDC_DATADEF,
							sizeof(_DCDCRemoteDataDef),(uint8_t*)(&m_st_DCDCRemoteData));	
			}
			index1 = 1;
		}
	}
}


//===========================================================================
// End of file.
//===========================================================================



