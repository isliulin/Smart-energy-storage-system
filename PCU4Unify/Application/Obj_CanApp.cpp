

#include "Publics.h"

//数据收发结构体
typedef void (Class_CanDataExchg::*CAN_DATA_RECV_T)(void *DataPtr);



/***************************************************************************
函数名:SysCanInit
功能描述:
作者:
日期:
****************************************************************************/
void Class_CanApp::SysCanInit(void)
{

}

/***************************************************************************
函数名：Class_CanApp::MainRoutle
功能描述：CAN短帧、长帧数据收发处理
输入参数：无
返回值：无
使用说明：
****************************************************************************/

void Class_CanApp::MainRoutle(void)
{
	CanMsgRecvProc();
	CanMsgXmitProc();
	objCanMstMonLongFrame.App_MsgProc();

}

/***************************************************************************
函数名：Class_CanApp::CanMsgRecvProc
功能描述：CAN接收消息处理,接收来自CAN1、CAN2、CAN3的消息，并放入接收队列
输入参数：无
返回值：无
使用说明：
****************************************************************************/

void Class_CanApp::CanMsgRecvProc(void)
{

	int16_t i,ret;
  //  char A=0;
	_CanBMSAppMsgDef tempData1;
	uint8_t	data1[8];
	tempData1.pDataPtr = data1;

	_CanInnerAppMsgDef tempData2;
	uint8_t	data2[8];
	tempData2.pDataPtr = data2;

	_CanMstMonAppMsgDef tempData3;
	uint8_t	data3[8];
	tempData3.pDataPtr = data3;

	int16_t i16NewMsg = E_NON_MSG_XMIT;
	

	//一个主循环只进行3次消息队列消息的解析
    //BMS,CAN2接收

	for (i = 0;i < 3;i++)
	{
		ret = objCanBMSProtocol.MsgRecvProc(&tempData1);
		
		if (ret == 0)
		{
			break;
		}
		else
		{
			i16NewMsg = objCanDataExchg.RecvBMSMsgProc(&tempData1);
		}

		if (i16NewMsg == E_HAVE_MSG_XMIT)
		{
			ret = objCanBMSProtocol.MsgXmitProc(&tempData1);
			if (ret == -1)
			{
				//错误处理
			}
		}
	}
        
    //Inter CAN1
	for (i = 0;i < 3;i++)
	{
		ret = objCanInnerProtocol.MsgRecvProc(&tempData2);
		if (ret == 0)
		{
			break;
		}
		else
		{
			 if(tempData2.u16DestId >= MAC_ID_BATTERYBOX0 && tempData2.u16DestId <= MAC_ID_BATTERYBOX3)
			{
				i16NewMsg = objCanDataExchg.RecvInnerEngBattMsg(&tempData2);
			}
			else
			{
				i16NewMsg = RecvInnerMsgDispatch(&tempData2);
			}
		}

		if (i16NewMsg == E_HAVE_MSG_XMIT)
		{
			ret = objCanInnerProtocol.MsgXmitProc(&tempData2);
			if (ret == -1)
			{
				//错误处理
			}
		}
	}

	//一个主循环只进行3次消息队列消息的解析
    //master monitor,CAN3接收
	for (i = 0;i < 3;i++)
	{
	
		ret = objCanMasterMonPrtcl.MsgRecvProc(&tempData3);			
		
		if (ret == 0)
		{
			break;
		}
		
		else
		{
			if(tempData3.u16DestId >= MAC_ID_PDU0 && tempData3.u16DestId <= MAC_ID_PDU31)
			{
				
				objCanDataExchg.MasterToInner(&tempData3);
				
			}

			else if(tempData3.u16DestId >= MAC_ID_StoreEnergyBOX1 && tempData3.u16DestId <= MAC_ID_StoreEnergyBOX30)
			{
				i16NewMsg = objCanDataExchg.RecvMonsterToStoredEnergyBoxMsg(&tempData3);
			}
			
			else
			{
				
			}
		}

		if (i16NewMsg == E_HAVE_MSG_XMIT)
		{
			ret = objCanMasterMonPrtcl.MsgXmitProc(&tempData3);

			if (ret == -1)
			{
				//错误处理
			}
		}
	}
	
}

/********************************************************************************
RecvInnerMsgDispatch-----接收信息处理
                     	
Parameters:            pApdu:接收到的数据帧
Return Value: 
Precondition: 接收到的数据在应用层又解析成模拟量，状态量，控制量，告警量信息
Postcondition: 
********************************************************************************/
int16_t Class_CanApp::RecvInnerMsgDispatch(IN _CanInnerAppMsgDef *pApdu)
{
	//CAN_APDU_T TempApdu;
	int16_t i;
	_CanMstMonAppMsgDef tempdata;
	int16_t i16NewMsg = E_NON_MSG_XMIT;
	uint16_t srcAddr = pApdu->u16MACId;
	uint16_t desAddr = pApdu->u16DestId;
	uint16_t pgn = pApdu->u16PGN;
	uint8_t temp1[8];
	tempdata.pDataPtr = temp1;

	static int16_t curr[4] = {0};
	static int16_t power[4] = {0};
	int16_t currSum = 0;
	int16_t powerSum = 0;
	static int32_t volt;

	if (srcAddr == (MAC_ID_PDU0+16))
	{
		if (pgn == 0x3300)
		{
			//power[0] = *((int16_t*)(pApdu->pDataPtr+6));
			power[0] = S2U16(pApdu->pDataPtr[6]);
		}
		if (pgn == 0x3000)
		{
			curr[0] = 4000 - *((int16_t*)(pApdu->pDataPtr+4));
			volt = *((int16_t*)(pApdu->pDataPtr+2));
		}
	}
	
	if (srcAddr == (MAC_ID_PDU0+17))
	{
		if (pgn == 0x3300)
		{
			//power[1] = *((int16_t*)(pApdu->pDataPtr+6));
			power[1] = S2U16(pApdu->pDataPtr[6]);
		}
		if (pgn == 0x3000)
		{
			curr[1] = 4000 - *((int16_t*)(pApdu->pDataPtr+4));
		}
	}

	if (srcAddr == (MAC_ID_PDU0+18))
	{
		if (pgn == 0x3300)
		{
			//power[2] = *((int16_t*)(pApdu->pDataPtr+6));
			power[2] = S2U16(pApdu->pDataPtr[6]);
		}
		if (pgn == 0x3000)
		{
			curr[2] = 4000 - *((int16_t*)(pApdu->pDataPtr+4));
		}
	}

	if (srcAddr == (MAC_ID_PDU0+19))
	{
		if (pgn == 0x3300)
		{
			//power[3] = *((int16_t*)(pApdu->pDataPtr+6));
			power[3] = S2U16(pApdu->pDataPtr[6]);
		}
		if (pgn == 0x3000)
		{
			curr[3] = 4000 - *((int16_t*)(pApdu->pDataPtr+4));
		}
	}

	for (i = 0;i < 4;i++)
	{
		currSum += curr[i];
		powerSum += power[i];
	}
	//curr[0]=4000-(curr[0]*4);
	currSum = curr[0]*4;
	/*if (volt>0)
	{
		currSum = ((int32_t)powerSum*1000)/volt;
	}
	else
	{
		currSum=0;
	}*/
	

	if (srcAddr >= MAC_ID_PDU0 && srcAddr <= MAC_ID_PDU31)
	{
		if (desAddr == 0xfa)
		{
			//转发
			for (i= 0;i<pApdu->u16DataLen;i++)
			{
				tempdata.pDataPtr[i] = pApdu->pDataPtr[i];
			}
			tempdata.u16DataLen = pApdu->u16DataLen;
			tempdata.u16DestId = 0xfa;
			tempdata.u16MACId = pApdu->u16MACId;
			tempdata.u16MsgSendPeriod = pApdu->u16MsgSendPeriod;
			tempdata.u16offset = pApdu->u16offset;
			tempdata.u16PGN = pApdu->u16PGN;
			tempdata.u16Priority = pApdu->u16Priority;
			tempdata.u16XmitActive = TRUE;

			if ((srcAddr == (MAC_ID_PDU0+16)))
			{
				if (pgn == 0x3300)
				{
					*((int16_t*)(tempdata.pDataPtr+6)) = powerSum;
				}
				if (pgn == 0x3000)
				{
					*((int16_t*)(tempdata.pDataPtr+4)) = 4000-currSum;
				}
			}
			objCanDataExchg.TransSetReqCmdToMastorMon(&tempdata,desAddr);
		}
//		objCanDataPool.m_st_aPDUParam_D[srcAddr - MAC_ID_PDU0].u16OfflineCnt = 0;//PDU 离线计数器清零
//		objCanDataPool.m_st_aPDUParam_D[srcAddr - MAC_ID_PDU0].State.Bits.bOnline = YES;
	}
	
	return i16NewMsg;
}


/***************************************************************************
函数名：Class_CanApp::CanMsgXmitProc
功能描述：将消息装入发送队列，再根据需求分别发送到BMS、后台、CCU、集控
输入参数：无
返回值：无
使用说明：
****************************************************************************/

void    Class_CanApp::CanMsgXmitProc(void)
{
 	int16_t ret;
	int16_t	newMsg = E_NON_MSG_XMIT;
	_CanBMSAppMsgDef tempData1;
   	 _CanInnerAppMsgDef tempData2;
	_CanMstMonAppMsgDef tempData3;
	
	//一个主循环只进行3次消息队列消息的解析
	ret = objCanBMSProtocol.m_CanXmitQueue.FlagQueue();

	for (int16_t i = 0; i < 4; i++)
	{
		if (E_QUEUE_FULL != ret)		//队列不满，将消息装入发送队列
		{
			newMsg = objCanDataExchg.GetToBMSMsg(&tempData1);
			
			if (E_HAVE_MSG_XMIT == newMsg)
			{
				ret = objCanBMSProtocol.MsgXmitProc(&tempData1);
			}
		}
		else
		{
			break;
		}
	}


	//一个主循环只进行8次消息队列消息的解析
	ret = objCanInnerProtocol.m_CanXmitQueue.FlagQueue();

	for (int16_t i = 0; i < 4; i++)
	{
		if (E_QUEUE_FULL != ret)
		{
			newMsg = objCanDataExchg.GetToCCUMsg(&tempData2);

			if (E_HAVE_MSG_XMIT == newMsg)
			{
				ret = objCanInnerProtocol.MsgXmitProc(&tempData2);
			}
		}
		else
		{
			break;
		}
	}


	//一个主循环只进行8次消息队列消息的解析
	ret = objCanInnerProtocol.m_CanXmitQueue.FlagQueue();

	for (int16_t i = 0; i < 4; i++)
	{
		if (E_QUEUE_FULL != ret)
		{
			newMsg = objCanDataExchg.GetBGMonitorMsg(&tempData2);

			if (E_HAVE_MSG_XMIT == newMsg)
			{
				ret = objCanInnerProtocol.MsgXmitProc(&tempData2);
			}
		}
		else
		{
			break;
		}
	}

	//一个主循环只进行8次消息队列消息的解析
	ret = objCanMasterMonPrtcl.m_CanXmitQueue.FlagQueue();
	
	for (int16_t i = 0; i < 4; i++)
	{
		if (E_QUEUE_FULL != ret)
		{
			newMsg = objCanDataExchg.GetToMastorMonMsg(&tempData3);
			
			if (E_HAVE_MSG_XMIT == newMsg)
			{
				ret = objCanMasterMonPrtcl.MsgXmitProc(&tempData3);
			}
		}
		else
		{
			break;
		}
	}
	
/*
	//事件记录，一个主循环最大上传10条记录
	for (count = 0; count < 10; count++)
	{
		if (QUEUE_FULL != ret)
		{
			newMsg = DataExchgObj.GetEventLogMsg(&tempData);
			if (E_HAVE_MSG_XMIT == newMsg)
			{
				ret = ProtocolObj.MsgXmitProc(&tempData);
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}*/
}




