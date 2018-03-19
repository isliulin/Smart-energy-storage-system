
#ifndef _MST_LONGFRAME_H
#define _MST_LONGFRAME_H

#include "Publics.h"

#define	PGN_MSTMON_ACT_PROT_PARAM	0xB000

#define	QUEUE_SIZE	50

enum
{
    E_XMIT_IDLE = 0,
    E_XMIT_REQ = 1,
    E_XMIT_DATA = 2,
    E_XMIT_END = 3,
};
enum
{
	E_SEND = 0,
	E_SEND_END = 1,
};

typedef struct
{
	uint16_t u16DataLen;
	uint32_t u32Valuetype;
	uint16_t u16Frame_Len;
	uint16_t u16DesAddr;
	uint16_t u16SrcAddr;
	uint16_t u16ProtNO;
	uint16_t u16XmitEnd;
	uint16_t u16RecvEnd;
	uint8_t* pDataPtr;
	_DataBlockDef*	pDataBlock;
}_LongFrameDef;

class Class_CanMstMonLongFrame
{
	private:
		uint16_t m_u16_front;
		uint16_t m_u16_rear;
		uint16_t m_u16_size;
		uint16_t m_u16_counter;
		_LongFrameDef  m_queue[QUEUE_SIZE];//???????
		
		uint16_t m_u16_HaveMsgXmit;
		uint16_t m_i16_XmitState;
		uint16_t m_i16_DataXmitActive;

		_LongFrameDef m_u8_aPDURecvBuff[PDU_MAX_NUM];
		_LongFrameDef m_u8_aCCURecvBuff[CCU_MAX_NUM];
		
	public:
		Class_CanRecvQueue m_CanRecvQueue;
		

	private:
        	
		void 	App_RecvProc(void);
		void	App_XmitProc(_LongFrameDef *msg);

		void 	App_SendReqResp(uint16_t u16SrcAddr,uint16_t u16DesAddr,
									uint32_t u32VaultType,uint16_t u16Framelen);

		void 	App_SendEndResp(uint16_t u16SrcAddr,uint16_t u16DesAddr,
										uint32_t u32VaultType,uint16_t u16Datalen);
		
		void 	App_SendReq(uint16_t u16SrcAddr,uint16_t u16DesAddr,
									uint32_t u32VaultType,uint16_t u16Datalen);
		
		int16_t App_SendData(uint16_t u16SrcAddr,uint16_t u16DesAddr,
										uint16_t u16Datalen,uint8_t* pData);

		void 	Dat_FrameXmit(_CanMstMonAppMsgDef *message);
		int16_t Dat_FrameRecv(OUT _LongFrameDef *pApdu);
		void 	Dat_FrameParseToApp(INOUT _LongFrameDef *pApdu,IN _CanMstMonRxProtocolDef *pFrame);
		void 	Dat_RecvMsgDispatch(_LongFrameDef *pLFMsg);

		void 	Dat_QueueInit(void);
		int16_t Dat_DelQueue(void);
    	int16_t Dat_GetFlagQueue(void);
        _LongFrameDef Dat_GetOldestNode(void);
		int16_t Dat_InsQueue(_LongFrameDef message);
		
	public:
		void 	Dat_Init(void);
		void 	App_MsgProc(void);
		int16_t App_TransData(uint16_t u16SrcAddr,uint16_t u16DesAddr,
									uint32_t u32VaultType,uint16_t u16Datalen,uint8_t *pData);
		void  	Dat_ClearQueue(void);
};

#endif


