/**
  ******************************************************************************
  * @file    CAN/Normal/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.1.2
  * @date    09/28/2009
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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
#include "stm32f10x_it.h"
#include "UHF_125K.h"
/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup CAN_Normal
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  extern vu32 TimingDelay;
  if(TimingDelay) TimingDelay--;
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/
void RTC_IRQHandler(void)
{
  extern vu32 ElapseTimeEnd[TIMER_END];
  extern volatile bool IDEventFlag[TIMER_END], OnTimeFlag[TIMER_END], RTCDisableFlag;
  extern vu32 RTCValue;

  uint8_t i;
  
  if(RTCDisableFlag == TRUE) 
  {
    RTC_ClearITPendingBit(RTC_IT_SEC);
    RTCValue++;
    return;
  }
  if(RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
	u32 rtcvalue = 0;
	
    RTC_WaitForLastTask();
    RTC_ClearITPendingBit(RTC_IT_SEC);
    RTC_WaitForLastTask();
    RTCValue = RTC_GetCounter();
    rtcvalue = RTCValue;
    if(*(u32 *)(&IDEventFlag[0]) != 0x00L) //前0~3
	{   
      for( i = 0; i < 4; i++) //依次查询
	  { 
        if(IDEventFlag[i] == TRUE) //定时器开启标志
		{ 
          if(rtcvalue > ElapseTimeEnd[i]) 
		  {
		  	OnTimeFlag[i] = TRUE;//定时器到来
          }
        }   
      }
    }
#if TIMER_MAX > 4
    if(*(u32 *)(&IDEventFlag[4]) != 0x00L) //4~7
	{   
      for( i = 4; i < TIMER_END; i++)  //依次查询
	  {
        if(IDEventFlag[i] == TRUE) //定时器开启标志
		{  
          if(rtcvalue > ElapseTimeEnd[i]) 
		  {
		  	OnTimeFlag[i] = TRUE;//定时器到来
          }
        }   
      }
    }
#endif
  }
}

/**
  * @brief  This function handles CAN1 Handler.
  * @param  None
  * @retval None
  */
extern void CAN1IrqFuc(void);
void CAN1_RX0_IRQHandler(void) //
{
	CAN1IrqFuc();
}
extern void CAN2IrqFuc(void);
void CAN2_RX0_IRQHandler(void) 
{
  CAN2IrqFuc();
}

/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_IRQHandler(void)
{
  extern volatile bool TIM2flag;
  if(TIM_GetITStatus(TIM2, TIM_IT_Update)!=RESET) {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    TIM_Cmd(TIM2, DISABLE);//禁能定时器
    TIM2flag = FALSE;
  }
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
