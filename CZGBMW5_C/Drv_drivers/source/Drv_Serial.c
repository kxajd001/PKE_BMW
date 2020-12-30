/***************************************************************				
�ļ��� 	  ��drv_uart.c
����ļ�	  ��
�ļ�ʵ�ֹ��� ������д�Ĳ��������ļ�
����		  ��kxajd
�汾		  ��V1.0 20170306
-----------------------------------------------------------------
�޸ļ�¼:
��  �� 		 �汾		 �޸��� 	 �޸�����

*****************************************************************/
#include "Drv_Serial.h"
/***************************************************************
������	     ��void USART2_Config(u32 baud) 
����		     ��
�������˵�� ��
�������˵�� ��
����ֵ˵��    ��
**************************************************************/
void USART2_Config(u32 baud)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART2_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_USART2, &GPIO_InitStructure);

	/* Configure USART1 Rx (PA10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART2_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIO_USART2, &GPIO_InitStructure);

	//uart��ʼ��
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// Configure USARTx 
	USART_Init(USART2, &USART_InitStructure);

	// Enable USARTx Receive interrupts 
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	//
	// Enable the USARTx
	USART_Cmd(USART2, ENABLE);//ʹ��UART

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

/***************************************************************
������	     ��USART2_IRQHandler 
����		     ��
�������˵�� ��
�������˵�� ��
����ֵ˵��    ��
**************************************************************/
void USART2_IRQHandler(void)
{
	u8 ch = 0;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)//����жϷ���
	{
		ch = (u8)USART_ReceiveData(USART2);
		//��ȡ����
		RxBuffer[RxCounter]= ch;
		RxCounter++;
		if(RxCounter>=250)
		{
			RxCounter=0;
		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);	//����ж�
	}
}

//����printf֧��
//�������º����Ͳ���Ҫʹ��keil�е�Use MciroLIB 
#pragma import(__use_no_semihosting)             
	//������??aD����a��??��3?o����y				   
	struct __FILE 
	{ 
		int handle; 
	
	}; 
	
	FILE __stdout;		 
	//?����?_sys_exit()��?����?a��1��?��??��?��?�꨺?	 
_sys_exit(int x) 
	{ 
		x = x; 
	} 
	//???����?fputco����y 
	int fputc(int ch, FILE *f)
	{	   
		while((USART2->SR&0X40)==0);//  
		USART2->DR = (u8) ch;   
		return ch;
	}
/******************* (C) COPYRIGHT 2017 kxadj *****END OF FILE****/
