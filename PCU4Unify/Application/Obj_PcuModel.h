/*==========================================================*/
/* Title		:	Obj_PCUControler.h														*/
/* Description	: 	PCU controler											*/
// Author		:	wangyufei
/* Date			:	2017.10.30														*/
/*==========================================================*/



#ifndef _PCUMODEL_H
#define _PCUMODEL_H

#include "Publics.h"

#define	MAC_ID_MASTERMON			250	//0xFA  SCU Addr


//TM101地址
enum MAC_ID_StoreEnergyBOX
{
	MAC_ID_StoreEnergyBOX1 = 100,
	MAC_ID_StoreEnergyBOX2 = 101,
	MAC_ID_StoreEnergyBOX3,
	MAC_ID_StoreEnergyBOX4,
	MAC_ID_StoreEnergyBOX30 = 129,
};

typedef enum 
{
	MAC_ID_BATTERY0 = 0x01,
	MAC_ID_BATTERY1 = 0x02,
	MAC_ID_BATTERY2 = 0x03,
	MAC_ID_BATTERY3 = 0x04,
	MAC_ID_BATTERY4 = 0x05,
	MAC_ID_BATTERY5 = 0x06,
	MAC_ID_BATTERY6 = 0x07,
	MAC_ID_BATTERY7 = 0x08,
	MAC_ID_BATTERY8 = 0x09,	
	MAC_ID_BATTERY9 = 0x0a,
	MAC_ID_PCS 		= 0x27,
}_BMS_PCS_MAC_ID_Def;


typedef union
{
	uint16_t u16Word1All;

	struct
	{
		uint16_t bJ59_5 	:1;
		uint16_t bJ59_6		:1;
		uint16_t bJ59_7		:1;
		uint16_t bJ59_8		:1;//柜1灭火器反馈信号
		uint16_t bRsd8		:1;
		uint16_t bRsd7		:1;
		uint16_t bJ59_1		:1;//柜1断路器反馈信号
		uint16_t bJ59_2		:1;
		
		uint16_t bJ59_3		:1;
		uint16_t bJ59_4		:1;
		uint16_t bRsd6:1;
		uint16_t bRsd5:1;
		uint16_t bRsd4:1;
		uint16_t bRsd3:1;
		uint16_t bRsd2:1;
		uint16_t bRsd1:1;
	}EB_TM101;
}_DIStateDef;

typedef union	PDU_State
{
	uint32_t u32Word1All;
	struct
	{
		//byte0
		uint32_t bPDUState		:3;	//PDU充电机状态,0:待机状态， 1 等待充电，2 充电中，3 结束充电
		uint32_t bChgGunState	:2; //充电枪连接状态，0 没有连接   1连接中   2连接上
		uint32_t bOnline		:1;	//本PDU在线
		uint32_t bFault			:1;	//充电机故障
		uint32_t bWarn			:1;	//充电机告警

		//byte2
		uint32_t bVINGot		:1;		// VIN码获取成功
		uint32_t bChgState		:3;
		uint32_t bNumDC			:4;

		//byte3
		uint32_t bTestStage		:3;
		uint32_t bContactorRelyState    :1;		//:1; //接触器的状态
		uint32_t bUpdateFlag	:1;
		uint32_t bRes1			:1;
		uint32_t bSpdState			:1;
		uint32_t bInsulResult	:1;

		//byte4
		uint32_t bParaContactorState	:1;	//并联接触器的状态
		uint32_t  bACContactorState  :1;
		uint32_t  bFanFault			:1;
		uint32_t  bChanelMatchEndFlag		:1; //通道匹配结束标记
		uint32_t  bOutContactorSticking	:1;
		uint32_t bDoorState		:1;		
		uint32_t bTestReq		:1; //检测请求
		uint32_t bRecvOncmd		:1;
	}Bits;
}_PDUStateDef;

enum	MASTERMONITOR_NUM
{
	//E_MM_TEST = 0,
	E_MM_ALLOW_UPGRADE=0,
	E_MM_LIFE,
	E_MM_CONFIRM,
	
	E_MM_STATUS2,
	E_MM_STATUS3,
	E_MM_STATUS4,
	E_MM_STATUS5,
	E_MM_STATUS6,
	E_MM_STATUS7,
	E_MM_STATUS8,
	E_MM_STATUS1,

	E_MM_TIME_REMAIN,
	E_MM_CHG_STRATY,
	E_MM_LONGFRAME_UP,
	E_MM_LONG_RQ,


	E_MM_CONNMANA,
	E_MM_CONNMANG,
	E_MM_RECVPACKET,
	E_MM_RECVALL,
};



class Class_PcuModel
{	

	private:
//		Class_PCUOnOffControler ObjPcuOnOffCtrl;
//		Class_ChargeControler ObjChargeCtrl;



	public:
		int16_t mcp_offline_cnt;
		_DIStateDef  m_st_FBState;		
		_PDUStateDef m_st_SelfState;

	public:
		int16_t 		m_i16_SelfAddr;		


	public:

		void Dat_Init(void);//初始化
		void PCUAddr_Init(void);

		void SystemSchedule(void);//系统任务
		

     
};




#endif


