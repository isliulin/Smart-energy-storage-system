
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H
#define __ADC_H	

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x.h"

void	BSP_ADC_Init(void);
void 	BSP_Int_SamplingData(void);
void Delay_1us(u16 delay_count);

#ifdef __cplusplus
}
#endif

#endif


