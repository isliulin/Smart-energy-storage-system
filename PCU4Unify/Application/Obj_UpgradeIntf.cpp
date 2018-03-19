#include "Publics.h"
#include "CRC.h"

#pragma location="VER_DATA"//0x08009800//
__root const uint64_t VER_DATA=0x54531003C6A444D2;
									   //0x7766554433221100
									   //byte0:特定序列【D2】
									   //byte1:特定序列【44】
									   //byte2:特定序列【A4】
									   //byte3:软件版本【C6--标准软件  】
									   //byte4:硬件版本【03--第3版     】
									   //byte5:软件系列【10--1.0软件   】
									   //byte6:产品类别【53 "S" ASSIC 码
									   //byte7:特定序列【54 "T"ASSIC 码


#define FLG_NUM 7  //半字
uint16_t iapbuf[1024];
extern __IO uint32_t can_reflg;

uint8_t RCV_DATA[USART_REC_LEN]; //__attribute__ ((at(0X20001000)));	//RAM 区


__IO u32 Wait_Update_Delay;
union
{
	FLG boot_flg;
	uint16_t flg_stor_16[FLG_NUM];
	uint8_t flg_stor_8[FLG_NUM*2];
}flg_S;

typedef enum
{
	nouse = 0,
	_63_ALLOW_SEN ,
	_64_update_ok,
	_64_crc_fault = 0x03,
	_eb_receive_ok,
	_64_TxErr_larger3,
	_67_the_package_len_error = 0x08,
	_67_the_package_crc_error,
	_67_write_flash_faild= 0x0b,
	_64_code_Recv_ok = 0x0e,
	_67_the_package_ok = 0xAA,
	_63_giveup_link = 0xFF,
}UPDATE_RESUL;





/***************************************************************************
函数名:Init
功能描述:
函数调用
作者:
日期:
****************************************************************************/

void Class_Upgrade::Dat_Init(void)
{
//	u8PackageSqucNext = 0;
//	m_St_UpgdRecvData.u16PackageSquc = 0;
//	u8PackageSqucBak = 0;
//	u16PackageNumIndex = 0;
//	u8PackgeRecvOverFlag = 0;
	u16UpgdOffCnt = 0;
	package_num =0;
        u8UpgdOKDelayFlag=0;
	updata_count=0;	
	Program_length=0;
	APP_LEN= 0;
	Write_Addr =APP_STOR_ADDR; 
	m_st_VersionCheckState = WORD0;
	m_st_VersionCheckResult = 0;
	objPcuModel.m_st_SelfState.Bits.bUpdateFlag = 0;
	m_CanRecvQueue.CanRecvQueueInit();
	objUpgrade.UpdataFlag = 0;
}

/***************************************************************************
函数名:MsgRecvProc
功能描述:设置量请求消息重发标志
作者:
日期:
****************************************************************************/
int16_t    Class_Upgrade::MsgRecvProc(void)
{
	int16_t ret = 0;
	//CanRxMsg tempPacked;
	/*_CanInnerRxProtocolDef tempPacked;

	//queue is not null
	while(E_QUEUE_NULL != m_CanRecvQueue.FlagQueue())
	{
		//接收队列里面已经有比较老的数据，接收后删除该数据
		tempPacked.Frame = m_CanRecvQueue.GetOldestNode();

		m_CanRecvQueue.DelQueue();

		//把接收协议层的数据转换成应用层的数据，处理后的应用层数据
		//赋值给指针m_NonFragRecvMsg.HeadInfo，最后返回给指针pApdu
//		FrameParseToApdu(pApdu, &tempPacked);
		App_UpgradeDataExchg(tempPacked.Frame);//(m_CurRecivedFrame.Frame);
		
		ret = sizeof(_CanInnerAppMsgDef);
		return ret;
	}*/
		//CanRxMsg tempPacked;
	_CanMstMonRxProtocolDef tempPacked;

	//queue is not null
	while(E_QUEUE_NULL != m_CanRecvQueue.FlagQueue())
	{
		//接收队列里面已经有比较老的数据，接收后删除该数据
		tempPacked.Frame = m_CanRecvQueue.GetOldestNode();

		m_CanRecvQueue.DelQueue();

		//把接收协议层的数据转换成应用层的数据，处理后的应用层数据
		//赋值给指针m_NonFragRecvMsg.HeadInfo，最后返回给指针pApdu
		
		App_UpgradeDataExchg(tempPacked.Frame);
		ret = sizeof(_CanMstMonAppMsgDef);
		return ret;
	}

	return ret;
}
void Class_Upgrade::App_UpgradeManage(void)
{

	uint8_t  Result = 0;
	uint8_t  Newmsg = 0;

	//static uint8_t cntbak = 0;
	Dat_UpgradeOffline();
	/*if(objCharger.App_GetOnOffCmd() == 1
			|| objCanDataPool.m_st_RecvCCUMsgObj.u32PDUState.Bits.bRecvOncmd == 1)
	{
		return;
	}*/

	// 接收到升级请求命令
	if(objUpgrade.m_St_UpgdIntf.u8UpgdCmd == 0x55)
	{
		u8HaveMsgType = Ask_updata;
		m_St_UpgdRecvData.Msgtype = Ask_updata;
		m_St_UpgdRecvData.length =  1;
		m_St_UpgdRecvData.data[0] = 0x55;
		objUpgrade.m_St_UpgdIntf.u8UpgdCmd =0;
		APP_LEN= 0;
		m_st_VersionCheckResult = 0;
		Write_Addr =APP_STOR_ADDR; 							//写入FLASH的地址
		package_num = 0;								//接收包编号
		memset(&flg_S.boot_flg,0x00,sizeof(FLG));
		m_St_UpgdIntf.u8UpgdCmdRp = 0xAA;	
		objCanDataExchg.m_st_ToMstMonMsgObj.MsgObj[E_MM_ALLOW_UPGRADE].u16MACId =objPcuModel.m_i16_SelfAddr;
		objCanDataExchg.m_st_ToMstMonMsgObj.MsgObj[E_MM_ALLOW_UPGRADE].u16XmitActive = TRUE;
		objPcuModel.m_st_SelfState.Bits.bUpdateFlag = 1;
		objCanMasterMonPrtcl.m_CanXmitQueue.ClearQueue();
							//写入FLASH的地址
		memset(&flg_S.boot_flg,0x00,sizeof(FLG));

		//再次开始升级后复位升级终止标志
		u16UpgdStopFlag = 0;

	}
	if(u8UpgdErrCnt > 0
		||Result == _64_crc_fault
		||Result == _63_giveup_link
		||Result == _67_write_flash_faild
		||Result == _67_the_package_crc_error
		||Result == _67_the_package_len_error)
	{
		//objPcuModel.m_st_SelfState.Bits.bUpdateFlag = 0;
		UpdataFlag = 0;
	}
	MsgRecvProc();

	//升级状态机
	if(u8HaveMsgType)
	{
		Result = IAP_Program(&m_St_UpgdRecvData);
		u8HaveMsgType = 0;
	}

	//需要给后台发送响应消息
	if(Result == _63_ALLOW_SEN)
	{
		Newmsg = 1;

		m_st_TxBuff.PackedMsg.ExtId.bPF = PROTNO_CNCTMNGT;
		m_st_TxBuff.PackedMsg.DLC = 4;
		//数据域
		m_st_TxBuff.Frame.Data[0] = 0x11;
		m_st_TxBuff.Frame.Data[1] = u16PackageNum & 0x00ff;
		m_st_TxBuff.Frame.Data[2] = (uint8_t)(u16PackageNum >>8);
		m_st_TxBuff.Frame.Data[3] = package_num;
	}
	else if((Result == _67_write_flash_faild) ||(Result == _67_the_package_ok)
		||Result == _67_the_package_crc_error ||Result == _67_the_package_len_error)
	{
		m_St_UpgdIntf.u8PackgeRecvOver = Result;	
		Newmsg = 1;

		m_st_TxBuff.PackedMsg.ExtId.bPF = PROTNO_PACKAGE_OVER;
		m_st_TxBuff.PackedMsg.DLC = 1;
		//数据域
		m_st_TxBuff.Frame.Data[0] = m_St_UpgdIntf.u8PackgeRecvOver;

	}
	else if(Result == _63_giveup_link)
	{
		Newmsg = 1;
		m_st_TxBuff.PackedMsg.ExtId.bPF= PROTNO_CNCTMNGT;
		m_st_TxBuff.PackedMsg.DLC = 1;
		//数据域
		m_st_TxBuff.Frame.Data[0] = _63_giveup_link;
	}
	else if(Result == _eb_receive_ok)
	{
	}
	else if(Result == _64_crc_fault)
	{
		Newmsg = 1;
		
		m_st_TxBuff.PackedMsg.ExtId.bPF= PROTNO_CODE_OVER;
		m_st_TxBuff.PackedMsg.DLC = 1;
		//数据域
		m_st_TxBuff.Frame.Data[0] = _64_crc_fault;
	
	}
	else if(	u8UpgdErrCnt >0)
	{
		Newmsg = 1;
		u8UpgdErrCnt = 0;
		
		m_st_TxBuff.PackedMsg.ExtId.bPF= PROTNO_CODE_OVER;
		m_st_TxBuff.PackedMsg.DLC = 1;
		//数据域
		m_st_TxBuff.Frame.Data[0] = _64_TxErr_larger3;
		
	}
	else if(Result == _64_update_ok)    //xiugai 
	{
		Newmsg = 1;
		
		m_st_TxBuff.PackedMsg.ExtId.bPF= PROTNO_CODE_OVER;
		m_st_TxBuff.PackedMsg.DLC = 1;
		//数据域
		m_st_TxBuff.Frame.Data[0] = _64_code_Recv_ok;
		u8UpgdOKDelayFlag=1;
	  
	}

	
	if(Newmsg )
	{
		Newmsg = 0;

		App_UpgradeMsgRspose(&m_st_TxBuff);
		if(u8UpgdOKDelayFlag)
		{
                   u8UpgdOKDelayFlag=0;
	           Delay_10us(1000);
		}
	}

	if(Result == _64_update_ok)    //xiugai 
	{
		SoftReset();
	}	
		
}

void Class_Upgrade::App_UpgradeMsgRspose(_CanMstMonTxProtocolDef *pApdu)
{

	_CanMstMonTxProtocolDef pFrame;
	
        int i = 0;

/*
	tempFrame.PackedMsg.ExtId.bCnt = NONFRAG_MSG;
	tempFrame.PackedMsg.ExtId.bSrcAddr = objPDU.m_i16_SelfAddr;
	tempFrame.PackedMsg.ExtId.bProtNO = message.PackedMsg.ExtId.bProtNO ;
	tempFrame.PackedMsg.ExtId.bPTP = PTP;
	tempFrame.PackedMsg.ExtId.bDstAddr = MAC_ID_CCU;//点对点,源地址
	tempFrame.PackedMsg.ExtId.bRes1 = 1;
	tempFrame.PackedMsg.ExtId.bRes2 = 1;

	tempFrame.PackedMsg.IDE = CAN_ID_EXT;
	tempFrame.PackedMsg.RTR = CAN_RTR_DATA;
	tempFrame.PackedMsg.DLC = message.PackedMsg.DLC;
	//数据域
	for(int i = 0;i < 8;i++)
	{
		tempFrame.Frame.Data[i] = message.Frame.Data[i];
	}

	objCanInnerProtocol.XmitFramePost(&(tempFrame.Frame));
*/

	// = pApdu->u16MACId;  //目的地址
	// = pApdu->u16PGN;
	//pFrame->PackedMsg.ExtId.bPS = pApdu->u16MACId;//目的地址

	//pFrame->PackedMsg.ExtId._4Bytes_T.

	pFrame.PackedMsg.ExtId.bDataPage = 0;
	pFrame.PackedMsg.ExtId.bPriority = pApdu->PackedMsg.ExtId.bPriority;
	pFrame.PackedMsg.ExtId.bRev = 0;
	pFrame.PackedMsg.ExtId.bR = 0;

	pFrame.PackedMsg.ExtId.bSourceAddr = objPcuModel.m_i16_SelfAddr;//pApdu->u16MACId;//86
	pFrame.PackedMsg.ExtId.bPF = pApdu->PackedMsg.ExtId.bPF;
	pFrame.PackedMsg.ExtId.bPS = 0xfa;//0xF4; BMS的源地址 244

	pFrame.PackedMsg.DLC = pApdu->PackedMsg.DLC;
	pFrame.PackedMsg.IDE = CAN_ID_EXT;
	pFrame.PackedMsg.RTR = CAN_RTR_DATA;

	for (i = 0; i < pApdu->PackedMsg.DLC; i++)
	{

		pFrame.PackedMsg.MsgData[i] = pApdu->PackedMsg.MsgData[i];


	}
	objCanMasterMonPrtcl.XmitFramePost(&(pFrame.Frame));

}


/***************************************************************************
函数名:App_UpgradeDataExchg
功能描述:	这部分是长帧处理
函数调用
作者:
日期:
****************************************************************************/
void Class_Upgrade::App_UpgradeDataExchg(CanRxMsg message)
{
static	_CanMstMonRxProtocolDef TempFrame;
	
	 TempFrame.Frame = message;

	if(TempFrame.PackedMsg.ExtId.bPF== PROTNO_CNCTMNGT)
	{
		u8HaveMsgType = Program;
		m_St_UpgdRecvData.Msgtype = Program;
	}
	else if(TempFrame.PackedMsg.ExtId.bPF == PROTNO_DATATSFR)
	{
		u8HaveMsgType = Data_tx;
		m_St_UpgdRecvData.Msgtype = Data_tx;
	}
	else if(TempFrame.PackedMsg.ExtId.bPF == PROTNO_CODEOVER)
	{
		u8HaveMsgType = End_all_program;
		m_St_UpgdRecvData.Msgtype = End_all_program;
	}
	else if(TempFrame.PackedMsg.ExtId.bPF == PROTNO_UPGRADE)
	{
		objUpgrade.m_St_UpgdIntf.u8UpgdCmd = 0x55;
		objUpgrade.UpdataFlag = 1;
	}


	
	m_St_UpgdRecvData.length =  TempFrame.Frame.DLC;
	// 接收长度判断
	for(int i = 0;i < 8;i++)
	{
		((uint8_t*)(&(m_St_UpgdRecvData.data)))[i] = TempFrame.Frame.Data[i];
	}


	// 接收数据包超时，结束不到数据结束升级
	// 数据偏移，
}



void Class_Upgrade::Dat_UpgradeOffline(void)
{
	if (g_st_TimerFlag.b1s)
	{
		u16UpgdOffCnt++;
		UpDownLimited(u16UpgdOffCnt, 20, 0)
		if(u16UpgdOffCnt>10)
		{
			//u16UpgdStopFlag = 1;
			objPcuModel.m_st_SelfState.Bits.bUpdateFlag = 0;
		}
	}
}



//appxaddr:应用程序的起始地址
//appbuf:应用程序CODE.
//appsize:应用程序大小(字节).
int Class_Upgrade::iap_write_appbin(uint32_t appxaddr,uint8_t *appbuf,uint32_t appsize)
{
	uint16_t t;
	uint16_t i=0;
	uint16_t temp;
	uint32_t fwaddr=appxaddr;//当前写入的地址
	uint8_t *dfu=appbuf;
	for(t=0;t<appsize;t+=2)
	{						    
		temp=(uint16_t)dfu[1]<<8;
		temp+=(uint16_t)dfu[0];	  
		dfu+=2;
		iapbuf[i++]=temp;	    
		if(i==1024)
		{
			i=0;
			if(STMFLASH_Write(fwaddr,iapbuf,1024) ==0)
					return 0;
			fwaddr+=2048;//偏移2048  16=2*8.所以要乘以2.
		}
	}
	if(i) 
		if(STMFLASH_Write(fwaddr,iapbuf,i)==0)//将最后的一些内容字节写进去.  
			return 0;
	return 1;
}


void Class_Upgrade::Set_FLG(void)
{
	flg_S.boot_flg.FLG_CRC_Stor = CRC_GetModbus16(flg_S.flg_stor_8, (FLG_NUM-1)*2);
	INTX_DISABLE(); 
	FLASH_Unlock();
	STMFLASH_Write(APP_FLG_ADDR,flg_S.flg_stor_16,FLG_NUM);
	FLASH_Lock();
	INTX_ENABLE();
}
void Class_Upgrade::SoftReset(void)
{
	__set_FAULTMASK(1);      
	NVIC_SystemReset();
}

int16_t Class_Upgrade::IAP_Program(_UPGRADE_RECV_DATA *RE_stor)
{
	UPDATE_RESUL result=nouse;
	int16_t s_cnt =0;
	static	uint16_t crc_ca;										//集控发送的CRC校验值
	static	uint16_t my_crc_data;								//计算的CRC校验值
	switch(RE_stor->Msgtype)
	{
		case Program:
				if(RE_stor->data[0]==0x10)
				{
					result = _63_ALLOW_SEN;
					package_num=1;
					Program_length = *(vu32 *)&RE_stor->data[1];  			//保存本包数据长度
					u16PackageNum = *(uint16_t *)&RE_stor->data[5];  
				}
				if(RE_stor->data[0]==0x13)
				{	s_cnt =(unsigned int)(*(unsigned int *) &RE_stor->data[1]);
					if(APP_LEN==s_cnt	)//(unsigned int)(*(unsigned int *) &RE_stor->data[1]))	
					{
						crc_ca=(unsigned short)(*(unsigned short *)&RE_stor->data[5]);
						m_st_VersionCheckResult |= App_VersionCheck(RCV_DATA, APP_LEN);
						my_crc_data = CRC_GetModbus16(RCV_DATA, APP_LEN);
						if(my_crc_data==crc_ca)																				
						{
							if(iap_write_appbin(Write_Addr,RCV_DATA,APP_LEN)==0)		//更新FLASH代码 
							{
								result =_67_write_flash_faild;
								package_num=0;									//数据包数
								APP_LEN=0;										//总程序字节数
								u8UpgdErrCnt = 3;
							}
							else
							{
								Write_Addr =Write_Addr +APP_LEN;
								result = _67_the_package_ok;
								package_num=0;								//数据包数
								APP_LEN=0;		
								u8UpgdErrCnt = 0;
							}											//总程序字节数
						}
						else										//校验失败
						{
							result = _67_the_package_crc_error;
							package_num=0;						//数据包数
							APP_LEN=0;							//总程序字节数
							u8UpgdErrCnt ++;
						}
					}
					else if (APP_LEN !=0)										//接收到的字节数与定义的不同
					{
						result = _67_the_package_len_error;
						package_num=0;							//数据包数
						APP_LEN=0;								//总程序字节数

						u8UpgdErrCnt ++;
					}
				}
			break;
			case End_all_program:
						
						APP_LEN=(unsigned int)(*(unsigned int *) &RE_stor->data[0]);											
						crc_ca=(unsigned short)(*(unsigned short *)&RE_stor->data[4]);
						my_crc_data =CRC_Get16(APP_STOR_ADDR,APP_LEN);
						if(my_crc_data == crc_ca
							&& m_st_VersionCheckResult == VER_OK)    																				
						{
							flg_S.boot_flg.CRC_stor = crc_ca;
							flg_S.boot_flg.New_flg = 0x0001;
							flg_S.boot_flg.Program_length = APP_LEN;
							Set_FLG();
							result = _64_update_ok;
						}
						else												//校验失败
						{
							result = _64_crc_fault;
						}																
				break;
		case Data_tx:
			if(RE_stor->data[0]==((uint8_t)package_num))											
			{
				memcpy(&RCV_DATA[(package_num-1)*7],&RE_stor->data[1],(RE_stor->length-1));
				APP_LEN+=(RE_stor->length-1);						//字节数计数
				package_num++;									//包数计数
				result = _eb_receive_ok;
                                if(((RE_stor->length-1) !=7) && package_num < 293)
                                {
                                  	result = _eb_receive_ok;
                                }
			}
			else				//如果出现丢包，收到的包编号与期望的不同则3次重新接收本包数据
			{
				result = _63_giveup_link;
				package_num=0;																							
				APP_LEN=0;		
				
				u8UpgdErrCnt++;
			}
			break;
			
		default:
			break;
	}
	return result;	
}

/***************************************************************************
函数名:App_VersionCheck
功能描述:
函数调用
作者:
日期:
****************************************************************************/
int16_t Class_Upgrade::App_VersionCheck(unsigned char *appbuf,uint32_t appsize)
{
	uint32_t i = 0;
	uint8_t CheckData;
	//unsigned char *dfu = appbuf;
//	uint8_t Check_Result = VER_UNKONOW;
	u64_VerData.u64Word1All = VER_DATA;
	for(i = 0; i < appsize; i++)
	{							
		//CheckData = (uint8_t)dfu[0];
		//dfu += 1;
		CheckData = RCV_DATA[i];
		switch(m_st_VersionCheckState)
		{
			case WORD0:
				
				if(CheckData == u64_VerData.Bits.Word0)
				{
					m_st_VersionCheckState = WORD1;
				}
				else
				{
					m_st_VersionCheckState = WORD0;
				}
				
				break;

			case WORD1:
				
				if(CheckData == u64_VerData.Bits.Word1)
				{
					m_st_VersionCheckState = WORD2;
				}
				else
				{
					m_st_VersionCheckState = WORD0;
				}
								
				break;	

			case WORD2:
				
				if(CheckData == u64_VerData.Bits.Word2)
				{
					m_st_VersionCheckState = WORD3;
				}
				else
				{
					m_st_VersionCheckState = WORD0;
				}
								
				break;

			case WORD3:
				
				if(CheckData == u64_VerData.Bits.Word3)
				{
					m_st_VersionCheckState = WORD4;
				}
				else
				{
					m_st_VersionCheckState = WORD0;
				}
								
				break;

			case WORD4:
				
				if(CheckData == u64_VerData.Bits.Word4)
				{
					m_st_VersionCheckState = WORD5;
				}
				else
				{
					m_st_VersionCheckState = WORD0;
				}
								
				break;

			case WORD5:
				
				if(CheckData == u64_VerData.Bits.Word5)
				{
					m_st_VersionCheckState = WORD6;
				}
				else
				{
					m_st_VersionCheckState = WORD0;
				}
								
				break;

			case WORD6:
				
				if(CheckData == u64_VerData.Bits.Word6)
				{
					m_st_VersionCheckState = WORD7;
				}
				else
				{
					m_st_VersionCheckState = WORD0;
				}
								
				break;

			case WORD7:
				
				if(CheckData == u64_VerData.Bits.Word7)
				{
					return VER_OK;
				}
				else
				{
					m_st_VersionCheckState = WORD0;
				}
								
				break;
		
			default:

				break;
		}
	}
	return VER_UNKONOW;
}


//extern const unsigned short g_McRctable_16[256];

/*void  Class_Upgrade::test(void)
{
	uint32_t Write_Addr;	
	Write_Addr =APP_STOR_ADDR;  				//写入地址
	STMFLASH_Write(Write_Addr,(uint16_t *)g_McRctable_16,256);
	flg_S.boot_flg.CRC_stor = 0x00;
	flg_S.boot_flg.New_flg = 0x0001;
	flg_S.boot_flg.Program_length = 512;
	Set_FLG();
									
	SoftReset();
}*/


//===========================================================================
// End of file.
//===========================================================================


