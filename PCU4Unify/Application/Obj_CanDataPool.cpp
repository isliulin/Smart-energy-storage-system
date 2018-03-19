#include "Publics.h"
#include <String.h>

/***************************************************************************
函数名:BMSInit
功能描述:
函数调用
作者:
日期:
****************************************************************************/
void Class_CanDataPool::Dat_Init(void)
{

	VERSION_DEFINE_SOFTWARE(m_u32_SoftWareVer,VER_V(5),VER_B(02),VER_D(03));//版本号设置
	VERSION_DEFINE_HARDWARE(m_u32_SoftWareVer,VER_A(m_u32_HardWareVer));//版本号设置

}

/***************************************************************************
函数名:App_DataDeal
功能描述:
函数调用
作者:
日期:
****************************************************************************/
void	Class_CanDataPool::Dat_DataMainten(void)
{
	Dat_BMSDataInterface();
	Dat_InnerDataInterface();
	
}


/***************************************************************************
函数名:Dat_CanDataInterface
功能描述:
函数调用
作者:
日期:
****************************************************************************/
void Class_CanDataPool::Dat_BMSDataInterface(void)
{
												  

}

void Class_CanDataPool::Dat_InnerDataInterface(void)
{
	
}
