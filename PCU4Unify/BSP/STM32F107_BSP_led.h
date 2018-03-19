
#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
 extern "C" {
#endif


#include "stm32f10x.h"

//-------------------------------------LED--------------------------------------------------------------

/** 定义为枚举类型，方便多个指示灯时扩展 **/
/** 指示灯定义 **/
/*typedef enum
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
  LED4 = 3
} Led_TypeDef;
*/

typedef enum
{
  LED1 = 0,
  LED2 = 1,
//  LED3 = 2,
//  LED4 = 3,

  // Color led
  LED_GREEN = LED1,
//  LED_YELLOW = LED2,
  LED_RED = LED2,

  // Function led
  LED_RUN  = LED_GREEN,
//  LED_WARN    = LED_YELLOW,
  LED_FAULT   = LED_RED
}Led_TypeDef;


// 指示灯管脚资源个数定义

#define LEDn                             2

// LED指示灯管脚资源定义  输出低电平点亮指示灯
#define LED1_PIN_NUM                     7 /* bitband别名区使用宏定义  */
#define LED1_PIN                         GPIO_Pin_7
#define LED1_GPIO_PORT                   GPIOD
#define LED1_GPIO_CLK                    RCC_APB2Periph_GPIOD
#define LED1OBB                          Periph_BB((uint32_t)&LED1_GPIO_PORT->ODR, LED1_PIN_NUM)//等价于Periph_BB((uint32_t)&GPIOD->ODR, 12)

#define LED2_PIN_NUM                     6
#define LED2_PIN                         GPIO_Pin_6
#define LED2_GPIO_PORT                   GPIOD
#define LED2_GPIO_CLK                    RCC_APB2Periph_GPIOD
#define LED2OBB                          Periph_BB((uint32_t)&LED2_GPIO_PORT->ODR, LED2_PIN_NUM)

//#define LED3_PIN_NUM                     3
//#define LED3_PIN                         GPIO_Pin_3
//#define LED3_GPIO_PORT                   GPIOB
//#define LED3_GPIO_CLK                    RCC_APB2Periph_GPIOB
//#define LED3OBB                          Periph_BB((uint32_t)&LED3_GPIO_PORT->ODR, LED3_PIN_NUM)

/*
#define LED4_PIN_NUM                     7
#define LED4_PIN                         GPIO_Pin_7
#define LED4_GPIO_PORT                   GPIOD
#define LED4_GPIO_CLK                    RCC_APB2Periph_GPIOD
#define LED4OBB                          Periph_BB((uint32_t)&LED4_GPIO_PORT->ODR, LED4_PIN_NUM)
*/

void LED_Init(void);
void BSP_LED_Init(Led_TypeDef Led);
void BSP_LED_Off(Led_TypeDef Led);
void BSP_LED_On(Led_TypeDef Led);
void BSP_LED_Toggle(Led_TypeDef Led);

#ifdef __cplusplus
}
#endif

#endif


