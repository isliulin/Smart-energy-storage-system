
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H
#define __ADC_H	

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x.h"

#define NUM_AD_CHANNEL  11//ADC1的通道数
typedef enum
{

	AD0_AC_VoltA = 0,	//Vfuse_AD
	AD1_AC_VoltB = 1,	//Vout_AD
	AD4_AC_VoltC = 2,	//Vbat_AD
}_ADsample;


typedef struct
{
	int16_t VoltageA;
	int16_t VoltageB;
	int16_t VoltageC;
	int16_t CurrentA;
	int16_t CurrentB;
	int16_t CurrentC;
}_AC_SampleValue;

extern _AC_SampleValue		AC_SampleValue;
//extern  int16_t 	AD_DATA[NUM_AD_CHANNEL];


void	BSP_ADC_Init(void);
void 	BSP_Int_SamplingData(void);

#ifdef __cplusplus
}
#endif

#endif


