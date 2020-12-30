/***************************************************************				
文件名 	  ：App_flash.h
相关文件	  ：
文件实现功能 ：
作者		  ：kxajd
版本		  ：V1.0 20170308
-----------------------------------------------------------------
修改记录:
日  期 		 版本		 修改人 	 修改内容

*****************************************************************/
#ifndef _APP_FLASH_H_
#define _APP_FLASH_H_

//////////////////////////////////////////////////////////////////////////////////////////////////////
//用户根据自己的需要设置
#define STM32_FLASH_SIZE 256 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1                      //使能FLASH写入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址

uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);
void Test_Write(uint32_t WriteAddr,uint16_t WriteData);
void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);

#endif

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

