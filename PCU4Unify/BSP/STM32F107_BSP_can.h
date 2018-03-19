/******************** (C) COPYRIGHT 2013 www.armjishu.com  ********************
 * 文件名  ：can.h
 * 描述    ：
 * 实验平台：STM32神舟开发板
 * 标准库  ：STM32F10x_StdPeriph_Driver V3.5.0
 * 作者    ：www.armjishu.com
*******************************************************************************/
#ifndef __CAN_H
#define __CAN_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"

  void BSP_CanInit(void);
  uint8_t BSP_Can1_TranMsg(CanTxMsg* TxMessage);
  void BSP_Can2_TranMsg(CanTxMsg* TxMessage);

  void BSP_CAN1_CloseInt(void);
  void BSP_CAN1_OpenInt(void);

  void BSP_CAN2_CloseInt(void);
  void BSP_CAN2_OpenInt(void);
  void BSP_Can2_Init(void);

  void BSP_CAN3_CloseInt(void);
  void BSP_CAN3_OpenInt(void);
  void BSP_Can3_Init(void);



#ifdef __cplusplus
}
#endif

#endif



