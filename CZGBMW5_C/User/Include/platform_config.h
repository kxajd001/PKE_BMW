/** 
  ******************************************************************************
  * @file    CAN/Normal/platform_config.h 
  * @author  MCD Application Team
  * @version V3.1.2
  * @date    09/28/2009
  * @brief   Evaluation board specific configuration file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#include "Drv_Time.h"
#include "Drv_CAN.h"

#include "Drv_SPI.h"
#include "Drv_Cmt2300.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line corresponding to the STMicroelectronics evaluation board
   used to run the example */
//#define __DEBUG_PRO   //lcw midfy
#define __USER_BMW5_F

#define Time2_1us 1


#define __USER_PCB      1    //40
/* Define the STM32F10x hardware depending on the used evaluation board */
#define U16_MAX    ((u16)65535u)

#define GPIO_CAN1                       GPIOA         //CAN BUS
#define GPIO_PIN_CAN1RX                 GPIO_Pin_11
#define GPIO_PIN_CAN1TX                 GPIO_Pin_12
#define GPIO_CAN2                       GPIOB         //CAN BUS2
#define GPIO_PIN_CAN2RX                 GPIO_Pin_12 
#define GPIO_PIN_CAN2TX                 GPIO_Pin_13 

#define GPIO_JP                         GPIOA         //拨码开关
#define GPIO_PIN_JP1                    GPIO_Pin_2
#define GPIO_PIN_JP2                    GPIO_Pin_1
#define GPIO_PIN_JP3                    GPIO_Pin_0

#define GPIO_CTRL_OUT                 GPIOA         //控制输出  V5.1 PCB
#define GPIO_PIN_0R_OUT               GPIO_Pin_10   //0R电阻    V5.1 PCB
#define GPIO_PIN_200R_OUT             GPIO_Pin_9    //200R电阻  V5.1 PCB

#define GPIO_CTRL_KEY                   GPIOB         //启动按键
#define GPIO_PIN_CTRL_KEY1              GPIO_Pin_15   //V5.1
#define GPIO_PIN_CTRL_KEY2              GPIO_Pin_14   //V5.1

#define GPIO_STUDY_IN                   GPIOB         //学习匹配键
#define GPIO_PIN_STUDY_IN               GPIO_Pin_3
#define GPIO_NXP_LOCK                   GPIOB         //控制NXP锁车键，高频发射
#define GPIO_PIN_NXP_LOCK               GPIO_Pin_4
#define GPIO_NXP_KEY_OPEN 	            GPIOB         //控制NXP解锁键，LF接收
#define GPIO_PIN_NXP_KEY_OPEN           GPIO_Pin_3
#define GPIO_UHF_RX                     GPIOB         //????????
#define GPIO_PIN_UHF_RX                 GPIO_Pin_5   //  V5.1
#define GPIO_IMMO_RX                    GPIOC         //NXP串口数据接收
#define GPIO_PIN_IMMO_RX                GPIO_Pin_11
#define GPIO_ACC_IN                     GPIOA         //ACC电平判断
#define GPIO_PIN_ACC_IN                 GPIO_Pin_15
#define GPIO_NXP_POWER                  GPIOC         //NXP电路电源控制
#define GPIO_PIN_NXP_POWER              GPIO_Pin_10

#define GPIO_LED		                GPIOC         //NXP电路电源控制
#define GPIO_PIN_LED	                GPIO_Pin_9

/**************************************************************************************/

#define READ_START_KEY_IN               GPIO_ReadInputDataBit(GPIO_ACC_IN,GPIO_PIN_ACC_IN)
#define READ_IS_CLOSE_UPWIN             GPIO_ReadInputDataBit(GPIO_JP,GPIO_PIN_JP3) //SHOW 3
#define READ_IS_CHANGE_REV              GPIO_ReadInputDataBit(GPIO_JP,GPIO_PIN_JP2) //SHOW 2
#define READ_IS_REV_INDEX               GPIO_ReadInputDataBit(GPIO_JP,GPIO_PIN_JP1) //SHOW 1

#define READ_UHF_IN                     GPIO_ReadInputDataBit(GPIO_UHF_RX, GPIO_PIN_UHF_RX) 

#define READ_STUDY_IN                   GPIO_ReadInputDataBit(GPIO_STUDY_IN, GPIO_PIN_STUDY_IN) 
     


#define REMOTE_LOCK_ON                  GPIO_SetBits(GPIO_CTRL_OUT, GPIO_PIN_0R_OUT)
#define REMOTE_LOCK_OFF                 GPIO_ResetBits(GPIO_CTRL_OUT, GPIO_PIN_0R_OUT)
#define REMOTE_UNLOCK_ON                GPIO_SetBits(GPIO_CTRL_OUT, GPIO_PIN_200R_OUT)
#define REMOTE_UNLOCK_OFF               GPIO_ResetBits(GPIO_CTRL_OUT, GPIO_PIN_200R_OUT)

#define NXP_POWER_ON                    GPIO_ResetBits(GPIO_NXP_POWER, GPIO_PIN_NXP_POWER)
#define NXP_POWER_OFF                   GPIO_SetBits(GPIO_NXP_POWER, GPIO_PIN_NXP_POWER)


#define NXP_NXP_KEYOPEN_ON              GPIO_ResetBits(GPIO_NXP_KEY_OPEN, GPIO_PIN_NXP_KEY_OPEN)
#define NXP_NXP_KEYOPEN_OFF             GPIO_SetBits(GPIO_NXP_KEY_OPEN, GPIO_PIN_NXP_KEY_OPEN)

#define NXP_LED_ON                      GPIO_ResetBits(GPIO_LED, GPIO_PIN_LED)
#define NXP_LED_OFF                     GPIO_SetBits(GPIO_LED, GPIO_PIN_LED)
#define NXP_LED_TOG                     (GPIO_WriteBit(GPIO_LED, GPIO_PIN_LED, (BitAction)((1-GPIO_ReadOutputDataBit(GPIO_LED, GPIO_PIN_LED)))))

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */




#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
