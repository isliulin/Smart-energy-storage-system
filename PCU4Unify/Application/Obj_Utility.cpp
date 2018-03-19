/*==========================================================*/
/* Title		:	Obj_Utility.cpp													*/
/* Description	: 	Utility											*/
// Author		:	wangyufei
/* Date			:	2017.12.12														*/
/*==========================================================*/

#include  "publics.h"
#include  <math.h>


//AD采样频率：1/12*（12.5+239.5）*11个通道= 231us(4.3Hz)
//每秒50Hz的次数，频率定标扩大100倍
#define         INT_CAL_NUMBER_1S      432900    //1000000*100 /(（1/12MHz) *（12.5+239.5）*11）)
uint16_t	K30HzPointCntCnst = 144;	//30Hz对应的采样点个数，(1/30)/231 * 1000000 = 144.3
uint16_t	K80HzPointCntCnst = 54;		//80Hz对应的采样点个数，(1/80)/231 * 1000000 = 54.11
uint16_t	K4p5msCntCnst = 20;			//4.5ms / 231us = 19.48
uint16_t	K1p8msCntCnst = 6;			//1.8ms / 231us = 7.79
uint16_t	K6p6msCntCnst = 29;			//6.6ms / 231us = 28.57
uint16_t	K360Cst = 65535;			//一个周期360度
uint16_t	K160msFilterCnst = 5;
uint16_t	K16msFilterCnst = 3;
/*
const FLAG_TABLE Fin50HzTable =
	{
		// (40.1~ 69.9Hz	2):  0	4000	7000	9999
		3,
		25,
		{0, 4025,	6975,	9999},
		{	0x01,	0x00,	0x02}
	};
*/
uint16_t m_i16_MainUpLimitV = 220;
uint16_t m_i16_MainDownLimitV = 165;
#define  AC80V     368                //AC有电压低判断点--  80V， (80*0.003375+1.5)*4096/3   - 2048  = 368
#define  AC60V     277                //AC无电压高判断点--60V， (60*0.003375+1.5)*4096/3   - 2048    = 277


//电压跌落判断次数13，(3.5ms/231us) 
#define ACDropCnt    		13

//频率定标系数
//50Hz---->5000 频率定标,（65535/87） * KFinCst = 5000000，87为50Hz采样点数Cnt = 20ms/231us  
#define KFinCst  6637;				

//硬件增益 G = 0.003375，AD采样基准1.5V对应市电0V，则输入最大值3V对应（3-1.5）/0.003375 = 444V，
//电压定标系数：瞬时值220V对应（220*0.003375+1.5)*4096/3 -2048 = 1126，定标220V定标4096
#define ACVKP  (4096*4096/1126)			//市电定标系数


void Class_Utility::App_DataInit(void)
{
	uint8_t i;
	for(i = 0;i < PHASE_IN;i++)
	{
		//定标系数
		m_UtilityCommon[i].m_i16_KVin_0 = ACVKP;
		//硬件增益 G = 0.003375，AD采样基准1.5V对应市电0V，掉电快检是用AD采样的绝对值判断的 
		//m_UtilityCommon[i].m_st_FastChk_0.m_i16_Lost_Chk_Point = 368;    //掉电快检判断点--  80V， (80*0.003375+1.5)*4096/3   - 2048    = 368
		m_UtilityCommon[i].m_st_FastChk_0.m_i16_Lost_Chk_Point = 650;      //掉电快检判断点--139V， （139*0.003375+1.5)*4096/3 - 2048 = 650
		//m_UtilityCommon[i].m_st_FastChk_0.m_i16_Surge_Chk_Point = 1475;  //浪涌快检判断点--288v， （288*0.003375+1.5)*4096/3 - 2048 = 1475
    }

	if(objPcsConfigManage.m_st_GetConfigPara.VoltOut == 380)	//380V
	{
		//掉电判断上下限的设定值
		m_i16_MainUpLimitV = 253;
		m_i16_MainDownLimitV = 165;
		m_i16_VinHysRange = 9;                                  //回差阈值--10v
        m_st_InputLCal_0.m_i16_VLmt4NegJudge = 46;	            //(10*0.003375+1.5)*4096/3 - 2048 = 46
	}
}
	
/************************************************************************************
函数名称：    Dat_Int_InputPreDeal()
功能描述:     输入侧模拟量预处理
函数入口:     AD出口值、定标系数
函数出口:     模拟量定标值
************************************************************************************/
void     Class_Utility::Dat_Int_InputPreDeal(void)
{
    // 输入相电压计算，定标系数格式为4.12
    m_UtilityCommon[PHASE_A].m_i16_Vin_0 =  ((INT32)AC_SampleValue.VoltageA * m_UtilityCommon[PHASE_A].m_i16_KVin_0) >> 12;
    m_UtilityCommon[PHASE_B].m_i16_Vin_0 =  ((INT32)AC_SampleValue.VoltageB * m_UtilityCommon[PHASE_B].m_i16_KVin_0) >> 12;
    m_UtilityCommon[PHASE_C].m_i16_Vin_0 =  ((INT32)AC_SampleValue.VoltageC * m_UtilityCommon[PHASE_C].m_i16_KVin_0) >> 12;
    // 输入线电压计算，定标系数格式为4.12
    for(int j=0;j<PHASE_IN;j++)
    {
        m_UtilityCommon[j].m_i16_Vinline = m_UtilityCommon[j].m_i16_Vin_0 - m_UtilityCommon[(j+1)%PHASE_IN].m_i16_Vin_0;
    }
	//周期判断
    m_i16_PointCntIn_0++;                      //中断点数计算，用于有效值计算
    if(m_i16_PointCntIn_0 > m_i16_PointIn_0)   //一个周期时间到
    {
        m_i16_PointCntIn_0 = 1;
    }
}
/************************************************************************************
函数名称：    Dat_Int_PhaseACal()
功能描述: A相相电压、线电压累加和计算
函数入口: 瞬时采样值
函数出口: 累加和
************************************************************************************/
void     Class_Utility::Dat_Int_PhaseACal(void)
{
    if(m_i16_PointCntIn_0 >= m_i16_PointIn_0)  //一个周期时间到
    {
        //A相电压周期累加的"平方和"清零 用于主循环运算的平方和赋值
        m_UtilityCommon[PHASE_A].m_un_VinRmsSumM_0 = m_UtilityCommon[PHASE_A].m_un_VinRmsSum_0;
        m_UtilityCommon[PHASE_A].m_un_VinRmsSum_0.dword = 0;
    }
    //A相电压平方和累加
    Dat_AC_Sum(&m_UtilityCommon[PHASE_A].m_un_VinRmsSum_0.dword,(INT32)m_UtilityCommon[PHASE_A].m_i16_Vin_0 * m_UtilityCommon[PHASE_A].m_i16_Vin_0,4);
    Dat_Int_VolzeroCrossCal(m_st_InputLCal_0,m_UtilityCommon[PHASE_A].m_i16_Vin_0);
}
/************************************************************************************
函数名称：    Dat_Int_PhaseBCal()
功能描述:     B相相电压、线电压累加和计算
函数入口:     瞬时采样值
函数出口:     累加和
************************************************************************************/
void     Class_Utility::Dat_Int_PhaseBCal(void)
{
    if(m_i16_PointCntIn_0 >= m_i16_PointIn_0)  //一个周期时间到
    {
        //B相电压周期累加的"平方和"清零 用于主循环运算的平方和赋值
        m_UtilityCommon[PHASE_B].m_un_VinRmsSumM_0 = m_UtilityCommon[PHASE_B].m_un_VinRmsSum_0;
        m_UtilityCommon[PHASE_B].m_un_VinRmsSum_0.dword = 0;
    }
    //B相电压平方和累加
    Dat_AC_Sum(&m_UtilityCommon[PHASE_B].m_un_VinRmsSum_0.dword,(INT32)m_UtilityCommon[PHASE_B].m_i16_Vin_0 * m_UtilityCommon[PHASE_B].m_i16_Vin_0,4);
}
/************************************************************************************
函数名称：    Dat_Int_PhaseCCal()
功能描述:     C相相电压、线电压累加和计算
函数入口:     瞬时采样值
函数出口:     累加和
************************************************************************************/
void     Class_Utility::Dat_Int_PhaseCCal(void)
{
    if(m_i16_PointCntIn_0 >= m_i16_PointIn_0)  //一个周期时间到
    {
        //C相电压周期累加的"平方和"清零 用于主循环运算的平方和赋值
        m_UtilityCommon[PHASE_C].m_un_VinRmsSumM_0 = m_UtilityCommon[PHASE_C].m_un_VinRmsSum_0;
        m_UtilityCommon[PHASE_C].m_un_VinRmsSum_0.dword = 0;
    }

    //C相电压平方和累加
    Dat_AC_Sum(&m_UtilityCommon[PHASE_C].m_un_VinRmsSum_0.dword,(INT32)m_UtilityCommon[PHASE_C].m_i16_Vin_0 * m_UtilityCommon[PHASE_C].m_i16_Vin_0,4);
}

/************************************************************************************
函数名称：    Dat_Int_VolzeroCrossCal()
功能描述:     计算过零点
函数入口:     瞬时值、数据封装结构体
函数出口: 过零点标志
************************************************************************************/
void    Class_Utility::Dat_Int_VolzeroCrossCal(volatile StructUtilityInOutputCal  &m_st_InOutputCal,INT16 m_i16_V )  //结构体变量传递首地址
{
  // 主路电压过零点及其频率计算
  m_st_InOutputCal.m_u16_TCnt++;
  m_st_InOutputCal.m_u16_HalfTCnt++;
 // m_st_InOutputCal.m_u32_TClkSmp = objTimerDrv.m_u32_ClkIntStart_0;   //在中断入口处实现
 // m_st_InOutputCal.m_i16_VOld_0 = m_i16_V;
  if(m_i16_V < 0)
  {
      m_st_InOutputCal.bPositivePolarity = 0;
      if(m_st_InOutputCal.bNegativePolarity == 1)
      {
          if(m_st_InOutputCal.m_u16_TCnt>= K30HzPointCntCnst)             //小于30 Hz
          {
//              m_st_InOutputCal.m_u32_T = 0;
              m_st_InOutputCal.m_u16_TCnt = 0;
              m_st_InOutputCal.m_u16_VCrossCnt = 0;
          }

          if(m_i16_V < (0-m_st_InOutputCal.m_i16_VLmt4NegJudge))		//小于-10V
          {
              m_st_InOutputCal.m_u16_VLowerCnt ++;		
          }
          //m_st_InOutputCal.btempNegativePolarity = 1;//负向过零
      }
      else
      {
          m_st_InOutputCal.bNegativePolarity = 1;
          //m_st_InOutputCal.btempNegativePolarity = 0;//负向过零
          m_st_InOutputCal.m_u16_HalfTCntBak = m_st_InOutputCal.m_u16_HalfTCnt;
          m_st_InOutputCal.m_u16_HalfTCnt = 0;
          if(m_st_InOutputCal.m_u16_HalfTCntBak >= (K80HzPointCntCnst/2))  // <80Hz判断为有效的周波
          {
              m_st_InOutputCal.m_u16_HalfTCntBak = 0;
          }
      }
  }
  else //正半周
  {
      m_st_InOutputCal.bNegativePolarity = 0;
      if(m_st_InOutputCal.bPositivePolarity == 1)
      {
          if(m_st_InOutputCal.m_u16_TCnt >= K30HzPointCntCnst)     //小于30Hz
          {
//              m_st_InOutputCal.m_u32_T = 0;
              m_st_InOutputCal.m_u16_TCnt = 0;
              m_st_InOutputCal.m_u16_VCrossCnt = 0;
          }
      }
      else
      {
          m_st_InOutputCal.m_u16_HalfTCntBak = m_st_InOutputCal.m_u16_HalfTCnt;
          m_st_InOutputCal.m_u16_HalfTCnt = 0;
          m_st_InOutputCal.bPositivePolarity = 1;
          if(m_st_InOutputCal.m_u16_TCnt >= K80HzPointCntCnst)                // 上一个周期小于80Hz
          {
              if(((m_st_InOutputCal.m_u16_HalfTCntBak>=K4p5msCntCnst)&&(m_st_InOutputCal.m_u16_VLowerCnt > K1p8msCntCnst))
                  ||(m_st_InOutputCal.m_u16_HalfTCntBak>=K6p6msCntCnst))
              {
                  m_st_InOutputCal.m_u16_HalfTCntBak = 0;
                  m_st_InOutputCal.m_u16_VLowerCnt = 0;
                  //m_st_InOutputCal.m_u32_TClkOld = m_st_InOutputCal.m_u32_TClk; //@TODO
                  //m_st_InOutputCal.m_u32_TClk= m_st_InOutputCal.m_u32_TClkSmp + ((INT32)m_i16_V * KIntPrdCnst) / (m_i16_V - m_st_InOutputCal.m_i16_VOld_0);
                  if(m_st_InOutputCal.m_u16_VCrossCnt >= 2)
                  {
                      //m_st_InOutputCal.m_u32_T = m_st_InOutputCal.m_u32_TClkOld- m_st_InOutputCal.m_u32_TClk;
                      m_st_InOutputCal.m_i16_DthetaInst = (INT32)K360Cst/ m_st_InOutputCal.m_u16_TCnt;
                      m_st_InOutputCal.m_un_DthetaBase_0.dword= FILTER(K160msFilterCnst, m_st_InOutputCal.m_i16_DthetaInst, m_st_InOutputCal.m_un_DthetaBase_0.dword);
                      m_un_VinThetaChange.dword = m_st_InOutputCal.m_un_DthetaBase_0.dword; 	   //计算角度增量
//                    m_st_InOutputCal.m_u16_TCntBak = m_st_InOutputCal.m_u16_TCnt;
                      m_st_InOutputCal.m_u16_TCnt= 0;
                  }
                  else
                  {
                      m_st_InOutputCal.m_u16_VCrossCnt++;
//                      m_st_InOutputCal.m_u32_T = 0;
                      m_st_InOutputCal.m_u16_TCnt = 0;
                  }
              }
          }
      }
  }
}


/************************************************************************************
函数名称：    Alg_Int_VinFastChk()
功能描述: A相电压输入快检
入口参数:     电压平方累加和、周期采样点数
出口参数:     电压瞬时有效值、滤波值、频率
************************************************************************************/

void   Class_Utility::Alg_Int_VinFastChk(void)
{
    //Full Pass Filter,produce 90 degree delay
    INT32 lTmp;
    INT16 iTmp;
    if(objPcsConfigManage.m_st_GetConfigPara.Frequency == 50)	//50Hz
   	{
    	lTmp = (INT32)(m_UtilityCommon[PHASE_A].m_i16_ValueLag90 + m_UtilityCommon[PHASE_A].m_i16_Vin_old) * 3947 - (INT32)(m_UtilityCommon[PHASE_A].m_i16_Vin_0) * 3798;
  
     }
   	else //60Hz
   	{
     	lTmp = (INT32)(m_UtilityCommon[PHASE_A].m_i16_ValueLag90 + m_UtilityCommon[PHASE_A].m_i16_Vin_old) * 3971 - (INT32)(m_UtilityCommon[PHASE_A].m_i16_Vin_0) * 3846;
   	}
     m_UtilityCommon[PHASE_A].m_i16_ValueLag90 = (INT16)(lTmp >> 12);

     //Fast Ac Drop detection.Ac dropping could be detected in nearly 3~5ms
     iTmp = ABS(m_UtilityCommon[PHASE_A].m_i16_Vin_0) + ABS(m_UtilityCommon[PHASE_A].m_i16_ValueLag90);

  if (iTmp < m_UtilityCommon[PHASE_A].m_st_FastChk_0.m_i16_Lost_Chk_Point) 	//Corresponds to 139Vac
     {
       if(m_i16_VinADropConfCnt_0 < ACDropCnt)					//13*280us = 3.64ms
       {
           m_i16_VinADropConfCnt_0++;
       }
       else
       {
           m_st_FlagVin.bVinLost_0 =1;
       }
     }
     else
     {
       if(m_i16_VinADropConfCnt_0 > 0)
       {
           m_i16_VinADropConfCnt_0--;
       }
       else
       {
           m_st_FlagVin.bVinLost_0 =0;
       }
     }

  	 m_UtilityCommon[PHASE_A].m_i16_Vin_old = m_UtilityCommon[PHASE_A].m_i16_Vin_0;
}


/************************************************************************************
函数名称：    Dat_VinRMS()
功能描述:     有效值及频率计算
入口参数:     电压平方累加和、周期采样点数
出口参数:     电压瞬时有效值、滤波值、频率
************************************************************************************/
void    Class_Utility::Dat_VinRMS(void)
{
    INT16 i16_Tmp_VRmsRange;
    m_i16_VinABCRmsMax = 0;
    //主路电压有效值计算
    for(int i=0;i<PHASE_IN;i++)
    {
        //输入相电压有效值计算
        m_UtilityCommon[i].m_i16_VinPrms = Dat_AC_Prms(m_UtilityCommon[i].m_un_VinRmsSumM_0.dword,m_i16_PointcntInM_0);
        m_UtilityCommon[i].m_un_VinRms.dword = FILTER(K16msFilterCnst, m_UtilityCommon[i].m_i16_VinPrms,m_UtilityCommon[i].m_un_VinRms.dword);

        //输入线电压有效值计算
        if(m_st_FlagVin.bSnglPhsIn == 1)   //单相输入系统不计算线电压
        {
            m_UtilityCommon[i].m_i16_VinlinePrms = 0;
            m_UtilityCommon[i].m_un_VinlineRms.dword = 0;
        }
        else
        {
            m_UtilityCommon[i].m_i16_VinlinePrms = Dat_AC_Prms(m_UtilityCommon[i].m_un_VinlineSumM_0.dword,m_i16_PointcntInM_0);
            m_UtilityCommon[i].m_un_VinlineRms.dword = FILTER(K16msFilterCnst, m_UtilityCommon[i].m_i16_VinlinePrms,m_UtilityCommon[i].m_un_VinlineRms.dword);
        }

        m_i16_VinABCRmsMax = MAX2(m_i16_VinABCRmsMax,m_UtilityCommon[i].m_un_VinRms.half.hword);

        //零偏电压计算
       // m_UtilityCommon[i].m_i16_VinZero_0 = ABS(Dat_AC_Avg(m_UtilityCommon[i].m_i32_VinSumM_0,m_i16_PointcntInM_0));
    }
}




/************************************************************************************
函数名称：    Dat_FrequencyCal()
功能描述:     频率计算
入口参数:     电压平方累加和、周期采样点数
出口参数:     电压瞬时有效值、滤波值、频率
************************************************************************************/
void    Class_Utility::Dat_FrequencyCal(void)
{
    INT32 i32_temp;
  
    //频率计算
    i32_temp = (INT32)m_un_VinThetaChange.half.lword * KFinCst;
    i32_temp = i32_temp>>16;
    i32_temp = i32_temp + (INT32)m_un_VinThetaChange.half.hword * KFinCst;
    m_un_FinMean.dword += (((INT32)i32_temp<<6) - m_un_FinMean.dword)>>10;
    m_i16_FinMean = m_un_FinMean.half.hword;
    m_un_FinMeanForCal.dword += (((INT32)i32_temp<<6) - m_un_FinMeanForCal.dword)>>6;
    i32_temp = ABS(m_un_FinMeanForCal.half.hword);
    if (i32_temp < 3000) //30Hz
    {
        i32_temp = 3000;
    }
    else if (i32_temp > 8000) //80Hz
    {
        i32_temp = 8000;
    }
    m_i16_PointcntInM_0 = (INT32)INT_CAL_NUMBER_1S / i32_temp;          //计算一个周期点数
    m_i16_PointIn_0 = m_i16_PointcntInM_0;		//周期点数

	/*
    if(m_st_FlagVin.bSnglPhsIn == 1)   //快检系数，采用A相频率--适用于单相输入系统（全通滤波器计算用）
    {
      //
    }
    else  // 每个中断电压角度增量计算   (三相输入系统计算用)
    {
        m_i16_AngleIncrease = ((INT32)m_un_FinMeanForCal.half.hword * 4295 )>> 20;
    }
    */
    /*
    //根据输入频率范围确定快检系数
    for(int i = 0; i < PHASE_IN; i++)
    {
        INT16 i16_Tmp_FinFlag = m_UtilityCommon[i].m_i16_FinSeg;
       	m_UtilityCommon[i].m_i16_FinSeg = igap (m_i16_FinMean, &Fin50HzTable, i16_Tmp_FinFlag);
        FastChkFactor_Assign(m_UtilityCommon[i].m_st_AllPassFilterCalc_0.m_i16_Factor,FastChkFactor[m_UtilityCommon[i].m_i16_FinSeg]);
    }
    */
}
/************************************************************************************
函数名称：    Dat_VinFlag()
功能描述:     输入标志位处理，综合每相电压、频率、掉电等，加工系统输入标志位
入口参数:     电压有效值、频率、掉电
出口参数:     输入标志位
************************************************************************************/
void    Class_Utility::Dat_VinFlag(void)
{
    //输入频率，电压判断
    Dat_InVoltFreqTotalFlag();
    //输入单三相判断
    Dat_NumOfPhaseJudge();
}
/*************************************************************************************************
 函数名称：    Dat_InVoltFreqTotalFlag()
功能描述:  输入电压判断
入口参数:
出口参数:
 *************************************************************************************************/
void  Class_Utility::Dat_InVoltFreqTotalFlag(void)
{
    //--------------------------------------------------------------------------------------------------------------------------------------//
    //综合三相标志位
    for(int i=0;i<PHASE_IN;i++)
    {
        //电压
        m_UtilityCommon[i].m_i16_VinSeg = ((INT32)m_UtilityCommon[i].m_un_VinRms.half.hword * 220) >> 12;

        //电压回差判断
        if((m_UtilityCommon[i].m_i16_VinSeg > (m_i16_MainUpLimitV - m_i16_VinHysRange))||(m_UtilityCommon[i].m_i16_VinSeg < (m_i16_MainDownLimitV + m_i16_VinHysRange)))
        {
            if(m_UtilityCommon[i].m_i16_VinSeg < (m_i16_MainDownLimitV + m_i16_VinHysRange))
            {
                m_st_FlagVin.bMainLowAbnormal = 1;
            }
            else
            {
                m_st_FlagVin.bMainHighAbnormal = 1;
            }
            m_i16_VinHysRange = 9;   //9V回差
        }
        else
        {
            m_i16_VinHysRange = 0;
            m_st_FlagVin.bMainLowAbnormal = 0;
            m_st_FlagVin.bMainHighAbnormal = 0;
        }
    }

    //市电掉电由异常到正常需要确认1s
    if(m_st_FlagVin.bVinLost_0 == 1)			//中断中可3.6ms判断
    {
        m_i16_MainLostCnt = 0;
        m_st_FlagVin.bMainLostValidate = 1;     //市电掉电标志置1
    }
    else
    {
        if(g_st_TimerFlag.b100ms == 1)
        {
            m_i16_MainLostCnt++;
            if(m_i16_MainLostCnt >= 10  )           //延时1S到
            {
                m_st_FlagVin.bMainLostValidate = 0;         //市电掉电标志清0
                m_i16_MainLostCnt = 10;
            }
        }
    }

/*
    //市电浪涌由异常到正常需要确认1s
    if(m_st_FlagVin.bVinSurge_0 == 1)
    {
        m_i16_MainSurgeCnt = 0;
        m_st_FlagVin.bMainSurgeValidate = 1;   //市电浪涌标志置1
    }
    else
    {
        if(g_st_TimerFlag.b100ms == 1)
        {
            m_i16_MainSurgeCnt++;
            if(m_i16_MainSurgeCnt >= 10 )           //延时1S到
            {
                m_st_FlagVin.bMainSurgeValidate = 0;            //市电浪涌标志清0
                m_i16_MainSurgeCnt = 10;
            }
        }
    }

    //频率
    UINT16 u16_Tmp_FlagFinBak = m_UtilityCommon[PHASE_A].m_st_FlagVinPhase.bFin;
    m_UtilityCommon[PHASE_A].m_st_FlagVinPhase.bFin = igap (m_i16_FinMean, &Fin50HzTable, u16_Tmp_FlagFinBak);
    m_u16_FlagFinTotal = m_UtilityCommon[PHASE_A].m_st_FlagVinPhase.bFin;
*/

    //主路正常标志位维护
    if((m_st_FlagVin.bMainLostValidate == 1)
         || (m_u16_FlagFinTotal != 0)           					//市电已掉或频率异常
         || (m_st_FlagVin.bMainHighAbnormal ==1)
         ||(m_st_FlagVin.bMainLowAbnormal == 1))    				//电压异常
    {
        m_i16_MainOKDelayCnt = 0;
        m_st_FlagVin.bMainOk = 0;                                   //主路电压异常
    }
    else
    {
        if(g_st_TimerFlag.b100ms == 1)
        {
            m_i16_MainOKDelayCnt++;
            if(m_i16_MainOKDelayCnt >= 50)                  		//延时5S到
            {
                m_st_FlagVin.bMainOk = 1;                           //置主路电压正常
                m_i16_MainOKDelayCnt = 50;
            }
        }
    }
}
/*************************************************************************************************
 函数名称：    Dat_NumOfPhaseJudge()
功能描述:     三相单相判别处理函数(主路和旁路适用)
入口参数:
出口参数:
 *************************************************************************************************/
void    Class_Utility::Dat_NumOfPhaseJudge(void)
{
    UINT16 Temp_Result[3] ;

    if(m_st_FlagVin.bPhsJudgOver == 1)
    {
        return;    //已经判断过直接返回
    }

    if((m_UtilityCommon[PHASE_A].m_un_VinRms.half.hword > AC80V)
       ||(m_UtilityCommon[PHASE_B].m_un_VinRms.half.hword > AC80V)
       ||(m_UtilityCommon[PHASE_C].m_un_VinRms.half.hword > AC80V))
    {
        m_i16_VinPhsCheckStart = 1;   //输入有电压
    }
    else if((m_UtilityCommon[PHASE_A].m_un_VinRms.half.hword < AC60V)
            &&(m_UtilityCommon[PHASE_B].m_un_VinRms.half.hword < AC60V)
            &&(m_UtilityCommon[PHASE_C].m_un_VinRms.half.hword < AC60V))
    {
        m_i16_VinPhsCheckStart = 0;
        m_i16_VinSinglPhsConfCnt = 0;  //计数器清零
        m_i16_VinThrPhsConfCnt = 0;
    }

    if(1 == m_i16_VinPhsCheckStart)
    {
        for(INT16 i=0;i<PHASE_IN;i++)
        {
            if(ABS(m_UtilityCommon[i].m_i16_Vinline) < AC60V)
            {
                Temp_Result[i] = 1;
            }
            else
            {
                Temp_Result[i] = 0;
            }
        }

        if((Temp_Result[0] == 1)&&(Temp_Result[1] == 1)&&(Temp_Result[2] == 1))
        {
            m_i16_VinSinglPhsConfCnt++;

            if ( m_i16_VinSinglPhsConfCnt >= 500)
            {
                m_st_FlagVin.bPhsJudgOver = 1;
                m_st_FlagVin.bSnglPhsIn = 1; // 单相输入
            }
        }
        else
        {
            m_i16_VinThrPhsConfCnt++;

            if(m_i16_VinThrPhsConfCnt >= 500)
            {
                m_st_FlagVin.bPhsJudgOver = 1;                          // 三相输入
                m_st_FlagVin.bSnglPhsIn = 0;
            }
        }
    }
}


/************************************************************************************
函数名称：    Dat_AC_Sum()
功能描述:     累加和计算
函数入口:     瞬时采样值或瞬时采样值均方根
函数出口:     采样周期累加和
************************************************************************************/
void  Class_Utility::Dat_AC_Sum(INT32* m_i32_Sum, INT32 m_i32_Value, INT16 m_i16_Bit_Shift)
{
  *m_i32_Sum += ((INT32)m_i32_Value >> m_i16_Bit_Shift);

}
/************************************************************************************
函数名称：    Dat_AC_Avg()
功能描述:     累加和
函数入口:     瞬时采样值或瞬时采样值累加和、周期采样点数
函数出口:     采样周期累加和
************************************************************************************/
INT16  Class_Utility::Dat_AC_Avg(INT32 m_i32_Sum, INT16 m_i16_Pointcnt)
{
  INT16 i16_Tmp_SumOfAvg;

  i16_Tmp_SumOfAvg = m_i32_Sum / m_i16_Pointcnt;
  return (i16_Tmp_SumOfAvg);
}
/************************************************************************************
函数名称：    Dat_AC_Prms()
功能描述:     累加和
函数入口:     瞬时采样值或瞬时采样值均方根、周期采样点数
函数出口:     采样周期累加和
************************************************************************************/
INT16  Class_Utility::Dat_AC_Prms(INT32 m_i32_Sum, INT16 m_i16_Pointcnt)
{
  INT32 i32_Tmp_SumOfSqr;
  INT16 i16_Tmp_Sum;
  i32_Tmp_SumOfSqr = m_i32_Sum / m_i16_Pointcnt;
  i16_Tmp_Sum=(isqrt(i32_Tmp_SumOfSqr))<<2;				//采样值平方和>>4,开方后<<2
  return i16_Tmp_Sum;//(isqrt(i32_Tmp_SumOfSqr));
}




// isqrt()
INT16  Class_Utility::isqrt(long ax)
{
  UNLONG temp;
  UINT16 temp1;
  INT16 xn1,xn;

  if (ax<=0) return(0);
  if (ax<=3) return(1);
  temp.dword=ax;
  temp1=0;

  while(temp.half.hword<0x4000)
  {
      temp1++;
      temp.dword=temp.dword<<1;
  }

  temp1=(temp1>>1)+1;
  temp.dword=ax;

  while (temp1>0)
  {
      temp.dword=temp.dword<<1;
      temp1--;
  }

  xn=temp.dword>>16;
  xn1=(xn+ax/xn)>>1;

  while (ABS(xn1-xn)>1)
  {
      xn=xn1;
      xn1=(xn+ax/xn)>>1;
  }

   return (xn1);

}



void Class_Utility::App_UtilityProcess()
{
	Dat_VinFlag();			//输入单、三相判断，各种标志加工
	Dat_VinRMS();			//有效值计算
	Dat_FrequencyCal();		//频率计算
}


//===========================================================================
// End of file.
//============================================================


