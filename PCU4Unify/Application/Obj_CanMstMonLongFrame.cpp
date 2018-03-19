
#include "Publics.h"


/***************************************************************************
函数名:Dat_Init
功能描述:初始化函数
输入参数:none
返回值:none
说明:
作者:
日期:
****************************************************************************/
void Class_CanMstMonLongFrame::Dat_Init(void)
{
	m_CanRecvQueue.CanRecvQueueInit();
	
	Dat_QueueInit();
	m_u16_HaveMsgXmit = FALSE;
	m_i16_XmitState = E_XMIT_IDLE;
	m_i16_DataXmitActive = FALSE;
}

/***************************************************************************
函数名:App_TransData
功能描述:发送数据，外部发送数据调用
输入参数:原地址，目的地址，长帧编号，数据长度、数据头指针
返回值:是否发送成功
说明:
作者:
日期:
****************************************************************************/
int16_t Class_CanMstMonLongFrame::App_TransData(uint16_t u16SrcAddr,uint16_t u16DesAddr,
													uint32_t u32VaultType,uint16_t u16Datalen,uint8_t *pData)
{
	int16_t err,i;
	_DataBlockDef	*pMemBlock;
	_LongFrameDef msg;

	if( (u16Datalen <= 0) || (pData == NULL) )
	{
		return RET_ERR;
	}

	/*if (u16DesAddr < 231 || u16DesAddr > 240)
	{
		return;
	}*/
	
	//赋值
	msg.u16DataLen = u16Datalen;
	msg.u32Valuetype = u32VaultType;
	msg.u16Frame_Len = (u16Datalen-1)/7+1;
	msg.u16DesAddr = u16DesAddr;
	msg.u16SrcAddr = u16SrcAddr;
	msg.u16XmitEnd = 0;
	msg.u16RecvEnd = 0;

	if (E_QUEUE_FULL != Dat_GetFlagQueue())
	{
		//获取存储空间，写入数据
		objMemory.Dat_GetMemBlock(&pMemBlock,u16Datalen,&err);
		if (err == E_NO_ERR)
		{
			msg.pDataBlock = pMemBlock;
			for (i = 0;i < u16Datalen;i++)
			{
				msg.pDataBlock->pBlockMem[i] = pData[i];
			}
			Dat_InsQueue(msg);
            return RET_OK;
		}
	}
    return RET_ERR;
}

/***************************************************************************
函数名:App_MsgProc
功能描述:数据处理，主函数调用
输入参数:void
返回值:none
说明:
作者:
日期:
****************************************************************************/
void    Class_CanMstMonLongFrame::App_MsgProc(void)
{
	static _LongFrameDef msg;
	if(objUpgrade.UpdataFlag == 1)
	{
		return;
	}
	else
	{
		if (0 == m_u16_HaveMsgXmit)
		{
			if (E_QUEUE_NULL != Dat_GetFlagQueue())
			{
				msg = Dat_GetOldestNode();
				Dat_DelQueue();
				m_u16_HaveMsgXmit = 1;
			}
		}
		App_XmitProc(&msg);
		App_RecvProc();
	}
}

/***************************************************************************
函数名:App_XmitProc
功能描述:数据发送处理
输入参数:要发送的数据包
返回值:none
说明:
作者:
日期:
****************************************************************************/
void    Class_CanMstMonLongFrame::App_XmitProc(_LongFrameDef *msg)
{
	int16_t ret;
	static int16_t s_XmitCnt = 0;
	static uint16_t scuofflinecnt;
	switch(m_i16_XmitState)
	{
		case E_XMIT_IDLE:
			if (m_u16_HaveMsgXmit == 1)
			{
				s_XmitCnt = 0;
				m_i16_XmitState = E_XMIT_REQ;
			}
			break;
			
		case E_XMIT_REQ:
			if (1 == g_st_TimerFlag.b10ms)
			{
				s_XmitCnt++;
				if  (s_XmitCnt == 8)
				{
					scuofflinecnt++;
					if(scuofflinecnt > 125)
					{
						objFailManager.ScuOfflineError = 1;
					}
					else
					{
						objFailManager.ScuOfflineError = 0;
					}
					App_SendReq(msg->u16SrcAddr,msg->u16DesAddr,msg->u32Valuetype,msg->u16DataLen);
				}
				if (s_XmitCnt >= 8)//80ms请求一次
				{
					s_XmitCnt = 0;
				}
			}
			//连续10次请求没有响应，则发送结束
			/*if (s_XmitCnt >= 10)
			{
				m_i16_XmitState = E_XMIT_END;
			}*/

			if (m_i16_DataXmitActive == TRUE)//发送有响应
			{
				scuofflinecnt = 0;
				m_i16_XmitState = E_XMIT_DATA;
			}
			break;
			
		case E_XMIT_DATA:
			
			if (1 == g_st_TimerFlag.b10ms)
			{
				ret = App_SendData(msg->u16SrcAddr,msg->u16DesAddr,msg->u16DataLen,msg->pDataBlock->pBlockMem);
				if (ret == E_SEND_END)
				{
					m_i16_XmitState = E_XMIT_END;
					objMemory.Dat_PutMemBlock(msg->pDataBlock);
				}
			}
			break;

		case E_XMIT_END:
			m_u16_HaveMsgXmit = FALSE;
			m_i16_DataXmitActive = FALSE;
			m_i16_XmitState = E_XMIT_IDLE;
			break;
			
		default: break;
	}
}

/***************************************************************************
函数名:App_RecvProc
功能描述:数据接收处理
输入参数:
返回值:none
说明:
作者:
日期:
****************************************************************************/
void  Class_CanMstMonLongFrame::App_RecvProc(void)
{
	int16_t i;
	uint32_t u32Valuetype;
	uint16_t DesAddr;
	uint16_t frameIndex;
	int16_t err;
	_DataBlockDef	*pMemBlock = 0;

	_LongFrameDef	tempData;
	_LongFrameDef	*pTempData2 = 0;
	uint8_t data[8];
	tempData.pDataPtr = data;
	
	
	if (0 == Dat_FrameRecv(&tempData))
	{
		return;
	}

	DesAddr = tempData.u16DesAddr;
	
	switch (tempData.u16ProtNO)
	{
		case PROTNO_TPCM://回复数据帧
			if(tempData.pDataPtr[0] == 0x10)
			{
				u32Valuetype = tempData.pDataPtr[5];
				u32Valuetype += (((uint32_t)tempData.pDataPtr[6])<<8);
				u32Valuetype += (((uint32_t)tempData.pDataPtr[7])<<16);
				tempData.u32Valuetype = u32Valuetype;
				tempData.u16DataLen = S2U16(tempData.pDataPtr[1]);
				tempData.u16Frame_Len = tempData.pDataPtr[3];
				tempData.u16XmitEnd = 0;

				if (DesAddr >= 181 && DesAddr <= 230)
				{
					pTempData2 = &(m_u8_aPDURecvBuff[DesAddr - 181]);
				}
				else if (DesAddr >=231 &&  DesAddr <= 240)
				{
					pTempData2 = &(m_u8_aCCURecvBuff[DesAddr - 231]);
				}

				if (pTempData2 != 0)
				{
					if (pTempData2->pDataBlock == 0)
					{
						objMemory.Dat_GetMemBlock(&pMemBlock,tempData.u16DataLen,&err);
					}
					else
					{
					        pMemBlock = pTempData2->pDataBlock;
					}
					if (pMemBlock != 0)
					{
						*pTempData2 = tempData;
						pTempData2->pDataBlock = pMemBlock;
						//注意源地址和目的地址需要交换，
						App_SendReqResp(tempData.u16DesAddr,tempData.u16SrcAddr,
									  tempData.u32Valuetype,tempData.u16Frame_Len);
					}
				}
			}
			else if (tempData.pDataPtr[0] == 0x11)
			{
				m_i16_DataXmitActive = TRUE;
			}
			else if(tempData.pDataPtr[0] == 0x13)
			{
				
			}
			break;
			
		case PROTNO_REQ:

			break;
			
		case PROTNO_TPDT:

			if (DesAddr >= 181 && DesAddr <= 230)
			{
				pTempData2 = &(m_u8_aPDURecvBuff[DesAddr - 181]);
			}
			else if (DesAddr >=231 &&  DesAddr <= 240)
			{
				pTempData2 = &(m_u8_aCCURecvBuff[DesAddr - 231]);
			}

			if (pTempData2 != 0)
			{
				frameIndex = tempData.pDataPtr[0];
					
				if ((frameIndex <= pTempData2->u16Frame_Len)
					&& (frameIndex >= 1))
				{
					for (i = 0; i < 7; i++)
					{
						pTempData2->pDataBlock->pBlockMem[(frameIndex-1)*7+i] = tempData.pDataPtr[i+1];
					}
				}

				if (frameIndex >= pTempData2->u16Frame_Len)
				{
					//注意源地址和目的地址需要交换，
					//数据接收完成，发送响应结果0x13,
					App_SendEndResp(pTempData2->u16DesAddr,	pTempData2->u16SrcAddr,
										pTempData2->u32Valuetype,pTempData2->u16DataLen);
					//对数据进行解析
					Dat_RecvMsgDispatch(pTempData2);

					//解析完成后，释放接收长帧占用的内存块
					objMemory.Dat_PutMemBlock(pTempData2->pDataBlock);
					pTempData2->pDataBlock = 0;
				}
			}
			break;
		
		default:
			
			break;
	}
}

/***************************************************************************
函数名:App_SendReq
功能描述:长帧发送请求
输入参数:
返回值:none
说明:
作者:
日期:
****************************************************************************/
void Class_CanMstMonLongFrame::App_SendReq(uint16_t u16SrcAddr,uint16_t u16DesAddr,
														uint32_t u32VaultType, uint16_t u16Datalen)
{
	_CanMstMonAppMsgDef tempFrame;
	uint8_t temp[8];
	tempFrame.pDataPtr = temp;

	tempFrame.u16PGN = ((uint16_t)PROTNO_TPCM)<<8;
    tempFrame.u16Priority = 6;
    tempFrame.u16DataLen = 8;
	tempFrame.u16DestId = u16DesAddr;
	tempFrame.u16MACId = u16SrcAddr;

	tempFrame.pDataPtr[0] = 0x10;
	tempFrame.pDataPtr[1] = u16Datalen&0xff;
	tempFrame.pDataPtr[2] = (u16Datalen>>8)&0xff;
	tempFrame.pDataPtr[3] = (u16Datalen-1)/7+1;
	tempFrame.pDataPtr[4] = 0xff;
	tempFrame.pDataPtr[5] = u32VaultType&0x000000ff;
	tempFrame.pDataPtr[6] = (u32VaultType&0x0000ff00)>>8;
	tempFrame.pDataPtr[7] = (u32VaultType&0x00ff0000)>>16;

	Dat_FrameXmit(&(tempFrame));
}



/***************************************************************************
函数名:App_SendEndResp
功能描述:长帧响应
输入参数:
返回值:none
说明:
作者:
日期:
****************************************************************************/
void Class_CanMstMonLongFrame::App_SendEndResp(uint16_t u16SrcAddr,uint16_t u16DesAddr,
														uint32_t u32VaultType,uint16_t u16Datalen)
{
	_CanMstMonAppMsgDef tempFrame;
	uint8_t temp[8];
	tempFrame.pDataPtr = temp;
	tempFrame.u16PGN = ((uint16_t)PROTNO_TPCM)<<8;
    tempFrame.u16Priority = 6;
    tempFrame.u16DataLen = 8;
	tempFrame.u16DestId = u16DesAddr;
	tempFrame.u16MACId = u16SrcAddr;

	tempFrame.pDataPtr[0] = 0x13;
	tempFrame.pDataPtr[1] = u16Datalen&0xff;
	tempFrame.pDataPtr[2] = (u16Datalen>>8)&0xff;
	tempFrame.pDataPtr[3] = (u16Datalen-1)/7+1;
	tempFrame.pDataPtr[4] = 0xff;
	tempFrame.pDataPtr[5] = u32VaultType&0x000000ff;
	tempFrame.pDataPtr[6] = (u32VaultType&0x0000ff00)>>8;
	tempFrame.pDataPtr[7] = (u32VaultType&0x00ff0000)>>16;

	Dat_FrameXmit(&(tempFrame));
}

/***************************************************************************
函数名:App_SendReqResp
功能描述:长帧响应
输入参数:
返回值:none
说明:
作者:
日期:
****************************************************************************/
void Class_CanMstMonLongFrame::App_SendReqResp(uint16_t u16SrcAddr,uint16_t u16DesAddr,
														uint32_t u32VaultType,uint16_t u16Framelen)
{
	_CanMstMonAppMsgDef tempFrame;
	uint8_t temp[8];
	tempFrame.pDataPtr = temp;
	tempFrame.u16PGN = ((uint16_t)PROTNO_TPCM)<<8;
    tempFrame.u16Priority = 6;
    tempFrame.u16DataLen = 8;
	tempFrame.u16DestId = u16DesAddr;
	tempFrame.u16MACId = u16SrcAddr;

	tempFrame.pDataPtr[0] = 0x11;
	tempFrame.pDataPtr[1] = u16Framelen;
	tempFrame.pDataPtr[2] = 1;
	tempFrame.pDataPtr[3] = 0xff;
	tempFrame.pDataPtr[4] = 0xff;
	tempFrame.pDataPtr[5] = u32VaultType&0x000000ff;
	tempFrame.pDataPtr[6] = (u32VaultType&0x0000ff00)>>8;
	tempFrame.pDataPtr[7] = (u32VaultType&0x00ff0000)>>16;

	Dat_FrameXmit(&(tempFrame));
}


/***************************************************************************
函数名:App_SendData
功能描述:数据发送
输入参数:
返回值:none
说明:
作者:
日期:
****************************************************************************/
int16_t Class_CanMstMonLongFrame::App_SendData(uint16_t u16SrcAddr,uint16_t u16DesAddr,
																		uint16_t u16Datalen,uint8_t* pData)
{
	int16_t ret = E_SEND;

	_CanMstMonAppMsgDef tempdata;
	int i = 0;
	uint8_t temp[8];
	uint8_t *TempSendBuff = pData;
	tempdata.pDataPtr = temp;
	static int16_t s_i16FrameIndex = 1;
	static int16_t s_i16DataCnt = 0;

	tempdata.u16PGN = ((uint16_t)PROTNO_TPDT)<<8;
    tempdata.u16Priority = 6;
    tempdata.u16DataLen = 8;
	tempdata.u16DestId = u16DesAddr;
	tempdata.u16MACId = u16SrcAddr;
	tempdata.pDataPtr[0] = s_i16FrameIndex;

	for(i = 0; i < 7; i++)
	{
		if (s_i16DataCnt < u16Datalen)
		{
			tempdata.pDataPtr[i+1] = TempSendBuff[s_i16DataCnt];
		}
		else
		{
			tempdata.pDataPtr[i+1] = 0xff;
		}
		s_i16DataCnt++;
	}
	s_i16FrameIndex++;

	Dat_FrameXmit(&tempdata);
	
	if (s_i16DataCnt >= u16Datalen)
	{
		s_i16DataCnt = 0;
		s_i16FrameIndex = 1;
		ret = E_SEND_END;
	}

	return ret;
}

void Class_CanMstMonLongFrame::Dat_FrameXmit(_CanMstMonAppMsgDef *msg)
{
    int16_t i;  
    _CanMstMonTxProtocolDef tempFrame;

	tempFrame.PackedMsg.ExtId.bDataPage = 0;
	tempFrame.PackedMsg.ExtId.bR = 0;
	tempFrame.PackedMsg.ExtId.bPriority = msg->u16Priority;
	tempFrame.PackedMsg.ExtId.bRev = 0;
	
	tempFrame.PackedMsg.ExtId.bSourceAddr = msg->u16MACId;//86
	tempFrame.PackedMsg.ExtId.bPF = ((msg->u16PGN & 0xFF00) >> 8);
	tempFrame.PackedMsg.ExtId.bPS = msg->u16DestId;//0xF4; BMS的源地址 244
	
	tempFrame.PackedMsg.IDE = CAN_ID_EXT;
	tempFrame.PackedMsg.RTR = CAN_RTR_DATA;
	tempFrame.PackedMsg.DLC = 8;

	for (i = 0; i < msg->u16DataLen; i++)
	{
		tempFrame.PackedMsg.MsgData[i] = msg->pDataPtr[i];
	}

	objCanMasterMonPrtcl.XmitFramePost(&(tempFrame.Frame));
}

int16_t Class_CanMstMonLongFrame::Dat_FrameRecv(OUT _LongFrameDef *pApdu)
{
	int16_t ret = 0;
	//CanRxMsg tempPacked;
	_CanMstMonRxProtocolDef tempPacked;

	//queue is not null
	while(E_QUEUE_NULL != m_CanRecvQueue.FlagQueue())
	{
		//接收队列里面已经有比较老的数据，接收后删除该数据
		tempPacked.Frame = m_CanRecvQueue.GetOldestNode();

		m_CanRecvQueue.DelQueue();

		//把接收协议层的数据转换成应用层的数据，处理后的应用层数据
		//赋值给指针m_NonFragRecvMsg.HeadInfo，最后返回给指针pApdu
//		FrameToApdu(pApdu, &tempPacked);
		Dat_FrameParseToApp(pApdu, &tempPacked);//(m_CurRecivedFrame.Frame);
		
		ret = sizeof(_CanMstMonRxProtocolDef);
		return ret;
	}

	return ret;
}


void Class_CanMstMonLongFrame::Dat_FrameParseToApp(INOUT _LongFrameDef *pApdu,IN _CanMstMonRxProtocolDef *pFrame)
{
	int16_t i;

//	if ((pFrame->PackedMsg.ExtId.bSrcAddr>=MAC_ID_CCU0)&&(pFrame->PackedMsg.ExtId.bSrcAddr<=MAC_ID_CCU8))//单帧
	{
		pApdu->u16ProtNO = pFrame->PackedMsg.ExtId.bPF;
		pApdu->u16DataLen = pFrame->PackedMsg.DLC;
		pApdu->u16SrcAddr = pFrame->PackedMsg.ExtId.bSourceAddr;
		pApdu->u16DesAddr = pFrame->PackedMsg.ExtId.bPS;
					
		for (i = 0; i < pApdu->u16DataLen; i++)
		{
			pApdu->pDataPtr[i] = pFrame->Frame.Data[i];
		}
	}
	// 接收数据包超时，结束不到数据结束升级
	// 数据偏移，
}

/***************************************************************************
函数名:Dat_RecvMsgDispatch
功能描述:
输入参数:
返回值:none
说明:
作者:
日期:
****************************************************************************/
void Class_CanMstMonLongFrame::Dat_RecvMsgDispatch(_LongFrameDef *pLFMsg)
{
	switch (pLFMsg->u32Valuetype)
	{
		case PGN_MSTMON_ACT_PROT_PARAM:
		//	objEMParameterSet.App_Set_ACTPort_Parameter(pLFMsg);
			break;
			
		default:
			break;
	}
}


/***************************************************************************
函数名:Dat_QueueInit
功能描述:
作者:
日期:
****************************************************************************/
void  Class_CanMstMonLongFrame::Dat_QueueInit(void)
{
    m_u16_size = QUEUE_SIZE;
    m_u16_front = 0;//头指针
    m_u16_rear = 0;//尾指针
    m_u16_counter = 0;//队内消息数
    memset(&m_queue,0,sizeof(_LongFrameDef)* QUEUE_SIZE);
}

/***************************************************************************
函数名:InsQueue
功能描述:
作者:
日期:
****************************************************************************/
int16_t   Class_CanMstMonLongFrame::Dat_InsQueue(_LongFrameDef message)
{
	 if (m_u16_front == (m_u16_rear + 1)%m_u16_size)
     {
        return (E_QUEUE_FULL);
     }
     else
     {
        m_queue[m_u16_rear] = message;//写入消息
        m_u16_rear = (m_u16_rear + 1)%m_u16_size;//防止队列尾指针越界
        return(E_QUEUE_NORMAL);
     }
}

/***************************************************************************
函数名:DelQueue
功能描述 :获取队列消息
作者:
日期:
****************************************************************************/
int16_t Class_CanMstMonLongFrame::Dat_DelQueue(void)
{
	//_LongFrameDef SendBuff;

    if (m_u16_front == m_u16_rear)
    {
		return  NULL;
	}
	else
	{
        //SendBuff = m_queue[m_u16_front];
		m_u16_front = (m_u16_front + 1)%m_u16_size;//防止队列头指针越界

		return 1;
	}
}
/***************************************************************************
函数名:ClearQueue
功能描述 :清除队列
作者:
日期:
****************************************************************************/
void Class_CanMstMonLongFrame::Dat_ClearQueue(void)
{
    m_u16_front = 0;//头指针
    m_u16_rear = 0;//尾指针
    m_u16_counter = 0;//队内消息数
}

/***************************************************************************
函数名:FlagQueue
功能描述 :检测循环队列的状态，
-1:队列满
0:队列空
n:normal,表示还有几个空闲位
作者:
日期:
****************************************************************************/
int16_t   Class_CanMstMonLongFrame::Dat_GetFlagQueue(void)
{
    if (m_u16_front == (m_u16_rear + 1)%m_u16_size)
    {
        return(E_QUEUE_FULL);
    }
	if (m_u16_front == m_u16_rear)
    {
		return  (E_QUEUE_NULL);
	}

    return (E_QUEUE_NORMAL);
}

/***************************************************************************
函数名:GetOldestNode
功能描述 :读取最老的节点内容
作者:
日期:
****************************************************************************/
_LongFrameDef Class_CanMstMonLongFrame::Dat_GetOldestNode(void)
{
	return m_queue[m_u16_front];
}



