
/*==========================================================*/
/* Title		:	Obj_ChargeControler.h														*/
/* Description	: 	charge controler											*/
// Author		:	wangyufei
/* Date			:	2017.10.30														*/
/*==========================================================*/

#ifndef _CHARGECONTROLER_H
#define _CHARGECONTROLER_H

#include "Publics.h"

#define  MODEL_FORBID			0//null
#define  LVRT					1//低压穿越
#define	 ISLAND_EFFECT_PROTECT 	2//孤岛保护	

#define BMS_CUR_OFFSET	1
#define BMS_SOC_LOW		10
#define BMS_SOC_HIGH	95

#define ACDC_WORK_MODE_VOLT			0x01
#define ACDC_WORK_MODE_ACPOWER		0x02
#define ACDC_WORK_MODE_DCPOWER		0x05
#define ACDC_WORK_MODE_CUR			0x06
#define ACDC_WORK_MODE_INDEPENDENT	0x04

#define DCDC_WORK_MODE_VOLT			0x01
#define DCDC_WORK_MODE_CURRENT	    0x02


#define ACDC_LIMIT_CUR		objPcsConfigManage.m_st_GetConfigPara.ACModuleNum*60*1000/600
#define ACDC_LIMIT_POW		objPcsConfigManage.m_st_GetConfigPara.ACModuleNum*60
#define DCDC_MAX_POW		objPcsConfigManage.m_st_GetConfigPara.DCModuleNum*60

//#define ACDC_LIMIT_CUR		400
//#define ACDC_LIMIT_POW		240
#define ACDC_OUT_VOLT		765
#define ACDC_MAX_POW_STOP	0

#define ACDC_POW_STOP		0
#define ACDC_POW_DISCHARGE	10

#define DCDC_OUT_VLOT		560
#define ACDC_OUT_VLOT		650
//#define DCDC_MAX_POW		400
#define DCDC_OUT_CURRENT_STOP	0
#define DCDC_OUT_CURRENT_CHARGE	-30
#define DCDC_OUT_CURRENT_DISCHARGE	50

#define BMS_MAX_CURRENT			ACDC_LIMIT_CUR*ObjPcuOnOffCtrl.pcuCTLState.systemPowerLimit/100//限制最大功率限制最大电流

typedef enum   
{
	pcu_set_on_off = 1,
	pcu_set_change,
	pcu_set_on_offline,
	pcu_set_acdc_workmode,
	pcu_set_dcdc_workmode,
	pcu_set_acdc_outvolt,
	pcu_set_acdc_pow,
	pcu_set_dcdc_outvolt,
	pcu_set_dcdc_maxpow,
	pcu_set_dcdc_current,
	pcu_set_system_pow,
	pcu_set_restart_start = 20

}_SCU_TO_PCU_CMD;



typedef struct
{
	uint16_t acdcWorkmode;
	uint16_t acdcOutVolt;
	uint16_t acdcMaxPow;
	int16_t acdcEnergy;
	int16_t acdcReactPow;
	int16_t acdcReactPowFact;
	int16_t acdcCurrent;
	uint16_t acdcMaxChargePow;
	uint16_t acdcMaxDischargePow;
	uint16_t acdcMaxChargeCur;
	uint16_t acdcMaxDischargeCur;
	
	uint16_t dcdcWorkMode;
	uint16_t dcdcOutVolt;
	uint16_t dcdcMaxPow;
	int16_t dcdcCurrent;

}_Acdc_Dcdc_WorkSet;



class Class_ChargeControler
{	

	private:

		
		


	public:
		_Batt_DataInfoDef bmsDat;//电池组信息
		_Batt_DataInfoDef bmsDatInfo[PCU_MODEL_MAXNUM];//多路配置

		
		_Acdc_Dcdc_WorkSet acdc_dcdc_workset;//模块设置信息
		_Acdc_Dcdc_WorkSet acdc_dcdc_modelset[PCU_MODEL_MAXNUM];//多路配置

		
		_Acdc_Dcdc_WorkSet acdc_dcdc_debugset;	
		_Acdc_Dcdc_WorkSet acdc_dcdc_debugModelset[PCU_MODEL_MAXNUM];//多路配置

	private:

		
		void AcdcDcdcDebugInit(void);//for debug
		void AcdcDcdcChargeInit(void);
		void AcdcDcdcChargeSet(void);//充电参数设置
		void AcdcDcdcChargeStop(void);//停止充电参数设置
		void AcdcDcdcDischargeSet(uint8_t ctl,uint16_t power);//放电参数设置
		void AcdcDcdcDischargeStop(void);//停止放电参数设置
		void AcdcDcdcDebugSet(void);//debug set
		void SystemOnlineWork(void);//并网工作
		void SystemOfflineWork(void);//离网工作
		void ParaSet(_Acdc_Dcdc_WorkSet* modelset);//充放电参数设置
		void DcdcParaSet(_Acdc_Dcdc_WorkSet* modelset);
		void DischargeWork(uint8_t ctl);//放电工作模式
		void ChargeWork(void);//充电工作模式




	public:
        void bmsDatRead(void);//BMS参数读取             
		void AcdcDcdcSet(uint8_t acAddr,uint8_t dcAddr,_Acdc_Dcdc_WorkSet workset); //模块参数设置
		uint8_t AcdcDcdcRead(uint8_t acAddr,uint8_t dcAddr,_Acdc_Dcdc_WorkSet workset); //模块参数读取

//		void SetPcuOnOffControler(Class_PCUOnOffControler* objPcuOnOffControler);//设置开关机模块指针
		void Dat_Init(void);//初始化
		void ChargeDebug(void);//debug model
		void ChargeStopControl(void);//停止充电设置
		void ChargeControl(void);//充电设置
		void DisChargeControl(uint8_t ctl,uint16_t power);//放电设置

		void AcFunctionSet(uint8_t acdcID,int16_t para);//设置模块功能模式
		void SystemSchedule(void);//系统任务

     
};




#endif
//============================================================
// End of file.
//============================================================




