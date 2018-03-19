
#ifndef  RS485_COMM_H
#define  RS485_COMM_H

#include"Publics.h"



#define		COMTIMEOUT		200
#define		MODULEACKTIME	50			//模块目前响应时间为50ms

enum ID_RS485_COM
{

	MAC_ID_DCAC0 = 20,
	MAC_ID_DCAC1 = 21,
	MAC_ID_DCAC2 = 22,
	MAC_ID_DCAC7 = 27,

	MAC_ID_BATT_DCDC0 = 40,
	MAC_ID_BATT_DCDC1 = 41, 
	MAC_ID_BATT_DCDC2 = 42,
	MAC_ID_BATT_DCDC7 = 47,
};




typedef struct
{			
	uint8_t 	u8MacId;
	uint8_t 	u8CmdType;
	uint16_t 	u16DataStartAdd;
	uint16_t 	u16DataLen;
	uint16_t 	u16Crc;
}_ModBusTxDef;


typedef struct
{			
	uint8_t 	u8MacId;
	uint8_t 	u8CmdType;
	uint8_t 	u8DataLen;
	uint8_t 	u8Data[256];
	uint16_t 	u16Crc;
}_ModBusRxDef;



class Class_RS485App
{

		public:

			//_ModBusTxDef		XmitFrame;
			//_ModBusTxDefBuf   m_modBusTxBuff;
			//_ModbusRecvProtocolDef m_ModBusRecvProtocol;
			uint16_t	m_u16_ModuleHaveMsgXmit ;
			uint16_t	m_u16_ACDCHaveMsgXmit ;
			uint16_t	m_u16_DCDCHaveMsgXmit ;
			uint16_t	m_u16_ACDCRefreshID;
			uint16_t	m_u16_DCDCRefreshID;
		private:
			void   App_MsgXmitProc(void);
			void   App_MsgRecvProc(void);
			

		public:
			void 	Dat_Init(void);
			int16_t App_SendMsg(_ModBusTxDef msg);
			void 	App_MainRout(void);
		private:

};

#endif




