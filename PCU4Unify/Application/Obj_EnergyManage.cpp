
/*==========================================================*/
/* Title		:	ObjChargSys.cpp													*/
/* Description	: 	ObjChargSys realization 												*/
/* Date		:	2015.7.29													*/
/*==========================================================*/
#include "Publics.h"

/************************************************************************************
函数名称：	Dat_PDUInit()
功能描述：	初始化
函数调用:
执行过程描述：PDU数据初始化
***********************************************************************************/
void	Class_EnergyManage::Dat_Init(void)
{
	m_u8_RunStrategy = ENERGY_EV;
}

/************************************************************************************
函数名称：	App_Run
功能描述：	
函数调用:
执行过程描述：
***********************************************************************************/
void    Class_EnergyManage::App_Run(void)
{

	int16_t i16temp = objPcuModel.m_i16_SelfAddr;
	


	/*if(i16temp >= MAC_ID_DCACBOX1 && i16temp <= MAC_ID_DCACBOX5)//???ACDC?
	{
		objACDCMonitor.App_Manager();
		//objACmeter.App_Manage();
	}*/

	if (i16temp >= MAC_ID_StoreEnergyBOX1&& i16temp <= MAC_ID_StoreEnergyBOX30) //储能一体化机柜
	{
		objStoredEnergy.App_Manager(); //储能数据管理
		objBMS.App_BmsOffLineCheck();	//BMS离线检测
		
	}
    else if(i16temp >= MAC_ID_PDU0 && i16temp <= MAC_ID_PDU31)
	{
		//objChargeSys.App_ChgSysProc();
	}
	/*else if(i16temp >= MAC_ID_BATTERYBOX0 && i16temp <= MAC_ID_BATTERYBOX3)//???
	{
		//????????
		objEnergyBattery.App_Manage();
		//objDCMeter.App_Manage();
	}*/

	else
	{
		//
	}

}


/************************************************************************************
????????????????????	App_RunStrategy()
??????????è????????	????????????
??????????÷????:
??????????????????è??????????????????·??è??????????????ò????·????????????é
***********************************************************************************/
void	Class_EnergyManage::App_RunStrategy(void)
{

}




