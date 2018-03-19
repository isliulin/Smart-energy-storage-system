/*==========================================================*/
/* Title		:	ObjSysRTC.h														*/
/* Description	: 	ObjSysRTC definition												*/
/* Date		:	2015.7.29													*/
/*==========================================================*/
#include "Publics.h"

/************************************************************************************
函数名称：	Dat_Init()
功能描述：
************************************************************************************/
void  Class_SystemRTC::Dat_Init(void)
{
	s_SysCalendar.Second = 0x00;
	s_SysCalendar.Minute = 0x39;
	s_SysCalendar.Hour = 0x14;
	s_SysCalendar.Day = 0x05;			
	s_SysCalendar.Week = 0;
	s_SysCalendar.Month = 0x02;
	s_SysCalendar.Year.word = 0x07E2;
	
}
/************************************************************************************
函数名称：	App_SysConfigProc()
功能描述：实时更新系统时钟
************************************************************************************/
void     Class_SystemRTC::App_SysConfigProc(void)
{
	App_SysClock();
}


_CALENDAR  Class_SystemRTC:: APP_DateCarryProcessing(void)
{
	uint8_t	u8_Monthday[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
	uint16_t u16_SumMinute;
	_CALENDAR OutHex_calendar;

	u16_SumMinute = s_SysCalendar.Hour*60 + s_SysCalendar.Minute;

	OutHex_calendar.Year.half.hbyte= s_SysCalendar.Year.half.hbyte;
	OutHex_calendar.Year.half.lbyte= s_SysCalendar.Year.half.lbyte;
	OutHex_calendar.Month = s_SysCalendar.Month;
	OutHex_calendar.Day = s_SysCalendar.Day;
	OutHex_calendar.Second = s_SysCalendar.Second;
/*	
	if(u16_SumMinute < 24*60)			//一天以内
	{
		OutHex_calendar.Hour = u16_SumMinute/60;
		OutHex_calendar.Minute = u16_SumMinute%60;
	}
*/	
	if (u16_SumMinute >= 24*60) 						//超过一天
	{
		if(s_SysCalendar.Day == u8_Monthday[s_SysCalendar.Month])	//月底最后天
		{
			if(s_SysCalendar.Month == 12) 					//年底最后一天
			{
				if (s_SysCalendar.Year.half.lbyte < 0xFF)	
				{
					OutHex_calendar.Year.half.lbyte =s_SysCalendar.Year.half.lbyte + 1;
	                OutHex_calendar.Year.half.hbyte =s_SysCalendar.Year.half.hbyte;
				}
				else			//年进位处理（如1999年）
				{
					OutHex_calendar.Year.half.lbyte = 0x00;
					OutHex_calendar.Year.half.hbyte = s_SysCalendar.Year.half.hbyte + 1;
				}
					
				OutHex_calendar.Month = 1;
			}
			else 
			{
				OutHex_calendar.Month += 1;
			}
			OutHex_calendar.Day = 1;	
		}
		else 
		{
			OutHex_calendar.Day += 1;	
		}
		
		OutHex_calendar.Hour = (u16_SumMinute -24*60)/60;
		OutHex_calendar.Minute = (u16_SumMinute -24*60)%60;
	}
	
	return OutHex_calendar;
}


/************************************************************************************
函数名称：	App_SysClock()
功能描述：实时更新系统时钟
************************************************************************************/
void     Class_SystemRTC::App_SysClock(void)
{
	
	if(g_st_TimerFlag.b1s == 1)
	{
		s_SysCalendar.Second+= 1;

		if(s_SysCalendar.Second >= 60)
		{
			s_SysCalendar.Second = 0;
			s_SysCalendar.Minute++;
		}
					
		if(s_SysCalendar.Minute >= 60)
		{
			s_SysCalendar.Minute = 0;
			s_SysCalendar.Hour++;
		}
		
		if(s_SysCalendar.Hour >= 24)
		{
	//		s_SysCalendar.Hour = 0;
	//		s_SysCalendar.Day++;
			APP_DateCarryProcessing();
		}

	}
}
/************************************************************************************
函数名称：	WriteSystemRTCTime()
功能描述：更新系统时钟,与集控时间保持同步
************************************************************************************/
void Class_SystemRTC:: APP_WriteSystemRTCTime(_CALENDAR m_st_Calendar)
{
  /*
	s_SysCalendar.Year.half.hbyte = BCDToHex(objCanDataPool.m_st_RecvMastMon.m_st_TimeSync_D.SyncCalendar.Year.half.hbyte);
	s_SysCalendar.Year.half.lbyte = BCDToHex(objCanDataPool.m_st_RecvMastMon.m_st_TimeSync_D.SyncCalendar.Year.half.lbyte);
	s_SysCalendar.Month =BCDToHex(objCanDataPool.m_st_RecvMastMon.m_st_TimeSync_D.SyncCalendar.Month);
	s_SysCalendar.Day = BCDToHex(objCanDataPool.m_st_RecvMastMon.m_st_TimeSync_D.SyncCalendar.Day);
	s_SysCalendar.Hour = BCDToHex(objCanDataPool.m_st_RecvMastMon.m_st_TimeSync_D.SyncCalendar.Hour);
	s_SysCalendar.Minute = BCDToHex(objCanDataPool.m_st_RecvMastMon.m_st_TimeSync_D.SyncCalendar.Minute);
	s_SysCalendar.Second = BCDToHex(objCanDataPool.m_st_RecvMastMon.m_st_TimeSync_D.SyncCalendar.Second);
  */
	
}

_CALENDAR  Class_SystemRTC:: APP_PreChargeOverTime(uint16_t TimeDis,_CALENDAR m_st_Calendar)
{


	uint8_t	u8_Monthday[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
	uint16_t u16_SumMinute;
	_CALENDAR OutHex_calendar;
	_CALENDAR OutBCD_calendar;
	u16_SumMinute = m_st_Calendar.Hour*60 + m_st_Calendar.Minute + TimeDis;

	
	OutHex_calendar.Year.half.hbyte= m_st_Calendar.Year.half.hbyte;
	OutHex_calendar.Year.half.lbyte= m_st_Calendar.Year.half.lbyte;
	OutHex_calendar.Month = m_st_Calendar.Month;
	OutHex_calendar.Day = m_st_Calendar.Day;
	OutHex_calendar.Second = m_st_Calendar.Second;
	
	if(u16_SumMinute < 24*60)			//一天以内
	{
		OutHex_calendar.Hour = u16_SumMinute/60;
		OutHex_calendar.Minute = u16_SumMinute%60;
	}
	else 						//超过一天
	{
		if(m_st_Calendar.Day == u8_Monthday[m_st_Calendar.Month])	//月底最后天
		{
			if(m_st_Calendar.Month == 12) 					//年底最后一天
			{
				if (m_st_Calendar.Year.half.lbyte < 0xFF)	
				{
					OutHex_calendar.Year.half.lbyte =m_st_Calendar.Year.half.lbyte + 1;
	                OutHex_calendar.Year.half.hbyte =m_st_Calendar.Year.half.hbyte;
				}
				else			//年进位处理（如1999年）
				{
					OutHex_calendar.Year.half.lbyte = 0x00;
					OutHex_calendar.Year.half.hbyte = m_st_Calendar.Year.half.hbyte + 1;
				}
					
				OutHex_calendar.Month = 1;
			}
			else 
			{
				OutHex_calendar.Month += 1;
			}
			OutHex_calendar.Day = 1;	
		}
		else 
		{
			OutHex_calendar.Day += 1;	
		}
		
		OutHex_calendar.Hour = (u16_SumMinute -24*60)/60;
		OutHex_calendar.Minute = (u16_SumMinute -24*60)%60;
	}
	OutBCD_calendar.Year.half.hbyte = HexToBCD(OutHex_calendar.Year.half.hbyte);     
	OutBCD_calendar.Year.half.lbyte = HexToBCD(OutHex_calendar.Year.half.lbyte);     
	OutBCD_calendar.Month= HexToBCD(OutHex_calendar.Month);
	OutBCD_calendar.Day= HexToBCD(OutHex_calendar.Day);
	OutBCD_calendar.Hour= HexToBCD(OutHex_calendar.Hour);
	OutBCD_calendar.Minute= HexToBCD(OutHex_calendar.Minute);
	OutBCD_calendar.Second = HexToBCD(OutHex_calendar.Second);
	return OutBCD_calendar;
}

