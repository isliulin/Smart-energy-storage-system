/*==========================================================*/
/* Title		:	Obj_PCUControler.h														*/
/* Description	: 	PCU controler											*/
// Author		:	wangyufei
/* Date			:	2017.10.30														*/
/*==========================================================*/


#include "Publics.h"


/*===========================================================================*
 *  函数名称：Class_PcuModel::Dat_Init(void)
 *  功能描述：init
 *  输入参数： 无
 *  返回值：       无
 *  使用说明： 
 *==========================================================================*/
void Class_PcuModel::Dat_Init()
{

	//------数据层初始化--------
		objSystemRTC.Dat_Init();	//软件计时初始化
		objEEPROM.Dat_Init();
	//	objPcsConfigManage.PCU_Version_DatInit();
		objPcsConfigManage.ConfigManageDat_Init();//初始化配置信息
		objPcsConfigManage.DefaultConfigDat_Init();//默认初始化（预防eeprom出问题）
		objMemory.Dat_Init();
		objBMS.Dat_Init();
		
	//	objCanDataPool.Dat_Init();
		objPcuModel.PCUAddr_Init();
		objUtility.App_DataInit();
		
	//	objRS485App.Dat_Init();		//内容为空
		objCanDataExchg.Dat_Init();
		objCanBMSProtocol.Dat_Init();
		objCanInnerProtocol.Dat_Init();
		objCanMasterMonPrtcl.Dat_Init();
		objUpgrade.Dat_Init();//系统升级
	
		//objSensor.DataInit();//温湿度传感器
		objModBusProtocol.Dat_Init();
	
		objCanMstMonLongFrame.Dat_Init();
	//	ObjEMMonitor.Dat_Init();	//内容为空
	
		ObjPcuOnOffCtrl.Dat_Init();//开关机模块
		ObjChargeCtrl.Dat_Init();//充电模块
		objStoredEnergy.Dat_Init();
		objFailManager.Dat_Init();


}



/***************************************************************************
函数名:Class_PcuModel::PCUAddr_Init
功能描述:初始化PCU板子的地址
函数调用：由主函数调用
作者:
日期:
****************************************************************************/
void Class_PcuModel::PCUAddr_Init(void)
{
	uint16_t	u16temp;
	u16temp = objEEPROM.App_ReadSingleByte(PCU_ADDR);
	if( u16temp >= MAC_ID_StoreEnergyBOX1 && u16temp <= MAC_ID_StoreEnergyBOX30)
	{
		m_i16_SelfAddr = u16temp;
	}
}


/*===========================================================================*
 *	函数名称：Class_PcuModel::SystemSchedule(void)
 *	功能描述：//系统任务
 *	输入参数： 无
 *	返回值：	   无
 *	使用说明：
 *==========================================================================*/

void Class_PcuModel::SystemSchedule()//系统任务
{
	objEEPROM.App_SaveManage();				//存储管理：将数据真正写入eeprom	

//	objCanDataPool.Dat_DataMainten();		//内容为空

	objRS485App.App_MainRout();
	
	objCanApp.MainRoutle();
	
	objSystemRTC.App_SysConfigProc();	//启用软件计时
	
	ObjEMMonitor.Dat_DataMainten();//上传至集控数据
	objUtility.App_UtilityProcess();

	//上电5S后
	if (g_st_TimerFlag.bPowerOn5sEnd == 1)
	{
		
		//BSP_Relay_Open(RUNLED_RELAY);  //testing
		
		//市电计算
		objUtility.App_UtilityProcess();
		
		objEnergyManage.App_Run();			

		//故障管理
		objFailManager.App_GetFailStateMsg();
		//温湿度传感器查询
		//objSensor.MsgProc();	 
		
		//PDU 升级管理
		objUpgrade.App_UpgradeManage();
	
		ObjPcuOnOffCtrl.SystemSchedule();	//开关机模块
		ObjChargeCtrl.SystemSchedule();		//充电模块
	
		
	}


	

}


