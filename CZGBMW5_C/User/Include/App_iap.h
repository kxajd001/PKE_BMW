/***************************************************************				
�ļ��� 	  ��App_iap.h
����ļ�	  ��
�ļ�ʵ�ֹ��� ��
����		  ��kxajd
�汾		  ��V1.0 20170306
-----------------------------------------------------------------
�޸ļ�¼:
��  �� 		 �汾		 �޸��� 	 �޸�����

*****************************************************************/
#ifndef _APP_IAP_H_
#define _APP_IAP_H_

#define FLASH_IAP_ADDR		0x08000000  	//iap������ʼ��ַ(�����FLASH)
									      //����0X08000000~0X08003FFF�Ŀռ�ΪIAPʹ��
#define PARAM_COVERT_MAX_LENGTH		40	//�ַ���ת��Ϊ������ʱ����󻺴泤��,���ܳ���255
#define COMMAND_NUM					4	//��ǰshell��������������.��Ҫ�������������
#define APP_CONFIG_ADDR 		0X08002FFC	//���õ�ַ
#define APP_CONFIG_SET_VALUE	0XFFFF	//����ֵ   app
#define APP_CONFIG_CLEAR_VALUE	0X5555	//����ֵ   iap
#define QIANS_STAT_ADDR 		0x0801FFF0	//���õ�ַ
//
void iap_Func(void);
void List_Proc_Func(void);
void Help_Proc_Func(void);
void debug_Func(void);
extern bool Debug_flag;
#endif
/******************* (C) COPYRIGHT 2017 kxadj *****END OF FILE****/

