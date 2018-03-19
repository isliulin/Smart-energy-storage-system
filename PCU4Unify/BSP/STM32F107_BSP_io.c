
#include "STM32F107_BSP_io.h"
#include "STM32F107_BSP_timer.h"

uint16_t	g_i16_aDIState[DIn];

//g_i8_aD0State[0] --u98
//g_i8_aD0State[1] --u101
//g_i8_aD0State[2] --u100
//g_i8_aD0State[3] --u102
//g_i8_aD0State[4] --u35
//g_i8_aD0State[5] --u36

uint8_t		g_i8_aD0State[6];
uint8_t		g_i8_aD0StateBackup[6];



//_RelayOutputStateDef	g_st_RelayState;




//-----------GPIO input port------------------------------
/**-------------------------------------------------------
  * @函数名 BSP_DI_Init
  * @功能
  * @参数
  * @返回值 无
***------------------------------------------------------*/

GPIO_TypeDef* DI_PORT[] = {PORT_DI_FANHOT_FB,	PORT_DI_EPO_FB,	PORT_DI_PROTECT_FB,	PORT_DI_ACBREAK_FB,	PORT_DI_DCBREAK1_FB,PORT_DI_DCBREAK2_FB,PORT_DI_DCBREAK3_FB,PORT_DI_DCBREAK4_FB };
const uint16_t DI_PIN[] = {PIN_DI_FANHOT_FB,	PIN_DI_EPO_FB,	PIN_DI_PROTECT_FB,	PIN_DI_ACBREAK_FB,	PIN_DI_DCBREAK1_FB,	PIN_DI_DCBREAK2_FB,	PIN_DI_DCBREAK3_FB,PIN_DI_DCBREAK4_FB };
const uint32_t DI_CLK[] = {CLK_DI_FANHOT_FB,	CLK_DI_EPO_FB,	CLK_DI_PROTECT_FB,	CLK_DI_ACBREAK_FB,	CLK_DI_DCBREAK1_FB,	CLK_DI_DCBREAK2_FB,	CLK_DI_DCBREAK3_FB,CLK_DI_DCBREAK4_FB };




/**-------------------------------------------------------
  * @函数名 BSP_IO_Dat_Init
  * @功能
  * @参数
  * @返回值 无
***------------------------------------------------------*/
void BSP_IO_Dat_Init(void)
{
	int16_t i;
	for(i=0; i<6; i++)
	{
		g_i8_aD0State[i] = 0;
		g_i8_aD0StateBackup[i] = 1;
	}
}

void  BSP_DI_Init(_DI_TypeDef DI)
{

	GPIO_InitTypeDef GPIO_InitStructure;

  	// Enable the DI Clock
 	RCC_APB2PeriphClockCmd(DI_CLK[DI] | RCC_APB2Periph_AFIO, ENABLE);

  	/* Configure Button pin as input floating */
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = DI_PIN[DI];
  	GPIO_Init(DI_PORT[DI], &GPIO_InitStructure);
}

/**-------------------------------------------------------
  * @函数名 BSP_DI_Filter
  * @功能
  * @参数
  * @返回值 无
***------------------------------------------------------*/
void BSP_DI_Filter(void)
{
	int16_t i;
	static int16_t s_ConfirmCnt[DIn];

	if (g_st_TimerFlag.b1ms)
	{
		for (i = DI_START; i < DIn; i++)
		{
			if (1 == GPIO_ReadInputDataBit(DI_PORT[i], DI_PIN[i]))
			{
				s_ConfirmCnt[i]++;
				if (s_ConfirmCnt[i] >= 3)
				{
					s_ConfirmCnt[i] = 3;
					g_i16_aDIState[i] = E_HIGH;
				}
			}
			else
			{
				s_ConfirmCnt[i]--;
				if (s_ConfirmCnt[i] <= 0)
				{
					s_ConfirmCnt[i] = 0;
					g_i16_aDIState[i] = E_LOW;
				}
			}

		}
	}
}
/**-------------------------------------------------------
  * @函数名 BSP_DI_GetState
  * @功能
  * @参数
  * @返回值 无
***------------------------------------------------------*/
int16_t BSP_DI_GetState(_DI_TypeDef DI)
{
	return (g_i16_aDIState[DI]);
}

/**-------------------------------------------------------
  * @函数名 BSP_DI_GetState
  * @功能
  * @参数
  * @返回值 无
***------------------------------------------------------*/
int16_t BSP_DI_GetRTState(_DI_TypeDef DI)
{
	return (GPIO_ReadInputDataBit(DI_PORT[DI], DI_PIN[DI]));
}


//-----------------------------Output Relay Control--------------------------------
//GPIO_TypeDef* RELAY_PORT[] = 	{DO_RES_RELAY_GPIO_PORT,	DO_BMS24V_RELAY_GPIO_PORT,	DO_INSULTNEG_RELAY_GPIO_PORT,	DO_ELOCKN_RELAY_GPIO_PORT,	DO_ELOCKP_RELAY_GPIO_PORT,	DO_FAN1_RELAY_GPIO_PORT,	DO_HEART_RELAY_GPIO_PORT,	DO_ACBREAK_RELAY_GPIO_PORT,		DO_BMSCONTACTOR_RELAY_GPIO_PORT,	DO_PARACONTACTOR_RELAY_GPIO_PORT,	DO_POSLED_RELAY_GPIO_PORT,		DO_ERRORLED_RELAY_GPIO_PORT,	DO_CHARGELED_RELAY_GPIO_PORT,	DO_INSULTPOS_RELAY_GPIO_PORT,	DO_BMS12V_RELAY_GPIO_PORT,		DO_LAMPRED_RELAY_GPIO_PORT,		DO_LAMPGREEN_RELAY_GPIO_PORT, DO_SAMP_RELAY_GPIO_PORT, DO_PARACONTACTOR_RELAY2_GPIO_PORT,DO_PULSEELOCK_RELAY_GPIO_PORT};
//const uint16_t RELAY_GPIO_PIN[] = 	{DO_RES_RELAY_PIN,			DO_BMS24V_RELAY_PIN,		DO_INSULTNEG_RELAY_PIN,			DO_ELOCKN_RELAY_PIN,		DO_ELOCKP_RELAY_PIN,			DO_FAN1_RELAY_PIN,			DO_HEART_RELAY_PIN,			DO_ACBREAK_RELAY_PIN,			DO_BMSCONTACTOR_RELAY_PIN,			DO_PARACONTACTOR_RELAY_PIN,			DO_POSLED_RELAY_PIN,			DO_ERRORLED_RELAY_PIN,			DO_CHARGELED_RELAY_PIN,			DO_INSULTPOS_RELAY_PIN,			DO_BMS12V_RELAY_PIN,			DO_LAMPRED_RELAY_PIN,			DO_LAMPGREEN_RELAY_PIN, DO_SAMP_RELAY_PIN, DO_PARACONTACTOR_RELAY2_PIN,DO_PULSEELOCK_RELAY_PIN};
//const uint32_t RELAY_GPIO_CLK[] = 	{DO_RES_RELAY_GPIO_CLK,		DO_BMS24V_RELAY_GPIO_CLK,	DO_INSULTNEG_RELAY_GPIO_CLK,	DO_ELOCKN_RELAY_GPIO_CLK,	DO_ELOCKP_RELAY_GPIO_CLK,	DO_FAN1_RELAY_GPIO_CLK,		DO_HEART_RELAY_GPIO_CLK,	DO_ACBREAK_RELAY_GPIO_CLK,		DO_BMSCONTACTOR_RELAY_GPIO_CLK,	DO_PARACONTACTOR_RELAY_GPIO_CLK,	DO_POSLED_RELAY_GPIO_CLK,		DO_ERRORLED_RELAY_GPIO_CLK, 	DO_CHARGELED_RELAY_GPIO_CLK,	DO_INSULTPOS_RELAY_GPIO_CLK,	DO_BMS12V_RELAY_GPIO_CLK,		DO_LAMPRED_RELAY_GPIO_CLK,		DO_LAMPGREEN_RELAY_GPIO_CLK,DO_SAMP_RELAY_GPIO_CLK, DO_SAMP_RELAY_GPIO_CLK,DO_PULSEELOCK_RELAY_GPIO_CLK};

GPIO_TypeDef* RELAY_PORT[] = {DO_FAN1_RELAY_GPIO_PORT,	DO_FAN2_RELAY_GPIO_PORT,	DO_IDLE_RELAY_GPIO_PORT,DO_ONLINE_RELAY_GPIO_PORT,	DO_OFFLINE_RELAY2_GPIO_PORT,DO_FAULTLED_RELAY_GPIO_PORT};
const uint16_t RELAY_GPIO_PIN[] = {DO_FAN1_RELAY_PIN,		DO_FAN2_RELAY_PIN,		DO_IDLE_RELAY_PIN,		DO_ONLINE_RELAY_PIN,		DO_OFFLINE_RELAY2_PIN,DO_FAULTLED_RELAY_PIN}; 
const uint32_t RELAY_GPIO_CLK[] = {DO_FAN1_RELAY_GPIO_CLK,	DO_FAN2_RELAY_GPIO_CLK,	DO_IDLE_RELAY_GPIO_CLK,	DO_ONLINE_RELAY_GPIO_CLK, 	DO_OFFLINE_RELAY2_GPIO_CLK,DO_FAULTLED_RELAY_GPIO_CLK};



/**-------------------------------------------------------
  * @函数名 BSP_Relay_Init
  * @功能   初始化Relay的GPIO管脚，配置为推挽输出
  * @参数
  * @返回值 无
***------------------------------------------------------*/
void BSP_Relay_Init(Relay_TypeDef rly)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* Enable the GPIO_LED Clock */
    /* 使能Relay对应GPIO的Clock时钟 */
    RCC_APB2PeriphClockCmd(RELAY_GPIO_CLK[rly], ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);     //禁止JTAG

    /* Configure the GPIO_LED pin */
    /* 初始化Relay的GPIO管脚，配置为推挽输出 */
    GPIO_InitStructure.GPIO_Pin = RELAY_GPIO_PIN[rly];
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(RELAY_PORT[rly], &GPIO_InitStructure);
}

/**-------------------------------------------------------
  * @函数名 BSP_Relay_Close
  * @功能   熄灭LED灯
  * @返回值 无
***------------------------------------------------------*/
void BSP_Relay_Open(Relay_TypeDef rly)
{
	/*
	if (rly == BMSCONTACTOR_RELAY)
	{
		RELAY_PORT[rly]->BSRR= RELAY_GPIO_PIN[rly];
		return;
	}
	*/
	RELAY_PORT[rly]->BRR = RELAY_GPIO_PIN[rly];
	//  RisingEdgePulse(CPDO[rly]);
}

/**-------------------------------------------------------
  * @函数名 BSP_Relay_Open
  * @功能   点亮LED灯
  * @返回值 无
***------------------------------------------------------*/
void BSP_Relay_Close(Relay_TypeDef rly)
{
	/*
	if (rly == BMSCONTACTOR_RELAY)
	{
		RELAY_PORT[rly]->BRR = RELAY_GPIO_PIN[rly];
		return;
	}
	*/
    RELAY_PORT[rly]->BSRR= RELAY_GPIO_PIN[rly];
//  RisingEdgePulse(CPDO[rly]);
}



