/***************************************************************				
�ļ��� 	  ��App_command.c
����ļ�	  ��
�ļ�ʵ�ֹ��� ��
����		  ��kxajd
�汾		  ��V1.0 20170306
-----------------------------------------------------------------
�޸ļ�¼:
��  �� 		 �汾		 �޸��� 	 �޸�����

*****************************************************************/
#include "App_command.h"

//uint8_t gu8Ver[9]={0001};        //����ϵͳ�汾��Ϣ
uint8_t gu8PCconnect=0;
uint8_t gu8commandLength1=0;
uint8_t gu8commandLength2=0;
uint8_t commandStringBuffer[250]={0};
//�����б�,�����50�ֽ�
u8 commandStringList[][50] = \
{
	"view",\
	"list",\
	"gotoiap",\
	"debug",
};
//
/***************************************************************
������	     ��StringGetLength
����		     ����ȡ�ַ�������
�������˵�� ��string   ������ַ���
�������˵�� ��return �ַ�������
����ֵ˵��    ��
**************************************************************/
u8 StringGetLength(u8* string)
{
	u8 i = 0; 
	while(*(string+i) != '\0')
	{
		i++;
	}
	return i;
}

//�ص��������鶨��
Command_Proc_Func Command_Proc_Func_Table[] = 
{
	Help_Proc_Func,
	List_Proc_Func,
	iap_Func,
	debug_Func
};
/***************************************************************
������	     ��App_Command_Sever
����		     ����λ�����������������
�������˵�� ��
�������˵�� ��
����ֵ˵��    ��
**************************************************************/
void App_Command_Sever(void)
{
	u8 i,j;
	for(i=0,j=0;i<gu8commandLength1;i++)
	{
		if((uartRxBuf[i]!=0x00)&&(uartRxBuf[i]!=' '))    //ȥ���ո��Լ���������
		{
			commandStringBuffer[j]=uartRxBuf[i];
			j++;
		}
	}
	gu8commandLength1=j;
	for(i = 0; i < COMMAND_NUM; i++)
	{
		gu8commandLength2 = StringGetLength(commandStringList[i]);
		if(gu8commandLength1 == gu8commandLength2)
		{
			//������ͬ,�ȶ�ÿ���ַ�
			for(j = 0; j < gu8commandLength1; j++)
			{
				if(commandStringBuffer[j] == commandStringList[i][j])continue;
				else break;
			}
			if(j == gu8commandLength1)//�ȶԳɹ�
			{
				//���ú���
				Command_Proc_Func_Table[i]();
				return;
			}
		}
		else
		{
			//ֱ�ӳ��Ȳ�ͬ,����Ҫ�ȶ���
			continue;
		}
	}
}


/******************* (C) COPYRIGHT 2017 kxadj *****END OF FILE****/

