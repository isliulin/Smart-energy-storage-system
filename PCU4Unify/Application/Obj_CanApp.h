

#ifndef _SYS_CAN_APP_H
#define _SYS_CAN_APP_H

#include "Publics.h"


enum REQ_MSG_CLASS
{
    E_CAN_ANALOG_MSG_CLASS = 1,
    E_CAN_CFG_MSG_CLASS,
	E_CAN_STATE_MSG_CLASS,
	E_CAN_CONTROL_MSG_CLASS,
	E_CAN_ALARM_MSG_CLASS,
	E_CAN_DISCRETE_MSG_CLASS,
	E_CAN_MSG_CLASS_EOL,
};


class   Class_CanApp
{
    private:

        //void	MsgRecvDispatch(IN CAN_APDY_T *pApdu);

		void	CanMsgRecvProc(void);
		void	CanMsgXmitProc(void);

		void	CovMsgProc(void);
		void	SyncMsgProc(void);

		int16_t 	RecvInnerMsgDispatch(IN _CanInnerAppMsgDef *pApdu);


    public:
        void	SysCanInit(void);
		void	MainRoutle(void);
};


#endif




