
#ifndef _PUBLICS_H_
#define _PUBLICS_H_

//-------------常用宏定义------------------------------
#ifndef IN
	#define	IN
#endif

#ifndef OUT
	#define	OUT
#endif

#ifndef INOUT
	#define	INOUT
#endif

#ifndef TRUE
    #define TRUE                1
#endif

#ifndef FALSE
    #define FALSE               0
#endif

#ifndef YES
    #define YES                 1
#endif

#ifndef NO
    #define NO                  0
#endif

#ifndef NULL
	#define NULL 0//((void *)0)
#endif
#ifndef RET_OK
    #define RET_OK		((int16_t)1)
#endif

#ifndef RET_ERR
    #define RET_ERR		((int16_t)0)
#endif

#define CHAR_BIT 8
/***********************************************************************/
//实现位型数组
/***********************************************************************/
#define CHAR_BIT 8
#define BITMASK(b)		(1 << ((b) % CHAR_BIT))
#define BITSLOT(b)		((b) / CHAR_BIT)
#define BITSET(a, b)	(((a)[BITSLOT(b)]) |= (BITMASK(b)))

#define BITCLEAR(a, b)	(((a)[BITSLOT(b)]) &= (~(BITMASK(b))))
#define BITTEST(a, b)	((((a)[BITSLOT(b)]) & (BITMASK(b))) >> (b % 8))//若b为8号PDU，则本应该a[1]右移0位,却右移了8位，直接置0,故对8取余
#define BITNSLOTS(nb)	((nb + CHAR_BIT - 1) / CHAR_BIT)

//声明一个固定长度（50个bit）的位数组：	uint8_t bitarray[BITNSLOTS(50)]
//设置位数组中的某一位：				BITSET(bitarray, 24)
//清除位数组中的某一位：				BITCLEAR(bitarray, 24)
//检测位数组中的某一位：				if(BITTEST(bitarray, 24))

/***********************************************************************/

#define LEN_ARRAY(arr)    (sizeof(arr)/sizeof(*arr))
#define SIGN        (((x)<=0)? 0:1)
#define ABS(x)      (((x)>= 0)?(x):-(x))

#define	DownLimit16(Value,DownValue){Value = (Value <= DownValue) ? DownValue : Value;}
#define	UpLimit16(Value,UpValue){Value = (Value >= UpValue) ? UpValue : Value;}

#define UpDownLimited(Var,Max,Min) {(Var) = ((Var)>=(Max))?(Max):(Var);(Var) = ((Var)<=(Min))?(Min):(Var);}
#define FILTER(ik,iVar,iMean)   (((((int32_t)(iVar)<<16) - (iMean))>>(ik))+(iMean))

//浮点数采用IEEE32位标准浮点数格式（标准C语言浮点数格式）
#define	FloatToI32(fValue)	(*((int32_t*)&fValue))
#define	I32ToFloat(i32Value)	(*((float*)&i32Value))
#define	BIG_LITTLE_TRANVERSE32(x)	((((uint32_t)(x) & 0xff000000) >> 24) | (((uint32_t)(x) & 0x00ff0000) >> 8) | (((uint32_t)(x) & 0x0000ff00) << 8) | (((uint32_t)(x) & 0x000000ff) << 24))
					 					
#define	BIG_LITTLE_TRANVERSE16(x)	((((uint16_t)(x) & 0xff00) >> 8) | (((uint16_t)(x) & 0x00ff) << 8))
#define	BIG_LITTLE_TRANVERSE8(x)	((((uint8_t)(x) & 0xf0) >> 4) | (((uint8_t)(x) & 0x0f) << 4))


#define	VERSION_DEFINE_SOFTWARE(version,V,B,D)	{version += ((((uint32_t)V)%1000)*1000 + (B%10)*100 + D%100);}
#define	VERSION_DEFINE_HARDWARE(version,A)	{version += ((((uint32_t)A)%10000)*1000000);}


#define		S2U8(x)		(*((volatile uint8_t*)(&(x))))
#define		S2U16(x)	(*((volatile uint16_t*)(&(x))))
#define		S2U32(x)	(*((volatile uint32_t*)(&(x))))

#define	VER_V(num)	(num)
#define	VER_B(num)	(num)
#define	VER_D(num)	(num)
#define	VER_A(num)	(num)


//PDU个数
#define	PDU_NUM				32
#define	HVDC_NUM				8
#define	CCU_NUM				1
#define 	SWITCH_NUM			((uint16_t)10)
#define 	ALONE_ACDC_NUM_MAX	((uint16_t)4)
#define	PV_CNTRL_NUM_MAX		((uint16_t)7)
#define		CCU_MAX_NUM		10
#define		PDU_MAX_NUM		32


//-----系统文件--------------------------------------
#include <String.h>
#include <stdio.h>


//-------BSP-----------------------------------------
#include "stm32f10x.h"
#include "STM32F107_BSP_dma.h"
#include "STM32F107_BSP_timer.h"
#include "STM32F107_BSP_led.h"
#include "STM32F107_BSP_can.h"
#include "STM32F107_BSP_adc.h"
#include "STM32F107_BSP_I2c.h"
#include "STM32F107_BSP_usart.h"
#include "STM32F107_BSP_io.h"
#include "flash.h"
#include "CRC.h"
#include "stm32f10x_flash.h"
//#include "STM32F107_BSP_spi.h"
#include "mcp2515_api.h"
//#include "stm32f10x_map.h"


//--------Application---------------------------------
#include "Obj_Utility.h"
#include "Obj_EEPROM.h"
#include "Obj_Memory.h"
#include "Obj_BMS.h"
#include "Obj_RS485App.h"
//#include "Obj_AnalogData.h"
#include "Obj_PcuModel.h"
#include "Obj_PcsConfigManage.h"
#include "Obj_FailManager.h"
#include "Obj_CanDataPool.h"
#include "Obj_CanQueue.h"
#include "Obj_CanDataExchg.h"
#include "Obj_CanBMSProtocol.h"
#include "Obj_CanMstMonProtocol.h"
#include "Obj_CanInnerProtocol.h"
#include "Obj_CanApp.h"
#include "Obj_CanMstMonLongFrame.h"
#include "Obj_EnergyManage.h"
#include "Obj_ModbusProtocol.h"
#include "Obj_UpgradeIntf.h"
#include "Obj_SystemRTClock.h"
#include "Obj_StoredEnergy.h"
#include "Obj_SensorCtrl.h"
#include "Obj_EMMonitor.h"
#include "Obj_PCUOnOffControler.h"
#include "Obj_ChargeControler.h"



//---------对象声明----------------------------------------

//extern  Class_PCUControler		objPCUControler;
extern	 Class_Utility		        objUtility;
extern	 Class_PcuModel	                objPcuModel;
extern  Class_PCSConfigManage		objPcsConfigManage;
extern  Class_EEPROM 		   	objEEPROM;
extern  Class_Memory		   	objMemory;
extern	 Class_BMS			objBMS;
//extern  Class_AnalogData	        objAnalogData;
extern  Class_EnergyManage		objEnergyManage;
extern  Class_CanDataPool   	        objCanDataPool;
extern  Class_CanDataExchg   	        objCanDataExchg;
extern  Class_CanBMSProtocol	        objCanBMSProtocol;
extern  Class_CanInnerProtocol	        objCanInnerProtocol;
extern  Class_CanApp			objCanApp;
extern	Class_FailManager		objFailManager;
extern	Class_Upgrade			objUpgrade;
extern Class_CanMasterMonPrtcl 	objCanMasterMonPrtcl;
extern	Class_CanMstMonLongFrame	objCanMstMonLongFrame;
extern Class_SystemRTC 	        objSystemRTC;
extern Class_StoredEnergy 		objStoredEnergy; //储能柜
extern Class_ModBusProtocol  	         objModBusProtocol;
extern Class_Sensor                     objSensor;
extern Class_RS485App  	         objRS485App;
extern	Class_EMMonitor			 ObjEMMonitor;
extern Class_PCUOnOffControler          ObjPcuOnOffCtrl;//开关机模块
extern	Class_ChargeControler            ObjChargeCtrl;//充电模块




//****************************队列*************************************

#define	FULL_QUEUE		(0) //队列已满
#define	ENQUEUE_SUCEED	(1)	//入队成功
#define EMPTY_QUEUE		(2) //队列为空
#define EDQUEUE_SUCCEED (3) //出队成功
#define NOEMPTY_QUEUE	(4) //队列非空





//队列数据单元结构体
typedef struct
{
	uint16_t 	u16Addr;
	uint16_t	u16Len;
	int16_t		i16data;
	_DataBlockDef*	pDataBlock;
}_QueueDataDef;

/*
//队列数据单元结构体
typedef struct
{
	uint16_t 	u16Addr;
	uint16_t	u16Len;
	char		cdata;
	_DataBlockDef*	pDataBlock;
}_QueueDataDef1;
*/

//队列结构体定义
struct	Seq_Queue
{
	uint16_t	u16size;//队列长度
	int16_t 	head;//队头
	int16_t		end;//队尾
	int16_t		fence;
	_QueueDataDef* pQueueArray;//队列中数据
	Seq_Queue(_QueueDataDef* saveBuff, uint16_t ArrSize):u16size(ArrSize),head(0),end(0),fence(0),pQueueArray(saveBuff){};//队列结构体默认初始化，相当于类的构造函数
};
int16_t	Dat_deQueueSeq(struct Seq_Queue* paqu, _QueueDataDef* data);//出队，删除队列头部元素
int16_t	Dat_enQueueSeq(struct Seq_Queue* paqu, _QueueDataDef data);//入队
void 	Dat_GetQueueFenceData(struct Seq_Queue* paqu, _QueueDataDef* data);//从队列中取出fence指定的值
int16_t   Dat_GetQueueFlag(struct Seq_Queue* paqu);

uint16_t uiCRC16(uint8_t *puchMsg, uint16_t  uiDataLen);

#endif
