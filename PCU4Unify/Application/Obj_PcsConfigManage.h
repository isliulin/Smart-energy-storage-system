/*==========================================================*/
/*Copyright (C), 2015-2017, XIAN TGOOD Tech. Co., Ltd.		*/
/* Title		:	Obj_PcsConfigManage.h					*/
/* Description	:	Class_PCSConfigManage definition		*/
/* Author		:	Hexianguo								*/
/* Date			:	2017.12.14								*/
/*==========================================================*/
#ifndef PCS_ConfigManage_H
#define PCS_ConfigManage_H

#include "Publics.h"

//#define	EEP_BIOCKS		((int16_t)16)
//#define	BIOCK_SIZE		((int16_t)64)
//uint8_t PcsConfig1[80];	//从EEPROM读出配置管理信息，存储到数组，62Byte
//uint8_t PcsConfig2[80];	//备份1
//uint8_t PcsConfig3[80];	//备份2

#define	 AC_MODULE_NUM_MAX 8	//系统目前8个模块
#define	 DC_MODULE_NUM_MAX 8
#define	 AC_MAJOR_NUM_MAX  8
#define  DC_MAJOR_NUM_MAX  8
#define  BMS_NUM_MAX  4			//4路电池

#define ACMODULE_ADDR_OFFSET   12	//AC模块地址偏移
#define DCMODULE_ADDR_OFFSET   28	//DC模块地址偏移
#define BMS_ADDR_OFFSET   	   44	//BMS地址偏移


#define ALTITUDE_ADDR_OFFSET  52	//海拔地址偏移


#define	 PGN_PCS_CONFIG			0xC400		//PCS柜设置

#define	   ParamAddrBase		0x0400			//EEPROM存储配置管理参数首地址 1024开始（1k~2k）
#define	   BackUpBaseAddr1		0x0800			//EEPROM备份首地址1 (2k~3k)
#define	   BackUpBaseAddr2		0x0C00			//EEPROM备份首地址2 (3k~4k)

#define	   EPOTimeBaseAddr		0x1000			//存储EPO发生时间，上传到SCU历史记录 (4k~)

#define    EPOERRORADDR			4096			//EPO错误时间存储地址


//配置管理参数存储首地址
#define	   PCU_ADDR					(ParamAddrBase + 0x00)		//PCU地址
#define	   FREQUENCY_ADDR			(ParamAddrBase + 0x01)		//频率
#define    CABINET_TYPE_ADDR   		(ParamAddrBase + 0x02)		//单并柜模式
#define	   ALTITUDE_RANGE_ADDR		(ParamAddrBase + 0x03)		//海拔 1字节  01：<4000米 02：>4000

#define    AC_METER_ADDR     		(ParamAddrBase + 0x04)		//是否有交流电表
#define    DC_METER_ADDR     		(ParamAddrBase + 0x05)		//是否有直流电表
#define    ACMETER_BAUD_ADDR  		(ParamAddrBase + 0x06)		//AC电表通讯波特率	1字节
#define    DCMETER_BAUD_ADDR  		(ParamAddrBase + 0x07)		//DC电表通讯波特率	1字节

#define    AC_MODE_NUM_ADDR     	(ParamAddrBase + 0x08)		//ACDC个数
#define    DC_MODE_NUM_ADDR   		(ParamAddrBase + 0x09)		//DCDC个数
#define    MODE_GROUP_NUM_ADDR    	(ParamAddrBase + 0x0A)		//模块组数
#define    MODE_LEVEL_ADDR     		(ParamAddrBase + 0x0B)		//模块级数：一级、二级

#define    ACMODE_ATTRIBUTE_ADDR    (ParamAddrBase + 0x0C)		//AC模块地址属性 16字节
#define    DCMODE_ATTRIBUTE_ADDR    (ParamAddrBase + 0x1C)		//DC模块地址属性 16字节

#define    BMS_ATTRIBUTE_ADDR    	(ParamAddrBase + 0x2C)		//BMS地址属性 8字节

#define	   VOLT_OUT_ADDR			(ParamAddrBase + 0x34)		//输出电压 2字节
#define	   ACMETER_COM_PARA_ADDR	(ParamAddrBase + 0x36)		//AC电表通讯参数:起始位、数据位、校验位、停止位 2字节
#define	   DCMETER_COM_PARA_ADDR	(ParamAddrBase + 0x38)		//DC电表通讯参数	2字节

#define    SOFTWARE_VERSION_ADDR    (ParamAddrBase + 0x3A)     	//软件版本号 3字节
#define    HARDWARE_VERSION_ADDR    (ParamAddrBase + 0x3D)     	//硬件版本号 1字节

#define    UART1_BAUD_ADDR		    (ParamAddrBase + 0x3E)     	//uart1波特率 1字节
#define    UART2_BAUD_ADDR		    (ParamAddrBase + 0x3F)     	//uart2波特率 1字节



//配置管理参数备份首地址1
#define	   PCU_ADDR1				(BackUpBaseAddr1 + 0x00)		//PCU地址
#define	   FREQUENCY_ADDR1			(BackUpBaseAddr1 + 0x01)		//频率
#define    CABINET_TYPE_ADDR1   	(BackUpBaseAddr1 + 0x02)		//单并柜模式
#define	   ALTITUDE_RANGE_ADDR1		(BackUpBaseAddr1 + 0x03)		//海拔 1字节  01：<4000米 02：>4000

#define    AC_METER_ADDR1     		(BackUpBaseAddr1 + 0x04)		//是否有交流电表
#define    DC_METER_ADDR1     		(BackUpBaseAddr1 + 0x05)		//是否有直流电表
#define    ACMETER_BAUD_ADDR1  		(BackUpBaseAddr1 + 0x06)		//AC电表通讯波特率	1字节
#define    DCMETER_BAUD_ADDR1  		(BackUpBaseAddr1 + 0x07)		//DC电表通讯波特率	1字节

#define    AC_MODE_NUM_ADDR1    	(BackUpBaseAddr1 + 0x08)		//ACDC个数
#define    DC_MODE_NUM_ADDR1   		(BackUpBaseAddr1 + 0x09)		//DCDC个数
#define    MODE_GROUP_NUM_ADDR1    	(BackUpBaseAddr1 + 0x0A)		//模块组数
#define    MODE_LEVEL_ADDR1     	(BackUpBaseAddr1 + 0x0B)		//模块级数：一级、二级

#define    ACMODE_ATTRIBUTE_ADDR1   (BackUpBaseAddr1 + 0x0C)		//AC模块地址属性 16字节
#define    DCMODE_ATTRIBUTE_ADDR1   (BackUpBaseAddr1 + 0x1C)		//DC模块地址属性 16字节

#define    BMS_ATTRIBUTE_ADDR1    	(BackUpBaseAddr1 + 0x2C)		//BMS地址属性 8字节

#define	   VOLT_OUT_ADDR1			(BackUpBaseAddr1 + 0x34)		//输出电压 2字节
#define	   ACMETER_COM_PARA_ADDR1	(BackUpBaseAddr1 + 0x36)		//AC电表通讯参数:起始位、数据位、校验位、停止位 2字节
#define	   DCMETER_COM_PARA_ADDR1	(BackUpBaseAddr1 + 0x38)		//DC电表通讯参数	2字节

#define    SOFTWARE_VERSION_ADDR1   (BackUpBaseAddr1 + 0x3A)     	//软件版本号 3字节
#define    HARDWARE_VERSION_ADDR1   (BackUpBaseAddr1 + 0x3D)     	//硬件版本号 1字节

#define    UART1_BAUD_ADDR1		    (BackUpBaseAddr1 + 0x3E)     	//uart1波特率 1字节
#define    UART2_BAUD_ADDR1		    (BackUpBaseAddr1 + 0x3F)     	//uart2波特率 1字节



//配置管理参数备份首地址2
#define	   PCU_ADDR2				(BackUpBaseAddr2 + 0x00)		//PCU地址
#define	   FREQUENCY_ADDR2			(BackUpBaseAddr2 + 0x01)		//频率
#define    CABINET_TYPE_ADDR2   	(BackUpBaseAddr2 + 0x02)		//单并柜模式
#define	   ALTITUDE_RANGE_ADDR2		(BackUpBaseAddr2 + 0x03)		//海拔 1字节  01：<4000米 02：>4000

#define    AC_METER_ADDR2     		(BackUpBaseAddr2 + 0x04)		//是否有交流电表
#define    DC_METER_ADDR2     		(BackUpBaseAddr2 + 0x05)		//是否有直流电表
#define    ACMETER_BAUD_ADDR2  		(BackUpBaseAddr2 + 0x06)		//AC电表通讯波特率	1字节
#define    DCMETER_BAUD_ADDR2  		(BackUpBaseAddr2 + 0x07)		//DC电表通讯波特率	1字节

#define    AC_MODE_NUM_ADDR2     	(BackUpBaseAddr2 + 0x08)		//ACDC个数
#define    DC_MODE_NUM_ADDR2   		(BackUpBaseAddr2 + 0x09)		//DCDC个数
#define    MODE_GROUP_NUM_ADDR2    	(BackUpBaseAddr2 + 0x0A)		//模块组数
#define    MODE_LEVEL_ADDR2     	(BackUpBaseAddr2 + 0x0B)		//模块级数：一级、二级

#define    ACMODE_ATTRIBUTE_ADDR2   (BackUpBaseAddr2 + 0x0C)		//AC模块地址属性 16字节
#define    DCMODE_ATTRIBUTE_ADDR2   (BackUpBaseAddr2 + 0x1C)		//DC模块地址属性 16字节

#define    BMS_ATTRIBUTE_ADDR2   	(BackUpBaseAddr2 + 0x2C)		//BMS地址属性 8字节

#define	   VOLT_OUT_ADDR2			(BackUpBaseAddr2 + 0x34)		//输出电压 2字节
#define	   ACMETER_COM_PARA_ADDR2	(BackUpBaseAddr2 + 0x36)		//AC电表通讯参数:起始位、数据位、校验位、停止位 2字节
#define	   DCMETER_COM_PARA_ADDR2	(BackUpBaseAddr2 + 0x38)		//DC电表通讯参数	2字节

#define    SOFTWARE_VERSION_ADDR2   (BackUpBaseAddr2 + 0x3A)     	//软件版本号 3字节
#define    HARDWARE_VERSION_ADDR2   (BackUpBaseAddr2 + 0x3D)     	//硬件版本号 1字节

#define    UART1_BAUD_ADDR2		    (BackUpBaseAddr2 + 0x3E)     	//uart1波特率 1字节
#define    UART2_BAUD_ADDR2		    (BackUpBaseAddr2 + 0x3F)     	//uart2波特率 1字节



//EPO 历史记录时间
#define	   EPO_TIME_ADDR			(EPOTimeBaseAddr + 0x00)		


typedef enum   
{
	PCS_ConfigPara1 = 1,
	PCS_ConfigPara2,
	PCS_ConfigPara3,
	PCS_ConfigPara4,
	PCS_ConfigPara5,
	PCS_ConfigPara6,
	PCS_ConfigPara7,
	PCS_ConfigPara8,
	PCS_ConfigPara9,
	PCS_ConfigPara10,
	PCS_ConfigPara11,
	
}_PCS_CONFIG_CMD;


//Master Monitor  应用层数据包
typedef struct
{
    uint16_t  u16PGN;
    uint16_t  u16Priority;
    uint16_t  u16DataLen;
	uint16_t  u16MsgSendPeriod;
	uint16_t  u16DestId;
	uint16_t  u16MACId;
    uint16_t  u16XmitActive;
    //uint16_t  RECVFresh;
    uint16_t   u16offset;
    uint8_t*   pDataPtr;
}_CanMstMonAppMsgDef;

typedef struct					//软硬件版本号
{
	
	uint8_t SoftVersion[3];
	uint8_t HardVersion;
	
}_VersionManageDef;


typedef struct
{
	uint8_t PCUAddr;					//PCU地址
	uint8_t Frequency;					//频率50Hz	
	uint8_t CabinetType;				//01：单柜，02：并柜
	uint8_t AltitudeRange;				//海拔范围：临界4000
	
	uint8_t ACMeter;					//00：无交流电表，01：厂家1；02：厂家2，03：厂家3...
	uint8_t DCMeter;					//00：无直流电表，01：厂家1；02：厂家2，03：厂家3...	
	uint8_t ACMeterBaudRate;			//电表通讯波特率
	uint8_t DCMeterBaudRate;	
	
	uint8_t ACModuleNum;				//AC模块个数
	uint8_t DCModuleNum;				//DC模块个数
	uint8_t ModeGroupNum;				//模块组数
	uint8_t ModuleLevel;				//模块级数：单级（AC）两级（ACDC）
	
	uint16_t ACModeAddrAttribute[AC_MODULE_NUM_MAX];	//AC模块地址属性 高字节高4位主从标志:0表示从模块,1主模块,高字节低4位组标志;低字节:模块地址
	uint16_t DCModeAddrAttribute[DC_MODULE_NUM_MAX];	//DC模块地址属性
	
	uint16_t BMSAddrAttribute[BMS_NUM_MAX];				//BMS地址属性
		
	
	uint16_t VoltOut;						//输出电压380V
	uint16_t ACMeterComPara;				//电表通讯参数:起始位、数据位、校验位、停止位
	uint16_t DCMeterComPara;

	uint8_t SoftVersion[3];					//软硬件版本号
	uint8_t HardVersion;
	uint8_t BaudRateUart1;					//两路485通讯波特率
	uint8_t BaudRateUart2;
}_ConfigManageDef;	//配置管理(64 Byte) Max Pagewrite 64Byte


typedef struct
{
	uint8_t PCUAddr;					//PCU地址
	uint8_t Frequency;					//频率50Hz	
	uint8_t CabinetType;				//01：单柜，02：并柜
	uint8_t AltitudeRange;				//海拔范围：临界4000
	
	uint8_t ACMeter;					//00：无交流电表，01：厂家1；02：厂家2，03：厂家3...
	uint8_t DCMeter;					//00：无直流电表，01：厂家1；02：厂家2，03：厂家3...	
	uint8_t ACMeterBaudRate;			//电表通讯波特率
	uint8_t DCMeterBaudRate;	
	
	uint8_t ACModuleNum;				//AC模块个数
	uint8_t DCModuleNum;				//DC模块个数
	uint8_t ModeGroupNum;				//模块组数
	uint8_t ModuleLevel;				//模块级数：单级（AC）两级（ACDC）

	uint8_t ACMajorNum;					//AC主模块个数
	uint8_t DCMajorNum;					//DC主模块个数
	
	uint8_t ACMajorAddr[AC_MAJOR_NUM_MAX];	//AC主机地址
	uint8_t DCMajorAddr[DC_MAJOR_NUM_MAX];	//DC主机地址
	
	uint8_t ACModeAddr[AC_MODULE_NUM_MAX];	//AC模块地址 
	uint8_t DCModeAddr[DC_MODULE_NUM_MAX];	//DC模块地址
	
	uint8_t BMSAddr[BMS_NUM_MAX];	//BMS地址
	
	uint16_t VoltOut;						//输出电压380V
	uint16_t ACMeterComPara;				//AC电表通讯参数:起始位、数据位、校验位、停止位
	uint16_t DCMeterComPara;				//	

	uint8_t SoftVersion[3];					//软硬件版本号
	uint8_t HardVersion;
	uint8_t BaudRateUart1;					//两路485通讯波特率
	uint8_t BaudRateUart2;
}_GetConfigParameterDef;	//获取配置信息(对外接口结构体72 Byte)

class Class_PCSConfigManage
{
	private:
			
	public:

		uint8_t PcsConfig1[80];	//从EEPROM读出配置管理信息，存储到数组，64Byte
		uint8_t PcsConfig2[80];	//备份1
		uint8_t PcsConfig3[80];	//备份2	
		_VersionManageDef	m_VersionManage;
		_ConfigManageDef	m_st_ConfigManage;
		_GetConfigParameterDef	m_st_GetConfigPara;
		_GetConfigParameterDef	m_st_ConfigParaBack1;
		_GetConfigParameterDef	m_st_ConfigParaBack2;

	
	public:
	
	void	PCU_Version_DatInit(void);	//PCU版本初始化
	void	ConfigManageDat_Init(); //目前直接初始化配置管理结构体
	void	DefaultConfigDat_Init();	//默认配置信息
	void	RecvMstMonMsgDispatch(_CanMstMonAppMsgDef *pApdu);	//解析集控下发配置信息
	void	WriteConfigInfo(void);
	void	ReadConfigInfo(void);
	void	GetPcsConfigInfo(_GetConfigParameterDef *pData);
	void	PCSConfigSchedule(void);
	
};


#endif
//============================================================
// End of file.
//============================================================


