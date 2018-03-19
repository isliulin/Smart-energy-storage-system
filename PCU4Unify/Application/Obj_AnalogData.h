
#ifndef _ANALOG_DATA_H
#define _ANALOG_DATA_H

#include "Publics.h"



/*
//PDU1.0
typedef enum
{
	CHG_VOLT = 0,
	CHG_CURR,
	AIR_TEMP,
	INSULT_VOLT,
	VO1A,
	VO2A,
	VO3A,
	VO4A,
	VO5A,
	VO6A,
	VO7A,
	VO8A,
	BATT_VOLT,
}_AnalogTypeDef;*/


//PDU2.0
//VIN1~VIN10
//VBAT_AD VPOS-PE_AD  VOUT_AD CURR_AD  HeatTEMP1_AD ENTEMP_AD

typedef enum
{
	CHG_VFAUSE_AD0 = 0,
	CHG_VOUT_AD1 = 1,
	CHG_VBAT_AD4 = 2,
	CHG_VPOS_AD5 = 3,
	CHG_CURFAN1_AD6 = 4,
	CHG_CURR_AD7 = 5,
	PT1000A_AD10 = 6,
	PA1000B_AD11 = 7,//PE volt
	HUM_AD12 = 8,
	NTC_EN_AD13 = 9,
	CC_AD14 = 10,
}_AnalogTypeDef;






class   Class_AnalogData
{
	private:
		int16_t	   m_i16_aRealTimeValue[NUM_AD_CHANNEL];
		

	public:
		int16_t    m_i16_aK[NUM_AD_CHANNEL];//校正比例参数
		int16_t    m_i16_aOffset[NUM_AD_CHANNEL];//校正偏置
		UNLONG	   m_un_aFilterValue[NUM_AD_CHANNEL];


	private:


	public:
		void Dat_Init(void);
		void Dat_SampleProc(void);

		//数据获取，给其他模块接口
		int16_t Dat_GetFilterValue(int16_t type);
		int16_t Dat_GetRealTimeValue(int16_t type);

		int32_t Dat_FILTER(int32_t temp, uint16_t Cnst);
};

#endif


