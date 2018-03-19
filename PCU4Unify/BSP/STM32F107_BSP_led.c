
 /********************   (C) COPYRIGHT 2013 www.armjishu.com   ********************
 * 文件名  ：led.c
 * 描述    ：提供STM32F107VC神舟IV号开发板的库函数
 * 实验平台：STM32神舟IV号开发板
 * 作者    ：www.armjishu.com
 * 修改日期：2014/03
**********************************************************************************/
#include "STM32F107_BSP_led.h"

GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT};//, LED3_GPIO_PORT};
const uint16_t GPIO_PIN_NUM[LEDn] = {LED1_PIN_NUM, LED2_PIN_NUM};//, LED3_PIN_NUM};
const uint16_t GPIO_PIN[LEDn] = {LED1_PIN, LED2_PIN};//, LED3_PIN};
const uint32_t GPIO_CLK[LEDn] = {LED1_GPIO_CLK, LED2_GPIO_CLK};//, LED3_GPIO_CLK};

/**-------------------------------------------------------
  * @函数名 LED_Init
  * @功能   初始化板载4个LED的GPIO管脚，配置为推挽输出
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* 使能LED对应GPIO的Clock时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    /* 初始化LED的GPIO管脚，配置为推挽输出 */				  //设置端口
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	//GPIO_Pin_4 | GPIO_Pin_7;   //定义了4个led灯管脚
														 	  //设置对端口的模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		  //设置为 推挽(Push-Pull)输出
														 	  //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //最大输出速度为50MHz

    GPIO_Init(GPIOD, &GPIO_InitStructure);	                  //调用GPIO初始化函数
}

/**-------------------------------------------------------
  * @函数名 BSP_LED_Init
  * @功能   初始化LED的GPIO管脚，配置为推挽输出
  * @参数   LED1  对应开发板上第一个指示灯
  *         LED2  对应开发板上第二个指示灯
  *         LED3  对应开发板上第三个指示灯
  *         LED4  对应开发板上第四个指示灯
  * @返回值 无
***------------------------------------------------------*/
void BSP_LED_Init(Led_TypeDef Led)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); 	//关闭JTAG
    /* Enable the GPIO_LED Clock */
    /* 使能LED对应GPIO的Clock时钟 */
    RCC_APB2PeriphClockCmd(GPIO_CLK[Led], ENABLE);

    /* Configure the GPIO_LED pin */
    /* 初始化LED的GPIO管脚，配置为推挽输出 */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN[Led];
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);
}


/**-------------------------------------------------------
  * @函数名 BSP_LED_Off
  * @功能   熄灭LED灯
  * @参数   LED1
  * @参数   LED2
  * @参数   LED3
  * @参数   LED4
  * @返回值 无
***------------------------------------------------------*/
void BSP_LED_Off(Led_TypeDef Led)
{
  GPIO_PORT[Led]->BRR = GPIO_PIN[Led];
}

/**-------------------------------------------------------
  * @函数名 BSP_LED_On
  * @功能   点亮LED灯
  * @参数   LED1
  * @参数   LED2
  * @参数   LED3
  * @参数   LED4
  * @返回值 无
***------------------------------------------------------*/
void BSP_LED_On(Led_TypeDef Led)
{
  GPIO_PORT[Led]->BSRR = GPIO_PIN[Led];
}

/**-------------------------------------------------------
  * @函数名 BSP_LED_On
  * @功能   点亮LED灯
  * @参数   LED1
  * @参数   LED2
  * @参数   LED3
  * @参数   LED4
  * @返回值 无
***------------------------------------------------------*/
void BSP_LED_Toggle(Led_TypeDef Led)
{
  GPIO_PORT[Led]->ODR ^= GPIO_PIN[Led];
}



