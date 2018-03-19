
#include "Publics.h"

uint16_t	ExchgDebug[10];
#define     MAC_ID_DEBUG    0x22
#define FRAME_OFFSET_OF(type, member)		((int32_t)&((type *)0)->member)
#define CHARGELIFECOMAND(index)				(((index)==0)?0x5a:0xa5)

/*************************************************************************************************************
	数据交互包
**************************************************************************************************************/

const    _CanBMSAppMsgDef   ToBattMsgObjs[] =
{
	{PGN_C2B_ContrlCMD, 6, 8, 200, MAC_ID_BATTERY0, MAC_ID_PCS, 1, (uint8_t*)&objBMS.Contrl_Fram}		//200ms

};

/*const    _CanMstMonAppMsgDef   ToMstMonMsgObjs[] =
{
	{PGN_SYSVERSION, 6, 8, 200, MAC_ID_MASTERMON, MAC_ID_PCS,1, 0, (uint8_t*)&objEEPROM.m_VersionManage} //200ms

};*/
const    _CanMstMonAppMsgDef   ToMstMonMsgObjs[] =
{
	{PGN_CHARGE_RESPONE_UPDATE_REPORT,6,1,250,MAC_ID_MASTERMON,MAC_ID_StoreEnergyBOX1,0,0,(uint8_t*)&objUpgrade.m_St_UpgdIntf.u8UpgdCmdRp} 
};

const _CanInnerAppMsgDef ToBGMonitorMsgObjs[] =
{
	{0,NULL},

};


#define TO_BATTERYTY_MSG_OBJ_LEN 	LEN_ARRAY(ToBattMsgObjs)
#define TO_MstMonitor_MSG_OBJ_LEN 	LEN_ARRAY(ToMstMonMsgObjs)


/***************************************************************************
函数名:Class_CanDataExchg::Dat_Init
功能描述：BMS消息数组初始化
输入参数：无
返回值：无
使用说明：
****************************************************************************/

void Class_CanDataExchg::Dat_Init(void)
{
	int16_t i;

	m_u8_TotalPackNums = 0;
	m_u32_PGN = 0;
	m_i16_NULL=0;
	 m_u8_ModInfo1_SendFlag=0;
	 m_u8_ModInfo2_SendFlag=0;
	 m_u8_ModInfo2_ReqFlag=0;
	m_u8_Mod_LongFrame_ID=0;
	m_u8_MsgSendFlag = 0;


	for(i = 0;i<TO_BATTERYTY_MSG_OBJ_LEN;i++)
	{
		m_ToBatteryMsg.MsgObj[i] = ToBattMsgObjs[i];
	}
	m_ToBatteryMsg.datalen = TO_BATTERYTY_MSG_OBJ_LEN;


	for(i = 0;i<TO_MstMonitor_MSG_OBJ_LEN;i++)
	{
		m_st_ToMstMonMsgObj.MsgObj[i] = ToMstMonMsgObjs[i];
	}
	m_st_ToMstMonMsgObj.msgLen = TO_MstMonitor_MSG_OBJ_LEN;

	//-----------------------------------------------------------------
	m_u16_ArrayCnt = 0;

}


/***************************************************************************
函数名:Class_CanDataExchg::TransMsgToInner
功能描述：发送设置请求消息给内部总线PCU
输入参数：内部消息帧、源地址、目的地址
返回值：无
使用说明：
****************************************************************************/

void Class_CanDataExchg::TransMsgToInner(_CanInnerAppMsgDef *msg,uint8_t srcAddr,uint8_t desAddr)
{
	int16_t ret;
	_CanInnerAppMsgDef		*tempMsg = msg;

	tempMsg->u16MACId = srcAddr;
	tempMsg->u16DestId = desAddr;

	ret = objCanInnerProtocol.MsgXmitProc(tempMsg);
	if (ret == -1)
	{
		//错误处理
	}
}

/***************************************************************************
函数名:Class_CanDataExchg::TransSetReqCmdToMastorMon
功能描述：发送设置请求命令给集控
输入参数：集控消息数据包、目的地址
返回值：无
使用说明：
****************************************************************************/

void Class_CanDataExchg::TransSetReqCmdToMastorMon(_CanMstMonAppMsgDef *msg,uint16_t desAddr)
{

	int16_t ret;
  	  _CanMstMonAppMsgDef      *tempMsg= msg;

	ret = objCanMasterMonPrtcl.MsgXmitProc(tempMsg);
	if (ret == -1)
	{
		//错误处理
	}
}

/***************************************************************************
函数名:Class_CanDataExchg::TransSetReqCmdToBMS
功能描述：发送设置请求命令给BMS
输入参数：BMS消息数据包、目的地址
返回值：无
使用说明：
****************************************************************************/

void Class_CanDataExchg::TransSetReqCmdToBMS(_CanBMSAppMsgDef *msg)
{
	int16_t ret;
	_CanBMSAppMsgDef      *tempMsg= msg;

	ret = objCanBMSProtocol.MsgXmitProc(tempMsg);
	if (ret == -1)
	{
		//错误处理
	}
}

/***************************************************************************
函数名:Class_CanDataExchg::TransDataToMastorMon
功能描述：发送数据给集控
输入参数：源地址、PGN、数据长度、待发送的数据指针
返回值：无
使用说明：
****************************************************************************/

void Class_CanDataExchg::TransDataToMastorMon(uint16_t srcAddr,uint16_t u16PGN,int16_t dataLen,uint8_t *pData)
{
	uint8_t i,j;
	//int16_t ret;
	uint8_t u8temp[8];
	_CanMstMonAppMsgDef tempdata;

	if (dataLen < 1 || dataLen > 8)
	{
		return;
	}
	if(pData == NULL)
	{
		return ;
	}
	for (i = 0;i < m_st_ToMstMonMsgObj.msgLen;i++)
	{
		if (m_st_ToMstMonMsgObj.MsgObj[i].u16PGN == u16PGN)
		{
			tempdata = m_st_ToMstMonMsgObj.MsgObj[i];
			tempdata.pDataPtr = u8temp;
			tempdata.u16MACId = srcAddr;
			for (j = 0;j < dataLen;j++)
			{
				tempdata.pDataPtr[j] = pData[j];
			}
			TransSetReqCmdToMastorMon(&tempdata,MAC_ID_MASTERMON);
			break;
		}
	}
}



/***************************************************************************
函数名：Class_CanDataExchg::GetToBMSMsg
功能描述：PCU将命令帧发送到BMS
输入参数：BMS应用层数据包结构体指针
返回值：有无消息发送标志
使用说明：
****************************************************************************/

int16_t    Class_CanDataExchg::GetToBMSMsg(OUT _CanBMSAppMsgDef *pApdu)
{
	int16_t 	i;
	int16_t	newMsg = E_NON_MSG_XMIT;

	for (i = 0; i < m_ToBatteryMsg.datalen; i++)
		{
			if (TRUE == m_ToBatteryMsg.MsgObj[i].u16XmitActive)
			{
				*pApdu = m_ToBatteryMsg.MsgObj[i];
				newMsg = E_HAVE_MSG_XMIT;
				m_ToBatteryMsg.MsgObj[i].u16XmitActive = FALSE;
				break;
			}
		}

	return newMsg;
}


/***************************************************************************
函数名：Class_CanDataExchg::GetToCCUMsg
功能描述：PCU将命令帧发送到CCU
输入参数：_CanInnerAppMsgDef *pApdu
返回值：有无消息发送标志
使用说明：
****************************************************************************/

int16_t    Class_CanDataExchg::GetToCCUMsg(OUT _CanInnerAppMsgDef *pApdu)
{

	int16_t	newMsg = E_NON_MSG_XMIT;

	return newMsg;
}

/***************************************************************************
函数名：Class_CanDataExchg::GetToMastorMonMsg
功能描述：PCU将命令帧发送到集控
输入参数：集控应用层数据包结构体指针
返回值：有无消息发送标志
使用说明：
****************************************************************************/

int16_t    Class_CanDataExchg::GetToMastorMonMsg(OUT _CanMstMonAppMsgDef *pApdu)
{
	int16_t 	i;
	int16_t	newMsg = E_NON_MSG_XMIT;
	for (i = 0; i < m_st_ToMstMonMsgObj.msgLen; i++)
	{
		if (TRUE == m_st_ToMstMonMsgObj.MsgObj[i].u16XmitActive)
		{
			*pApdu = m_st_ToMstMonMsgObj.MsgObj[i];
			newMsg = E_HAVE_MSG_XMIT;
			m_st_ToMstMonMsgObj.MsgObj[i].u16XmitActive = FALSE;
			break;
		}
	}

	return newMsg;
}

/***************************************************************************
函数名：Class_CanDataExchg::GetBGMonitorMsg
功能描述：PCU将命令帧发送到后台
输入参数：_CanInnerAppMsgDef *pApdu
返回值：有无消息发送标志
使用说明：
****************************************************************************/

int16_t    Class_CanDataExchg::GetBGMonitorMsg(OUT _CanInnerAppMsgDef *pApdu)
{
	int16_t i;
	int16_t	newMsg = E_NON_MSG_XMIT;

	for (i = 0; i < m_st_ToBGMonitorMsgObj.msgLen; i++)
	{
		if (TRUE == m_st_ToBGMonitorMsgObj.MsgObj[i].u16XmitActive)
		{
			*pApdu = m_st_ToBGMonitorMsgObj.MsgObj[i];
			newMsg = E_HAVE_MSG_XMIT;
			m_st_ToBGMonitorMsgObj.MsgObj[i].u16XmitActive = FALSE;
			break;
		}
	}

	return newMsg;
}

/***************************************************************************
函数名：Class_CanDataExchg::RecvBMSMsgProc
功能描述：收到BMS数据并解析处理
输入参数：BMS应用层数据包结构体指针
返回值：有无消息发送标志
使用说明：
****************************************************************************/

int16_t   Class_CanDataExchg::RecvBMSMsgProc(_CanBMSAppMsgDef *pApdu)
{

	int16_t newMsg = E_NON_MSG_XMIT;


//	_CanBMSAppMsgDef *tempData = pApdu;

	objBMS.App_MsgDispatch(pApdu);		//收到BMS数据并解析
		
	objBMS.g_BmsHaveMsgXimt = E_HAVE_MSG_XMIT;	//BMS有消息上报

	return newMsg;
}

/***************************************************************************
函数名：Class_CanDataExchg::RecvInnerEngBattMsg
功能描述：收到电池消息处理
输入参数：_CanInnerAppMsgDef *pApdu
返回值：有无消息发送标志
使用说明：
****************************************************************************/

int16_t Class_CanDataExchg::RecvInnerEngBattMsg(_CanInnerAppMsgDef *pApdu)
{

	int16_t newMsg = E_NON_MSG_XMIT;



//	if(pApdu->u16DestId >= MAC_ID_DCACBOX1&&pApdu->u16DestId <= MAC_ID_DCACBOX5)
	{

		switch(pApdu->u16PGN)
		{
       		
			case PGN_SLAVE_BATT_STATUS:
				/*objStoredEnergy.m_u16_DCDCslaveStatus =  (*((uint16_t*)(pApdu->pDataPtr)));
				objStoredEnergy.m_u16_DCDCslaveVolt =  (*((uint16_t*)(&pApdu->pDataPtr[2])));
				objStoredEnergy.m_u16_DCDCslaveCurr =  (*((uint16_t*)(&pApdu->pDataPtr[4])));
                                */
			break;
		}

	}

	return newMsg;



}

/***************************************************************************
函数名：Class_CanDataExchg::RecvMonsterToStoredEnergyBoxMsg
功能描述：集控下发储能柜消息解析处理
输入参数：集控应用层数据包结构体指针
返回值：消息发送标志
使用说明：
****************************************************************************/

int16_t  Class_CanDataExchg::RecvMonsterToStoredEnergyBoxMsg(_CanMstMonAppMsgDef *pApdu)
{
	uint16_t temp;
	uint16_t scuCMD;
	int16_t newMsg = E_NON_MSG_XMIT;
	uint8_t relaycomand;
        uint8_t data[8];

	_CanMstMonAppMsgDef tempData;
	tempData.pDataPtr = data;
	tempData = *pApdu;


	if(pApdu->u16DestId >= MAC_ID_StoreEnergyBOX1&&pApdu->u16DestId <= MAC_ID_StoreEnergyBOX30)
	{
	
		switch(pApdu->u16PGN)
		{
       		//ACDC/DCDC开关机命令
			case PGN_MODULE_CONTROL:
				
				objCanDataPool.m_i16_TurnOnOffAddress = pApdu->pDataPtr[0];//模块地址
				scuCMD =  (pApdu->pDataPtr[1] << 8) | pApdu->pDataPtr[2];
				switch(scuCMD)
				{
					case pcu_set_on_off:

						ObjPcuOnOffCtrl.pcuWorkState.systemTurnOnOffScuCtrl 
							= (pApdu->pDataPtr[3] << 8) | pApdu->pDataPtr[4];//开关机命令
						
						break;
					case pcu_set_change:
						 if((pApdu->pDataPtr[3] != 0x00) || (pApdu->pDataPtr[4] != 0x00))
                         {
                            ObjPcuOnOffCtrl.pcuWorkState.systemChargeStateScuCtrl 
								= (pApdu->pDataPtr[3] << 8) | pApdu->pDataPtr[4];//充电命令
                         }
					
						break;
					case pcu_set_on_offline:
						if((pApdu->pDataPtr[3] != 0x00) || (pApdu->pDataPtr[4] != 0x00))
						{
						   ObjPcuOnOffCtrl.pcuWorkState.systemOnOfflineScuCtrl 
							   = (pApdu->pDataPtr[3] << 8) | pApdu->pDataPtr[4];//离网并网命令
						}

					
						break;
					case pcu_set_acdc_workmode:
						if((pApdu->pDataPtr[3] != 0x00) || (pApdu->pDataPtr[4] != 0x00))
						{
						   	temp = (pApdu->pDataPtr[3] << 8) | pApdu->pDataPtr[4];//离网并网命令
							if(temp >= 1 && temp <= 6)//工作模式只能在1到6之间
							{
								ObjPcuOnOffCtrl.pcuWorkState.systemAcdcWorkMode = temp;
							}
							   
						}

						break;
					case pcu_set_dcdc_workmode:
						if((pApdu->pDataPtr[3] != 0x00) || (pApdu->pDataPtr[4] != 0x00))
						{
						   	temp = (pApdu->pDataPtr[3] << 8) | pApdu->pDataPtr[4];//离网并网命令
							if(temp >= 1 && temp <= 6)//工作模式只能在1到2之间
							{
								ObjPcuOnOffCtrl.pcuWorkState.systemDcdcWorkMode = temp;
							}
							   
						}

						break;
					case pcu_set_acdc_outvolt:
						if((pApdu->pDataPtr[3] != 0x00) || (pApdu->pDataPtr[4] != 0x00))
						{
						   ObjPcuOnOffCtrl.pcuWorkState.systemAcdcOutVolt 
							   = (pApdu->pDataPtr[3] << 8) | pApdu->pDataPtr[4];//离网并网命令
						   if(ObjPcuOnOffCtrl.pcuWorkState.systemAcdcOutVolt < 500)//电压值不能设置小于500，大于900
						   {
								ObjPcuOnOffCtrl.pcuWorkState.systemAcdcOutVolt = 500;
						   }
						   else if(ObjPcuOnOffCtrl.pcuWorkState.systemAcdcOutVolt > 900)
						   {
								ObjPcuOnOffCtrl.pcuWorkState.systemAcdcOutVolt = 900;
						   }
						  
						   ObjPcuOnOffCtrl.pcuWorkState.systemAcdcOutVolt = ObjPcuOnOffCtrl.pcuWorkState.systemAcdcOutVolt*10;
						   
						}

						break;
					case pcu_set_acdc_pow:
						//ObjPcuOnOffCtrl.pcuWorkState.systemDisChargePower 
						//	= ((pApdu->pDataPtr[3] << 8) | pApdu->pDataPtr[4])*10;//ACDC功率设置，只在并网放电模式有效

						break;
					case pcu_set_dcdc_outvolt:

						break;
					case pcu_set_dcdc_maxpow:

						break;
					case pcu_set_dcdc_current:
						ObjChargeCtrl.acdc_dcdc_debugset.dcdcCurrent 
							= ((pApdu->pDataPtr[3] << 8) | pApdu->pDataPtr[4])*10;//设置DCDC电流

						break;
					case pcu_set_system_pow:
						ObjPcuOnOffCtrl.pcuWorkState.systemDisChargePower 
							= ((pApdu->pDataPtr[3] << 8) | pApdu->pDataPtr[4])*10;//系统功率设置，只在并网放电模式有效
						break;

					case pcu_set_restart_start:
						ObjPcuOnOffCtrl.pcuWorkState.systemRestart 
							= (pApdu->pDataPtr[3] << 8) | pApdu->pDataPtr[4];//设置系统重启命令

						break;
					default:
					
						break;
				
				}
	

			break;

			//PCS柜设置
			case PGN_PCS_CONFIG:
				objPcsConfigManage.RecvMstMonMsgDispatch(&tempData);//@todo 验证
				break;
	
			case PGN_RTCTIME_SYN://RTC时间同步
			/*				
				Calendar.Year.half.hbyte = pApdu->pDataPtr[0];	//传过去暂时没用
				Calendar.Year.half.lbyte = pApdu->pDataPtr[1];
				Calendar.Month = pApdu->pDataPtr[2];
				Calendar.Week = pApdu->pDataPtr[3];				//传过去暂时没用
				Calendar.Day = pApdu->pDataPtr[4];
				Calendar.Hour = pApdu->pDataPtr[5];
				Calendar.Minute = pApdu->pDataPtr[6];
				Calendar.Second = pApdu->pDataPtr[7];
				WriteRTCTime(Calendar);
			*/
			//采用软件计时
				objSystemRTC.s_SysCalendar.Year.half.hbyte = pApdu->pDataPtr[0];
				objSystemRTC.s_SysCalendar.Year.half.lbyte = pApdu->pDataPtr[1];
				objSystemRTC.s_SysCalendar.Month		   = pApdu->pDataPtr[2];
				objSystemRTC.s_SysCalendar.Day			   = pApdu->pDataPtr[3];
				objSystemRTC.s_SysCalendar.Hour			   = pApdu->pDataPtr[4];
				objSystemRTC.s_SysCalendar.Minute		   = pApdu->pDataPtr[5];
				objSystemRTC.s_SysCalendar.Second		   = pApdu->pDataPtr[6];	  
				break;
				
			case PGN_MSTMON_REQUEST:
                                
				scuCMD = (pApdu->pDataPtr[1] << 8) | pApdu->pDataPtr[2];
				switch (scuCMD)
				{
					case PGN_PCS_DATA2:
						if(pApdu->pDataPtr[3] == 0x01)
						{
							objCanDataPool.m_u8_PCSinfo2Flag = 0;	//请求PGN_PCS_DATA2帧，停止标志置0
							objCanMstMonLongFrame.Dat_ClearQueue();
							objMemory.Dat_CleanMemBlock();
						}
						else if(pApdu->pDataPtr[3] == 0x02)
						{
							objCanDataPool.m_u8_PCSinfo2Flag = 1;	//已收到PGN_PCS_DATA2帧，停止标志置1
							objCanMstMonLongFrame.Dat_ClearQueue();
							ObjPcuOnOffCtrl.ClearEPOError();
						}
					default:
							break;
				}
				break;

			//设置ACDC参数
			case PGN_SET_ACDCPARA:
				uint16_t acdcoffset;
				objCanDataPool.m_i16_SetACDCFlag = 1;
				objCanDataPool.m_i16_ACDCID = pApdu->pDataPtr[0] - 8;  //将模块地址改为modbus地址
				acdcoffset = BIG_LITTLE_TRANVERSE16((int16_t)(*(int16_t *)&pApdu->pDataPtr[1]));	//根据offset选择设置的寄存器
				objCanDataPool.m_i16_ACDCSetData =BIG_LITTLE_TRANVERSE16((int16_t)(*(int16_t *)&pApdu->pDataPtr[3]));	//设置参数	  
				if((acdcoffset >= 0)&&(acdcoffset <= 12))
					{
						switch(acdcoffset)
						{
							case 0x0000:
								objCanDataPool.m_i16_ACDCSetID = WORKMODE;
								break;
							case 0x0001:
								objCanDataPool.m_i16_ACDCSetID = VOLTSETTING;
								break;
							case 0x0002:
								objCanDataPool.m_i16_ACDCSetID = POWLIMIT;
								break;
							case 0x0003:
								objCanDataPool.m_i16_ACDCSetID = ENERGY_P;
								break;
							case 0x0004:
								objCanDataPool.m_i16_ACDCSetID = POWER_RATE;
								break;
							//case 0x0005:
								//objCanDataPool.m_i16_ACDCSetID = RESERVED1;
								//break;
							case 0x0006:
								objCanDataPool.m_i16_ACDCSetID = REACTPOWMODE;
								break;
							case 0x0007:
								objCanDataPool.m_i16_ACDCSetID = REACTPOWSET;
								break;
							case 0x0008:
								objCanDataPool.m_i16_ACDCSetID = REACTPOWFACT;
								break;
							case 0x0009:
								objCanDataPool.m_i16_ACDCSetID = SING_PARACONNECT;
								break;
							case 0x000a:
								objCanDataPool.m_i16_ACDCSetID = MASTERORSLAVE;
								break;
							//case 0x000b:
								//objCanDataPool.m_i16_ACDCSetID = RESERVED2;
								//break;
							case 0x000c:
								objCanDataPool.m_i16_ACDCSetID = PARACONNECTADDR;
								break;
						}
				}
				break;
			//设置DCDC参数
			case PGN_SET_DCDCPARA:
				uint16_t dcdcoffset;
				objCanDataPool.m_i16_SetDCDCFlag = 1;
				objCanDataPool.m_i16_DCDCID = pApdu->pDataPtr[0] - 25;	//将模块地址改为modbus地址
				dcdcoffset = BIG_LITTLE_TRANVERSE16((int16_t)(*(int16_t *)&pApdu->pDataPtr[1]));	//根据offset选择设置的寄存器
				objCanDataPool.m_i16_DCDCSetData =BIG_LITTLE_TRANVERSE16((int16_t)(*(int16_t *)&pApdu->pDataPtr[3]));	//设置参数
				if((dcdcoffset >= 0)&&(dcdcoffset<= 3))
				{
				switch (dcdcoffset)
					{
						case 0x0000:	
							objCanDataPool.m_i16_DCDCSetID = WORKMODE;
							break;
						case 0x0001:
							objCanDataPool.m_i16_DCDCSetID = VOLTSETTING;
							break;						
						case 0x0002:
							objCanDataPool.m_i16_DCDCSetID = POWLIMIT;
							break;
						case 0x0003:
							objCanDataPool.m_i16_DCDCSetID = 0x1003;
							break;
					}
				}
				break;
			case PGN_CCU_SET_REQ_PARAM:
				if (pApdu->pDataPtr[0] == 3)
				{
					App_SetCCUCanID(pApdu);
				}
				else if (pApdu->pDataPtr[0] == 4)
				{
					App_GetCCUCanID(pApdu);
				}
				newMsg = E_HAVE_MSG_XMIT;
				break;
			
		}

	}
	
	if(newMsg == E_HAVE_MSG_XMIT)
	{
		if((pApdu->u16PGN == PGN_CHARGE_START)
			|| (pApdu->u16PGN == PGN_CHARGE_STOP)
			|| (pApdu->u16PGN == PGN_EMERGENCY_STOP)
			|| (pApdu->u16PGN == PGN_DISCHARGE_START)
			|| (pApdu->u16PGN == PGN_DISCHARGE_STOP))
		{
			pApdu->u16PGN = PGN_TOMASTMON_CONFIRM;
			pApdu->u16DataLen = 8;
			pApdu->u16DestId = MAC_ID_MASTERMON;
			pApdu->u16MACId =  tempData.u16DestId;
			pApdu->u16Priority = 6;

			pApdu->pDataPtr[0] = relaycomand;
			pApdu->pDataPtr[1] = 0x00;
			pApdu->pDataPtr[2] = 0xff;
			pApdu->pDataPtr[3] = 0xff;
			pApdu->pDataPtr[4] = 0xff;
			pApdu->pDataPtr[5] = 0;
			pApdu->pDataPtr[6] = (uint8_t)(tempData.u16PGN >> 8);
			pApdu->pDataPtr[7] = (uint8_t)(tempData.u16PGN);
		}
		else
		{
			pApdu->u16DestId = MAC_ID_MASTERMON;
			pApdu->u16MACId =  tempData.u16DestId;
		}
	}
	
	return newMsg;
}

/***************************************************************************
函数名：Class_CanDataExchg::MasterToInner
功能描述：集控到内部CAN的消息发送
输入参数：集控应用层数据包结构体指针
返回值：无
使用说明：
****************************************************************************/

void Class_CanDataExchg::MasterToInner(_CanMstMonAppMsgDef *pApdu)
{
	_CanInnerAppMsgDef  tempdata;
//	uint16_t desAddr = 0;
	tempdata.u16DestId = pApdu->u16DestId;
	tempdata.u16DataLen= pApdu->u16DataLen;
	tempdata.u16MACId = pApdu->u16MACId;
	tempdata.pDataPtr = pApdu->pDataPtr;
	tempdata.u16MsgSendPeriod= pApdu->u16MsgSendPeriod;
	tempdata.u16XmitActive= TRUE;
	tempdata.u16Priority = pApdu->u16Priority;
	tempdata.u16PGN = pApdu->u16PGN;
	//TransSetReqCmdToInner( &tempdata,  desAddr);
	TransMsgToInner(&tempdata,objPcuModel.m_i16_SelfAddr,tempdata.u16DestId);
}

/***************************************************************************
函数名：Class_CanDataExchg::App_SetCCUCanID
功能描述：设置CCU的ID
输入参数：集控应用层数据包结构体指针
返回值：无
使用说明：
****************************************************************************/

void Class_CanDataExchg::App_SetCCUCanID(_CanMstMonAppMsgDef *pApdu)
{
	int16_t tempID = pApdu->pDataPtr[2];

	_CanMstMonAppMsgDef  tempdata;
	uint8_t temp[4];
//	uint8_t	addr = MAC_ID_MASTERMON;
	tempdata.pDataPtr = temp;
	
	tempdata = *pApdu;
	
	tempdata.u16MACId = pApdu->u16DestId;
	tempdata.u16DestId = MAC_ID_MASTERMON;
	
	tempdata.pDataPtr[0] = SET_PDU_ID;
	tempdata.pDataPtr[1] = pApdu->pDataPtr[1];
	tempdata.pDataPtr[2] = pApdu->pDataPtr[2];
	tempdata.pDataPtr[3] = pApdu->pDataPtr[3];

	if( (tempID >= MAC_ID_StoreEnergyBOX1) && (tempID <= MAC_ID_StoreEnergyBOX30))//CCU
	{
		objPcuModel.m_i16_SelfAddr = tempID;
		objEEPROM.App_WriteWord(PCU_ADDR,tempID);
	//	TransSetReqCmdToMastorMon(&tempdata,addr);
	}
}

/***************************************************************************
函数名：Class_CanDataExchg::App_GetCCUCanID
功能描述：读取CCU的ID
输入参数：集控应用层数据包结构体指针
返回值：无
使用说明：
****************************************************************************/

void Class_CanDataExchg::App_GetCCUCanID(_CanMstMonAppMsgDef *pApdu)
{
	_CanMstMonAppMsgDef  tempdata;
	uint8_t temp[4];
//	uint8_t addr = MAC_ID_MASTERMON;
	tempdata.pDataPtr = temp;
	
	tempdata = *pApdu;
	
	tempdata.u16MACId = pApdu->u16DestId;
	tempdata.u16DestId = MAC_ID_MASTERMON;
	
	tempdata.pDataPtr[0] = GET_PDU_ID;
	tempdata.pDataPtr[1] = 0;
	tempdata.pDataPtr[2] = objPcuModel.m_i16_SelfAddr;

	tempdata.u16DataLen = 3;


	//TransSetReqCmdToMastorMon(&tempdata,addr);
}



