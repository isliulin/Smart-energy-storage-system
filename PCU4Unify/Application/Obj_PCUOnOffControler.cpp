/*==========================================================*/
/* Title		:	Obj_PCUOnOffControler.h														*/
/* Description	: 	PCU OnOffcontroler											*/
// Author		:	wangyufei
/* Date			:	2017.10.30														*/
/*==========================================================*/

#include "Publics.h"




	//开关机等待时间

static	uint16_t time_acdc_turnon_wait;
static	uint16_t time_dcdc_turnon_wait;
static	uint16_t time_acdc_turnoff_wait;
static	uint16_t time_dcdc_turnoff_wait;


/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::getEPOError(void)
 *  功能描述：获取EPO错误时间
 *  输入参数： 无
 *  返回值：  功率设置
 *  使用说明： 
 *==========================================================================*/

void Class_PCUOnOffControler::getEPOError(_CALENDAR* epoTime)
{
	objEEPROM.App_ReadNumBytes(EPOERRORADDR, (uint8_t*)epoTime,sizeof(_CALENDAR));//EPO错误写入EEROM
}

/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::ClearEPOError(void)
 *  功能描述：清除EPO错误时间
 *  输入参数： 无
 *  返回值：  功率设置
 *  使用说明： 
 *==========================================================================*/

void Class_PCUOnOffControler::ClearEPOError()
{
	_CALENDAR epoTmp;	
	objEEPROM.App_ReadNumBytes(EPOERRORADDR, (uint8_t*)(&epoTmp),sizeof(_CALENDAR));//EPO错误写入EEROM
	
	if((epoTmp.Month == 0xFF) && (epoTmp.Day == 0xFF) && (epoTmp.Hour == 0xFF) && (epoTmp.Minute == 0xFF) 
		&& (epoTmp.Second == 0xFF) && (epoTmp.Year.word == 0xFFFF))
	{

	}
	else
	{
		memset(&epoTmp, 0xFF, sizeof(_CALENDAR));
		objEEPROM.App_WriteNumBytes(EPOERRORADDR, (uint8_t*)(&epoTmp),sizeof(_CALENDAR));//EPO错误清零
	}
	

}


/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::getSystemPowerSet(void)
 *  功能描述：获取系统功率设置
 *  输入参数： 无
 *  返回值：  功率设置
 *  使用说明： 
 *==========================================================================*/

uint16_t  Class_PCUOnOffControler::getSystemPowerSet()
{
	return pcuWorkState.systemDisChargePower;

}


/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::getSystemTurnOnOffState(void)
 *  功能描述：获取系统开关机状态
 *  输入参数： 无
 *  返回值：       0是关机，1是开机
 *  使用说明： 
 *==========================================================================*/

int8_t  Class_PCUOnOffControler::getSystemTurnOnOffState()
{
		if(pcuWorkState.systemTurnOnOk == 1)
		{
			return 1;
		}
		else if(pcuWorkState.systemTurnOffOk == 1)
		{
			return 0;
		}
		else
		{
			return -1;
		}
}

/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::getSystemOnOfflineState(void)
 *  功能描述：获取系统并离网状态
 *  输入参数： 无
 *  返回值：       0是离机，1是并网
 *  使用说明： 
 *==========================================================================*/

int8_t  Class_PCUOnOffControler::getSystemOnOfflineState()
{
		if(pcuCTLState.systemOnOfflineCtrl == SCU_CTRL_ONLINE)
		{
			return 1;
		}
		else if(pcuCTLState.systemOnOfflineCtrl == SCU_CTRL_OFFLINE)
		{
			return 0;
		}
		else
		{
			return -1;
		}
		

}

/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::getSystemChargeState(void)
 *  功能描述：获取系统充放电状态
 *  输入参数： 无
 *  返回值：       0是待机，1是充电，2是放电
 *  使用说明： 
 *==========================================================================*/

int8_t  Class_PCUOnOffControler::getSystemChargeState()
{
	if(pcuCTLState.systemChargeStateCtrl  == SCU_CTRL_CHARGE)
	{
		return 1;
	}
	else if(pcuCTLState.systemChargeStateCtrl == SCU_CTRL_DISCHARGE)
	{
		return 2;
	}
	else if(pcuCTLState.systemChargeStateCtrl == SCU_CTRL_STOPCHARGE)
	{
		return 0;
	}
	else
	{
		return -1;
	}


}



uint8_t  Class_PCUOnOffControler::get_communication_error()
{
	if((objFailManager.App_GetFailState(FAIL_ACDC1_COMM_ABNORMAL)) || (objFailManager.App_GetFailState(FAIL_ACDC2_COMM_ABNORMAL)) 
		|| (objFailManager.App_GetFailState(FAIL_ACDC3_COMM_ABNORMAL)) || (objFailManager.App_GetFailState(FAIL_ACDC4_COMM_ABNORMAL))
		|| (objFailManager.App_GetFailState(FAIL_ACDC5_COMM_ABNORMAL)) || (objFailManager.App_GetFailState(FAIL_ACDC6_COMM_ABNORMAL))
		|| (objFailManager.App_GetFailState(FAIL_ACDC7_COMM_ABNORMAL)) || (objFailManager.App_GetFailState(FAIL_ACDC8_COMM_ABNORMAL))
		|| (objFailManager.App_GetFailState(FAIL_DCDC1_COMM_ABNORMAL)) || (objFailManager.App_GetFailState(FAIL_DCDC2_COMM_ABNORMAL))
		|| (objFailManager.App_GetFailState(FAIL_DCDC3_COMM_ABNORMAL)) || (objFailManager.App_GetFailState(FAIL_DCDC4_COMM_ABNORMAL))
		|| (objFailManager.App_GetFailState(FAIL_DCDC5_COMM_ABNORMAL)) || (objFailManager.App_GetFailState(FAIL_DCDC6_COMM_ABNORMAL))
		|| (objFailManager.App_GetFailState(FAIL_DCDC7_COMM_ABNORMAL)) || (objFailManager.App_GetFailState(FAIL_DCDC8_COMM_ABNORMAL))
		|| (objFailManager.App_GetFailState(FAIL_BATTERY1_COMM_ABNORMAL)) || (objFailManager.App_GetFailState(FAIL_BATTERY2_COMM_ABNORMAL))
		|| (objFailManager.App_GetFailState(FAIL_BATTERY3_COMM_ABNORMAL)) || (objFailManager.App_GetFailState(FAIL_BATTERY4_COMM_ABNORMAL)))
	{
		return 1;
	}
	else
	{
		return 0;
	}

}


/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::GetSystemErrState(void)
 *  功能描述：检查ACDC DCDC BMS错误状态
 *  输入参数： 无
 *  返回值：       -1 is err,0 is normal
 *  使用说明：
 *==========================================================================*/

void Class_PCUOnOffControler::GetSystemErrState()
{
#ifdef PCU_CONFIG_ENABLE
	
		 //读取ACDC状态是否异常
		 if((objFailManager.App_GetModuleFailState(FAIL_ACDC1_FAULT)) || (objFailManager.App_GetModuleFailState(FAIL_ACDC2_FAULT))
			|| (objFailManager.App_GetModuleFailState(FAIL_ACDC3_FAULT)))
		 {
			pcuWorkState.systemAcdcErr = 1;
		 }
		 else
		 {
			pcuWorkState.systemAcdcErr = 0;
		 }
		 //读取DCDC状态是否异常
		 if((objFailManager.App_GetModuleFailState(FAIL_DCDC1_FAULT)) || (objFailManager.App_GetModuleFailState(FAIL_DCDC2_FAULT))
			|| (objFailManager.App_GetModuleFailState(FAIL_DCDC3_FAULT)))
		 {
			pcuWorkState.systemDcdcErr = 1;
		 }
		 else
		 {
			pcuWorkState.systemDcdcErr = 0;
		 }
#else
	
		 uint8_t i;
		  //读取ACDC错误状态
		  for(i = 0;i < pcuOnoffConfig.acdcCtrlNum;i++)
		  {
			 if(objFailManager.App_GetModuleFailState(pcuOnoffConfig.acdcCtrlAddr[i]))
			 {
				 pcuWorkState.systemAcdcErr = 1;
				 break;//有错误异常
			 }
			 else if( i == pcuOnoffConfig.acdcCtrlNum -1)
			 {
				 pcuWorkState.systemAcdcErr = 0;
			 }
		  }
	
			 
		  //读取DCDC错误状态
		  for(i = 0;i < pcuOnoffConfig.dcdcCtrlNum;i++)
		  {
			 if(objFailManager.App_GetModuleFailState(pcuOnoffConfig.dcdcCtrlAddr[i]))
			 {
				 pcuWorkState.systemDcdcErr = 1;
				 break;//有错误异常
			 }
			 else if( i == pcuOnoffConfig.dcdcCtrlNum -1)
			 {
				 pcuWorkState.systemDcdcErr = 0;
			 }
		  }
	
#endif	


}


/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::GetErrState(void)
 *  功能描述：检查ACDC DCDC BMS错误状态
 *  输入参数： 无
 *  返回值：       -1 is err,0 is normal
 *  使用说明：
 *==========================================================================*/

int8_t Class_PCUOnOffControler::GetErrState()
{
	if((pcuWorkState.systemAcdcErr == 1) || (pcuWorkState.systemDcdcErr == 1)
		|| (pcuWorkState.systemBmsErr > 3))
	{
		return -1;
	}
	else
	{
		return 0;
	}

}


/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::SystemStateCheck(void)
 *  功能描述：系统状态检测
 *  输入参数： 无
 *  返回值：       无
 *  使用说明： 
 *==========================================================================*/

void Class_PCUOnOffControler::SystemStateCheck()
{
 	 GetSystemErrState();
	 SystemPowerLimit();//根据电池和模块错误状态更新功率限制

	 //读取ACDC开关机状态
//	 if((1 == objStoredEnergy.Dat_GetModuleRunState(0)) && (1 == objStoredEnergy.Dat_GetModuleRunState(1))
//	 	&& (1 == objStoredEnergy.Dat_GetModuleRunState(2)))
#ifdef PCU_CONFIG_ENABLE

	 if((objStoredEnergy.Dat_GetModuleRunState(MAC_ID_DCAC0) > 0) && (objStoredEnergy.Dat_GetModuleRunState(MAC_ID_DCAC1) > 0))
	 {
		pcuWorkState.acdcTurnOnOK = 1;
		pcuWorkState.acdcTurnOffOk = 0;
	 }
	 else
	 {
		pcuWorkState.acdcTurnOnOK = 0;
		pcuWorkState.acdcTurnOffOk = 1;
	 }
	 //读取DCDC开机状态
//	 if((1 == objStoredEnergy.Dat_GetModuleRunState(3)) && (1 == objStoredEnergy.Dat_GetModuleRunState(4))
//	 	&& (1 == objStoredEnergy.Dat_GetModuleRunState(5)))
	 if(objStoredEnergy.Dat_GetModuleRunState(MAC_ID_BATT_DCDC0) > 0)
	 {
		pcuWorkState.dcdcTurnOnOK = 1;
		pcuWorkState.dcdcTurnOffOk = 0;
	 }
	 else
	 {
		pcuWorkState.dcdcTurnOnOK = 0;
		pcuWorkState.dcdcTurnOffOk = 1;
	 }
#else
	 uint8_t i;
	 //读取ACDC开机状态
	 for(i = 0;i < pcuOnoffConfig.acdcCtrlNum;i++)
	 {
		if(objStoredEnergy.Dat_GetModuleRunState(pcuOnoffConfig.acdcCtrlAddr[i]) <= 0)
		{
			pcuWorkState.acdcTurnOnOK = 0;
			pcuWorkState.acdcTurnOffOk = 1;
			break;//没有开机成功
		}
		else if( i == pcuOnoffConfig.acdcCtrlNum -1)
		{
			pcuWorkState.acdcTurnOnOK = 1;
			pcuWorkState.acdcTurnOffOk = 0;
		}
	 }

	 if(pcuOnoffConfig.modelCtrlLevel == 2)	//读取DCDC开机状态
	 {	 for(i = 0;i < pcuOnoffConfig.dcdcCtrlNum;i++)
		 {
			if(objStoredEnergy.Dat_GetModuleRunState(pcuOnoffConfig.dcdcCtrlAddr[i]) <= 0)
			{
				pcuWorkState.dcdcTurnOnOK = 0;
				pcuWorkState.dcdcTurnOffOk = 1;
				break;//没有开机成功
			}
			else if( i == pcuOnoffConfig.dcdcCtrlNum -1)
			{
				pcuWorkState.dcdcTurnOnOK = 1;
				pcuWorkState.dcdcTurnOffOk = 0;

			}
		 }
	 }

#endif

	 if(pcuWorkState.systemTurnOnOk == 1)//防止系统掉电，检测模块开机状态
	 {
			if(pcuOnoffConfig.modelCtrlLevel == 2)
			{
				if((pcuWorkState.acdcTurnOnOK != 1) || (pcuWorkState.dcdcTurnOnOK != 1))
				{
					pcuWorkState.systemTurnOnOk = 0;
					pcuWorkState.systemTurnOffOk = 1;

				}

			}
			else if((pcuOnoffConfig.modelCtrlLevel == 1) && (pcuWorkState.acdcTurnOnOK != 1))
			{
				pcuWorkState.systemTurnOnOk = 0;//@todo for test
				pcuWorkState.systemTurnOffOk = 1;
				
			}
	 }
	 if(pcuWorkState.systemTurnOffOk == 1)//防止系统掉电，检测模块关机状态
	 {
			if(pcuOnoffConfig.modelCtrlLevel == 2)
			{
				if((pcuWorkState.acdcTurnOffOk != 1) || (pcuWorkState.dcdcTurnOffOk != 1))
				{
					pcuWorkState.systemTurnOffOk = 0;
					pcuWorkState.systemTurnOnOk = 1;

				}

			}
			else if((pcuOnoffConfig.modelCtrlLevel == 1) && (pcuWorkState.acdcTurnOffOk != 1))
			{
				pcuWorkState.systemTurnOffOk = 0;//@todo for test
				pcuWorkState.systemTurnOnOk = 1;

			}
	 }

	 //读取BMS状态是否异常
	 pcuWorkState.systemBmsErr = objBMS.App_Batt_StatusFlag();
	 //读取电池断开闭合状态
	 if(SWITCH_ON == objBMS.App_RespContactorStatus())
	 {
		pcuWorkState.bmsTurnOnOk = 1;
	 }
	 
     //@todo ADC 采样检测是否异常

	 //@todo 读取集控开机信号
	 
#ifdef PCUCTRL_ON_OFF_SWITCH
	
	 if(E_LOW == BSP_DI_GetState(DI_POWERON_FB))
	 {//检测到开机信号
	        if(pcuWorkState.systemEPOTurnOff != 1)
	        {
	            pcuWorkState.systemTurnOn = 1;
				pcuWorkState.systemTurnOff = 0;
	        }
			
	 }
	 if(E_LOW == BSP_DI_GetState(DI_POWEROFF_FB))
	 {//检测到关机信号
			pcuWorkState.systemTurnOff = 1;
			pcuWorkState.systemTurnOn = 0;
	 }
#else
	if((SCU_CTRL_TURNON == ObjPcuOnOffCtrl.pcuWorkState.systemTurnOnOffScuCtrl) 
		&& (pcuCTLState.systemTurnOnBan == 0)) 
	{//集控开机信号
		    if(pcuWorkState.systemEPOTurnOff != 1)
	        {
	            pcuWorkState.systemTurnOn = 1;
				pcuWorkState.systemTurnOff = 0;
	        }

	}
	else if(SCU_CTRL_TURNOFF == ObjPcuOnOffCtrl.pcuWorkState.systemTurnOnOffScuCtrl)
	{
		//集控关机信号
			pcuWorkState.systemTurnOff = 1;
			pcuWorkState.systemTurnOn = 0;

	}

#endif

	 if(E_LOW == BSP_DI_GetState(DI_EPO_FB))
	 {//检测到EPO信号

//BSP_Relay_Close(ONLINE_LED); //J62-2 FOR TEST
//	objEEPROM.WriteEpoTime();
//BSP_Relay_Open(ONLINE_LED); //J62-2 FOR TEST

	 pcuWorkState.systemEPOTurnOff = 1;
	 pcuWorkState.systemTurnOff = 1;
	 pcuWorkState.systemTurnOn = 0;
	 pcuWorkState.acdcTurnOn = 0;
	 pcuWorkState.dcdcTurnOn = 0;
	 pcuWorkState.bmsTurnOn = 0;


	 }
     else
     {
            pcuWorkState.systemEPOTurnOff = 0;
     }
	  //for test
	 pcuWorkState.bmsTurnOffOk = 1;
	 
	//检测通讯故障
 	 pcuErrorState.systemCommunicationError = get_communication_error();
	
	//检测并离网控制
	if(pcuErrorState.systemCommunicationError == 0)//@todo for test
	{
		pcuCTLState.systemOnOfflineCtrl = pcuWorkState.systemOnOfflineScuCtrl;

	}
	//检测开关机控制
	pcuCTLState.systemTurnOnOffCtrl = pcuWorkState.systemTurnOnOffScuCtrl;
	//检测充放电控制
	pcuCTLState.systemChargeStateCtrl = pcuWorkState.systemChargeStateScuCtrl;
	 
}

/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::
TurnOnFan(void)
 *  功能描述：打开风机
 *  输入参数： 无
 *  返回值：       
 *  使用说明：
 *==========================================================================*/
void Class_PCUOnOffControler::TurnOnFan(void)
{
		BSP_Relay_Close(FAN1_RELAY);//打开风机
		BSP_Relay_Close(FAN2_RELAY);//打开风机

}

/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::
TurnOffFan(void)
 *  功能描述：关闭风机
 *  输入参数： 无
 *  返回值：       
 *  使用说明：
 *==========================================================================*/
void Class_PCUOnOffControler::TurnOffFan(void)
{

		BSP_Relay_Open(FAN1_RELAY);//关闭风机
		BSP_Relay_Open(FAN2_RELAY);//关闭风机

}

/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::
TurnOffFanDelay(void)
 *  功能描述：10分钟后关闭风机
 *  输入参数： 无
 *  返回值：       
 *  使用说明：
 *==========================================================================*/
void Class_PCUOnOffControler::TurnOffFanDelay(void)
{
	static uint16_t fanCount1;
	
	if(pcuCTLState.TurnOffFanDelay == 1)
	{

		
			if(g_st_TimerFlag.b1s == 1)
			{
				fanCount1++;
			}
			if(fanCount1 > 600)	//10分钟
			{
				fanCount1 = 0;
				
				pcuCTLState.TurnOffFanDelay = 0;
				TurnOffFan();//关闭风机
			}

	}


}


/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::
TurnOnFanDelay(void)
 *  功能描述：关闭风机，30分钟后再开
 *  输入参数： 无
 *  返回值：       
 *  使用说明：
 *==========================================================================*/
void Class_PCUOnOffControler::TurnOnFanDelay(void)
{
	static uint16_t timeCount;
	static uint8_t tempFan;
	if(pcuCTLState.TurnOnFanDelay == 1)
	{
			if(tempFan == 0)
			{
				TurnOffFan();//关闭风机
				tempFan = 1;
		
			}
		
			if(g_st_TimerFlag.b1s == 1)
			{
				timeCount++;
			}
			if(timeCount > 1800)	//30分钟
			{
				timeCount = 0;
				tempFan = 0;
				pcuCTLState.TurnOnFanDelay = 0;
				TurnOnFan();//打开风机
			}

	}
	

}




/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::SystemErrorResponse(void)
 *  功能描述：根据故常处理接口做出响应
 *  输入参数： 无
 *  返回值：       
 *  使用说明：
 *==========================================================================*/
void Class_PCUOnOffControler::SystemErrorResponse(void)
{

	if(objFailManager.PCSFailLevel.Bits.FaultLed > 0)
	{
		pcuErrorState.systemErr = 1;	// 开启故障灯	
		
	}
	else if(objFailManager.PCSFailLevel.Bits.FaultLed == 0)
	{
		pcuErrorState.systemErr = 0;	

	}
	
	if(objFailManager.PCSFailLevel.Bits.ModuleTurnOff  > 0)
	{
		ObjPcuOnOffCtrl.pcuWorkState.systemTurnOnOffScuCtrl = SCU_CTRL_TURNOFF;
	}
	else if(objFailManager.PCSFailLevel.Bits.ModuleTurnOff == 0)
	{
		

	}

	if(objFailManager.PCSFailLevel.Bits.EPO  > 0)
	{
			//检测到EPO信号
			pcuWorkState.systemEPOTurnOff = 1;
			pcuWorkState.systemTurnOff = 1;
            pcuWorkState.systemTurnOn = 0;
            pcuWorkState.acdcTurnOn = 0;
            pcuWorkState.dcdcTurnOn = 0;
            pcuWorkState.bmsTurnOn = 0;
	}
	else if(objFailManager.PCSFailLevel.Bits.EPO == 0)
	{
			//pcuWorkState.systemEPOTurnOff = 0;
	}

	if(objFailManager.PCSFailLevel.Bits.Reduce_power_p5  > 0)
	{
		pcuCTLState.systemPowerLimit = 50;
		//@todu add acdc_dcdc_workset.acdcMaxPow =ACDC_MAX_POW*10*ObjPcuOnOffCtrl.pcuCTLState.systemPowerLimit/100 ;	//设置ACDC最大功率
	}
	else if(objFailManager.PCSFailLevel.Bits.Reduce_power_p5 == 0)
	{
		pcuCTLState.systemPowerLimit = 100;

	}

	if(objFailManager.PCSFailLevel.Bits.Reduce_power_fan  > 0)
	{
		pcuCTLState.systemPowerLimit = 50;//@todu 待调整
	}
	else if(objFailManager.PCSFailLevel.Bits.Reduce_power_fan == 0)
	{
		pcuCTLState.systemPowerLimit = 100;

	}

	if(objFailManager.PCSFailLevel.Bits.TurnOnFan_30min  > 0)
	{
		pcuCTLState.TurnOnFanDelay = 1;
	}
	else if(objFailManager.PCSFailLevel.Bits.TurnOnFan_30min == 0)
	{
		pcuCTLState.TurnOnFanDelay = 0;		

	}

	if(objFailManager.PCSFailLevel.Bits.BanTurnOn  > 0)
	{
		pcuCTLState.systemTurnOnBan = 1;
	}
	else if(objFailManager.PCSFailLevel.Bits.BanTurnOn == 0)
	{
		pcuCTLState.systemTurnOnBan = 0;

	}

	


}



/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::SystemErrorResponse(void)
 *  功能描述：根据故常处理接口做出响应
 *  输入参数： 无
 *  返回值：       
 *  使用说明：
 *==========================================================================*/
void Class_PCUOnOffControler::SystemPowerLimit(void)
{//@todo 根据模块和电池故障信息，限制pcuCTLState.systemPowerLimit;//系统功率限制
	uint8_t i;
	uint8_t acTurnoffNum = 0;
	uint8_t dcTurnoffNum = 0;
	uint8_t tempLimit;
	uint8_t tempTurnoffNum;
	if(pcuWorkState.systemTurnOnOk){
		for(i = 0;i < objPcsConfigManage.m_st_GetConfigPara.ACModuleNum;i++)
		{
				if(objStoredEnergy.Dat_GetModuleRunState(objPcsConfigManage.m_st_GetConfigPara.ACModeAddr[i]) <= 0)
				{
					acTurnoffNum++;
				}
		}
		for(i = 0;i < objPcsConfigManage.m_st_GetConfigPara.DCModuleNum;i++)
		{
				if(objStoredEnergy.Dat_GetModuleRunState(objPcsConfigManage.m_st_GetConfigPara.DCModeAddr[i]) <= 0)
				{
					dcTurnoffNum++;
				}
		}
	}
	tempTurnoffNum = MAX(acTurnoffNum,dcTurnoffNum);
	tempLimit = 100*(objPcsConfigManage.m_st_GetConfigPara.ACModuleNum - tempTurnoffNum)
		/objPcsConfigManage.m_st_GetConfigPara.ACModuleNum;
	pcuCTLState.systemPowerLimit = MIN(tempLimit, pcuCTLState.systemPowerLimit);

}

/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::SystemLEDCTL(void)
 *  功能描述：LED灯逻辑控制
 *  输入参数： 无
 *  返回值：       
 *  使用说明：
 *==========================================================================*/
void Class_PCUOnOffControler::SystemLED_FAN_CTL(void)
{
	static uint8_t flag_idle;//继电器只执行一次动作
	static uint8_t flag_online;
	static uint8_t flag_offline;
	static uint8_t flag_err;
	static uint8_t flag_err_cancel;
	
	if((pcuWorkState.systemTurnOffOk == 1) && (flag_idle == 0))//待机状态
	{
		flag_idle = 1;
		flag_online = 0;
		flag_offline = 0;
		BSP_Relay_Close(IDLE_LED);//点亮待机灯
		BSP_Relay_Open(ONLINE_LED);
		BSP_Relay_Open(OFFLINE_LED);
		TurnOffFan();//关闭风机
		//pcuCTLState.TurnOffFanDelay = 1; @todo 10分钟后关闭风机

	}
	else if((pcuWorkState.systemTurnOnOk == 1) && (pcuCTLState.systemOnOfflineCtrl == SCU_CTRL_ONLINE)
		&& (flag_online == 0))//并网状态
	{
		flag_idle = 0;
		flag_online = 1;
		flag_offline = 0;
		BSP_Relay_Close(ONLINE_LED); //点亮并网灯
		BSP_Relay_Open(IDLE_LED);
		BSP_Relay_Open(OFFLINE_LED);
		TurnOnFan();//打开风机

	}
	else if((pcuWorkState.systemTurnOnOk == 1) && (pcuCTLState.systemOnOfflineCtrl == SCU_CTRL_OFFLINE)
		&& (flag_offline == 0))//离网状态
	{
		flag_idle = 0;
		flag_online = 0;
		flag_offline = 1;
		BSP_Relay_Close(OFFLINE_LED); //点亮离网灯
		BSP_Relay_Open(IDLE_LED);
		BSP_Relay_Open(ONLINE_LED);
		TurnOnFan();//打开风机

	}

	if((pcuWorkState.systemAcdcErr > 0) || (pcuWorkState.systemDcdcErr > 0) 

		|| (pcuWorkState.systemBmsErr > 3) || (1 == pcuErrorState.systemCommunicationError) 
		|| (pcuErrorState.systemErr))//故障状态
	{
		if(flag_err == 0)
		{
			flag_err = 1;
			flag_err_cancel = 0;
			BSP_Relay_Close(FAULT_LED); //点亮故障灯

		}


	}
	else
	{
		if(flag_err_cancel == 0)
		{
			flag_err = 0;
			flag_err_cancel = 1;
			BSP_Relay_Open(FAULT_LED);
		}
		

	}
}





/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::PcuOnOffConfigInit(void)
 *  功能描述：//配置管理初始化
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
void Class_PCUOnOffControler::PcuOnOffConfigInit(void)
{
	uint8_t i;
	_GetConfigParameterDef pcsConfig;

	memset(&pcuOnoffConfig, 0, sizeof(_PcuOnOffConfig));
	objPcsConfigManage.GetPcsConfigInfo(&pcsConfig);
	pcuOnoffConfig.modelCtrlLevel = pcsConfig.ModuleLevel;
	pcuOnoffConfig.acdcCtrlNum = pcsConfig.ACMajorNum;
	pcuOnoffConfig.dcdcCtrlNum = pcsConfig.DCMajorNum;
	pcuOnoffConfig.bmsCtrlNum = pcsConfig.ModeGroupNum;
	pcuOnoffConfig.acdcFunctionSet = MODEL_FORBID;//@todo 从配置读取


	for (i = 0; i < pcuOnoffConfig.acdcCtrlNum; i++)
	{
		pcuOnoffConfig.acdcCtrlAddr[i] = pcsConfig.ACMajorAddr[i];
						
	}

		for (i = 0; i < pcuOnoffConfig.dcdcCtrlNum; i++)
	{
		pcuOnoffConfig.dcdcCtrlAddr[i] = pcsConfig.DCMajorAddr[i];
		
	}
	
	for (i = 0; i < pcuOnoffConfig.bmsCtrlNum ; i ++)	//模块路数=电池路数
	{
		pcuOnoffConfig.bmsCtrlAddr[i] = pcsConfig.BMSAddr[i];		
						
	}
	

	pcuOnoffConfig.powerLimit = pcsConfig.AltitudeRange;//根据海拔限制功率：1是海拔小于4K，2是海拔大于4K；
	if(pcuOnoffConfig.powerLimit == 1)
	{
		pcuCTLState.systemPowerLimit = 100;
	}
	else if(pcuOnoffConfig.powerLimit == 2)
	{
		pcuCTLState.systemPowerLimit = 70;
	}

}


/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::Dat_Init(void)
 *  功能描述：init
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/

void Class_PCUOnOffControler::Dat_Init()
{
		memset(&pcuCTLState, 0, sizeof(_PcuCTLState));
		memset(&pcuErrorState, 0, sizeof(_PcuErrorState));
		memset(&pcuWorkState, 0, sizeof(_PcuWorkState));
		pcuWorkState.systemTurnOffOk = 1;
		pcuWorkState.systemChargeStateScuCtrl = SCU_CTRL_STOPCHARGE;
		pcuWorkState.systemOnOfflineScuCtrl = SCU_CTRL_ONLINE;
		pcuCTLState.systemOnOfflineCtrl = SCU_CTRL_ONLINE;
		pcuWorkState.systemDisChargePower = ACDC_POW_DISCHARGE;
		pcuWorkState.systemAcdcWorkMode = ACDC_WORK_MODE_VOLT;
		pcuWorkState.systemDcdcWorkMode = DCDC_WORK_MODE_CURRENT;
		pcuWorkState.systemAcdcOutVolt = ACDC_OUT_VLOT;
		//pcuCTLState.systemPowerLimit = 100;
		PcuOnOffConfigInit();
        
}

/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::SystemTurnOnAcdc(void)
 *  功能描述：turnon acdc
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/

void Class_PCUOnOffControler::SystemTurnOnAcdc()
{
	
	if(pcuWorkState.systemAcdcErr == 1)//ACDC状态异常
	{
			//上报异常
			pcuWorkState.acdcTurnOn = 0;
			
	
	}
	else if(pcuWorkState.acdcTurnOnAlrdy != 1)
	{
		//ObjChargeCtrl.ChargeStopControl();//待机设置
#ifdef PCU_CONFIG_ENABLE
		objStoredEnergy.App_SetModuleOnOff(MAC_ID_DCAC0, ACDC_ON);

#else
		uint8_t i;
		for(i = 0;i<pcuOnoffConfig.acdcCtrlNum;i++)
		{
			objStoredEnergy.App_SetModuleOnOff(pcuOnoffConfig.acdcCtrlAddr[i] , ACDC_ON);
		}
		

#endif
		

		pcuWorkState.acdcTurnOnAlrdy = 1;
	
	}
	if(pcuWorkState.acdcTurnOnAlrdy == 1)
	{
	
		if(g_st_TimerFlag.b100ms)
		{
			time_acdc_turnon_wait++;
		}
		
		if((time_acdc_turnon_wait > 30) && (pcuWorkState.acdcTurnOnOK != 1))
		{//ACDC开机失败，重新发送命令
			time_acdc_turnon_wait = 0;
			pcuWorkState.acdcTurnOnAlrdy =0;
	
		}

	}



}

/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::SystemTurnOnDcdc(void)
 *  功能描述：turnon dcdc
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/

void Class_PCUOnOffControler::SystemTurnOnDcdc()
{

	if(pcuWorkState.systemDcdcErr == 1)//DCDC状态异常
	{
			//@todo//异常报警
			//pcuWorkState.SystemErr = 1;
			time_dcdc_turnon_wait = 0;
			pcuWorkState.dcdcTurnOn = 0;
			
	
	}
	else
	{
		if(pcuWorkState.dcdcTurnOn != 1)
		{//等1S看DCDC状态是否异常
			if(g_st_TimerFlag.b100ms)
			{
				time_dcdc_turnon_wait++;
			}
			if(time_dcdc_turnon_wait > 10)
			{//DCDC状态正常
				time_dcdc_turnon_wait = 0;
				pcuWorkState.dcdcTurnOn = 1;//@todo 确定DCDC模块工作正常
		
			}	

		}
		if((pcuWorkState.dcdcTurnOn == 1) && (pcuWorkState.dcdcTurnOnAlrdy != 1))
		{
#ifdef PCU_CONFIG_ENABLE
			objStoredEnergy.App_SetModuleOnOff(MAC_ID_BATT_DCDC0, ACDC_ON);

#else
			uint8_t i;
			for(i = 0;i<pcuOnoffConfig.dcdcCtrlNum;i++)
			{
				objStoredEnergy.App_SetModuleOnOff(pcuOnoffConfig.dcdcCtrlAddr[i] , ACDC_ON);
			}
		

#endif

			//ObjChargeCtrl.ChargeStopControl();//待机设置

			pcuWorkState.dcdcTurnOnAlrdy = 1;
		
		}
		if(pcuWorkState.dcdcTurnOnAlrdy == 1)
		{
		
			if(g_st_TimerFlag.b100ms)
			{
				time_dcdc_turnon_wait++;
			}
			
			if((time_dcdc_turnon_wait > 30) && (pcuWorkState.dcdcTurnOnOK != 1))
			{//DCDC开机失败，重新发送命令
				time_dcdc_turnon_wait = 0;
				pcuWorkState.dcdcTurnOnAlrdy =0;
		
			}

		}


	}
	

}

/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::SystemTurnOnOnline(void)
 *  功能描述：并网开机
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
void Class_PCUOnOffControler::SystemTurnOnOnline()
{
	static int16_t pcu_online_turnon_states = PCU_ONLINE_IDLE;
	switch(pcu_online_turnon_states)
	{

		case PCU_ONLINE_IDLE:
			if(pcuWorkState.systemBmsErr > 3)
			{
                 
		        //错误处理模块进行异常处理	           
		                         
			}//收到开机命令pcuWorkState.systemTurnOn = 1
			else if((pcuWorkState.systemTurnOn == 1) && (pcuWorkState.systemTurnOnOk != 1))
			{
				
				objBMS.App_SetBattOnOff(POWER_ON);//吸合BMS继电器
				//@需要吸合多路继电器
		        pcu_online_turnon_states = PCU_ONLINE_TURNON_AC;       
			}
			else if(pcuWorkState.systemTurnOnOk == 1)
			{
								
			}
		
			break;

		case PCU_ONLINE_TURNON_AC:
			if((pcuWorkState.systemAcdcErr == 1) || (pcuWorkState.systemBmsErr > 3))
			{
			
		        //错误处理模块进行异常处理	          
		                         
			}
			else
			{
				
				SystemTurnOnAcdc();
							
				if(pcuWorkState.acdcTurnOnOK == 1)//1级，2级模块控制
				{
					if(pcuOnoffConfig.modelCtrlLevel == 2)
					{
						pcu_online_turnon_states = PCU_ONLINE_TURNON_DC;

					}
					else if(pcuOnoffConfig.modelCtrlLevel == 1)
					{
						pcu_online_turnon_states = PCU_ONLINE_TURNON_OK;

					}
					
				}
			}
			break;

		case PCU_ONLINE_TURNON_DC:
			if((pcuWorkState.systemAcdcErr == 1) || (pcuWorkState.systemDcdcErr == 1) 
				|| (pcuWorkState.systemBmsErr > 3))
			{
					//错误处理模块进行异常处理	                
		                
		                         
			}
			else
			{
				
				SystemTurnOnDcdc();
							
				if(pcuWorkState.dcdcTurnOnOK == 1)
				{
					pcu_online_turnon_states = PCU_ONLINE_TURNON_OK;
				}
			}
			break;

		case PCU_ONLINE_TURNON_OK:
			pcuWorkState.systemTurnOnOk = 1;
			pcuWorkState.systemTurnOffOk = 0;
            pcuWorkState.bmsTurnOnOk = 0;
		    pcuWorkState.acdcTurnOnAlrdy = 0;
		    pcuWorkState.dcdcTurnOnAlrdy = 0;

			pcu_online_turnon_states = PCU_ONLINE_IDLE;
			
			break;

		default:
			break;
				
		

	}


 
}

/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::SystemTurnOffDcdc(void)
 *  功能描述：turn off dcdc
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/

void Class_PCUOnOffControler::SystemTurnOffDcdc()
{	
	

	if(pcuWorkState.dcdcTurnOffAlrdy != 1)
	{
	
#ifdef PCU_CONFIG_ENABLE
		objStoredEnergy.App_SetModuleOnOff(MAC_ID_BATT_DCDC0, ACDC_OFF);
		
#else
		uint8_t i;
		for(i = 0;i<pcuOnoffConfig.dcdcCtrlNum;i++)
		{
			objStoredEnergy.App_SetModuleOnOff(pcuOnoffConfig.dcdcCtrlAddr[i] , ACDC_OFF);
		}
				
		
#endif


        //ObjChargeCtrl.ChargeStopControl();//待机设置
		pcuWorkState.dcdcTurnOffAlrdy = 1;
	
	}
	if(pcuWorkState.dcdcTurnOffAlrdy == 1)
	{
	
		if(g_st_TimerFlag.b100ms)
		{
			time_dcdc_turnoff_wait++;
			
		}
		//@todo adc检测 DCDC关机成功,pcuWorkState.dcdcTurnOffOK = 1
		if((time_dcdc_turnoff_wait > 30) && (pcuWorkState.dcdcTurnOffOk != 1))
		{//DCDC关机失败，重新发送命令
			time_dcdc_turnoff_wait = 0;
			pcuWorkState.dcdcTurnOffAlrdy =0;
	
		}

	}


}

/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::SystemTurnOffAcdc(void)
 *  功能描述：turn off acdc
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/

void Class_PCUOnOffControler::SystemTurnOffAcdc()
{
	
	if(pcuWorkState.acdcTurnOffAlrdy != 1)
	{	//ACDC关机机   

#ifdef PCU_CONFIG_ENABLE
		objStoredEnergy.App_SetModuleOnOff(MAC_ID_DCAC0, ACDC_OFF);
		
#else
		uint8_t i;
		for(i = 0;i<pcuOnoffConfig.acdcCtrlNum;i++)
		{
			objStoredEnergy.App_SetModuleOnOff(pcuOnoffConfig.acdcCtrlAddr[i] , ACDC_OFF);
		}
				
		
#endif		
		//ObjChargeCtrl.ChargeStopControl();//待机设置
		pcuWorkState.acdcTurnOffAlrdy = 1;
	
	}
	if(pcuWorkState.acdcTurnOffAlrdy == 1)
	{
	
		if(g_st_TimerFlag.b100ms)
		{
			time_acdc_turnoff_wait++;
			
		}
		
		if((time_acdc_turnoff_wait > 30) && (pcuWorkState.acdcTurnOffOk != 1))
		{//ACDC开机失败，重新发送命令
			time_acdc_turnoff_wait = 0;
			pcuWorkState.acdcTurnOffAlrdy =0;
	
		}

	}
	
}


/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::SystemTurnOffOnline(void)
 *  功能描述：并网关机
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
void Class_PCUOnOffControler::SystemTurnOffOnline()
{
	static int16_t pcu_online_turnoff_states = PCU_ONLINE_IDLE1;
	switch(pcu_online_turnoff_states)
	{

		case PCU_ONLINE_IDLE1:
			if(pcuWorkState.systemBmsErr > 3)
			{
                   
		       //错误处理模块进行异常处理	          
		                         
			}
			if((pcuWorkState.systemTurnOff == 1) && (pcuWorkState.systemTurnOffOk != 1))
			{
				
				
				if(pcuWorkState.acdcTurnOnOK == 1)//1级，2级模块控制
				{
					if(pcuOnoffConfig.modelCtrlLevel == 2)
					{
						pcu_online_turnoff_states = PCU_ONLINE_TURNOFF_DC;

					}
					else if(pcuOnoffConfig.modelCtrlLevel == 1)
					{
						pcu_online_turnoff_states = PCU_ONLINE_TURNOFF_AC;

					}
					
				}
		
				     
			}
			else if(pcuWorkState.systemTurnOffOk == 1)
			{
				
			}
		
			break;

		case PCU_ONLINE_TURNOFF_DC:
			if(pcuWorkState.systemDcdcErr == 1)
			{
			
		       //错误处理模块进行异常处理	          
		                         
			}
			
			
			SystemTurnOffDcdc();
						
			if(pcuWorkState.dcdcTurnOffOk == 1)
			{

				pcu_online_turnoff_states = PCU_ONLINE_TURNOFF_AC;
			}
			
			break;

		case PCU_ONLINE_TURNOFF_AC:
			if(pcuWorkState.systemAcdcErr == 1)
			{
				//错误处理模块进行异常处理	          
		                         
			}

			
			SystemTurnOffAcdc();
						
			if(pcuWorkState.acdcTurnOffOk == 1)
			{

				pcu_online_turnoff_states = PCU_ONLINE_TURNOFF_OK;
			}
			
			break;

		case PCU_ONLINE_TURNOFF_OK:
			pcuWorkState.systemTurnOffOk = 1;
			pcuWorkState.dcdcTurnOff = 0;
			pcuWorkState.acdcTurnOff = 0;
			pcuWorkState.systemTurnOff = 0;
			pcuWorkState.systemTurnOnOk = 0;
			pcuWorkState.dcdcTurnOffAlrdy = 0;
			pcuWorkState.acdcTurnOffAlrdy = 0;
			//断开继电器电池供电
			objBMS.App_SetBattOnOff(POWER_OFF);
			//@需要断开多路继电器

	
			//@todo 断开辅助电源
			//@todo 断开并网接触器

			pcuWorkState.systemChargeStateScuCtrl = SCU_CTRL_STOPCHARGE;//关机后恢复默认设置
			pcuWorkState.systemOnOfflineScuCtrl = SCU_CTRL_ONLINE;				
			pcuWorkState.systemDisChargePower = ACDC_POW_DISCHARGE;
			pcuCTLState.systemOnOfflineCtrl = SCU_CTRL_ONLINE;

			pcu_online_turnoff_states = PCU_ONLINE_IDLE1;
			
			break;

		default:
			break;
				
		

	}



		
}



/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::EPOTurnOff(void)
 *  功能描述：EPO关机
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/

void Class_PCUOnOffControler::EPOTurnOff()
{
  if(pcuWorkState.systemEPOTurnOff == 1)
  {
 //   BSP_Relay_Close(ACBREAK_RELAY);//打开并网接触器控制 紧急断电
  //  BSP_Relay_Close(FAN_RELAY);//打开并网接触器控制 紧急断电
    
  }
  
}

/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::SystemTurnOnOffline(void)
 *  功能描述：离网开机
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
void Class_PCUOnOffControler::SystemTurnOnOffline()
{
	static int16_t pcu_offline_turnon_states = PCU_OFFLINE_IDLE;
	switch(pcu_offline_turnon_states)
	{

		case PCU_OFFLINE_IDLE:
			if(pcuWorkState.systemBmsErr > 3)
			{
                 
		       //错误处理模块进行异常处理	         
		                         
			}//收到开机命令pcuWorkState.systemTurnOn = 1
			else if((pcuWorkState.systemTurnOn == 1) && (pcuWorkState.systemTurnOnOk != 1) 
				&& (ObjChargeCtrl.bmsDat.Group_SOC > 20))
			{
				
				objBMS.App_SetBattOnOff(POWER_ON);//吸合BMS继电器
				//@需要吸合多路继电器
				if(pcuOnoffConfig.modelCtrlLevel == 2)
				{
					pcu_offline_turnon_states = PCU_OFFLINE_TURNON_DC;

				}
				else if(pcuOnoffConfig.modelCtrlLevel == 1)
				{
					pcu_offline_turnon_states = PCU_OFFLINE_TURNON_AC;

				}
		             
			}
			else if(pcuWorkState.systemTurnOnOk == 1)
			{
				
			}
		
			break;

		case PCU_OFFLINE_TURNON_DC:
			if((pcuWorkState.systemDcdcErr == 1) || (pcuWorkState.systemBmsErr > 3))
			{
			
		       //错误处理模块进行异常处理	          
		                         
			}
			else
			{
				objBMS.App_SetBattOnOff(POWER_ON);//吸合BMS继电器
				SystemTurnOnDcdc();
							
				if(pcuWorkState.dcdcTurnOnOK == 1)
				{
					pcu_offline_turnon_states = PCU_OFFLINE_TURNON_AC;
				}
			}
			break;

		case PCU_OFFLINE_TURNON_AC:
			if((pcuWorkState.systemAcdcErr == 1) || (pcuWorkState.systemDcdcErr == 1) 
				|| (pcuWorkState.systemBmsErr > 3))
			{
				//错误处理模块进行异常处理	          
		                         
			}
			else
			{
				SystemTurnOnAcdc();
							
				if(pcuWorkState.acdcTurnOnOK == 1)
				{
					pcu_offline_turnon_states = PCU_OFFLINE_TURNON_OK;
				}
			}
			break;

		case PCU_OFFLINE_TURNON_OK:
			pcuWorkState.systemTurnOnOk = 1;
			pcuWorkState.systemTurnOffOk = 0;
            pcuWorkState.bmsTurnOnOk = 0;
		    pcuWorkState.acdcTurnOnAlrdy = 0;
		    pcuWorkState.dcdcTurnOnAlrdy = 0;

			pcu_offline_turnon_states = PCU_OFFLINE_IDLE;
			
			break;

		default:
			break;
				
		

	}

		
        
}

/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::SystemTurnOffOffline(void)
 *  功能描述：离网关机
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
void Class_PCUOnOffControler::SystemTurnOffOffline()
{
	static int16_t pcu_offline_turnoff_states = PCU_OFFLINE_IDLE1;
	switch(pcu_offline_turnoff_states)
	{

		case PCU_OFFLINE_IDLE1:
			if(pcuWorkState.systemBmsErr > 3)
			{
                   
		        //错误处理模块进行异常处理	          
		                         
			}
			if((pcuWorkState.systemTurnOff == 1) && (pcuWorkState.systemTurnOffOk != 1))
			{
				pcu_offline_turnoff_states = PCU_OFFLINE_TURNOFF_AC;       
			}
			else if(pcuWorkState.systemTurnOffOk == 1)
			{
				
			}
		
			break;

		case PCU_OFFLINE_TURNOFF_AC:
			if(pcuWorkState.systemAcdcErr == 1)
			{
			
		        //错误处理模块进行异常处理	          
		                         
			}
		
			SystemTurnOffAcdc();
						
			if(pcuWorkState.acdcTurnOffOk == 1)
			{
				if(pcuOnoffConfig.modelCtrlLevel == 2)
				{
					pcu_offline_turnoff_states = PCU_OFFLINE_TURNOFF_DC;

				}
				else if(pcuOnoffConfig.modelCtrlLevel == 1)
				{
					pcu_offline_turnoff_states = PCU_OFFLINE_TURNOFF_OK;

				}
				
			}
			
			break;

		case PCU_OFFLINE_TURNOFF_DC:
			if(pcuWorkState.systemDcdcErr == 1)
			{
				//错误处理模块进行异常处理	           
		                         
			}

			SystemTurnOffDcdc();
						
			if(pcuWorkState.dcdcTurnOffOk == 1)
			{
				pcu_offline_turnoff_states = PCU_OFFLINE_TURNOFF_OK;
			}
			
			break;

		case PCU_OFFLINE_TURNOFF_OK:
			pcuWorkState.systemTurnOffOk = 1;
			pcuWorkState.dcdcTurnOff = 0;
			pcuWorkState.acdcTurnOff = 0;
			pcuWorkState.systemTurnOff = 0;
			pcuWorkState.systemTurnOnOk = 0;
			pcuWorkState.dcdcTurnOffAlrdy = 0;
			pcuWorkState.acdcTurnOffAlrdy = 0;
			
			//断开继电器电池供电
			objBMS.App_SetBattOnOff(POWER_OFF);
			//@需要断开多路继电器
			//@todo 断开辅助电源
			//@todo 断开并网接触器

			pcuWorkState.systemChargeStateScuCtrl = SCU_CTRL_STOPCHARGE;//关机后恢复默认设置
			pcuWorkState.systemOnOfflineScuCtrl = SCU_CTRL_ONLINE;				
			pcuWorkState.systemDisChargePower = ACDC_POW_DISCHARGE;
			pcuCTLState.systemOnOfflineCtrl = SCU_CTRL_ONLINE;

			pcu_offline_turnoff_states = PCU_OFFLINE_IDLE1;
			
			break;

		default:
			break;
				
		

	}
        
}

/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::SystemRestart(void)
 *  功能描述：系统重启
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
void Class_PCUOnOffControler::SystemRestart()
{
	static int16_t pcu_restart_states = PCU_RESTART_IDLE;
	switch(pcu_restart_states)
	{

		case PCU_RESTART_IDLE:
			if(pcuWorkState.systemRestart == SCU_CTRL_RESTART)
			{
				pcu_restart_states = PCU_RESTART_TURNOFF;
				pcuWorkState.systemRestart = 0;

			}
			else
			{
				//idle
			}
			       
			
		
			break;

		case PCU_RESTART_TURNOFF:
			if(pcuWorkState.systemTurnOffOk == 1)
			{
				pcu_restart_states = PCU_RESTART_OK;
			}
			else
			{
				pcuWorkState.systemTurnOff = 1;
			}
				

			
			break;

		case PCU_RESTART_OK:
			
			objUpgrade.SoftReset();//系统重启
			pcu_restart_states = PCU_RESTART_IDLE;
			
			
			break;
		
		default:
			break;
				
		

	}


}


/*===========================================================================*
 *  函数名称：Class_PCUOnOffControler::SystemSchedule(void)
 *  功能描述：开关机模块工作任务
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/

void Class_PCUOnOffControler::SystemSchedule()
{
		//static uint8_t tempCount;
		EPOTurnOff();//EPO关机
		TurnOnFanDelay();
		TurnOffFanDelay();
		if(g_st_TimerFlag.b100ms)
		{

			SystemStateCheck();//系统状态检测
			SystemErrorResponse();//错误处理
			SystemRestart();//系统重启
			
			if(pcuCTLState.systemOnOfflineCtrl == SCU_CTRL_ONLINE)//并网模式
			{
				SystemTurnOnOnline();
				SystemTurnOffOnline();
			}
			else if(pcuCTLState.systemOnOfflineCtrl == SCU_CTRL_OFFLINE)//离网模式
			{
				SystemTurnOnOffline();
				SystemTurnOffOffline();
			}

			
		}

		if(g_st_TimerFlag.b1s)
		{
			SystemLED_FAN_CTL();
		}
}
                                
/*
void Class_PCUOnOffControler::LEDTurnOn(){
  static	uint8_t countTemp;

 
  if(g_st_TimerFlag.b100ms)
  {                   
    countTemp++;
  
  }
  if(countTemp == 5)
  {
      
      BSP_Relay_Close(RUNLED_RELAY); 
      
  }
  else if(countTemp >= 10)
  {
      countTemp = 0;
      BSP_Relay_Open(RUNLED_RELAY); 
  }

}

*/




