/*==========================================================*/
/* Title		:	Obj_FailManager.cpp						*/
/* Description	: 	systerm fault mannger					*/
// Author		:	zhangpeiqi								*/
/* Date			:	2017.11.14								*/
/*==========================================================*/

#include "Publics.h"
#include <String.h>

int16_t FailDebug[3] = {0,0,0};

/*===========================================================================*
 *  函数名称：FailManager::Dat_Init(void)
 *  功能描述：故障管理初始化
 *  输入参数：none
 *  返回值：none
 *  使用说明： 
 *==========================================================================*/
void Class_FailManager::Dat_Init(void)
{
	uint8_t index;
	m_u64_FailState = 0;
	m_u64_FailStateBak = 0;
	m_u16_ModuleFailState = 0;
	//@todo 模块ID自检
	_GetConfigParameterDef pcsConfig;
	objPcsConfigManage.GetPcsConfigInfo(&pcsConfig);
	PCSData2.BatteryNum = 1;
	PCSData2.ModulelStage = pcsConfig.ModuleLevel;
	PCSData2.PCUAddr = pcsConfig.PCUAddr;
	PCSData2.PCUHardVer = 2;
	PCSData2.PCUSoftVer[0] = 1; 
	PCSData2.PCUSoftVer[1] = 0; 
	PCSData2.PCUSoftVer[2] = 1; 
	PCSData2.ACDCNum = pcsConfig.ACModuleNum;
	PCSData2.DCDCNum = pcsConfig.DCModuleNum;
	PCSData2.DCMeter = pcsConfig.DCMeter;
	PCSData2.ACMeter = pcsConfig.ACMeter;
	PCSData2.ForcerNum = pcsConfig.CabinetType;
	PCSData2.Volt = pcsConfig.VoltOut;
	PCSData2.Freq = pcsConfig.Frequency;
	PCSData2.Elevation = pcsConfig.AltitudeRange;
	ObjPcuOnOffCtrl.getEPOError(&(PCSData2.epoErrorTime));
}

/*===========================================================================*
 *	函数名称：FailManager::App_vFailReport(uint64_t uFailName, uint16_t uFailValState)
 *	功能描述：上报故障状态（1为故障产生,0为故障消失）
 *	输入参数：uFailName:故障类型		uFailValState:系统故障状态
 *	返回值：none
 *	使用说明： 
 *==========================================================================*/
void Class_FailManager::App_vFailReport(uint64_t uFailName, uint16_t uFailValState)
{
	if(uFailValState != 0)//新故障产生
	{
		m_u64_FailState |= uFailName;
	}
	else//老故障消失
	{
		m_u64_FailState &= ~uFailName;
	}
}

/*===========================================================================*
 *	函数名称：FailManager::App_GetFailState(uint64_t uFailName)
 *	功能描述：获取故障状态
 *	输入参数：uFailName:故障类型
 *	返回值：1为故障发生/0为故障未发生
 *	使用说明： 
 *==========================================================================*/
uint64_t Class_FailManager::App_GetFailState(uint64_t uFailName)
{
	return(uFailName&m_u64_FailState);
}
/*===========================================================================*
 *	函数名称：FailManager::App_ModuleFailReport(uint16_t uFailName,uint16_t uFailValState)
 *	功能描述：上报故障状态（1为故障产生,0为故障消失）
 *	输入参数：uFailName:故障类型		uFailValState:模块故障状态
 *	返回值：none
 *	使用说明： 
 *==========================================================================*/
void Class_FailManager::App_ModuleFailReport(uint16_t uFailName,uint16_t uFailValState)
{
	if(uFailValState != 0)//新故障产生
	{
		m_u16_ModuleFailState |= uFailName;
	}
	else//老故障消失
	{
		m_u16_ModuleFailState &= ~uFailName;
	}
}
/*===========================================================================*
 *	函数名称：FailManager::App_GetModuleFailState(uint16_t moduleID)
 *	功能描述：获取模块故障状态
 *	输入参数：moduleID
 *	返回值：0为无报警也无故障，1为模块告警，2为模块故障
 *	使用说明： 
 *==========================================================================*/

uint8_t  Class_FailManager::App_GetModuleFailState(uint16_t moduleID)
{
	uint8_t idoffset;
	if((moduleID < ACDCMAXADDR) && (moduleID > ACDCMINADDR))
	{
		idoffset = moduleID - ACDCMINADDR;
		return(*(&(ModuleErrorState.ACDC1_State) + idoffset));
	}
	else if((moduleID < DCDCMAXADDR) && (moduleID > DCDCMINADDR))
	{
		idoffset = moduleID - DCDCMINADDR;
		return(*(&(ModuleErrorState.DCDC1_State) + idoffset));
	}
}

/*===========================================================================*
 *	函数名称：FailManager::App_GetFailStateMsg(void)
 *	功能描述：查询并上报新故障
 *	输入参数：none
 *	返回值：none
 *	使用说明： 
 *==========================================================================*/
void Class_FailManager::App_GetFailStateMsg(void)
{
	//获取模块运行故障,不上报集控
	Dat_GetModuleState();	
	//系统故障状态获取，上报集控
	//系统通讯故障获取
	Dat_GetCommFailState();	
	//获取系统交流侧故障
	Dat_GetSysACFault();
	//系统级故障获取
	Dat_GetSysLevelFault();
	//系统直流侧故障获取
	Dat_GetSysDCFault();
	//更新故障处理方法
	Dat_UpdataFailLevel();	
	//PCS柜信息1获取
	Dat_GetSysData();		
}

/*===========================================================================*
 *	函数名称：FailManager::Dat_GetModuleState(void)
 *	功能描述：查询并上报模块运行状态（不上报集控）
 *	输入参数：none
 *	返回值：none
 *	使用说明： 
 *==========================================================================*/
void Class_FailManager::Dat_GetModuleState(void)
{
	uint16_t  acdcnum,dcdcnum;	//模块计数
	Module_Run_State  acdcstate,dcdcstate;		//模块运行状态（其中1为告警，2为故障）
	//查询六个模块状态 0-2:ACDC 3-5:DCDC
	if(objStoredEnergy.m_st_ModuleInfo.ACModuleNum > 0)
	{
		for(acdcnum = 0;acdcnum < objStoredEnergy.m_st_ModuleInfo.ACModuleNum;acdcnum++)
		{
			acdcstate = objStoredEnergy.App_GetModuleState(objStoredEnergy.m_st_ModuleInfo.ACModeAddr[acdcnum]);
			if (acdcstate == MODULE_STATE_WARN)
			*(&(ModuleErrorState.ACDC1_State) + acdcnum) = 1;
			else if (acdcstate == MODULE_STATE_FAULT)
			*(&(ModuleErrorState.ACDC1_State) + acdcnum) = 2;
			//如果模块正常运行，消除故障信息
			else if (acdcstate == MODULE_STATE_RUN)
			{
				*(&(ModuleErrorState.ACDC1_State) + acdcnum) = 0;
			}
		}	
	}
	if(objStoredEnergy.m_st_ModuleInfo.DCModuleNum > 0)
	{
		for(dcdcnum = 0;dcdcnum < objStoredEnergy.m_st_ModuleInfo.DCModuleNum;dcdcnum++)
		{
			dcdcstate = objStoredEnergy.App_GetModuleState(objStoredEnergy.m_st_ModuleInfo.DCModeAddr[dcdcnum]);
			if (dcdcstate == MODULE_STATE_WARN)
			*(&(ModuleErrorState.DCDC1_State) + dcdcnum) = 1;
			else if (dcdcstate == MODULE_STATE_FAULT)
			*(&(ModuleErrorState.DCDC1_State) + dcdcnum) = 2;
			//如果模块正常运行，消除故障信息
			else if (dcdcstate == MODULE_STATE_RUN)
			{
				*(&(ModuleErrorState.DCDC1_State) + dcdcnum) = 0;
			}
		}
	}
}


/*===========================================================================*
 *	函数名称：FailManager::Dat_GetIOState(void)
 *	功能描述：查询并上报IO量故障
 *	输入参数：none
 *	返回值：none
 *	使用说明： 
 *==========================================================================*/
void Class_FailManager::Dat_GetIOState(void)
{
	uint8_t index;	//直流熔丝计数
	//uint8_t state;	//IO口电平
	//并联接触器、环境温度、水浸暂无
	/*for(index = 0;index < 5;index++)
	{
		state = BSP_DI_GetState(_DI_TypeDef (DI_EPO_FB + index));
		if (state == 0)
		App_vFailReport(FAIL_EMERGENCY_STOP << index, FAIL_ACK);	//输出告警信息
		else if (state == 1)
		App_vFailReport(FAIL_EMERGENCY_STOP << index, FAIL_RELEASE);	//清除故障
	}
	*/
	//风机
	if(BSP_DI_GetState(DI_FANHOT_FB))
	{
		App_vFailReport(FAIL_FAN_TEMP_FAULT,FAIL_RELEASE);
	}
	else
	{
		App_vFailReport(FAIL_FAN_TEMP_FAULT,FAIL_ACK);
	}	
	//急停
	/*if(BSP_DI_GetState(DI_EPO_FB))
	{
		App_vFailReport(FAIL_EMERGENCY_STOP,FAIL_RELEASE);
	}
	else
	{
		App_vFailReport(FAIL_EMERGENCY_STOP,FAIL_ACK);
	}*/	
/*	//门磁
	if(BSP_DI_GetState(DI_DOOR_FB))
	{
		App_vFailReport(FAIL_DOOR_FAULT,FAIL_RELEASE);
	}
	else
	{
		App_vFailReport(FAIL_DOOR_FAULT,FAIL_ACK);
	}
	*/
	//防雷
	if(BSP_DI_GetState(DI_PROTECT_FB))
	{
		App_vFailReport(FAIL_SPD_FAULT,FAIL_RELEASE);
	}
	else
	{
		App_vFailReport(FAIL_SPD_FAULT,FAIL_ACK);
	}
	//交流接触器
	if(BSP_DI_GetState(DI_ACBREAK_FB))
	{
		App_vFailReport(FAIL_ACCONTACTOR_FAULT,FAIL_ACK);

	}
	else
	{
		App_vFailReport(FAIL_ACCONTACTOR_FAULT,FAIL_RELEASE);
	}
	//@todo 水浸异常
	/*if(BSP_DI_GetState(水浸))
	{
		App_vFailReport(FAIL_FLOOD_FAULT,FAIL_RELEASE);
	}
	else
	{
		App_vFailReport(FAIL_FLOOD_FAULT,FAIL_ACK);
	}*/
	//@todo 直流接触器
	/*if(BSP_DI_GetState(直流接触器))
	{
		App_vFailReport(FAIL_DC_CONTACTOR,FAIL_RELEASE);
	}
	else
	{
		App_vFailReport(FAIL_DC_CONTACTOR,FAIL_ACK);
	}*/
	//@todo 交流断路器
	/*if(BSP_DI_GetState(交流断路器))
	{
		App_vFailReport(FAIL_ACBREAK_FAULT,FAIL_RELEASE);
	}
	else
	{
		App_vFailReport(FAIL_ACBREAK_FAULT,FAIL_ACK);
	}*/
	//@todo 变压器
	/*if(BSP_DI_GetState(变压器))
	{
		App_vFailReport(FAIL_TRANSFOR_TEMP_FAULT,FAIL_RELEASE);
	}
	else
	{
		App_vFailReport(FAIL_TRANSFOR_TEMP_FAULT,FAIL_ACK);
	}*/
	//直流熔丝
	for(index = 0;index < 4;index ++)
	{
		if(BSP_DI_GetState(_DI_TypeDef (DCBREAK1_FB + index)))
		{
			App_vFailReport(FAIL_DC_FUSE1_FAULT << index,FAIL_ACK);
		}
		else
		{
			App_vFailReport(FAIL_DC_FUSE1_FAULT << index,FAIL_RELEASE);
		}
	}
}
/*===========================================================================*
 *	函数名称：FailManager::Dat_GetCommFailState(void)
 *	功能描述：查询系统各种通信故障状态
 *	输入参数：none
 *	返回值：none
 *	使用说明： 
 *==========================================================================*/

void  Class_FailManager::Dat_GetCommFailState(void)
{
	Dat_GetACDCOffLineState();
	//如果为二级系统才获取DCDC通讯故障
	if(objStoredEnergy.m_st_ModuleInfo.ModuleLevel == 2)
	{
		Dat_GetDCDCOffLineState();
	}
	Dat_GetBMSOffLineState();
	Dat_GetOffLineState();
}
/*===========================================================================*
 *	函数名称：FailManager::Dat_GetBMSOffLineState(void)
 *	功能描述：查询并上报BMS离线状态
 *	输入参数：none
 *	返回值：none
 *	使用说明： 
 *==========================================================================*/
void Class_FailManager::Dat_GetBMSOffLineState(void)
{
//	uint8_t index;
	uint8_t state;
	//电池应有四组
	//for(index = 0;index < 4;index++)
	state = objBMS.App_ReportBmsLinkStatus();
	
	if (state == B2C_LINK_ABNORMAL)
	{
		//cnt = 0;
		App_vFailReport(FAIL_BATTERY1_COMM_ABNORMAL, FAIL_ACK);
	}
	else if (state == B2C_LINK_NORMAL)
	{
		//if (g_st_TimerFlag.b1s == 1)
		//{
		//	cnt++;
		//}
		//if (cnt >= 10)
		//{
			App_vFailReport(FAIL_BATTERY1_COMM_ABNORMAL, FAIL_RELEASE);
			//cnt = 0;
		//}
	}
}
/*===========================================================================*
 *	函数名称：FailManager::Dat_GetACDCOffLineState(void)
 *	功能描述：查询并上报模块离线状态
 *	输入参数：none
 *	返回值：none
 *	使用说明： 
 *==========================================================================*/

void Class_FailManager::Dat_GetACDCOffLineState(void)
{
	uint8_t index;
	uint8_t state;
	for(index = 0;index < objStoredEnergy.m_st_ModuleInfo.ACModuleNum;index++)
	{
		state = objStoredEnergy.APP_ReportModuleLinkStatus(objStoredEnergy.m_st_ModuleInfo.ACModeAddr[index]);
		if (state == MODULE_LINK_ABNORMAL)
		{
			App_vFailReport(FAIL_ACDC1_COMM_ABNORMAL << index, FAIL_ACK);
		}
		else
			App_vFailReport(FAIL_ACDC1_COMM_ABNORMAL << index, FAIL_RELEASE);
	}
}

/*===========================================================================*
 *	函数名称：FailManager::Dat_GetDCDCOffLineState(void)
 *	功能描述：查询并上报模块离线状态
 *	输入参数：none
 *	返回值：none
 *	使用说明： 
 *==========================================================================*/

void Class_FailManager::Dat_GetDCDCOffLineState(void)
{
	uint8_t index;
	uint8_t state;
	for(index = 0;index < objStoredEnergy.m_st_ModuleInfo.DCModuleNum;index++)
	{
		state = objStoredEnergy.APP_ReportModuleLinkStatus(objStoredEnergy.m_st_ModuleInfo.DCModeAddr[index]);
		if (state == MODULE_LINK_ABNORMAL)
		{
			App_vFailReport(FAIL_DCDC1_COMM_ABNORMAL << (index), FAIL_ACK);
		}
		else
			App_vFailReport(FAIL_DCDC1_COMM_ABNORMAL << (index), FAIL_RELEASE);
	}
}

/*===========================================================================*
 *	函数名称：FailManager::Dat_GetOffLineState(void)
 *	功能描述：温湿度、电表离线状态获取
 *	输入参数：none
 *	返回值：none
 *	使用说明： 
 *==========================================================================*/
void Class_FailManager::Dat_GetOffLineState(void)
{
	uint8_t acmeter,dcmeter,temp,humity,x = 0;//x为接口返回的值
	acmeter = x;
	dcmeter = x;
	temp = x;
	humity = x;
	if(acmeter == 1)
	{
		App_vFailReport(FAIL_ACMETER_COMM_ABNORMAL, FAIL_ACK);
	}
	else
	{
		App_vFailReport(FAIL_ACMETER_COMM_ABNORMAL, FAIL_RELEASE);
	}
	if(dcmeter == 1)
	{
		App_vFailReport(FAIL_DCMETER_COMM_ABNORMAL, FAIL_ACK);
	}
	else
	{
		App_vFailReport(FAIL_DCMETER_COMM_ABNORMAL, FAIL_RELEASE);
	}
	if(temp == 1)
	{
		App_vFailReport(FAIL_TEMP_COMM_ABNORMAL, FAIL_ACK);
	}
	else
	{
		App_vFailReport(FAIL_TEMP_COMM_ABNORMAL, FAIL_RELEASE);
	}
	if(humity ==1 )
	{
		App_vFailReport(FAIL_HUMITY_COMM_ABNORMAL, FAIL_ACK);
	}
	else
	{
		App_vFailReport(FAIL_HUMITY_COMM_ABNORMAL, FAIL_RELEASE);
	}
}

/*===========================================================================*
 *	函数名称：FailManager::Dat_GetBMSDataState(void)
 *	功能描述：BMS数据状态获取
 *	输入参数：none
 *	返回值：none
 *	使用说明： 
 *==========================================================================*/
/*void Class_FailManager::Dat_GetBMSDataState(void)
{
	uint8_t bmsdatastate,x = 0;//x为接口返回的值
	bmsdatastate = x;
	if(bmsdatastate == 1)
	{
		App_vFailReport(FAIL_BMS_DATA_ABNORMAL, FAIL_ACK);
	}
	else
	{
		App_vFailReport(FAIL_BMS_DATA_ABNORMAL, FAIL_RELEASE);
	}

}*/

/*===========================================================================*
 *	函数名称：FailManager::Dat_GetSysACFault(void)
 *	功能描述：系统并网输入故障获取
 *	输入参数：none
 *	返回值：none
 *	使用说明： 
 *==========================================================================*/
void Class_FailManager::Dat_GetSysACFault(void)
{
	uint8_t state = 0,index;//x为接口返回的值
	for(index = AC_LACKPHASE;index <= AC_LOSE;index++)
	{
		//@todo市电计算给出接口
		//state = (index);
		if(state == 1)
			App_vFailReport(FAIL_INPUT_LACKPHASE << index, FAIL_ACK);
		else
			App_vFailReport(FAIL_INPUT_LACKPHASE << index, FAIL_RELEASE);
	}
}

/*===========================================================================*
 *	函数名称：FailManager::Dat_GetSysDCFault(void)
 *	功能描述：系统级故障获取
 *	输入参数：none
 *	返回值：none
 *	使用说明： 
 *==========================================================================*/

void  Class_FailManager::Dat_GetSysDCFault(void)
{
	uint8_t insultchekstate,battterystate,dcstate,x = 0,index;//x为接口返回的值
	//@todo  4路电池绝缘检测获取
	insultchekstate = objBMS.App_Batt_StatusFlag(); //电池绝缘检测接口
	battterystate = x;
	if(insultchekstate == BATT_FAULT)
	{
		App_vFailReport(FAIL_INSULT_ABNORM, FAIL_ACK);
	}
	else
		App_vFailReport(FAIL_INSULT_ABNORM, FAIL_RELEASE);
	if(battterystate == 1)
		App_vFailReport(FAIL_BATTERY_REVERSE, FAIL_ACK);
	else
		App_vFailReport(FAIL_BATTERY_REVERSE, FAIL_RELEASE);
	for(index = 0;index < 3;index++)
	{
		dcstate = x;
		if(dcstate == 1)
			App_vFailReport(FAIL_DC_LOSEVOLT << index, FAIL_ACK);
		else
			App_vFailReport(FAIL_DC_LOSEVOLT << index, FAIL_RELEASE);
	}
}

/*===========================================================================*
 *	函数名称：FailManager::Dat_GetSysData(void)
 *	功能描述：系统量获取
 *	输入参数：none
 *	返回值：none
 *	使用说明： 
 *==========================================================================*/
void  Class_FailManager::Dat_GetSysData(void)
{
	//ABC为系统量接口
	uint16_t test1 = 0x0001;
	int16_t  test2 = 0x0002;
	int16_t dcpower = 0;
	uint8_t modulenum;
	uint16_t onoffstate,onofflinestate,chargestate;
	if (g_st_TimerFlag.b100ms == 1)
	{
		onoffstate = ObjPcuOnOffCtrl.getSystemTurnOnOffState();
		onofflinestate = ObjPcuOnOffCtrl.getSystemOnOfflineState();
		chargestate = ObjPcuOnOffCtrl.getSystemChargeState();
		PCSData1.FailState = m_u64_FailState;
		//模拟量采样获取
		PCSData1.PCSData.temp = objStoredEnergy.APP_GetMaxTemp();
		PCSData1.PCSData.humity = 2;
		PCSData1.PCSData.Transfor_Temp = 3;
		PCSData1.PCSData.ChargePower = 4;
		PCSData1.PCSData.DisChargePower = 5;
		PCSData1.PCSData.ActivePower = 6;
		PCSData1.PCSData.ReactivePower = 7;
		PCSData1.PCSData.PowerFactor = 8;
		PCSData1.PCSData.P_Voltage = objStoredEnergy.m_st_aACDCData[0].stMeter.P_Voltage;
		PCSData1.PCSData.N_Voltage = objStoredEnergy.m_st_aACDCData[0].stMeter.N_Voltage;
		//@todo二级系统计算
		for(modulenum = 0;modulenum < objStoredEnergy.m_st_ModuleInfo.ACModuleNum;modulenum++)
		{
			dcpower += objStoredEnergy.m_st_aACDCData[modulenum].stMeter.SYSPowerP;
		}
		PCSData1.PCSData.DCPower = dcpower;
		//并离网模式
		if(onofflinestate == 1)
		{
			PCSData1.PCSData.OnOffGrid = 1;
		}
		else if(onofflinestate == 0)
		{
			PCSData1.PCSData.OnOffGrid = 2;
		}
		//ABC相电压、电流、频率
		PCSData1.PCSData.OutputVolt_A = 13;
		PCSData1.PCSData.OutputVolt_B = 14;
		PCSData1.PCSData.OutputVolt_C = 15;
		PCSData1.PCSData.OutputCurr_A = 16;
		PCSData1.PCSData.OutputCurr_B = 17;
		PCSData1.PCSData.OutputCurr_C = 18;
		PCSData1.PCSData.Freq = 19;
		if(chargestate == 0)
		{
			PCSData1.PCSData.Sysstate = 1;
		}
		else if(chargestate == 1)
		{
			PCSData1.PCSData.Sysstate = 2;
		}	
		else if(chargestate == 2)
		{
			PCSData1.PCSData.Sysstate = 3;
		}
		if(onoffstate == 1)
		{
			PCSData1.PCSData.OnOffState = 1;
		}
		else if(onoffstate == 0)
		{
			PCSData1.PCSData.OnOffState = 2;
		}
		PCSData1.PCSData.PowerSetData = ObjPcuOnOffCtrl.getSystemPowerSet();
	}
}

/*===========================================================================*
 *	函数名称：FailManager::App_UploadPCSData1(void)
 *	功能描述：向集控上传PCS柜信息1
 *	输入参数：none
 *	返回值：none
 *	使用说明： 
 *==========================================================================*/

void Class_FailManager::App_UploadPCSData1(void)
{
	objCanMstMonLongFrame.App_TransData(objPcuModel.m_i16_SelfAddr,250,PGN_PCS_DATA1,sizeof(PCSData1),(uint8_t*)(&PCSData1));
}
/*===========================================================================*
 *	函数名称：FailManager::App_UploadPCSData2(void)
 *	功能描述：向集控上传PCS柜信息2
 *	输入参数：none
 *	返回值：none
 *	使用说明： 
 *==========================================================================*/

void Class_FailManager::App_UploadPCSData2(void)
{
	objCanMstMonLongFrame.App_TransData(objPcuModel.m_i16_SelfAddr,250,PGN_PCS_DATA2,sizeof(PCSData2),(uint8_t*)(&PCSData2));
}

/*===========================================================================*
 *	函数名称：FailManager::Dat_UpdataFailLevel(void)
 *	功能描述：更新故障等级及处理方法
 *	输入参数：none
 *	返回值：none
 *	使用说明： 
 *==========================================================================*/

void Class_FailManager::Dat_UpdataFailLevel(void)
{
	uint8_t index;//模块个数计数
	//故障状态为0时，处理方法为0，故障灯不亮
	if (PCSData1.FailState == 0)
	{
		PCSFailLevel.FailLevel = 0;
		PCSFailLevel.Bits.FaultLed = 0;
	}
	else if((PCSData1.FailState != 0) || (ScuOfflineError == 1) || (Dat_ModuleErrorState() == ModuleError))
	{
		PCSFailLevel.Bits.FaultLed = 1;
	}
	//电池通信异常时，给出判断直流通道是否中断动作，电池侧根据是否中断切换本地电池管理逻辑
	if((App_GetFailState(FAIL_BATTERY1_COMM_ABNORMAL)) || (App_GetFailState(FAIL_BATTERY2_COMM_ABNORMAL)) 
	|| (App_GetFailState(FAIL_BATTERY3_COMM_ABNORMAL)) || (App_GetFailState(FAIL_BATTERY4_COMM_ABNORMAL)))
	{
		PCSFailLevel.Bits.CheckDCState = 1;
	}
	else
	{
		PCSFailLevel.Bits.CheckDCState = 0;
	}
	//EPO
	/*if(App_GetFailState(FAIL_EMERGENCY_STOP))
	{
		PCSFailLevel.Bits.EPO = 1;
	}
	else
	{
		PCSFailLevel.Bits.EPO = 0;
	}*/
	//@todo 模块通信故障降额逻辑
	//风机过温告警
	if(App_GetFailState(FAIL_FAN_TEMP_FAULT))
	{
		PCSFailLevel.Bits.Reduce_power_fan = 1;
		PCSFailLevel.Bits.TurnOnFan_30min = 1;
	}
	else
	{
		PCSFailLevel.Bits.Reduce_power_fan = 0;
		PCSFailLevel.Bits.TurnOnFan_30min = 0;
	}
	//直流熔丝&绝缘检测
	if((App_GetFailState(FAIL_DC_FUSE1_FAULT)) || (App_GetFailState(FAIL_DC_FUSE1_FAULT)) 
	|| (App_GetFailState(FAIL_DC_FUSE1_FAULT)) || (App_GetFailState(FAIL_DC_FUSE1_FAULT)) || (App_GetFailState(FAIL_INSULT_ABNORM)))
	{
		PCSFailLevel.Bits.ModuleTurnOff = 1;
	}
	else
	{
		PCSFailLevel.Bits.ModuleTurnOff = 0;
	}
	//变压器
	if(App_GetFailState(FAIL_TRANSFOR_TEMP_FAULT))
	{
		PCSFailLevel.Bits.Reduce_power_p5 = 1;
	}
	else
	{
		PCSFailLevel.Bits.Reduce_power_p5 = 0;
	}
	//绝缘检测
	if(App_GetFailState(FAIL_INSULT_ABNORM) || App_GetFailState(FAIL_BATTERY_REVERSE)
		|| App_GetFailState(FAIL_DC_FUSE1_FAULT) || App_GetFailState(FAIL_DC_FUSE2_FAULT)
		|| App_GetFailState(FAIL_DC_FUSE3_FAULT) || App_GetFailState(FAIL_DC_FUSE4_FAULT))
	{
		PCSFailLevel.Bits.BanTurnOn = 1;
	}
	else
	{
		PCSFailLevel.Bits.BanTurnOn = 0;
	}

}

/*===========================================================================*
 *	函数名称：FailManager::Dat_GetSysLevelFault(void)
 *	功能描述：获取系统级故障
 *	输入参数：none
 *	返回值：none
 *	使用说明： 
 *==========================================================================*/

void Class_FailManager::Dat_GetSysLevelFault(void)
{
	Dat_GetIOState();		//获取IO量故障
	//机柜温度
	//@todo 告警点为模块功率降额的标准点 需进一步确定
	if((objStoredEnergy.APP_GetMaxTemp()) > 700)
	{
		App_vFailReport(FAIL_PCS_AMBINENT_TEMP, FAIL_ACK);
	}
	else
	{
		App_vFailReport(FAIL_PCS_AMBINENT_TEMP, FAIL_RELEASE);
	}
	//离网AC输出过流
	if(objStoredEnergy.APP_GetACDCFaultstate(OUTPUT_OVER_CURR) == 1)
	{
		App_vFailReport(FAIL_OFFLINE_AC_OVERCURR, FAIL_ACK);
	}
	else
	{
		App_vFailReport(FAIL_OFFLINE_AC_OVERCURR, FAIL_RELEASE);
	}
	//离网AC输出短路
	if(objStoredEnergy.APP_GetACDCFaultstate(OUTPUT_SHORT_CIRCUIT) == 1)
	{
		App_vFailReport(FAIL_OFFLINE_SHORT_CIRCUIT, FAIL_ACK);
	}
	else
	{
		App_vFailReport(FAIL_OFFLINE_SHORT_CIRCUIT, FAIL_RELEASE);
	}

}

/*===========================================================================*
 *	函数名称：FailManager::Dat_ModuleErrorState(void)
 *	功能描述：获取模块状态
 *	输入参数：none
 *	返回值：模块状态，0为模块正常，1为模块故障/告警
 *	使用说明： 
 *==========================================================================*/

uint8_t	Class_FailManager::Dat_ModuleErrorState(void)
{
	uint8_t  acdcnum,dcdcnum,state = ModuleNormal;	//模块计数
	for(acdcnum = 0;acdcnum < objStoredEnergy.m_st_ModuleInfo.ACModuleNum;acdcnum++)
	{
		if(*(&(ModuleErrorState.ACDC1_State) + acdcnum) > 0)
		{
			state = ModuleError;
			break;
		}
	}
	if(objStoredEnergy.m_st_ModuleInfo.ModuleLevel == 2 )
	{
		for(dcdcnum = 0;dcdcnum < objStoredEnergy.m_st_ModuleInfo.DCModuleNum;dcdcnum++)
		{
			if(*(&(ModuleErrorState.DCDC1_State) + dcdcnum) > 0)
			{
				state = ModuleError;
				break;
			}
		}
	}
	return state;
}


