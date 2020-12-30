/***************************************************************				
�ļ��� 	  ��App_flash.h
����ļ�	  ��
�ļ�ʵ�ֹ��� ��
����		  ��kxajd
�汾		  ��V1.0 20170308
-----------------------------------------------------------------
�޸ļ�¼:
��  �� 		 �汾		 �޸��� 	 �޸�����

*****************************************************************/
#ifndef _APP_FLASH_H_
#define _APP_FLASH_H_

//////////////////////////////////////////////////////////////////////////////////////////////////////
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 256 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1                      //ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ

uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);
void Test_Write(uint32_t WriteAddr,uint16_t WriteData);
void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);

#endif

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

