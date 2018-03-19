
#ifndef _CAN_QUEUE_H
#define _CAN_QUEUE_H

#include "Publics.h"

#define XMIT_QUEUE_SIZE 50
#define RECV_QUEUE_SIZE 50

//------CAN 应用层帧协议定义------
//system CAN PDU frame

typedef union
{
    uint16_t u16Word;
    struct
    {
        uint16_t  Low:8;
        uint16_t  High:8;
    }ByteWide;
}WordTo2BytesDef;


enum
{
    E_QUEUE_FULL = -1,//队列满
    E_QUEUE_NORMAL = 1,//队列正常
    E_QUEUE_NULL = 0    //队列空
};


//接收队列定义
class   Class_CanRecvQueue
{
    public:
        void    CanRecvQueueInit(void);
        int16_t   InsQueue(CanRxMsg message);
        CanRxMsg    *DelQueue(void);
        int16_t   FlagQueue(void);
        CanRxMsg    GetOldestNode(void);

    //private:
        uint16_t  m_u16_size;//队列容量
        uint16_t  m_u16_front;//头指针
        uint16_t  m_u16_rear;//尾指针
        uint16_t  m_u16_counter;//队内消息数
        CanRxMsg    m_queue[RECV_QUEUE_SIZE];//存储空间首地址
};


//发送队列定义
class   Class_CanXmitQueue
{
    public:
        void    CanXmitQueueInit(void);
        int16_t   InsQueue(CanTxMsg message);
        CanTxMsg    *DelQueue(void);
        int16_t   FlagQueue(void);
        CanTxMsg    GetOldestNode(void);
        void        ClearQueue(void);

   // private:
        uint16_t  m_u16_size;//队列容量
        uint16_t  m_u16_front;//头指针
        uint16_t  m_u16_rear;//尾指针
        uint16_t  m_u16_counter;//队内消息数
        CanTxMsg    m_queue[XMIT_QUEUE_SIZE];//存储空间首地址
};


#define XMIT_QUEUE_SIZE_MODBUS	50
#define RECV_QUEUE_SIZE_MODBUS	50
#define RECV_QUEUE_SIZE_MODULEID 50

class Class_ModBusXmitQueue
{
	public:
		uint16_t m_u16_front;
		uint16_t m_u16_rear;
		uint16_t m_u16_size;
		uint16_t m_u16_counter;
		_ModBusTxDef  m_queue[XMIT_QUEUE_SIZE_MODBUS];
			
	public:


	private:
		


	public:
          	void 		ClearQueue(void);
		void 			QueueInit(void);
		_ModBusTxDef*	DelQueue(void);
		int16_t 		FlagQueue(void);
		_ModBusTxDef 	GetOldestNode(void);
		int16_t 		InsQueue(_ModBusTxDef message);
};

class Class_ModBusRecvQueue
{

		public:
			uint16_t m_u16_front;
			uint16_t m_u16_rear;
			uint16_t m_u16_size;
			uint16_t m_u16_counter;
			_ModBusRxDef  m_queue[RECV_QUEUE_SIZE_MODBUS];

		private:
			
		public:
			void 		QueueInit(void);
			
			_ModBusRxDef*	DelQueue(void);
			int16_t 		FlagQueue(void);
			_ModBusRxDef	GetOldestNode(void);
			int16_t		InsQueue(_ModBusRxDef message);

		private:
		
			void 		ClearQueue(void);
};


class Class_ModuleIDRecvQueue
{

		public:
			uint16_t m_u16_front;
			uint16_t m_u16_rear;
			uint16_t m_u16_size;
			uint16_t m_u16_counter;
			uint8_t  m_queue[RECV_QUEUE_SIZE_MODULEID];

		private:
			
		public:
			void 		QueueInit(void);
			
			uint8_t		DelQueue(void);
			int16_t 		FlagQueue(void);
			uint8_t 	GetOldestNode(void);
			int16_t		InsQueue(uint8_t moduleID);

		private:
		
			void 		ClearQueue(void);
};

#endif


