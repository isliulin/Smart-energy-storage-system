
#include "Publics.h"
#include <math.h>

//AD的4055----750V
//充电电流 3277 ----160A

typedef struct
{
	int16_t	i16Type;//数据类型
	int16_t	i16ScalFactor;//定标参数
	int16_t	i16FiltCnst;//滤波常量
}_AnalogDataTypeDef;



//pdu2.0
const _AnalogDataTypeDef aAnlgParamTbl[NUM_AD_CHANNEL] =
{
	{CHG_VFAUSE_AD0,8889,	8},//快熔两端电压
	{CHG_VOUT_AD1,	8889,	8},//输出端电压
	{CHG_VBAT_AD4,	8889,	8},//电池电压，输出值0~7500,对应0~750V
	{CHG_VPOS_AD5,	10000,	8},//绝缘检测正极电压
	{CHG_CURFAN1_AD6, 0,	10},//风扇电压
	{CHG_CURR_AD7, 2000,	8},//输出电流，输出值0~1900,对应0~190A
	{PT1000A_AD10,	1587,	8},//温度采样1
	{PA1000B_AD11,	1587,	8},//温度采样2
	{HUM_AD12, 4096,	8},//湿度检测
	{NTC_EN_AD13,	1587,	8},//环境温度
	{CC_AD14, 120,	2},//CC1检测
        
};

/*
//pdu1.0
const _AnalogDataTypeDef aAnlgParamTbl[NUM_AD_CHANNEL] =
{
	{CHG_VOLT,	8889,	5},//电池电压，输出值0~7500,对应0~750V
	{CHG_CURR,	2000,	5},//输出电流，输出值0~1900,对应0~190A
	{AIR_TEMP,	4096,	5},//温度
	{INSULT_VOLT,8889,	5},//电池电压，输出值0~7500,对应0~750V
	{4,			4096,	5},
	{5,			4096,	5},
	{6,			4096,	5},
	{7,			4096,	5},
	{8,			4096,	5},
	{9,			4096,	5},
	{10,		4096,	5},
	{11,		4096,	5},
	{BATT_VOLT,	8889,	5},//电池电压，输出值0~7500,对应0~750V

};*/

/***************************************************************************
函数名:Dat_Init
功能描述:
函数调用
作者:
日期:
****************************************************************************/
void Class_AnalogData::Dat_Init(void)
{
	int16_t i;
	//注意，这里m_i16_aK[] 和 m_un_aFilterValue[]需要存储到EEPROM中
	for (i = 0;i < NUM_AD_CHANNEL;i++)
	{
		m_i16_aK[i] = 1024;
		m_i16_aOffset[i] = 0;

		m_un_aFilterValue[i].dword = 0;
                if(i==6||i==7||i==9)
                {
                  m_i16_aOffset[i] = -467;
                }
	}

	m_i16_aK[0] = 218;
	m_i16_aOffset[0] = 25;
	m_i16_aK[1] = 218;
	m_i16_aOffset[1] = 2048;		//1.5V基准
	m_i16_aK[2] = 218;
	m_i16_aOffset[0] = 25;
	m_i16_aK[3] = 218;
	m_i16_aOffset[2] = 25;
	m_i16_aK[5] = 0;
	m_i16_aOffset[5] = 0;
}

/***************************************************************************
函数名:Dat_SampleProc
功能描述:
函数调用
作者:
日期:
****************************************************************************/
void Class_AnalogData::Dat_SampleProc(void)
{
	int16_t i;
	int32_t temp;
 //      static int16_t FanCnt=0;

	for (i = 0;i < NUM_AD_CHANNEL;i++)
	{

	#if 0
		//定标
		temp = (((int32_t)(aAnlgParamTbl[i].i16ScalFactor)) * AD_DATA[i])>>12;
		
		//校正
		temp = (((int32_t)m_i16_aK[i]) * temp)>>10;//比例增益校正
		temp += m_i16_aOffset[i];//零点偏置校正

		//----------------------电池采样通道有一个偏置------	        
		if(i==CHG_VBAT_AD4)
		{
			temp = (((int32_t)(aAnlgParamTbl[i].i16ScalFactor)) * (AD_DATA[i]-372))>>12;

			//校正
			temp = (((int32_t)m_i16_aK[i]) * temp)>>10;//比例增益校正
			temp += m_i16_aOffset[i];//零点偏置校正

		}
		//------------------------------------------------------------------

		//瞬时值
		m_i16_aRealTimeValue[i] = temp;

		//滤波值
		m_un_aFilterValue[i].dword = FILTER(aAnlgParamTbl[i].i16FiltCnst, temp, m_un_aFilterValue[i].dword);
	#endif
		temp += m_i16_aOffset[i];					 //零点偏置校正
		temp = (((int32_t)m_i16_aK[i]) * AD_DATA[i]);//比例增益校正

	
		//瞬时值
		m_i16_aRealTimeValue[i] = temp / 100;

		//滤波值
		m_un_aFilterValue[i].dword = m_i16_aRealTimeValue[i];  //Dat_FILTER(temp, 10) /100;
	}
}


/***************************************************************************
函数名:Dat_GetFilterValue
功能描述:
函数调用
作者:
日期:
****************************************************************************/
int16_t Class_AnalogData::Dat_GetFilterValue(int16_t type)
{
	return (m_un_aFilterValue[type].half.lword);
}

/***************************************************************************
函数名:Dat_GetRealTimeValue
功能描述:
函数调用
作者:
日期:
****************************************************************************/
int16_t Class_AnalogData::Dat_GetRealTimeValue(int16_t type)
{
	return (m_i16_aRealTimeValue[type]);
}

/***************************************************************************
函数名:Dat_FILTER
功能描述:
函数调用
作者:
日期:
****************************************************************************/
int32_t Class_AnalogData::Dat_FILTER(int32_t temp, uint16_t Cnst)
{
	static uint16_t i;
	int32_t retData;
	uint32_t u32Sum;
	static int32_t DataBuf[10] = {0};

	
	//for(i = 0; i < (Cnst - 1); i++)
	{
		DataBuf[i] = DataBuf[i + 1];	
	}

	DataBuf[Cnst] = temp;

	u32Sum = 0;
	for(i = 0; i < Cnst; i++)
	{
		u32Sum += DataBuf[i];
	}
	
	retData = u32Sum / Cnst;
	
	return retData;
}


