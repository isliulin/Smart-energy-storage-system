#include "stm32f10x.h"
//#include "dma.h"



 u8 DMA1_MEM_LEN;
//DMA1的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从外设模式->存储器/16位数据宽度/存储器增量模式
                  //DMA_CHx:DMA通道CHx		   //cpar:外设地址 //cmar:存储器地址//cndtr:数据传输量
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{	  
    DMA_InitTypeDef DMA_InitStructure;

 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		//使能DMA时钟
	
    DMA_DeInit(DMA_CHx);   									//将DMA的通道1寄存器重设为缺省值

	DMA1_MEM_LEN=cndtr;										//通道数据长度
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  		//DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  			//DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  	//数据传输方向，从外设发送到内存  DMA_CCRX位4
	DMA_InitStructure.DMA_BufferSize = cndtr;  				//DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //外设数据宽度为16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; 		//内存数据宽度为16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  							//工作在循环缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; 						//DMA通道 x拥有高优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  								//DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA_CHx, &DMA_InitStructure);  									//根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	 
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
	
	DMA_Cmd(DMA1_Channel1, ENABLE);//启动DMA通道 	
}

/*===========================================================================*
 *  函数名称：void NVIC_DMAConfig(void)
 *  功能描述：DMA1中断配置函数
 *  用法说明：
 *  输入参数：
 *  输出参数：
 *  设计人员：
 *  创建日期：
 *==========================================================================*/



void NVIC_DMAConfig(void)
{ 
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		
}



	  
