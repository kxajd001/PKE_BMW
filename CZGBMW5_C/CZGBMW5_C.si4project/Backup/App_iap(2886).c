//#include "APP_iap.h"
//#include "stm32f10x.h"
#include "UHF_125K.h"
bool Debug_flag = FALSE;

#define FLASH_IAP_ADDR		0x08000000  	//iap������ʼ��ַ(�����FLASH)
											//����0X08000000~0X0800FFFF�Ŀռ�ΪIAPʹ��

typedef  void (*iapfun)(void);				//����һ���������͵Ĳ���.
iapfun jump2iap;


//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

extern void DelayXms(vu32 nTime);
void iap_jump(u32 iapxaddr)
{
	if(((*(vu32*)iapxaddr)&0x2FFE0000)==0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.0x20000000��sram����ʼ��ַ,Ҳ�ǳ����ջ����ַ
	{ 
		printf("����ϵ����\r\n");
		DelayXms(20);
		printf("ok\r\n");
		DelayXms(20);
		jump2iap=(iapfun)*(vu32*)(iapxaddr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		MSR_MSP(*(vu32*)iapxaddr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		jump2iap();									//��ת��APP.
	}
	else
	{
		printf("iap program loss,please check\r\n");
	}
}

/***************************************************************
������	     ��Help_Proc_Func
����		     ��
�������˵�� ��
�������˵�� ��
����ֵ˵��    ��
**************************************************************/
void Help_Proc_Func(void)
{
	printf("CZG_BMW_NEW5: v0.1\r\n");
	printf("modify by   : CZG\r\n");
	printf("%s %s\r\n",  __DATE__,__TIME__);
	printf("*****************************************************\r\n");
}
/***************************************************************
������	     ��List_Proc_Func
����		     ��
�������˵�� ��
�������˵�� ��
����ֵ˵��    ��
**************************************************************/
void List_Proc_Func(void)
{
	printf("�鿴�汾 :veiw\r\n");
	printf("����ָ�� :list\r\n");
	printf("�������� :gotoiap\r\n");
	printf("��ӡ��Ƶ :debug\r\n");
	printf("*****************************************************\r\n");
}

void iap_Func(void)
{
	CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);    //��ֹCAN1
	CAN_ITConfig(CAN2, CAN_IT_FMP0, DISABLE);    //��ֹCAN2
	TIM_Cmd(TIM2, DISABLE);  //������ֹͣ����
	Test_Write(APP_CONFIG_ADDR,APP_CONFIG_CLEAR_VALUE);
	DelayXms(60);
	if(STMFLASH_ReadHalfWord(APP_CONFIG_ADDR)!=APP_CONFIG_CLEAR_VALUE)
	{
		Test_Write(APP_CONFIG_ADDR,APP_CONFIG_CLEAR_VALUE);
	}
	NVIC_SystemReset();//��λ
	DelayXms(5);
//	iap_jump(FLASH_IAP_ADDR);//��ת��iap�ĸ�λ������ַ
}

void debug_Func(void)
{
	printf("Debug_flag\r\n");
	Debug_flag = TRUE;
}











