/*==========================================================*/
/* Title		:	ObjChargSys.cpp													*/
/* Description	: 	ObjChargSys realization 												*/
/* Date		:	2015.7.29													*/
/*==========================================================*/
#ifndef  	SYSTEMRTC_H
#define		SYSTEMRTC_H
#include "Publics.h"



class Class_SystemRTC
{


	private:

                void    App_SysClock();
	public:

		_CALENDAR       s_SysCalendar; 
                
		void	        Dat_Init(void);	
		void	        App_SysConfigProc(void);
		_CALENDAR  		APP_DateCarryProcessing(void);
		_CALENDAR       APP_TimeDisToPoint(uint16_t TimeDis);
		void 			APP_WriteSystemRTCTime(_CALENDAR m_st_Calendar);
		_CALENDAR		APP_PreChargeOverTime(uint16_t TimeDis,_CALENDAR m_st_Calendar);


};
#endif