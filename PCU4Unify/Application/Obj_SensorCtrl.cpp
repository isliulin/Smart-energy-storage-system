#include "Publics.h"




void Class_Sensor::DataInit()
{

	m_TxFreshFlag = 0;
	m_DataAddr = 0;
}


void Class_Sensor::MsgProc(void)
{
	App_DataScheduling();
	App_SensorCntrlOffLineCrl();	
}

void Class_Sensor::App_RecvMsgDispatch(IN _ModBusRxDef* pApdu)
{

	/*	u8 ReadData[4];
		uint8_t i = 0;
		//发送数据处理
	
	*/

//	int16_t i,index;
//	uint8_t* pData;
	
//	index = pApdu->u8MacId - MAC_ID_SENSOR0;

//	switch(pApdu->u8CmdType)
//	{
//		case 0x03://实时数据
//			pData = (uint8_t*)(&(m_st_SenCtrlData[index].SensorData));
//			for (i = 0;i < sizeof(_SenSorDataDef);i+=2)
//			{
//                         *((int16_t*)&(pData[i]))= BIG_LITTLE_TRANVERSE16(S2U16(pApdu->u8Data[i]));
//			}
//		break;
//
//		case 0x04://设置量数据
//
//			
//		break;
//
//
//		default: 
//
//			break;
//	}

//	m_st_SenCtrlData[index].u16OfflineCnt = 0;
//	m_st_SenCtrlData[index].Online = 1;
}


void Class_Sensor::App_TransReqDataToSensor(uint8_t SensorID,uint8_t cmd,uint16_t dataAdd,uint16_t dataLen)
{
	_ModBusTxDef tempMsg;
/*
	if (SensorID > MAC_ID_SENSOR9 || SensorID < MAC_ID_SENSOR0)
	{
		return;
	}
*/
	tempMsg.u8MacId = SensorID;
	tempMsg.u8CmdType = cmd;
	tempMsg.u16DataStartAdd = dataAdd;
	tempMsg.u16DataLen = dataLen;
	objRS485App.App_SendMsg(tempMsg);
}

/***************************************************************************
函数名:App_ACDCOffLineCrl
功能描述:离线处理
作者:
日期:
****************************************************************************/
void Class_Sensor::App_SensorCntrlOffLineCrl(void)
{
  	int16_t i;
	
	if (g_st_TimerFlag.b100ms == 1)
	{
		for (i = 0; i < SENSOR_MAX_NUM; i++)
		{
			if (++m_st_SenCtrlData[i].u16OfflineCnt >= 300)
			{
				m_st_SenCtrlData[i].u16OfflineCnt = 300;
				m_st_SenCtrlData[i].Online = NO;
			}
		}
	}
}

