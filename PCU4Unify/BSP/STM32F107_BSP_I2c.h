
#ifndef __BSP_I2C_EE_H
#define __BSP_I2C_EE_H


#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define IICWAITTimeOut 500

typedef union
{
	unsigned short word;
	struct
	{
		unsigned char lbyte;
		unsigned char hbyte;
	}half;
}_YearDef;

typedef struct 
{
	_YearDef Year;		//年	
	unsigned char Month;		//月
	unsigned char Week;		//星期
	unsigned char Day;			//日
	unsigned char Hour;		//时
	unsigned char Minute;		//分
	unsigned char Second;		//秒
}_CALENDAR;

extern  _CALENDAR Calendar;

/* The M24C08W contains 4 blocks (128byte each) with the adresses below: E2 = 0 */
/* EEPROM Addresses defines */
#define EEPROM_Block0_ADDRESS 0xA0   /* E2 = 0 */
#define EEPROM_Block1_ADDRESS 0xA4 /* E2 = 0 */
//#define EEPROM_Block2_ADDRESS 0xA4 /* E2 = 0 */
//#define EEPROM_Block3_ADDRESS 0xA6 /* E2 = 0 */
#define  EEPTx_BUFSIZE      sizeof(EEP_Tx_Buffer) / sizeof(EEP_Tx_Buffer[0])
#define  EEP_Firstpage      0x00
#define  EEP_Randompage	    0x06


//RTC
#define PCF8563_ADDRESS 0xA2

#define TIMERCTLREG		0x0E
#define CTLSTATUS2REG	0x01	
#define CLKOUTCTLREG	0x0D
#define CTLSTATUS1REG 	0x00
#define SECONDSREG		0x02
#define MINUTESREG		0x03
#define HOURSREG		0x04
#define DAYSREG			0x05
#define WEEKDAYSREG		0x06
#define MONTHSREG		0x07
#define YEARSREG		0x08


//TestStatus TransferStatus;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void BSP_I2C_EepromInit(void);
void BSP_I2C_EepromByteWrite(u8* pBuffer, u16 WriteAddr,u8 chipsel);
void BSP_I2C_EepromPageWrite(u8* pBuffer, u16 WriteAddr, u8 NumByteToWrite,u8 chipsel);
void BSP_I2C_EepromBufferWrite(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite,u8 chipsel);
void BSP_I2C_EepromBufferRead(u8* pBuffer, u16 ReadAddr, u16 NumByteToRead,u8 chipsel);
void BSP_RTC_Init(void);
void ReadRTCTime(void);
void WriteRTCTime(_CALENDAR m_st_Calendar);
void Delay_10us(u16 delay_count);
u16 HexToBCD(u16 Input);
u16 BCDToHex(u16  Input);


#ifdef __cplusplus
}
#endif

#endif


