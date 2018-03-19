
#ifndef _UPGRADE_H
#define _UPGRADE_H

#include "Publics.h"

#define STM32_FLASH_SIZE_ADDR 0x1FFFf7e0

#if STM32_FLASH_SIZE<512
#define BOOT_ADDR					0x08000000    //30K-7800
#define APP_FLG_ADDR		0x08002000 		//2K-800
#define FLASH_APP_ADDR		0x08002800  	
//#define APP_STOR_ADDR	        0x08011000   

#define APP_STOR_ADDR	        0x08021400


#define CONFIG_ADDR				0x08038000    //配置文件 4K-1000
#define LOG_ADDR					0x08039000		//日志	28K-B000
#else
#define BOOT_ADDR					0x08000000    //62K-F800
#define APP_FLG_ADDR			0x0800f800 		//2K-800
#define FLASH_APP_ADDR		0x08010000  	//第一个应用程序起始地址(存放在FLASH) 200k-32000
#define APP_STOR_ADDR			0x08042000    //第二分应用程序起始地址	200K-32000
#define CONFIG_ADDR				0x08074000    //配置文件 4K-1000
#define LOG_ADDR					0x08075000		//日志	44K-B000
#endif
#define USART_REC_LEN  			2*1024 //定义最大接收字节数 

#pragma pack(1)
typedef struct
{
	unsigned short New_flg;    //程序存储区有需要升级的程序 1，否则0
	unsigned int Program_length;	//升级程序的长度
	unsigned int CRC_stor;		//升级程序的CRC-16
	unsigned short Updata_result;	//升级成功 1，升级失败 0；
	unsigned short FLG_CRC_Stor;  //flg区内容的CRC-16
}FLG;
#pragma pack()
typedef enum updata_pgn
{
	Ask_updata=0x61,
	Ack_updata=0x62,
	
	Program=0x63,
	Data_tx=0xeb,
	
	End_updata=0x64,
	Thebag_end =0x67,
	
	End_all_program = 0x68,
	
	Continue_breakpoint=0x65,
	TX_lastpackagenum=0x66
	
	
}Updata_pgn;

typedef enum priority1
{
	Ask_updata_priority=6,
	Ack_updata_priority=6,
	
	Program_priority=6,
	Data_tx_priority=6,
	
	End_updata_priority=6,
	The_bag_end_priority =6,
	
	Continue_breakpoint_priority=6,
	TX_lastpackagenum_priority=6
}Updata_priority;
typedef enum sta
{
	allow_up=0,
	program_top=1,
	rcv_data=2,
	end_up=3
}MIAN_STA;

typedef struct
{

	uint8_t	Msgtype;
	uint8_t	length;
	uint8_t	data[8];

}_UPGRADE_RECV_DATA;

typedef enum VersionCheck
{
	WORD0	= 0,
	WORD1	= 1,
	WORD2	= 2,
	WORD3	= 3,
	WORD4	= 4,
	WORD5	= 5,
	WORD6	= 6,
	WORD7	= 7,
	END_CHECK
}_VERSION_CHECK;

enum
{
	VER_UNKONOW	= 0,
	VER_OK		= 1,
	VER_WRONG	= 2
};

typedef struct
{
	uint8_t		u8UpgdCmdRp;		//允许升级响应
									//0xAA允许升级
									//0x00 不允许升级
									//0x01 枪状态不允许
									//0x02 片内FLASH太小不允许升级

	uint8_t		u8PackgeRecvOver;
									//0xAA本包数据接收成功
									//0x00本报数据接收失败
									//0x08 程序长度错误
									//0x09 程序校验错误
									//0x0B 写入FLASH失败

	uint8_t		u8UpgdCmd;
}_UPGRADE_DATA;

typedef union VersionCheckData
{
	uint64_t u64Word1All;
	struct
	{
	uint64_t Word0			:8;
	uint64_t Word1			:8;
	uint64_t Word2			:8;
	uint64_t Word3			:8;
	uint64_t Word4			:8;
	uint64_t Word5			:8;
	uint64_t Word6			:8;
	uint64_t Word7			:8;
	}Bits;
}_VERSION_CHECK_DATA;


class   Class_Upgrade
{
	public:
		_UPGRADE_DATA		m_St_UpgdIntf;	
		_UPGRADE_RECV_DATA	m_St_UpgdRecvData;	
              _CanMstMonTxProtocolDef      m_st_TxBuff;
//		_CanInnerTxProtocolDef 	m_st_TxBuff;

//	uint8_t	u8PackageSqucBak;
		uint16_t m_st_VersionCheckState;
		uint16_t m_st_VersionCheckResult;
		_VERSION_CHECK_DATA u64_VerData;

	
	uint16_t	u16PackageNum;
//	uint32_t	u32ByteNumInPackage;

//	uint16_t	u16PackageNumMaxRecv;
//	uint8_t	u8PackageSqucNext;

//	uint32_t	u32ByteNumInPackageEnd;
//	uint16_t	u16PackageChecksum;
	
	uint8_t	u8UpgdOffStFlag;
	uint16_t	u16UpgdOffCnt;
	uint16_t	u16UpgdStopFlag;
	uint8_t     u8UpgdOKDelayFlag;

	uint8_t	u8UpgdErrCnt;

	uint8_t	u8HaveMsgType;


	uint8_t updata_count;	
	uint32_t Program_length,APP_LEN;
	uint32_t Write_Addr;								//写入FLASH的地址
	//Write_Addr =APP_STOR_ADDR;  				//写入地址
	//package_num=0;											//数据包数
	//APP_LEN=0;													//总程序字节数
	//Program_length=0
	uint16_t package_num;								//接收包编号

	Class_CanRecvQueue m_CanRecvQueue;

	public:
		uint8_t UpdataFlag;
		void Dat_Init(void);
		void App_UpgradeDataExchg(CanRxMsg message);
		void App_UpgradeManage(void);
		void App_UpgradeMsgRspose(_CanMstMonTxProtocolDef *message);

		void Dat_UpgradeOffline(void);

		int16_t App_VersionCheck(unsigned char *appbuf,uint32_t appsize);
		int16_t   MsgRecvProc(void);


	
	int iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 applen);	//在指定地址开始,写入bin
//	void updata_end(uint8_t sta);
	int16_t IAP_Program(_UPGRADE_RECV_DATA *RE_stor);
	void Set_FLG(void);
	void test_boot(void);
//	void allow_updata(uint8_t statues);
	void SoftReset(void);
	int cpuGetFlashSize(void);	
};

#endif


