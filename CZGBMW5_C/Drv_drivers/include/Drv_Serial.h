/***************************************************************				
�ļ��� 	  ��drv_uart.h
����ļ�	  ��
�ļ�ʵ�ֹ��� ��
����		  ��kxajd
�汾		  ��V1.0 20170406
-----------------------------------------------------------------
�޸ļ�¼:
��  �� 		 �汾		 �޸��� 	 �޸�����

*****************************************************************/
#ifndef _DRV_UART_H_
#define _DRV_UART_H_
#include "stm32f10x.h"
#include "stdio.h"
//#include "UHF_125K.h"
//uart4
#define GPIO_USART2                 GPIOA
#define GPIO_Pin_USART2_TX         GPIO_Pin_2  //
#define GPIO_Pin_USART2_RX         GPIO_Pin_3  //


//
void USART2_Config(u32 baud);


extern uint8_t RxBuffer[];
extern uint8_t RxCounter;
#endif
/******************* (C) COPYRIGHT 2017 kxadj *****END OF FILE****/










