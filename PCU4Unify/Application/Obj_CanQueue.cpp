
#include "Publics.h"

/***************************************************************************
函数名:CanRecvQueueInit
功能描述:关机原因上报
作者:
日期:
****************************************************************************/
void  Class_CanRecvQueue::CanRecvQueueInit(void)
{
    m_u16_size = RECV_QUEUE_SIZE;
    m_u16_front = 0;//头指针
    m_u16_rear = 0;//尾指针
    m_u16_counter = 0;//队内消息数
}

/***************************************************************************
函数名:InsQueue
功能描述:关机原因上报
作者:
日期:
****************************************************************************/
int16_t   Class_CanRecvQueue::InsQueue(CanRxMsg message)
{
	 if (m_u16_front == (m_u16_rear + 1)%m_u16_size)
     {
        return (E_QUEUE_FULL);
     }
     else
     {
        m_queue[m_u16_rear] = message;//写入消息
        m_u16_rear = (m_u16_rear + 1)%m_u16_size;//防止队列尾指针越界
        //m_u16_counter++;
        return(E_QUEUE_NORMAL);
     }
}

/***************************************************************************
函数名:DelQueue
功能描述 :获取队列消息
作者:
日期:
****************************************************************************/
CanRxMsg* Class_CanRecvQueue::DelQueue(void)
{

   	CanRxMsg*   pMessage;

    if (m_u16_front == m_u16_rear)
    {
		return  NULL;
	}
	else
	{
		pMessage = &m_queue[m_u16_front];
		m_u16_front = (m_u16_front + 1)%m_u16_size;//防止队列头指针越界

		return(pMessage);
	}
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
int16_t   Class_CanRecvQueue::FlagQueue(void)
{
	if (m_u16_front == ((m_u16_rear + 1)%m_u16_size))
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
CanRxMsg  Class_CanRecvQueue::GetOldestNode(void)
{
    return m_queue[m_u16_front];
}


/***************************************************************************
函数名:CanXmitQueueInit
功能描述:关机原因上报
作者:
日期:
****************************************************************************/
void  Class_CanXmitQueue::CanXmitQueueInit(void)
{
    m_u16_size = XMIT_QUEUE_SIZE;
    m_u16_front = 0;//头指针
    m_u16_rear = 0;//尾指针
    m_u16_counter = 0;//队内消息数
    memset(m_queue,0,sizeof(CanTxMsg)* XMIT_QUEUE_SIZE);

}

/***************************************************************************
函数名:InsQueue
功能描述:
作者:
日期:
****************************************************************************/
int16_t   Class_CanXmitQueue::InsQueue(CanTxMsg message)
{

	 if (m_u16_front == (m_u16_rear + 1)%m_u16_size)
     {
        return (E_QUEUE_FULL);
     }
     else
     {
        m_queue[m_u16_rear] = message;//写入消息
        m_u16_rear = (m_u16_rear + 1)%m_u16_size;//防止队列尾指针越界
        //m_u16_counter++;
        return(E_QUEUE_NORMAL);
     }
}

/***************************************************************************
函数名:DelQueue
功能描述 :获取队列消息
作者:
日期:
****************************************************************************/
CanTxMsg* Class_CanXmitQueue::DelQueue(void)
{
	CanTxMsg*   pMessage;

    if (m_u16_front == m_u16_rear)
    {
		return  NULL;
	}
	else
	{
		pMessage = &m_queue[m_u16_front];
		m_u16_front = (m_u16_front + 1)%m_u16_size;//防止队列头指针越界

		return(pMessage);
	}
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
int16_t   Class_CanXmitQueue::FlagQueue(void)
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
CanTxMsg    Class_CanXmitQueue::GetOldestNode(void)
{
    return m_queue[m_u16_front];
}

/***************************************************************************
函数名:ClearQueue
功能描述 :清除队列
作者:
日期:
****************************************************************************/
void    Class_CanXmitQueue::ClearQueue(void)
{
    m_u16_front = 0;//头指针
    m_u16_rear = 0;//尾指针
    m_u16_counter = 0;//队内消息数
}





/****************************************************************************/
void  Class_ModBusXmitQueue::QueueInit(void)
{
    m_u16_size = XMIT_QUEUE_SIZE_MODBUS;
    m_u16_front = 0;//头指针
    m_u16_rear = 0;//尾指针
    m_u16_counter = 0;//队内消息数
    memset(m_queue,0,sizeof(_ModBusTxDef)* XMIT_QUEUE_SIZE_MODBUS);
}


int16_t   Class_ModBusXmitQueue::InsQueue(_ModBusTxDef message)
{

	 if (m_u16_front == (m_u16_rear + 1)%m_u16_size)
     {
        return (E_QUEUE_FULL);
     }
     else
     {
     	//if (message.u16DataStartAdd == 0x03)
     	//{
			//ClearQueue();
		//}
        m_queue[m_u16_rear] = message;//写入消息
       
        m_u16_rear = (m_u16_rear + 1)%m_u16_size;//防止队列尾指针越界
        return(E_QUEUE_NORMAL);
     }
}


_ModBusTxDef* Class_ModBusXmitQueue::DelQueue(void)
{
    _ModBusTxDef*   pMessage;
    if (m_u16_front == m_u16_rear)
    {
        return  NULL;
    }
    else
    {
        pMessage = &m_queue[m_u16_front];
        m_u16_front = (m_u16_front + 1)%m_u16_size;//防止队列头指针越界

        return(pMessage);
    }
}



int16_t   Class_ModBusXmitQueue::FlagQueue(void)
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


_ModBusTxDef Class_ModBusXmitQueue::GetOldestNode(void)
{
    return m_queue[m_u16_front];
}

/***************************************************************************
函数名:ClearQueue
功能描述 :清除队列
作者:
日期:
****************************************************************************/
void Class_ModBusXmitQueue::ClearQueue(void)
{
    m_u16_front = 0;//头指针
    m_u16_rear = 0;//尾指针
    m_u16_counter = 0;//队内消息数
}


/****************************************************************************/
void  Class_ModBusRecvQueue::QueueInit(void)
{
    m_u16_size = RECV_QUEUE_SIZE_MODBUS;
    m_u16_front = 0;//头指针
    m_u16_rear = 0;//尾指针
    m_u16_counter = 0;//队内消息数
    memset(m_queue,0,sizeof(_ModBusRxDef)* RECV_QUEUE_SIZE_MODBUS);

}

/***************************************************************************
函数名:InsQueue
功能描述:
作者:
日期:
****************************************************************************/
int16_t   Class_ModBusRecvQueue::InsQueue( _ModBusRxDef message)
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
_ModBusRxDef* Class_ModBusRecvQueue::DelQueue(void)
{
	_ModBusRxDef*   pMessage;

    if (m_u16_front == m_u16_rear)
    {
		return  NULL;
	}
	else
	{
		pMessage = &m_queue[m_u16_front];
		m_u16_front = (m_u16_front + 1)%m_u16_size;//防止队列头指针越界

		return(pMessage);
	}
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
int16_t Class_ModBusRecvQueue::FlagQueue(void)
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
_ModBusRxDef Class_ModBusRecvQueue::GetOldestNode(void)
{
	return m_queue[m_u16_front];

}

/***************************************************************************
函数名:ClearQueue
功能描述 :清除队列
作者:
日期:
****************************************************************************/
void Class_ModBusRecvQueue::ClearQueue(void)
{
    m_u16_front = 0;//头指针
    m_u16_rear = 0;//尾指针
    m_u16_counter = 0;//队内消息数
}


/****************************************************************************/
void  Class_ModuleIDRecvQueue::QueueInit(void)
{
    m_u16_size = RECV_QUEUE_SIZE_MODULEID;
    m_u16_front = 0;//头指针
    m_u16_rear = 0;//尾指针
    m_u16_counter = 0;//队内消息数
    memset(m_queue,0,sizeof(uint8_t)* RECV_QUEUE_SIZE_MODBUS);

}

/***************************************************************************
函数名:InsQueue
功能描述:
作者:
日期:
****************************************************************************/
int16_t   Class_ModuleIDRecvQueue::InsQueue(uint8_t moduleID)
{
	 if (m_u16_front == (m_u16_rear + 1)%m_u16_size)
     {
        return (E_QUEUE_FULL);
     }
     else
     {
       	m_queue[m_u16_rear] = moduleID;//写入消息
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
uint8_t Class_ModuleIDRecvQueue::DelQueue(void)
{
	uint8_t   moduleID;

    if (m_u16_front == m_u16_rear)
    {
		return  NULL;
	}
	else
	{
		moduleID = m_queue[m_u16_front];
		m_u16_front = (m_u16_front + 1)%m_u16_size;//防止队列头指针越界

		return(moduleID);
	}
}

/***************************************************************************
函数名:FlagQueue
功能描述 :检测循环队列的状态，-1:队列满0:队列空n:normal,表示还有几个空闲位
作者:
日期:
****************************************************************************/
int16_t Class_ModuleIDRecvQueue::FlagQueue(void)
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
uint8_t Class_ModuleIDRecvQueue::GetOldestNode(void)
{
	return m_queue[m_u16_front];

}

/***************************************************************************
函数名:ClearQueue
功能描述 :清除队列
作者:
日期:
****************************************************************************/
void Class_ModuleIDRecvQueue::ClearQueue(void)
{
    m_u16_front = 0;//头指针
    m_u16_rear = 0;//尾指针
    m_u16_counter = 0;//队内消息数
}

//===========================================================================
// End of file.
//===========================================================================

