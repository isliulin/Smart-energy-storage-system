
#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"


typedef union   
{
	uint16_t all;
	struct
	{
		uint16_t	b_SW1		:1;		// 0：闭合；1:断开
		uint16_t	b_SW2		:1;		// 0：断开；1:闭合
		uint16_t	b_SW3		:1;		// 0：亮；	1:灭
		uint16_t	b_SW4		:1;		// 0：光耦关；1:光耦开
		uint16_t	b_SW5		:1;		// 0：灭；	1:亮
		uint16_t	b_SW6		:1;     // 0:低电平; 1:高电平（低写有效）
	    uint16_t	b_SW7		:1;		//
		uint16_t    b_SW8		:1;
		uint16_t  	bResvd		:8;
	}BIT;
}_ExOutGPIORegDef;			

typedef union
{
	uint16_t 	all;
	struct
	{
		uint16_t	b_SW1		:1;		// 0：闭合；1:断开
		uint16_t	b_SW2		:1;		// 0：断开；1:闭合
		uint16_t	b_SW3		:1;		// 0：亮；	1:灭
		uint16_t	b_SW4		:1;		// 0：光耦关；1:光耦开
		uint16_t	b_SW5		:1;		// 0：灭；	1:亮
		uint16_t	b_SW6		:1;     // 0:低电平; 1:高电平（低写有效）
	    uint16_t	b_SW7		:1;		// 1:	辅助电源关机信号,低有效
		uint16_t    b_SW8		:1;
		uint16_t  	bResvd		:8;
	}BIT;
}_ExInGPIORegDef;

//------------------------------GPIO define--------------------------------------
typedef enum 
{  
	DI_START  = 0,
	DI_FANHOT_FB     = 0,		//风机
	DI_EPO_FB  = 1,				//急停
	DI_PROTECT_FB    = 2,		//防雷
	DI_ACBREAK_FB = 3,		//AC 
	DCBREAK1_FB	 = 4,		//DC
	DCBREAK2_FB	 =5,		//DC
	DCBREAK3_FB	=6,		//DC
	DCBREAK4_FB	=7,		//DC
	//DI_GUNPOS_FB = 3,		//
	//DI_BMSAUXPOWER_FB = 4,
	//DI_ELOCK_FB = 5,  
	//DI_BMSAUX12_FB = 10,
  	DI_NUM,
}_DI_TypeDef;

typedef enum
{
	E_LOW = 0,
	E_HIGH = 1,
}_IOStateDef;

#define DIn                        		(DI_NUM)

/**
 * @brief DI define
 */

#define PORT_DI_FANHOT_FB	          GPIOD			//风机故障
#define PIN_DI_FANHOT_FB	          GPIO_Pin_12
#define CLK_DI_FANHOT_FB	          RCC_APB2Periph_GPIOD

#define PORT_DI_EPO_FB	          GPIOD                        //急停反馈
#define PIN_DI_EPO_FB	          GPIO_Pin_13
#define CLK_DI_EPO_FB	          RCC_APB2Periph_GPIOD

#define PORT_DI_PROTECT_FB	          GPIOD			//防雷保护
#define PIN_DI_PROTECT_FB	          GPIO_Pin_14
#define CLK_DI_PROTECT_FB	          RCC_APB2Periph_GPIOD

#define PORT_DI_ACBREAK_FB	          GPIOD		//交流断路器
#define PIN_DI_ACBREAK_FB	          GPIO_Pin_15
#define CLK_DI_ACBREAK_FB	          RCC_APB2Periph_GPIOD

#define PORT_DI_DCBREAK1_FB	                 GPIOC//直流断路器
#define PIN_DI_DCBREAK1_FB	                 GPIO_Pin_6
#define CLK_DI_DCBREAK1_FB	                 RCC_APB2Periph_GPIOC

#define PORT_DI_DCBREAK2_FB	          		GPIOC
#define PIN_DI_DCBREAK2_FB	                 GPIO_Pin_7
#define CLK_DI_DCBREAK2_FB	                 RCC_APB2Periph_GPIOC

#define PORT_DI_DCBREAK3_FB	         	  GPIOC
#define PIN_DI_DCBREAK3_FB	          	  GPIO_Pin_8
#define CLK_DI_DCBREAK3_FB	         	  RCC_APB2Periph_GPIOC

#define PORT_DI_DCBREAK4_FB	          GPIOC
#define PIN_DI_DCBREAK4_FB	          GPIO_Pin_9
#define CLK_DI_DCBREAK4_FB	          RCC_APB2Periph_GPIOC






/*
#define PORT_DI_GUNPOS_FB	          GPIOC
#define PIN_DI_GUNPOS_FB	          GPIO_Pin_9
#define CLK_DI_GUNPOS_FB	          RCC_APB2Periph_GPIOC

#define PORT_DI_BMSAUXPOWER_FB	          GPIOC
#define PIN_DI_BMSAUXPOWER_FB	          GPIO_Pin_13
#define CLK_DI_BMSAUXPOWER_FB	          RCC_APB2Periph_GPIOC

#define PORT_DI_BMSAUX12_FB	          GPIOD
#define PIN_DI_BMSAUX12_FB	          GPIO_Pin_9
#define CLK_DI_BMSAUX12_FB	          RCC_APB2Periph_GPIOD


#define PORT_DI_ELOCK_FB	          GPIOD
#define PIN_DI_ELOCK_FB	          GPIO_Pin_11
#define CLK_DI_ELOCK_FB	          RCC_APB2Periph_GPIOD
*/
/*
#define PORT_DI_EPO_FB	                 GPIOC
#define PIN_DI_EPO_FB	                 GPIO_Pin_6
#define CLK_DI_EPO_FB	                 RCC_APB2Periph_GPIOC

#define PORT_DI_DOOR_FB	          		GPIOC
#define PIN_DI_DOOR_FB	                 GPIO_Pin_7
#define CLK_DI_DOOR_FB	                 RCC_APB2Periph_GPIOC

#define PORT_DI_SPD_FB	         	  GPIOC
#define PIN_DI_SPD_FB	          	  GPIO_Pin_8
#define CLK_DI_SPD_FB	         	  RCC_APB2Periph_GPIOC

#define PORT_DI_ACBREAK_FB	          GPIOD
#define PIN_DI_ACBREAK_FB	          GPIO_Pin_12
#define CLK_DI_ACBREAK_FB	          RCC_APB2Periph_GPIOD

#define PORT_DI_FANHOT_FB	          GPIOD                        //风机热保护接触器反馈
#define PIN_DI_FANHOT_FB	          GPIO_Pin_13
#define CLK_DI_FANHOT_FB	          RCC_APB2Periph_GPIOD

#define PORT_DI_POWERON_FB	          GPIOD
#define PIN_DI_POWERON_FB	          GPIO_Pin_14
#define CLK_DI_POWERON_FB	          RCC_APB2Periph_GPIOD

#define PORT_DI_POWEROFF_FB	          GPIOD
#define PIN_DI_POWEROFF_FB	          GPIO_Pin_15
#define CLK_DI_POWEROFF_FB	          RCC_APB2Periph_GPIOD
*/



void  BSP_DI_Init(_DI_TypeDef DI);
void  BSP_DI_Filter(void);
int16_t BSP_DI_GetState(_DI_TypeDef DI);
int16_t BSP_DI_GetRTState(_DI_TypeDef DI);


//-DO define-------------------------------------------------------------------
typedef enum
{
	FAN1_RELAY = 0,		//Fan1 relay
	FAN2_RELAY = 1,		//Fan2 relay
	IDLE_LED = 2,	//IDLE_LED
	ONLINE_LED = 3,	//ONLINE_LED
	OFFLINE_LED = 4, 	//OFFLINE_LED
	FAULT_LED = 5,	//FAULT_LED
/*
	ISULT_GND_RELAY = 0,
	BMS24V_RELAY = 1,
	INSULTNEG_RELAY = 2,
	ELOCKN_RELAY = 3,
	ELOCKP_RELAY = 4,
	FAN1_RELAY = 5,
	ACCONTACTOR_RELAY=7,
	POSLED_RELAY=10,
	ERRORLED_RELAY=11,
	CHARGELED_RELAY=12,
	INSULTPOS_RELAY=13,
	BMS12V_RELAY=14,

	LAMPGREEN_RELAY=16,
	SAMP_RELAY=17,
	PULSEELOCK_RELAY =19
*/	
}Relay_TypeDef;

// 指示灯管脚资源个数定义

#define RELAYn                             5
/*
#define DO_RES_RELAY_PIN_NUM            0
#define DO_RES_RELAY_PIN                     GPIO_Pin_0
#define DO_RES_RELAY_GPIO_PORT        GPIOE
#define DO_RES_RELAY_GPIO_CLK           RCC_APB2Periph_GPIOE

#define DO_BMS24V_RELAY_PIN_NUM            7
#define DO_BMS24V_RELAY_PIN                     GPIO_Pin_7
#define DO_BMS24V_RELAY_GPIO_PORT        GPIOE
#define DO_BMS24V_RELAY_GPIO_CLK           RCC_APB2Periph_GPIOE

#define DO_INSULTNEG_RELAY_PIN_NUM            9
#define DO_INSULTNEG_RELAY_PIN                     GPIO_Pin_9
#define DO_INSULTNEG_RELAY_GPIO_PORT        GPIOE
#define DO_INSULTNEG_RELAY_GPIO_CLK           RCC_APB2Periph_GPIOE

#define DO_ELOCKN_RELAY_PIN_NUM            10
#define DO_ELOCKN_RELAY_PIN                     GPIO_Pin_10
#define DO_ELOCKN_RELAY_GPIO_PORT        GPIOC
#define DO_ELOCKN_RELAY_GPIO_CLK           RCC_APB2Periph_GPIOC

#define DO_ELOCKP_RELAY_PIN_NUM            11
#define DO_ELOCKP_RELAY_PIN                     GPIO_Pin_11
#define DO_ELOCKP_RELAY_GPIO_PORT        GPIOC
#define DO_ELOCKP_RELAY_GPIO_CLK           RCC_APB2Periph_GPIOC
*/

//FAN1
#define DO_FAN1_RELAY_PIN_NUM            0
#define DO_FAN1_RELAY_PIN                     GPIO_Pin_1
#define DO_FAN1_RELAY_GPIO_PORT        GPIOD
#define DO_FAN1_RELAY_GPIO_CLK           RCC_APB2Periph_GPIOD

//FAN2
#define DO_FAN2_RELAY_PIN_NUM            1
#define DO_FAN2_RELAY_PIN                     GPIO_Pin_0
#define DO_FAN2_RELAY_GPIO_PORT        GPIOD
#define DO_FAN2_RELAY_GPIO_CLK           RCC_APB2Periph_GPIOD

// IDLE LED
#define DO_IDLE_RELAY_PIN_NUM           2
#define DO_IDLE_RELAY_PIN                     GPIO_Pin_4
#define DO_IDLE_RELAY_GPIO_PORT        GPIOD
#define DO_IDLE_RELAY_GPIO_CLK           RCC_APB2Periph_GPIOD
// ONLINE LED
#define DO_ONLINE_RELAY_PIN_NUM           3
#define DO_ONLINE_RELAY_PIN                     GPIO_Pin_3
#define DO_ONLINE_RELAY_GPIO_PORT        GPIOD
#define DO_ONLINE_RELAY_GPIO_CLK           RCC_APB2Periph_GPIOD
//OFFLINE LED
#define DO_OFFLINE_RELAY2_PIN_NUM           4
#define DO_OFFLINE_RELAY2_PIN                     GPIO_Pin_2
#define DO_OFFLINE_RELAY2_GPIO_PORT        GPIOE
#define DO_OFFLINE_RELAY2_GPIO_CLK           RCC_APB2Periph_GPIOE
// Fault LED
#define DO_FAULTLED_RELAY_PIN_NUM           5
#define DO_FAULTLED_RELAY_PIN                     GPIO_Pin_6
#define DO_FAULTLED_RELAY_GPIO_PORT        GPIOD
#define DO_FAULTLED_RELAY_GPIO_CLK           RCC_APB2Periph_GPIOD


/*
//Fan 
#define DO_FAN_RELAY_PIN_NUM            0
#define DO_FAN_RELAY_PIN                     GPIO_Pin_0
#define DO_FAN_RELAY_GPIO_PORT        GPIOD
#define DO_FAN_RELAY_GPIO_CLK           RCC_APB2Periph_GPIOD
// AC Breaker
#define DO_ACBREAK_RELAY_PIN_NUM            1
#define DO_ACBREAK_RELAY_PIN                     GPIO_Pin_1
#define DO_ACBREAK_RELAY_GPIO_PORT        GPIOD
#define DO_ACBREAK_RELAY_GPIO_CLK           RCC_APB2Periph_GPIOD
// Fault LED
#define DO_FAULTLED_RELAY_PIN_NUM           3
#define DO_FAULTLED_RELAY_PIN                     GPIO_Pin_3
#define DO_FAULTLED_RELAY_GPIO_PORT        GPIOD
#define DO_FAULTLED_RELAY_GPIO_CLK           RCC_APB2Periph_GPIOD
// Running LED
#define DO_RUNLED_RELAY_PIN_NUM           4
#define DO_RUNLED_RELAY_PIN                     GPIO_Pin_4
#define DO_RUNLED_RELAY_GPIO_PORT        GPIOD
#define DO_RUNLED_RELAY_GPIO_CLK           RCC_APB2Periph_GPIOD

//Grid LED
#define DO_RUNLED_RELAY2_PIN_NUM           2
#define DO_RUNLED_RELAY2_PIN                     GPIO_Pin_2
#define DO_RUNLED_RELAY2_GPIO_PORT        GPIOE
#define DO_RUNLED_RELAY2_GPIO_CLK           RCC_APB2Periph_GPIOE
*/
/*

#define DO_LAMPGREEN_RELAY_PIN_NUM           1
#define DO_LAMPGREEN_RELAY_PIN                     GPIO_Pin_1
#define DO_LAMPGREEN_RELAY_GPIO_PORT        GPIOB
#define DO_LAMPGREEN_RELAY_GPIO_CLK           RCC_APB2Periph_GPIOB

#define DO_FAN1_RELAY_PIN_NUM            12
#define DO_FAN1_RELAY_PIN                     GPIO_Pin_12
#define DO_FAN1_RELAY_GPIO_PORT        GPIOC
#define DO_FAN1_RELAY_GPIO_CLK           RCC_APB2Periph_GPIOC


#define DO_ERRORLED_RELAY_PIN_NUM           6
#define DO_ERRORLED_RELAY_PIN                     GPIO_Pin_6
#define DO_ERRORLED_RELAY_GPIO_PORT        GPIOD
#define DO_ERRORLED_RELAY_GPIO_CLK           RCC_APB2Periph_GPIOD

#define DO_CHARGELED_RELAY_PIN_NUM           7
#define DO_CHARGELED_RELAY_PIN                     GPIO_Pin_7
#define DO_CHARGELED_RELAY_GPIO_PORT        GPIOD
#define DO_CHARGELED_RELAY_GPIO_CLK           RCC_APB2Periph_GPIOD

#define DO_INSULTPOS_RELAY_PIN_NUM           8
#define DO_INSULTPOS_RELAY_PIN                     GPIO_Pin_8
#define DO_INSULTPOS_RELAY_GPIO_PORT        GPIOD
#define DO_INSULTPOS_RELAY_GPIO_CLK           RCC_APB2Periph_GPIOD

#define DO_BMS12V_RELAY_PIN_NUM           10
#define DO_BMS12V_RELAY_PIN                     GPIO_Pin_10
#define DO_BMS12V_RELAY_GPIO_PORT        GPIOD
#define DO_BMS12V_RELAY_GPIO_CLK           RCC_APB2Periph_GPIOD

#define DO_LAMPRED_RELAY_PIN_NUM           0
#define DO_LAMPRED_RELAY_PIN                     GPIO_Pin_0
#define DO_LAMPRED_RELAY_GPIO_PORT        GPIOB
#define DO_LAMPRED_RELAY_GPIO_CLK           RCC_APB2Periph_GPIOB

#define DO_SAMP_RELAY_PIN_NUM           3
#define DO_SAMP_RELAY_PIN                     GPIO_Pin_3
#define DO_SAMP_RELAY_GPIO_PORT        GPIOE
#define DO_SAMP_RELAY_GPIO_CLK           RCC_APB2Periph_GPIOE

#define DO_PULSEELOCK_RELAY_PIN_NUM           14
#define DO_PULSEELOCK_RELAY_PIN                     GPIO_Pin_14
#define DO_PULSEELOCK_RELAY_GPIO_PORT        GPIOE
#define DO_PULSEELOCK_RELAY_GPIO_CLK           RCC_APB2Periph_GPIOE
*/

void  	BSP_Relay_Init(Relay_TypeDef rly);
void	BSP_Relay_Open(Relay_TypeDef rly);
void	BSP_Relay_Close(Relay_TypeDef rly);

//------------------------------------------------------------------------





//------------------------------------------------------------------------



// 指示灯管脚资源个数定义
void 	BSP_IO_Dat_Init(void);


#ifdef __cplusplus
}
#endif

#endif





