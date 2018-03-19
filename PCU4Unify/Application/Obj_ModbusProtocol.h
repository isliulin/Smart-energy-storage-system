
#ifndef _ModbusProtocol_H
#define _ModbusProtocol_H

#include "Publics.h"
#define	 MAXCOMNUM		256


typedef struct
{			
	uint8_t 			SendBuf[40];
	uint8_t 			SendPosi;
	uint8_t 			SendCount;

}_ModBusTxDefBuf;

typedef struct
{			
	uint8_t 			ReceBuf[MAXCOMNUM];
	uint16_t 			ReceCount;
	uint16_t 			ReceTimeOut;
	uint16_t 			ReceFlag;

}_ModbusRecvProtocolDef;



class   Class_ModBusProtocol
{
		public:
			Class_ModBusRecvQueue  m_ModBusRecvQue;
			Class_ModBusXmitQueue   m_ModBusXmitQue;
			Class_ModuleIDRecvQueue m_ACIDRecvQue;
			Class_ModuleIDRecvQueue m_DCIDRecvQue;
			
			_ModBusTxDef      XmitFrame;
			_ModBusTxDefBuf   m_modBusTxBuff;
			_ModbusRecvProtocolDef m_ModBusRecvProtocol;

		private:
			

		public:
			void Dat_Init();
			
			void RS485RecvFcb(uint8_t* data);
			void AppToModbus(_ModBusTxDef  modBusData);
			int16_t GetModBusMsgProc(void);
			void ModuBusNoReceData();
			int8_t ModBusBspToApp(uint8_t* pData,_ModBusRxDef *ModBusRecvData);
		private:

                      
};

#endif


