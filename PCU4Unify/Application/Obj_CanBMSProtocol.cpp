
//SysCanProtocol.cpp

#include "Publics.h"
#include "Obj_CanQueue.h"


/***************************************************************************
函数名:SysCanProtocolIni
功能描述:
作者:
日期:
****************************************************************************/
void Class_CanBMSProtocol::Dat_Init(void)
{
	//队列初始化
	m_CanRecvQueue.CanRecvQueueInit();
	m_CanXmitQueue.CanXmitQueueInit();

	m_u32_SeriNumRecvFromMon = 0xffffffff;
	m_u16_LogicIdBuff = 0xffff;
	m_u16_FrameType = 0xff;//初始化成0xff,获取到有效值为1或者0

	m_u16_CanXmitBusy = 0;
}

/***************************************************************************
函数名:SysCanXmitFcb
功能描述:协议层消息发送函数
输入参数:none
返回值:none
说明:将发送队列的消息写入CAN发送寄存器，由中断程序调用
作者:
日期:
****************************************************************************/
int16_t    Class_CanBMSProtocol::CanXmitFcb(void *pUnused)
{
	CanTxMsg tempPacked;

	//队列不为空
	if (E_QUEUE_NULL != m_CanXmitQueue.FlagQueue())
	{
		tempPacked = m_CanXmitQueue.GetOldestNode();
		m_CanXmitQueue.DelQueue();
		
		//set sending flag
		m_u16_CanXmitBusy = TRUE;
		//发送数据
		BSP_Can2_TranMsg(&tempPacked);

		BSP_CAN2_OpenInt();
	}
	else
	{
		BSP_CAN2_CloseInt();
		
		//clear sending flag
		m_u16_CanXmitBusy = FALSE;
	}
	return 0;
}

/***************************************************************************
函数名:SysCanRecvFcb
功能描述:协议层消息发送函数
输入参数:none
返回值:none
说明:将发送队列的消息写入CAN发送寄存器，由中断程序调用
作者:
日期:
****************************************************************************/
int16_t    Class_CanBMSProtocol::CanRecvFcb(IN CanRxMsg *pMsg)
{
	m_CurRecivedFrame.Frame = *pMsg;

	//if (MAC_ID_CHG_DISCHGER == m_CurRecivedFrame.PackedMsg.ExtId.bPS)
	//{
		m_CanRecvQueue.InsQueue(m_CurRecivedFrame.Frame);
	//}

	return 0;
}

/***************************************************************************
函数名：Class_CanBMSProtocol::MsgXmitProc
功能描述：将接收队列里的协议层消息帧转化成应用层
输入参数：_CanBMSAppMsgDef *pApdu,应用层消息
返回值：是否收到BMS消息帧
使用说明：
****************************************************************************/

int16_t    Class_CanBMSProtocol::MsgRecvProc(OUT _CanBMSAppMsgDef *pApdu)
{
	int16_t ret = 0;
	//CanRxMsg tempPacked;
	_CanBMSRxProtocolDef tempPacked;

	//queue is not null
	while(E_QUEUE_NULL != m_CanRecvQueue.FlagQueue())
	{
		//接收队列里面已经有比较老的数据，接收后删除该数据
		tempPacked.Frame = m_CanRecvQueue.GetOldestNode();
		
		m_CanRecvQueue.DelQueue();

		//把接收协议层的数据转换成应用层的数据，处理后的应用层数据
		//赋值给指针m_NonFragRecvMsg.HeadInfo，最后返回给指针pApdu
		FrameParseToApdu(pApdu, &tempPacked);

		ret = sizeof(_CanBMSAppMsgDef);
		return ret;
	}
	
	return ret;
}


/***************************************************************************
函数名：Class_CanBMSProtocol::MsgXmitProc
功能描述：将应用层消息帧转化成协议层
输入参数：应用层消息
返回值：协议层发送队列的状态
使用说明：
****************************************************************************/


int16_t    Class_CanBMSProtocol::MsgXmitProc(_CanBMSAppMsgDef *pApdu)
{
	int16_t ret = 0;
	//CanTxMsg tempFrame;
	_CanBMSTxProtocolDef tempFrame;
	
	FrameParseToPpdu(pApdu,&tempFrame);

	ret = XmitFramePost(&(tempFrame.Frame));

	return ret;
}

	
/***************************************************************************
函数名：Class_CanBMSProtocol::FrameParseToApdu
功能描述：驱动协议层到应用层消息映射
输入参数：协议层消息、应用层消息
返回值：无
使用说明：
****************************************************************************/

void    Class_CanBMSProtocol::FrameParseToApdu(INOUT _CanBMSAppMsgDef *pApdu,IN _CanBMSRxProtocolDef *pFrame)
{
 	int16_t i;
	
	//注意，如果说这块需要接收连续的多帧数据，则需要进行进一步解析组合数据

	pApdu->u16MACId = pFrame->PackedMsg.ExtId.bSourceAddr; //目的地址接收
	pApdu->u16PGN = ((uint16_t)(pFrame->PackedMsg.ExtId.bPF))<<8;

	pApdu->u16DataLen = pFrame->PackedMsg.DLC;
	for (i = 0; i < pApdu->u16DataLen; i++)
	{
		pApdu->pDataPtr[i] = pFrame->PackedMsg.MsgData[i];
	}	
}

/***************************************************************************
函数名：Class_CanBMSProtocol::FrameParseToPpdu
功能描述：发送消息从应用层到协议驱动层的转换
输入参数：协议层消息、应用层消息
返回值：无
使用说明：
****************************************************************************/

void    Class_CanBMSProtocol::FrameParseToPpdu(OUT _CanBMSAppMsgDef *pApdu,IN _CanBMSTxProtocolDef *pFrame)
{
 	int16_t i;

#if 0
	pFrame->PackedMsg.ExtId.bDataPage = 0;
	pFrame->PackedMsg.ExtId.bR = 0;
	pFrame->PackedMsg.ExtId.bRev = 0;
	pFrame->PackedMsg.ExtId.bPriority = pApdu->u16Priority;
    pFrame->PackedMsg.ExtId.bPF = ((pApdu->u16PGN & 0xFF00) >> 8);


    pFrame->PackedMsg.ExtId.bPF = 0x16;
	pFrame->PackedMsg.ExtId.bSourceAddr = MAC_ID_CHG_DISCHGER;
	pFrame->PackedMsg.ExtId.bPS = 0x01;
#endif

	pFrame->PackedMsg.ExtId.bSourceAddr = pApdu->u16MACId;
	pFrame->PackedMsg.ExtId.bPS = pApdu->u16DestId;		//发往BMS
	pFrame->PackedMsg.ExtId.bPF = (pApdu->u16PGN & 0xFF00) >> 8;		//PGN
	pFrame->PackedMsg.ExtId.bDataPage = 0;
	pFrame->PackedMsg.ExtId.bR = 0;
	pFrame->PackedMsg.ExtId.bPriority = 6;
	pFrame->PackedMsg.ExtId.bRev = 0;
	
	pFrame->PackedMsg.DLC = pApdu->u16DataLen;
	pFrame->PackedMsg.IDE = CAN_ID_EXT;
	pFrame->PackedMsg.RTR = CAN_RTR_DATA;

	for (i = 0; i < pApdu->u16DataLen; i++)
	{
		pFrame->PackedMsg.MsgData[i] = pApdu->pDataPtr[i];		//send data
	}
}


/***************************************************************************
函数名：Class_CanBMSProtocol::XmitFramePost
功能描述：将应用层消息帧转化成协议层，然后通过CAN2发送出去
输入参数：CanTxMsg* pCanPacked,协议层消息
返回值：队列满 返回 -1，  队列不满 返回 0
使用说明：存入新的待发送消息到队列，并从队列中取出老的消息，放入CAN发送寄存器；
****************************************************************************/

int16_t    Class_CanBMSProtocol::XmitFramePost(IN CanTxMsg* pCanPacked)
{
 	CanTxMsg tempPacked = *pCanPacked;
        
	BSP_CAN2_CloseInt();
	
	if (E_QUEUE_FULL == m_CanXmitQueue.FlagQueue())
	{
		BSP_CAN2_OpenInt();
		return E_QUEUE_FULL;
	}
	else
	{
		m_CanXmitQueue.InsQueue(tempPacked);
	}

	if (FALSE == m_u16_CanXmitBusy)
	{
		m_u16_CanXmitBusy = TRUE;
		
		tempPacked = m_CanXmitQueue.GetOldestNode();
		
		m_CanXmitQueue.DelQueue();
		BSP_Can2_TranMsg(&tempPacked);
	}
	BSP_CAN2_OpenInt();
	return 0;
}

//===========================================================================
// End of file.
//===========================================================================


