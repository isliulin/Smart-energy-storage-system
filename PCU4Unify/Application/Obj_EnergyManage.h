
/*==========================================================*/
/* Title		:	Charge Mamager.h									*/
/* Description	: 	Class_ChargerMamager definition					*/
/* Date			:	NOV.2010
// Author		: 								*/
/*==========================================================*/
#ifndef _ENERGY_MANAGER_H
#define _ENERGY_MANAGER_H

#include "Publics.h"

#define ENERGY_EV   1
#define ENERGY_STORAGE   2
#define CHG_MODE       1
#define DISCHG_MODE    2



class Class_EnergyManage
{
	public:
		uint8_t		m_u8_WorkMode;
		uint8_t		m_u8_RunStrategy;

	private:

	private:
		
		void		App_RunStrategy(void);	//运行策略
	public:
		void		Dat_Init(void);		//充电优先级
		void    	App_Run(void);
};
#endif
//============================================================
// End of file.
//============================================================


