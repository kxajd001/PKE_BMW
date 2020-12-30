/***************************************************************				
文件名 	  ：App_uart.c
相关文件	  ：
文件实现功能 ：应用程序用到的uart
作者		  ：kxajd
版本		  ：V1.0 20170306
-----------------------------------------------------------------
修改记录:
日  期 		 版本		 修改人 	 修改内容

*****************************************************************/
#include "app_uart.h"
#include "App_command.h"
//变量声明区
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
函数名	     ：uartReceive 
功能		     ：帧数据处理
输入参数说明 ：
输出参数说明 ：
返回值说明    ：
**************************************************************/
void (* const uartReceive[])(void) = {
	uartRx_SyncByte,			
//	uartRx_DataType,
//	uartRx_DataLen,
//	uartRx_DataAndCs
};
/***************************************************************
函数名	     ：uartRx_SyncByte 
功能		     ：效验帧头
输入参数说明 ：
输出参数说明 ：
返回值说明    ：
**************************************************************/
void uartRx_SyncByte(void)
{
	if(uartRxByte == '\r')   //收到回车
	{
		uartRxStep=1;		
	}
	else 
	{
		if(uartRxStep)
		{
		  if(uartRxByte =='\n')    //收到换行
			{
				//收到一帧命令
				gu8commandLength1=uartRxBufPos;
				App_Command_Sever();
				uartRxBufPos=0;
				uartRxStep=0;
			}
			else     //接收一帧失败
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
函数名	     ：uartRx_DataType 
功能		     ：获取帧类型
输入参数说明 ：
输出参数说明 ：
返回值说明    ：
**************************************************************/
void uartRx_DataType(void)
{
	uartRxBufPos               = 0;
	uartRxBuf[uartRxBufPos] = uartRxByte;
	uartRxBufPos++;
	uartRxStep++;	
}
/***************************************************************
函数名	     ：uartRx_DataLen 
功能		     ：获取帧长度
输入参数说明 ：
输出参数说明 ：
返回值说明    ：
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
函数名	     ：uartRx_DataAndCs 
功能		     ：获取帧数据以及效验帧
输入参数说明 ：
输出参数说明 ：
返回值说明    ：
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
函数名	     ：Drv_LinuartTxCirQuePutInc 
功能		     ：实现发送数据的累加
输入参数说明 ：
输出参数说明 ：
返回值说明    ：
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
函数名	     ：Drv_uartTx_ACK 
功能		     ：收到正确数据返回一个0xFF
输入参数说明 ：
输出参数说明 ：
返回值说明    ：
**************************************************************/
void Drv_uartTx_ACK(void)
{
	TxBuffer[TxCounter]=ACK;
	Drv_LinuartTxCirQuePutInc();
}
/***************************************************************
函数名	     ：Drv_uartTx_NOACK 
功能		     ：收到错误的数据
输入参数说明 ：
输出参数说明 ：
返回值说明    ：
**************************************************************/
void Drv_uartTx_NOACK(void)
{
	TxBuffer[TxCounter]=NACK;
	Drv_LinuartTxCirQuePutInc();
}

/***************************************************************
函数名	     ：Drv_LinuartTx 
功能		     ：帧数据打包
输入参数说明 ：data_type 帧头 *p_data 帧数据   data_len 帧长度
输出参数说明 ：
返回值说明    ：
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
函数名	     ：Drv_uartTxQuePolling 
功能		     ：判断是否有数据需要发送
输入参数说明 ：
输出参数说明 ：
返回值说明    ：
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
函数名	     ：Drv_uartRxQuePolling 
功能		     ：判断是否有数据需要接收
输入参数说明 ：
输出参数说明 ：
返回值说明    ：
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
函数名	     ：Drv_UartQueSever 
功能		     ：与上位机通讯的uart服务函数
输入参数说明 ：
输出参数说明 ：
返回值说明    ：
**************************************************************/
void Drv_UartQueSever(void)
{
	Drv_uartTxQuePolling();
	Drv_uartRxQuePolling();
}

/******************* (C) COPYRIGHT 2017 kxadj *****END OF FILE****/
