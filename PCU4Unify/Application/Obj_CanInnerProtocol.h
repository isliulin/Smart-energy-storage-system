
//SysCanProtocol.h
#ifndef _CAN_INTER_PROTOCOL_H
#define _CAN_INTER_PROTOCOL_H

#include "Publics.h"

#define FRAG_MSG    1
#define NONFRAG_MSG 0


enum INNER_CAN_MAC_ID
{
	MAC_ID_HVDC0 = 0x00,
	MAC_ID_HVDC1,
	MAC_ID_HVDC2,
	MAC_ID_HVDC3,
	MAC_ID_HVDC4,
	MAC_ID_HVDC5,
	MAC_ID_HVDC6,
	MAC_ID_HVDC7,
	MAC_ID_HVDC8,	
	MAC_ID_HVDC9,
	MAC_ID_HVDC10,
	MAC_ID_HVDC11,
	MAC_ID_HVDC12,
	MAC_ID_HVDC63 = 0x3f,

	
	
	MAC_ID_PDU0	= 0xB5,
	MAC_ID_PDU1,
	MAC_ID_PDU2,
	MAC_ID_PDU3,
	MAC_ID_PDU4,
	MAC_ID_PDU5,
	MAC_ID_PDU6,

	MAC_ID_PDU19 = MAC_ID_PDU0 + 19,
	MAC_ID_PDU20 = MAC_ID_PDU0 + 20,
	MAC_ID_PDU21 = MAC_ID_PDU0 + 21,
	MAC_ID_PDU22 = MAC_ID_PDU0 + 22,
	MAC_ID_PDU23 = MAC_ID_PDU0 + 23,
	MAC_ID_PDU24 = MAC_ID_PDU0 + 24,
	MAC_ID_PDU25 = MAC_ID_PDU0 + 25,
	MAC_ID_PDU31 = 0xD4,
	MAC_ID_PDU_UNVALID	= 0xD5,

	MAC_ID_BOX0 = 0x6F,
	MAC_ID_BOX1,
	MAC_ID_BOX2,
	MAC_ID_BOX3,
	MAC_ID_BOX4,
	MAC_ID_BOX5,
	MAC_ID_BOX6,
	MAC_ID_BOX7,
	MAC_ID_BOX8,
	MAC_ID_BOX9,
	MAC_ID_BOX10,
	MAC_ID_BOX11,
	MAC_ID_BOX12,
	MAC_ID_BOX63 = 0x78,
	

	MAC_ID_CCU0	= 0xF0,
	MAC_ID_CCU1,
	MAC_ID_CCU2,
	MAC_ID_CCU3,
	MAC_ID_CCU4,
	MAC_ID_CCU5,
	MAC_ID_CCU6,
	MAC_ID_CCU7,
	MAC_ID_CCU8,
};


	typedef struct
	{

		uint32_t bRes1			:1;
		uint32_t bRes2			:1;
		uint32_t bCnt			:1;
		uint32_t bSrcAddr		:8;
		uint32_t bDstAddr		:8;
		uint32_t bPTP			:1;
		uint32_t bProtNO		:9;//源地址
		uint32_t bRev			:3;
		
	}_InnerExtFrameIdDef;


	typedef union
	{
	    CanTxMsg Frame;
	    struct
	    {
	        uint32_t 			StdId;
		    _MasterMonFrameIdDef	ExtId;
			uint8_t 			IDE;
			uint8_t 			RTR;
			uint8_t 			DLC;
		    uint8_t				MsgData[8];
	    }PackedMsg;

	}_CanInnerTxProtocolDef;


	typedef union
	{
	    CanRxMsg Frame;
	    struct
	    {
			CanRxMsg Frame;
			struct
			{
				uint32_t			StdId;
				_MasterMonFrameIdDef	ExtId;
				uint8_t 			IDE;
				uint8_t 			RTR;
				uint8_t 			DLC;
				uint8_t 			MsgData[8];
				uint8_t 			FMI;
			}PackedMsg;

	    }PackedMsg;
	}_CanInnerRxProtocolDef;



class   Class_CanInnerProtocol
{
    public:
        //初始化对象过程
        void	Dat_Init(void);
        //信息接收、发送回调函数
		int16_t	CanRecvFcb(IN CanRxMsg *pMsg);
        int16_t	CanXmitFcb(void *pUnused);

        //消息接收处理
		int16_t   MsgRecvProc(_CanInnerAppMsgDef* pApdu);
        int16_t   MsgXmitProc(_CanInnerAppMsgDef* pApdu);

        void    GetLogicID(void);
        void    CanCommFail(void);
		int16_t   XmitFramePost(IN CanTxMsg* pCanPacked);
		
    private:

        //发送消息处理函数
        
        void    FrameParseToApdu(INOUT _CanInnerAppMsgDef *pApdu,IN _CanMstMonRxProtocolDef *pFrame);
        void    FrameParseToPpdu(OUT _CanInnerAppMsgDef *pFrame,IN _CanMstMonTxProtocolDef *pApdu);

    public:

		Class_CanRecvQueue m_CanRecvQueue;
		Class_CanXmitQueue m_CanXmitQueue;

        uint16_t  m_u16_CanXmitBusy;

        _CanMstMonRxProtocolDef  m_CurRecivedFrame;


        //与应用层交互数据
       // NONFRAG_MSG_T   m_NonFragRecvMsg;

        uint16_t  m_u16_LogicIdOK;
        uint16_t  m_u16_LogicIdReq;
        int16_t   m_i16_LogicIdReqDlyCnt;
        uint32_t  m_u32_SeriNumRecvFromMon;
        uint16_t  m_u16_LogicIdBuff;
        uint16_t  m_u16_FrameType;
        int16_t   m_i16_CanFailCnt;
};

#endif



