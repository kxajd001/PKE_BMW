/***************************************************************				
文件名 	  ：App_flash.c
相关文件	  ：
文件实现功能 ：
作者		  ：kxajd
版本		  ：V1.0 20170308
-----------------------------------------------------------------
修改记录:
日  期 		 版本		 修改人 	 修改内容

*****************************************************************/
#include "UHF_125K.h"

#define STM_SECTOR_SIZE 1024 //字节

uint16_t STMFLASH_BUF[STM_SECTOR_SIZE/2];//最多是2K字节

/***************************************************************
函数名	     ：STMFLASH_Write_NoCheck
功能		     ：不检查，直接写入
输入参数说明 ：WriteAddr:起始地址
			pBuffer:数据指针
			NumToWrite:半字(16位)数
输出参数说明 ：
返回值说明    ：此地址对应的数据
**************************************************************/
void STMFLASH_Write_NoCheck(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)   
{ 			 		 
	uint16_t i;
	for(i=0;i<NumToWrite;i++)
		{
			FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
			WriteAddr+=2;//地址增加2.
		}  
} 
/***************************************************************
函数名	     ：STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead) 	
功能		     ：从指定地址开始读出指定长度的数据
输入参数说明 ：ReadAddr:起始地址
			pBuffer:数据指针
			NumToWrite:半字(16位)数
输出参数说明 ：
返回值说明    ：
**************************************************************/
void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)   	
{
	uint16_t i;
	for(i=0;i<NumToRead;i++)
		{
			pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
			ReadAddr+=2;//偏移2个字节.	
		}
}



/***************************************************************
函数名	     ：uint16_t STMFLASH_ReadHalfWord(uint32_t faddr) 	
功能		     ：读取指定地址的半字
输入参数说明 ：faddr 读地址(此地址必须为2的倍数!!)
输出参数说明 ：
返回值说明    ：此地址对应的数据
**************************************************************/
void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)	
{
	uint32_t secpos;	   //扇区地址
	uint16_t secoff;	   //扇区内偏移地址(16位字计算)
	uint16_t secremain; //扇区内剩余地址(16位字计算)	   
	uint16_t i;    
	uint32_t offaddr;   //去掉0X08000000后的地址
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//非法地址
	FLASH_Unlock();						//解锁
	offaddr=WriteAddr-STM32_FLASH_BASE;		        //实际偏移地址.
	secpos=offaddr/STM_SECTOR_SIZE;			//扇区地址  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//在扇区内的偏移(2个字节为基本单位.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//扇区剩余空间大小   
	if(NumToWrite<=secremain)secremain=NumToWrite;//不大于该扇区范围
	while(1) 
		{	
			STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//读出整个扇区的内容
			for(i=0;i<secremain;i++)//校验数据
				{
					if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//需要擦除  	  
				}
			if(i<secremain)//需要擦除
				{
					FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//擦除这个扇区
					for(i=0;i<secremain;i++)//复制
						{
							STMFLASH_BUF[i+secoff]=pBuffer[i];	  
						}
					STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//写入整个扇区  
				}
			else
				{
					STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间. 	
				}
			if(NumToWrite==secremain)break;//写入结束了
			else//写入未结束
				{
					secpos++;				//扇区地址增1
					secoff=0;				//偏移位置为0 	 
					pBuffer+=secremain;  	//指针偏移
					WriteAddr+=secremain;	//写地址偏移	   
					NumToWrite-=secremain;	//字节(16位)数递减
					if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//下一个扇区还是写不完
					else secremain=NumToWrite;//下一个扇区可以写完了
				}									 
		};	
	FLASH_Lock();//上锁
}
/***************************************************************
函数名	     ：uint16_t STMFLASH_ReadHalfWord(uint32_t faddr) 	
功能		     ：读取指定地址的半字
输入参数说明 ：faddr 读地址(此地址必须为2的倍数!!)
输出参数说明 ：
返回值说明    ：此地址对应的数据
**************************************************************/
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr)
{
    return *(vu16*)faddr; 
}
/***************************************************************
函数名	     ：Test_Write(uint32_t WriteAddr,uint16_t WriteData)   	
功能		     ：把data写入对应的flash地址
输入参数说明 ：writeAddr 写的起始地址 writeData要写入的数据
输出参数说明 ：
返回值说明    ：
**************************************************************/
void Test_Write(uint32_t WriteAddr,uint16_t WriteData)   	
{
    STMFLASH_Write(WriteAddr,&WriteData,1);//写入一个字 
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
