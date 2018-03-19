/**
  ******************************************************************************
  * @file    CAN/LoopBack/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
//#include "stm32_eval.h"
#include "STM32F107_BSP_led.h"
#include "STM32F107_BSP_Timer.h"
#include "STM32F107_BSP_can.h"
#include "Publics.h"
#include <stdio.h>

#include "STM32F107_BSP_usart.h"



/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Printf
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint32_t ret;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles CAN1 Handler.
  * @param  None
  * @retval None
  */
/*#ifndef STM32F10X_CL
void USB_LP_CAN1_RX0_IRQHandler(void)
#else
void CAN1_RX0_IRQHandler(void)
#endif
{
  CanRxMsg RxMessage;

  RxMessage.StdId=0x00;
  RxMessage.ExtId=0x00;
  RxMessage.IDE=0;
  RxMessage.DLC=0;
  RxMessage.FMI=0;
  RxMessage.Data[0]=0x00;
  RxMessage.Data[1]=0x00;

  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

  if((RxMessage.ExtId==0x1234) && (RxMessage.IDE==CAN_ID_EXT)
     && (RxMessage.DLC==2) && ((RxMessage.Data[1]|RxMessage.Data[0]<<8)==0xDECA))
  {
    ret = 1;
  }
  else
  {
    ret = 0;
  }
}*/
extern CanTxMsg TxMessage;

void TIM4_IRQHandler(void)
{

	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {

		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
}



void TIM5_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
		Timer_Tick_Maintain();

		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
    }
}


/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

void CAN1_TX_IRQHandler(void)
//中断处理
{
	CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
	objCanInnerProtocol.CanXmitFcb(NULL);


}

void CAN2_TX_IRQHandler(void)
//中断处理
{
	CAN_ClearITPendingBit(CAN2,CAN_IT_TME);
  	 objCanBMSProtocol.CanXmitFcb(NULL);
}



/**
  * @brief  This function handles CAN1 RX0 Handler.
  * @param  None
  * @retval None
  */

void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	memset(&RxMessage, 0, sizeof(CanRxMsg));
  	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

	objCanInnerProtocol.CanRecvFcb(&RxMessage);

}

/**
  * @brief  This function handles CAN1 RX0 Handler.
  * @param  None
  * @retval None
  */
void CAN2_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	memset(&RxMessage, 0, sizeof(CanRxMsg));	
	CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);
	objCanBMSProtocol.CanRecvFcb(&RxMessage);


	/*if ((RxMessage.StdId == 0x321)&&(RxMessage.IDE == CAN_ID_STD)&&(RxMessage.DLC == 1)&&(RxMessage.Data[0] == 0xAA))
	{
		BSP_LED_Off(LED1);//LED_Display(0x03);
	}
	else
	{
		BSP_LED_On(LED1);
	}*/
}

void EXTI9_5_IRQHandler(void)
{
	CanRxMsg RxMessage;
	memset(&RxMessage, 0, sizeof(CanRxMsg));
  	//SPI_MCP2515_Receive(&RxMessage);
        char d=0,d1=0,d2=0,d3=0;
        /*
          char Te=0,Re=0;
	 Te=mcp2515_Read(TEC);
	 Re=mcp2515_Read(REC);
	 if(Te>0 ||Re>0)
	 {
	 	//mcp2515_Write(TEC,0x00);
		//mcp2515_Write(REC,0x00);
	 }
       */
	  d = mcp2515_Read(CANINTF);
         if((d & 0x01)==RX0IF_SET)  //MCP2515接收中断
         {
           MCP2525_RecieveCanMsg(&RxMessage,RX0IF_SET); 
           objCanMasterMonPrtcl.CanRecvFcb(&RxMessage);

         }
	if((d & 0x02)==RX1IF_SET)
	{
	    MCP2525_RecieveCanMsg(&RxMessage,RX1IF_SET); 
           objCanMasterMonPrtcl.CanRecvFcb(&RxMessage);
	}
		
         if(((d & 0x04)==TX0IF_SET) 
		||((d & 0x08)==TX1IF_SET)
		||((d & 0x10)==TX2IF_SET))//mcp2515 发送中断
        {
		//objCanMasterMonPrtcl.m_u16_CanIsrXmitFlag = 1;
		  objCanMasterMonPrtcl.CanXmitFcb(NULL);
		  objPcuModel.mcp_offline_cnt =0;			
        } 

		
	 if((d & 0x20)==ERRIF_SET)	
	 {
		d1=mcp2515_Read(EFLG);
        d2=mcp2515_Read(TEC);
		d3=mcp2515_Read(REC);
                mcp2515_Write(EFLG,0);
	 }
        /*
	 if((d & 0x80)==MERRF_SET)
	 {
 		d1=mcp2515_Read(EFLG);
               d2=mcp2515_Read(TEC);
		d3=mcp2515_Read(REC);
	 }
	*/
	if((d & 0x40)==WAKIF_SET)
	{
            //    mcp2515_Reset();
			mcp2515_Write(CANCTRL, (REQOP_NORMAL | CLKEN | CLKPRE));
     
	}
        mcp2515_Write(CANINTF,0x00);
  	EXTI_ClearITPendingBit(EXTI_Line7);
}


void EXTI15_10_IRQHandler(void)//EPO中断触发后直接写EEPROM
{
	if(EXTI_GetITStatus(EXTI_Line13) != RESET)//EXTI13产生了中断
	{
		//BSP_Relay_Close(ONLINE_LED); //J62-2 FOR TEST
		objEEPROM.WriteEpoTime();
		//BSP_Relay_Open(ONLINE_LED); //J62-2 FOR TEST
	}
	EXTI_ClearITPendingBit(EXTI_Line13);//清除中断



}


void DMA1_Channel1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC1))
	{
		DMA_ClearITPendingBit(DMA1_IT_GL1);
		BSP_Int_SamplingData();			        //获取减去基准后的AD转换值
		objUtility.Dat_Int_InputPreDeal();		//转换成电压值
		objUtility.Dat_Int_PhaseACal();			//计算A相累加和，计算过零点
		objUtility.Dat_Int_PhaseBCal();			//计算B相累加和
		objUtility.Dat_Int_PhaseCCal();			//计算C相累加和
		objUtility.Alg_Int_VinFastChk();		//输入A相掉电快检
	}
}

/**
  * @brief	This function handles UART2 RX0 Handler.
  * @param	None
  * @retval None
  */
void USART1_IRQHandler(void)
{
        if (USART_GetFlagStatus(USART1, USART_FLAG_TC) != RESET)
	{
		USART_ClearFlag(USART1, USART_FLAG_TC);	
		BSP_Usart1XmitFcb();
               objModBusProtocol.m_ModBusRecvProtocol.ReceCount = 0;
	
	}
	

	objModBusProtocol.m_ModBusRecvProtocol.ReceTimeOut = COMTIMEOUT;			//超时时间为10MS
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)	//判断是否为接收中断
	{
		objModBusProtocol.m_ModBusRecvProtocol.ReceBuf[objModBusProtocol.m_ModBusRecvProtocol.ReceCount] = USART_ReceiveData(USART1);
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
		if(objModBusProtocol.m_ModBusRecvProtocol.ReceCount < MAXCOMNUM-1)
		{
			objModBusProtocol.m_ModBusRecvProtocol.ReceCount++;
		}
		else
		{
			objModBusProtocol.m_ModBusRecvProtocol.ReceCount = 0;
		}

		if(objModBusProtocol.m_ModBusRecvProtocol.ReceCount >= 3)
		{
			if(((objModBusProtocol.m_ModBusRecvProtocol.ReceBuf[1] == 0x03) || (objModBusProtocol.m_ModBusRecvProtocol.ReceBuf[1] == 0x04))
				&&((objModBusProtocol.m_ModBusRecvProtocol.ReceCount) >= (objModBusProtocol.m_ModBusRecvProtocol.ReceBuf[2]+5)))
			{
				objModBusProtocol.RS485RecvFcb(objModBusProtocol.m_ModBusRecvProtocol.ReceBuf);
                objModBusProtocol.m_ModBusRecvProtocol.ReceCount = 0;
			}
			else if((objModBusProtocol.m_ModBusRecvProtocol.ReceBuf[1] == 0x05) && (objModBusProtocol.m_ModBusRecvProtocol.ReceCount == 8))
			{
				objCanDataPool.m_i16TurnonoffMoudle = 0;
				objModBusProtocol.RS485RecvFcb(objModBusProtocol.m_ModBusRecvProtocol.ReceBuf);
				objModBusProtocol.m_ModBusRecvProtocol.ReceCount = 0;
			}
			else if((objModBusProtocol.m_ModBusRecvProtocol.ReceBuf[1] == 0x06) && (objModBusProtocol.m_ModBusRecvProtocol.ReceCount == 8))
			{
				objCanDataPool.m_i16_SetACDCFlag = 0;
				objModBusProtocol.RS485RecvFcb(objModBusProtocol.m_ModBusRecvProtocol.ReceBuf);
				objModBusProtocol.m_ModBusRecvProtocol.ReceCount = 0;
			}
		}
	}	
}

void USART2_IRQHandler(void)
{

	

}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */



