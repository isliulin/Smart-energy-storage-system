/******************** (C) COPYRIGHT 2013 www.armjishu.com  ********************
 * 文件名  ：can.c
 * 描述    ：
 * 实验平台：STM32神舟开发板
 * 标准库  ：STM32F10x_StdPeriph_Driver V3.5.0
 * 作者    ：www.armjishu.com
*******************************************************************************/
#include "STM32F107_BSP_led.h"
#include "STM32F107_BSP_can.h"
//#include "usart.h"


CAN_InitTypeDef        CAN_InitStructure;
CAN_FilterInitTypeDef  CAN_FilterInitStructure;


CanTxMsg CAN1_TxMessage;
CanTxMsg CAN2_TxMessage;
CanTxMsg CAN3_TxMessage;


void RCC_Config(void);
void GPIO_Config(void);
void NVIC_Config(void);
void CAN1_Config(void);
void CAN2_Config(void);



uint8_t BSP_Can1_TranMsg(CanTxMsg* TxMessage)
{
    return (CAN_Transmit(CAN1, TxMessage));
}

void BSP_Can2_TranMsg(CanTxMsg* TxMessage)
{
    CAN_Transmit(CAN2, TxMessage);
}

void BSP_CAN1_CloseInt(void)
{
	CAN_ITConfig(CAN1,CAN_IT_TME, DISABLE);

}

void BSP_CAN1_OpenInt(void)
{
	CAN_ITConfig(CAN1,CAN_IT_TME, ENABLE);
}

void BSP_CAN2_CloseInt(void)
{
	CAN_ITConfig(CAN2,CAN_IT_TME, DISABLE);

}

void BSP_CAN2_OpenInt(void)
{
	CAN_ITConfig(CAN2,CAN_IT_TME, ENABLE);
}

void BSP_CAN3_CloseInt(void)
{
	  //mcp2515_Write(CANINTF,0x00);

}

void BSP_CAN3_OpenInt(void)
{
	  //mcp2515_Write(CANINTF,0x04);
}




/*
* 函数名：BSP_CanInit
* 描述	：初始化
* 输入	：void
* 输出	: void
*/

void BSP_CanInit(void)
{
	// CAN时钟初始化
	RCC_Config();

	// CAN相关GPIO管脚初始化
	GPIO_Config();

	//CAN中断相关配置
	NVIC_Config();

	// CAN初始化配置
	CAN1_Config();
	CAN2_Config();
 }

 void BSP_Can2_Init(void)
 {
	CAN_ITConfig(CAN2,CAN_IT_TME, ENABLE);
  	CAN_ITConfig(CAN2,CAN_IT_TME, DISABLE);   //关
	CAN2_Config();
 }


 /*
 * 函数名：RCC_Config
 * 描述  ：配置 CAN 时钟配置
 * 输入  ：无
 * 输出  : -PASSED   成功
 *         -FAILED   失败
 */
void RCC_Config(void)
{
  /* GPIOB, GPIOD and AFIO clocks enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

  /* CAN1 and CAN2 Periph clocks enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2, ENABLE);
}


/**
  * @brief  Config the GPIOs.
  * @param  None
  * @retval None
  */
void GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//zhaolvhua

  /* Configure CAN1 RX pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure CAN2 RX pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Configure CAN1 TX pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure CAN2 TX pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // Remap CAN1 and CAN2 GPIOs
  //GPIO_PinRemapConfig(GPIO_Remap1_CAN1 , ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_CAN2, ENABLE);
}


/**
  * @brief  Config the NVIC for CAN.
  * @param  None
  * @retval None
  */
void NVIC_Config(void)
{

  NVIC_InitTypeDef  NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;

  NVIC_Init(&NVIC_InitStructure);


  NVIC_InitStructure.NVIC_IRQChannel = CAN1_TX_IRQn;
  //NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
  //NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = CAN2_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_Init(&NVIC_InitStructure);


  CAN_ITConfig(CAN1,CAN_IT_TME, ENABLE);
  CAN_ITConfig(CAN1,CAN_IT_TME, DISABLE);   //关

  CAN_ITConfig(CAN2,CAN_IT_TME, ENABLE);
  CAN_ITConfig(CAN2,CAN_IT_TME, DISABLE);   //关

}


/**
  * @brief  Config the CAN.
  * @param  None
  * @retval None
  */
void CAN1_Config(void)
{
  /* CAN register init */
  CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN1 cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = ENABLE;//DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;

//  波特率计算 r = 36M /36(1+4+3) = 125kbpsrgfg
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;//CAN_SJW_1tq;
 /* CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;//CAN_BS1_3tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;//CAN_BS2_5tq;
  CAN_InitStructure.CAN_Prescaler = 36;////4;//波特率预分频数*/
  CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;//CAN_BS1_3tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;//CAN_BS2_5tq;
  CAN_InitStructure.CAN_Prescaler = 45;////4;//波特率预分频数


  CAN_Init(CAN1, &CAN_InitStructure);

  // CAN1中断配置
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

  /* CAN1 filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

  CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE);

  /* CAN2 filter init */
  //CAN_FilterInitStructure.CAN_FilterNumber = 14;
  //CAN_FilterInit(&CAN_FilterInitStructure);

  /* Transmit */
  CAN1_TxMessage.StdId = 0x321;
  CAN1_TxMessage.ExtId = 0x01;
  CAN1_TxMessage.RTR = CAN_RTR_DATA;
  CAN1_TxMessage.IDE = CAN_ID_STD;
  CAN1_TxMessage.DLC = 2;

}


void CAN2_Config(void)
{
  /* CAN register init */
  CAN_DeInit(CAN2);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN1 cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = ENABLE;//DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;

//  波特率计算 r = 36M /9(1+13+9) = 250kbpsrgfg
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;//CAN_SJW_1tq;
  /*CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;//CAN_BS1_3tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;//CAN_BS2_5tq;
  CAN_InitStructure.CAN_Prescaler = 18;////4;//波特率预分频数*/
  CAN_InitStructure.CAN_BS1 = CAN_BS1_13tq;//CAN_BS1_3tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;//CAN_BS2_5tq;
  CAN_InitStructure.CAN_Prescaler = 9;////4;//波特率预分频数

  CAN_Init(CAN2, &CAN_InitStructure);

  // CAN2中断配置
  CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);

  // CAN1 filter init
  CAN_FilterInitStructure.CAN_FilterNumber = 14;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

  CAN_ITConfig(CAN2,CAN_IT_TME,ENABLE);

  // Transmit
  CAN2_TxMessage.StdId = 0x321;
  CAN2_TxMessage.ExtId = 0x01;
  CAN2_TxMessage.RTR = CAN_RTR_DATA;
  CAN2_TxMessage.IDE = CAN_ID_STD;
  CAN2_TxMessage.DLC = 2;

}

void CAN3_Config(void)
{
  /* CAN register init */
  CAN_DeInit(CAN1);
  CAN_DeInit(CAN2);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN1 cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;

//  波特率计算 r = 36M /9(1+4+3) = 500kbpsrgfg
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;//CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;//CAN_BS1_3tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;//CAN_BS2_5tq;
  CAN_InitStructure.CAN_Prescaler = 9;////4;//波特率预分频数


  CAN_Init(CAN1, &CAN_InitStructure);
  CAN_Init(CAN2, &CAN_InitStructure);

  /* CAN1 filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

  /* CAN2 filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 14;
  CAN_FilterInit(&CAN_FilterInitStructure);

  /* Transmit */
  CAN3_TxMessage.StdId = 0x321;
  CAN3_TxMessage.ExtId = 0x01;
  CAN3_TxMessage.RTR = CAN_RTR_DATA;
  CAN3_TxMessage.IDE = CAN_ID_STD;
  CAN3_TxMessage.DLC = 2;

}


/**
  * @}
  */


