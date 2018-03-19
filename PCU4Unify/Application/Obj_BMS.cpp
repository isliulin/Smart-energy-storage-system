/*==========================================================*/
/* Title		:	Obj_BMS.cpp								*/
/* Description	:	BMS Communication With Can				*/
/* Author		:	Hexianguo								*/
/* Date			:	2017.11.14								*/
/*==========================================================*/

#include "Publics.h"

/*
static const float BmsSeqChgCurr[21][14] =			//不同SOC、温度下，BMS请求充电电流值
{
	{0, 0.1, 0.116, 0.116, 0.372, 0.372, 0.5, 0.5, 0.75, 0.8, 0.8, 0.8, 0.279, 0},
	{0, 0.1, 0.116, 0.116, 0.372, 0.372, 0.5, 0.5, 0.75, 0.8, 0.8, 0.8, 0.279, 0},
	{0, 0.1, 0.116, 0.116, 0.372, 0.372, 0.5, 0.5, 0.75, 0.8, 0.8, 0.8, 0.279, 0},
	{0, 0.1, 0.116, 0.116, 0.372, 0.372, 0.5, 0.5, 0.75, 0.8, 0.8, 0.8, 0.279, 0},

	{0, 0.1, 0.116, 0.116, 0.372, 0.372, 0.5, 0.5, 1, 1, 1, 0.8, 0.279, 0},
	{0, 0.1, 0.116, 0.116, 0.372, 0.372, 0.5, 0.5, 1, 1, 1, 0.8, 0.279, 0},

	{0, 0, 0.116, 0.116, 0.372, 0.372, 0.5, 0.5, 1, 1, 1, 0.8, 0.279, 0},
	{0, 0, 0.116, 0.116, 0.372, 0.372, 0.5, 0.5, 1, 1, 1, 0.8, 0.279, 0},
	{0, 0, 0.116, 0.116, 0.372, 0.372, 0.5, 0.5, 1, 1, 1, 0.8, 0.279, 0},
	{0, 0, 0.116, 0.116, 0.372, 0.372, 0.5, 0.5, 1, 1, 1, 0.8, 0.279, 0},
	{0, 0, 0.116, 0.116, 0.372, 0.372, 0.5, 0.5, 1, 1, 1, 0.8, 0.279, 0},
	{0, 0, 0.116, 0.116, 0.372, 0.372, 0.5, 0.5, 1, 1, 1, 0.8, 0.279, 0},
	{0, 0, 0.116, 0.116, 0.372, 0.372, 0.5, 0.5, 1, 1, 1, 0.8, 0.279, 0},
	{0, 0, 0.116, 0.116, 0.372, 0.372, 0.5, 0.5, 1, 1, 1, 0.8, 0.279, 0},
	{0, 0, 0.116, 0.116, 0.372, 0.372, 0.5, 0.5, 1, 1, 1, 0.8, 0.279, 0},
	{0, 0, 0.116, 0.116, 0.372, 0.372, 0.5, 0.5, 1, 1, 1, 0.8, 0.279, 0},
	{0, 0, 0.116, 0.116, 0.372, 0.372, 0.5, 0.5, 1, 1, 1, 0.8, 0.279, 0},
	{0, 0, 0.116, 0.116, 0.372, 0.372, 0.5, 0.5, 1, 1, 1, 0.8, 0.279, 0},
	{0, 0, 0.116, 0.116, 0.372, 0.372, 0.5, 0.5, 1, 1, 1, 0.8, 0.279, 0},
	{0, 0, 0.116, 0.116, 0.372, 0.372, 0.5, 0.5, 1, 1, 1, 0.8, 0.279, 0},
	{0, 0, 0.116, 0.116, 0.372, 0.372, 0.5, 0.5, 1, 1, 1, 0.8, 0.279, 0}
};
*/

/***************************************************************************
函数名：Class_BMS::Dat_Init
功能描述：BMS_Param结构体初始化为0
输入参数：无
返回值：无
使用说明：
****************************************************************************/

void Class_BMS::Dat_Init(void)
{
	memset(&BMS_Param, 0, sizeof(_St_BMS_Param));
}

/***************************************************************************
函数名:App_BMSProc
功能描述:
函数调用
作者:
日期:
****************************************************************************/
void Class_BMS::App_BMSProc(void)
{
//	App_Batt_StatusFlag();
//	App_BMSFaultCheck();
//	App_RespContactorStatus();
//	App_Get_BattDataInfo(&objStoredEnergy.Batt_DataInfo);
	
//	App_PCS_STATUS_Info();
//	App_PCSToBMS_Msg();
}

/***************************************************************************
函数名:App_BMSProc
功能描述:
函数调用
作者:
日期:
****************************************************************************/
void Class_BMS::App_BMS_Can_Debug(void)
{
		
}
/***************************************************************************
函数名:App_PCS_STATUS_Info
功能描述:
函数调用
作者:
日期:
****************************************************************************/
void Class_BMS::App_PCS_STATUS_Info(void)
{
	
}

/***************************************************************************
函数名：Class_BMS::App_MsgDispatch
功能描述：BMS上报电池数据解析
输入参数：BMS应用层数据包结构体指针
返回值：无
使用说明：
****************************************************************************/

void Class_BMS::App_MsgDispatch(_CanBMSAppMsgDef *pApdu)
{
	uint16_t u16Cmd;
//	u16Cmd = (pApdu->u16PGN) | ((uint8_t)pApdu->u16MACId);
    BMS_Param.BMS_Mode_Id = pApdu->u16MACId - 1;	//(桑顿协议)BMS地址为1-10，Mode Id为0-9
	u16Cmd = pApdu->u16PGN;		
	switch(u16Cmd)
	{

		case PGN_B2C_STATUS1:
			BMS_Param.B2C_Status1.u16BattAllowedChgCurrMax		= ((uint16_t)(pApdu->pDataPtr[1]) << 8) |(pApdu->pDataPtr[0]);
			
			BMS_Param.B2C_Status1.u16BattAllowedDisChgCurrMax		= ((uint16_t)(pApdu->pDataPtr[3]) << 8) |(pApdu->pDataPtr[2]);
			
			BMS_Param.B2C_Status1.u16BattGroupVolt				= ((uint16_t)(pApdu->pDataPtr[5]) << 8) |(pApdu->pDataPtr[4]);
				
			BMS_Param.B2C_Status1.u16BattGroupCurr				= (((uint16_t)(pApdu->pDataPtr[7]) << 8) |(pApdu->pDataPtr[6])) - 32000;
			
			break;

		case PGN_B2C_STATUS2:
			BMS_Param.B2C_Status2.u16BattAllowedChgPowerMax			= ((uint16_t)(pApdu->pDataPtr[1]) << 8) |(pApdu->pDataPtr[0]);
		
			BMS_Param.B2C_Status2.u16BattAllowedDisChgPowerMax		= ((uint16_t)(pApdu->pDataPtr[3]) << 8) |(pApdu->pDataPtr[2]);
			
			BMS_Param.B2C_Status2.u16BattAllowedDisChgVoltMin			= ((uint16_t)(pApdu->pDataPtr[5]) << 8) |(pApdu->pDataPtr[4]);
		
			BMS_Param.B2C_Status2.Group_SOC							= pApdu->pDataPtr[6];
			BMS_Param.B2C_Status2.Group_SOH							= pApdu->pDataPtr[7];
			break;	
			
		case PGN_B2C_STATUS3:
			BMS_Param.B2C_Status3.u8BattStateBits.u8Word1All 	= pApdu->pDataPtr[0];
			BMS_Param.B2C_Status3.Batt_type 					= pApdu->pDataPtr[1];
			BMS_Param.B2C_Status3.Batt_Capacity					= ((uint16_t)(pApdu->pDataPtr[3]) << 8) |(pApdu->pDataPtr[2]);           
			BMS_Param.B2C_Status3.Env_Temp			 			= pApdu->pDataPtr[4] - 40;	//温度偏移量
			BMS_Param.B2C_Status3.Reserved			 			= pApdu->pDataPtr[5]; 
			BMS_Param.B2C_Status3.Batt_ChgCnt		 			= ((uint16_t)(pApdu->pDataPtr[7]) << 8) |(pApdu->pDataPtr[6]); 
			break;

		case PGN_B2C_WarnFlag:
			BMS_Param.B2C_Warn.Slight_Warn1.u8Word1All		= pApdu->pDataPtr[0];
			BMS_Param.B2C_Warn.Slight_Warn2.u8Word1All 		= pApdu->pDataPtr[1];
			BMS_Param.B2C_Warn.Medium_Warn1.u8Word1All		= pApdu->pDataPtr[2];
			BMS_Param.B2C_Warn.Medium_Warn2.u8Word1All		= pApdu->pDataPtr[3];
			BMS_Param.B2C_Warn.Serious_Warn1.u8Word1All 	= pApdu->pDataPtr[4];
			BMS_Param.B2C_Warn.Serious_Warn2.u8Word1All		= pApdu->pDataPtr[5]; 
			BMS_Param.B2C_Warn.Reserved1 					= pApdu->pDataPtr[6];		
			BMS_Param.B2C_Warn.Reserved2 					= pApdu->pDataPtr[7];
			break;
					
		case PGN_B2C_CellInfo1:
			BMS_Param.B2C_VoltInfo.Cell_VoltMin 	= ((uint16_t)(pApdu->pDataPtr[1]) << 8) |(pApdu->pDataPtr[0]); 
			BMS_Param.B2C_VoltInfo.CellNum_VoltMin	= pApdu->pDataPtr[2];
			BMS_Param.B2C_VoltInfo.Cell_VoltMax		= ((uint16_t)(pApdu->pDataPtr[4]) << 8) |(pApdu->pDataPtr[3]); 
			BMS_Param.B2C_VoltInfo.CellNum_VoltMax 	= pApdu->pDataPtr[5]; 
			BMS_Param.B2C_VoltInfo.Reserved1 		= pApdu->pDataPtr[6];
			BMS_Param.B2C_VoltInfo.Reserved2 		= pApdu->pDataPtr[7];
			break;

		case PGN_B2C_CellInfo2:
			BMS_Param.B2C_Temp_SOC.Cell_TempMin 	= pApdu->pDataPtr[0];
			BMS_Param.B2C_Temp_SOC.CellNum_TempMin 	= pApdu->pDataPtr[1];
			BMS_Param.B2C_Temp_SOC.Cell_TempMax		= pApdu->pDataPtr[2];
			BMS_Param.B2C_Temp_SOC.CellNum_TempMax	= pApdu->pDataPtr[3];
			BMS_Param.B2C_Temp_SOC.Cell_SOC_Min 	= pApdu->pDataPtr[4];
			BMS_Param.B2C_Temp_SOC.CellNum_SOC_Min 	= pApdu->pDataPtr[5]; 
			BMS_Param.B2C_Temp_SOC.Cell_SOC_Max 	= pApdu->pDataPtr[6];
			BMS_Param.B2C_Temp_SOC.CellNum_SOC_Max 	= pApdu->pDataPtr[7];
			break;
			
		case PGN_B2C_CellInfo3:
			BMS_Param.B2C_Cell_Info1.Cell_DisChgVoltMin			= ((uint16_t)(pApdu->pDataPtr[1]) << 8) |(pApdu->pDataPtr[0]);
			BMS_Param.B2C_Cell_Info1.Cell_ChgVoltMax			= ((uint16_t)(pApdu->pDataPtr[3]) << 8) |(pApdu->pDataPtr[2]);
			BMS_Param.B2C_Cell_Info1.Cell_DisChgTempMin			= pApdu->pDataPtr[4];
			BMS_Param.B2C_Cell_Info1.Cell_DisChgTempMax			= pApdu->pDataPtr[5];	
			BMS_Param.B2C_Cell_Info1.Cell_ChgTempMin			= pApdu->pDataPtr[6];
			BMS_Param.B2C_Cell_Info1.Cell_ChgTempMax			= pApdu->pDataPtr[7];
			break;

		case PGN_B2C_CellInfo4:
			BMS_Param.B2C_Cell_Info2.CellNum_MidVoltLack		= pApdu->pDataPtr[0];
			BMS_Param.B2C_Cell_Info2.CellNum_MidVoltOver		= pApdu->pDataPtr[1];
			BMS_Param.B2C_Cell_Info2.CellNum_MidTempLow			= pApdu->pDataPtr[2];
			BMS_Param.B2C_Cell_Info2.CellNum_MidTempHigh		= pApdu->pDataPtr[3];
			BMS_Param.B2C_Cell_Info2.Reserved1					= pApdu->pDataPtr[4];
			BMS_Param.B2C_Cell_Info2.Reserved2					= pApdu->pDataPtr[5];	
			BMS_Param.B2C_Cell_Info2.Reserved3					= pApdu->pDataPtr[6];
			BMS_Param.B2C_Cell_Info2.Reserved4					= pApdu->pDataPtr[7];
			break;

		default:
			break;	
	}


	m_i16_EnergyBattOnline = YES;
	m_u16_BMSOfflineCnt = 0;
}
/***************************************************************************
函数名：Class_BMS::App_SetBattOnOff
功能描述：设置电池功率接触器开关
输入参数：电池功率接触器开关动作（1：上电，2：下电）
返回值：无
使用说明：
****************************************************************************/

void Class_BMS::App_SetBattOnOff(uint16_t u16OnOffState)
{
	_CanBMSAppMsgDef tempdata;
	uint8_t temp[8];
	
	tempdata = objCanDataExchg.m_ToBatteryMsg.MsgObj[INDEX_SET_BATT_CMD];
	tempdata.pDataPtr = temp;
	
	if(u16OnOffState == ENERGY_BATT_POWER_ON)
	{
		tempdata.pDataPtr[0] = 0x21;
	}
	else if(u16OnOffState == ENERGY_BATT_POWER_OFF)
	{
		tempdata.pDataPtr[0] = 0x40;
	}
	tempdata.pDataPtr[1] = 0;
	tempdata.pDataPtr[2] = 0;
	tempdata.pDataPtr[3] = 0;
	tempdata.pDataPtr[4] = 0;
	tempdata.pDataPtr[5] = 0;
	tempdata.pDataPtr[6] = 0;
	tempdata.pDataPtr[7] = 0;
	objCanDataExchg.TransSetReqCmdToBMS(&tempdata);
}


/***************************************************************************
函数名:Class_BMS::App_UploadBMSInfo
功能描述:上传BMS信息到集控
输入参数：无
返回值：无
使用说明：使用长帧发送
****************************************************************************/

void Class_BMS::App_UploadBMSInfo(void)
{
//	ObjEMMonitor.Dat_RefreshBMSData();
	if (g_BmsHaveMsgXimt == E_HAVE_MSG_XMIT)
	{
		g_BmsHaveMsgXimt = E_NON_MSG_XMIT;
		objCanMstMonLongFrame.App_TransData(objPcuModel.m_i16_SelfAddr,250,PGN_BMSINFO_UPLOADSCU,sizeof(_St_BMS_Param),(uint8_t*)(&BMS_Param));
	}
		
}


/***************************************************************************
函数名:Class_BMS::App_Batt_StatusFlag
功能描述:1.反馈电池状态信息（告警等级、是否故障）给开关机模块，开关机模块据此判断是否开机 
		 2.反馈给故障管理模块
输入参数：无
返回值：告警等级
使用说明：
****************************************************************************/
BATT_STATE Class_BMS::App_Batt_StatusFlag(void)
{
	if((BMS_Param.B2C_Warn.Slight_Warn1.u8Word1All >= BMS_ALARM_OCCUR)
		|| (BMS_Param.B2C_Warn.Slight_Warn2.u8Word1All >= BMS_ALARM_OCCUR 
			&& BMS_Param.B2C_Warn.Slight_Warn2.Bits.Batt_InsulationFault == BATT_INSULATION_NORMAL))
		{
			return SLIGHT_WARN;
		}
	if((BMS_Param.B2C_Warn.Medium_Warn1.u8Word1All >= BMS_ALARM_OCCUR) 	 
		|| (BMS_Param.B2C_Warn.Medium_Warn2.u8Word1All >= BMS_ALARM_OCCUR 
			&& BMS_Param.B2C_Warn.Medium_Warn2.Bits.Batt_InsulationFault == BATT_INSULATION_NORMAL))
		{
			return MEDIUM_WARN;	
		}
	if((BMS_Param.B2C_Warn.Serious_Warn1.u8Word1All >= BMS_ALARM_OCCUR)
		|| (BMS_Param.B2C_Warn.Serious_Warn2.u8Word1All >= BMS_ALARM_OCCUR 
			&& BMS_Param.B2C_Warn.Serious_Warn2.Bits.Batt_InsulationFault == BATT_INSULATION_NORMAL))
		{
			return SERIOUS_WARN;
		}
	if((BMS_Param.B2C_Warn.Slight_Warn2.Bits.Batt_InsulationFault == BATT_INSULATION_FAULT) 
			|| (BMS_Param.B2C_Warn.Medium_Warn2.Bits.Batt_InsulationFault == BATT_INSULATION_FAULT)
				|| (BMS_Param.B2C_Warn.Serious_Warn2.Bits.Batt_InsulationFault == BATT_INSULATION_FAULT))
		{
			return BATT_FAULT;
		}
	if((BMS_Param.B2C_Warn.Slight_Warn1.u8Word1All == BMS_NO_ALARM)
		&& (BMS_Param.B2C_Warn.Slight_Warn2.u8Word1All == BMS_NO_ALARM)
		 && (BMS_Param.B2C_Warn.Medium_Warn1.u8Word1All == BMS_NO_ALARM)
		  && (BMS_Param.B2C_Warn.Medium_Warn2.u8Word1All == BMS_NO_ALARM)
		   && (BMS_Param.B2C_Warn.Serious_Warn1.u8Word1All == BMS_NO_ALARM)
		    && (BMS_Param.B2C_Warn.Serious_Warn2.u8Word1All == BMS_NO_ALARM))
		{
			return BATT_NORMAL;
		}
			return UNKNOWN;
}


/***************************************************************************
函数名:Class_BMS::App_Get_BattDataInfo
功能描述:上报电池实时数据
输入参数：电池实时数据结构体指针
返回值：无
使用说明：
****************************************************************************/

void Class_BMS::App_Get_BattDataInfo(_Batt_DataInfoDef *pData)
{
	pData->BattAllowedChgCurrMax	 = (B2C_Status1.u16BattAllowedChgCurrMax ) / 10;
	pData->BattAllowedDisChgCurrMax	 = (B2C_Status1.u16BattAllowedDisChgCurrMax) /10;
	pData->BattGroupVolt			 = (B2C_Status1.u16BattGroupVolt) / 10;
	pData->BattGroupCurr			 = (B2C_Status1.u16BattGroupCurr) / 10;
	pData->BattAllowedChgPowerMax	 = (B2C_Status2.u16BattAllowedChgPowerMax) / 10;
	pData->BattAllowedDisChgPowerMax = (B2C_Status2.u16BattAllowedDisChgPowerMax) / 10;
	pData->BattAllowedDisChgVoltMin	 = (B2C_Status2.u16BattAllowedDisChgVoltMin) / 10;	
	pData->Group_SOC				 = B2C_Status2.Group_SOC;
	pData->Group_SOH				 = B2C_Status2.Group_SOH;

}

/***************************************************************************
函数名:Class_BMS::App_RespMaxChgCurr
功能描述:不同SOC、温度下，BMS请求充电电流值
输入参数：无
返回值：最大充电电流
使用说明：
****************************************************************************/

float Class_BMS::App_RespMaxChgCurr(void)
{
	float  MaxChgCurr;
	int8_t Temp;
	int8_t Soc;
//	uint8_t i,j;
	Temp = BMS_Param.B2C_Status3.Env_Temp - 40;
	Soc = B2C_Status2.Group_SOC;
	if(Temp <= -10 || Temp > 55)
	{
	/*	for (i = 0; i < 21; i++)
		{
			for (j = 0; j < 14; j++)
			{
				MaxChgCurr = BmsSeqChgCurr[i][0]*C;
			}
		}
	*/	
		MaxChgCurr = 0;
		return MaxChgCurr;
	}
	if(Temp > -5 && Temp <= 5)
	{
		MaxChgCurr = 0.116*C;
		return MaxChgCurr;
	}
	
	if(Temp > 5 && Temp <= 10)
	{
		MaxChgCurr = 0.372*C;
		return MaxChgCurr;
	}
	
	if(Temp > 10 && Temp <= 15)
	{
		MaxChgCurr = 0.5*C;
		return MaxChgCurr;
	}
	
	if(Temp > 45 && Temp <= 48)
	{
		MaxChgCurr = 0.8*C;
		return MaxChgCurr;
	}
	
	if(Temp > 48 && Temp <= 55)
	{
		MaxChgCurr = 0.279*C;
		return MaxChgCurr;
	}

	if((Temp > -10 && Temp <= -5) && (Soc >= 75 && Soc <= 100))
	{
		MaxChgCurr = 0.1*C;
		return MaxChgCurr;
	}

	if((Temp > -10 && Temp <= -5) && (Soc >= 0 && Soc < 75))
	{
		MaxChgCurr = 0;
		return MaxChgCurr;
	}

	if((Temp > 15 && Temp <= 20) && (Soc >= 85 && Soc <= 100))
	{
		MaxChgCurr = 0.75*C;
		return MaxChgCurr;
	}

	if((Temp > 15 && Temp <= 20) && (Soc >= 0 && Soc < 85))
	{
		MaxChgCurr = 1*C;
		return MaxChgCurr;
	}

	if((Temp > 20 && Temp <= 45) && (Soc >= 85 && Soc <= 100))
	{
		MaxChgCurr = 0.8*C;
		return MaxChgCurr;
	}

	if((Temp > 20 && Temp <= 45) && (Soc >= 0 && Soc < 85))
	{
		MaxChgCurr = 1*C;
		return MaxChgCurr;
	}
	return -1;
}


/***************************************************************************
函数名:Class_BMS::App_RespContactorStatus
功能描述:反馈电池接触器状态给开关机处理
输入参数：无
返回值：电池接触器状态
使用说明：根据高特电子协议，目前此位暂未使用（即不能反馈接触器状态）
****************************************************************************/


uint8_t Class_BMS::App_RespContactorStatus(void)
{
	return Contrl_Fram.PCS_Contrl_CMD.Bits.Contactor_Status;        //此位暂未使用
}



/***************************************************************************
函数名:Class_BMS::App_BmsOffLineCheck
功能描述:10s检测一次BMS到PCU的通信链路是否正常
输入参数：无
返回值：无
使用说明：
****************************************************************************/

void Class_BMS::App_BmsOffLineCheck(void)
{
	if(g_st_TimerFlag.b10ms == 1)
	{
		m_u16_BMSOfflineCnt++;
		if (m_u16_BMSOfflineCnt >= 1000)
		{
			m_u16_BMSOfflineCnt = 1000;
			m_i16_EnergyBattOnline = NO;
		}
	}		
}

/***************************************************************************
函数名:Class_BMS::App_ReportBmsLinkStatus
功能描述:上报BMS到PCU的通信链路状态
输入参数：无
返回值：通信链路状态
使用说明：
****************************************************************************/

uint8_t Class_BMS::App_ReportBmsLinkStatus(void)
{
	if (m_i16_EnergyBattOnline == NO)
	{
		return B2C_LINK_ABNORMAL;
	}
	else
		return B2C_LINK_NORMAL;

}


