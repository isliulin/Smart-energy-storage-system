
//SysCanProtocol.h
#ifndef _BMS_CAN_PROTOCOL_H
#define _BMS_CAN_PROTOCOL_H

#include "Publics.h"

#define FRAG_MSG    1
#define NONFRAG_MSG 0


//驱动层与协议层的格式互换
	typedef union
	{
	    uint16_t u16Word;
	    struct
	    {
	        uint16_t Low: 8;
	        uint16_t High: 8;
	    }ByteWide;
	}WORD_TO_2BYTES_T;


	typedef struct
	{
		uint32_t bSourceAddr	:8;//源地址
		uint32_t bPS			:8;//目的地址
		uint32_t bPF			:8;
		uint32_t bDataPage	:1;
		uint32_t bR			:1;
		uint32_t bPriority		:3;

		uint32_t bRev			:3;
		
	}_BMSExtFrameIdDef;


	typedef union
	{
	    CanTxMsg Frame;
	    struct
	    {
	        uint32_t 			StdId;
		    _BMSExtFrameIdDef	ExtId;
			uint8_t 			IDE;
			uint8_t 			RTR;
			uint8_t 			DLC;
		    uint8_t				MsgData[8];
	    }PackedMsg;
	}_CanBMSTxProtocolDef;


	typedef union
	{
	    CanRxMsg Frame;
	    struct
	    {
	        uint32_t 			StdId;
		    _BMSExtFrameIdDef	ExtId;
			uint8_t 			IDE;
			uint8_t 			RTR;
			uint8_t 			DLC;
		    uint8_t			 	MsgData[8];
			uint8_t 			FMI;
	    }PackedMsg;
	}_CanBMSRxProtocolDef;

	


class   Class_CanBMSProtocol
{
    public:
        //初始化对象过程
        void	Dat_Init(void);
        //信息接收、发送回调函数
		int16_t	CanRecvFcb(IN CanRxMsg *pMsg);
        int16_t	CanXmitFcb(void *pUnused);

        //消息接收处理
		int16_t   MsgRecvProc(_CanBMSAppMsgDef* pApdu);
        int16_t   MsgXmitProc(_CanBMSAppMsgDef* pApdu);

        void    GetLogicID(void);
        void    CanCommFail(void);

    private:

        //发送消息处理函数
        int16_t   XmitFramePost(IN CanTxMsg* pCanPacked);
        void    FrameParseToApdu(INOUT _CanBMSAppMsgDef *pApdu,IN _CanBMSRxProtocolDef *pFrame);
        void    FrameParseToPpdu(OUT _CanBMSAppMsgDef *pFrame,IN _CanBMSTxProtocolDef *pApdu);

    public:

		Class_CanRecvQueue m_CanRecvQueue;
		Class_CanXmitQueue m_CanXmitQueue;
		
        uint16_t  m_u16_CanXmitBusy;

        _CanBMSRxProtocolDef  m_CurRecivedFrame;

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













































