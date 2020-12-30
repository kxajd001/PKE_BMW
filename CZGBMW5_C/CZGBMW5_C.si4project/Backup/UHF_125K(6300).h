#ifndef _UHF_125K_H
#define _UHF_125K_H 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "platform_config.h"
#include "App_flash.h"
#include "App_iap.h"
#include "App_uart.h"
#include "stdio.h"
/* Exported types ------------------------------------------------------------*/
#define TIMER_MAX		7					//��ʱ������
#define RemoteID_Length 15              //   ң����֡���ݳ���
typedef enum {
  EVENT_NOP = 0, 
  EVENT_LFSEND,
  EVENT_REMOTE_OPEN,
  EVENT_REMOTE_CLOSE,
  EVENT_STUDY_KEY,
  EVENT_HRON
//  EVENT_CARFD
} EventState;
typedef enum {
  DOOR_NOP = 0,
  DOOR_CLOSE,
  DOOR_UPWINING,
  DOOR_CLOSED,
  DOOR_OPEN
}DoorState;
typedef enum {
  CAR_POWER_OFF = 0, 
  CAR_POWER_ACC, 
  CAR_POWER_ON, 
  CAR_POWER_STARTING,
  CAR_POWER_RUN
}CarState;
typedef enum {
  TIMER_LF = 0, 
  TIMER_UPWIN = 1,  
	TIMER_RESET = 2,
	TIMER_BACKLFFIND = 3,
	TIMER_POWEROFF =4,
	TIMER_DOORHRON =5,
	TIMER_ANTI_WAIT =6,
  TIMER_END = TIMER_MAX
} TIMER_STATUS;
typedef enum{
	LF_NO_SLEEP=0,
	LF_SLEEPED
}LF_SLEEP_STATUS;
  
#define REMOTE_ID_ADDR_HOME   ((u32)0x0801F000UL)
#define COUNT_ADDR_HOME   	  ((u32)0x08010000UL)
#define CONFIG_ADD_HOME       ((u32)0x0801F800UL)

#define x100ms(n)           (n)
#ifdef Time2_1us
#define TIM2_xus(n)         (n)
#else
#define TIM2_xus(n)         (n/2)
#endif
#define _RESET_WAIT_TIME		x100ms(50)
#define _FIND_FAST_LF_TIME  x100ms(4)   //����״̬���ҵ�ң�غ󣬼�������ң�أ�����2~3�κ��ж�Ϊ��Чң��
#define _FIND_MID_LF_TIME   x100ms(9)  //6 //����״̬����ң�أ�����״̬��δ�ҵ�ң�غ󣬼�������ң��
#define _FIND_SLOW_LF_TIME  x100ms(20)//x100ms(20)  //����״̬����ң��
#define _FIND_BACKRM_LF_TIME  x100ms(10*60*5)//�������������5����ʱ��Կ��
#define _FIND_SLOWEST_LF_TIME x100ms(100) //����״̬��Կ�׳���5���ӣ������ٶ�
#define _STUDY_TIME         x100ms(100) //ѧϰģʽ����ʱ��
#define _UPWIN_TIME         x100ms(6)  //�������ʱ��600ms
#define _CARFD_TIME         x100ms(5)  //���� 
#define _VIRKEY_TIME        x100ms(15)  //��𰴼�ģ��ʱ��
#define _VIRSEND_TIME				x100ms(5)
//#define _HRON_TIME          x100ms(5)   //���500ms
#define _REV_CAN_TIME				x100ms(2)		//�ȴ�ָ������CAN�������ݳ�ʱ
#define _SECOND_UPWIN_TIME  x100ms(120)  //�ȴ�12s֮���ٴ�����
#define _DOORHRON_TIME      x100ms(6)   //ÿ��600ms ����һ��
#define _WAIT_ANTITHEFH_TIME x100ms(50)  //wait 3
#define _WAIT_POWEROFF_TIME  x100ms(50)
//#define _WAIT_HUPWIN_TIME		x100ms(12)	//�ȴ���
#define _72H_STOP_LF_TIME		x100ms(10*3600*72UL)	//��Ƶ��������
#define _UNREV_CAN_ACC_TIME x100ms(30)
#define JP1_H()						(GPIO_ReadInputDataBit(GPIO_JP, GPIO_PIN_JP1)&1)
/* Private function prototypes -----------------------------------------------*/
void USER_Configuration(void);
void CheckOnTime(void);
void CheckLfRemote(void);
void CheckIOState(void);
void CheckconfigState(void);
void OnEvnetLFSend(void);
void OnEventRemoteOpen(void);
void OnEventRemoteClose(void);
void OnEventStudy(void);
void OnEventHron(void);
void OnEventCarFD(void);
bool CANSendDataConst(CAN_TypeDef* CANx,CanTxMsg const *TxMessageTmp) ;
/* Exported macro ------------------------------------------------------------*/
#endif
/******************* (C) COPYRIGHT 2011 ECARTEK *****END OF FILE****/
