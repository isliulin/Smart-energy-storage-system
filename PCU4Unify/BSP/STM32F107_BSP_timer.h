
#ifndef __TIMER_H
#define __TIMER_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"



typedef struct 
{
	uint16_t	b50us					:1;					//BIT0  50us时间到
	uint16_t	b1ms					:1;					//BIT0  1:100ms时间到
	uint16_t	b4ms					:1;					//BIT0  1:100ms时间到
	uint16_t	b10ms					:1;					//BIT2	1:20ms时间到
	uint16_t	b100ms					:1;					//BIT6	1:1ms时间到
	uint16_t	b500ms					:1;					//BIT6	1:1ms时间到
	uint16_t	b1s						:1;					//BIT0
	uint16_t	b10s					:1;					//BIT0
	uint16_t	bPowerOn5sEnd			:1;					//BIT0
	uint16_t	bResv					:7;				

}_TimerFlagDef;

#define	Fint		40000									//int frequency is 20K,the times of enter int in 1s
#define	K1sCnst		20000									//0.1*200000 = 20000
#define	K500msCnst	10000									//0.1*200000 = 10000
#define	K100msCnst	2000									//0.1*20000 = 2000
#define	K40msCnst	800										//0.04*20000 = 800
#define	K20msCnst	400										//0.02*20000 = 400									
#define	K14msCnst	280										//0.014*20000 = 200
#define	K10msCnst	200										//0.01*20000 = 200
#define	K4msCnst 	80										//0.001*20000 = 20
#define	K1msCnst 	20										//0.001*20000 = 20
#define	K50usCnst 	1										//0.00005*20000 = 1


#define	PWMMAXCNT		10									
extern _TimerFlagDef	g_st_TimerFlag;
extern uint16_t	g_u16_Timer50usCnt;

void 	BSP_TimerInit(void);
void 	Timer_Tick_Maintain(void);
void	BSP_Timer_FlagMaintain(void);
void	BSP_PowerOn5sDelay(void);

#ifdef __cplusplus
}
#endif

#endif


