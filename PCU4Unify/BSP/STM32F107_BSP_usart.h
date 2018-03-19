
#ifndef __BSP_USART_H
#define __BSP_USART_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"

#define USART_BAUDRATE1 38400


typedef enum 
{
  COM1 = 0,   
  COM2 = 1,
  COM4 = 2
} COM_TypeDef; 






void BSP_METER485SendData(u8 *Data,u8 SendNum);
void BSP_COM_Init(void);
void BSP_METER485_WE(void);
void BSP_METER485_RD(void);
void BSP_Usart1XmitFcb(void);


void BSP_SocLightSendData(u8 *Data,u8 SendNum);






#ifdef __cplusplus
}
#endif

#endif


