/*==========================================================*/
/* Title		:	Obj_PCUControler.h														*/
/* Description	: 	PCU controler											*/
// Author		:	wangyufei
/* Date			:	2017.10.30														*/
/*==========================================================*/


#include "Obj_PCUControler.h"
#include "Publics.h"


	//开关机等待时间

static	uint16_t time_acdc_turnon_wait;
static	uint16_t time_dcdc_turnon_wait;
static	uint16_t time_acdc_turnoff_wait;
static	uint16_t time_dcdc_turnoff_wait;

static	uint16_t time_bms_wait;
static	uint16_t time_tmp_wait;



void Class_PCUControler::SystemStateCheck()
{
     //读取ACDC状态是否异常
	 if((objFailManager.App_GetFailState(FAIL_ACDC1_FAULT)) || (objFailManager.App_GetFailState(FAIL_ACDC2_FAULT))
	 	|| (objFailManager.App_GetFailState(FAIL_ACDC3_FAULT)))
	 {
		pcuWorkState.systemAcdcErr = 1;
	 }
	 else
	 {
		pcuWorkState.systemAcdcErr = 0;
	 }
     //读取DCDC状态是否异常
	 if((objFailManager.App_GetFailState(FAIL_DCDC1_FAULT)) || (objFailManager.App_GetFailState(FAIL_DCDC2_FAULT))
	 	|| (objFailManager.App_GetFailState(FAIL_DCDC3_FAULT)))
	 {
		pcuWorkState.systemDcdcErr = 1;
	 }
	 else
	 {
		pcuWorkState.systemDcdcErr = 0;
	 }

	 //读取ACDC开关机状态
//	 if((1 == objStoredEnergy.Dat_GetModuleRunState(0)) && (1 == objStoredEnergy.Dat_GetModuleRunState(1))
//	 	&& (1 == objStoredEnergy.Dat_GetModuleRunState(2)))
	 if((objStoredEnergy.Dat_GetModuleRunState(0) > 0) && (objStoredEnergy.Dat_GetModuleRunState(1) > 0))
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
	 if(objStoredEnergy.Dat_GetModuleRunState(3) > 0)
	 {
		pcuWorkState.dcdcTurnOnOK = 1;
		pcuWorkState.dcdcTurnOffOk = 0;
	 }
	 else
	 {
		pcuWorkState.dcdcTurnOnOK = 0;
		pcuWorkState.dcdcTurnOffOk = 1;
	 }

	 //读取BMS状态是否异常
	 pcuWorkState.systemBmsErr = objBMS.App_Batt_StatusFlag();
	 //读取电池断开闭合状态
	 if(SWITCH_ON == objBMS.App_RespContactorStatus())
	 {
		pcuWorkState.bmsTurnOnOk = 1;
	 }
//	pcuWorkState.bmsTurnOnOk = 0;	//for test 
//	pcuWorkState.bmsTurnOffOk = 1; //for test
	 
     //@todo ADC 采样检测是否异常

	 //@todo 读取集控开机信号
	 
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
	 if(E_LOW == BSP_DI_GetState(DI_EPO_FB))
	 {//检测到EPO信号
			pcuWorkState.systemEPOTurnOff = 1;
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
	 
//	  pcuWorkState.acdcTurnOnOK = 1;
//	  pcuWorkState.dcdcTurnOnOK = 1;
//	 if(E_LOW == BSP_DI_GetState(DI_ACBREAK_FB))
//	 {
//			pcuWorkState.acdcTurnOnOK = 1;
//			pcuWorkState.dcdcTurnOnOK = 1;
//			pcuWorkState.acdcTurnOffOk = 1;
//			pcuWorkState.dcdcTurnOffOk = 1;
//			pcuWorkState.bmsTurnOnOk = 1;
	
//	 }

	
	 
}

//模块充电时的参数设置
void Class_PCUControler::AcdcDcdcChargeInit()
{
	uint8_t index;
	acdc_dcdc_workset.acdcWorkmode = 0x01 ;	//设置ACDC工作模式
	acdc_dcdc_workset.acdcOutVolt =760*10 ;	//设置ACDC直流输出电压
	acdc_dcdc_workset.acdcMaxPow =10*10 ;	//设置ACDC最大功率
//	acdc_dcdc_workset.acdcEnergy = ;	//ACDC有功设定
//	acdc_dcdc_workset.acdcReactPow = ;	//ACDC无功设定
//	acdc_dcdc_workset.acdcReactPowFact = ;


	acdc_dcdc_workset.dcdcWorkMode = 0x0003;//恒功率模式
//acdc_dcdc_workset.dcdcOutVolt = bmsDat.BattGroupVolt + 10;
	acdc_dcdc_workset.dcdcOutVolt = 560*10;
	acdc_dcdc_workset.dcdcCurrent = 10*10;
	acdc_dcdc_workset.dcdcMaxPow = 6*10;
}



//模块充电时的参数设置
void Class_PCUControler::AcdcDcdcChargeSet()
{
	uint8_t index;
	acdc_dcdc_workset.acdcWorkmode = 0x01 ;	//设置ACDC工作模式
	acdc_dcdc_workset.acdcOutVolt =760*10 ;	//设置ACDC直流输出电压
	acdc_dcdc_workset.acdcMaxPow =10*10 ;	//设置ACDC最大功率
//	acdc_dcdc_workset.acdcEnergy = ;	//ACDC有功设定
//	acdc_dcdc_workset.acdcReactPow = ;	//ACDC无功设定
//	acdc_dcdc_workset.acdcReactPowFact = ;


	acdc_dcdc_workset.dcdcWorkMode = 0x0003;//恒功率模式
//acdc_dcdc_workset.dcdcOutVolt = bmsDat.BattGroupVolt + 10;
	acdc_dcdc_workset.dcdcOutVolt = 560*10;
	acdc_dcdc_workset.dcdcCurrent = 10*10;
	acdc_dcdc_workset.dcdcMaxPow = 6*10;
}

//模块停止充电时的参数设置
void Class_PCUControler::AcdcDcdcChargeStop()
{
	uint8_t index;
	acdc_dcdc_workset.acdcWorkmode = 0x01 ;	//设置ACDC工作模式
	acdc_dcdc_workset.acdcOutVolt =760*10 ;	//设置ACDC直流输出电压
	acdc_dcdc_workset.acdcMaxPow =10*10 ;	//设置ACDC最大功率
//	acdc_dcdc_workset.acdcEnergy = ;	//ACDC有功设定
//	acdc_dcdc_workset.acdcReactPow = ;	//ACDC无功设定
//	acdc_dcdc_workset.acdcReactPowFact = ;


	acdc_dcdc_workset.dcdcWorkMode = 0x0003;//恒功率模式
	acdc_dcdc_workset.dcdcOutVolt = bmsDat.BattGroupVolt*10;
	//acdc_dcdc_workset.dcdcOutVolt = 560*10;
	acdc_dcdc_workset.dcdcCurrent = 0;
	acdc_dcdc_workset.dcdcMaxPow = 6*10;
}



//模块放电时的参数设置
void Class_PCUControler::AcdcDcdcDischargeSet()
{
	uint8_t index;
	acdc_dcdc_workset.acdcWorkmode = 0x01 ; //设置ACDC工作模式
	acdc_dcdc_workset.acdcOutVolt =760*10 ; //设置ACDC直流输出电压
	acdc_dcdc_workset.acdcMaxPow =10*10 ;	//设置ACDC最大功率
//	acdc_dcdc_workset.acdcEnergy = ;	//ACDC有功设定
//	acdc_dcdc_workset.acdcReactPow = ;	//ACDC无功设定
//	acdc_dcdc_workset.acdcReactPowFact = ;


	acdc_dcdc_workset.dcdcWorkMode = 0x0003;//恒功率模式
	acdc_dcdc_workset.dcdcOutVolt = 560*10;
	acdc_dcdc_workset.dcdcCurrent = 10*10;
	acdc_dcdc_workset.dcdcMaxPow = 6*10;


}


//模块停止放电时的参数设置
void Class_PCUControler::AcdcDcdcDischargeStop()
{

	acdc_dcdc_workset.acdcWorkmode = 0x01 ; //设置ACDC工作模式
	acdc_dcdc_workset.acdcOutVolt =760*10 ; //设置ACDC直流输出电压
	acdc_dcdc_workset.acdcMaxPow =10*10 ;	//设置ACDC最大功率
//	acdc_dcdc_workset.acdcEnergy = ;	//ACDC有功设定
//	acdc_dcdc_workset.acdcReactPow = ;	//ACDC无功设定
//	acdc_dcdc_workset.acdcReactPowFact = ;


	acdc_dcdc_workset.dcdcWorkMode = 0x0003;//恒功率模式
	acdc_dcdc_workset.dcdcOutVolt = 560*10;
	acdc_dcdc_workset.dcdcCurrent = 10*10;
	acdc_dcdc_workset.dcdcMaxPow = 6*10;


}


void Class_PCUControler::AcdcDcdcSet()//acdc dcdc 模块参数设置
{

	uint8_t index;

	for(index = 0;index<3;index++)
	{
		objStoredEnergy.APP_SetACDCWorkMode(index+MAC_ID_DCAC0,acdc_dcdc_workset.acdcWorkmode);	//设置ACDC工作模式
		objStoredEnergy.APP_SetACDCOutVolt(index+MAC_ID_DCAC0,acdc_dcdc_workset.acdcOutVolt); //设置ACDC直流输出电压
		objStoredEnergy.APP_SetACDCMaxPow(index+MAC_ID_DCAC0,acdc_dcdc_workset.acdcMaxPow);	//设置ACDC最大功率
		//objStoredEnergy.APP_SetACDCEnergy(MAC_ID_DCAC0,acdc_dcdc_workset.acdcEnergy);	//ACDC有功设定
		//objStoredEnergy.APP_SetACDCReactPow(MAC_ID_DCAC0,acdc_dcdc_workset.acdcReactPow);	//ACDC无功设定
		//objStoredEnergy.APP_SetACDCReactPowFact(MAC_ID_DCAC0,acdc_dcdc_workset.acdcReactPowFact);	//ACDC无功功率因数设定
		objStoredEnergy.APP_SetDCDCWorkMode(index+MAC_ID_BATT_DCDC0,acdc_dcdc_workset.dcdcWorkMode);	//设置DCDC工作模式
		objStoredEnergy.APP_SetDCDCOutVolt(index+MAC_ID_BATT_DCDC0,acdc_dcdc_workset.dcdcOutVolt);	//设置DCDC直流输出电压
		objStoredEnergy.APP_SetDCDCMaxPow(index+MAC_ID_BATT_DCDC0,acdc_dcdc_workset.dcdcMaxPow);	//设置DCDC最大功率
		objStoredEnergy.APP_SetDCDCCurrent(index+MAC_ID_BATT_DCDC0,acdc_dcdc_workset.dcdcCurrent);	//设置DCDC电流

	}


}

uint8_t Class_PCUControler::AcdcDcdcRead()//acdc dcdc 模块参数设置
{
	uint8_t result1 = 0;
	uint8_t result2 = 0;
	uint8_t result3 = 0;
	uint8_t result4 = 0;
	uint8_t result5 = 0;
	uint8_t result6 = 0;
	uint8_t result7 = 0;
	
	result1 = objStoredEnergy.APP_GetACDCWorkMode(MAC_ID_DCAC0,acdc_dcdc_workset.acdcWorkmode);	//读ACDC工作模式
	result2 = objStoredEnergy.APP_GetACDCOutVolt(MAC_ID_DCAC0,acdc_dcdc_workset.acdcOutVolt);	//读ACDC直流输出电压
	result3 = objStoredEnergy.APP_GetACDCMaxPow(MAC_ID_DCAC0,acdc_dcdc_workset.acdcMaxPow);	//读ACDC最大功率
	//objStoredEnergy.APP_GetACDCEnergy(MAC_ID_DCAC0,acdc_dcdc_workset.acdcEnergy);	//读ACDC有功
	//objStoredEnergy.APP_GetACDCReactPow(MAC_ID_DCAC0,acdc_dcdc_workset.acdcReactPow);	//读ACDC无功
	//objStoredEnergy.APP_GetACDCReactPowFact(MAC_ID_DCAC0,acdc_dcdc_workset.acdcReactPowFact);	//读ACDC无功功率因数
	result4 = objStoredEnergy.APP_GetDCDCWorkMode(MAC_ID_BATT_DCDC0,acdc_dcdc_workset.dcdcWorkMode);	//读DCDC工作模式
	result5 = objStoredEnergy.APP_GetDCDCOutVolt(MAC_ID_BATT_DCDC0,acdc_dcdc_workset.dcdcOutVolt);	//读DCDC直流输出电压
	result6 = objStoredEnergy.APP_GetDCDCMaxPow(MAC_ID_BATT_DCDC0,acdc_dcdc_workset.dcdcMaxPow);	//读DCDC最大功率
	result7 = objStoredEnergy.APP_GetDCDCCurrent(MAC_ID_BATT_DCDC0,acdc_dcdc_workset.dcdcCurrent);	//读DCDC电流

	if((result1 == 1) && (result2 == 1) && (result3 == 1) && (result4 == 1)
		&& (result5 == 1) && (result6 == 1) && (result7 == 1))
	{
		return 1;
	}
	else
	{
		return 0;
	}

}

void Class_PCUControler::bmsDatRead()//bms参数读取
{
	objBMS.App_Get_BattDataInfo(&ObjChargeCtrl.bmsDat);

//objStoredEnergy.App_SetBattOnOff(POWER_ON);




}



void Class_PCUControler::Dat_Init()
{
        time_acdc_turnon_wait = 0;
        time_acdc_turnoff_wait = 0;
		time_dcdc_turnon_wait = 0;
        time_dcdc_turnoff_wait = 0;
        time_bms_wait = 0;
        time_tmp_wait = 0;

		AcdcDcdcChargeInit();

		memset(&pcuWorkState, 0, sizeof(_PcuWorkState));
        
}

void Class_PCUControler::SystemTurnOnAcdc()
{
	
	if(pcuWorkState.systemAcdcErr == 1)//ACDC状态异常
	{
			//@todo//上报异常
			pcuWorkState.acdcTurnOn = 0;
			
	
	}//收到开机命令pcuWorkState.acdcTurnOn = 1
	else if((pcuWorkState.acdcTurnOn == 1) && (pcuWorkState.acdcTurnOnAlrdy != 1))
	{	
//		for(u8 i = 0;i<3;i++)
//		{
			objStoredEnergy.App_SetModuleOnOff(MAC_ID_DCAC0, ACDC_ON);
//		}
		pcuWorkState.acdcTurnOnAlrdy = 1;
	
	}
	if(pcuWorkState.acdcTurnOnAlrdy == 1)
	{
	
		if(g_st_TimerFlag.b100ms)
		{
			time_acdc_turnon_wait++;
		}
		//@todo ACDC开机成功,pcuWorkState.acdcTurnOnOK = 1
		if((time_acdc_turnon_wait > 30) && (pcuWorkState.acdcTurnOnOK != 1))
		{//ACDC开机失败，重新发送命令
			time_acdc_turnon_wait = 0;
			pcuWorkState.acdcTurnOnAlrdy =0;
	
		}
/*		else if(pcuWorkState.acdcTurnOnOK == 1){
			pcuWorkState.acdcTurnOn = 0;
			pcuWorkState.acdcTurnOnAlrdy = 0;
			pcuTimeWait.time_acdc_wait = 0;
		}*/
	}



}


void Class_PCUControler::SystemTurnOnDcdc()
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
//			for(u8 i = 0;i<3;i++)
//			{
				objStoredEnergy.App_SetModuleOnOff(MAC_ID_BATT_DCDC0, ACDC_ON);
//			}
			pcuWorkState.dcdcTurnOnAlrdy = 1;
		
		}
		if(pcuWorkState.dcdcTurnOnAlrdy == 1)
		{
		
			if(g_st_TimerFlag.b100ms)
			{
				time_dcdc_turnon_wait++;
			}
			//@todo DCDC开机成功,pcuWorkState.dcdcTurnOnOK = 1
			if((time_dcdc_turnon_wait > 30) && (pcuWorkState.dcdcTurnOnOK != 1))
			{//DCDC开机失败，重新发送命令
				time_dcdc_turnon_wait = 0;
				pcuWorkState.dcdcTurnOnAlrdy =0;
		
			}
/*			else if(pcuWorkState.dcdcTurnOnOK == 1){
				pcuWorkState.dcdcTurnOn = 0;
				pcuWorkState.dcdcTurnOnAlrdy = 0;
				pcuTimeWait.time_dcdc_wait = 0;
			}*/
		}


	}
	

}

void Class_PCUControler::SystemTurnOnOnline()
{
	//	@todo  //读取电池信息

	//@todo ADC检测 IF ACDC测没有电，就打开继电器
	//BSP_Relay_Close(ACBREAK_RELAY);//闭合并网接触器控制//闭合继电器ACDC供电

	if((pcuWorkState.systemAcdcErr == 1) || (pcuWorkState.systemDcdcErr == 1) || (pcuWorkState.systemBmsErr > 3))
	{
			//@todo 异常处理上报？
			pcuWorkState.systemTurnOn = 0;
                        
            BSP_Relay_Close(FAULTLED_RELAY); //for test           
                         
	}//收到开机命令pcuWorkState.systemTurnOn = 1
	else if((pcuWorkState.systemTurnOn == 1) && (pcuWorkState.systemTurnOnOk != 1))
	{
		BSP_Relay_Close(RUNLED_RELAY);
                
		//@TODO 设置ACDC方向及电流
		pcuWorkState.acdcTurnOn = 1;
		if(pcuWorkState.acdcTurnOnOK != 1)
		{
			SystemTurnOnAcdc(); 
		}
		else//变量清零
		{
			pcuWorkState.acdcTurnOn = 0;
			pcuWorkState.acdcTurnOnAlrdy = 0;
			time_acdc_turnon_wait = 0;

			if(pcuWorkState.acdcDcdcSetOk != 1)
			{
				
				AcdcDcdcSet();//@todo设置模块参数
				pcuWorkState.acdcDcdcSetOk = 1;
			}

			
		}
		
		if((pcuWorkState.acdcTurnOnOK == 1) && (pcuWorkState.acdcDcdcSetOk == 1) &&(pcuWorkState.dcdcTurnOnOK != 1))
		{

			SystemTurnOnDcdc(); 
		}
		else if(pcuWorkState.dcdcTurnOnOK == 1)
		{
			pcuWorkState.dcdcTurnOn = 0;
			pcuWorkState.dcdcTurnOnAlrdy = 0;
			time_dcdc_turnon_wait = 0;
		}
		
		
		if(pcuWorkState.dcdcTurnOnOK == 1)
		{
			if(pcuWorkState.systemBmsErr>3)
			{
				//电池故障达到4级
				//@todo 上报集控电池有问题，开机失败
				pcuWorkState.systemTurnOn = 0;
				pcuWorkState.acdcTurnOnOK = 0;
				pcuWorkState.dcdcTurnOnOK = 0;
				pcuWorkState.systemTurnOnOk = 0;
			}
			else
			{
				pcuWorkState.systemTurnOnOk = 1;
			}

			
		}
        if(pcuWorkState.systemTurnOnOk == 1)
		{
                //@todo 上报集控并网开机成功
                pcuWorkState.systemTurnOffOk = 0;
               // pcuWorkState.systemTurnOn = 0;
               // pcuWorkState.acdcTurnOnOK = 0;
               // pcuWorkState.dcdcTurnOnOK = 0;
                //pcuWorkState.systemTurnOnOk = 0;
                pcuWorkState.bmsTurnOnOk = 0;
			    pcuWorkState.acdcTurnOnAlrdy = 0;
			    pcuWorkState.dcdcTurnOnAlrdy = 0;
                BSP_Relay_Close(RUNLED_RELAY); 

				AcdcDcdcSet();//@todo设置模块参数
				BSP_Relay_Close(ACBREAK_RELAY);//打开风机
		}

		
	}


			
	
 
}


void Class_PCUControler::SystemTurnOffDcdc()
{	

	if(pcuWorkState.dcdcTurnOff != 1)//DCDC状态异常
	{
			//@todo//不关机
			
			
	
	}
	else if((pcuWorkState.dcdcTurnOff == 1) && (pcuWorkState.dcdcTurnOffAlrdy != 1))
	{	
//		for(u8 i = 0;i<3;i++)
//		{
			objStoredEnergy.App_SetModuleOnOff(MAC_ID_BATT_DCDC0, ACDC_OFF);
//		}
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
		else if(pcuWorkState.dcdcTurnOffOk == 1){
			pcuWorkState.dcdcTurnOff = 0;
			pcuWorkState.dcdcTurnOffAlrdy = 0;
			time_dcdc_turnoff_wait = 0;
		}
	}


}

void Class_PCUControler::SystemTurnOffAcdc()
{
	if(pcuWorkState.acdcTurnOff != 1)//ACDC状态异常
	{
			//不关闭
			
	
	}//@todo 收到开机命令pcuWorkState.acdcTurnOn = 1
	else if((pcuWorkState.acdcTurnOff == 1) && (pcuWorkState.acdcTurnOffAlrdy != 1))
	{	//ACDC关机机   
//		for(u8 i = 0;i<3;i++)
//		{
			objStoredEnergy.App_SetModuleOnOff(MAC_ID_DCAC0, ACDC_OFF);
//		}
		pcuWorkState.acdcTurnOffAlrdy = 1;
	
	}
	if(pcuWorkState.acdcTurnOffAlrdy == 1)
	{
	
		if(g_st_TimerFlag.b100ms)
		{
			time_acdc_turnoff_wait++;
			
		}
		//@todo ACDC关机成功,pcuWorkState.acdcTurnOffOK = 1
		if((time_acdc_turnoff_wait > 30) && (pcuWorkState.acdcTurnOffOk != 1))
		{//ACDC开机失败，重新发送命令
			time_acdc_turnoff_wait = 0;
			pcuWorkState.acdcTurnOffAlrdy =0;
	
		}
		else if(pcuWorkState.acdcTurnOffOk == 1){
			pcuWorkState.acdcTurnOff = 0;
			pcuWorkState.acdcTurnOffAlrdy = 0;
			time_acdc_turnoff_wait = 0;
		}
	}
	
}


void Class_PCUControler::SystemTurnOffOnline()
{
	if((pcuWorkState.systemAcdcErr == 1) || (pcuWorkState.systemDcdcErr == 1) || (pcuWorkState.systemBmsErr > 3))
	{
			//@todo 异常处理上报？

	}//@todo 收到开机命令pcuWorkState.systemTurnOff = 1                
    else
    {
      BSP_Relay_Open(FAULTLED_RELAY); //for test
    }
        
        
	if((pcuWorkState.systemTurnOff == 1) && (pcuWorkState.systemTurnOffOk != 1))
	{	
		if(pcuWorkState.dcdcTurnOffOk != 1)
		{
			pcuWorkState.dcdcTurnOff = 1;
			SystemTurnOffDcdc();
		}
		else if(pcuWorkState.dcdcTurnOffOk == 1)
		{

		
			//断开继电器电池供电
			objBMS.App_SetBattOnOff(POWER_OFF);
			
			
			if(pcuWorkState.bmsTurnOffOk == 1)//电池已断开@todo处理
			{
				pcuWorkState.dcdcTurnOffOk = 0;
				pcuWorkState.acdcTurnOff = 1;
			}
		}
		if(pcuWorkState.acdcTurnOff == 1)
		{
			SystemTurnOffAcdc();
		}
		if(pcuWorkState.acdcTurnOffOk == 1)
		{
			pcuWorkState.systemTurnOffOk = 1;
                        
		}
		if(pcuWorkState.systemTurnOffOk == 1)
		{
			pcuWorkState.dcdcTurnOff = 0;
			pcuWorkState.acdcTurnOff = 0;
			pcuWorkState.systemTurnOff = 0;
			pcuWorkState.systemTurnOnOk = 0;
			pcuWorkState.dcdcTurnOffAlrdy = 0;
			pcuWorkState.acdcTurnOffAlrdy = 0;
            BSP_Relay_Open(RUNLED_RELAY); 
			//@todo 断开并网接触器
			//@todo	断开辅助电源
			//@todo 上报并网关机状态
			BSP_Relay_Open(ACBREAK_RELAY);//关闭风机
		}
	}


        
}

uint8_t tempCount1 = 0; 

void Class_PCUControler::ChargeControl()//充电控制
{
	
//	if(g_st_TimerFlag.b1s)
//	{
//		tempCount1++;
//	}
//	if(tempCount1 > 30)
//	{
		objBMS.App_SetBattOnOff(POWER_ON);
//	}
	

	bmsDatRead();
	
	
	AcdcDcdcChargeSet();	
	if(AcdcDcdcRead() != 1)//参数设置不对
	{
		

		AcdcDcdcSet();//@todo设置模块参数
	}
	else if((pcuWorkState.systemDcdcErr != 1) && (pcuWorkState.systemBmsErr != 1)
		&& (bmsDat.Group_SOC <= 95))
	{
		//objStoredEnergy.App_SetBattOnOff(POWER_ON);
		
		//AcdcDcdcSet();//@todo设置模块参数

		
	}

	if(bmsDat.Group_SOC > 95)
	{
		AcdcDcdcChargeStop();
		AcdcDcdcSet();//@todo停止充电

	}


}


void Class_PCUControler::DischargeControl()//放电控制
{

	if(g_st_TimerFlag.b1s)
	{
		tempCount1++;
	}
	if(tempCount1 > 60)
	{
		objBMS.App_SetBattOnOff(POWER_ON);
	}
	

	bmsDatRead();

		
	if((pcuWorkState.systemDcdcErr != 1) && (pcuWorkState.systemBmsErr != 1)
		&& (bmsDat.Group_SOC >= 30))
	{
		//@todo 打开DCDC
		//@todo 打开ACDC

		//@todo如果ACDC,DCDC已经打开,并且参数设置成功
		AcdcDcdcDischargeSet();
		AcdcDcdcSet();//@todo设置模块参数

		
	}



	if(bmsDat.Group_SOC < 30)
	{
		AcdcDcdcDischargeStop();
		AcdcDcdcSet();//@todo设置模块参数 停止放电
		
	}

			



}



void Class_PCUControler::EPOTurnOff()
{
  if(pcuWorkState.systemEPOTurnOff == 1)
  {
 //   BSP_Relay_Close(ACBREAK_RELAY);//打开并网接触器控制 紧急断电
    BSP_Relay_Close(FAN_RELAY);//打开并网接触器控制 紧急断电
    
  }
  
}


void Class_PCUControler::SystemTurnOnOffline()
{
	
        
}

void Class_PCUControler::SystemTurnOffOffline()
{
	
        
}

void Class_PCUControler::SystemSchedule()
{
	
            EPOTurnOff();
			SystemStateCheck();
			SystemTurnOnOnline();
			SystemTurnOffOnline();
			if((pcuWorkState.systemTurnOnOk) == 1 && (pcuWorkState.systemTurnOff != 1)){
				ChargeControl();
			}




	

        
}
                                
 uint8_t countTemp = 0;
void Class_PCUControler::LEDTurnOn(){

 
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





