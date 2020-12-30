/***************************************************************				
�ļ��� 	  ��App_flash.c
����ļ�	  ��
�ļ�ʵ�ֹ��� ��
����		  ��kxajd
�汾		  ��V1.0 20170308
-----------------------------------------------------------------
�޸ļ�¼:
��  �� 		 �汾		 �޸��� 	 �޸�����

*****************************************************************/
#include "UHF_125K.h"

#define STM_SECTOR_SIZE 1024 //�ֽ�

uint16_t STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����2K�ֽ�

/***************************************************************
������	     ��STMFLASH_Write_NoCheck
����		     ������飬ֱ��д��
�������˵�� ��WriteAddr:��ʼ��ַ
			pBuffer:����ָ��
			NumToWrite:����(16λ)��
�������˵�� ��
����ֵ˵��    ���˵�ַ��Ӧ������
**************************************************************/
void STMFLASH_Write_NoCheck(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)   
{ 			 		 
	uint16_t i;
	for(i=0;i<NumToWrite;i++)
		{
			FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
			WriteAddr+=2;//��ַ����2.
		}  
} 
/***************************************************************
������	     ��STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead) 	
����		     ����ָ����ַ��ʼ����ָ�����ȵ�����
�������˵�� ��ReadAddr:��ʼ��ַ
			pBuffer:����ָ��
			NumToWrite:����(16λ)��
�������˵�� ��
����ֵ˵��    ��
**************************************************************/
void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)   	
{
	uint16_t i;
	for(i=0;i<NumToRead;i++)
		{
			pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
			ReadAddr+=2;//ƫ��2���ֽ�.	
		}
}



/***************************************************************
������	     ��uint16_t STMFLASH_ReadHalfWord(uint32_t faddr) 	
����		     ����ȡָ����ַ�İ���
�������˵�� ��faddr ����ַ(�˵�ַ����Ϊ2�ı���!!)
�������˵�� ��
����ֵ˵��    ���˵�ַ��Ӧ������
**************************************************************/
void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)	
{
	uint32_t secpos;	   //������ַ
	uint16_t secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	uint16_t secremain; //������ʣ���ַ(16λ�ּ���)	   
	uint16_t i;    
	uint32_t offaddr;   //ȥ��0X08000000��ĵ�ַ
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	FLASH_Unlock();						//����
	offaddr=WriteAddr-STM32_FLASH_BASE;		        //ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;			//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//����ʣ��ռ��С   
	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
	while(1) 
		{	
			STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//������������������
			for(i=0;i<secremain;i++)//У������
				{
					if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����  	  
				}
			if(i<secremain)//��Ҫ����
				{
					FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
					for(i=0;i<secremain;i++)//����
						{
							STMFLASH_BUF[i+secoff]=pBuffer[i];	  
						}
					STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������  
				}
			else
				{
					STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 	
				}
			if(NumToWrite==secremain)break;//д�������
			else//д��δ����
				{
					secpos++;				//������ַ��1
					secoff=0;				//ƫ��λ��Ϊ0 	 
					pBuffer+=secremain;  	//ָ��ƫ��
					WriteAddr+=secremain;	//д��ַƫ��	   
					NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
					if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
					else secremain=NumToWrite;//��һ����������д����
				}									 
		};	
	FLASH_Lock();//����
}
/***************************************************************
������	     ��uint16_t STMFLASH_ReadHalfWord(uint32_t faddr) 	
����		     ����ȡָ����ַ�İ���
�������˵�� ��faddr ����ַ(�˵�ַ����Ϊ2�ı���!!)
�������˵�� ��
����ֵ˵��    ���˵�ַ��Ӧ������
**************************************************************/
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr)
{
    return *(vu16*)faddr; 
}
/***************************************************************
������	     ��Test_Write(uint32_t WriteAddr,uint16_t WriteData)   	
����		     ����dataд���Ӧ��flash��ַ
�������˵�� ��writeAddr д����ʼ��ַ writeDataҪд�������
�������˵�� ��
����ֵ˵��    ��
**************************************************************/
void Test_Write(uint32_t WriteAddr,uint16_t WriteData)   	
{
    STMFLASH_Write(WriteAddr,&WriteData,1);//д��һ���� 
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
