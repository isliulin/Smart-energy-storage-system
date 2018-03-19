
/********************   (C) COPYRIGHT 2013 www.armjishu.com   ********************
 * 文件名  ：usart.h
 * 描述    ：提供STM32F107VC神舟IV号开发板的库函数
 * 实验平台：STM32神舟开发板
 * 作者    ：www.armjishu.com 
**********************************************************************************/
#include "STM32F107_BSP_usart.h"
#include <stdio.h>


/* 资源定义 ------------------------------------------------------------------*/
/** 定义为枚举类型，方便多个指示灯时扩展 **/ 


/** 串口管脚资源个数定义  **/
#define COMn                             3
#define RS485n                           2

/** 串口1管脚资源定义  **/
#define STM32_COM1_STR                "USART1"
#define STM32_COM1                    USART1
#define STM32_COM1_CLK                RCC_APB2Periph_USART1
#define STM32_COM1_TX_PIN             GPIO_Pin_9
#define STM32_COM1_TX_GPIO_PORT       GPIOA
#define STM32_COM1_TX_GPIO_CLK        RCC_APB2Periph_GPIOA
#define STM32_COM1_RX_PIN             GPIO_Pin_10
#define STM32_COM1_RX_GPIO_PORT       GPIOA
#define STM32_COM1_RX_GPIO_CLK        RCC_APB2Periph_GPIOA
#define STM32_COM1_IRQn               USART1_IRQn

/** 串口2管脚资源定义 (USART2 pins remapped on GPIOD) **/
#define STM32_COM2_STR                "USART2"
#define STM32_COM2                    USART2
#define STM32_COM2_CLK                RCC_APB1Periph_USART2
#define STM32_COM2_TX_PIN             GPIO_Pin_2
#define STM32_COM2_TX_GPIO_PORT       GPIOA
#define STM32_COM2_TX_GPIO_CLK        RCC_APB2Periph_GPIOA
#define STM32_COM2_RX_PIN             GPIO_Pin_3
#define STM32_COM2_RX_GPIO_PORT       GPIOA
#define STM32_COM2_RX_GPIO_CLK        RCC_APB2Periph_GPIOA
#define STM32_COM2_IRQn               USART2_IRQn

/** 串口4管脚资源定义 (USART4 pins remapped on GPIOD) **/
#define STM32_COM4_STR                "USART4"
#define STM32_COM4                    UART4
#define STM32_COM4_CLK                RCC_APB1Periph_UART4
#define STM32_COM4_TX_PIN             GPIO_Pin_10
#define STM32_COM4_TX_GPIO_PORT       GPIOC
#define STM32_COM4_TX_GPIO_CLK        RCC_APB2Periph_GPIOC
#define STM32_COM4_RX_PIN             GPIO_Pin_11
#define STM32_COM4_RX_GPIO_PORT       GPIOC
#define STM32_COM4_RX_GPIO_CLK        RCC_APB2Periph_GPIOC
#define STM32_COM4_IRQn               UART4_IRQn

__IO uint32_t TimingDelay;

USART_TypeDef* COM_USART[COMn] = {STM32_COM1, STM32_COM2,STM32_COM4}; 
GPIO_TypeDef* COM_TX_PORT[COMn] = {STM32_COM1_TX_GPIO_PORT, STM32_COM2_TX_GPIO_PORT, STM32_COM4_TX_GPIO_PORT};
GPIO_TypeDef* COM_RX_PORT[COMn] = {STM32_COM1_RX_GPIO_PORT, STM32_COM2_RX_GPIO_PORT, STM32_COM4_RX_GPIO_PORT};
const uint32_t COM_USART_CLK[COMn] = {STM32_COM1_CLK, STM32_COM2_CLK, STM32_COM4_CLK};
const uint32_t COM_TX_PORT_CLK[COMn] = {STM32_COM1_TX_GPIO_CLK, STM32_COM2_TX_GPIO_CLK, STM32_COM4_TX_GPIO_CLK};
const uint32_t COM_RX_PORT_CLK[COMn] = {STM32_COM1_RX_GPIO_CLK, STM32_COM2_RX_GPIO_CLK, STM32_COM4_RX_GPIO_CLK};
const uint16_t COM_TX_PIN[COMn] = {STM32_COM1_TX_PIN, STM32_COM2_TX_PIN, STM32_COM4_TX_PIN};
const uint16_t COM_RX_PIN[COMn] = {STM32_COM1_RX_PIN, STM32_COM2_RX_PIN, STM32_COM4_RX_PIN};

//私有函数
static void	COM_Init(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);
static void Delay_10us(u16 delay_count);
static void UART_Init(void);
static void NVIC_USARTConfig(void);
static void GPIO_Config(void);

typedef	struct
{
	int16_t i16DataLen;
	uint16_t u16XmitBusy;
	uint8_t *pBuff;
}_SCISendDataDef;

static volatile _SCISendDataDef	g_st_UsartSendData = 0;




/*******************************************************************************
* Function Name  : void Delay_us(UINT32 delay_count)
* Description    : delay--ms
* Input          : UINT32 delay_count 6000=1mS @48MHz
* Output         : None
* Return         : None
*******************************************************************************/

void Delay_10us(u16 delay_count)
{
	volatile u16 i,j;
	for (i=0; i<delay_count; i++) 
	{
		for (j = 0; j< 100; j++);
	}
}

/**-------------------------------------------------------
  * @函数名 COM_Init
  * @功能   对STM32的USART初始化底层函数
  * @参数1  COM1  对应STM32的USART1 对应开发板上串口1
  *         COM2  对应STM32的USART2 对应开发板上串口2
  * @参数2  指向一个成员已赋值USART_InitTypeDef结构体的指针
  * @返回值 无
***------------------------------------------------------*/
void COM_Init(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIO clock */
    /* 使能STM32的USART对应GPIO的Clock时钟 */
    RCC_APB2PeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM] | RCC_APB2Periph_AFIO, ENABLE);

    if (COM == COM1)
    {
        /* 使能STM32的USART1的Clock时钟 */
        RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE); 
    }
    else
    {
        /* Enable the USART2 Pins Software Remapping */
        /* 使能STM32的USART2的管脚重映射 */
//        GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
        /* 使能STM32的USART的Clock时钟 */
        RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
    }

    /* Configure USART Tx as alternate function push-pull */
    /* 初始化STM32的USART的TX管脚，配置为复用功能推挽输出 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

    /* Configure USART Rx as input floating */
    /* 初始化STM32的USART的RX管脚，配置为复用功能输入 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
    GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

    /* USART configuration */
    /* 根据传入的参数初始化STM32的USART配置 */
    USART_Init(COM_USART[COM], USART_InitStruct);

    /* Enable USART */
    /* 使能STM32的USART功能模块 */
    USART_Cmd(COM_USART[COM], ENABLE);
}





/**-------------------------------------------------------
  * 函数名 void METER485_WE(void)
  * 功能   RS485写使能打开
  * 参数   无
  * 返回值 无
***------------------------------------------------------*/
void BSP_METER485_WE(void)
{  
	GPIOA->BSRR |= 1<<15; 
}

/**-------------------------------------------------------
  * 函数名 void METER485_RD(void)
  * 功能   RS485设置为读取模式
  * 参数   无
  * 返回值 无
***------------------------------------------------------*/
void BSP_METER485_RD(void)
{
	GPIOA->BRR |= 1<<15; 
}

/**-------------------------------------------------------
  * @函数名 void NVIC_USARTConfig(void)
  * @功能   配置UART中断向量参数函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/

void NVIC_USARTConfig(void)
{ 
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	// Enable the UART1 gloabal Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	
}

/**-------------------------------------------------------
  * @函数名 GPIO_Config
  * @功能   端口设置
  * @参数1  
  *         
  *			
  * @参数2  
  * @返回值 无
***------------------------------------------------------*/
void GPIO_Config(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure;

	//COM1--METER
	//WE口配置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	BSP_METER485_RD();

}


/**-------------------------------------------------------
  * @函数名 UART_Init
  * @功能   UART初始化函数
  * @参数1  
  *         
  *			
  * @参数2  
  * @返回值 无
***------------------------------------------------------*/
void UART_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
	//GPIO_InitTypeDef  GPIO_InitStructure;

		//COM1--METE
	USART_InitStructure.USART_BaudRate = USART_BAUDRATE1;              //串口的波特率，例如115200 最高达4.5Mbits/s
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据字长度(8位或9位)
	USART_InitStructure.USART_StopBits = USART_StopBits_1;      //可配置的停止位-支持1或2个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;         //无奇偶校验  
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //双工模式，使能发送和接收
	COM_Init(COM1, &USART_InitStructure);  // 调用STM32的USART初始化底层函数
	COM_Init(COM2, &USART_InitStructure);  //

	//USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);	//中断配置


	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);				//中断配置
	
	USART_ITConfig(USART1,USART_IT_TXE,DISABLE);				//中断配置
	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);
	

}

/**-------------------------------------------------------
  * @函数名 BSP_COM_Init
  * @功能   面向用户的USART初始化函数
  * @参数1  
  *         
  *			
  * @参数2  
  * @返回值 无
***------------------------------------------------------*/
void BSP_COM_Init(void)
	{
		UART_Init();
		GPIO_Config();
		NVIC_USARTConfig();
	}


/*===========================================================================*
 *  函数名称：BSP_METER485SendData(u8 *Data,u8 SendNum)
 *  功能描述：METER发送数据函数
 *  用法说明：
 *  输入参数：*Data--需要发送的数据缓存  u8 SendNum--发送的数据长度
 *  输出参数：
 *  设计人员：
 *  创建日期：
 *==========================================================================*/
 void BSP_METER485SendData(u8 *Data,u8 SendNum)
{
/*
	u8 i;

	BSP_METER485_WE();		
	Delay_10us(10);
	for(i=0; i<SendNum; i++)
	{
		USART_SendData(USART1, Data[i]);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	}
	Delay_10us(10);
	BSP_METER485_RD();
   */     
	//*****************************************

	if (SendNum < 3)
	{
		return;
	}
	
	if (g_st_UsartSendData.u16XmitBusy == FALSE)
	{
		g_st_UsartSendData.pBuff = Data;
		g_st_UsartSendData.i16DataLen = SendNum;
		g_st_UsartSendData.u16XmitBusy = TRUE;

		BSP_METER485_WE();
		USART_ClearFlag(USART1, USART_FLAG_TC);
		Delay_10us(10);
		USART_SendData(USART1, *g_st_UsartSendData.pBuff++);
		g_st_UsartSendData.i16DataLen--;
	}
}

void BSP_Usart1XmitFcb(void)
{
	if (g_st_UsartSendData.i16DataLen > 0)
	{
		USART_SendData(USART1, *g_st_UsartSendData.pBuff++);
		g_st_UsartSendData.i16DataLen--;
	}
	else
	{
		g_st_UsartSendData.u16XmitBusy = FALSE;
		BSP_METER485_RD();
	}
}

        

void BSP_SocLightSendData(u8 *Data,u8 SendNum)
{
	u8 i;
	Delay_10us(10);
	for(i=0; i<SendNum; i++)
	{
	USART_SendData(USART2, Data[i]);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	}
	
}

/*
    加入以下代码,支持printf函数,不需要选择use MicroLIB	  
*/

#ifndef MicroLIB
// #pragma import(__use_no_semihosting)             //没有实现fgetc时需要声明该参数   
// 标准库需要的支持函数 使用printf()调试打印不需要实现该函数                
typedef struct  
{ 
	int handle; 
    // Whatever you require here. If the only file you are using is     
    // standard output using printf() for debugging, no file handling     
    // is required. 
}__FILE; 


FILE __stdout;

//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x)
{ 
	x = x; 
} 
//重定义fputc函数 如果使用MicroLIB只需要重定义fputc函数即可  
int fputc(int ch, FILE *f)
{
    // Place your implementation of fputc here
    //Loop until the end of transmission 
    while (USART_GetFlagStatus(STM32_COM2, USART_FLAG_TC) == RESET)
    {}

    // e.g. write a character to the USART 
    USART_SendData(STM32_COM2, (uint8_t) ch);

    return ch;
}
/*
可以直接使用putchar
不需要再定义 int putchar(int ch)，因为stdio.h中有如下定义
 #define putchar(c) putc(c, stdout)
*/

int ferror(FILE *f) {  
    // Your implementation of ferror  
    return EOF;  
} 
#endif 

FILE __stdin;

int fgetc(FILE *fp)
{
    int ch = 0;
	
    while(USART_GetFlagStatus(STM32_COM2, USART_FLAG_RXNE) == RESET)
    {
      
      
    }

    ch = (int)STM32_COM2->DR & 0xFF;
	
    putchar(ch); //回显
	
	return ch;
}


#ifdef  USE_FULL_ASSERT
// 需要在工程设置Option(快捷键ALT+F7)C++属性页的define栏输入"USE_FULL_ASSERT"
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
    // User can add his own implementation to report the file name and line number,
    //   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line)
     
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);

    // Infinite loop
    while (1)
    {
      
    }
}
#endif




