/***************************************************************				
File name 	  ：Drv_CAN.c
Description     ：
author          ：kxajd
version/date    ：V1.0 2018/3/15
copy            ：ECARTEK
-----------------------------------------------------------------
modify:
date 		 version		 Modifier   	 Update

*****************************************************************/
#include "stm32f10x.h"
#include "platform_config.h"
/*******************************************************************************
* Function Name  : CAN_Configuration
* Description    : CAN的波特率设置
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void CAN_Configuration(void)
{
	extern CanTxMsg const ConstCanCmdLock;
	extern CanTxMsg const CarGetWindowLQ;
	extern CanTxMsg const CarGetWindowRQ;
	extern CanTxMsg const CarGetWindowLH;
	extern CanTxMsg const CarGetWindowRH;
	extern CanTxMsg const ConstCarCmdCloseSKY;
	extern CanTxMsg const CarGetlonkmode;
	extern CanTxMsg const ConstCanCarLFBACK;
	extern CanTxMsg const CarChargingState;
	
	extern CanTxMsg CarGetCarLockState;
	extern CanTxMsg CarGetWindowSKY;
	extern CanTxMsg CanCmdCart;
	extern CanTxMsg CanGetDoorstate;
	extern CanTxMsg CanGetNumKey;
	extern CanTxMsg CanGetAccState;
	extern CanTxMsg CarGetRemote;
	extern CanTxMsg CarFd;
	extern CanTxMsg  engine_state ;
	extern CanTxMsg CarGetCarTbox;

	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1|RCC_APB1Periph_CAN2, ENABLE);
	
	CAN_DeInit(CAN1);
	CAN_DeInit(CAN2);
	
	CAN_InitStructure.CAN_TTCM= DISABLE;	//禁止时间触发通信模式
	CAN_InitStructure.CAN_ABOM= ENABLE;	//软件对CAN_MCR寄存器的INRQ位进行置1随后清0后，一旦硬件检测到128次11位连续的隐性位，就退出离线状态。
	CAN_InitStructure.CAN_AWUM= DISABLE;	//睡眠模式通过清除CAN_MCR寄存器的SLEEP位，由软件唤醒
	CAN_InitStructure.CAN_NART= DISABLE;	//CAN报文只被发送1次，不管发送的结果如何（成功、出错或仲裁丢失）
	CAN_InitStructure.CAN_RFLM= DISABLE;	//在接收溢出时FIFO未被锁定，当接收FIFO的报文未被读出，下一个收到的报文会覆盖原有的报文
	CAN_InitStructure.CAN_TXFP= DISABLE;	//发送FIFO优先级由报文的标识符来决定
	//CAN_InitStructure.CAN_Mode= CAN_Mode_LoopBack;	//CAN硬件工作在环回模式
	CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;	//CAN硬件工作在正常模式
	//CAN_InitStructure.CAN_Mode= CAN_Mode_Silent;	//CAN硬件工作在静默模式
	CAN_InitStructure.CAN_SJW=  CAN_SJW_1tq;	//重新同步跳跃宽度1个时间单位
	CAN_InitStructure.CAN_BS1=  CAN_BS1_8tq;	//时间段1为8个时间单位
	CAN_InitStructure.CAN_BS2=  CAN_BS2_7tq;	//时间段2为7个时间单位
	CAN_InitStructure.CAN_Prescaler= (u16)CanCmdCart.Data[2];	//(PCLK1/((1+8+7)*5)) = 8Mhz/16/5 = 100Kbits设定了一个时间单位的长度9
	CAN_Init(CAN1, &CAN_InitStructure);
	CAN_InitStructure.CAN_Prescaler= (u16)CanCmdCart.Data[3];	//(PCLK1/((1+8+7)*5)) = 8Mhz/16/5 = 100Kbits设定了一个时间单位的长度9
	CAN_Init(CAN2, &CAN_InitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber= 0;	//指定了待初始化的过滤器0
	CAN_FilterInitStructure.CAN_FilterMode= CAN_FilterMode_IdList;	//指定了过滤器将被初始化到的模式为标识符列表模式
	CAN_FilterInitStructure.CAN_FilterScale=  CAN_FilterScale_16bit;	//给出了过滤器位宽1个32位过滤器
	CAN_FilterInitStructure.CAN_FilterIdHigh= (ConstCanCmdLock.StdId << 5);//(TxMessageCar[CAN_DOORSTAT].StdId << 5);	//用来设定过滤器标识符（32位位宽时为其高段位，16位位宽时为第一个）
	CAN_FilterInitStructure.CAN_FilterIdLow=  (CarGetWindowLQ.StdId << 5);	//用来设定过滤器标识符（32位位宽时为其低段位，16位位宽时为第二个
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= (CarGetWindowRQ.StdId << 5);	//用来设定过滤器屏蔽标识符或者过滤器标识符（32位位宽时为其高段位，16位位宽时为第一个
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=  (CarGetWindowLH.StdId << 5);//(TxMessageCar[CAN_GLASSSTAT].StdId << 5);	//用来设定过滤器屏蔽标识符或者过滤器标识符（32位位宽时为其低段位，16位位宽时为第二个
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment= CAN_FIFO0;	//设定了指向过滤器的FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation= ENABLE;	//使能过滤器
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber= 1;	//指定了待初始化的过滤器0
	CAN_FilterInitStructure.CAN_FilterMode= CAN_FilterMode_IdList;	//指定了过滤器将被初始化到的模式为标识符列表模式
	CAN_FilterInitStructure.CAN_FilterScale=  CAN_FilterScale_16bit;	//给出了过滤器位宽1个32位过滤器
	CAN_FilterInitStructure.CAN_FilterIdHigh= (CarGetWindowRH.StdId << 5);	//用来设定过滤器标识符（32位位宽时为其高段位，16位位宽时为第一个）
	CAN_FilterInitStructure.CAN_FilterIdLow=  (ConstCarCmdCloseSKY.StdId << 5);	//用来设定过滤器标识符（32位位宽时为其低段位，16位位宽时为第二个
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= (CarGetCarLockState.StdId << 5);	//用来设定过滤器屏蔽标识符或者过滤器标识符（32位位宽时为其高段位，16位位宽时为第一个
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=  (CarGetWindowSKY.StdId << 5);	//用来设定过滤器屏蔽标识符或者过滤器标识符（32位位宽时为其低段位，16位位宽时为第二个
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment= CAN_FIFO0;	//设定了指向过滤器的FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation= ENABLE;	//使能过滤器
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber= 2;	//指定了待初始化的过滤器0
	CAN_FilterInitStructure.CAN_FilterMode= CAN_FilterMode_IdList;	//指定了过滤器将被初始化到的模式为标识符列表模式
	CAN_FilterInitStructure.CAN_FilterScale=  CAN_FilterScale_16bit;	//给出了过滤器位宽1个32位过滤器
	CAN_FilterInitStructure.CAN_FilterIdHigh= (CanCmdCart.StdId << 5);	//用来设定过滤器标识符（32位位宽时为其高段位，16位位宽时为第一个）
	CAN_FilterInitStructure.CAN_FilterIdLow=  (CanGetDoorstate.StdId << 5);	//用来设定过滤器标识符（32位位宽时为其低段位，16位位宽时为第二个
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= (CanGetNumKey.StdId<< 5);	//用来设定过滤器屏蔽标识符或者过滤器标识符（32位位宽时为其高段位，16位位宽时为第一个
	CAN_FilterInitStructure.CAN_FilterMaskIdLow= ( CanGetAccState.StdId<<5);	//用来设定过滤器屏蔽标识符或者过滤器标识符（32位位宽时为其低段位，16位位宽时为第二个
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment= CAN_FIFO0;	//设定了指向过滤器的FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation= ENABLE;	//使能过滤器
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber= 3;	//指定了待初始化的过滤器0
	CAN_FilterInitStructure.CAN_FilterMode= CAN_FilterMode_IdList;	//指定了过滤器将被初始化到的模式为标识符列表模式
	CAN_FilterInitStructure.CAN_FilterScale=  CAN_FilterScale_16bit;	//给出了过滤器位宽1个32位过滤器
	CAN_FilterInitStructure.CAN_FilterIdHigh= (CarGetRemote.StdId << 5);	//用来设定过滤器标识符（32位位宽时为其高段位，16位位宽时为第一个）
	CAN_FilterInitStructure.CAN_FilterIdLow=  (CarFd.StdId<< 5);	//用来设定过滤器标识符（32位位宽时为其低段位，16位位宽时为第二个
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= (CarGetCarTbox.StdId<< 5);	//用来设定过滤器屏蔽标识符或者过滤器标识符（32位位宽时为其高段位，16位位宽时为第一个
	CAN_FilterInitStructure.CAN_FilterMaskIdLow= ( CarGetlonkmode.StdId<<5);	//用来设定过滤器屏蔽标识符或者过滤器标识符（32位位宽时为其低段位，16位位宽时为第二个
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment= CAN_FIFO0;	//设定了指向过滤器的FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation= ENABLE;	//使能过滤器
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_FilterInitStructure.CAN_FilterNumber= 4;	//指定了待初始化的过滤器0
	CAN_FilterInitStructure.CAN_FilterMode= CAN_FilterMode_IdList;	//指定了过滤器将被初始化到的模式为标识符列表模式
	CAN_FilterInitStructure.CAN_FilterScale=  CAN_FilterScale_16bit;	//给出了过滤器位宽1个32位过滤器
	CAN_FilterInitStructure.CAN_FilterIdHigh= (engine_state.StdId << 5);	//用来设定过滤器标识符（32位位宽时为其高段位，16位位宽时为第一个）
	CAN_FilterInitStructure.CAN_FilterIdLow=  (0<< 5);	//用来设定过滤器标识符（32位位宽时为其低段位，16位位宽时为第二个
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= (0<< 5);	//用来设定过滤器屏蔽标识符或者过滤器标识符（32位位宽时为其高段位，16位位宽时为第一个
	CAN_FilterInitStructure.CAN_FilterMaskIdLow= ( 0<<5);	//用来设定过滤器屏蔽标识符或者过滤器标识符（32位位宽时为其低段位，16位位宽时为第二个
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment= CAN_FIFO0;	//设定了指向过滤器的FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation= ENABLE;	//使能过滤器
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);//使能指定的CAN中断 

	CAN_FilterInitStructure.CAN_FilterNumber= 14;	//指定了待初始化的过滤器0
	CAN_FilterInitStructure.CAN_FilterMode= CAN_FilterMode_IdList;	//指定了过滤器将被初始化到的模式为标识符列表模式
	CAN_FilterInitStructure.CAN_FilterScale=  CAN_FilterScale_16bit;	//给出了过滤器位宽1个32位过滤器
	CAN_FilterInitStructure.CAN_FilterIdHigh= (CarGetlonkmode.StdId << 5);	//用来设定过滤器标识符（32位位宽时为其高段位，16位位宽时为第一个）
	CAN_FilterInitStructure.CAN_FilterIdLow=  (ConstCanCarLFBACK.StdId<< 5);	//用来设定过滤器标识符（32位位宽时为其低段位，16位位宽时为第二个
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= (CarChargingState.StdId<< 5);	//用来设定过滤器屏蔽标识符或者过滤器标识符（32位位宽时为其高段位，16位位宽时为第一个
	CAN_FilterInitStructure.CAN_FilterMaskIdLow= ( CanGetDoorstate.StdId<<5);	//用来设定过滤器屏蔽标识符或者过滤器标识符（32位位宽时为其低段位，16位位宽时为第二个
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment= CAN_FIFO0;	//设定了指向过滤器的FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation= ENABLE;	//使能过滤器
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);//使能指定的CAN中断 
}

/******************* (C) COPYRIGHT 2018 ECARTEK *****END OF FILE****/
