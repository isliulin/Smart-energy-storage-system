
#include "STM32F107_BSP_io.h"


/*
* 函数名：BSP_Init
* 描述	：初始化
* 输入	：void
* 输出	: void
*/


void Delay_1us(u16 delay_count)
{
	volatile	u16 i,j;
	for (i=0; i< delay_count; i++)
	{
		for (j = 0; j< 72; j++);
	}
}



