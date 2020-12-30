/***************************************************************				
File name 	  ��Drv_Time.h
Description     ��
author          ��kxajd
version/date    ��V1.0 2018/1/11
copy            ��ECARTEK
-----------------------------------------------------------------
modify:
date 		 version		 Modifier   	 Update

*****************************************************************/

#ifndef _DRV_TIME_H
#define _DRV_TIME_H 


#define GPIO_125K                       GPIOB         //��Ƶ�ź�
#define GPIO_PIN_125K_L                 GPIO_Pin_6     //p58
#define GPIO_PIN_125K_R                 GPIO_Pin_7     //p69
#define LF125K_TIMx                     TIM4          //��ʱ��PWM���
#define LF125KR_Channelx                TIM_Channel_1
#define LF125KL_Channelx                TIM_Channel_2





void TIM_Configuration(void);

#endif
/******************* (C) COPYRIGHT 2018 ECARTEK *****END OF FILE****/

