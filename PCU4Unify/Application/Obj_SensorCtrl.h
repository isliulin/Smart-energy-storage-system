#ifndef _CONTAIN_BATTERY_H

#define _CONTAIN_BATTERY_H
#include "Publics.h"
#define    BATTER_MAX_NUM 10
#define    SENSOR_MAX_NUM 10
//命令
#define    CMD_READ_SWITCH_STATE   0x01
#define    CMD_READ_REGISTER      0x03
#define    CMD_WRITE_SWITCH  	0x05
#define    CMD_WRITE_REGISTER   0x06
//数据地址
#define    CHANNEL_TEMP_ADDR    	0x0000        // 温度
#define    CHANNEL_HUMID_ADDR    0x0001	//湿度
#define    CHANNEL_CO0_ADDR    0x0002  //温度继电器
#define    CHANNEL_CO1_ADDR    0x0003   //湿度继电器
#define    CHANNEL_STATE_ADDR   0x0004//状态
#define    CHANNEL_CLO_ADDR      0x0100
#define    CHANNEL_CHI_ADDR      0x0101
#define    CHANNEL_CHC_ADDR      0x0102
#define    CHANNEL_CC_ADDR      0x0103
#define    CHANNEL_SD_ADDR      0x0104
#define    CHANNEL_SHC_ADDR      0x0105
#define    CHANNEL_SC_ADDR      0x0106
#define    CHANNEL_T1_ADDR      0x0107
#define    CHANNEL_T2_ADDR      0x0108
#define    CHANNEL_STP_ADDR      0x0109

//
#define  TX_DataLen   0x02


//储能电池



typedef struct
{
	uint8_t u8Cmd;
}_ToPDUCmdTurnOnMsg;
typedef struct
{
	uint8_t u8Cmd;
}_ToPDUTurnOFFCmdMsg;



typedef struct
{
	int16_t  i16TempData;
	int16_t   i16HumidData;
	int16_t   i16CO0state;
	int16_t   i16CO1state;
	int16_t   i16Swstate;


	
}_SenSorDataDef;

typedef struct
{			
	uint16_t     Online;			//在线状态
	uint16_t     u16OfflineCnt;
	uint16_t	 u16State;
	_SenSorDataDef	SensorData;

}_SenCtlDataDef;





class Class_Sensor
{
	private:
		uint8_t m_TxFreshFlag;
		uint16_t m_DataAddr;
		_ModBusTxDef m_TxData;
                
	public:
		_SenCtlDataDef   m_st_SenCtrlData[SENSOR_MAX_NUM];
		
	
	private:
		void App_TransReqDataToSensor(uint8_t SensorID,uint8_t cmd,uint16_t dataAdd,uint16_t dataLen);
		
		void App_SensorCntrlOffLineCrl(void);
  
	public:
		void DataInit();
		void MsgProc(void);
		void Ctrl_Cmd(uint8_t ValueType,uint16_t  u16DataAddr,uint8_t Value );
		void App_DataScheduling(void);
		void App_RecvMsgDispatch(IN _ModBusRxDef* pApdu);
};

#endif































