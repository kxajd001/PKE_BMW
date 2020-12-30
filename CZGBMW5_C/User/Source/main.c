/**
  ******************************************************************************
  * @file    CAN/Normal/main.c 
  * @author  MCD Application Team
  * @version V3.1.2
  * @date    09/28/2009
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "platform_config.h"
#include "UHF_125K.h"
/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup CAN_Normal
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile EventState EventFlag;

/* Private function prototypes -----------------------------------------------*/
vu32 TimingDelay = 0; //�δ�ʱ�Ӽ���ֵ
void DelayXms(vu32 nTime)
{ 
	SysTick->VAL   = ((u32)0x00000000);;	//����δ�ʱ�Ӽ�����
	SysTick->CTRL |= ((u32)0x00000001);;	//�����δ�ʱ�Ӽ�����
	TimingDelay = (nTime << 4);
	while(TimingDelay != 0) {}
	SysTick->CTRL &= ((u32)0xFFFFFFFE);;	//ֹͣ�δ�ʱ�Ӽ�����
}
void RCC_Configuration(void)
{  
	ErrorStatus HSEStartUpStatus;
	
	RCC_DeInit();//RCC system reset(for debug purpose)
	RCC_HSEConfig(RCC_HSE_ON);  //ʹ���ⲿ����
	HSEStartUpStatus = RCC_WaitForHSEStartUp(); //�ȴ�����
	if(HSEStartUpStatus == SUCCESS)
	{
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //ʹ��FLASH BUFFER
		FLASH_SetLatency(FLASH_Latency_2);  //Flash 2���ȴ�����
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 
		RCC_PCLK2Config(RCC_HCLK_Div1); 
		RCC_PCLK1Config(RCC_HCLK_Div1);
		//RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_2);  //8Mhz
		//RCC_PLLCmd(ENABLE);
		//while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){}
		//RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		//while(RCC_GetSYSCLKSource() != 0x08){}
		while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET){}
		RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);
		while(RCC_GetSYSCLKSource() != 0x04){}
	} 
	else 
	{
		RCC_HSICmd(ENABLE);	//ʹ���ڲ�������
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);	//ʹ��FLASH BUFFER
		FLASH_SetLatency(FLASH_Latency_2);	//Flash 2���ȴ�����
		RCC_HCLKConfig(RCC_SYSCLK_Div1);	//HCLK = SYSCLK
		RCC_PCLK2Config(RCC_HCLK_Div1);	//PCLK2 = HCLK
		RCC_PCLK1Config(RCC_HCLK_Div1);	//PCLK1 = HCLK
		while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET){}	//�ȴ� HSI ����
		RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);	//ѡ�� HSI Ϊϵͳʱ��
		while(RCC_GetSYSCLKSource() != 0x00){}	//�ȴ�ϵͳʱ�Ӿ���*/
	}
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC|/* RCC_APB2Periph_GPIOD|*/ RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR| RCC_APB1Periph_BKP| RCC_APB1Periph_UART4, ENABLE);
}

void IWDG_Configuration(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	// ��ʹ�ܣ�iwdg�ļĴ�����д����,������д��0x5555�����ܲ����Ĵ���
	IWDG_SetPrescaler(IWDG_Prescaler_256);	//�������Ź�ʹ���ڲ���������LSI��(40KHz)��LSI����256��Ƶ
	IWDG_SetReload(3000);	// �趨�������Ź��������ļ���ֵ(0x000~0xFFF;0~4095)����λʱ��Ϊ19.2s
	IWDG_ReloadCounter();	// ���ض������Ź�����������Ĵ���д��0xAAAA�����߸��¼���ֵ
	IWDG_Enable();	// �������Ź�����Ĵ���д��0xCCCC����
}
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO| RCC_APB2Periph_GPIOA| RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); //����JTAG GPIO
	GPIO_DeInit(GPIOA);
	GPIO_DeInit(GPIOB);
	GPIO_DeInit(GPIOC);
	//GPIO_DeInit(GPIOD);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_CAN1RX;      //CAN1��������� RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	      //��������
	GPIO_Init(GPIO_CAN1, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_CAN1TX;      //CAN1��������� TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     //�����������
	GPIO_Init(GPIO_CAN1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_CAN2RX;    //CAN2��������� RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	    //��������
	GPIO_Init(GPIO_CAN2, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_CAN2TX;    //CAN2��������� TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //�����������
	GPIO_Init(GPIO_CAN2, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_JP1| GPIO_PIN_JP2| GPIO_PIN_JP3; //���뿪��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //��������
	GPIO_Init(GPIO_JP, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_0R_OUT| GPIO_PIN_200R_OUT; //��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������    
	GPIO_Init(GPIO_CTRL_OUT, &GPIO_InitStructure);
	GPIO_ResetBits(GPIO_CTRL_OUT, GPIO_PIN_0R_OUT);
	GPIO_ResetBits(GPIO_CTRL_OUT, GPIO_PIN_200R_OUT);  

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_CTRL_KEY1 | GPIO_PIN_CTRL_KEY2;  //NXP_LOCK/OPEN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	GPIO_Init(GPIO_CTRL_KEY, &GPIO_InitStructure);
	GPIO_ResetBits(GPIO_CTRL_KEY, GPIO_PIN_CTRL_KEY1);
	GPIO_ResetBits(GPIO_CTRL_KEY, GPIO_PIN_CTRL_KEY2);  

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_ACC_IN;  //ACC
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIO_ACC_IN, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_NXP_LOCK | GPIO_PIN_NXP_KEY_OPEN;  //NXP_LOCK/OPEN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  //��©���
	GPIO_Init(GPIO_NXP_LOCK, &GPIO_InitStructure);
	GPIO_SetBits(GPIO_NXP_LOCK, GPIO_PIN_NXP_LOCK);
	GPIO_SetBits(GPIO_NXP_KEY_OPEN, GPIO_PIN_NXP_KEY_OPEN);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_NXP_POWER; //NXP_POWER
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  //��©���
	GPIO_Init(GPIO_NXP_POWER, &GPIO_InitStructure);
	GPIO_SetBits(GPIO_NXP_POWER, GPIO_PIN_NXP_POWER);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED; //NXP_POWER
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  //��©���
	GPIO_Init(GPIO_LED, &GPIO_InitStructure);
	GPIO_SetBits(GPIO_LED, GPIO_PIN_LED);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_125K_L| GPIO_PIN_125K_R|GPIO_PIN_125K_M; //��Ƶ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������    
	GPIO_Init(GPIO_125K, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_IMMO_RX; //IMMO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
	GPIO_Init(GPIO_IMMO_RX, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_UHF_RX; //UHF_RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
	GPIO_Init(GPIO_UHF_RX, &GPIO_InitStructure); 
/*
	//si4355
	GPIO_InitStructure.GPIO_Pin = GPIO_RF_PWRDNPin|GPIO_RF_NSELPin;  //si4355 Ƭѡ�Լ���Դ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	GPIO_Init(GPIO_RF_PWRDN, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_RF_MOSIPin|GPIO_RF_SCLKPin;  //SPI��������Լ�clk���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	GPIO_Init(GPIO_RF_MOSI, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_RF_MISO_INPin; //SPI��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
	GPIO_Init(GPIO_RF_MISO_IN, &GPIO_InitStructure); */
}

void SetGpioPinNxpOpen(uint8_t PinNxpOpenState)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_NXP_KEY_OPEN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIO_NXP_KEY_OPEN, &GPIO_InitStructure);
	if(PinNxpOpenState != Bit_SET) 
	{
		GPIO_ResetBits(GPIO_NXP_KEY_OPEN, GPIO_PIN_NXP_KEY_OPEN);
	}
	else 
	{
		GPIO_SetBits(GPIO_NXP_KEY_OPEN, GPIO_PIN_NXP_KEY_OPEN);
	}
}
void SetGpioPinStudyIn(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_STUDY_IN; //STUDY
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//
	GPIO_Init(GPIO_STUDY_IN, &GPIO_InitStructure);
}
extern void USART2_Config(u32 baud);
void USART_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx;
	USART_Init(UART4, &USART_InitStructure);
	USART_Cmd(UART4, DISABLE);
	USART2_Config(115200);
}

void RTC_Configuration(void)
{
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR| RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);	//����BKP Domain
	BKP_DeInit();	//��λBKP Domain 
	RCC_LSICmd(ENABLE);	//�����ڲ�����ʱ�ӣ�40KHz��
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET){}	//�ȴ� LSE ����
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);	//���� RTC ʱ��Դ
	RCC_RTCCLKCmd(ENABLE);	//���� RTC ʱ��
	RTC_WaitForSynchro();	//�ȴ� RTC APB �Ĵ�����һ�������
	RTC_SetPrescaler(4000);	//���� RTC �������� 100ms
	RTC_WaitForLastTask();	//�ȴ� RTC APB �Ĵ�����һ�������
	RTC_SetCounter(0);
	RTC_WaitForLastTask();	//�ȴ� RTC APB �Ĵ�����һ������� 
	RTC_ITConfig(RTC_IT_SEC, ENABLE);	//ʹ�� RTC �����ж�
	RTC_WaitForLastTask();	//�ȴ� RTC APB �Ĵ�����һ������� 
}
void SysTick_Configuration(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);	//���� HCLK Ϊʱ��Դ
	SysTick_Config(500);  //  500/8000000 ����
}
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef  NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	//��ʱ��ʱ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);  

	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);



	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;//IRQn_Type
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

 int main(void)
{
	//SystemInit();
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x03000);     //�ж���������
	RCC_Configuration();
	GPIO_Configuration();
	RTC_Configuration(); 
	SysTick_Configuration();
	TIM_Configuration();
	CAN_Configuration();
	//vInitRadio();
#ifndef __DEBUG_PRO
	IWDG_Configuration();	//�������Ź�����
#endif
	USART_Configuration();
	NVIC_Configuration();
	USER_Configuration();
	printf("run app");
	while(1)
	{
		#ifndef __DEBUG_PRO
		IWDG_ReloadCounter();	//ι��
		#endif
		switch(EventFlag)
		{
			case EVENT_NOP:
			{
				#ifndef __DEBUG_PRO      
				__WFI(); 
				#endif
				break; //���¼�����˯��
			}
			case EVENT_LFSEND:    
			{
				EventFlag = EVENT_NOP;
				OnEvnetLFSend();
				break; //��Ƶ�����¼�
			}
			case EVENT_REMOTE_OPEN: 
			{
				EventFlag = EVENT_NOP; 
				OnEventRemoteOpen();
				break;
			}
			case EVENT_REMOTE_CLOSE:
			{
				EventFlag = EVENT_NOP; 
				OnEventRemoteClose(); 
				break;
			}
			case EVENT_STUDY_KEY:  
			{
				EventFlag = EVENT_NOP;
				OnEventStudy(); 
				break;
			}
			default:
			{
				EventFlag = EVENT_NOP;
				break;  //����δ������¼�
			}
		}
		CheckLfRemote();
		CheckconfigState();
		Drv_UartQueSever();
		if(EventFlag == EVENT_NOP)
		{
			CheckOnTime();
		}
	}
}
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
