/*==========================================================*/
/* Title		:	Obj_StoredEnergy.h						*/
/* Description	:	Class_StoredEnergy definition			*/
// Author		:	zhangpeiqi								*/
/* Date			:	2017.11.14								*/
/*==========================================================*/

#ifndef _STOREDENERGY_H
#define _STOREDENERGY_H
#include "Publics.h"

#define	ACDC_NUM 	3
#define DCDC_NUM    3
#define ACDC_TEST_ID	2
#define DCDC_TEST_ID	5
#define ACDCMINADDR	20
#define ACDCMAXADDR 39
#define DCDCMINADDR 40
#define DCDCMAXADDR 59
#define	ACDC_ON		(1)
#define	ACDC_OFF	(2)
#define ACDC_NUM_MAX 20
#define DCDC_NUM_MAX 20

#define DCAC_CMD_REQ       	0x03
#define DCAC_CMD_SET        0x05
#define DCAC_CMD_WRITE    0x06
#define DCAC_CMD_ONOFF       0x0202
#define MAX(x,y)   (((x)>=(y))?(x):(y))

//模块离线状态
enum Module_offline_state
{
	MODULE_LINK_NORMAL = 0,
	MODULE_LINK_ABNORMAL = 1,
	UNKNOWN_STATE		= -1
};
//ACDC模块数据地址
enum ACDC_Read_Addr
{
	DCAC_U_VOLT = 0x0001,
	DCAC_U_CURR = 0x0002,
	DCAC_V_VOLT = 0x0003,
	DCAC_V_CURR = 0x0004,
	DCAC_W_VOLT = 0x0005,
	DCAC_W_CURR = 0x0006,
	DCAC_FREQ	= 0x0007,
	DCAC_SYS_POWER_P = 0x0008,
	DCAC_SYS_POWER_N = 0x0009,
	DCAC_SYS_POWER_V = 0x000A,
	DCAC_PF     = 0x000B,
	DCAC_CURR_P = 0x000D,
	DCAC_CURR_N = 0x000E,

	DCAC_VOLT_P = 0x000F,
	DCAC_VOLT_N = 0x0010,
	DCAC_VOLT_D = 0x0011,
	DCAC_POWER  = 0x0012,
	DCAC_RUN_STATE = 0x0013,
	DCAC_WARN_STATE = 0x0014,
	DCAC_FAULT_STATE = 0x0015,

	DCAC_RESERVED1 = 0x0016,
	DCAC_IGBTTEMP = 0x002B,
};

//读取DCDC参数
enum DCDC_Read_Addr
{
	DCDC_P_Voltage     = 0X0001,
	DCDC_INPUTCURRENT  = 0x000D,
	DCDC_OUTPUTCURRENT = 0x000E,
	DCDC_RESERVED1     = 0x0016,
	DCDC_IGBTTEMP      = 0x002B,
	
};

//设置DCDC参数
#define	DCDC_CURRENTSET				0x1003
//模块运行状态
enum Module_Run_State
{
	MODULE_STATE_RUN	=	0,
	MODULE_STATE_WARN	=	1,
	MODULE_STATE_FAULT	=	2,
};
//模块设置参数读取偏移量
#define OFFSET 100
#define POWLIMIT  0x1002

//模块设置参数地址（无符号）
typedef enum 
{
	WORKMODE = 0x1000,			//工作模式
	VOLTSETTING = 0x1001,		//直流电压设置
	CONSTANTVOLT_CURRLIMIT = 0x1002,	//恒压模式电流限制
	POWER_RATE = 0x1004,		//功率变化率
	REACTPOWMODE = 0x1006,		//无功设置方式
	ONOFFLINE_SET = 0x1011,		//并离网设置
	DC_MAXVOLT = 0x1012,		//直流最大电压
	DC_MINVOLT = 0x1013,		//直流最小电压
	DISCHARGE_MAXCURR = 0x1014,//最大放电电流
	CHARGE_MAXCURR = 0x1015,	//最大充电电流
	DISCHARGE_MAXPOWER = 0x1016,//最大放电功率
	CHARGE_MAXPOWER = 0x1017,	//最大充电功率
	SING_PARACONNECT = 0x1018,	//单并机标志
	MASTERORSLAVE = 0x1019,		//主/从机标志
	MODULENUM	= 0x101A,		//模块数量
	PARACONNECTADDR = 0x101B,		//并机地址
	MODULEADDR  = 0x101C,		//模块位置
	MODBUSADDR  = 0x101D,		//modbus地址
}_ModuleWriteAddrDef_U;

//模块设置参数地址（有符号）
typedef enum 
{
	ENERGY_P = 0x1003,			//有功设定
	CONSTANTCURR = 0x1005,		//恒流模式电流值
	REACTPOWSET = 0x1007,		//无功设置
	REACTPOWFACT = 0x1008,		//功率因数
	FUNCTIONMODE = 0x100C,		//功能模式
	LVRT_HIGHVOLT = 0x100D,		//LVRT高额定电压
	LVRT_LOWVOLT = 0x100E,		//LVRT低额定电压
	LVRT_REIMBURSEDTIME = 0x100F,	//LVRT补偿时间
	ISLANDP_FREQ_FACTOR = 0x1010, //孤岛保护频率扰动系数
}_ModuleWriteAddrDef_S;	

//模块设置参数地址
typedef enum 
{
	DCDC_WORKMODE = 0x1000,			//工作模式
	DCDC_VOLTOUT = 0x1001,		//直流电压设置
	DCDC_MAXPOW_LIMIT = 0x1002,	//恒压模式电流限制
	DCDC_CURRENT = 0x1003,		//功率变化率
}_DC_ModuleWriteAddrDef;	
	
enum ACDC_Fault_Word
{
	OUTPUT_SHORT_CIRCUIT = 0x000A,
	OUTPUT_OVER_CURR = 0x000C,
};
/*
enum Module_Fault
{
	Module_NORMAL			= 0,	//模块正常
	Module_AC_UNBANLANCE	= 1,	//模块交流不平衡告警
	Module_AC_LACKPHASE		= 2,	//模块交流缺相告警
	Module_CURR_UNBANLANCE	= 3,	//模块严重不均流
	Module_COMM_ABNORMAL	= 4,	//模块内部通信异常告警
	Module_OFFLINE			= 5,	//模块离线告警
};
*/
typedef struct
{
	uint8_t	aModule_NORMAL;
	uint8_t	aModule_AC_UNBANLANCE;
	uint8_t	aModule_AC_LACKPHASE;
	uint8_t	aModule_CURR_UNBANLANCE;
	uint8_t	aModule_COMM_ABNORMAL;
	uint8_t	aModule_OFFLINE;
}_ACModuleFaultDef;

typedef struct
{
	uint8_t	bModule_NORMAL;
	uint8_t	bModule_AC_UNBANLANCE;
	uint8_t	bModule_AC_LACKPHASE;
	uint8_t	bModule_CURR_UNBANLANCE;
	uint8_t	bModule_COMM_ABNORMAL;
	uint8_t	bModule_OFFLINE;
}_DCModuleFaultDef;

typedef struct
{
	_ACModuleFaultDef		st_ACModuleFault;	//模块故障
	_DCModuleFaultDef		st_DCModuleFault;
}_ModuleFaultDef;
///////////////////////////////////ACDC模块信息//////////////////////////////////////
/*
//ACDC故障
typedef struct	
{
	//byte0
	uint16_t	GridVolHigh		:1;	//电网电压高故障
	uint16_t	GridVolLow		:1;	//电网电压低故障
	uint16_t	VolPhaseFault	:1;	//电压相序故障
	uint16_t	OC	:1; //过流
	uint16_t	OT	:1; //过温
	uint16_t	ChargeTimeout		:1;//充电超时
	uint16_t	ParaAddOverRange	:1; //并机地址超限
	uint16_t	AddrConflict	:1; //并机地址超限
	uint16_t	ParaCableFault	:1;//并机线缆故障
	uint16_t	ParaMastConflict	:1;//并机主机冲突
	uint16_t	SingnalCableFault	:1; //信号线缆故障
	uint16_t	PowerPCBDismatch	:1;//功率版型不一致
	uint16_t	DC_OV	:1;//直流输出过压
	uint16_t	FlashFault	:1;//写Flash故障
	uint16_t	OCTimes_OR	:1; //过流次数超限
	uint16_t	OVTimes_OR	:1;	//过压次数超限
		
}_ACDCFaultDef;
*/
//ACDC告警
typedef struct	
{
	//byte0
	uint16_t Fan1Fault		:1;	//风扇1告警
	uint16_t Fan2Fault		:1;	//风扇1告警
	uint16_t Fan3Fault		:1;	//风扇3告警
	uint16_t Reserved	:13;	//保留
	
		
}_ACDCWarmDef;

//ACDC运行状态
typedef struct
{
	
	uint16_t WarmFlag		:1;	//告警
	uint16_t Contrav_Run	:1;	//逆变运行
	uint16_t Rectif_Run		:1;	//整流运行
	uint16_t Reserved		:13;//
		
}_ACDCWorkStateDef;

//ACDC模块数据第一帧
typedef struct
{

	uint16_t 		U_Voltage;		//U电压,扩大10倍
	int16_t 		U_Current;		//U电流，扩大10倍，-400A偏移	
	uint16_t 		V_Voltage;		//V电压,扩大10倍
	int16_t 		V_Current;		//V电流，扩大10倍，-400A偏移	
	uint16_t 		W_Voltage;		//W电压,扩大10倍
	int16_t 		W_Current;		//W电流，扩大10倍，-400A偏移
	uint16_t 		Freq;			//频率扩大100倍	
	int16_t 		SYSPowerP;		//有功功率，最小分辨率为10W
	int16_t 		SYSPowerN;		//无功功率
	uint16_t 		SYSPowerV;		//可视功率
	int16_t 		PF;				//电流，扩大10倍，-400A偏移
	uint16_t		TopPlate_State;	//上板State
	int16_t        	P_Current;		//正电流
	int16_t         N_Current;		//负电流
	uint16_t        P_Voltage;		//正电压
	uint16_t        N_Voltage;		//负电压

	uint16_t        Dou_Voltage;    //双边电压	
	int16_t 		DCPower;	//直流功率
	_ACDCWorkStateDef		WorkState;			//工作状态	 bit0：告警bit1：运行bit2：故障
	uint16_t		WarnState;		//告警状态
	uint16_t		FaultState;		//故障状态	
	int16_t			Input_DC1;		//输入直流1
	int16_t			Input_DC;		//输入直流
	uint16_t		TopPlate_Fault;	//上板故障
	uint16_t		HardwareVersion_H;			//硬件版本号高位
	uint16_t		HardwareVersion_L;			//硬件版本号低位
	uint16_t		SoftVersion_H;				//软件版本号高位
	uint16_t		SoftVersion_L;				//软件版本号低位
}_ACDCDataDef_1;

//ACDC模块数据第二帧
 typedef struct
{
	int16_t IGBT1_Temp;
	int16_t IGBT2_Temp;
	int16_t IGBT3_Temp;
	int16_t IGBT4_Temp;
	int16_t IGBT5_Temp;
	int16_t IGBT6_Temp;

	int16_t Import_Temp; 		//模块进风口温度
	int16_t Export_Temp;		//模块出风口温度
}_ACDCDataDef_2;
/*
//ACDC设置数据第一帧
typedef struct
{
	uint16_t    Mode;				//模式
	uint16_t    VOLT_SET;			//电压设置
	uint16_t    Power_P;			//最大功率限制
	int16_t     Power_N;			//有功设定
	uint16_t    Power_K;			//功率变化率
	uint16_t 	Reserved1[1];		//保留
	uint16_t	React_SET_Mode;		//无功设置方式
	int16_t 	ReactPow;			//无功设定
	int16_t		PowFact;			//功率因数
}_ACDCSettingDataDef_1;

//ACDC设置数据第二帧
typedef struct
{
	uint16_t	Sing_ParaConnect;	//单并机标志
	uint16_t	MasterSlave;		//主从机标志
	uint16_t	ModuleNumber;		//模块数量
	uint16_t	ConnectAddr;		//并机地址
	uint16_t	ModuleAddr;			//模块位置
	uint16_t	ModbusAddr;			//modbus地址
	
}_ACDCSettingDataDef_2;
*/

//ACDC设置数据
typedef struct
{
	uint16_t    Mode;				//模式
	uint16_t    VOLT_SET;			//电压设置
	uint16_t    Power_P;			//最大功率限制
	int16_t     Power_N;			//有功设定
	uint16_t    Power_K;			//功率变化率
	int16_t 	ConstantCurr;		//恒流模式电流值
	uint16_t	React_SET_Mode;		//无功设置方式
	int16_t 	ReactPow;			//无功设定
	int16_t		PowFact;			//功率因数
	uint16_t	Reserved1[3];		//保留
	int16_t		FunctionMode;		//功能模式
	int16_t		LvrtHighVolt;		//lvrt高额定电压
	int16_t		LvrtLowVolt;		//lvrt低额定电压
	int16_t     Lvrt_ReimbursTime;	//lvrt补偿时间
	int16_t		IslandP_FreqFactor;	//孤岛保护频率扰动系数
	uint16_t	OnOffLineSet;		//并离网设置
	uint16_t	DCMaxVolt;			//直流最大电压
	uint16_t	DCMinvolt;			//直流最小电压
	uint16_t	Discharge_MaxCurr;	//放电最大电流
	uint16_t	Charge_MaxCurr;		//充电最大电流
	uint16_t	Discharge_MaxPower;	//放电最大功率
	uint16_t	Charge_MaxPower;	//充电最大功率
	uint16_t	Sing_ParaConnect;	//单并机标志
	uint16_t	MasterSlave;		//主从机标志
	uint16_t	ModuleNumber;		//模块数量
	uint16_t	ConnectAddr;		//并机地址
	uint16_t	ModuleAddr;			//模块位置
	uint16_t	ModbusAddr;			//modbus地址
}_ACDCSettingDataDef_1;

//ACDC读取数据（所有数据）
typedef struct
{
	_ACDCDataDef_1	stMeter;		//读取量
	uint16_t		Reserved1[14];	//保留
	_ACDCDataDef_2	stMeter2;		//读取量2
	
}_ACDCReadDataDef;

//ACDC模块读取数据
typedef struct
{
	uint16_t     Online;			//在线状态
	uint16_t     u16OfflineCnt;
	uint16_t	 u16State;
	uint8_t		 ModID;				//模块ID
	_ACDCDataDef_1	stMeter;		//读取量
	uint16_t		Reserved1[14];	//保留
	_ACDCDataDef_2	stMeter2;		//读取量2
	_ACDCSettingDataDef_1	stConfig1;	//设置量
	//_ACDCSettingDataDef_2	stConfig2;	//设置量2

}_ACDCDataDef;

///////////////////////////////////DCDC模块信息//////////////////////////////////////

typedef struct	
{
	//word
	uint16_t    Fault_InputVolHigh        :1;	//输入电压高故障
    uint16_t    Fault_InputVolLow		    :1;	//输入电压低故障
    uint16_t	Fault_OverCurrent	    :1;	//过流故障
    uint16_t	Fault_OverTemp	                :1; //过问故障
    uint16_t	SoftStartTimeout	                :1; //软起超时
    uint16_t	ParaAddOverRange	:1; //并机地址超限
    uint16_t	ParaAddrConflict	    :1; //并机地址冲突
    uint16_t	ParaCableFault	    :1; //并机线缆故障
    uint16_t	ParaMastConflict	:1; //并机主机冲突
    uint16_t	SingnalCableFault	:1; //信号线缆故障
    uint16_t	PowerPCBDismatch	:1; //功率板类型不匹配
    uint16_t	Reserved	            :1; //保留
    uint16_t    Reserved1           :1; //保留
    uint16_t	FlashFault	        :1; //写FLASH故障
    uint16_t	OCTimes_OR	        :1; //过流次数超限
    uint16_t	OVTimes_OR	        :1; //过压次数超限
	
}_DCDCFaultDef;


//DCDC告警信息
typedef struct	
{
	//byte0
	uint16_t Fan1Fault		:1;
	uint16_t Fan2Fault		:1;
	uint16_t Fan3Fault		:1;
	uint16_t Reserved	    :13;			
}_DCDCWarmDef;

//DCDC运行状态
typedef struct	
{
	uint16_t    WarmFlag		:1;
	uint16_t    RunFlag         :1;
	uint16_t    FaultFlag		:1;	
	uint16_t    Reserved        :13;
	
}_DCDCWorkStateDef;

//DCDC设置信息
typedef struct
{
	uint16_t	WorkMode;
	uint16_t	VoltSet;
	uint16_t	MaxPow;
	int16_t		CurrentSet;
}_DCDCSettingDataDef;

//DCDC信息1
typedef struct
{
	uint16_t	P_Voltage;		//正母线电压
	int16_t		U_Current;		//U相电流
	uint16_t    N_Voltage;		//负母线电压
	int16_t 	V_Current;		//V相电流
	uint16_t    Dou_Voltage;    //双边电压
	int16_t 	W_Current;		//W相电流
	uint16_t 	Freq;			//频率
	int16_t 	SYSPowerP;		//有功功率，最小分辨率为10W
	int16_t 	SYSPowerN;		//无功功率
	uint16_t 	SYSPowerV;		//可视功率
	int16_t		PF;				
	uint16_t    Reserved1;
	int16_t ImportCurrent;
    int16_t ExportCurrent;
    uint16_t    ImportVolt;
    uint16_t    ExportVolt;
    uint16_t    BatterVolt;
    int16_t DCPower;
    
    _DCDCWorkStateDef  DevStatus;
    _DCDCWarmDef       AlarmStatus;
    _DCDCFaultDef      FaultStatus;
	uint16_t	Reserved2[3];
	uint16_t	HardwaveVersion_H;
	uint16_t	HardwaveVersion_L;
	uint16_t	SoftwareVersion_H;
	uint16_t	SoftwareVersion_L;
}_DCDCDataDef_1;

typedef struct
{
	int16_t IGBT1_Temp;
	int16_t IGBT2_Temp;
	int16_t IGBT3_Temp;
	int16_t IGBT4_Temp;
	int16_t IGBT5_Temp;
	int16_t IGBT6_Temp;

	int16_t Import_Temp;
	int16_t Export_Temp;
}_DCDCDataDef_2;

//DCDC读取数据（所有数据）
typedef struct
{
	_DCDCDataDef_1	stMeter;		//读取量
	uint16_t	Reserved3[14];		
	_DCDCDataDef_2	stMeter2;		//读取量2
	
}_DCDCReadDataDef;


typedef struct
{
	uint16_t     Online;			//在线状态
	uint16_t     u16OfflineCnt;
	uint16_t	 u16State;
	uint8_t		 ModID;				//模块ID
	_DCDCDataDef_1	stMeter;		//读取量
	uint16_t	Reserved3[14];		
	_DCDCDataDef_2	stMeter2;		//读取量2
	_DCDCSettingDataDef  stConfig;

}_DCDCDataDef;

typedef struct
{
	int16_t		i16_ModID;
	//_ACDCWorkStateDef	stACDCWorkState;
	_ACDCDataDef_1		stACDCData;
	_DCDCWorkStateDef	stDCDCWorkState;
}_ModuleWorkStateDataDef;

typedef struct
{
	int16_t 	i16_ModID;
	_ACDCSettingDataDef_1 stACDCSettingData1;
	//_ACDCSettingDataDef_2 stACDCSettingData2;
	_DCDCSettingDataDef stDCDCSettingData;
}_ModuleSettingDataDef;

typedef struct
{

	int16_t U_Current;		//U电流，扩大10倍，-400A偏移	
	int16_t V_Current;		//V电流，扩大10倍，-400A偏移
	int16_t W_Current;		//W电流，扩大10倍，-400A偏移
	
}_ACDCWorkCurrentDef;

typedef struct
{

	int16_t U_Current;		//U电流，扩大10倍，-400A偏移	
	int16_t V_Current;		//V电流，扩大10倍，-400A偏移
	int16_t W_Current;		//W电流，扩大10倍，-400A偏移
	
}_DCDCWorkCurrentDef;


typedef struct
{

	_ACDCWorkCurrentDef  stACDCWorkCurrent;
	_DCDCWorkCurrentDef	 stDCDCWorkCurrent;	
	
}_ModuleWorkCurrrentDef;

enum   TYPE_BATTERY
{
	INDEX_SET_BATT_CMD = 0,
};

//从EEPROM读出模块级数、个数、地址信息
typedef struct
{
	uint8_t ModuleLevel;	//	模块级数
	uint8_t ACModuleNum;		//ACDC数量
	uint8_t DCModuleNum;		//DCDC数量
	uint8_t ACModeAddr[DCDC_NUM_MAX];	//ACDC地址
	uint8_t DCModeAddr[DCDC_NUM_MAX];	//DCDC地址
	uint8_t ACMajorNum;	//ACDC主机数量
	uint8_t DCMajorNum;	//DCDC主机数量
	uint8_t ACMajorAddr[ACDC_NUM_MAX];//ACDC主机地址
	uint8_t DCMajorAddr[DCDC_NUM_MAX];//DCDC主机地址
}_ModuleInfoDef;

class Class_StoredEnergy
{
	private:
	uint16_t  SetInsqueueDelayTime;
	uint16_t  RealDataDelayTime;
	uint8_t   SetDataFrameNum;
	uint8_t   RealDataFrameNum;
	public:
	uint16_t  ACDCSetChangeFlag;
    //_GetConfigParameterDef		m_st_ModuleInfo;
    _ModuleInfoDef	 m_st_ModuleInfo;
	_ACDCDataDef	 m_st_aACDCData[ACDC_NUM_MAX];		//ACDC模块数据
	_DCDCDataDef	 m_st_bDCDCData[DCDC_NUM_MAX];	//DCDC模块数据
	_ModuleWorkStateDataDef m_st_ModuleWorkState;		//模块工作状态
	_ModuleWorkCurrrentDef	m_st_ModuleWorkCurrrent;	//模块工作电流
	_ModuleSettingDataDef m_st_ModuleSettingdata;		//模块设置信息
	uint8_t			m_u8_RunState;	 //运行状态
	uint8_t 		BaudrateFactor;	 //波特率系数，指每秒可以传送的字节数，用来算查询数据的入队时间
	_ModuleFaultDef m_st_ModuleFault;
	private:
		void	App_TransReqData(uint8_t dcacID,uint8_t cmd,uint16_t dataAdd,uint16_t dataLen);
		void	App_DataScheduling(void);
		void	Dat_RefreshModuleWorkState(uint16_t index);
		void	Dat_RefreshACDCSetData(uint16_t acdcID);
		void	Dat_RefreshDCDCSetData(uint16_t dcdcID);
		void 	App_ModuleOffLineCrl(void);
	public:
		void	Dat_Init(void);
		void	App_Manager(void);
		void	App_RecvACDCMsgDispatch(IN _ModBusRxDef* pApdu);
		void	App_SetModuleOnOff(uint8_t dcacID, uint16_t  u16DCACState);
		Module_Run_State	App_GetModuleState(uint16_t moduleID);
		//void	App_SetAllModulesOnOff(void);
		void	App_RecvDCDCMsgDispatch(IN _ModBusRxDef* pApdu);
		void	App_SetACDCAddr(uint8_t dcacID, uint16_t  u16Addr);
		void	App_SetAllACDCLimitPowers(void);
		void	App_SetACDCParaTo485(uint8_t dcacID,uint16_t setID, uint16_t  u16para);
		void	App_SetACDCPara(void);
		void	App_SetDCDCParaTo485(uint8_t dcdcID,uint16_t setID, uint16_t  u16para);
		void	App_SetDCDCPara(void);
		uint8_t	Dat_GetModuleRunState(uint16_t moduleID);
		void	APP_SetACDCWorkMode(uint8_t acdcID,uint16_t u16para);	//设置ACDC工作模式
		void	APP_SetACDCOutVolt(uint8_t acdcID,uint16_t u16para);	//设置ACDC直流输出电压
		//void	APP_SetACDCMaxPow(uint8_t acdcID,uint16_t u16para);	//设置ACDC最大功率
		void	APP_SetACDCEnergy(uint8_t acdcID,int16_t para);	//ACDC有功设定
		void	APP_SetACDCReactPow(uint8_t acdcID,int16_t para);	//ACDC无功设定
		void	APP_SetACDCReactPowFact(uint8_t acdcID,int16_t para);	//ACDC无功功率因数设定
		void	APP_SetACDCFunctionMode(uint8_t acdcID,int16_t para);	//ACDC功能模式设置
		void	APP_SetDCDCWorkMode(uint8_t dcdcID,uint16_t u16para);	//设置DCDC工作模式
		void	APP_SetDCDCOutVolt(uint8_t dcdcID,uint16_t u16para);	//设置DCDC直流输出电压
		void	APP_SetDCDCMaxPow(uint8_t dcdcID,uint16_t u16para);	//设置DCDC最大功率
		void	APP_SetDCDCCurrent(uint8_t acdcID,int16_t para);	//设置DCDC电流
		uint8_t	APP_GetACDCWorkMode(uint8_t acdcID,uint16_t u16para);	//读ACDC工作模式
		uint8_t	APP_GetACDCOutVolt(uint8_t acdcID,uint16_t u16para);	//读ACDC直流输出电压
		//uint8_t	APP_GetACDCMaxPow(uint8_t acdcID,uint16_t u16para);	//读ACDC最大功率
		uint8_t	APP_GetACDCEnergy(uint8_t acdcID,int16_t para);	//读ACDC有功
		uint8_t	APP_GetACDCReactPow(uint8_t acdcID,int16_t para);	//读ACDC无功
		uint8_t	APP_GetACDCFunctionMode(uint8_t acdcID,int16_t para);	//ACDC功能模式读取
		uint8_t	APP_GetACDCReactPowFact(uint8_t acdcID,int16_t para);	//读ACDC无功功率因数
		uint8_t	APP_GetDCDCWorkMode(uint8_t dcdcID,uint16_t u16para);	//读DCDC工作模式
		uint8_t	APP_GetDCDCOutVolt(uint8_t dcdcID,uint16_t u16para);	//读DCDC直流输出电压
		uint8_t	APP_GetDCDCMaxPow(uint8_t dcdcID,uint16_t u16para);	//读DCDC最大功率
		uint8_t	APP_GetDCDCCurrent(uint8_t dcdcID,int16_t para);	//读DCDC电流
		int16_t APP_GetMaxTemp(void);	//获取最大进风口温度
		uint8_t APP_GetACDCFaultstate(uint16_t faultname);//获取ACDC具体故障状态
		//void	APP_GetACDCFault(void);	//ACDC故障获取
		//void	APP_GetDCDCFault(void);//DCDC故障获取
		Module_offline_state APP_ReportModuleLinkStatus(uint8_t moduleID);
		void	APP_SetACDCConstantCurr(uint8_t acdcID,int16_t para);	//设置ACDC恒流电流
		uint8_t APP_GetACDCConstantCurr(uint8_t acdcID,int16_t para);	//设置ACDC恒流电流
		void	APP_SetACDCUnsignedPara(uint8_t acdcID,_ModuleWriteAddrDef_U writeaddr,uint16_t para);	//设置ACDC无符号参数
		uint8_t APP_GetACDCUnsignedPara(uint8_t acdcID,_ModuleWriteAddrDef_U writeaddr,uint16_t para);//获取ACDC无符号参数
		void	APP_SetACDCSignedPara(uint8_t acdcID,_ModuleWriteAddrDef_S writeaddr,int16_t para);	//设置ACDC有符号参数
		uint8_t APP_GetACDCSignedPara(uint8_t acdcID,_ModuleWriteAddrDef_S writeaddr,int16_t para);//获取ACDC有符号参数
		uint8_t APP_GetDCPara(uint8_t dcdcID,_DC_ModuleWriteAddrDef writeaddr,uint16_t para);
		void APP_SetDCPara(uint8_t dcdcID,_DC_ModuleWriteAddrDef writeaddr,uint16_t para);
};
#endif
		

