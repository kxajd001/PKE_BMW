/***************************************************************				
文件名 	  ：App_uart.h
相关文件	  ：
文件实现功能 ：
作者		  ：kxajd
版本		  ：V1.0 20170306
-----------------------------------------------------------------
修改记录:
日  期 		 版本		 修改人 	 修改内容

*****************************************************************/
#ifndef _APP_UART_H_
#define _APP_UART_H_
#include "UHF_125K.h"
#define UART_RING_BUFSIZE  (250)
#define UART_SYN_BYTE         (0x2E)
#define PACKET_SIZE               (128)

#define ACK                              (0xFF)
#define NACK                            (0xF0)



extern uint8_t uartRxBuf[UART_RING_BUFSIZE];
extern uint8_t uartRxStep;
extern uint8_t RxBuffer[UART_RING_BUFSIZE];
extern uint8_t RxCounter;

//函数声明区
void uartRx_SyncByte(void);
void uartRx_DataType(void);
void uartRx_DataLen(void);
void uartRx_DataAndCs(void);
void Drv_uartTx_ACK(void);
void Drv_uartTx_NOACK(void);
void Drv_UartQueSever(void);
void Drv_LinuartTx(const uint8_t data_type,const  uint8_t *p_data, const uint8_t data_len);
#endif

/******************* (C) COPYRIGHT 2017 kxadj *****END OF FILE****/

