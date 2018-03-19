/*==========================================================*/
/* Title		:	Obj_FailManager.h														*/
/* Description	: 	Class_FailManager definition											*/
//Author:
/* Date		:	Sep.2010														*/
/*==========================================================*/
#ifndef		_FAIL_MANAGER_H
#define     _FAIL_MANAGER_H

#include "Publics.h"
#include "Obj_StoredEnergy.h"
/******************故障 define*******************************/

//不向集控上报模块故障定义
#define		FAIL_ACDC1_WARN				((uint16_t)1<<0)//ACDC1告警
#define 	FAIL_ACDC2_WARN				((uint16_t)1<<1)//ACDC2告警
#define 	FAIL_ACDC3_WARN				((uint16_t)1<<2)//ACDC3告警
#define		FAIL_DCDC1_WARN				((uint16_t)1<<3)//DCDC1告警
#define 	FAIL_DCDC2_WARN				((uint16_t)1<<4)//DCDC2告警
#define 	FAIL_DCDC3_WARN				((uint16_t)1<<5)//DCDC3告警
#define 	FAIL_ACDC1_FAULT			((uint16_t)1<<6)//ACDC1故障
#define 	FAIL_ACDC2_FAULT			((uint16_t)1<<7)//ACDC2故障
#define 	FAIL_ACDC3_FAULT			((uint16_t)1<<8)//ACDC3故障
#define 	FAIL_DCDC1_FAULT			((uint16_t)1<<9)//DCDC1故障
#define 	FAIL_DCDC2_FAULT			((uint16_t)1<<10)//DCDC2故障
#define 	FAIL_DCDC3_FAULT			((uint16_t)1<<11)//DCDC3故障

//向集控上报故障定义
#define		FAIL_ACDC1_COMM_ABNORMAL	((uint64_t)1<<0)//ACDC1通信故障
#define 	FAIL_ACDC2_COMM_ABNORMAL	((uint64_t)1<<1)//ACDC2通信故障
#define 	FAIL_ACDC3_COMM_ABNORMAL	((uint64_t)1<<2)//ACDC3通信故障
#define		FAIL_ACDC4_COMM_ABNORMAL	((uint64_t)1<<3)//ACDC4通信故障
#define 	FAIL_ACDC5_COMM_ABNORMAL	((uint64_t)1<<4)//ACDC5通信故障
#define 	FAIL_ACDC6_COMM_ABNORMAL	((uint64_t)1<<5)//ACDC6通信故障
#define 	FAIL_ACDC7_COMM_ABNORMAL	((uint64_t)1<<6)//ACDC7通信故障
#define 	FAIL_ACDC8_COMM_ABNORMAL	((uint64_t)1<<7)//ACDC8通信故障


#define 	FAIL_DCDC1_COMM_ABNORMAL	((uint64_t)1<<8)//DCDC1通信故障
#define 	FAIL_DCDC2_COMM_ABNORMAL	((uint64_t)1<<9)//DCDC2通信故障
#define 	FAIL_DCDC3_COMM_ABNORMAL	((uint64_t)1<<10)//DCDC3通信故障
#define 	FAIL_DCDC4_COMM_ABNORMAL	((uint64_t)1<<11)//DCDC4通信故障
#define		FAIL_DCDC5_COMM_ABNORMAL	((uint64_t)1<<12)//DCDC5通信故障
#define		FAIL_DCDC6_COMM_ABNORMAL	((uint64_t)1<<13)//DCDC6通信故障
#define		FAIL_DCDC7_COMM_ABNORMAL	((uint64_t)1<<14)//DCDC7通信故障
#define		FAIL_DCDC8_COMM_ABNORMAL	((uint64_t)1<<15)//DCDC8通信故障


#define		FAIL_BATTERY1_COMM_ABNORMAL	((uint64_t)1<<16)//电池1通信异常
#define		FAIL_BATTERY2_COMM_ABNORMAL	((uint64_t)1<<17)//电池2通信异常
#define		FAIL_BATTERY3_COMM_ABNORMAL	((uint64_t)1<<18)//电池3通信异常
#define		FAIL_BATTERY4_COMM_ABNORMAL	((uint64_t)1<<19)//电池4通信异常


#define		FAIL_ACMETER_COMM_ABNORMAL	((uint64_t)1<<24)//PCS交流电表通信异常
#define		FAIL_DCMETER_COMM_ABNORMAL	((uint64_t)1<<25)//直流电表通信异常
#define		FAIL_TEMP_COMM_ABNORMAL		((uint64_t)1<<26)//温度传感器通信异常
#define		FAIL_HUMITY_COMM_ABNORMAL	((uint64_t)1<<27)//湿度传感器通信异常
//#define		FAIL_BMS_DATA_ABNORMAL		((uint64_t)1<<28)//BMS数据异常

#define		FAIL_INPUT_LACKPHASE		((uint64_t)1<<32)//系统输入缺相
#define		FAIL_INPUT_FREQ_ABNORMAL	((uint64_t)1<<33)//系统输入频率异常
#define		FAIL_INPUT_UNDERVOLT		((uint64_t)1<<34)//系统输入欠压	
#define		FAIL_INPUT_OVERVOLT			((uint64_t)1<<35)//系统输入过压
#define		FAIL_INPUT_LOSE				((uint64_t)1<<36)//系统输入掉电

//#define		FAIL_EMERGENCY_STOP			((uint64_t)1<<40)//系统急停告警
#define		FAIL_DOOR_FAULT				((uint64_t)1<<41)//门磁故障
#define		FAIL_SPD_FAULT				((uint64_t)1<<42)//防雷器故障
#define		FAIL_ACCONTACTOR_FAULT		((uint64_t)1<<43)//交流接触器故障
#define		FAIL_FAN_TEMP_FAULT			((uint64_t)1<<44)//风机过热告警
#define		FAIL_FAN_FAULT				((uint64_t)1<<45)//风机故障
#define		FAIL_DC_CONTACTOR			((uint64_t)1<<46)//直流接触器故障
#define		FAIL_PCS_AMBINENT_TEMP		((uint64_t)1<<47)//PCS柜环境温度告警
#define		FAIL_FLOOD_FAULT			((uint64_t)1<<48)//水浸故障

#define		FAIL_OFFLINE_AC_OVERCURR	((uint64_t)1<<49)//离网AC输出过流
#define		FAIL_OFFLINE_SHORT_CIRCUIT	((uint64_t)1<<50)//离网AC输出短路

#define		FAIL_DC_FUSE1_FAULT			((uint64_t)1<<51)//直流熔丝1断故障
#define		FAIL_DC_FUSE2_FAULT			((uint64_t)1<<52)//直流熔丝2断故障
#define		FAIL_DC_FUSE3_FAULT			((uint64_t)1<<53)//直流熔丝3断故障
#define		FAIL_DC_FUSE4_FAULT			((uint64_t)1<<54)//直流熔丝4断故障
#define		FAIL_ACBREAK_FAULT			((uint64_t)1<<55)//交流断路器故障
#define		FAIL_TRANSFOR_TEMP_FAULT	((uint64_t)1<<56)//变压器过温故障
#define		FAIL_MODULEID_SELFCHECK		((uint64_t)1<<57)//模块ID自检


#define		FAIL_INSULT_ABNORM			((uint64_t)1<<59)//绝缘检测故障
#define		FAIL_BATTERY_REVERSE		((uint64_t)1<<60)//电池反接
#define		FAIL_DC_LOSEVOLT			((uint64_t)1<<61)//直流侧欠压
#define		FAIL_DC_OVERVOLT			((uint64_t)1<<62)//直流侧过压
#define		FAIL_DC_SHORT_CIRCUIT		((uint64_t)1<<63)//直流侧短路


//故障数目
#define		FAIL_NUM			60

#define		FAIL_RELEASE		0//故障消失
#define		FAIL_ACK			1//故障产生

typedef enum
{
	AC_LACKPHASE = 0,
	AC_FREQ_ABNORMAL = 1,
	AC_UNDERVOLT = 2,
	AC_OVERVOLT = 3,
	AC_LOSE = 4,
}AC_FaultDef;

typedef enum
{
    E_AUX_POWER = 0,
    E_BATTERY_POWER = 1,
    E_HVDC_POWER = 2,
    E_CHG_ONOFF = 3,
    E_Modul_NUM = 4,
}E_ModuleDef;

//关机故障及自开机故障标志等（使能标志）
typedef struct
{
    E_ModuleDef module;
    uint64_t uFailOffMask;//系统关机故障
    uint64_t uFailReleaseOnMask;//故障消失后可自开机故障
}FailPropertyMaskDef;

const FailPropertyMaskDef FailMaskTable[E_Modul_NUM] =
{
	{E_AUX_POWER,	 0x0000000000000000,0x0000000000000000},
	{E_BATTERY_POWER,0x0000000000000000,0x0000000000000000},
	{E_HVDC_POWER,	 0x0000000000000000,0x0000000000000000},
	{E_CHG_ONOFF,	 0xffffffffffff,0xfffffffffff},
};

/**********************systerm data define****************************/
//PCS柜常变量
#pragma pack(1)
typedef struct
{
	int16_t		temp;		//温度
	uint16_t	humity;		//湿度
	int16_t		Transfor_Temp;//变压器温度
	uint16_t	ChargePower;	//充电电能
	uint16_t	DisChargePower;	//放电电能
	int16_t		ActivePower;	//有功功率
	int16_t		ReactivePower;	//无功功率
	int16_t		PowerFactor;	//功率因数
	uint16_t	P_Voltage;	//直流正母线电压
	uint16_t	N_Voltage;	//直流负母线电压
	int16_t		DCPower;	//直流功率
	uint8_t		OnOffGrid;	//并离网模式
	int16_t		OutputVolt_A;//A相输出电压
	int16_t		OutputVolt_B;//B相输出电压
	int16_t		OutputVolt_C;//C相输出电压
	int16_t		OutputCurr_A;//A相输出电流
	int16_t		OutputCurr_B;//B相输出电流
	int16_t		OutputCurr_C;//C相输出电流
	int16_t		Freq;		//输出频率
	uint8_t		Sysstate;	//系统状态--01待机--02充电--03放电
	uint16_t	OnOffState;	//开关机状态--01开机--02关机
	uint16_t	PowerSetData;	//系统功率设置量
}PCSDateDef;
#pragma pack()

//PCS柜信息1(常变)
#pragma pack(1)
typedef struct
{
	uint64_t		FailState;	//故障状态
	PCSDateDef		PCSData;
}PCSDataDef_1;
#pragma pack()

//PCS柜信息2(很少变化)
#pragma pack(1)
typedef struct
{
	uint8_t		BatteryNum;	//电池路数
	uint8_t		ModulelStage;//模块级数
	uint8_t		PCUAddr;	//PCU地址
	uint8_t		PCUHardVer;	//PCU硬件版本号
	uint8_t		PCUSoftVer[3];//PCU软件版本号
	uint8_t		ACDCNum;	//ACDC个数
	uint8_t		DCDCNum;	//DCDC个数
	uint8_t		DCMeter;	//是否有直流电表及其类型
	uint8_t		ACMeter;	//是否有交流电表及其类型
	uint8_t		ForcerNum;	//单/并柜模式
	uint16_t	Volt;
	uint8_t		Freq;
	uint16_t	Elevation;
	_CALENDAR   epoErrorTime;
	
	
}PCSDataDef_2;
#pragma pack()

typedef union
{
	uint16_t FailLevel;
	struct
	{
		uint16_t FaultLed:1;	//开启故障灯
		uint16_t ModuleTurnOff:1; //模块关机
		uint16_t CheckDCState:1;//检测直流通道是否中断，若未中断，加入本地电池管理
		uint16_t EPO:1;		//EPO动作，直流断路器、交流断路器、AC、DC模块都下发关机信号，系统需要下电并与电池、电网侧断开
		uint16_t Reduce_power_p5:1;//降额百分之50
		uint16_t Reduce_power_fan:1;//风机故障降额
		uint16_t TurnOnFan_30min:1;//30分钟后开启风扇
		uint16_t BanTurnOn:1;		//不允许开机
	}Bits;
	
}FailLevelDef;

typedef struct
{
	uint8_t	ACDC1_State;
	uint8_t	ACDC2_State;
	uint8_t	ACDC3_State;
	uint8_t	ACDC4_State;
	uint8_t	ACDC5_State;
	uint8_t	ACDC6_State;
	uint8_t	ACDC7_State;
	uint8_t	ACDC8_State;
	uint8_t	DCDC1_State;
	uint8_t	DCDC2_State;
	uint8_t	DCDC3_State;
	uint8_t	DCDC4_State;
	uint8_t	DCDC5_State;
	uint8_t	DCDC6_State;
	uint8_t	DCDC7_State;
	uint8_t	DCDC8_State;
}_ModuleErrorState;

typedef enum
{
	ModuleError = 1,
	ModuleNormal = 0,
}_ModuleState;

/**********************class define****************************/

class Class_FailManager
{
	public:
		// Class_OnOffManager*  m_apOnOffManager[E_Modul_NUM];//by wangyufei 20171115

		uint64_t	m_u64_FailState;	//故障状态
		uint16_t	m_u16_ModuleFailState;	//模块故障状态
		uint32_t	m_u32_FailState_U1;	 //故障状态
		uint32_t	m_u32_FailState_U2;	 //故障状态
		PCSDataDef_1	PCSData1;
		PCSDataDef_2	PCSData2;
		FailLevelDef	PCSFailLevel;
		uint8_t		ScuOfflineError;
		_ModuleErrorState ModuleErrorState;
	private:
		uint64_t   m_u64_FailStateBak;	//故障状态备份

	public:
		void	Dat_Init(void);
		void 	App_GetFailStateMsg(void);//获取故障状态信息
		uint64_t App_GetFailState(uint64_t uFailName);
		uint8_t  App_GetModuleFailState(uint16_t moduleID);

		//report the specific failure
		void  App_vFailReport(uint64_t uFailName, uint16_t uFailValState);
		void  App_ModuleFailReport(uint16_t uFailName,uint16_t uFailValState);
		//void  App_UploadFailState(void);//上报故障状态
		void  App_UploadPCSData1(void);//上报PCS柜信息1
		void  App_UploadPCSData2(void);//上报PCS柜信息2

	public:
		friend	class	CanDataExchg;

	private:
		void  Dat_GetModuleState(void);//模块运行状态获取
		void  Dat_GetCommFailState(void);//系统所有通信故障获取-----1
		void  Dat_GetACDCOffLineState(void);//ACDC离线状态获取-----1.1
		void  Dat_GetDCDCOffLineState(void);//DCDC离线状态获取-----1.2
		void  Dat_GetBMSOffLineState(void);//电池离线状态获取-------1.3
		void  Dat_GetOffLineState(void);//温湿度、电表离线状态获取-----1.4
		void  Dat_GetSysACFault(void);//系统交流侧故障----------------2
		void  Dat_GetSysLevelFault(void);//系统级故障获取-------------3
		void  Dat_GetIOState(void);//IO量故障获取------------------------3.1
		void  Dat_GetSysDCFault(void);//系统直流侧故障
		//void  Dat_GetSysOfflineFault(void);//系统离网输出故障

		void  Dat_GetSysData(void);//系统量获取
		void Dat_UpdataFailLevel(void);//获取系统故障等级	
		void  Dat_GetBMSDataState(void);//BMS数据状态获取
		uint8_t	Dat_ModuleErrorState(void);//所有模块状态获取，有告警/故障返回1,无告警/故障返回0
};
#endif



