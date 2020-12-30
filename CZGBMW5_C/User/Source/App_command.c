/***************************************************************				
文件名 	  ：App_command.c
相关文件	  ：
文件实现功能 ：
作者		  ：kxajd
版本		  ：V1.0 20170306
-----------------------------------------------------------------
修改记录:
日  期 		 版本		 修改人 	 修改内容

*****************************************************************/
#include "App_command.h"

//uint8_t gu8Ver[9]={0001};        //用于系统版本信息
uint8_t gu8PCconnect=0;
uint8_t gu8commandLength1=0;
uint8_t gu8commandLength2=0;
uint8_t commandStringBuffer[250]={0};
//命令列表,命令最长50字节
u8 commandStringList[][50] = \
{
	"view",\
	"list",\
	"gotoiap",\
	"debug",
};
//
/***************************************************************
函数名	     ：StringGetLength
功能		     ：获取字符串长度
输入参数说明 ：string   输入的字符串
输出参数说明 ：return 字符串长度
返回值说明    ：
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

//回调函数数组定义
Command_Proc_Func Command_Proc_Func_Table[] = 
{
	Help_Proc_Func,
	List_Proc_Func,
	iap_Func,
	debug_Func
};
/***************************************************************
函数名	     ：App_Command_Sever
功能		     ：上位机发过来的命令处理函数
输入参数说明 ：
输出参数说明 ：
返回值说明    ：
**************************************************************/
void App_Command_Sever(void)
{
	u8 i,j;
	for(i=0,j=0;i<gu8commandLength1;i++)
	{
		if((uartRxBuf[i]!=0x00)&&(uartRxBuf[i]!=' '))    //去掉空格以及多余数据
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
			//长度相同,比对每个字符
			for(j = 0; j < gu8commandLength1; j++)
			{
				if(commandStringBuffer[j] == commandStringList[i][j])continue;
				else break;
			}
			if(j == gu8commandLength1)//比对成功
			{
				//调用函数
				Command_Proc_Func_Table[i]();
				return;
			}
		}
		else
		{
			//直接长度不同,不需要比对了
			continue;
		}
	}
}


/******************* (C) COPYRIGHT 2017 kxadj *****END OF FILE****/

