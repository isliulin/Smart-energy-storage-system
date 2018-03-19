
#ifndef _EMMonitor_H
#define _EMMonitor_H

#include "Publics.h"


#define MODDATA_UPLOADTIME 5
#define ACDC_MODID 20		//ACDC模块ID为20~39
#define DCDC_MODID 40		//DCDC模块ID为40~59
typedef union
{
	uint16_t u16Word1All;
	struct
	{		
		uint16_t bBrk1		:1;
		uint16_t bBrk2		:1;
		uint16_t bBrk3		:1;
		uint16_t bBrk4		:1;
		uint16_t bBrk5		:1;
		uint16_t bBrk6		:1;
		uint16_t bBrk7		:1;
		uint16_t bFireExt		:1;
		uint16_t bRsv		:8;
	}Bits;
}_PVDIStateFbDef;



#pragma pack(1)
typedef struct
{
	uint8_t	u8DCMeterAddr;
	int16_t	i16DCMeterVol;
	int16_t	i16DCMeterCur;
	int16_t	i16DCMeterPower;
	uint16_t	u16DCMeterEngPL;
	uint16_t	u16DCMeterEngPH;
	uint16_t	u16DCMeterEngNL;
	uint16_t	u16DCMeterEngNH;
	int16_t	i16DCMeterPT;
	int16_t	i16DCMeterCT;
	int32_t	i32Reserved;
}_DCMeterRemoterDataDef;
#pragma pack()


#pragma pack(1)
typedef struct
{

	uint16_t    Mode;				//模式
	uint16_t	Sing_ParaConnect;	//单并机标志
	uint16_t	MasterSlave;		//主从机标志
	uint16_t	ConnectAddr;		//并机地址
	uint16_t	ModbusAddr;			//modbus地址

}_ACDCSetUploadDataDef;
#pragma pack()

#pragma pack(1)
typedef struct
{
	int16_t		i16_ModID;
	_ACDCDataDef_1	stACDCData;
	_ACDCSetUploadDataDef stACDCSetData;
	int16_t 	Import_Temp;
}_ACDCRemoteDataDef;
#pragma pack()


typedef union
{
	uint16_t u16Word1All;
	struct
	{
		uint16_t bfb1:1;
		uint16_t bfb2:1;
		uint16_t bfb3:1;
		uint16_t bfb4:1;	
		uint16_t bfb5:1;	
		uint16_t bfb6:1;	
		uint16_t bfb7:1;
		uint16_t bfb8:1;

		uint16_t bRsv		:8;
	}Bits;
}_EBattDIStateFbDef;

#pragma pack(1)
typedef struct
{
	uint16_t GroupID;
	_DCDCDataDef_1  meter1;
	_DCDCSettingDataDef stconfig;
	int16_t  Improt_Temp;
}_DCDCRemoteDataDef;
#pragma pack()

class   Class_EMMonitor
{
	private:


	public:
		
		_PVDIStateFbDef				m_st_PVCabinetStFb;
		
		_EBattDIStateFbDef			m_st_EBattCabinetStFb;
		//_EBattRemoteDef				m_st_EBattRemoteData;
			
		_ACDCRemoteDataDef			m_st_ACDCRemoteData;
		
		_DCMeterRemoterDataDef		m_st_DCMeterRemoteData[3];
		_DCDCRemoteDataDef			m_st_DCDCRemoteData;
			
	private:

		void	Dat_RefreshDCDCData(int16_t index);
		void Dat_RefreshACDCData(int16_t index);
	
		void Dat_ACDCBoxMsgXmit();
		void Dat_DCDCBoxMsgXmit();
		void Dat_StoredEnergyBoxMsgXmit();
		void Dat_SysControlBoxMsgXmit();
		void Dat_ACLoadBoxMsgXmit();
		void Dat_AloneACDCBoxMsgXmit();

	public:
		
		void	Dat_Init(void);
		void	Dat_DataMainten(void);

		void Dat_RefreshBMSData();



};

#endif



