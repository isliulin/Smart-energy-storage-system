/*****************************************************************************
  * @file    main.c
  * @author  Store Energy R&D Team
  * @version V1.1.0
  * @date    07-October-2017
  * @brief   Main program body
  *****************************************************************************/

#include "main.h"

#include <stdio.h>
#include <String.h>
#include <Assert.h>
#include "Publics.h"



void OpenAllLED()
{
	BSP_Relay_Close(IDLE_LED);//点亮待机灯
	BSP_Relay_Close(ONLINE_LED); //点亮并网灯
	BSP_Relay_Close(OFFLINE_LED); //点亮离网灯
	BSP_Relay_Close(FAULT_LED); //点亮故障灯
        
    BSP_Relay_Close(FAN1_RELAY);//打开风机
	BSP_Relay_Close(FAN2_RELAY);//打开风机


}

void CloseAllLED()
{
	BSP_Relay_Open(IDLE_LED);//点亮待机灯
	BSP_Relay_Open(ONLINE_LED); //点亮并网灯
	BSP_Relay_Open(OFFLINE_LED); //点亮离网灯
	BSP_Relay_Open(FAULT_LED); //点亮故障灯



}


int main(void)
{
	
    //--------驱动层初始化-------
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2800);	
    //static int16_t s_cnt0=0,s_cnt1=0;    

    BSP_Init();
	
	objPcsConfigManage.ReadConfigInfo();	//从EEPROM读PCS柜配置信息
    objPcuModel.Dat_Init();
	objPcsConfigManage.PCSConfigSchedule();	//配置管理模块任务
	
	OpenAllLED();//for test

	//计算主循环时间
	volatile int32_t WhileStartCnt = 0;
	volatile int32_t WhileEndCnt = 0;
	volatile int32_t WhileCnt = 0;

	Delay_10us(1000000);

	CloseAllLED();//for test

//	ReadRTCTime();//for test
//	objEEPROM.App_WriteNumBytes(EPOERRORADDR, (uint8_t*)(&Calendar),sizeof(_CALENDAR));//EPO错误写入EEROM
//	ObjPcuOnOffCtrl.ClearEPOError();
	
	while(1)
	{
        
		WhileEndCnt = g_u16_Timer50usCnt;
		WhileCnt = (WhileEndCnt - WhileStartCnt + 40000)%40000;
		objCanDataExchg.m_i16_Timer = WhileCnt;
		WhileStartCnt = g_u16_Timer50usCnt;
		
		BSP_Timer_FlagMaintain();
	//	ReadRTCTime();
		BSP_DI_Filter();

		objPcuModel.SystemSchedule();
        
	}
	return 0;
}


void BSP_Init(void)
{
	//通信端口初始化	
	BSP_CanInit();
	BSP_I2C_EepromInit();
//	BSP_RTC_Init();			//@todo删除硬件计时
	BSP_COM_Init();
	BSP_ADC_Init();
	NVIC_DMAConfig();
	BSP_TimerInit();
	BSP_SPI_Init();

	//输入端口初始化
	BSP_DI_Init(DI_FANHOT_FB);	//J59-1
	BSP_DI_Init(DI_EPO_FB);	//J59-2
	BSP_DI_Init(DI_PROTECT_FB);	//J59-3
	BSP_DI_Init(DI_ACBREAK_FB);    //J59-4
	BSP_DI_Init(DCBREAK1_FB);		//J59-5
	BSP_DI_Init(DCBREAK2_FB);	//J59-6
	BSP_DI_Init(DCBREAK3_FB);		//J59-7
	BSP_DI_Init(DCBREAK4_FB);		//J59-8



	BSP_IO_Dat_Init();
/*
	//LED初始化	
	BSP_LED_Init(LED_RUN);
	//BSP_LED_Init(LED_WARN);
	BSP_LED_Init(LED_FAULT);
	  
	BSP_LED_Off(LED_RUN);
	//BSP_LED_Off(LED_WARN);
	BSP_LED_Off(LED_FAULT);
*/
	//output继电器初始化
	BSP_Relay_Init(FAN1_RELAY);	//J52-2
	BSP_Relay_Init(FAN2_RELAY); //J52-3
	BSP_Relay_Init(IDLE_LED);	//J60-2
	BSP_Relay_Init(ONLINE_LED); //J60-3
	BSP_Relay_Init(OFFLINE_LED);	//J60-4
	BSP_Relay_Init(FAULT_LED);		//J60-5

	BSP_Relay_Open(FAN1_RELAY);      //J52-2
	BSP_Relay_Open(FAN2_RELAY);	//J52-3
	BSP_Relay_Open(IDLE_LED);	//J60-2
	BSP_Relay_Open(ONLINE_LED);	//J60-3
	BSP_Relay_Open(OFFLINE_LED); //J60-4
	BSP_Relay_Open(FAULT_LED);	//J60-5

}


