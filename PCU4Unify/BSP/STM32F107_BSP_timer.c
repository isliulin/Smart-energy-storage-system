

#include "STM32F107_BSP_timer.h"

uint16_t	g_u16_Timer50usCnt;

_TimerFlagDef	g_st_TimerFlag;



//私有函数
static void NVIC_TimerConfig(void);
static void Timer4Config(void);
static void Timer5Config(void);
static void TIM1_PWM_Init(void);
static void GPIO_Config(void);
/*
* 函数名：BSP_TimerInit
* 描述	：初始化
* 输入	：void
* 输出	: void
*/


void BSP_TimerInit(void)
{
	NVIC_TimerConfig();
	Timer4Config();
	Timer5Config();
	//TIM1_PWM_Init();
}

/**-------------------------------------------------------
  * @函数名 NVIC_TIMConfiguration
  * @功能   配置TIMER中断向量参数函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/


void NVIC_TimerConfig(void)
{ 
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	// Enable the TIM5 gloabal Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
    // Enable the TIM5 gloabal Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
	
}

/**-------------------------------------------------------
  * @函数名 NVIC_TIM4Config
  * @功能   配置TIM4参数函数，每秒计数器中断1次 
  * @参数   无
  * @返回值 无
  定时周期:((7200*10000)/72M) = 1s
***------------------------------------------------------*/
void Timer4Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    // TIM4 clock enable 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);


    // 自动重装载寄存器周期的值(计数值)
    TIM_TimeBaseStructure.TIM_Period = (10000 - 1);

	// 累计 TIM_Period个频率后产生一个更新或者中断 
    // 这个就是预分频系数
    TIM_TimeBaseStructure.TIM_Prescaler = (7200 - 1);
	
    // 采样分频 
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    // 向上计数模式 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    // 初始化定时器4
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    // 清除溢出中断标志
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

    // 溢出中断使能
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

    // 计数器使能
    TIM_Cmd(TIM4, ENABLE); 
}


/**-------------------------------------------------------
  * @函数名 NVIC_TIM5Configuration
  * @功能   配置TIM5参数函数，每秒计数器中断20000次 
  * @参数   无
  * @返回值 无
  定时周期:((72*50)/72M) = 1ms
***------------------------------------------------------*/
void Timer5Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    // TIM5 clock enable 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);


    // 自动重装载寄存器周期的值(计数值)
    TIM_TimeBaseStructure.TIM_Period = (50 - 1);

	// 累计 TIM_Period个频率后产生一个更新或者中断 
    // 这个就是预分频系数
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);
	
    // 采样分频 
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    // 向上计数模式 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    // 初始化定时器5
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

    // 清除溢出中断标志
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);

    // 溢出中断使能
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

    // 计数器使能
    TIM_Cmd(TIM5, ENABLE); 
}


/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configure the used I/O ports pin
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

  /* Configure I2C2 pins: SCL and SDA */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

}

/**-------------------------------------------------------
  * @函数名 TIM3_PWM_Init
  * @功能   配置TIM3为PWM模式
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void TIM1_PWM_Init(void)
{

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
	// TIM5 clock enable 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	GPIO_Config();
	// 自动重装载寄存器周期的值(计数值)

	TIM_TimeBaseStructure.TIM_Period = (PWMMAXCNT - 1);

	// 累计 TIM_Period个频率后产生一个更新或者中断 
	// 这个就是预分频系数
	TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);

	// 采样分频 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	// 向上计数模式 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	//重复寄存器，用于自动更新PWM占空比
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	// 初始化定时器3
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);


	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;		 
	TIM_BDTRInitStructure.TIM_DeadTime = 0x90;										
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;				 
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
	TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);		


	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, ENABLE);

	/* TIM IT enable */
//	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);

	//TIM_ARRPreloadConfig(TIM3, ENABLE);

	/* TIM3 enable counter */
	TIM_Cmd(TIM1, ENABLE);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	
}

//中断时钟维护
void	Timer_Tick_Maintain(void)
{
	g_u16_Timer50usCnt++;
	if(g_u16_Timer50usCnt >= Fint)
	{
		g_u16_Timer50usCnt = 0;
	}
}

/*
void	BSP_PowerOn5sDelay(void)
{
	static int16_t s_i16Cnt;
	if(g_st_TimerFlag.bPowerOn5sEnd == 0)
	{
		if(g_st_TimerFlag.b100ms == 1)
		{
			if (s_i16Cnt++ > 50)
			{
				g_st_TimerFlag.bPowerOn5sEnd = 1;
			}
		}
	}
}
*/

//主程序时钟维护
void	BSP_Timer_FlagMaintain(void)
{
	uint16_t	u16_temp;
						
	static uint16_t	s_u16Clk50usCnt = 0;						
	static uint16_t	s_u16Clk1msCnt = 0;	
	static uint16_t	s_u16Clk4msCnt = 0;
	static uint16_t	s_u16Clk10msCnt = 0;
   	static uint16_t	s_u16Clk100msCnt = 0;    
	static uint16_t	s_u16Clk500msCnt = 0;   
 	static uint16_t	s_u16Clk1sCnt = 0;
	static uint16_t	s_u16Clk5sCnt = 0;
	static uint16_t	s_u16Clk10sCnt = 0; 
	
	g_st_TimerFlag.b50us = 0;
	g_st_TimerFlag.b1ms = 0;
	g_st_TimerFlag.b4ms = 0;
	g_st_TimerFlag.b10ms = 0;
	g_st_TimerFlag.b100ms = 0;
	g_st_TimerFlag.b500ms = 0;
	g_st_TimerFlag.b1s = 0;
	g_st_TimerFlag.b10s = 0;
	
	//-----50us-----
	u16_temp = ((uint32_t)g_u16_Timer50usCnt)/K50usCnst;

	if (u16_temp != s_u16Clk50usCnt)
	{
		s_u16Clk50usCnt = u16_temp;
		g_st_TimerFlag.b50us = 1;							//置50us定时标志
	}
	
	//-----1ms-----
	u16_temp = ((uint32_t)g_u16_Timer50usCnt)/K1msCnst;

	if (u16_temp != s_u16Clk1msCnt)	
	{
		s_u16Clk1msCnt = u16_temp;
		g_st_TimerFlag.b1ms = 1;						//置1ms定时标志
	}

	//-----4ms-----
	u16_temp = ((uint32_t)g_u16_Timer50usCnt)/K4msCnst;

	if (u16_temp != s_u16Clk4msCnt)	
	{
		s_u16Clk4msCnt = u16_temp;
		g_st_TimerFlag.b4ms = 1;						//置4ms定时标志
	}

	//-----10ms-----
	u16_temp = ((uint32_t)g_u16_Timer50usCnt)/K10msCnst;

	if (u16_temp != s_u16Clk10msCnt)	
	{
		s_u16Clk10msCnt = u16_temp;
		g_st_TimerFlag.b10ms = 1;							//置10ms定时标志
	}

	//-----100ms-----
	u16_temp = ((uint32_t)g_u16_Timer50usCnt)/K100msCnst;

	if (u16_temp != s_u16Clk100msCnt)
	{
		s_u16Clk100msCnt = u16_temp;
		g_st_TimerFlag.b100ms = 1;							//置100ms定时标志
	}

	//-----500ms-----
	u16_temp = ((uint32_t)g_u16_Timer50usCnt)/K500msCnst;

	if (u16_temp != s_u16Clk500msCnt)
	{
		s_u16Clk500msCnt = u16_temp;
		g_st_TimerFlag.b500ms = 1;							//置500ms定时标志
	}


	//-----1s-----
	u16_temp = ((uint32_t)g_u16_Timer50usCnt)/K1sCnst;

	if (u16_temp != s_u16Clk1sCnt)	
	{
		s_u16Clk1sCnt = u16_temp;
		g_st_TimerFlag.b1s = 1;							//置1s定时标志
	}

	//上电5s延迟
	if(g_st_TimerFlag.bPowerOn5sEnd == 0)
	{
		if(g_st_TimerFlag.b100ms == 1)
		{
			if (s_u16Clk5sCnt++ > 50)
			{
				g_st_TimerFlag.bPowerOn5sEnd = 1;
			}
		}
	}

	//-----10s-----
	if (g_st_TimerFlag.b1s)
	{
		s_u16Clk10sCnt++;
		if (s_u16Clk10sCnt >= 10)
		{
			g_st_TimerFlag.b10s = 1;							//置10s定时标志
			s_u16Clk10sCnt = 0;
		}
	}
}




