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
vu32 TimingDelay = 0; //滴答时钟计数值
void DelayXms(vu32 nTime)
{ 
	SysTick->VAL   = ((u32)0x00000000);;	//清除滴答时钟计数器
	SysTick->CTRL |= ((u32)0x00000001);;	//开启滴答时钟计数器
	TimingDelay = (nTime << 4);
	while(TimingDelay != 0) {}
	SysTick->CTRL &= ((u32)0xFFFFFFFE);;	//停止滴答时钟计数器
}
void RCC_Configuration(void)
{  
	ErrorStatus HSEStartUpStatus;
	
	RCC_DeInit();//RCC system reset(for debug purpose)
	RCC_HSEConfig(RCC_HSE_ON);  //使用外部晶振
	HSEStartUpStatus = RCC_WaitForHSEStartUp(); //等待晶振
	if(HSEStartUpStatus == SUCCESS)
	{
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //使能FLASH BUFFER
		FLASH_SetLatency(FLASH_Latency_2);  //Flash 2个等待周期
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
		RCC_HSICmd(ENABLE);	//使能内部高速振荡
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);	//使能FLASH BUFFER
		FLASH_SetLatency(FLASH_Latency_2);	//Flash 2个等待周期
		RCC_HCLKConfig(RCC_SYSCLK_Div1);	//HCLK = SYSCLK
		RCC_PCLK2Config(RCC_HCLK_Div1);	//PCLK2 = HCLK
		RCC_PCLK1Config(RCC_HCLK_Div1);	//PCLK1 = HCLK
		while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET){}	//等待 HSI 就绪
		RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);	//选择 HSI 为系统时钟
		while(RCC_GetSYSCLKSource() != 0x00){}	//等待系统时钟就绪*/
	}
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC|/* RCC_APB2Periph_GPIOD|*/ RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR| RCC_APB1Periph_BKP| RCC_APB1Periph_UART4, ENABLE);
}

void IWDG_Configuration(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	// 打开使能，iwdg的寄存器有写保护,必须先写入0x5555，才能操作寄存器
	IWDG_SetPrescaler(IWDG_Prescaler_256);	//独立看门狗使用内部低速振荡器LSI，(40KHz)对LSI进行256分频
	IWDG_SetReload(3000);	// 设定独立看门狗计数器的计数值(0x000~0xFFF;0~4095)，复位时间为19.2s
	IWDG_ReloadCounter();	// 重载独立看门狗计数器，向寄存器写入0xAAAA，或者更新计数值
	IWDG_Enable();	// 开启看门狗，向寄存器写入0xCCCC即可
}
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO| RCC_APB2Periph_GPIOA| RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); //禁能JTAG GPIO
	GPIO_DeInit(GPIOA);
	GPIO_DeInit(GPIOB);
	GPIO_DeInit(GPIOC);
	//GPIO_DeInit(GPIOD);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_CAN1RX;      //CAN1驱动输入脚 RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	      //上拉输入
	GPIO_Init(GPIO_CAN1, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_CAN1TX;      //CAN1驱动输出脚 TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     //复用推挽输出
	GPIO_Init(GPIO_CAN1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_CAN2RX;    //CAN2驱动输入脚 RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	    //上拉输入
	GPIO_Init(GPIO_CAN2, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_CAN2TX;    //CAN2驱动输出脚 TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //复用推挽输出
	GPIO_Init(GPIO_CAN2, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_JP1| GPIO_PIN_JP2| GPIO_PIN_JP3; //拨码开关
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
	GPIO_Init(GPIO_JP, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_0R_OUT| GPIO_PIN_200R_OUT; //锁车解锁
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出    
	GPIO_Init(GPIO_CTRL_OUT, &GPIO_InitStructure);
	GPIO_ResetBits(GPIO_CTRL_OUT, GPIO_PIN_0R_OUT);
	GPIO_ResetBits(GPIO_CTRL_OUT, GPIO_PIN_200R_OUT);  

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_CTRL_KEY1 | GPIO_PIN_CTRL_KEY2;  //NXP_LOCK/OPEN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_Init(GPIO_CTRL_KEY, &GPIO_InitStructure);
	GPIO_ResetBits(GPIO_CTRL_KEY, GPIO_PIN_CTRL_KEY1);
	GPIO_ResetBits(GPIO_CTRL_KEY, GPIO_PIN_CTRL_KEY2);  

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_ACC_IN;  //ACC
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //悬浮输入
	GPIO_Init(GPIO_ACC_IN, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_NXP_LOCK | GPIO_PIN_NXP_KEY_OPEN;  //NXP_LOCK/OPEN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  //开漏输出
	GPIO_Init(GPIO_NXP_LOCK, &GPIO_InitStructure);
	GPIO_SetBits(GPIO_NXP_LOCK, GPIO_PIN_NXP_LOCK);
	GPIO_SetBits(GPIO_NXP_KEY_OPEN, GPIO_PIN_NXP_KEY_OPEN);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_NXP_POWER; //NXP_POWER
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  //开漏输出
	GPIO_Init(GPIO_NXP_POWER, &GPIO_InitStructure);
	GPIO_SetBits(GPIO_NXP_POWER, GPIO_PIN_NXP_POWER);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED; //NXP_POWER
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  //开漏输出
	GPIO_Init(GPIO_LED, &GPIO_InitStructure);
	GPIO_SetBits(GPIO_LED, GPIO_PIN_LED);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_125K_L| GPIO_PIN_125K_R|GPIO_PIN_125K_M; //低频发射
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出    
	GPIO_Init(GPIO_125K, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_IMMO_RX; //IMMO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//悬浮输入
	GPIO_Init(GPIO_IMMO_RX, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_UHF_RX; //UHF_RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//悬浮输入
	GPIO_Init(GPIO_UHF_RX, &GPIO_InitStructure); 
/*
	//si4355
	GPIO_InitStructure.GPIO_Pin = GPIO_RF_PWRDNPin|GPIO_RF_NSELPin;  //si4355 片选以及电源控制
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_Init(GPIO_RF_PWRDN, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_RF_MOSIPin|GPIO_RF_SCLKPin;  //SPI数据输出以及clk输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_Init(GPIO_RF_MOSI, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_RF_MISO_INPin; //SPI数据输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//悬浮输入
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
	PWR_BackupAccessCmd(ENABLE);	//允许BKP Domain
	BKP_DeInit();	//复位BKP Domain 
	RCC_LSICmd(ENABLE);	//开启内部低速时钟（40KHz）
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET){}	//等待 LSE 就绪
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);	//设置 RTC 时钟源
	RCC_RTCCLKCmd(ENABLE);	//开启 RTC 时钟
	RTC_WaitForSynchro();	//等待 RTC APB 寄存器上一动作完成
	RTC_SetPrescaler(4000);	//设置 RTC 基础周期 100ms
	RTC_WaitForLastTask();	//等待 RTC APB 寄存器上一动作完成
	RTC_SetCounter(0);
	RTC_WaitForLastTask();	//等待 RTC APB 寄存器上一动作完成 
	RTC_ITConfig(RTC_IT_SEC, ENABLE);	//使能 RTC 闹钟中断
	RTC_WaitForLastTask();	//等待 RTC APB 寄存器上一动作完成 
}
void SysTick_Configuration(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);	//设置 HCLK 为时钟源
	SysTick_Config(500);  //  500/8000000 周期
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

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	//定时器时间中断通道
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
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x03000);     //中断向量重置
	RCC_Configuration();
	GPIO_Configuration();
	RTC_Configuration(); 
	SysTick_Configuration();
	TIM_Configuration();
	CAN_Configuration();
	//vInitRadio();
#ifndef __DEBUG_PRO
	IWDG_Configuration();	//独立看门狗配置
#endif
	USART_Configuration();
	NVIC_Configuration();
	USER_Configuration();
	printf("run app");
	while(1)
	{
		#ifndef __DEBUG_PRO
		IWDG_ReloadCounter();	//喂狗
		#endif
		switch(EventFlag)
		{
			case EVENT_NOP:
			{
				#ifndef __DEBUG_PRO      
				__WFI(); 
				#endif
				break; //无事件进入睡眠
			}
			case EVENT_LFSEND:    
			{
				EventFlag = EVENT_NOP;
				OnEvnetLFSend();
				break; //低频发送事件
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
				break;  //其他未定义的事件
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
