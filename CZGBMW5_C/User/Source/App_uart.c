/***************************************************************				
�ļ��� 	  ��App_uart.c
����ļ�	  ��
�ļ�ʵ�ֹ��� ��Ӧ�ó����õ���uart
����		  ��kxajd
�汾		  ��V1.0 20170306
-----------------------------------------------------------------
�޸ļ�¼:
��  �� 		 �汾		 �޸��� 	 �޸�����

*****************************************************************/
#include "app_uart.h"
#include "App_command.h"
//����������
uint8_t RxBuffer[UART_RING_BUFSIZE];
uint8_t RxCounter;
uint8_t uartRxByte;
uint8_t uartRxStep;
uint8_t uartRxBufPos;
uint8_t uartRxBuf[UART_RING_BUFSIZE];
uint8_t uartRxDataLen;
uint8_t uartRxCirQueGet;
uint8_t TxBuffer[UART_RING_BUFSIZE];
uint8_t TxCounter;
uint8_t TxByte;
uint8_t UartTxCounter;

/***************************************************************
������	     ��uartReceive 
����		     ��֡���ݴ���
�������˵�� ��
�������˵�� ��
����ֵ˵��    ��
**************************************************************/
void (* const uartReceive[])(void) = {
	uartRx_SyncByte,			
//	uartRx_DataType,
//	uartRx_DataLen,
//	uartRx_DataAndCs
};
/***************************************************************
������	     ��uartRx_SyncByte 
����		     ��Ч��֡ͷ
�������˵�� ��
�������˵�� ��
����ֵ˵��    ��
**************************************************************/
void uartRx_SyncByte(void)
{
	if(uartRxByte == '\r')   //�յ��س�
	{
		uartRxStep=1;		
	}
	else 
	{
		if(uartRxStep)
		{
		  if(uartRxByte =='\n')    //�յ�����
			{
				//�յ�һ֡����
				gu8commandLength1=uartRxBufPos;
				App_Command_Sever();
				uartRxBufPos=0;
				uartRxStep=0;
			}
			else     //����һ֡ʧ��
			{
				uartRxStep=0;
			}
		}
		else   
		{
			uartRxBuf[uartRxBufPos] = uartRxByte;
			uartRxStep=0;
			uartRxBufPos++;
		}
		
	}
}
//void uartRx_SyncByte(void)
//{
//	if(uartRxByte == UART_SYN_BYTE)
//	{
//		uartRxStep++;		
//	}
//	else 
//	{
//		uartRxStep=0;
//	}
//}

/***************************************************************
������	     ��uartRx_DataType 
����		     ����ȡ֡����
�������˵�� ��
�������˵�� ��
����ֵ˵��    ��
**************************************************************/
void uartRx_DataType(void)
{
	uartRxBufPos               = 0;
	uartRxBuf[uartRxBufPos] = uartRxByte;
	uartRxBufPos++;
	uartRxStep++;	
}
/***************************************************************
������	     ��uartRx_DataLen 
����		     ����ȡ֡����
�������˵�� ��
�������˵�� ��
����ֵ˵��    ��
**************************************************************/
void uartRx_DataLen(void)
{
	if(uartRxByte <= UART_RING_BUFSIZE)
	{
		uartRxBuf[uartRxBufPos] = uartRxByte;
		uartRxDataLen              = uartRxByte+2;
		uartRxBufPos++;
		uartRxDataLen--;
		uartRxStep++;	
	}
	else
	{
		uartRxStep   = 0;			
	}
}

/***************************************************************
������	     ��uartRx_DataAndCs 
����		     ����ȡ֡�����Լ�Ч��֡
�������˵�� ��
�������˵�� ��
����ֵ˵��    ��
**************************************************************/
void uartRx_DataAndCs(void)
{
	uint8_t i, sum=0;

	uartRxBuf[uartRxBufPos] = uartRxByte;
	uartRxBufPos++;
	uartRxDataLen--;
	if(uartRxDataLen == 0)
	{	//rx finished
		//uartRxStep   = 0;
		//Check sum is ok?	
		for(i=0; i<uartRxBufPos-1; i++)	
		{

			sum += uartRxBuf[i];
		}
		uartRxBufPos = 0;
		if((sum^0xFF) ==uartRxBuf[ i])
		{			//check sum is OK
			Drv_uartTx_ACK();
			App_Command_Sever();
			uartRxStep=0;
		}
		else
		{					//check sum error	
			Drv_uartTx_NOACK();
			uartRxStep=0;
		}
	}
}
/***************************************************************
������	     ��Drv_LinuartTxCirQuePutInc 
����		     ��ʵ�ַ������ݵ��ۼ�
�������˵�� ��
�������˵�� ��
����ֵ˵��    ��
**************************************************************/
void Drv_LinuartTxCirQuePutInc(void)
{
	TxCounter++;
	if(TxCounter >= UART_RING_BUFSIZE)
	{

		TxCounter = 0;
	}

	if(TxCounter == UartTxCounter)
	{
		UartTxCounter=0;
		TxCounter=0;
		return;
	}
}
/***************************************************************
������	     ��Drv_uartTx_ACK 
����		     ���յ���ȷ���ݷ���һ��0xFF
�������˵�� ��
�������˵�� ��
����ֵ˵��    ��
**************************************************************/
void Drv_uartTx_ACK(void)
{
	TxBuffer[TxCounter]=ACK;
	Drv_LinuartTxCirQuePutInc();
}
/***************************************************************
������	     ��Drv_uartTx_NOACK 
����		     ���յ����������
�������˵�� ��
�������˵�� ��
����ֵ˵��    ��
**************************************************************/
void Drv_uartTx_NOACK(void)
{
	TxBuffer[TxCounter]=NACK;
	Drv_LinuartTxCirQuePutInc();
}

/***************************************************************
������	     ��Drv_LinuartTx 
����		     ��֡���ݴ��
�������˵�� ��data_type ֡ͷ *p_data ֡����   data_len ֡����
�������˵�� ��
����ֵ˵��    ��
**************************************************************/
void Drv_LinuartTx(const uint8_t data_type, 
                                    const uint8_t *p_data, const uint8_t data_len)
{	
	uint8_t i,checksum=0;
	
	TxBuffer[TxCounter]=UART_SYN_BYTE;
	Drv_LinuartTxCirQuePutInc();
	TxBuffer[TxCounter] = data_type;
	checksum+=TxBuffer[TxCounter];
	Drv_LinuartTxCirQuePutInc();
	TxBuffer[TxCounter] = data_len;
	checksum+=TxBuffer[TxCounter];
	Drv_LinuartTxCirQuePutInc();
	for(i=0; i<data_len; i++)
	{
		TxBuffer[TxCounter] = *(p_data+i);
		checksum+=TxBuffer[TxCounter];
		Drv_LinuartTxCirQuePutInc();
	}
	checksum = (checksum^0xff);
	TxBuffer[TxCounter]=checksum;
	Drv_LinuartTxCirQuePutInc();
}
/***************************************************************
������	     ��Drv_uartTxQuePolling 
����		     ���ж��Ƿ���������Ҫ����
�������˵�� ��
�������˵�� ��
����ֵ˵��    ��
**************************************************************/
void Drv_uartTxQuePolling(void)
{
	if(TxCounter==UartTxCounter)
	{
		return;
	}

	while(1)
	{
		TxByte=TxBuffer[UartTxCounter];
		USART_SendData(USART2, TxByte);
		while (!(USART2->SR & USART_FLAG_TXE));
		UartTxCounter++;
		if(UartTxCounter>=UART_RING_BUFSIZE)
		{
			UartTxCounter=0;
		}
		if(UartTxCounter==TxCounter)
		{
			break;
		}
	}
}
/***************************************************************
������	     ��Drv_uartRxQuePolling 
����		     ���ж��Ƿ���������Ҫ����
�������˵�� ��
�������˵�� ��
����ֵ˵��    ��
**************************************************************/
void	Drv_uartRxQuePolling(void)
{
	if(RxCounter == uartRxCirQueGet)
	{
		return;    		//RxQueue empty
	}
	/*--move data from rx circle queue to rx buffer--*/
	while(1)
	{
		uartRxByte = RxBuffer[uartRxCirQueGet];
		uartRx_SyncByte();					//receive data analyse
		uartRxCirQueGet++;
		if(uartRxCirQueGet >= UART_RING_BUFSIZE)
		{
			uartRxCirQueGet = 0;
		}
		if(uartRxCirQueGet == RxCounter)
		{
			break;
		}
	}		
}
/***************************************************************
������	     ��Drv_UartQueSever 
����		     ������λ��ͨѶ��uart������
�������˵�� ��
�������˵�� ��
����ֵ˵��    ��
**************************************************************/
void Drv_UartQueSever(void)
{
	Drv_uartTxQuePolling();
	Drv_uartRxQuePolling();
}

/******************* (C) COPYRIGHT 2017 kxadj *****END OF FILE****/
