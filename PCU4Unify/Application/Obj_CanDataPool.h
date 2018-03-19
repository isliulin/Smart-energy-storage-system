
#ifndef _CAN_DATA_POOL_H
#define _CAN_DATA_POOL_H

#include "Publics.h"

#define PROTNO_TPDT         0xeb
#define PROTNO_TPCM         0xec
#define PROTNO_REQ          0xea

enum   MAC_ID_BATTERYBOX_NUM
{
	MAC_ID_BATTERYBOX0 = 147,
	MAC_ID_BATTERYBOX1,
	MAC_ID_BATTERYBOX2,
	MAC_ID_BATTERYBOX3 = 150

};

#define PGN_CCU_SET_REQ_PARAM       0x1D00

#define	PGN_EMERGENCY_STOP			0x5100	//充电机紧急停止充电命令	10ms	1
#define	PGN_CHARGE_START			0x5200	//充电机启动充电命令		50ms	1
#define	PGN_CHARGE_STOP				0x5300	//充电机终止充电命令		50ms	1
#define	PGN_DISCHARGE_START			0x5900	//启动放电命令		50ms	1
#define	PGN_DISCHARGE_STOP			0x6000	//终止放电命令		50ms	1

#define	PGN_TOMASTMON_CONFIRM	0xE800	//回复集控确认命令				8


//SETTING信息
#define   PGN_SET_STARTADD              	0x1D00

#define	PGN_SETBATTERY_CMD		0xCA00
//系统故障信息
#define PGN_SYS_FAILSTATE		0xE200
//系统信号
#define PGN_PCS_DATA1		0xC200
#define PGN_PCS_DATA2		0xC300

#define SET_CCU_ID	0x03						//设置CCU的ID
#define GET_CCU_ID	0x04						//查询CCU的ID

#define PROTNO_CNCTMNGT		0x63	//PDU的升级连接管理帧
#define PROTNO_DATATSFR		0xeb	//PDU的升级数据传输
#define PROTNO_PACKAGE_OVER	0x67	//本包数据接收完成响应
#define PROTNO_CODEOVER		0x68	//程序发送结束请求
#define PROTNO_CODE_OVER		0x64	//程序接收完成响应
#define PROTNO_UPGRADE                  0x61 //升级命令
#define PGN_CHARGE_RESPONE_UPDATE_REPORT   0x6200    //充电模块响应程序升级
#define MAC_ID_CHARGE1		181
//传输方式
#define	PTP				1
#define	BROADCAST		0

//PCU软硬件版本信息
#define PGN_SYSVERSION			0xC300

//桑顿储能电池
#define PGN_B2C_STATUS1			0x1000	//BMS | PGN
#define PGN_B2C_STATUS2			0x1100
#define PGN_B2C_STATUS3			0x1200
#define PGN_B2C_WarnFlag		0x1300
#define PGN_B2C_CellInfo1		0x1400
#define PGN_B2C_CellInfo2		0x1500
#define PGN_B2C_CellInfo3		0x1600
#define PGN_B2C_CellInfo4		0x1700

#define PGN_C2B_ContrlCMD		0x1600


#define  PGN_START_PDU      		0x5200
#define  PGN_STOP_PDU      			0x5300
#define	 PGN_MODULE_CONTROL			0xCD00
#define  PGN_RTCTIME_SYN			0xBF00
//ACDC柜信息
#define	PGN_ACDC_DATADEF			0xCF00	//ACDC信息
#define	PGN_CHGDIG_INFOR2   		0xD700	//储能柜DCDC信息
#define	PGN_SETTING_CMD 			0xDE00	//ACDC功率设置命令
#define	PGN_SET_MAX_POWER    		0xB500  
#define PGN_SET_ACDCPARA			0xD000	//ACDC参数设置
#define PGN_SET_DCDCPARA			0xE100	//DCDC参数设置
#define PGN_MSTMON_REQUEST			0xEA00	//DCDC参数设置
//================================ Message struct typedef================================


//=================Battery  Inner Msg PGN Define================================

#define  PGN_SLAVE_BATT_STATUS      		0x0100
#define  PGN_SLAVE_BATT_VOLT_OUT      		0x0200
#define  PGN_SLAVE_BATT_VOLT_CURR     		0x0300

//Inner 应用层数据包
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
    uint16_t   u16offset;
    uint8_t*   pDataPtr;
}_CanInnerAppMsgDef;




typedef struct
{
	_CALENDAR SyncCalendar;
	/*
	uint8_t	u8Second;		//秒/压缩BCD码
	uint8_t	u8Minute;		//分/压缩BCD码
	uint8_t	u8Hour;			//时/压缩BCD码
	uint8_t	u8Day;			//日/压缩BCD码
	uint8_t	u8Month;		//月/压缩BCD码
	uint8_t	u8YearL;		//年高位/压缩BCD码
	uint8_t	u8YearH;		//年低位/压缩BCD码
	*/
	

}_MasterTimeSyncDataDef;			//充电机时间同步


typedef	struct
{
	_CanBMSAppMsgDef	MsgObj[40];//尽量大一些，要比实际定义的大
	int16_t	msgLen;
}_ToBMSMsgObjDef;

typedef	struct
{
	_CanBMSAppMsgDef	MsgObj[30];//尽量大一些，要比实际定义的大
	int16_t	msgLen;
}_RecvBMSMsgObjDef;


typedef	struct
{
	_CanInnerAppMsgDef	MsgObj[80];//尽量大一些，要比实际定义的大
	int16_t	msgLen;
}_ToInnerMsgObjDef;

typedef	struct
{
	_CanInnerAppMsgDef	MsgObj[40];//尽量大一些，要比实际定义的大
	int16_t	msgLen;
}_RecvInnerMsgObjDef;

typedef	struct
{
	_CanMstMonAppMsgDef	MsgObj[40];		//尽量大一些，要比实际定义的大
	int16_t	msgLen;
}_ToMstMonMsgObjDef;

typedef	struct
{
	_CanMstMonAppMsgDef	MsgObj[40];		//尽量大一些，要比实际定义的大
	int16_t	msgLen;
}_RecvMastMonMsgObjDef;

typedef	struct
{
	_CanMstMonAppMsgDef	MsgObj[40];		//尽量大一些，要比实际定义的大
	int16_t	msgLen;
}_DCACToMstMonMsgObjDef;


typedef	struct
{
	_CanMstMonAppMsgDef	MsgObj[40];		//尽量大一些，要比实际定义的大
	int16_t	msgLen;
}_BATTERYToMstMonMsgObjDef;


typedef struct
{
	_CanBMSAppMsgDef   MsgObj[40];
	uint8_t datalen;

}_RecvBatteryMsg;

typedef struct
{
	_CanBMSAppMsgDef   MsgObj[40];
	uint8_t datalen;

}_TOBatteryMsg;


class   Class_CanDataPool
{
	public:
		
		//模块 and PCU交互的数据

		int16_t  		m_i16TurnonoffMoudle;
		int16_t			m_i16_TurnOnOffAddress;		//开关机设备地址
		int16_t			m_i16_ACDCID;		//ACDC设备地址
		int16_t			m_i16_ACDCSetID;		//ACDC设置地址
		int16_t			m_i16_ACDCSetData;			//ACDC设置信息
		int16_t			m_i16_SetACDCFlag;			//ACDC设置标志
		int16_t			m_i16_DCDCID;		//DCDC设备地址
		int16_t			m_i16_DCDCSetID;		//DCDC设置地址
		int16_t			m_i16_DCDCSetData;			//DCDC设置信息
		int16_t			m_i16_SetDCDCFlag;			//DCDC设置标志
		uint8_t			m_u8_PCSinfo2Flag;		//发送PCS信息2标志
		uint32_t			m_u32_HardWareVer;
		uint32_t 			m_u32_SoftWareVer;
		
		
	private:
		
		void Dat_BMSDataInterface(void);
		void Dat_InnerDataInterface(void);
		

	public:

		void Dat_Init(void);
		void Dat_DataMainten(void);
};

#endif