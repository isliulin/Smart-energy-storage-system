/*==========================================================*/
/* Title		:	Obj_PCUOnOffControler.h														*/
/* Description	: 	PCU OnOffcontroler											*/
// Author		:	wangyufei
/* Date			:	2017.10.30														*/
/*==========================================================*/


#ifndef _PCUOnOffControler_H
#define _PCUOnOffControler_H

#include "Publics.h"

//#define	PCUCTRL_ON_OFF_SWITCH			//开关按钮控制,屏蔽掉为集控控制
//#define PCU_CONFIG_ENABLE				//单路模块默认配置，屏蔽掉宏后为多路可配置模式

#define			   SCU_CTRL_RESTART	  0x66
#define            SCU_CTRL_TURNON    0x55
#define            SCU_CTRL_TURNOFF   0xAA
#define            SCU_CTRL_ONLINE    0x99
#define            SCU_CTRL_OFFLINE   0x88
#define            SCU_CTRL_CHARGE    0x11
#define            SCU_CTRL_DISCHARGE   0x22
#define            SCU_CTRL_STOPCHARGE    0x33
#define            SCU_CTRL_DEBUG     0x44

#define			   PCU_MODEL_MAXNUM	  4

#define MIN(x,y)   (((x)<=(y))?(x):(y))


enum
{
    PCU_ONLINE_IDLE = 0,
    PCU_ONLINE_TURNON_AC,
    PCU_ONLINE_TURNON_DC,
    PCU_ONLINE_TURNON_OK,
};

enum
{
    PCU_ONLINE_IDLE1 = 0,
    PCU_ONLINE_TURNOFF_DC,
    PCU_ONLINE_TURNOFF_AC,
    PCU_ONLINE_TURNOFF_OK,
};


enum
{
    PCU_OFFLINE_IDLE = 0,
    PCU_OFFLINE_TURNON_DC,
    PCU_OFFLINE_TURNON_AC,
    PCU_OFFLINE_TURNON_OK,
};

enum
{
    PCU_OFFLINE_IDLE1 = 0,
    PCU_OFFLINE_TURNOFF_AC,
    PCU_OFFLINE_TURNOFF_DC,
    PCU_OFFLINE_TURNOFF_OK,
};

enum
{
    PCU_RESTART_IDLE = 0,
    PCU_RESTART_TURNOFF,
	PCU_RESTART_OK,
};



//开关机控制命令
enum PcuOnOffCMD
{
	PcuBootNormal = 0,
	PcuShutdownNormal,
	PcuShutdownEPO,
	PcuShutdownAbnormal,
	

};

typedef struct		//系统状态
{
	uint8_t PcuSystemOnOffline;//系统并离网状态：0离网，1并网
	uint8_t	PcuSystemOnOffState;//系统目前开关机状态：0关机，1开机

}_PcuSystemState;



typedef struct		//开关机命令
{
	uint8_t acdcTurnOn;//ACDC开机
	uint8_t acdcTurnOnAlrdy;//ACDC开机命令已发
	uint8_t acdcTurnOnOK;	//ACDC开机完成
	uint8_t acdcTurnOff;	//ACDC关机
	uint8_t acdcTurnOffAlrdy;//ACDC关机命令已发
	uint8_t acdcTurnOffOk;//ACDC关机完成
	
	uint8_t dcdcTurnOn;
	uint8_t dcdcTurnOnAlrdy;
	uint8_t dcdcTurnOnOK;
	uint8_t dcdcTurnOff;
	uint8_t dcdcTurnOffAlrdy;
	uint8_t dcdcTurnOffOk;
	
	uint8_t acdcDcdcSetOk;
	uint8_t bmsTurnOn;
	uint8_t bmsTurnOff;
	uint8_t bmsTurnOnOk;
	uint8_t bmsTurnOffOk;
//	uint8_t bmsOk;

	uint8_t systemTurnOn;//系统开机
	uint8_t systemTurnOff;//系统关机
	uint8_t systemTurnOnOk;//系统开机完成
	uint8_t systemTurnOffOk;//系统关机完成
	uint8_t systemAcdcErr;//ACDC错误
	uint8_t systemDcdcErr;//DCDC错误
	uint8_t systemBmsErr;//BMS错误
    uint8_t systemEPOTurnOff;//EPO紧急关机
//	uint8_t systemEPO;

	//集控命令
	uint16_t systemTurnOnOffScuCtrl;//系统开关机设置
	uint16_t systemOnOfflineScuCtrl;//系统并离网功率设置
	uint16_t systemChargeStateScuCtrl;//系统充放电设置
	uint16_t systemDisChargePower;//系统充放电功率设置
	uint16_t systemRestart;//系统重启命令
	uint16_t systemAcdcWorkMode;//ACDC工作模式设置
	uint16_t systemDcdcWorkMode;//DCDC工作模式设置
	uint16_t systemAcdcOutVolt;//ACDC输出电压设置

}_PcuWorkState;


typedef struct		//pcu错误状态
{
	uint8_t systemCommunicationError;	//系统通讯错误
	uint8_t systemErr;//系统错误
	

} _PcuErrorState;

typedef struct		//pcu控制状态
{
	uint8_t systemTurnOnOffCtrl;
	uint8_t systemOnOfflineCtrl;
	uint8_t systemChargeStateCtrl;
	uint8_t systemTurnOnBan;//禁止开机
	uint8_t systemPowerLimit;//功率限制
	uint8_t TurnOnFanDelay;//关闭风机，30分钟后开风机
	uint8_t TurnOffFanDelay;//10分钟后关闭风机

} _PcuCTLState;


typedef struct		//配置管理
{
	uint8_t 	modelCtrlLevel;//模块层级
	
	uint8_t 	acdcCtrlAddr[AC_MAJOR_NUM_MAX];//acdc控制地址
	uint8_t 	acdcCtrlNum;//acdc数量
	
	uint8_t		dcdcCtrlAddr[DC_MAJOR_NUM_MAX];
	uint8_t 	dcdcCtrlNum;
	
	uint8_t		bmsCtrlAddr[BMS_NUM_MAX];
	uint8_t		bmsCtrlNum;
	uint8_t		bmsCtrlOnOff[PCU_MODEL_MAXNUM];//电池端继电器开关控制
	uint8_t		powerLimit;						//根据海拔对功率进行限制
	uint8_t  	acdcFunctionSet;				//ac模块功能设置，默认为0
	
} _PcuOnOffConfig;//配置管理


class Class_PCUOnOffControler
{	

	private:
//		_PcuTimeWait  pcuTimeWait;

//		_Acdc_Dcdc_WorkSet acdc_dcdc_workset;
//		_Batt_DataInfoDef bmsDat;
//		Class_ChargeControler* ChargeCtrl;
		

	public:
		_PcuWorkState  pcuWorkState;
		_PcuErrorState pcuErrorState;
		_PcuCTLState   pcuCTLState;
		_PcuOnOffConfig pcuOnoffConfig;

			
		
	private:
		
		
        //void LEDTurnOn(void);//绿灯闪烁
		void SystemStateCheck(void);//异常检测
		void SystemTurnOnAcdc(void);//ACDC开机
		void SystemTurnOnDcdc(void);//dcdc开机
		void SystemTurnOffAcdc(void);//ACDC关机机
		void SystemTurnOffDcdc(void);//DCDC关机
        void EPOTurnOff(void);//EPO 关机
		uint8_t get_communication_error(void);
		void PcuOnOffConfigInit(void);//配置管理初始化
		void SystemErrorResponse(void);//故障处理函数
		void TurnOnFan(void);//打开风机
		void TurnOffFan(void);//关闭风机
		void TurnOnFanDelay(void);//关闭风机，30分钟后开风机
		void TurnOffFanDelay(void);//10分钟后关闭风机
		void SystemLED_FAN_CTL(void);//LED状态灯控制
		void GetSystemErrState(void);
		void SystemPowerLimit(void);//根据电池和模块错误状态更新功率限制
		
		//void AcdcDcdcSet(void); //模块参数设置
		//uint8_t AcdcDcdcRead(void); //模块参数读取
		//void bmsDatRead(void);//BMS参数读取
		//void ChargeControl(void);//充电控制
		//void DischargeControl(void);//放电控制
		//void AcdcDcdcChargeInit(void);
		//void AcdcDcdcChargeSet(void);//充电参数设置
		//void AcdcDcdcChargeStop(void);//停止充电参数设置
		//void AcdcDcdcDischargeSet(void);//放电参数设置
		//void AcdcDcdcDischargeStop(void);//停止放电参数设置


	public:

		void getEPOError(_CALENDAR* epoTime);
		void ClearEPOError();
		uint16_t getSystemPowerSet();
		int8_t  getSystemTurnOnOffState(void);
		int8_t  getSystemOnOfflineState(void);
		int8_t  getSystemChargeState(void);
		int8_t GetErrState(void);
		void Dat_Init(void);//初始化
		void SystemTurnOnOnline(void);//并网开机
		void SystemTurnOffOnline(void);//并网关机
		void SystemTurnOnOffline(void);	//离网开机
		void SystemTurnOffOffline(void);//离网关机
		void SystemRestart();//系统重启
		void SystemSchedule(void);//系统任务
//		void SetChargeControler(Class_ChargeControler* );//设置充电模块指针

     
};




#endif




