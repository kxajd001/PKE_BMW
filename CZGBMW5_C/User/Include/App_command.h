/***************************************************************				
�ļ��� 	  ��App_command.h
����ļ�	  ��
�ļ�ʵ�ֹ��� ��
����		  ��kxajd
�汾		  ��V1.0 20170306
-----------------------------------------------------------------
�޸ļ�¼:
��  �� 		 �汾		 �޸��� 	 �޸�����

*****************************************************************/
#ifndef _APP_COMMAND_H_
#define _APP_COMMAND_H_
#include "UHF_125K.h"

typedef void (*Command_Proc_Func)(void);


//��������ָ������
enum _PC_CRTOL_TYPE_{
	SET_COMMAND                 =0x01,            //��������
	SEND_CD_DATA                =0x04           //����ԭ������
};

extern uint8_t gu8PCconnect;
extern uint8_t gu8commandLength1;
void App_Command_Sever(void);
#endif
/******************* (C) COPYRIGHT 2017 kxadj *****END OF FILE****/
