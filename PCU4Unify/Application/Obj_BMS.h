/*==========================================================*/
/* Title		:	Obj_BMS.h								*/
/* Description	:	Class_BMS definition					*/
/* Author		:	Hexianguo								*/
/* Date			:	2017.11.14								*/
/*==========================================================*/


#include "Publics.h"
#ifndef _BMS_H
#define _BMS_H

#define C	200		//标准电流200A
#define PGN_BMSINFO_UPLOADSCU		0xDA00	//BMS信息上传SCU
#define	ENERGY_BATT_POWER_ON	(1)
#define	ENERGY_BATT_POWER_OFF	(2)

enum B2C_DATALINK_CHECK
{
	B2C_LINK_NORMAL 	= 0,	//BMS到PCU数据链路正常
	B2C_LINK_ABNORMAL	= 1	
};

//BMS 应用层数据包
typedef struct
{
    uint16_t  u16PGN;
    uint16_t  u16Priority;
    uint16_t  u16DataLen;
	uint16_t  u16MsgSendPeriod;
	uint16_t  u16DestId;
	uint16_t  u16MACId;
    uint16_t  u16XmitActive;
    //uint16_t  RECVFresh;
    uint8_t*   pDataPtr;
}_CanBMSAppMsgDef;


typedef enum   
{
	Lead_Acid_Batteries			    = 0x01,		//铅酸电池
	NiMH_Batteries				  	= 0x02,		//镍氢电池
	Lithium_Iron_Phosphate_Battery 	= 0x03,		//磷酸铁锂电池
	Lithium_Manganese_Oxide_Battery = 0x04,		//锰酸锂电池
	Lithium_Cobalt_Oxide_Battery 	= 0x05,		//钴酸锂电池
	Ternary_Material_Battery		= 0x06,		//三元材料电池
	Polymer_Lithium_Ion_Battery 	= 0x07,		//聚合物锂离子电池
	Lithium_Titanate_Batteries 		= 0x08,		//钛酸锂电池
	Other_Batteries 				= 0xFF,		//其他电池
}_BATTERY_Type;

enum BMS_ALARM
{
    BMS_NO_ALARM                = 0,
    BMS_ALARM_OCCUR             = 1
};

enum BATT_STATE 		//告警等级
{
	BATT_NORMAL 		   = 0, 
	SLIGHT_WARN 		   = 1,
	MEDIUM_WARN 		   = 2,
	SERIOUS_WARN		   = 3,
	BATT_FAULT			   = 4,
	UNKNOWN				   = -1
};


enum TURNONOFF_CMD
{
	NO_ACTION 	= 0,
	POWER_ON	= 1,	
	POWER_OFF	= 2,	
//	NO_ACTION 	= 3
};
	
enum PCS_CONTROL_RIGHT
{
	PCS_NORMAL				= 0,
	PCS_MAINTENANCE 		= 1
};

enum BMS_CONTACTOR_STATE
{
    SWITCH_OFF              = 0,
    SWITCH_ON               = 1
};

enum BATT_INSULATION_FLAG
{
    BATT_INSULATION_NORMAL              = 0, 	//电池组绝缘故障正常
    BATT_INSULATION_FAULT               = 1		//电池组绝缘故障发生
};

typedef struct
{
	uint16_t u16BattAllowedChgCurrMax;  		//电池组最大允许充电电流
	
	uint16_t u16BattAllowedDisChgCurrMax;    	//电池组最大允许放电电流
	
	uint16_t u16BattGroupVolt;  				//电池组总电压
	
	uint16_t u16BattGroupCurr;  				//电池组总电流	
	
}_Batt_Infor1Def;

typedef struct
{
	uint16_t u16BattAllowedChgPowerMax;  	//电池组最大允许充电功率
	
	uint16_t u16BattAllowedDisChgPowerMax;   //电池组最大允许放电功率
	
	uint16_t u16BattAllowedDisChgVoltMin;  	//电池组最小允许放电电压

	uint8_t Group_SOC;  						//电池组SOC
	uint8_t Group_SOH;  						//电池组SOH	
}_Batt_Infor2Def;

typedef struct
{
	uint16_t BattAllowedChgCurrMax;  			//电池组最大允许充电电流	
	uint16_t BattAllowedDisChgCurrMax;   		//电池组最大允许放电电流	
	uint16_t BattGroupVolt;  					//电池组总电压
	uint16_t BattGroupCurr;  					//电池组总电流	
	uint16_t BattAllowedChgPowerMax;			//电池组最大允许充电功率
	uint16_t BattAllowedDisChgPowerMax;			//电池组最大允许放电功率
	uint16_t BattAllowedDisChgVoltMin;			//电池组最小允许放电电压
	uint8_t Group_SOC;  						//电池组SOC
	uint8_t Group_SOH;  						//电池组SOH	
}_Batt_DataInfoDef;


typedef union
{
	uint8_t u8Word1All;
	struct
	{
	    uint8_t BattAllowedChg          :1;		//电池组充电允许
	    uint8_t BattAllowedDisChg       :1;		//电池组放电允许
	    uint8_t Reserved         		:1;		//保留
	    uint8_t BMS_Jumped			    :1;		//BMS跳机
	    uint8_t Batt_Empty              :1;		//电池放空
	    uint8_t Batt_ChgFull            :1;		//电池充满
	    uint8_t DisChg_Contactor_Status :1;		//放电接触器状态
	    uint8_t Chg_Contactor_Status    :1;		//充电接触器状态
	}Bits;
}_Batt_StateBits_Def;		//电池状态位定义

typedef struct
{
	_Batt_StateBits_Def		u8BattStateBits;		//电池状态位
	uint8_t					Batt_type;				//电池类型
	uint16_t				Batt_Capacity;			//额定容量
	uint8_t					Env_Temp;				//环境温度（主控采集温度值）
	uint8_t					Reserved;
	uint16_t				Batt_ChgCnt;			//电池充电次数（以达到80% SOC为一次计算）	
}_Batt_Infor3Def;

typedef union
{
	uint8_t u8Word1All;
	struct
	{
		uint8_t Cell_SOC_High	:1; 	//单体SOC过高
		uint8_t Cell_SOC_Low	:1;		//单体SOC过低
		uint8_t Cell_OverTemp	:1;		//单体过温
		uint8_t Cell_TempLack	:1;		//单体欠温
		uint8_t Batt_OverVolt	:1; 	//组端过压
		uint8_t Batt_LackVolt	:1;		//组端欠压
		uint8_t Cell_OverVolt	:1;		//单体过压
		uint8_t Cell_LackVolt	:1; 	//单体欠压
	}Bits;
}Status_Flag1;

typedef union
{
	uint8_t u8Word1All;
	struct
	{
		uint8_t Reserved1				:1; 	//保留
		uint8_t Reserved2				:1;		
		uint8_t ModeTempLowLimit_Warn	:1;		//模块温度下限告警
		uint8_t ModeTempHighLimit_Warn	:1;		//模块温度上限告警
		uint8_t BattCoreVolt_Over		:1;		//电芯电压差异过大
		uint8_t Batt_InsulationFault	:1; 	//电池组绝缘故障标志
		uint8_t Batt_ChgOverCurr		:1;		//电池组充电过流
		uint8_t Batt_DisChgOverCurr		:1;		//电池组放电过流
	}Bits;
}Status_Flag2;

typedef struct
{
	Status_Flag1		Slight_Warn1;		//轻度告警状态标志1
	Status_Flag2		Slight_Warn2;		//轻度告警状态标志2		
	Status_Flag1		Medium_Warn1;			//中度告警
	Status_Flag2		Medium_Warn2;		
	Status_Flag1		Serious_Warn1;		//严重告警
	Status_Flag2		Serious_Warn2;
	uint8_t				Reserved1;			
	uint8_t				Reserved2;
}_Batt_Warn_Def;

typedef struct
{
	uint16_t	Cell_VoltMin;		//单体最低电池电压
	uint8_t		CellNum_VoltMin;	//单体最低电池电压节号
	uint16_t	Cell_VoltMax;		//单体最高电池电压  
	uint8_t		CellNum_VoltMax;	//单体最高电池电压节号
	uint8_t		Reserved1;			
	uint8_t		Reserved2;
}_Cell_VoltInfo_Def;

typedef struct
{
	uint8_t		Cell_TempMin;		//单体最低电池温度
	uint8_t		CellNum_TempMin;	//节号	
	uint8_t		Cell_TempMax;		//单体最高电池温度
	uint8_t		CellNum_TempMax;	//节号	
	uint8_t		Cell_SOC_Min;		//单体最低SOC
	uint8_t		CellNum_SOC_Min;	//单体最低SOC节号
	uint8_t		Cell_SOC_Max;		//单体最高SOC
	uint8_t		CellNum_SOC_Max;	//单体最高SOC节号
}_Cell_Temp_SOC_Def;

typedef struct
{
	uint16_t	Cell_DisChgVoltMin;			//单体最低允许放电电压 
	uint16_t	Cell_ChgVoltMax;			//单体最高允许充电电压 
	uint8_t		Cell_DisChgTempMin;			//单体最低允许放电温度
	uint8_t		Cell_DisChgTempMax;			//单体最高允许放电温度
	uint8_t		Cell_ChgTempMin;			//单体最低允许充电温度
	uint8_t		Cell_ChgTempMax;			//单体最高允许充电温度
}_Cell_Info1_Def;

typedef struct
{
	uint8_t		CellNum_MidVoltLack;	//单体电池中度欠压数量
	uint8_t		CellNum_MidVoltOver;	//单体电池中度过压数量	
	uint8_t		CellNum_MidTempLow;		//单体电池中度欠温数量
	uint8_t		CellNum_MidTempHigh;	//单体电池中度过温数量	
	uint8_t		Reserved1;			
	uint8_t		Reserved2;			
	uint8_t		Reserved3;			
	uint8_t		Reserved4;			
}_Cell_Info2_Def;

typedef struct
{
	uint16_t				BMS_Mode_Id;
	_Batt_Infor1Def			B2C_Status1;		
	_Batt_Infor2Def			B2C_Status2;							
	_Batt_Infor3Def 		B2C_Status3;
	_Batt_Warn_Def			B2C_Warn;
	_Cell_VoltInfo_Def		B2C_VoltInfo;
	_Cell_Temp_SOC_Def		B2C_Temp_SOC;
	_Cell_Info1_Def			B2C_Cell_Info1;
	_Cell_Info2_Def			B2C_Cell_Info2;   
}_St_BMS_Param;


typedef union
{
	uint8_t u8Word1All;
	struct
	{
		uint8_t Contactor_Status		:1;		//接触器状态(DI状态) 0：断开 1：闭合
		uint8_t Reserved1				:1; 	//保留
		uint8_t Reserved2				:1;		
		uint8_t Reserved3				:1; 	//保留
		uint8_t Reserved4				:1;		
		uint8_t TurnOnOff_CMD			:2; 	//上/下电命令 00：无动作 01：上电    	      02：下电 03：无动作
		uint8_t Contrl_Right			:1;		//控制权 0：正常使用 1：维修状态
	}Bits;
}_PCS_SYS_ContrlCMD;			//PCS系统状态/命令

typedef struct
{
	_PCS_SYS_ContrlCMD		PCS_Contrl_CMD;	//PCS系统状态/命令
	uint8_t					Reserved1;		
	uint8_t					Reserved2;	
	uint8_t					Reserved3;	
	uint8_t					Reserved4;	
	uint8_t					Reserved5;	
	uint8_t					Reserved6;	
	uint8_t					Reserved7;	
}_C2B_Info_Def;

class   Class_BMS
{
    private:
		uint16_t 	 m_u16_BMSOfflineCnt; 

    public:

	  uint8_t	g_BmsHaveMsgXimt;
	  int16_t	m_i16_EnergyBattOnline;
	 
	  _St_BMS_Param		BMS_Param;
	  _C2B_Info_Def 	Contrl_Fram;
	  _Batt_Infor1Def	B2C_Status1;		
	  _Batt_Infor2Def	B2C_Status2;	

    private:
      void App_BMS_Can_Debug(void);
      void App_PCS_STATUS_Info(void);
      void App_PCSToBMS_Msg(void);
	  
    public:
      void Dat_Init(void);
	  
      void App_BMSProc(void);
	  void App_UploadBMSInfo();
	  BATT_STATE App_Batt_StatusFlag(void);
	  void App_MsgDispatch(_CanBMSAppMsgDef *pApdu);
	  void 	App_SetBattOnOff(uint16_t u16OnOffState);
	  
	  float App_RespMaxChgCurr(void);
	  void App_BmsOffLineCheck(void);
	  uint8_t App_ReportBmsLinkStatus(void);
	  uint8_t App_RespContactorStatus(void);
	  void App_Get_BattDataInfo(_Batt_DataInfoDef *pData);
	
};


#endif


