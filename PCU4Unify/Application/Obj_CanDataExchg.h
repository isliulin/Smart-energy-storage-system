
#ifndef _SYS_CAN_DATA_EXCHG_H
#define _SYS_CAN_DATA_EXCHG_H

#include "Publics.h"

//#include "Obj_StoredEnergy.h"
//#include "Obj_ChargeControler.h"

extern  uint16_t	ExchgDebug[10];
#define SET_PDU_ID	0x03	
#define GET_PDU_ID	0x04	
#define ENABLE                 1
#define DISENABLE              0

enum REQ_MSG_ST
{
    E_NON_MSG_XMIT = 0,
    E_HAVE_MSG_XMIT = 1
};

class   Class_CanDataExchg
{

    private:
		uint32_t	m_u32_PGN;
		uint8_t	m_u8_TotalPackNums;
		uint8_t 	m_u8_Mod_LongFrame_ID;
		uint8_t 	m_u8_ModInfo1_SendFlag;
		uint8_t	m_u8_ModInfo2_SendFlag;
		uint8_t	m_u8_ModInfo2_ReqFlag;
		uint8_t  	m_u8_MsgSendFlag;



	public:
		
		_ToMstMonMsgObjDef		m_st_ToMstMonMsgObj;
		_RecvMastMonMsgObjDef	m_st_RecvMastMonMsgObj;
		
  		_ToInnerMsgObjDef	m_st_ToPDUMsgObj;
		
		_ToInnerMsgObjDef	 m_st_ToInnerMsgObj;
		_RecvInnerMsgObjDef	m_st_RecvCCUMsgObj;


		_ToInnerMsgObjDef	m_st_ToBGMonitorMsgObj;


		_ToInnerMsgObjDef	m_st_ToTESTMonitorMsgObj;
		
		_DCACToMstMonMsgObjDef  m_st_DCACMonitorMsgObj;
		_BATTERYToMstMonMsgObjDef  m_st_BattrtMonistorObj;
		
	
		_TOBatteryMsg    m_ToBatteryMsg;


		_C2B_Info_Def		m_C2B_Contrl_CMD;		
		//--------------------软件示波器-----------------------------------------------------------------------------
		uint16_t	m_u16_WaveRcdCnt;
		uint16_t	m_u16_WaveRcdEnd;
		uint16_t	m_p16SoftMoniter[2];

		uint16_t	m_StartTramtData;
		uint16_t	m_u16_ArrayCnt;

		int16_t		m_i16_Timer;
		
		int16_t		m_i16_NULL;

		uint32_t	m_u32_NULL;
	

    public:
		void Dat_Init(void);


		void TransMsgToInner(_CanInnerAppMsgDef *msg,uint8_t srcAddr,uint8_t desAddr);
		void TransSetReqCmdToMastorMon(_CanMstMonAppMsgDef *msg,uint16_t desAddr);

		void TransDataToMastorMon(uint16_t srcAddr,uint16_t u16PGN,int16_t dataLen,uint8_t *pData);
		
 		void TransSetReqCmdToBMS(_CanBMSAppMsgDef *msg);


		int16_t GetToBMSMsg(OUT _CanBMSAppMsgDef *pApdu);
		int16_t GetToCCUMsg(OUT _CanInnerAppMsgDef *pApdu);
		int16_t GetToMastorMonMsg(OUT _CanMstMonAppMsgDef *pApdu);
		int16_t	GetBGMonitorMsg(OUT _CanInnerAppMsgDef *pApdu);

		int16_t RecvBMSMsgProc(_CanBMSAppMsgDef *pApdu);
		int16_t RecvMastorMonMsgProc(_CanMstMonAppMsgDef *pApdu);
	
		void 	MasterToInner(_CanMstMonAppMsgDef *pApdu);

		int16_t RecvMonsterToDCACMsg(_CanMstMonAppMsgDef *pApdu);
		int16_t RecvMonsterToStoredEnergyBoxMsg(_CanMstMonAppMsgDef *pApdu);	//储能接收集控信息
		int16_t RecvMonsterToAloneDCACMsg(_CanMstMonAppMsgDef *pApdu);


		int16_t RecvInnerEngBattMsg(_CanInnerAppMsgDef *pApdu);
		
		void App_SetCCUCanID(_CanMstMonAppMsgDef *pApdu);
		void App_GetCCUCanID(_CanMstMonAppMsgDef *pApdu);
};


#endif


