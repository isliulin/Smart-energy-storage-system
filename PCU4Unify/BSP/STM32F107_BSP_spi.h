
#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"


   
#define MAX_SPI_BUFF_LENGTH      40  
#define sCAN_SPI		 SPI2
#define sCAN_SPI_CLK		 RCC_APB1Periph_SPI2
#define sCAN_SPI_SCK_PIN         GPIO_Pin_13                 
#define sCAN_SPI_SCK_GPIO_PORT   GPIOB                       
#define sCAN_SPI_SCK_GPIO_CLK    RCC_APB2Periph_GPIOB  

#define sCAN_SPI_MISO_PIN        GPIO_Pin_14                 
#define sCAN_SPI_MISO_GPIO_PORT  GPIOB                       
#define sCAN_SPI_MISO_GPIO_CLK   RCC_APB2Periph_GPIOB  

#define sCAN_SPI_MOSI_PIN        GPIO_Pin_15                 
#define sCAN_SPI_MOSI_GPIO_PORT  GPIOB                       
#define sCAN_SPI_MOSI_GPIO_CLK   RCC_APB2Periph_GPIOB  

#define sCAN_CS_PIN              GPIO_Pin_12                
#define sCAN_CS_GPIO_PORT        GPIOB                       
#define sCAN_CS_GPIO_CLK         RCC_APB2Periph_GPIOB
   
   
#define sCAN_INT_PIN              GPIO_Pin_7                
#define sCAN_INT_GPIO_PORT        GPIOB                      
#define sCAN_INI_GPIO_CLK         RCC_APB2Periph_GPIOB
   
#define sCAN_RESET_PIN              GPIO_Pin_2                
#define sCAN_RESET_GPIO_PORT        GPIOD                      
#define sCAN_RESET_GPIO_CLK         RCC_APB2Periph_GPIOD
   
  void BSP_SPI_Init();
  void SPI_Config(void);
  void EXTI_Configuration(void);
  char spi_ReadorWrite(SPI_TypeDef* SPIx,char *buf, uint16_t count);
  u8  SPI_MCP2525_Send(u8 byte);
  
  void MCP2525_RecieveCanMsg(CanRxMsg* RxMessage, uint8_t u8Rxn);
  void mcp2515_tx0_extend(CanTxMsg* TxMessage);
  




#ifdef __cplusplus
}
#endif

#endif
