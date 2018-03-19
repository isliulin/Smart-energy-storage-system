
/*==========================================================*/
/* Title		:	Obj_ChargeControler.h														*/
/* Description	: 	charge controler											*/
// Author		:	wangyufei
/* Date			:	2017.10.30														*/
/*==========================================================*/


#include  "Publics.h"



/*===========================================================================*
 *  函数名称：Class_ChargeControler::bmsDatRead(void)
 *  功能描述：读取电池状态
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/

void Class_ChargeControler::bmsDatRead()//bms参数读取
{
	objBMS.App_Get_BattDataInfo(&bmsDat);
//@需要读取多路电池信息
	if(bmsDat.BattGroupVolt<200)	//数据保护，如果数据为异常值，则赋予典型值，避免根据功率算出的电流过大。
	{
		bmsDat.BattGroupVolt = 600;
	}
	if(bmsDat.Group_SOC == 0)
	{
		bmsDat.Group_SOC = 20;
	}

}


/*===========================================================================*
 *  函数名称：Class_ChargeControler::AcdcDcdcChargeInit(void)
 *  功能描述：模块初始化参数设置
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
void Class_ChargeControler::AcdcDcdcChargeInit()
{
#ifdef PCU_CONFIG_ENABLE
	if(ObjPcuOnOffCtrl.pcuOnoffConfig.modelCtrlLevel == 2)
	{
		acdc_dcdc_workset.acdcWorkmode = ACDC_WORK_MODE_VOLT ;	//设置ACDC工作模式
		acdc_dcdc_workset.acdcOutVolt =ACDC_OUT_VOLT*10 ;	//设置ACDC直流输出电压
		acdc_dcdc_workset.acdcMaxPow =ACDC_MAX_POW*10 ;	//设置ACDC最大功率
	//	acdc_dcdc_workset.acdcEnergy = ;	//ACDC有功设定
	//	acdc_dcdc_workset.acdcReactPow = ;	//ACDC无功设定
	//	acdc_dcdc_workset.acdcReactPowFact = ;
		acdc_dcdc_workset.dcdcWorkMode = DCDC_WORK_MODE_CURRENT;//恒功率模式
		acdc_dcdc_workset.dcdcOutVolt = DCDC_OUT_VLOT*10;
		acdc_dcdc_workset.dcdcCurrent = DCDC_OUT_CURRENT_STOP*10;
		acdc_dcdc_workset.dcdcMaxPow = DCDC_MAX_POW*10;
	}
	else if(ObjPcuOnOffCtrl.pcuOnoffConfig.modelCtrlLevel == 1)
	{//@添加1级设置
		acdc_dcdc_workset.acdcWorkmode = ACDC_WORK_MODE_DCPOWER ;	//设置ACDC工作模式
		acdc_dcdc_workset.acdcCurrent = DCDC_OUT_CURRENT_STOP;//设置电流
		
}
	
#else
	uint8_t i;
	if(ObjPcuOnOffCtrl.pcuOnoffConfig.modelCtrlLevel == 2)
	{

		
		for(i = 0;i < ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlNum ;i++)
		{
			acdc_dcdc_modelset[i].acdcWorkmode = ACDC_WORK_MODE_VOLT ;	//设置ACDC工作模式
			acdc_dcdc_modelset[i].acdcCurrent = DCDC_OUT_CURRENT_STOP;//设置电流
			acdc_dcdc_modelset[i].acdcEnergy = ACDC_MAX_POW_STOP;//设置功率
			acdc_dcdc_modelset[i].acdcMaxChargePow = ACDC_LIMIT_POW*10;//设置最大功率
			acdc_dcdc_modelset[i].acdcMaxDischargePow = ACDC_LIMIT_POW*10;//设置最大功率
			acdc_dcdc_modelset[i].acdcMaxChargeCur = ACDC_LIMIT_CUR*10;//设置最大电流
			acdc_dcdc_modelset[i].acdcMaxDischargeCur = ACDC_LIMIT_CUR*10;//设置最大电流
			acdc_dcdc_modelset[i].acdcOutVolt = ACDC_OUT_VOLT*10;//设置工作电压
		}

		for(i = 0;i < ObjPcuOnOffCtrl.pcuOnoffConfig.dcdcCtrlNum ;i++)
		{
			acdc_dcdc_modelset[i].dcdcWorkMode = DCDC_WORK_MODE_CURRENT;//恒功率模式
			acdc_dcdc_modelset[i].dcdcOutVolt = DCDC_OUT_VLOT*10;
			acdc_dcdc_modelset[i].dcdcCurrent = DCDC_OUT_CURRENT_STOP*10;
			acdc_dcdc_modelset[i].dcdcMaxPow = DCDC_MAX_POW*10;
		}
	}
	else if(ObjPcuOnOffCtrl.pcuOnoffConfig.modelCtrlLevel == 1)
	{//@添加1级设置
		for(i = 0;i < ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlNum ;i++)
		{
				acdc_dcdc_modelset[i].acdcWorkmode = ACDC_WORK_MODE_VOLT ;	//设置ACDC工作模式
				acdc_dcdc_modelset[i].acdcCurrent = DCDC_OUT_CURRENT_STOP;//设置电流
				acdc_dcdc_modelset[i].acdcEnergy = ACDC_MAX_POW_STOP;//设置功率
				acdc_dcdc_modelset[i].acdcMaxChargePow = ACDC_LIMIT_POW*10;//设置最大功率
				acdc_dcdc_modelset[i].acdcMaxDischargePow = ACDC_LIMIT_POW*10;//设置最大功率
				acdc_dcdc_modelset[i].acdcMaxChargeCur = ACDC_LIMIT_CUR*10;//设置最大电流
				acdc_dcdc_modelset[i].acdcMaxDischargeCur = ACDC_LIMIT_CUR*10;//设置最大电流
				acdc_dcdc_modelset[i].acdcOutVolt = ACDC_OUT_VOLT*10;//设置工作电压
	
		}

	}

#endif

}

void Class_ChargeControler::AcdcDcdcDebugInit()//for debug model
{
	uint8_t i;

	for(i = 0;i < ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlNum ;i++)
	{
			acdc_dcdc_debugModelset[i].acdcWorkmode = ACDC_WORK_MODE_VOLT ;	//设置ACDC工作模式
			acdc_dcdc_debugModelset[i].acdcCurrent = DCDC_OUT_CURRENT_STOP;//设置电流
			acdc_dcdc_debugModelset[i].acdcEnergy = ACDC_MAX_POW_STOP;//设置功率
			acdc_dcdc_debugModelset[i].acdcMaxChargePow = ACDC_LIMIT_POW*10;//设置最大功率
			acdc_dcdc_debugModelset[i].acdcMaxDischargePow = ACDC_LIMIT_POW*10;//设置最大功率
			acdc_dcdc_debugModelset[i].acdcMaxChargeCur = ACDC_LIMIT_CUR*10;//设置最大电流
			acdc_dcdc_debugModelset[i].acdcMaxDischargeCur = ACDC_LIMIT_CUR*10;//设置最大电流
	
	}

	for(i = 0;i < ObjPcuOnOffCtrl.pcuOnoffConfig.dcdcCtrlNum ;i++)
	{
			acdc_dcdc_debugModelset[i].dcdcWorkMode = DCDC_WORK_MODE_CURRENT;//恒功率模式
			acdc_dcdc_debugModelset[i].dcdcOutVolt = DCDC_OUT_VLOT*10;
			acdc_dcdc_debugModelset[i].dcdcCurrent = DCDC_OUT_CURRENT_STOP*10;
			acdc_dcdc_debugModelset[i].dcdcMaxPow = DCDC_MAX_POW*10;
	}

}




/*===========================================================================*
 *  函数名称：Class_ChargeControler::AcdcDcdcChargeSet(void)
 *  功能描述：模块充电参数设置
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
void Class_ChargeControler::AcdcDcdcChargeSet()
{
	float currentTemp;

#ifdef PCU_CONFIG_ENABLE

	if(ObjPcuOnOffCtrl.pcuOnoffConfig.modelCtrlLevel == 2)
	{
		
		acdc_dcdc_workset.acdcWorkmode = ACDC_WORK_MODE_VOLT ;	//设置ACDC工作模式
		acdc_dcdc_workset.acdcOutVolt =ACDC_OUT_VOLT*10 ;	//设置ACDC直流输出电压
		acdc_dcdc_workset.acdcMaxPow =ACDC_MAX_POW*10 ;	//设置ACDC最大功率
	//	acdc_dcdc_workset.acdcEnergy = ;	//ACDC有功设定
	//	acdc_dcdc_workset.acdcReactPow = ;	//ACDC无功设定
	//	acdc_dcdc_workset.acdcReactPowFact = ;
		acdc_dcdc_workset.dcdcWorkMode = DCDC_WORK_MODE_CURRENT;//恒功率模式
		currentTemp = objBMS.App_RespMaxChgCurr();
		currentTemp = MIN(currentTemp, BMS_MAX_CURRENT);//@根据模块个数限制最大电流
		acdc_dcdc_workset.dcdcCurrent = (int16_t)(currentTemp)*-10;
		acdc_dcdc_workset.dcdcMaxPow = DCDC_MAX_POW*10;
	}
	else if(ObjPcuOnOffCtrl.pcuOnoffConfig.modelCtrlLevel == 1)
	{//@添加1级设置
		acdc_dcdc_workset.acdcWorkmode = ACDC_WORK_MODE_CUR ;	//设置ACDC工作模式
		currentTemp = objBMS.App_RespMaxChgCurr();
		currentTemp = MIN(currentTemp, BMS_MAX_CURRENT);//@根据模块个数限制最大电流
		acdc_dcdc_workset.acdcCurrent = (int16_t)(currentTemp)*-10;//设置电流
		//acdc_dcdc_workset.acdcMaxPow =ACDC_MAX_POW*10*ObjPcuOnOffCtrl.pcuCTLState.systemPowerLimit/100 ;	//设置ACDC最大功率

	}
#else
	uint8_t i;
	if(ObjPcuOnOffCtrl.pcuOnoffConfig.modelCtrlLevel == 2)
	{
		
		for(i = 0;i < ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlNum;i++)
		{
			
			acdc_dcdc_modelset[i].acdcWorkmode = ACDC_WORK_MODE_VOLT ;	//设置ACDC工作模式
			acdc_dcdc_modelset[i].acdcOutVolt =ACDC_OUT_VOLT*10 ;	//设置ACDC直流输出电压
			//acdc_dcdc_modelset[i].acdcEnergy = ObjPcuOnOffCtrl.pcuWorkState.systemDisChargePower*-1;//设置并网放电功率			


		}

		for(i = 0;i < ObjPcuOnOffCtrl.pcuOnoffConfig.dcdcCtrlNum ;i++)
		{
			acdc_dcdc_modelset[i].dcdcOutVolt = DCDC_OUT_VLOT*10;
			acdc_dcdc_modelset[i].dcdcWorkMode = DCDC_WORK_MODE_CURRENT;//恒功率模式
			acdc_dcdc_modelset[i].dcdcMaxPow = DCDC_MAX_POW*10;
			//currentTemp = objBMS.App_RespMaxChgCurr();//@根据不同的电池设置电流
			//currentTemp = MIN(currentTemp, BMS_MAX_CURRENT);//@根据模块个数限制最大电流
			currentTemp = bmsDat.BattGroupVolt;//电池组电压
			acdc_dcdc_modelset[i].dcdcCurrent = ObjPcuOnOffCtrl.pcuWorkState.systemDisChargePower*1000
				/bmsDat.BattGroupVolt;//根据功率设置计算电流设置
			acdc_dcdc_modelset[i].dcdcCurrent = MIN(acdc_dcdc_modelset[i].dcdcCurrent, BMS_MAX_CURRENT);
			acdc_dcdc_modelset[i].dcdcCurrent = acdc_dcdc_modelset[i].dcdcCurrent*-1*ObjPcuOnOffCtrl.pcuCTLState.systemPowerLimit;//系统功率限制
		}
	}
	else if(ObjPcuOnOffCtrl.pcuOnoffConfig.modelCtrlLevel == 1)
	{//并网充电，恒流模式

		for(i = 0;i < ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlNum;i++)
		{
			
			acdc_dcdc_modelset[i].acdcWorkmode = ACDC_WORK_MODE_DCPOWER ;	//设置ACDC工作模式
			currentTemp = objBMS.App_RespMaxChgCurr();//@todu计算不同电池的输入电流
			currentTemp = MIN(currentTemp, BMS_MAX_CURRENT);//@根据模块个数限制最大电流
			//acdc_dcdc_modelset[i].acdcCurrent = (int16_t)(currentTemp)*-10;//设置电流
			//acdc_dcdc_modelset[i].acdcEnergy = acdc_dcdc_modelset[i].acdcCurrent*ACDC_OUT_VOLT/1000;//根据电流算功率，换算成KW
			acdc_dcdc_modelset[i].acdcEnergy = ObjPcuOnOffCtrl.pcuWorkState.systemDisChargePower*-1;//设置并网放电功率
		}

	}

#endif

}

/*===========================================================================*
 *  函数名称：Class_ChargeControler::AcdcDcdcChargeStop(void)
 *  功能描述：模块停止充电参数设置
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
void Class_ChargeControler::AcdcDcdcChargeStop()
{

#ifdef PCU_CONFIG_ENABLE
	if(ObjPcuOnOffCtrl.pcuOnoffConfig.modelCtrlLevel == 2)
	{
		acdc_dcdc_workset.acdcWorkmode = ACDC_WORK_MODE_VOLT ;	//设置ACDC工作模式
		acdc_dcdc_workset.acdcOutVolt =ACDC_OUT_VOLT*10 ;	//设置ACDC直流输出电压
		acdc_dcdc_workset.acdcMaxPow =ACDC_MAX_POW*10 ;	//设置ACDC最大功率
	//	acdc_dcdc_workset.acdcEnergy = ;	//ACDC有功设定
	//	acdc_dcdc_workset.acdcReactPow = ;	//ACDC无功设定
	//	acdc_dcdc_workset.acdcReactPowFact = ;


		acdc_dcdc_workset.dcdcWorkMode = DCDC_WORK_MODE_CURRENT;//恒功率模式
		acdc_dcdc_workset.dcdcCurrent = DCDC_OUT_CURRENT_STOP;
		acdc_dcdc_workset.dcdcMaxPow = DCDC_MAX_POW*10;
	}
	else if(ObjPcuOnOffCtrl.pcuOnoffConfig.modelCtrlLevel == 1)
	{//@添加1级设置

		acdc_dcdc_workset.acdcWorkmode = ACDC_WORK_MODE_CUR ;	//设置ACDC工作模式
		acdc_dcdc_workset.acdcCurrent = DCDC_OUT_CURRENT_STOP;//设置电流

	}
#else
	uint8_t i;

	if(ObjPcuOnOffCtrl.pcuOnoffConfig.modelCtrlLevel == 2)
	{
		for(i = 0;i < ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlNum;i++)
		{
			
			acdc_dcdc_modelset[i].acdcWorkmode = ACDC_WORK_MODE_VOLT ;	//设置ACDC工作模式
			acdc_dcdc_modelset[i].acdcOutVolt =ACDC_OUT_VOLT*10 ;	//设置ACDC直流输出电压
			//acdc_dcdc_modelset[i].acdcEnergy = ObjPcuOnOffCtrl.pcuWorkState.systemDisChargePower*-1;//设置并网放电功率			


		}

		for(i = 0;i < ObjPcuOnOffCtrl.pcuOnoffConfig.dcdcCtrlNum ;i++)
		{
			acdc_dcdc_modelset[i].dcdcOutVolt = DCDC_OUT_VLOT*10;
			acdc_dcdc_modelset[i].dcdcWorkMode = DCDC_WORK_MODE_CURRENT;//恒功率模式
			acdc_dcdc_modelset[i].dcdcCurrent = DCDC_OUT_CURRENT_STOP;
			acdc_dcdc_modelset[i].dcdcMaxPow = DCDC_MAX_POW*10;
		}
	}
	else if(ObjPcuOnOffCtrl.pcuOnoffConfig.modelCtrlLevel == 1)
	{//@添加1级设置
		for(i = 0;i < ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlNum ;i++)
		{
				acdc_dcdc_modelset[i].acdcWorkmode = ACDC_WORK_MODE_DCPOWER ;	//设置ACDC工作模式
				acdc_dcdc_modelset[i].acdcCurrent = DCDC_OUT_CURRENT_STOP;//设置电流
				acdc_dcdc_modelset[i].acdcEnergy = ACDC_MAX_POW_STOP;//设置功率
				acdc_dcdc_modelset[i].acdcOutVolt = ACDC_OUT_VOLT*10;//设置工作电压
		}

	}
#endif


}

/*===========================================================================*
 *  函数名称：Class_ChargeControler::AcdcDcdcDebugSet(void)
 *  功能描述：模块调试模式充电参数设置
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
void Class_ChargeControler::AcdcDcdcDebugSet()
{
	uint8_t i;
	for(i = 0;i < ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlNum;i++)
	{
		
		acdc_dcdc_debugModelset[i].acdcWorkmode = ObjPcuOnOffCtrl.pcuWorkState.systemAcdcWorkMode ;	//设置ACDC工作模式
		acdc_dcdc_debugModelset[i].acdcCurrent = ObjChargeCtrl.acdc_dcdc_debugset.dcdcCurrent;//设置电流
		acdc_dcdc_debugModelset[i].acdcEnergy = ObjPcuOnOffCtrl.pcuWorkState.systemDisChargePower;//设置功率
		acdc_dcdc_debugModelset[i].acdcOutVolt = ObjPcuOnOffCtrl.pcuWorkState.systemAcdcOutVolt;
		acdc_dcdc_debugModelset[i].acdcMaxChargePow = ACDC_LIMIT_POW*10;//设置最大功率
		acdc_dcdc_debugModelset[i].acdcMaxDischargePow = ACDC_LIMIT_POW*10;//设置最大功率
		acdc_dcdc_debugModelset[i].acdcMaxChargeCur = ACDC_LIMIT_CUR*10;//设置最大电流
		acdc_dcdc_debugModelset[i].acdcMaxDischargeCur = ACDC_LIMIT_CUR*10;//设置最大电流

	}

	for(i = 0;i < ObjPcuOnOffCtrl.pcuOnoffConfig.dcdcCtrlNum ;i++)
	{
		acdc_dcdc_modelset[i].dcdcOutVolt = DCDC_OUT_VLOT*10;
		acdc_dcdc_modelset[i].dcdcWorkMode = ObjPcuOnOffCtrl.pcuWorkState.systemDcdcWorkMode;//恒功率模式
		acdc_dcdc_modelset[i].dcdcCurrent = DCDC_OUT_CURRENT_STOP;//@协议设置DC电流
		acdc_dcdc_modelset[i].dcdcMaxPow = DCDC_MAX_POW*10;
	}
}



/*===========================================================================*
 *  函数名称：Class_ChargeControler::AcdcDcdcDischargeSet(void)
 *  功能描述：模块放电参数设置
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
void Class_ChargeControler::AcdcDcdcDischargeSet(uint8_t ctl,uint16_t power)
{
#ifdef PCU_CONFIG_ENABLE
	if(ObjPcuOnOffCtrl.pcuOnoffConfig.modelCtrlLevel == 2)
	{
		acdc_dcdc_workset.acdcWorkmode = ACDC_WORK_MODE_VOLT ; //设置ACDC工作模式
		acdc_dcdc_workset.acdcOutVolt =ACDC_OUT_VOLT*10 ; //设置ACDC直流输出电压
		acdc_dcdc_workset.acdcMaxPow =ACDC_MAX_POW*10 ;	//设置ACDC最大功率
	//	acdc_dcdc_workset.acdcEnergy = ;	//ACDC有功设定
	//	acdc_dcdc_workset.acdcReactPow = ;	//ACDC无功设定
	//	acdc_dcdc_workset.acdcReactPowFact = ;
		acdc_dcdc_workset.dcdcWorkMode = DCDC_WORK_MODE_CURRENT;//恒功率模式
		acdc_dcdc_workset.dcdcOutVolt = DCDC_OUT_VLOT*10;
		acdc_dcdc_workset.dcdcCurrent = DCDC_OUT_CURRENT_DISCHARGE*10;
		acdc_dcdc_workset.dcdcMaxPow = DCDC_MAX_POW*10;
	}
	else if(ObjPcuOnOffCtrl.pcuOnoffConfig.modelCtrlLevel == 1)
	{//@添加1级设置
		acdc_dcdc_workset.acdcWorkmode = ACDC_WORK_MODE_CUR ;	//设置ACDC工作模式
		acdc_dcdc_workset.acdcCurrent = DCDC_OUT_CURRENT_DISCHARGE*10;//设置电流

	}
	
#else
	uint8_t i;
	float currentTemp;

	if(ObjPcuOnOffCtrl.pcuOnoffConfig.modelCtrlLevel == 2)
	{
		for(i = 0;i < ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlNum;i++)
		{
			
			acdc_dcdc_modelset[i].acdcWorkmode = ACDC_WORK_MODE_VOLT ;	//设置ACDC工作模式
			acdc_dcdc_modelset[i].acdcOutVolt =ACDC_OUT_VOLT*10 ;	//设置ACDC直流输出电压
			//acdc_dcdc_modelset[i].acdcEnergy = ObjPcuOnOffCtrl.pcuWorkState.systemDisChargePower;//设置并网放电功率			


		}

		for(i = 0;i < ObjPcuOnOffCtrl.pcuOnoffConfig.dcdcCtrlNum ;i++)
		{
			acdc_dcdc_modelset[i].dcdcOutVolt = DCDC_OUT_VLOT*10;
			acdc_dcdc_modelset[i].dcdcWorkMode = DCDC_WORK_MODE_CURRENT;//恒功率模式
			acdc_dcdc_modelset[i].dcdcMaxPow = DCDC_MAX_POW*10;
			//currentTemp = objBMS.App_RespMaxChgCurr();//@根据不同的电池设置电流
			//currentTemp = MIN(currentTemp, BMS_MAX_CURRENT);//@根据模块个数限制最大电流
			currentTemp = bmsDat.BattGroupVolt;//电池组电压
			acdc_dcdc_modelset[i].dcdcCurrent = ObjPcuOnOffCtrl.pcuWorkState.systemDisChargePower*1000
				/bmsDat.BattGroupVolt;//根据功率设置计算电流设置
			acdc_dcdc_modelset[i].dcdcCurrent = MIN(acdc_dcdc_modelset[i].dcdcCurrent, BMS_MAX_CURRENT);
			acdc_dcdc_modelset[i].dcdcCurrent = acdc_dcdc_modelset[i].dcdcCurrent*ObjPcuOnOffCtrl.pcuCTLState.systemPowerLimit;//系统功率限制
		}
	}
	else if(ObjPcuOnOffCtrl.pcuOnoffConfig.modelCtrlLevel == 1)
	{//并网放电，AC恒功率模式
		if(ctl == SCU_CTRL_ONLINE)
		{
			for(i = 0;i < ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlNum;i++)
			{

				acdc_dcdc_modelset[i].acdcWorkmode = ACDC_WORK_MODE_ACPOWER ; //设置ACDC工作模式
				acdc_dcdc_modelset[i].acdcEnergy = power;//设置并网放电功率
				

			}

		} 
		else if(ctl == SCU_CTRL_OFFLINE)//离网放电，独立逆变模式
		{
			for(i = 0;i < ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlNum;i++)
			{

				acdc_dcdc_modelset[i].acdcWorkmode = ACDC_WORK_MODE_INDEPENDENT ; //设置ACDC工作模式
				

			}

		}



	}

#endif


}



/*===========================================================================*
 *  函数名称：Class_ChargeControler::AcFunctionSet(void)
 *  功能描述：模块充放电调试模式
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
void Class_ChargeControler::AcFunctionSet(uint8_t acdcID,int16_t para)//for debug model
{
	uint8_t result = 0;
	result = objStoredEnergy.APP_GetACDCFunctionMode(acdcID,para);
	if(!result)
	{
		objStoredEnergy.APP_SetACDCFunctionMode(acdcID,para);

	}

}



/*===========================================================================*
 *  函数名称：Class_ChargeControler::AcdcDcdcSet(void)
 *  功能描述：设置模块参数
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/

void Class_ChargeControler::AcdcDcdcSet(uint8_t acAddr,uint8_t dcAddr,_Acdc_Dcdc_WorkSet workset)//acdc dcdc 模块参数设置
{

	if(ObjPcuOnOffCtrl.pcuOnoffConfig.modelCtrlLevel == 2)
	{

		objStoredEnergy.APP_SetACDCWorkMode(acAddr,workset.acdcWorkmode); //设置ACDC工作模式
		objStoredEnergy.APP_SetACDCOutVolt(acAddr,workset.acdcOutVolt); //设置ACDC直流输出电压
		//objStoredEnergy.APP_SetACDCMaxPow(acAddr,acdc_dcdc_workset.acdcMaxPow); //设置ACDC最大功率
		objStoredEnergy.APP_SetDCDCWorkMode(dcAddr,workset.dcdcWorkMode);	//设置DCDC工作模式
		objStoredEnergy.APP_SetDCDCOutVolt(dcAddr,workset.dcdcOutVolt);	//设置DCDC直流输出电压
		objStoredEnergy.APP_SetDCDCMaxPow(dcAddr,workset.dcdcMaxPow);	//设置DCDC最大功率
		objStoredEnergy.APP_SetDCDCCurrent(dcAddr,workset.dcdcCurrent);	//设置DCDC电流
	}
	else if(ObjPcuOnOffCtrl.pcuOnoffConfig.modelCtrlLevel == 1)
	{
		objStoredEnergy.APP_SetACDCWorkMode(acAddr,workset.acdcWorkmode); //设置ACDC工作模式
		objStoredEnergy.APP_SetACDCConstantCurr(acAddr,workset.acdcCurrent);//设置ACDC电流
		objStoredEnergy.APP_SetACDCEnergy(acAddr,workset.acdcEnergy);//设置ACDC功率
		
	}
	objStoredEnergy.ACDCSetChangeFlag = 1;
}



/*===========================================================================*
 *  函数名称：Class_ChargeControler::AcdcDcdcRead(void)
 *  功能描述：读取模块设置参数
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
uint8_t Class_ChargeControler::AcdcDcdcRead(uint8_t acAddr,uint8_t dcAddr,_Acdc_Dcdc_WorkSet workset)//acdc dcdc 模块参数设置
{
	uint8_t result1 = 0;
	uint8_t result2 = 0;
	uint8_t result3 = 0;
	uint8_t result4 = 0;
	uint8_t result5 = 0;
	uint8_t result6 = 0;
	uint8_t result7 = 0;

	if(ObjPcuOnOffCtrl.pcuOnoffConfig.modelCtrlLevel == 2)
	{	
		result1 = objStoredEnergy.APP_GetACDCWorkMode(acAddr,workset.acdcWorkmode);	//读ACDC工作模式
		result2 = objStoredEnergy.APP_GetACDCOutVolt(acAddr,workset.acdcOutVolt);	//读ACDC直流输出电压
		//result3 = objStoredEnergy.APP_GetACDCMaxPow(acAddr,acdc_dcdc_workset.acdcMaxPow);	//读ACDC最大功率
		result4 = objStoredEnergy.APP_GetDCDCWorkMode(dcAddr,workset.dcdcWorkMode);	//读DCDC工作模式
		result5 = objStoredEnergy.APP_GetDCDCOutVolt(dcAddr,workset.dcdcOutVolt);	//读DCDC直流输出电压
		result6 = objStoredEnergy.APP_GetDCDCMaxPow(dcAddr,workset.dcdcMaxPow);	//读DCDC最大功率
		result7 = objStoredEnergy.APP_GetDCDCCurrent(dcAddr,workset.dcdcCurrent);	//读DCDC电流

		if((result1 == 1) && (result2 == 1) && (result4 == 1)
			&& (result5 == 1) && (result6 == 1) && (result7 == 1))
		{
			objStoredEnergy.ACDCSetChangeFlag = 0;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(ObjPcuOnOffCtrl.pcuOnoffConfig.modelCtrlLevel == 1)
	{
		result1 = objStoredEnergy.APP_GetACDCWorkMode(acAddr,workset.acdcWorkmode);	//读ACDC工作模式
		result2 = objStoredEnergy.APP_GetACDCConstantCurr(acAddr,workset.acdcCurrent);//读ACDC电流
		result3 = objStoredEnergy.APP_GetACDCEnergy(acAddr,workset.acdcEnergy);//读ACDC功率

		if((result1 == 1) && (result2 == 1) && (result3 == 1))
		{
			objStoredEnergy.ACDCSetChangeFlag = 0;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	return -1;		//@todo 应该返回什么
}


/*===========================================================================*
 *  函数名称：Class_ChargeControler::Dat_Init(void)
 *  功能描述：init
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
void	Class_ChargeControler::Dat_Init(void)
{
	uint8_t i;
	AcdcDcdcChargeInit();
	AcdcDcdcDebugInit();//for debug model
	
	for(i = 0;i < ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlNum;i++)//功能模式初始化默认为低压穿越模式
	{
		AcFunctionSet(ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlAddr[i]
			,ObjPcuOnOffCtrl.pcuOnoffConfig.acdcFunctionSet);
	}
}


/*===========================================================================*
 *  函数名称：Class_ChargeControler::DcdcParaSet(void)
 *  功能描述：充放电参数设置
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
void Class_ChargeControler::DcdcParaSet(_Acdc_Dcdc_WorkSet* modelset)//充放电参数设置
{
uint8_t result1 = 0;
uint8_t result2 = 0;
uint8_t result3 = 0;
uint8_t result4 = 0;

uint8_t i = 0;



for(i = 0;i < ObjPcuOnOffCtrl.pcuOnoffConfig.dcdcCtrlNum;i++)
{

	result1 = objStoredEnergy.APP_GetDCPara(ObjPcuOnOffCtrl.pcuOnoffConfig.dcdcCtrlAddr[i]
		,DCDC_WORKMODE, modelset[i].dcdcWorkMode);//读最大放电功率
	result2 = objStoredEnergy.APP_GetDCPara(ObjPcuOnOffCtrl.pcuOnoffConfig.dcdcCtrlAddr[i]
		,DCDC_VOLTOUT, modelset[i].dcdcOutVolt);//读最大放电功率
	result3 = objStoredEnergy.APP_GetDCPara(ObjPcuOnOffCtrl.pcuOnoffConfig.dcdcCtrlAddr[i]
		,DCDC_MAXPOW_LIMIT, modelset[i].dcdcMaxPow);//读最大放电功率
	result4 = objStoredEnergy.APP_GetDCPara(ObjPcuOnOffCtrl.pcuOnoffConfig.dcdcCtrlAddr[i]
		,DCDC_CURRENT, modelset[i].dcdcCurrent);//读最大放电功率
		

	if(!result1)
	{
		objStoredEnergy.APP_SetDCPara(ObjPcuOnOffCtrl.pcuOnoffConfig.dcdcCtrlAddr[i]
				,DCDC_WORKMODE,modelset[i].dcdcWorkMode); //设置最大放电功率

	}
	if(!result2)
	{
		objStoredEnergy.APP_SetDCPara(ObjPcuOnOffCtrl.pcuOnoffConfig.dcdcCtrlAddr[i]
				,DCDC_VOLTOUT,modelset[i].dcdcOutVolt); //设置最大放电功率


	}
	if(!result3)
	{
		objStoredEnergy.APP_SetDCPara(ObjPcuOnOffCtrl.pcuOnoffConfig.dcdcCtrlAddr[i]
				,DCDC_MAXPOW_LIMIT,modelset[i].dcdcMaxPow); //设置最大放电功率


	}
	if(!result4)
	{
		objStoredEnergy.APP_SetDCPara(ObjPcuOnOffCtrl.pcuOnoffConfig.dcdcCtrlAddr[i]
				,DCDC_CURRENT,modelset[i].dcdcCurrent); //设置最大放电功率

	}
	
	if(!(result1 & result2 & result3 & result4))
	{
		objStoredEnergy.ACDCSetChangeFlag = 1;
	}
	else
	{
		objStoredEnergy.ACDCSetChangeFlag = 0;
	}

}

}



/*===========================================================================*
 *  函数名称：Class_ChargeControler::ParaSet(void)
 *  功能描述：充放电参数设置
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
void Class_ChargeControler::ParaSet(_Acdc_Dcdc_WorkSet* modelset)//充放电参数设置
{
uint8_t result1 = 0;
uint8_t result2 = 0;
uint8_t result3 = 0;
uint8_t result4 = 0;
uint8_t result5 = 0;
uint8_t result6 = 0;
uint8_t result7 = 0;
uint8_t result8 = 0;
uint8_t result9 = 0;


uint8_t i = 0;



for(i = 0;i < ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlNum;i++)
{

	result1 = objStoredEnergy.APP_GetACDCWorkMode(ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlAddr[i]
		,modelset[i].acdcWorkmode); //读ACDC工作模式
	result2 = objStoredEnergy.APP_GetACDCConstantCurr(ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlAddr[i]
		,modelset[i].acdcCurrent);//读ACDC电流
	result3 = objStoredEnergy.APP_GetACDCEnergy(ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlAddr[i]
		,modelset[i].acdcEnergy);//读ACDC功率
	result4 = objStoredEnergy.APP_GetACDCUnsignedPara(ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlAddr[i]
		,DISCHARGE_MAXPOWER, modelset[i].acdcMaxChargePow);//读最大放电功率
	result5 = objStoredEnergy.APP_GetACDCUnsignedPara(ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlAddr[i]
		,CHARGE_MAXPOWER,modelset[i].acdcMaxDischargePow);//读最大充电功率	
	result6 = objStoredEnergy.APP_GetACDCUnsignedPara(ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlAddr[i]
		,DISCHARGE_MAXCURR, modelset[i].acdcMaxChargeCur);//读最大放电功率
	result7 = objStoredEnergy.APP_GetACDCUnsignedPara(ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlAddr[i]
		,CHARGE_MAXCURR,modelset[i].acdcMaxDischargeCur);//读最大充电功率	
	result8 = objStoredEnergy.APP_GetACDCOutVolt(ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlAddr[i]
		,modelset[i].acdcOutVolt);//读ACDC电压
	result9 = objStoredEnergy.APP_GetACDCUnsignedPara(ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlAddr[i]
			,CONSTANTVOLT_CURRLIMIT, modelset[i].acdcMaxChargeCur);//恒压模式电流限制
	

	if(!result1)
	{
		objStoredEnergy.APP_SetACDCWorkMode(ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlAddr[i]
			,modelset[i].acdcWorkmode); //设置ACDC工作模式
	}
	if(!result2)
	{
		objStoredEnergy.APP_SetACDCConstantCurr(ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlAddr[i]
			,modelset[i].acdcCurrent);//设置ACDC电流

	}
	if(!result3)
	{
		objStoredEnergy.APP_SetACDCEnergy(ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlAddr[i]
			,modelset[i].acdcEnergy);//设置ACDC功率

	}
	if(!result4)
	{
		objStoredEnergy.APP_SetACDCUnsignedPara(ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlAddr[i]
				,DISCHARGE_MAXPOWER,modelset[i].acdcMaxChargePow); //设置最大放电功率

	}
	if(!result5)
	{
		objStoredEnergy.APP_SetACDCUnsignedPara(ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlAddr[i]
				,CHARGE_MAXPOWER,modelset[i].acdcMaxDischargePow);//设置最大充电功率	

	}
	if(!result6)
	{
		objStoredEnergy.APP_SetACDCUnsignedPara(ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlAddr[i]
				,DISCHARGE_MAXCURR,modelset[i].acdcMaxChargeCur); //设置最大放电功率

	}
	if(!result7)
	{
		objStoredEnergy.APP_SetACDCUnsignedPara(ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlAddr[i]
				,CHARGE_MAXCURR,modelset[i].acdcMaxDischargeCur);//设置最大充电功率	

	}
	if(!result8)
	{
		objStoredEnergy.APP_SetACDCOutVolt(ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlAddr[i]
				,modelset[i].acdcOutVolt);//设置ACDC电压	

	}
	if(!result9)
	{
		objStoredEnergy.APP_SetACDCUnsignedPara(ObjPcuOnOffCtrl.pcuOnoffConfig.acdcCtrlAddr[i]
				,CONSTANTVOLT_CURRLIMIT,modelset[i].acdcMaxChargeCur);//恒压模式电流限制	

	}
	if(!(result1 & result2 & result3 & result4 & result5 & result6 & result7 & result8&result9))
	{
		objStoredEnergy.ACDCSetChangeFlag = 1;
	}
	else
	{
		objStoredEnergy.ACDCSetChangeFlag = 0;
	}

}

}


/*===========================================================================*
 *  函数名称：Class_ChargeControler::ChargeDebug(void)
 *  功能描述：模块充放电调试模式
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
void Class_ChargeControler::ChargeDebug()//for debug model
{

	AcdcDcdcDebugSet();
	ParaSet(acdc_dcdc_debugModelset);
	DcdcParaSet(acdc_dcdc_debugModelset);
}


/*===========================================================================*
 *  函数名称：Class_ChargeControler::ChargeStopControl(void)
 *  功能描述：停止充电
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
void Class_ChargeControler::ChargeStopControl()
{
#ifdef PCU_CONFIG_ENABLE

	AcdcDcdcChargeStop();	
	if(AcdcDcdcRead(MAC_ID_DCAC0,MAC_ID_BATT_DCDC0,acdc_dcdc_workset) != 1)//参数设置不对
	{
		AcdcDcdcSet(MAC_ID_DCAC0,MAC_ID_BATT_DCDC0,acdc_dcdc_workset);//设置模块参数 停止放电
	}
#else
	

	AcdcDcdcChargeStop();
	ParaSet(acdc_dcdc_modelset);
	DcdcParaSet(acdc_dcdc_modelset);

	
	
#endif


}
/*===========================================================================*
 *  函数名称：Class_ChargeControler::ChargeControl(void)
 *  功能描述：充电设置
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
void Class_ChargeControler::ChargeControl()
{
#ifdef PCU_CONFIG_ENABLE

	AcdcDcdcChargeSet();	
	if(AcdcDcdcRead(MAC_ID_DCAC0,MAC_ID_BATT_DCDC0,acdc_dcdc_workset) != 1)//参数设置不对
	{
		AcdcDcdcSet(MAC_ID_DCAC0,MAC_ID_BATT_DCDC0,acdc_dcdc_workset);//设置模块参数 
	}
#else

	
	AcdcDcdcChargeSet();
	ParaSet(acdc_dcdc_modelset);
	DcdcParaSet(acdc_dcdc_modelset);



#endif

}

/*===========================================================================*
 *  函数名称：Class_ChargeControler::DisChargeControl(void)
 *  功能描述：放电设置
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
void Class_ChargeControler::DisChargeControl(uint8_t ctl,uint16_t power)
{
#ifdef PCU_CONFIG_ENABLE

	AcdcDcdcDischargeSet();	
	if(AcdcDcdcRead(MAC_ID_DCAC0,MAC_ID_BATT_DCDC0,acdc_dcdc_workset) != 1)//参数设置不对
	{
		AcdcDcdcSet(MAC_ID_DCAC0,MAC_ID_BATT_DCDC0,acdc_dcdc_workset);//设置模块参数 停止放电
	}
#else
	
	AcdcDcdcDischargeSet(ctl,power);
	ParaSet(acdc_dcdc_modelset);
	DcdcParaSet(acdc_dcdc_modelset);

	
	
#endif


}




/*===========================================================================*
 *  函数名称：Class_ChargeControler::ChargeWork(void)
 *  功能描述：并网充电
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
void Class_ChargeControler::ChargeWork()//并网充电控制
{


	//bmsDatRead();
	if ((bmsDat.BattGroupCurr < BMS_CUR_OFFSET) && (bmsDat.BattGroupCurr > -BMS_CUR_OFFSET))//如果电流过小,说明电池控制没有吸合
	{
		objBMS.App_SetBattOnOff(POWER_ON);//@todo 改成发一次?
	}

	

	if((ObjPcuOnOffCtrl.GetErrState() == 0) && (bmsDat.Group_SOC <= BMS_SOC_HIGH))
	{
		ChargeControl();
	}
		//@todo else 如果充电中出现错误怎么处理? 
		
	if(bmsDat.Group_SOC > BMS_SOC_HIGH)
	{
		ChargeStopControl();
		
	}


}


/*===========================================================================*
 *  函数名称：Class_ChargeControler::DischargeWork(void)
 *  功能描述：放电工作模式
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
void Class_ChargeControler::DischargeWork(uint8_t ctl)
{
	//bmsDatRead();
	if ((bmsDat.BattGroupCurr < BMS_CUR_OFFSET) && (bmsDat.BattGroupCurr > -BMS_CUR_OFFSET))//如果电流过小,说明电池控制没有吸合
	{
		objBMS.App_SetBattOnOff(POWER_ON);//@todo 改成发一次?
	}

		
	if((ObjPcuOnOffCtrl.GetErrState() == 0) && (bmsDat.Group_SOC >= BMS_SOC_LOW))
	{

		DisChargeControl(ctl,ObjPcuOnOffCtrl.pcuWorkState.systemDisChargePower);
	}
	
	if(bmsDat.Group_SOC < BMS_SOC_LOW)
	{
		ChargeStopControl();
		
	}



}


/*===========================================================================*
 *  函数名称：Class_ChargeControler::SystemOnlineWork(void)
 *  功能描述：并网任务
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
 void Class_ChargeControler:: SystemOnlineWork()
{
	switch(ObjPcuOnOffCtrl.pcuCTLState.systemChargeStateCtrl)
	{
	
		case SCU_CTRL_CHARGE:
			ChargeWork();
		break;
		
		case SCU_CTRL_DISCHARGE:
			DischargeWork(SCU_CTRL_ONLINE);
		break;
		
		case SCU_CTRL_STOPCHARGE:
			ChargeStopControl();
		break;
	
		case SCU_CTRL_DEBUG:
			ChargeDebug();//for debug model
		break;
	
		default:
	
		break;
			
	
	}

}

/*===========================================================================*
 *  函数名称：Class_ChargeControler::SystemOfflineWork(void)
 *  功能描述：离网任务
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
 void Class_ChargeControler:: SystemOfflineWork()
{
	 switch(ObjPcuOnOffCtrl.pcuCTLState.systemChargeStateCtrl)
	 {
	 
		 case SCU_CTRL_DISCHARGE:
			 DischargeWork(SCU_CTRL_OFFLINE);
		 break;
		 
		 case SCU_CTRL_STOPCHARGE:
			 ChargeStopControl();
		 break;
	 
		 case SCU_CTRL_DEBUG:
			 ChargeDebug();//for debug model
		 break;
	 
		 default:
	 
		 break;
			 
	 
	 }


}



/*===========================================================================*
 *  函数名称：Class_ChargeControler::SystemSchedule(void)
 *  功能描述：系统任务
 *  输入参数： 无
 *  返回值：       无
 *  使用说明：
 *==========================================================================*/
void Class_ChargeControler::SystemSchedule()//系统任务
{
	static uint8_t tempCount1; 
		

	if(g_st_TimerFlag.b100ms)
	{
		tempCount1++;
	}
	if(tempCount1 > 20)
	{       
        bmsDatRead();//电池状态检测
		//并网模式
		if(ObjPcuOnOffCtrl.pcuCTLState.systemOnOfflineCtrl == SCU_CTRL_ONLINE)
		{

			SystemOnlineWork();

		}
		else if(ObjPcuOnOffCtrl.pcuCTLState.systemOnOfflineCtrl == SCU_CTRL_OFFLINE)
		{
			SystemOfflineWork();

		}


		tempCount1 = 0;

	}



}




//===========================================================================
// End of file.
//===========================================================================






