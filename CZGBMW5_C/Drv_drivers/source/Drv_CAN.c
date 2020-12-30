/***************************************************************				
File name 	  ��Drv_CAN.c
Description     ��
author          ��kxajd
version/date    ��V1.0 2018/3/15
copy            ��ECARTEK
-----------------------------------------------------------------
modify:
date 		 version		 Modifier   	 Update

*****************************************************************/
#include "stm32f10x.h"
#include "platform_config.h"
/*******************************************************************************
* Function Name  : CAN_Configuration
* Description    : CAN�Ĳ���������
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void CAN_Configuration(void)
{
	extern CanTxMsg const ConstCanCmdLock;
	extern CanTxMsg const CarGetWindowLQ;
	extern CanTxMsg const CarGetWindowRQ;
	extern CanTxMsg const CarGetWindowLH;
	extern CanTxMsg const CarGetWindowRH;
	extern CanTxMsg const ConstCarCmdCloseSKY;
	extern CanTxMsg const CarGetlonkmode;
	extern CanTxMsg const ConstCanCarLFBACK;
	extern CanTxMsg const CarChargingState;
	
	extern CanTxMsg CarGetCarLockState;
	extern CanTxMsg CarGetWindowSKY;
	extern CanTxMsg CanCmdCart;
	extern CanTxMsg CanGetDoorstate;
	extern CanTxMsg CanGetNumKey;
	extern CanTxMsg CanGetAccState;
	extern CanTxMsg CarGetRemote;
	extern CanTxMsg CarFd;
	extern CanTxMsg  engine_state ;
	extern CanTxMsg CarGetCarTbox;

	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1|RCC_APB1Periph_CAN2, ENABLE);
	
	CAN_DeInit(CAN1);
	CAN_DeInit(CAN2);
	
	CAN_InitStructure.CAN_TTCM= DISABLE;	//��ֹʱ�䴥��ͨ��ģʽ
	CAN_InitStructure.CAN_ABOM= ENABLE;	//�����CAN_MCR�Ĵ�����INRQλ������1�����0��һ��Ӳ����⵽128��11λ����������λ�����˳�����״̬��
	CAN_InitStructure.CAN_AWUM= DISABLE;	//˯��ģʽͨ�����CAN_MCR�Ĵ�����SLEEPλ�����������
	CAN_InitStructure.CAN_NART= DISABLE;	//CAN����ֻ������1�Σ����ܷ��͵Ľ����Σ��ɹ���������ٲö�ʧ��
	CAN_InitStructure.CAN_RFLM= DISABLE;	//�ڽ������ʱFIFOδ��������������FIFO�ı���δ����������һ���յ��ı��ĻḲ��ԭ�еı���
	CAN_InitStructure.CAN_TXFP= DISABLE;	//����FIFO���ȼ��ɱ��ĵı�ʶ��������
	//CAN_InitStructure.CAN_Mode= CAN_Mode_LoopBack;	//CANӲ�������ڻ���ģʽ
	CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;	//CANӲ������������ģʽ
	//CAN_InitStructure.CAN_Mode= CAN_Mode_Silent;	//CANӲ�������ھ�Ĭģʽ
	CAN_InitStructure.CAN_SJW=  CAN_SJW_1tq;	//����ͬ����Ծ���1��ʱ�䵥λ
	CAN_InitStructure.CAN_BS1=  CAN_BS1_8tq;	//ʱ���1Ϊ8��ʱ�䵥λ
	CAN_InitStructure.CAN_BS2=  CAN_BS2_7tq;	//ʱ���2Ϊ7��ʱ�䵥λ
	CAN_InitStructure.CAN_Prescaler= (u16)CanCmdCart.Data[2];	//(PCLK1/((1+8+7)*5)) = 8Mhz/16/5 = 100Kbits�趨��һ��ʱ�䵥λ�ĳ���9
	CAN_Init(CAN1, &CAN_InitStructure);
	CAN_InitStructure.CAN_Prescaler= (u16)CanCmdCart.Data[3];	//(PCLK1/((1+8+7)*5)) = 8Mhz/16/5 = 100Kbits�趨��һ��ʱ�䵥λ�ĳ���9
	CAN_Init(CAN2, &CAN_InitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber= 0;	//ָ���˴���ʼ���Ĺ�����0
	CAN_FilterInitStructure.CAN_FilterMode= CAN_FilterMode_IdList;	//ָ���˹�����������ʼ������ģʽΪ��ʶ���б�ģʽ
	CAN_FilterInitStructure.CAN_FilterScale=  CAN_FilterScale_16bit;	//�����˹�����λ��1��32λ������
	CAN_FilterInitStructure.CAN_FilterIdHigh= (ConstCanCmdLock.StdId << 5);//(TxMessageCar[CAN_DOORSTAT].StdId << 5);	//�����趨��������ʶ����32λλ��ʱΪ��߶�λ��16λλ��ʱΪ��һ����
	CAN_FilterInitStructure.CAN_FilterIdLow=  (CarGetWindowLQ.StdId << 5);	//�����趨��������ʶ����32λλ��ʱΪ��Ͷ�λ��16λλ��ʱΪ�ڶ���
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= (CarGetWindowRQ.StdId << 5);	//�����趨���������α�ʶ�����߹�������ʶ����32λλ��ʱΪ��߶�λ��16λλ��ʱΪ��һ��
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=  (CarGetWindowLH.StdId << 5);//(TxMessageCar[CAN_GLASSSTAT].StdId << 5);	//�����趨���������α�ʶ�����߹�������ʶ����32λλ��ʱΪ��Ͷ�λ��16λλ��ʱΪ�ڶ���
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment= CAN_FIFO0;	//�趨��ָ���������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation= ENABLE;	//ʹ�ܹ�����
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber= 1;	//ָ���˴���ʼ���Ĺ�����0
	CAN_FilterInitStructure.CAN_FilterMode= CAN_FilterMode_IdList;	//ָ���˹�����������ʼ������ģʽΪ��ʶ���б�ģʽ
	CAN_FilterInitStructure.CAN_FilterScale=  CAN_FilterScale_16bit;	//�����˹�����λ��1��32λ������
	CAN_FilterInitStructure.CAN_FilterIdHigh= (CarGetWindowRH.StdId << 5);	//�����趨��������ʶ����32λλ��ʱΪ��߶�λ��16λλ��ʱΪ��һ����
	CAN_FilterInitStructure.CAN_FilterIdLow=  (ConstCarCmdCloseSKY.StdId << 5);	//�����趨��������ʶ����32λλ��ʱΪ��Ͷ�λ��16λλ��ʱΪ�ڶ���
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= (CarGetCarLockState.StdId << 5);	//�����趨���������α�ʶ�����߹�������ʶ����32λλ��ʱΪ��߶�λ��16λλ��ʱΪ��һ��
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=  (CarGetWindowSKY.StdId << 5);	//�����趨���������α�ʶ�����߹�������ʶ����32λλ��ʱΪ��Ͷ�λ��16λλ��ʱΪ�ڶ���
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment= CAN_FIFO0;	//�趨��ָ���������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation= ENABLE;	//ʹ�ܹ�����
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber= 2;	//ָ���˴���ʼ���Ĺ�����0
	CAN_FilterInitStructure.CAN_FilterMode= CAN_FilterMode_IdList;	//ָ���˹�����������ʼ������ģʽΪ��ʶ���б�ģʽ
	CAN_FilterInitStructure.CAN_FilterScale=  CAN_FilterScale_16bit;	//�����˹�����λ��1��32λ������
	CAN_FilterInitStructure.CAN_FilterIdHigh= (CanCmdCart.StdId << 5);	//�����趨��������ʶ����32λλ��ʱΪ��߶�λ��16λλ��ʱΪ��һ����
	CAN_FilterInitStructure.CAN_FilterIdLow=  (CanGetDoorstate.StdId << 5);	//�����趨��������ʶ����32λλ��ʱΪ��Ͷ�λ��16λλ��ʱΪ�ڶ���
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= (CanGetNumKey.StdId<< 5);	//�����趨���������α�ʶ�����߹�������ʶ����32λλ��ʱΪ��߶�λ��16λλ��ʱΪ��һ��
	CAN_FilterInitStructure.CAN_FilterMaskIdLow= ( CanGetAccState.StdId<<5);	//�����趨���������α�ʶ�����߹�������ʶ����32λλ��ʱΪ��Ͷ�λ��16λλ��ʱΪ�ڶ���
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment= CAN_FIFO0;	//�趨��ָ���������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation= ENABLE;	//ʹ�ܹ�����
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber= 3;	//ָ���˴���ʼ���Ĺ�����0
	CAN_FilterInitStructure.CAN_FilterMode= CAN_FilterMode_IdList;	//ָ���˹�����������ʼ������ģʽΪ��ʶ���б�ģʽ
	CAN_FilterInitStructure.CAN_FilterScale=  CAN_FilterScale_16bit;	//�����˹�����λ��1��32λ������
	CAN_FilterInitStructure.CAN_FilterIdHigh= (CarGetRemote.StdId << 5);	//�����趨��������ʶ����32λλ��ʱΪ��߶�λ��16λλ��ʱΪ��һ����
	CAN_FilterInitStructure.CAN_FilterIdLow=  (CarFd.StdId<< 5);	//�����趨��������ʶ����32λλ��ʱΪ��Ͷ�λ��16λλ��ʱΪ�ڶ���
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= (CarGetCarTbox.StdId<< 5);	//�����趨���������α�ʶ�����߹�������ʶ����32λλ��ʱΪ��߶�λ��16λλ��ʱΪ��һ��
	CAN_FilterInitStructure.CAN_FilterMaskIdLow= ( CarGetlonkmode.StdId<<5);	//�����趨���������α�ʶ�����߹�������ʶ����32λλ��ʱΪ��Ͷ�λ��16λλ��ʱΪ�ڶ���
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment= CAN_FIFO0;	//�趨��ָ���������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation= ENABLE;	//ʹ�ܹ�����
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_FilterInitStructure.CAN_FilterNumber= 4;	//ָ���˴���ʼ���Ĺ�����0
	CAN_FilterInitStructure.CAN_FilterMode= CAN_FilterMode_IdList;	//ָ���˹�����������ʼ������ģʽΪ��ʶ���б�ģʽ
	CAN_FilterInitStructure.CAN_FilterScale=  CAN_FilterScale_16bit;	//�����˹�����λ��1��32λ������
	CAN_FilterInitStructure.CAN_FilterIdHigh= (engine_state.StdId << 5);	//�����趨��������ʶ����32λλ��ʱΪ��߶�λ��16λλ��ʱΪ��һ����
	CAN_FilterInitStructure.CAN_FilterIdLow=  (0<< 5);	//�����趨��������ʶ����32λλ��ʱΪ��Ͷ�λ��16λλ��ʱΪ�ڶ���
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= (0<< 5);	//�����趨���������α�ʶ�����߹�������ʶ����32λλ��ʱΪ��߶�λ��16λλ��ʱΪ��һ��
	CAN_FilterInitStructure.CAN_FilterMaskIdLow= ( 0<<5);	//�����趨���������α�ʶ�����߹�������ʶ����32λλ��ʱΪ��Ͷ�λ��16λλ��ʱΪ�ڶ���
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment= CAN_FIFO0;	//�趨��ָ���������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation= ENABLE;	//ʹ�ܹ�����
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);//ʹ��ָ����CAN�ж� 

	CAN_FilterInitStructure.CAN_FilterNumber= 14;	//ָ���˴���ʼ���Ĺ�����0
	CAN_FilterInitStructure.CAN_FilterMode= CAN_FilterMode_IdList;	//ָ���˹�����������ʼ������ģʽΪ��ʶ���б�ģʽ
	CAN_FilterInitStructure.CAN_FilterScale=  CAN_FilterScale_16bit;	//�����˹�����λ��1��32λ������
	CAN_FilterInitStructure.CAN_FilterIdHigh= (CarGetlonkmode.StdId << 5);	//�����趨��������ʶ����32λλ��ʱΪ��߶�λ��16λλ��ʱΪ��һ����
	CAN_FilterInitStructure.CAN_FilterIdLow=  (ConstCanCarLFBACK.StdId<< 5);	//�����趨��������ʶ����32λλ��ʱΪ��Ͷ�λ��16λλ��ʱΪ�ڶ���
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= (CarChargingState.StdId<< 5);	//�����趨���������α�ʶ�����߹�������ʶ����32λλ��ʱΪ��߶�λ��16λλ��ʱΪ��һ��
	CAN_FilterInitStructure.CAN_FilterMaskIdLow= ( CanGetDoorstate.StdId<<5);	//�����趨���������α�ʶ�����߹�������ʶ����32λλ��ʱΪ��Ͷ�λ��16λλ��ʱΪ�ڶ���
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment= CAN_FIFO0;	//�趨��ָ���������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation= ENABLE;	//ʹ�ܹ�����
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);//ʹ��ָ����CAN�ж� 
}

/******************* (C) COPYRIGHT 2018 ECARTEK *****END OF FILE****/
