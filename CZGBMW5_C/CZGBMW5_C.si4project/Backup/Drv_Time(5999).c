/***************************************************************				
File name 	  ：Drv_Time.c
Description     ：
author          ：kxajd
version/date    ：V1.0 2018/1/11
copy            ：ECARTEK
-----------------------------------------------------------------
modify:
date 		 version		 Modifier   	 Update

*****************************************************************/

#include "stm32f10x.h"
#include "platform_config.h"
/*******************************************************************************
* Function Name  : TIM_Configuration
* Description    : Tim4 PWM输出 通道1，通道2，模拟125K低频信号发送，
                   Tim2 根据需要设置为1us，或者是2us
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void TIM_Configuration(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;


  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2| RCC_APB1Periph_TIM4,ENABLE);
   
  TIM_TimeBaseStructure.TIM_Period = 63;	//重载值 8us
  TIM_TimeBaseStructure.TIM_Prescaler = 0;	//分频值PCLK/(X+1)
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(LF125K_TIMx, &TIM_TimeBaseStructure);
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	//PWM输出模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_Pulse = 32;	//占空比 50%
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
  TIM_OC1Init(LF125K_TIMx, &TIM_OCInitStructure);	//LF信号发生器1
  TIM_OC1PreloadConfig(LF125K_TIMx, TIM_OCPreload_Enable);
  TIM_OC2Init(LF125K_TIMx, &TIM_OCInitStructure);	//LF信号发生器2
  TIM_OC2PreloadConfig(LF125K_TIMx, TIM_OCPreload_Enable);  
  TIM_ARRPreloadConfig(LF125K_TIMx, ENABLE);
  TIM_CtrlPWMOutputs(LF125K_TIMx, ENABLE);
  TIM_Cmd(LF125K_TIMx, DISABLE);   
  
  TIM_TimeBaseStructure.TIM_Period = U16_MAX;
 #ifdef Time2_1us
  TIM_TimeBaseStructure.TIM_Prescaler = 7;
 #else   //Time2_2US
   TIM_TimeBaseStructure.TIM_Prescaler = (16-1);
 #endif
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	//周期1us定时计数器
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	//清中断，以免一启用中断后立即产生中断
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  //TIM_Cmd(TIM2, ENABLE);
}


/******************* (C) COPYRIGHT 2018 ECARTEK *****END OF FILE****/
