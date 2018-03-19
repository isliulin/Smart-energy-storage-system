/*==========================================================*/
/* Title		:	Obj_Utility.h		    */
/* Description	        : 	Utility			   */
// Author		:	wangyufei
/* Date			:	2017.12.12		   */
/*==========================================================*/


#ifndef UTILITY_H
#define UTILITY_H

#include  "publics.h"

typedef  int16_t INT16;
typedef  uint16_t UINT16;
typedef  int32_t INT32;
typedef  uint32_t UINT32;
typedef union
{
	int32_t	dword;
	struct
	{
		uint16_t lword;
		int16_t	 hword;
	}half;
}UNLONG;

/*
typedef struct
{
	int16_t VoltageA;
	int16_t VoltageB;
	int16_t VoltageC;
	int16_t CurrentA;
	int16_t CurrentB;
	int16_t CurrentC;
}_AC_SampleValue;
*/

struct  structFlagVinPhase
{
    UINT16  bVin:3;
    /*      BIT00-02    A 相电压标志
                0000正常
                0001低异常
                0010欠压
                0100高异常
    */
    UINT16  bFin:2;
    /*      BIT06-07    频率标志
                00正常
                01低异常
                10高异常
    */
    UINT16  bPeNullFault:1;
        /*      BIT06-07    缺PE标志
                    00正常
                    01PE异常
        */
    UINT16  brsvd:10;
};
struct  structFlagVin
{
    UINT16  bMainOk                 :1;         // BIT01  1:主路电压正常
    UINT16  bMainOkBak              :1;         // BIT01  1:主路电压正常备份
    UINT16  bSnglPhsIn              :1;         // BIT00  1:单相输入；0:三相输入
    UINT16  bPhsJudgOver            :1;         // BIT03  1:相位判定完成
    UINT16  bVinLost_0              :1;         //相电压掉电
    UINT16  bVinSurge_0             :1;         //相电压浪涌
    UINT16  bMainLostValidate       :1;         //相电压掉电
    UINT16  bMainLowAbnormal     :1;         //输入电压有效值低异常 ,MYL
    UINT16  bMainHighAbnormal     :1;         //输入电压有效值高异常,MYL
    UINT16  bMainSurgeValidate      :1;         //相电压浪涌
    UINT16  bUtilityResume          :1;         //主路恢复延时到
    UINT16  bUtilityResumeDly       :1;         //主路恢复延迟
    UINT16  bFrqAdaptOver           :1;         //市电频率自适应结束
};

typedef struct
{
     INT16 factor[4];
}FASTCHK_FACTOR_TAB;


//过零点计算底层封装数据结构
struct StructUtilityInOutputCal
{
    UINT16 bPositivePolarity        :1;  //正半周标志
//    UINT16 btempPositivePolarity    :1;  //正半周标志,用于判断 旁路过零
    //UINT16 btempNegativePolarity    :1;  //负半周标志，用于判断负向过零
    UINT16 bNegativePolarity        :1;  //负半周标志
//    UINT16 RESV                     :12; //

    INT16 m_i16_VLmt4NegJudge;
    INT16  m_i16_DthetaInst;             //相角瞬时步长
//    INT16  m_i16_VOld_0;                 // 电压上一次的值
    UINT16 m_u16_TCnt;                   // 计数器
    UINT16 m_u16_HalfTCnt;
    //UINT16 m_u16_TCntBak;                // 瞬时周期点数
    UINT16 m_u16_HalfTCntBak;
    UINT16 m_u16_VLowerCnt;             //低于负半周下限 计数值
    UINT16 m_u16_VCrossCnt;              //电压正向过零次数
//    UINT32 m_u32_TClkSmp;                //采样时刻值
//    UINT32 m_u32_T;                      //电压周期
//    UINT32 m_u32_TClk      ;             // 电压正向过零时刻
//    UINT32 m_u32_TClkOld   ;             // 上周期电压正向过零时刻
    UNLONG m_un_DthetaBase_0;        // 主路相角基本步长
};

struct StructCrestCal               //峰值比计算底层封装结构体
{
    UINT16 m_u16_VinCrest;          //峰值比
    INT16 m_i16_VinCrestAckCnt;     //峰值比确认计数器
};

struct StructAllPassFilterCalc      //全通滤波器底层封装结构体
{
    INT16  m_i16_Value;             //本次瞬时值采样
    INT16  m_i16_Value_Old;         //前次瞬时值采样
    INT16  m_i16_ValueLag90;        //滞后90°瞬时值
    INT16  m_i16_ValueLag130;       //滞后130°瞬时值

    FASTCHK_FACTOR_TAB  m_i16_Factor;                   //全通滤波器系数
};

struct StructFastChk             //快检计算底层封装结构体
{
    INT16 m_i16_Lost_Chk_Point;          //掉电快检判断点
    INT16 m_i16_Surge_Chk_Point;         //浪涌快检判断点
    INT16 m_i16_Lost_FastChkConfCnt;     //掉电快检确认计数器
    INT16 m_i16_Surge_FastChkConfCnt;    //浪涌快检确认计数器
    INT16 m_i16_Lost_Chk_Result;         //掉电快检结果
    INT16 m_i16_Surge_Chk_Result;        //浪涌快检结果
};

#define  PHASE_IN 3
#define  PHASE_A  0
#define  PHASE_B  1
#define  PHASE_C  2


#define UpDownLimit16(Value,UpValue,DownValue)  {Value = (Value >= UpValue) ? UpValue : Value;  Value = (Value <= DownValue) ? DownValue : Value;}
#define UpDownLimit32(Value,UpValue,DownValue)  {Value = (Value >= UpValue) ? UpValue : Value;  Value = (Value <= DownValue) ? DownValue : Value;}

#define DownLimit16(Value,DownValue){Value = (Value <= DownValue) ? DownValue : Value;}
#define UpLimit16(Value,UpValue){Value = (Value >= UpValue) ? UpValue : Value;}

UINT16 VerifyDeal(UINT16 cond, INT16 *iVal, UINT16 uLimit,UINT16 uOldVal);

//#define   abs(x)  ((x >= 0) ? (x) : (-(x)))
#define s2u(x)  (*((UINT16*)(&x)))                              //struct->uint
#define SignInt(input)  (input > 0)
#define MAX2(x,y)   (((x)>=(y))?(x):(y))
#define MIN2(x,y)   (((x)<=(y))?(x):(y))
//#define     SIGN(x)                 (((x)<=0)?0:1)
#define     UPDNLMT16(Var,Max,Min)  {(Var)=((Var)>=(Max))?(Max):(Var);(Var)=((Var)<=(Min))?(Min):(Var);}
#define     UPDNLMT32(Var,Max,Min)  {(Var)=((Var)>=(Max))?(Max):(Var);(Var)=((Var)<=(Min))?(Min):(Var);}
//#define     FILTER(ik,iVar,lMean)   (((((INT32)(iVar)<<16)-(lMean))>>(ik))+(lMean))






/*三相新增的Public数据结构*/
//变换输出结构体
struct T_ThreePhaseTransform
{
    INT16   i16_Alpha;
    INT16   i16_Beta;

    INT16   i16_Mode;        //模值
    INT16   i16_ModeFilter;  //模值滤波值
    INT16   i16_SinQ;        //相角正弦值
    INT16   i16_CosQ;        //相角余弦值

    INT16   i16_D;
    INT16   i16_Q;
    INT16   i16_Z;
};
//三相电压/电流结构体
struct PhaABC
{
    INT16   i16_A;
    INT16   i16_B;
    INT16   i16_C;
};

//三相单相判别结构体
struct StructPhaseIdentify
{
    UINT16  bPhsJudgOver        :1;     //三单相判断结束标志位
    UINT16  bJudgeResult        :1;     //三单相判断结果标志位
    UINT16  bPhsLostBAK         :1;     //掉电状态备份

    INT16   Single_PhaseIdentifyCnt;
    INT16   Three_PhaseIdentifyCnt;

    INT16   m_i16PhsOfABC[3];           //三相输入电压 0~A 1~B 2~C
};




//============================================================
//
//============================================================
struct stUtility_Common
{
        volatile struct  structFlagVinPhase         m_st_FlagVinPhase;         //相电压标志位
        volatile struct  StructCrestCal             m_st_CrestCal_0;           //峰值比计算用
        struct  StructAllPassFilterCalc             m_st_AllPassFilterCalc_0;  //全通滤波器计算用
        volatile struct  StructFastChk              m_st_FastChk_0;            //快检计算用

        INT16       m_i16_KVin_0;                           // 相电压定标系数
        INT16       m_i16_Vin_0;                            // 相电压采样值
        INT16		m_i16_Vin_old;                          // 相电压上一个采样值
        INT16       m_i16_ValueLag90;                       //滞后90°瞬时值
        INT16       m_i16_VinZero_0;                        //主路电压零偏,用于检测市电缺零
        INT16       m_i16_FinSeg;                           //输入频率段
        INT16       m_i16_VinSeg;                         //输入电压段


        /*输入线电压计算Common[0]~AB [1]~BC [2]~CA*/

        INT16       m_i16_VinPrms;              //主路相电压即时有效值

        INT16       m_i16_Vinline;              //三相线电压分别对应
        INT16       m_i16_VinlinePrms;          //线电压瞬时有效值

        UNLONG      m_un_VinlineRms;            //线电压电压有效值(含滤波尾数)
        UNLONG      m_un_VinRms;                //主路相电压有效值(含滤波尾数)

        UNLONG      m_un_VinRmsSum_0;           //主路相电压平方累加和,中断累加用

        UNLONG      m_un_VinRmsSumM_0;          //主路相电压平方周期和,主程序求有效值用
        UNLONG      m_un_VinlineSum_0;          //主路线电压平方累加和,中断累加用
        UNLONG      m_un_VinlineSumM_0;         //主路线电压平方周期和,主程序求有效值用
};

class Class_Utility
{
    public:
        structFlagVin   m_st_FlagVin;                       //输入标志位
        StructUtilityInOutputCal   m_st_InputLCal_0;        //相电压过零点等计算用
        stUtility_Common m_UtilityCommon[PHASE_IN];         //公共数据池
        UNLONG      m_un_DthetaInLMean_0;                   //主路相角平均步长
        INT16       m_i16_VinTheta;                         //输入电压矢量角瞬时值[0,4000H]=[0,360度]
        UINT16      m_u16_FlagFinTotal;
        INT16       m_i16_MainLostCnt;                      //主路掉电确认计数器
        INT16       m_i16_MainSurgeCnt;                     //主路浪涌确认计数器
        INT16       m_i16_PointCntIn_0;                     // 主路电压累加点数,中断点数计数器
        INT16       m_i16_PointIn_0;                        //输入电压的点数,在一个工频周期内进中断的次数
        INT16       m_i16_PointcntInM_0;                    //主路电压周期点数,主程序求有效值用,在一个工频周期内进中断的次数

        INT16       m_i16_VinABCRmsMax;                     //主路输入电压有效值最大值
        INT16       m_i16_VinABCRmsMaxGap;                  //主路输入电压有效值最大值区间
        INT16       m_i16_AngleIncrease;                    // 每个中断角度增量
        INT16       m_i16_FinMean;                          //输入频率

    private:
        UNLONG      m_un_VinThetaChange;        //输入角度变化量

        UNLONG      m_un_FinMean;               //输入频率  50Hz－>5000
        UNLONG      m_un_FinMeanForCal;         //输入频率,用于计算，与unFinMean滤波时间常数不同

        INT16       m_i16_VinThetaOld;          //输入电压矢量角瞬时值[0,4000H]=[0,360度] 上一拍的值
        INT16       m_i16_VinHysRange;                      //电压会差范围


        INT16       m_i16_MainOKDelayCnt;            // 主路恢复开启延时计数器

        INT16       m_i16_VinPhsCheckStart;
        INT16       m_i16_VinSinglPhsConfCnt;      //三单输入判定确认计数器
        INT16       m_i16_VinThrPhsConfCnt;
		INT16       m_i16_VinADropConfCnt_0;       //A相掉电判断计数器

    private:
        void        Dat_InVoltFreqTotalFlag(void);
        void        Dat_NumOfPhaseJudge(void);
		INT16 		CalSin(INT16 Angle);
		INT16 		CalSqrt(long ax);
		INT16		isqrt(long ax);
		//void  FastChkFactor_Assign( FASTCHK_FACTOR_TAB &m_st_Factor,const FASTCHK_FACTOR_TAB  &factor_table ) {m_st_Factor = factor_table;}

		//交流量计算函数
		void    		Dat_Int_VolzeroCrossCal(volatile StructUtilityInOutputCal   &m_st_InOutputCal, INT16 m_i16_V);  //过零点计算
		inline void     Dat_AC_Sum(INT32* m_i32_Sum, INT32 m_i16_Value,INT16 m_i16_Bit_Shift);   //周期累加和计算
		inline INT16    Dat_AC_Avg(INT32 m_i32_Sum, INT16 m_i16_Pointcnt);
		inline INT16    Dat_AC_Prms(INT32 m_i32_Sum, INT16 m_i16_Pointcnt);                     //瞬时有效值计算
		inline INT16    Dat_AC_P(INT32 m_i32_Sum, INT16 m_i16_Pointcnt,INT16 m_i16_Bit_Shift);  //有功计算
		inline INT16    Dat_AC_S(INT16 m_i16_Vrms, INT16 m_i16_Irms,INT16 m_i16_Bit_Shift);     //视在计算
		void    		Dat_AC_Crest_Calc(INT16 m_i16_Value, INT16 m_i16_Prms,volatile StructCrestCal &m_st_CrestCal);   //峰值比计算
		void    		Dat_AC_Crest_Adjust(volatile StructCrestCal &m_st_CrestCal);                                    //峰值比调整
		
    public:
        void        Dat_Int_InputPreDeal(void);         //采样数据处理，周期判断
        void        Dat_Int_PhaseACal(void);            //A相相电压、线电压累加和计算
        void        Dat_Int_PhaseBCal(void);            //B相相电压、线电压累加和计算
        void        Dat_Int_PhaseCCal(void);            //C相相电压、线电压累加和计算
        void 	    Alg_Int_VinFastChk(void);           //输入快检
        void        Dat_VinRMS(void);                   //输入电压有效值计算
        void        Dat_FrequencyCal(void);             //频率计算
        void        Dat_VinFlag(void);                  //输入标志

        void	    App_DataInit(void);
        void 	    App_UtilityProcess(void);
};
#endif
//============================================================
// End of file.
//============================================================


