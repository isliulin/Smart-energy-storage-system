/******************** (C) COPYRIGHT 2013 www.armjishu.com  ********************
 * 文件名  ：adc.c
 * 描述    ：
 * 实验平台：PDUD1
 * 标准库  ：STM32F10x_StdPeriph_Driver V3.5.0
 * 作者    ：
*******************************************************************************/

#include "STM32F107_BSP_adc.h"
//#include "STM32F107_BSP_dma.h"

//#define NUM_AD_CHANNEL  11//ADC1的通道数

int16_t 	AD_DATA[11];	//VIN1~VIN10 VBAT_AD VPOS-PE_AD  VOUT_AD CURR_AD  HeatTEMP1_AD ENTEMP_AD

_AC_SampleValue		AC_SampleValue;

//------软件定义-----------------------
static void 	ADC_GPIO_Config(void);
static void 	ADC_Config(void);


void	BSP_ADC_Init(void)
{
	ADC_GPIO_Config();
	ADC_Config();
}

/*===========================================================================*
 *	函数名称：ADC_GPIO_Configuration
 *	功能描述：ADC模数转换的GPIO配置初始化函数
 *	用法说明：
 *	输入参数：
 *	输出参数：
 *	设计人员：
 *	创建日期：
 *==========================================================================*/

void ADC_GPIO_Config(void)
{
	//PA0~PC7------ADC00~ADC07
	//PB0~PB1------ADC08~ADC09
	//PC0~PC5------ADC10~ADC15

	GPIO_InitTypeDef GPIO_InitStructure;

	//使能端口1的时钟和ADC1的时钟，因为ADC1的通道1在PA1上
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1|RCC_APB2Periph_GPIOC,ENABLE );

	//PAx 作为模拟通道输入引脚						   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 	//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Pcx 作为模拟通道输入引脚						   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 	//模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);
        


}


/*===========================================================================*
 *	函数名称：void ADC_Config(void)
 *	功能描述：ADC模数转换配置函数
 *	用法说明：
 *	输入参数：
 *	输出参数：
 *	设计人员：
 *	创建日期：
 *==========================================================================*/

void ADC_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure; 

  	//设置ADC分频因子6 72M/6=12M,ADC最大时间不能超过14M，也就是ADC的时钟频率为12MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

  	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

  	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//本次实验使用的是ADC1，并ADC1工作在独立模式ADC_CR1的位19:16,即这几位为0000
  	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//ADC_ScanConvMode 用来设置是否开启扫描模式，本实验开启扫面模式.ADC_CR1的位8
  	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//ADC_ContinuousConvMode 用来设置是否开启连续转换模式 模数转换工作在连续转换模式，ADC_CR2的位1
  	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动 ADC_CR2的位19:17
  	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐ADC_CR2的位11
  	ADC_InitStructure.ADC_NbrOfChannel = NUM_AD_CHANNEL;	//顺序进行规则转换的ADC通道的数目ADC_SQR1位23:20
  	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

	//VIN1~VIN10
	//Vfuse_AD,Vout_AD,Vbat_AD,V_pos_AD,FAN_FB1_AD,CURR_AD,PT1000A_AD,PT1000B_AD,HUM_AD,PT1000_EN_AD,CC_AD
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5 );      
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 3, ADC_SampleTime_239Cycles5 );
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 4, ADC_SampleTime_239Cycles5 );
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 5, ADC_SampleTime_239Cycles5 );
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 6, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10,7, ADC_SampleTime_239Cycles5 );
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_11,8, ADC_SampleTime_239Cycles5 );
 	ADC_RegularChannelConfig(ADC1, ADC_Channel_12,9, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 10, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 11, ADC_SampleTime_239Cycles5 );
	
  	/*ADC_RegularChannelConfig(ADC1, ADC_Channel_3,12, ADC_SampleTime_239Cycles5 );
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 13, ADC_SampleTime_239Cycles5 );
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_9,14, ADC_SampleTime_239Cycles5 );
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 15, ADC_SampleTime_239Cycles5 );
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 16, ADC_SampleTime_239Cycles5 );*/
  	
        
  	//使能ADC1的DMA传输，ADC_CR2位8
  	ADC_DMACmd(ADC1, ENABLE);
	
  	//使能的ADC1,ADC_CR2位0
  	ADC_Cmd(ADC1, ENABLE);	

  	ADC_ResetCalibration(ADC1);	//使能复位校准，ADC_CR2位3  
  	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
  	ADC_StartCalibration(ADC1);	 //开启AD校准，ADC_CR2位2
  	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束

   	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//软件启动AD转换

	////DMA1通道1；外设为ADC1；存储器为AD_DATA；通道数为N个.
	MYDMA_Config(DMA1_Channel1,(u32)&ADC1->DR,(u32)&AD_DATA,NUM_AD_CHANNEL);
}

/**-------------------------------------------------------
  * @函数名 GetADCConvertedValue
  * @功能   获取ADC模数转换的结果
  * @参数   无
  * @返回值 ADC转换结果的变量，只有的低12位有效
***------------------------------------------------------*/
void BSP_Int_SamplingData(void)
{
	//获取市电A相电压采样值
	AC_SampleValue.VoltageA = AD_DATA[AD1_AC_VoltB]-2048;
	AC_SampleValue.VoltageB = AD_DATA[AD1_AC_VoltB]-2048;
	AC_SampleValue.VoltageC = AD_DATA[AD4_AC_VoltC]-2048;
}


