/***************************************************************				
文件名 	  ：App_iap.h
相关文件	  ：
文件实现功能 ：
作者		  ：kxajd
版本		  ：V1.0 20170306
-----------------------------------------------------------------
修改记录:
日  期 		 版本		 修改人 	 修改内容

*****************************************************************/
#ifndef _APP_IAP_H_
#define _APP_IAP_H_

#define FLASH_IAP_ADDR		0x08000000  	//iap程序起始地址(存放在FLASH)
									      //保留0X08000000~0X08003FFF的空间为IAP使用
#define PARAM_COVERT_MAX_LENGTH		40	//字符串转换为参数的时候最大缓存长度,不能超过255
#define COMMAND_NUM					4	//当前shell包含的命令总数.需要根据命令的数量
#define APP_CONFIG_ADDR 		0X08002FFC	//配置地址
#define APP_CONFIG_SET_VALUE	0XFFFF	//设置值   app
#define APP_CONFIG_CLEAR_VALUE	0X5555	//清零值   iap
#define QIANS_STAT_ADDR 		0x0801FFF0	//配置地址
//
void iap_Func(void);
void List_Proc_Func(void);
void Help_Proc_Func(void);
void debug_Func(void);
extern bool Debug_flag;
#endif
/******************* (C) COPYRIGHT 2017 kxadj *****END OF FILE****/

