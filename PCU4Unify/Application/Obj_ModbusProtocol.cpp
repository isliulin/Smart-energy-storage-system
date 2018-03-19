
/*==========================================================*/
/* Title		:	Obj_DCBus.cpp		    */
/* Description	: 	Class_ChargeManage realization 	    */
/* Date			:				    */
/*==========================================================*/

#include "Publics.h"
#include <math.h>

void  Class_ModBusProtocol::Dat_Init()
{
	m_ModBusRecvQue.QueueInit();
	m_ModBusXmitQue.QueueInit();
	m_ACIDRecvQue.QueueInit();
	m_DCIDRecvQue.QueueInit();
	m_ModBusRecvProtocol.ReceCount = 0;
	
	memset(m_ModBusRecvProtocol.ReceBuf,0,sizeof(_ModbusRecvProtocolDef));
}

int8_t  Class_ModBusProtocol::ModBusBspToApp(uint8_t* data,_ModBusRxDef *ModBusRecvData)
{
	int ret = 0;
	int i = 0;
	u16 uiCrcData;
	uint8_t datalen = 0;
	ModBusRecvData->u8MacId = data[0];
	ModBusRecvData->u8CmdType = data[1];
	ModBusRecvData->u8DataLen = data[2];
	
	datalen = data[2];
	for(i = 0;i < ModBusRecvData->u8DataLen;i++)
	{
		ModBusRecvData->u8Data[i] = data[3+i];
	}
	ModBusRecvData->u16Crc = *((uint16_t*)(&data[datalen+3]));
	uiCrcData = uiCRC16(m_ModBusRecvProtocol.ReceBuf,ModBusRecvData->u8DataLen+3);
	uiCrcData = BIG_LITTLE_TRANVERSE16(uiCrcData);
	if(ModBusRecvData->u16Crc == uiCrcData)
	{
		ret = 1;
	}
	return ret;
}

void Class_ModBusProtocol::RS485RecvFcb(uint8_t* data)
{
	 int8_t ret;
	 _ModBusRxDef message;
	 ret = ModBusBspToApp(data,&message);
	 if(ret == 1)
	 {
	 	m_ModBusRecvQue.InsQueue(message);
	 }

}

int16_t   Class_ModBusProtocol::GetModBusMsgProc(void)
{
	_ModBusTxDef message; 
//	int16_t ret = 0;
	while(E_QUEUE_NULL != m_ModBusXmitQue.FlagQueue())
	{
		//???????????????????????????????????????????
		message = m_ModBusXmitQue.GetOldestNode();
		m_ModBusXmitQue.DelQueue();
		AppToModbus(message);
	//	return ret;
	}
	return E_QUEUE_NULL;
}


void Class_ModBusProtocol::AppToModbus(_ModBusTxDef  modBusData)
{
	u16 uiCrcData;

	m_modBusTxBuff.SendBuf[0] = modBusData.u8MacId;                  //
	m_modBusTxBuff.SendBuf[1] = modBusData.u8CmdType;                       //??????
	m_modBusTxBuff.SendBuf[2] = (modBusData.u16DataStartAdd>> 8) & 0x00ff;  //??????????
	m_modBusTxBuff.SendBuf[3] = modBusData.u16DataStartAdd& 0x00ff;
	m_modBusTxBuff.SendBuf[4] = (modBusData.u16DataLen>> 8) & 0x00ff;     					//???????????
	m_modBusTxBuff.SendBuf[5] = modBusData.u16DataLen&0x00ff;
	uiCrcData = uiCRC16(m_modBusTxBuff.SendBuf,6);
	m_modBusTxBuff.SendBuf[6] = (uiCrcData >> 8) & 0x00ff;
	m_modBusTxBuff.SendBuf[7] = uiCrcData & 0x00ff;
	BSP_METER485SendData(m_modBusTxBuff.SendBuf,8);
}

/***************************************************************************
??????:ModuBusNoReceData(void)
???????:
????:
????:
****************************************************************************/
void Class_ModBusProtocol::ModuBusNoReceData(void)
{
	if(g_st_TimerFlag.b1ms == 1)
	{
		if(m_ModBusRecvProtocol.ReceTimeOut > 0)
		{
			m_ModBusRecvProtocol.ReceTimeOut--;
			if((m_ModBusRecvProtocol.ReceTimeOut == 0)&&(m_ModBusRecvProtocol.ReceCount > 0))
			{
				BSP_METER485_RD();
				m_ModBusRecvProtocol.ReceCount = 0;
			}
		}
	}
}


