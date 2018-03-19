#include "Publics.h"

void   Class_RS485App::Dat_Init(void)
{

}

void Class_RS485App::App_MainRout(void)
{
	App_MsgRecvProc();
	App_MsgXmitProc();
	
}

void   Class_RS485App::App_MsgXmitProc(void)
{
	_ModBusTxDef message; 
	static uint16_t s_XmitCnt = 0;
	static uint16_t delayTime = 0;
	uint16_t t = 0;
	if (g_st_TimerFlag.b10ms == 1)
	{
		s_XmitCnt++;
		if ((s_XmitCnt % ((delayTime / 10) + 4 )) != 0)
		{
			return;
		}
        s_XmitCnt = 0;
		if (E_QUEUE_NULL != objModBusProtocol.m_ModBusXmitQue.FlagQueue())
		{
			//接收队列里面已经有比较老的数据，接收后删除该数据
			message = objModBusProtocol.m_ModBusXmitQue.GetOldestNode();
			t = message.u16DataLen * 2 / objStoredEnergy.BaudrateFactor + MODULEACKTIME;
			//对计算的时间进行限制，避免算错进入死循环,目前取值较大保证通讯正常
			//@todo优化限制时间
			if(t >= 130 && t <= 200 )
			{
				delayTime = t;
			}
			objModBusProtocol.m_ModBusXmitQue.DelQueue();
			objModBusProtocol.AppToModbus(message);
		}
	}
}

void Class_RS485App::App_MsgRecvProc(void)
{
	_ModBusRxDef tempData;
	
	if(E_QUEUE_NULL != objModBusProtocol.m_ModBusRecvQue.FlagQueue())
	{
		//m_u16_ModuleHaveMsgXmit = 1;
		tempData = objModBusProtocol.m_ModBusRecvQue.GetOldestNode();
		objModBusProtocol.m_ModBusRecvQue.DelQueue();
		
		if(((tempData.u8MacId >= MAC_ID_DCAC0)&&(tempData.u8MacId <= MAC_ID_DCAC7))
				&& (objPcuModel.m_i16_SelfAddr >= MAC_ID_StoreEnergyBOX1 && objPcuModel.m_i16_SelfAddr <= MAC_ID_StoreEnergyBOX30))
		{
			//m_u16_ACDCHaveMsgXmit = 1;
			objStoredEnergy.App_RecvACDCMsgDispatch(&tempData);
		}
			
		else if(((tempData.u8MacId >= MAC_ID_BATT_DCDC0)&&(tempData.u8MacId <= MAC_ID_BATT_DCDC7))
				&&  (objPcuModel.m_i16_SelfAddr >= MAC_ID_StoreEnergyBOX1 && objPcuModel.m_i16_SelfAddr <= MAC_ID_StoreEnergyBOX30))
		{
			m_u16_DCDCHaveMsgXmit = 1;
			objStoredEnergy.App_RecvDCDCMsgDispatch(&tempData);
		}
	}
}


int16_t Class_RS485App::App_SendMsg(_ModBusTxDef msg)
{
	int16_t ret = -1;

//	if((msg.u8CmdType == DCAC_CMD_SET) && (msg.u16DataStartAdd == DCAC_CMD_ONOFF))
//	{
//		objModBusProtocol.m_ModBusXmitQue.ClearQueue();
//	}
	if (E_QUEUE_FULL != objModBusProtocol.m_ModBusXmitQue.FlagQueue())
	{
		objModBusProtocol.m_ModBusXmitQue.InsQueue(msg);

		ret = 1;
	}

	return ret;
}





