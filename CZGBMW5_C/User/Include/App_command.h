/***************************************************************				
文件名 	  ：App_command.h
相关文件	  ：
文件实现功能 ：
作者		  ：kxajd
版本		  ：V1.0 20170306
-----------------------------------------------------------------
修改记录:
日  期 		 版本		 修改人 	 修改内容

*****************************************************************/
#ifndef _APP_COMMAND_H_
#define _APP_COMMAND_H_
#include "UHF_125K.h"

typedef void (*Command_Proc_Func)(void);


//主机控制指令类型
enum _PC_CRTOL_TYPE_{
	SET_COMMAND                 =0x01,            //设置命令
	SEND_CD_DATA                =0x04           //请求原车数据
};

extern uint8_t gu8PCconnect;
extern uint8_t gu8commandLength1;
void App_Command_Sever(void);
#endif
/******************* (C) COPYRIGHT 2017 kxadj *****END OF FILE****/
