//#include "APP_iap.h"
//#include "stm32f10x.h"
#include "UHF_125K.h"
bool Debug_flag = FALSE;

#define FLASH_IAP_ADDR		0x08000000  	//iap程序起始地址(存放在FLASH)
											//保留0X08000000~0X0800FFFF的空间为IAP使用

typedef  void (*iapfun)(void);				//定义一个函数类型的参数.
iapfun jump2iap;


//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

extern void DelayXms(vu32 nTime);
void iap_jump(u32 iapxaddr)
{
	if(((*(vu32*)iapxaddr)&0x2FFE0000)==0x20000000)	//检查栈顶地址是否合法.0x20000000是sram的起始地址,也是程序的栈顶地址
	{ 
		printf("必须断电操作\r\n");
		DelayXms(20);
		printf("ok\r\n");
		DelayXms(20);
		jump2iap=(iapfun)*(vu32*)(iapxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(vu32*)iapxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		jump2iap();									//跳转到APP.
	}
	else
	{
		printf("iap program loss,please check\r\n");
	}
}

/***************************************************************
函数名	     ：Help_Proc_Func
功能		     ：
输入参数说明 ：
输出参数说明 ：
返回值说明    ：
**************************************************************/
void Help_Proc_Func(void)
{
	printf("CZG_BMW_NEW5: v0.1\r\n");
	printf("modify by   : CZG\r\n");
	printf("%s %s\r\n",  __DATE__,__TIME__);
	printf("*****************************************************\r\n");
}
/***************************************************************
函数名	     ：List_Proc_Func
功能		     ：
输入参数说明 ：
输出参数说明 ：
返回值说明    ：
**************************************************************/
void List_Proc_Func(void)
{
	printf("查看版本 :veiw\r\n");
	printf("操作指引 :list\r\n");
	printf("进入升级 :gotoiap\r\n");
	printf("打印低频 :debug\r\n");
	printf("*****************************************************\r\n");
}

void iap_Func(void)
{
	CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);    //禁止CAN1
	CAN_ITConfig(CAN2, CAN_IT_FMP0, DISABLE);    //禁止CAN2
	TIM_Cmd(TIM2, DISABLE);  //计数器停止工作
	Test_Write(APP_CONFIG_ADDR,APP_CONFIG_CLEAR_VALUE);
	DelayXms(60);
	if(STMFLASH_ReadHalfWord(APP_CONFIG_ADDR)!=APP_CONFIG_CLEAR_VALUE)
	{
		Test_Write(APP_CONFIG_ADDR,APP_CONFIG_CLEAR_VALUE);
	}
	NVIC_SystemReset();//复位
	DelayXms(5);
//	iap_jump(FLASH_IAP_ADDR);//跳转到iap的复位向量地址
}

void debug_Func(void)
{
	printf("Debug_flag\r\n");
	Debug_flag = TRUE;
}











