/***************************************************************				
�ļ��� 	  ��App_public.c
����ļ�	  ��
�ļ�ʵ�ֹ��� ��
����		  ��kxajd
�汾		  ��V1.0 20170308
-----------------------------------------------------------------
�޸ļ�¼:
��  �� 		 �汾		 �޸��� 	 �޸�����

*****************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "UHF_125K.h"
#include "string.h"
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

extern volatile EventState EventFlag;

vu32 RTCValue = 0;     //��ǰRTC��ֵ
volatile bool TIM2flag = FALSE; //TIM2��ʱ��������־
u32 ElapseTime[TIMER_END]    = {0};  //���ö�ʱ��ʱ�䲽��
vu32 ElapseTimeEnd[TIMER_END] = {0};  //���ö�ʱ��ʱ���յ�
volatile bool IDEventFlag[TIMER_END]  = {FALSE};  //���ö�ʱ������
volatile bool OnTimeFlag[TIMER_END]   = {FALSE};  //���ö�ʱ��ʱ�䵽��
volatile bool RTCDisableFlag = FALSE;
DoorState door = DOOR_OPEN;
CarState CarPower = CAR_POWER_ACC;
CanRxMsg RxMessage;   //CAN��������
bool AllowCloseUpwinFlag = TRUE;
bool ChangeRevMode = TRUE;
u32 StudyTime = _STUDY_TIME;
u8 RevIndexTotal = 1;
u8 ACCActiveCount = 0;
u8 Data[64] = {0};        //RF�������ݱ���
u8 CloseCount=0;
bool IsLQWinOpen = TRUE, IsLHWinOpen = TRUE, IsRQWinOpen = TRUE, IsRHWinOpen = TRUE, IsSKYWinOpen = FALSE;
bool CarIsLockFlag = FALSE;
bool CarDoorOpenFlag = FALSE;
bool PrvCarDoorOpenFlag = FALSE;
//bool LQWinActiveOnceFlag = FALSE, RQWinActiveOnceFlag = FALSE, LHWinActiveOnceFlag = FALSE, RHWinActiveOnceFlag = FALSE;
bool ConnectFlag = FALSE;
bool CarDWAActiveFlag = FALSE;
bool Disable_lf_flag = FALSE;
bool Disable_lf = FALSE;
bool Disable_lf_car_flag=FALSE;
bool Disable_lf_unlock=FALSE;
bool Disable_win = FALSE;
bool Disable_doorhron =FALSE;
bool Disable_alarm =TRUE;
bool AllowWriteFlag = FALSE;
bool FD_OPEN_flag  = FALSE;
bool AlarmOpen_flag = FALSE;
bool DoorHron_flag = FALSE;
bool OPEN_ANIT =FALSE;
bool LOCKMODEFLAG   =TRUE;
bool LFPowerOff_doorclose=FALSE;   
bool HronSelFlag =TRUE;
bool Disableglass =FALSE;    //�غ��Ӿ�
bool DisablePoweroffOncharg=FALSE;
bool CarLFFlagBcak =TRUE;
bool FindRMLFFlag =TRUE;
bool BACKLFOPENFLAG =TRUE;    //һֱʹ�ܺ�����
bool ENCarLFFlag =FALSE;
u8 preremote = 0;

u8 MCUID[4];
u8 TboxCnt=0;
u8 doordata=0;
u8 prvdoordata=0;   //20180828 midfy
u8 LFPrvdoordata=0;
u32 button_counter =0;
u16 prvTmp_button =0;
u16 DoorOpenLfcount=0; //����״̬��ң�ؼ�����
u8  DoorOpenNoLfcount =0;
u8  Lockedf = 0;
u8  DoorHoncount =0;
u8 UnfindRemoteCount = 0;
u8 FindRemoteCount = 0;
u8 q_Remotedata=0;
u8 q_Remoteunlockcnt=0;
//u8 RemoteIDLength=14;
LF_SLEEP_STATUS DoorOpenLFsleep=LF_NO_SLEEP;

u32 config_flag = 0xFFFFFFD8;
u32 config_flagtemp = 0xFFFFFFD8;
u8 configflag[8]={0,0,0,1,1,0,0,1};//{1,1,0,1,1,0,0,0};     ����Ҫ��configflag[7]=0
u8 configflagtemp[8]={0,0,0,1,1,0,0,1};//{1,1,0,1,1,0,0,0};

uc8 Lavida_LFData_Open[23] = {0x30,0x10,0x13,0x3B,0xC2,  0x03,0x03,0x03,0x03,  0x3B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x33,0x00  ,0x37,0x00};//////////

/***********************BMW**********************/
#ifdef __USER_BMW5_F
u8 LFData_StARTID[14] ={0x10,0xBA,0x1C,0x2B,0x00,0x0C,0x52,0x00,   0x23,  0x27,  0x2B,  0x2F,0x00,0x00};//////
u8 LFData_StartR1[20] ={0x10,0xBA,0x1C,0x2B,0x00,0xE1,0x62,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x23};

u8 LFData_Open[27] = {0x00,0x00,0x00,0x00,0x00,/*ms*/0x00,0x00,0x00,0x00,0x00/*ms*/,0x6F,0x0E,0x2E,0xC6,0x00,0x00,0x00,0x00,0x00,/*ms*/0x23,0x00, 0x27,0x00, 0x2B,0x00, 0x2F,0x00};/////
u8 LFData_Open1[27] = {0xD9,0xCD,0xDE,0x3D,0x13,/*ms*/0x21,0x0C,0x14,0xF1,0x4E/*ms*/,0x59,0x93,0x8D,0xE4,0xDB,0x00,0x00,0x00,0x00,/*ms*/0x23,0x00, 0x27,0x00, 0x2B,0x00, 0x2F,0x00};/////
u8 LFData_Open2[27] = {0x3A,0x5b,0xf4,0x27,0x17,/*ms*/0x21,0x04,0x75,0x7d,0x61/*ms*/,0x80,0x0f,0xde,0x47,0xa9,0x00,0x00,0x00,0x00,/*ms*/0x23,0x00, 0x27,0x00, 0x2B,0x00, 0x2F,0x00};/////
//u8 LFData_Open3[27] = {0xEE,0x23,0x73,0x1F,0x35,/*ms*/0x38,0xBF,0xDA,0x93,0x64/*ms*/,0xFE,0xB2,0x47,0x25,0x00,0x00,0x00,0x00,0x00,/*ms*/0x23,0x00, 0x27,0x00, 0x2B,0x00, 0x2F,0x00};/////
//u8 LFData_Open4[27] = {0xEE,0x23,0x73,0x1F,0x37,/*ms*/0x38,0x1D,0x48,0x8E,0xC8/*ms*/,0x99,0x6A,0xCF,0x8A,0x00,0x00,0x00,0x00,0x00,/*ms*/0x23,0x00, 0x27,0x00, 0x2B,0x00, 0x2F,0x00};/////


CanTxMsg CanTxTmp             			= {0x07EC,0,CAN_ID_STD,CAN_RTR_DATA,0x04,{0xDD,0xDD,0xDD,0xDD,0x00,0x00,0x00,0x00}};  //CAN���ͻ�������
CanTxMsg CanCmdCart     						= {0x07EC,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0xF1,0x14,0x01,0x01,0x45,0x43,0x41,0x52}};


CanTxMsg const CarGetWindowLQ 			= {0x03B6,0,CAN_ID_STD,CAN_RTR_DATA,0x03,{0x00,0x00,0x00,0x37,0x80,0x00,0x00,0x00}};  //��ǰ����λ����Ϣ
CanTxMsg const CarGetWindowRQ 			= {0x03B8,0,CAN_ID_STD,CAN_RTR_DATA,0x03,{0x00,0x00,0x00,0x37,0x80,0x00,0x00,0x00}};  //��ǰ����λ����Ϣ
CanTxMsg const CarGetWindowLH 			= {0x03B7,0,CAN_ID_STD,CAN_RTR_DATA,0x03,{0x00,0x00,0x00,0x37,0x80,0x00,0x00,0x00}};  //��󳵴�λ����Ϣ
CanTxMsg const CarGetWindowRH 			= {0x03B9,0,CAN_ID_STD,CAN_RTR_DATA,0x03,{0x00,0x00,0x00,0x37,0x80,0x00,0x00,0x00}};  //�Һ󳵴�λ����Ϣ

CanTxMsg const CarGetlonkmode       = {0x03D5,0,CAN_ID_STD,CAN_RTR_DATA,0x04,{0x29,0x00,0x00,0x37,0x80,0x00,0x00,0x00}};  //����ģʽ����ģʽ�趨  29 ���г��� 2A����ģʽ
																																												

//CanTxMsg const ConstCanCmdlock_0      = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x40,0x10,0x07,0x31,0x01,0x10,0x01,0x06}}; 
//CanTxMsg const ConstCanCmdlock_01     = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x04,{0x40,0x21,0x06,0xA8,0xAC,0x54,0x02,0x00}}; 
CanTxMsg const ConstCanCmdLock     		= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x07,{0x40,0x05,0x31,0x01,0xAC,0x57,0x02,0x00}};  //OBD ����
CanTxMsg const ConstCanCmdUpWin_F   	= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x40,0x06,0x31,0x01,0xA1,0x7F,0x40,0x00}};  //OBD ��ǰ��
//CanTxMsg const ConstCanCmdUpWin_L   	= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x72,0x06,0x31,0x01,0xA1,0x7F,0x50,0x00}};  //OBD ����
//CanTxMsg const ConstCanCmdCloseGlass 	= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x06,{0x40,0x04,0x2E,0xE4,0x54,0x01,0x00,0x00}};  //OBD �۵����Ӿ�
CanTxMsg const ConstCanCmdlockAccOFF1 = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x40,0x10,0x07,0x31,0x01,0x10,0x31,0x02}}; 
CanTxMsg const ConstCanCmdlockAccOFF2 = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x04,{0x40,0x21,0x02,0xD9,0xFF,0xFF,0xFF,0xFF}}; 
CanTxMsg const ConstCanCmdHorn   			= {0x5E1,0,CAN_ID_STD,CAN_RTR_DATA,0x07,{0x60,0xF0,0x05,0x01,0x01,0xFF,0xFF,0xFF}};  //OBD ������
CanTxMsg const ConstCanCmdHorn1   		= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x07,{0x40,0x05,0x2E,0xD2,0x98,0x00,0x80,0x00}};  //OBD ������
CanTxMsg const ConstCanCmdHorn0   		= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x07,{0x40,0x05,0x2E,0xD2,0x98,0x00,0x60,0x00}};  //OBD ������
CanTxMsg const ConstCanCmdHorn2   		= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x07,{0x40,0x05,0x2E,0xD2,0x98,0x00,0x40,0x00}};  //OBD ������

CanTxMsg const ConstCanCmdALARM       = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x06,{0x56,0x04,0x31,0x01,0xAA,0x70,0xFF,0xFF}};  //OBD ������ALARM,����5S9
CanTxMsg const ConstCanCmdUnLock   		= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x07,{0x40,0x05,0x31,0x01,0xAC,0x57,0x01,0x00}};  //OBD ����
//CanTxMsg const ConstCanCmdOpenGlass  	= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x06,{0x40,0x04,0x2E,0xE4,0x54,0x02,0x00,0x00}};  //OBD �򿪺��Ӿ�
CanTxMsg const ConstCanCmdCloseGlass 	= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x07,{0x40,0x05,0x2E,0xD3,0x24,0x00,0x01,0x00}};  //OBD �۵����Ӿ�
CanTxMsg const ConstCanCmdOpenGlass  	= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x07,{0x40,0x05,0x2E,0xD3,0x24,0x00,0x02,0x00}};  //OBD �򿪺��Ӿ�

CanTxMsg const ConstCanCmdLamp  			= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x40,0x10,0x07,0x2E,0xD5,0x42,0x00,0x12}};  //OBD ˫������Ϣ
CanTxMsg CanCmdLamp1 									= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x04,{0x40,0x21,0x00,0x64,0xD5,0x42,0x00,0x12}};  //0064=100 Ϊ ��1s
CanTxMsg const ConstCarCMDOBDSKY1     = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x56,0x10,0x07,0x31,0x01,0xA1,0x86,0xA1}};  //39OBD���촰
CanTxMsg const ConstCarCMDOBDSKY2     = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x56,0x21,0x64,0x02,0xFF,0xFF,0xFF,0xFF}};  //40OBD���촰
CanTxMsg const ConstCarCmdCloseSKY  	= {0x026E,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x1B,0x1B,0x1B,0x52,0xFF,0xFF,0xFF,0xFF}};  //���촰  data1==data2==0x5B 20180509�޸�
CanTxMsg const ConstCarCmdDoorPWR     = {0x03A3,0,CAN_ID_STD,CAN_RTR_DATA,0x02,{0x08,0xFF,0x00,0x00,0xFF,0xFF,0xFF,0xFF}};  //

CanTxMsg const ConstCanCmdlight  			= {0x06F3,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x40,0x10,0x07,0x2E,0xD5,0x42,0x00,0x4}};  //OBD �������
CanTxMsg CanCmdlight									= {0x06F3,0,CAN_ID_STD,CAN_RTR_DATA,0x04,{0x40,0x21,0x0B,0xB8,0x00,0x00,0x00,0x00}};  //03E8=1000 Ϊ 10s
CanTxMsg const caution_light_data4 		= {0x02B4,0,CAN_ID_STD,CAN_RTR_DATA,0x02,{0x01,0x00,0x0F,0xA0,0xFF,0xFF,0xFF,0xFF}};  //????
CanTxMsg const caution_light_data5 		= {0x02B4,0,CAN_ID_STD,CAN_RTR_DATA,0x02,{0x00,0x00,0x0F,0xA0,0xFF,0xFF,0xFF,0xFF}};  //????
CanTxMsg const off_light					 		= {0x06F3,0,CAN_ID_STD,CAN_RTR_DATA,0x04,{0x40,0x02,0x11,0x01,0xFF,0xFF,0xFF,0xFF}};  //????

		

CanTxMsg const cmd_unlock_data  			= {0x03A3,0,CAN_ID_STD,CAN_RTR_DATA,0x02,{0xC8,0xFF,0x00,0x00,0x00,0x00,0x00,0x00}};  //������������	�û�еԿ�׿���ʱ�ɼ�������
CanTxMsg const cmd_lock_data 					= {0x03A3,0,CAN_ID_STD,CAN_RTR_DATA,0x02,{0xD0,0xFF,0x00,0x00,0x00,0x00,0x00,0x00}};  	//������������	�û�еԿ������ʱ�ɼ�������

CanTxMsg /*const*/ ConstCanCmdCarLFSEND  	= {0x06F3,0,CAN_ID_STD,CAN_RTR_DATA,0x06,{0x40,0x04,0x31,0x01,0xAC,0x54,0x00,0x00}};  //OBD ����ԭ����Կ��
CanTxMsg const ConstCanCarLFBACK     	= {0x0640,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0xF1,0x10,0x0C,0x71,0x01,0xAC,0x54,0x00}};  //OBD ԭ����Կ�׷���ָ��

CanTxMsg const CarChargingState     = {0x03B4,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0xFF,0xFF,0xFD,0xFF,0xFF,0xFF,0x05,0xFF}};  //��ǰ����λ����Ϣ

CanTxMsg  CarGetRemote		 	= {0x023A,0,CAN_ID_STD,CAN_RTR_DATA,0x06,{0x11,0xF3,0x01,0x3F,0xFF,0xF0,0x00,0x00}};  //ң�����ź�, Data[2]:0x01-����;0x10-����;0x04-����;
CanTxMsg  CarGetWindowSKY 				= {0x03BA,0,CAN_ID_STD,CAN_RTR_DATA,0x07,{0x00,0xE1,0x00,0xE4,0x09,0x00,0x00,0x00}};  //data0 Ϊ0x00�촰��   �촰״̬
CanTxMsg  CanGetDoorstate     		= {0x02FC,0,CAN_ID_STD,CAN_RTR_DATA,0x07,{0x81,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00}};  //����״̬
CanTxMsg  CanGetNumKey    				= {0x00A2,0,CAN_ID_STD,CAN_RTR_DATA,0x02,{0x00,0x40,0x00,0x00,0x8C,0x00,0x00,0x00}};  //�п����ְ���
//CanTxMsg  CanGetAccState          = {0x03BB,0,CAN_ID_STD,CAN_RTR_DATA,0x05,{0x02,0xF0,0xFC,0xFF,0xFF,0xFF,0xFF,0xFF}};  //OBD data0==0x00 ACCOFF data0==0x05 accON
CanTxMsg  CanGetAccState          = {0x012F,0,CAN_ID_STD,CAN_RTR_DATA,0x05,{0x02,0xF0,0x86,0xFF,0xFF,0xFF,0xFF,0xFF}};  //OBD data0==0x86 ACCOFF data0==0x8A accON  0x88 accstart

CanTxMsg  CarGetCarLockState		 	= {0x02A0,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x88,0x88,0xF8,0x0C,0xFF,0xFF,0xFF,0xFF}};  //����״̬��Ϣ
CanTxMsg  CarGetCarTbox						= {0x00EC,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0xFF,0xFF,0xFF,0xFF,0x08,0x4F,0xFC,0xFF}};  //β�䶯��
CanTxMsg CarCmdAntiTheft          = {0x02FC,0,CAN_ID_STD,CAN_RTR_DATA,0x07,{0x34,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF}};//����״̬

CanTxMsg const cmd_into_security  = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x56,0x06,0x31,0x01,0xAA,0x79,0x00,0x01}};  //���������������
CanTxMsg const cmd_outo_security  = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x56,0x06,0x31,0x01,0xAA,0x79,0x00,0x00}};  //���������������
CanTxMsg const cmd_outo1_security = {0x23A,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x22,0xF3,0x01,0x3F,0xFC,0xF0,0xFF,0xFF}};  //���������������

CanTxMsg const cmd_Set_light      = {0x3D6,0,CAN_ID_STD,CAN_RTR_DATA,0x03,{0x01,0xF1,0xFF,0x00,0x00,0x00,0x00,0x00}};  //ȥ������˫��
CanTxMsg const cmd_Set_light0     = {0x3D6,0,CAN_ID_STD,CAN_RTR_DATA,0x03,{0x04,0xF4,0xFF,0x00,0x00,0x00,0x00,0x00}};  //ȥ��������ʾ��


CanTxMsg const cmd_POWEROFF0      = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x05,{0x12,0x03,0x22,0xf1,0x50,0x00,0x00,0x00}};  //�ϵ�ָ��
CanTxMsg const cmd_POWEROFF1      = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x05,{0x12,0x03,0x22,0x40,0xC3,0x00,0x00,0x00}};  //�ϵ�ָ��
CanTxMsg const cmd_POWEROFF2      = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x04,{0x12,0x30,0x00,0x00,0x00,0x00,0x00,0x00}};  //�ϵ�ָ��
CanTxMsg const cmd_POWEROFF3      = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x05,{0x40,0x03,0x22,0xf1,0x50,0x00,0x00,0x00}};  //�ϵ�ָ��
CanTxMsg const cmd_POWEROFF4      = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x07,{0x40,0x05,0x31,0x01,0xAC,0x51,0x00,0x00}};  //�ϵ�ָ��
CanTxMsg const cmd_POWEROFF5      = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x04,{0xdf,0x02,0x11,0x04,0x00,0x00,0x00,0x00}};  //�ϵ�ָ��
CanTxMsg const cmd_POWEROFF6      = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x05,{0x40,0x03,0x22,0xf1,0x50,0x00,0x00,0x00}};  //�ϵ�ָ��
CanTxMsg const cmd_POWEROFF7      = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x40,0x10,0x07,0x31,0x01,0x10,0x01,0x10}};  //�ϵ�ָ��
CanTxMsg const cmd_POWEROFF8      = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x04,{0x40,0x21,0x10,0x2a,0x00,0x00,0x00,0x00}};  //�ϵ�ָ��


CanTxMsg CarFd    ={0x02B4,0,CAN_ID_STD,CAN_RTR_DATA,0x02,{0x00,0xF1,0x00,0xFF,0xFF,0xFF,0xFF,0x00}};
#endif  //__USER_BMW3_F

/* Private macro -------------------------------------------------------------*/
uc8 CrcTable[256] = {	//CRC-8
  0x00,0x07,0x0E,0x09,0x1C,0x1B,0x12,0x15,0x38,0x3F,0x36,0x31,0x24,0x23,0x2A,0x2D,	
  0x70,0x77,0x7E,0x79,0x6C,0x6B,0x62,0x65,0x48,0x4F,0x46,0x41,0x54,0x53,0x5A,0x5D,	
  0xE0,0xE7,0xEE,0xE9,0xFC,0xFB,0xF2,0xF5,0xD8,0xDF,0xD6,0xD1,0xC4,0xC3,0xCA,0xCD,	
  0x90,0x97,0x9E,0x99,0x8C,0x8B,0x82,0x85,0xA8,0xAF,0xA6,0xA1,0xB4,0xB3,0xBA,0xBD,	
  0xC7,0xC0,0xC9,0xCE,0xDB,0xDC,0xD5,0xD2,0xFF,0xF8,0xF1,0xF6,0xE3,0xE4,0xED,0xEA,	
  0xB7,0xB0,0xB9,0xBE,0xAB,0xAC,0xA5,0xA2,0x8F,0x88,0x81,0x86,0x93,0x94,0x9D,0x9A,	
  0x27,0x20,0x29,0x2E,0x3B,0x3C,0x35,0x32,0x1F,0x18,0x11,0x16,0x03,0x04,0x0D,0x0A,	
  0x57,0x50,0x59,0x5E,0x4B,0x4C,0x45,0x42,0x6F,0x68,0x61,0x66,0x73,0x74,0x7D,0x7A,	
  0x89,0x8E,0x87,0x80,0x95,0x92,0x9B,0x9C,0xB1,0xB6,0xBF,0xB8,0xAD,0xAA,0xA3,0xA4,	
  0xF9,0xFE,0xF7,0xF0,0xE5,0xE2,0xEB,0xEC,0xC1,0xC6,0xCF,0xC8,0xDD,0xDA,0xD3,0xD4,	
  0x69,0x6E,0x67,0x60,0x75,0x72,0x7B,0x7C,0x51,0x56,0x5F,0x58,0x4D,0x4A,0x43,0x44,	
  0x19,0x1E,0x17,0x10,0x05,0x02,0x0B,0x0C,0x21,0x26,0x2F,0x28,0x3D,0x3A,0x33,0x34,	
  0x4E,0x49,0x40,0x47,0x52,0x55,0x5C,0x5B,0x76,0x71,0x78,0x7F,0x6A,0x6D,0x64,0x63,	
  0x3E,0x39,0x30,0x37,0x22,0x25,0x2C,0x2B,0x06,0x01,0x08,0x0F,0x1A,0x1D,0x14,0x13,	
  0xAE,0xA9,0xA0,0xA7,0xB2,0xB5,0xBC,0xBB,0x96,0x91,0x98,0x9F,0x8A,0x8D,0x84,0x83,	
  0xDE,0xD9,0xD0,0xD7,0xC2,0xC5,0xCC,0xCB,0xE6,0xE1,0xE8,0xEF,0xFA,0xFD,0xF4,0xF3};
/* Private functions ---------------------------------------------------------*/
extern void DelayXms(vu32 nTime);
extern void SetGpioPinNxpOpen(uint8_t PinNxpOpenState);
extern void SetGpioPinStudyIn(void);
void CheckDriveIsOK(void);
/*******************************************************************************
* Function Name  : Cal_crc8
* Description    : CRC-8
* Input          : ptr - �����ַ
                  len - ����
* Output         : None
* Return         : ECBYTE - ������
*******************************************************************************/
u8 Cal_crc8(u8 *ptr, u8 len) {
  u8 CRCValue = 0;
  u8 i;
  for(i = 0; i < len; i++)
    CRCValue = CrcTable[CRCValue ^ ptr[i]]; //���ʽ����CRC
  return CRCValue;
}

/*******************************************************************************
* Function Name  : SetTimer
* Description    : �������ö�ʱ��������ʹ��KillTimer���رգ�����һ����ʱ���ں󣬻������һ�����ڶ�ʱ
* Input          : nIDEvent - ��ʱ����ţ���TIMER_END
                   nElapse - ��ʱ����ʱʱ�� ����62.5ms
* Output         : None
* Return         : None 
*******************************************************************************/
void SetTimer(u8 nIDEvent, u32 nElapse) { //62.5ms
  OnTimeFlag[nIDEvent] = FALSE; //��ʱ���ڽ�����־
  ElapseTime[nIDEvent] = nElapse; //��ʱ����ʱ��
  ElapseTimeEnd[nIDEvent] = RTCValue + nElapse;  //��ʱ���ڽ���ʱ��RTCʱ��
  IDEventFlag[nIDEvent] = TRUE; //����RTC�ж�ʱ���жϼ�ʱ�����Ƿ����
}
/*******************************************************************************
* Function Name  : KillTimer
* Description    : ָֹͣ����Ŷ�ʱ�������Լ�ʱ
* Input          : nIDEvent - ��ʱ����ţ���TIMER_END
* Output         : None
* Return         : None 
*******************************************************************************/
void KillTimer(u8 nIDEvent) {
  OnTimeFlag[nIDEvent] = FALSE; //��ʱ���ڽ�����־
  IDEventFlag[nIDEvent] = FALSE;  //�ر�RTC�ж�ʱ���жϼ�ʱ�����Ƿ����
  ElapseTime[nIDEvent] = 0x00L; //�����ʱ����ʱ��
  ElapseTimeEnd[nIDEvent] = 0x00L;  //�����ʱ���ڽ���ʱ��RTCʱ��
}

void PowerOffSever(void)
{
		
		CarCmdAntiTheft.Data[0]=0x82;//����֮�󣬴򿪷���
		CANSendDataConst(CAN1,&CarCmdAntiTheft);
		DelayXms(1);
		CANSendDataConst(CAN2,&ConstCanCmdlockAccOFF1);
			//DelayXms(60);
		CarCmdAntiTheft.Data[0]=0x82;//����֮�󣬴򿪷���
		CANSendDataConst(CAN1,&CarCmdAntiTheft);
		DelayXms(1);
		CANSendDataConst(CAN2,&ConstCanCmdlockAccOFF2);
		CarCmdAntiTheft.Data[0]=0x82;//����֮�󣬴򿪷���
		CANSendDataConst(CAN1,&CarCmdAntiTheft);
//		CANSendDataConst(CAN2,&cmd_into_security);
}
/*******************************************************************************
* Function Name  : OnTimer
* Description    : ��ʱ����ʱ���ڽ����¼���Ӧ
* Input          : nIDEvent - ��ʱ����ţ���TIMER_END
* Output         : None
* Return         : None 
*******************************************************************************/
u8 secnedflag=0;
void OnTimer(u8 nIDEvent) 
{
	switch(nIDEvent) 
	{  
		case TIMER_LF:
			ElapseTimeEnd[TIMER_LF] += ElapseTime[TIMER_LF];
			EventFlag = EVENT_LFSEND;
		break;
		case TIMER_UPWIN:  //���촰
				CANSendDataConst(CAN2,&ConstCarCMDOBDSKY1);	//���촰
				DelayXms(5);
				CANSendDataConst(CAN2,&ConstCarCMDOBDSKY2);	
				KillTimer(TIMER_UPWIN);
		break; 
		case TIMER_RESET:
			NVIC_SystemReset(); //��λ
			DelayXms(1);
			KillTimer(TIMER_RESET);
		break;
		case TIMER_BACKLFFIND:
			FindRMLFFlag=FALSE;     //�رպ����߲���Կ��
			KillTimer(TIMER_BACKLFFIND);
		break;
		case TIMER_POWEROFF:
			printf("�ϵ�");
//			CANSendDataConst(CAN2,&cmd_into_security);   //�����Ƿ�һ��ָ��������
//			DelayXms(60);
			if(secnedflag==0)
			{
				if(IsLQWinOpen||IsLHWinOpen||IsRQWinOpen||IsRHWinOpen||IsSKYWinOpen)
				{
//					if(CarDoorOpenFlag==FALSE)
					{
						CANSendDataConst(CAN2,&ConstCarCMDOBDSKY1);	//���촰
						DelayXms(2);
						CANSendDataConst(CAN2,&ConstCarCMDOBDSKY2);
						CANSendDataConst(CAN2,&ConstCanCmdUpWin_F);	//��ǰ��
					}
				}
				else
				{
					if(DisablePoweroffOncharg==FALSE)   //�����ϵ�ط�Χ��
					{
						PowerOffSever();
						DelayXms(60);
						PowerOffSever();
					}	
					SetTimer(TIMER_ANTI_WAIT, _WAIT_ANTITHEFH_TIME);
					KillTimer(TIMER_POWEROFF);
				}
				secnedflag=1;
				ElapseTimeEnd[TIMER_POWEROFF] += ElapseTime[TIMER_POWEROFF];
			}
			else
			{
				secnedflag=0;		
				if(DisablePoweroffOncharg==FALSE)   //�����ϵ�ط�Χ��
				{
					PowerOffSever();
					DelayXms(60);
					PowerOffSever();
				}			
			 SetTimer(TIMER_ANTI_WAIT, _WAIT_ANTITHEFH_TIME);
				KillTimer(TIMER_POWEROFF);
			}
			break;
		case TIMER_DOORHRON:
			if(DoorHoncount<5)
			{
				ElapseTimeEnd[TIMER_DOORHRON] += ElapseTime[TIMER_DOORHRON];
				CANSendDataConst(CAN2,&ConstCanCmdHorn);  //���ñ�����
				DoorHoncount++;
			}
			else
			{
				KillTimer(TIMER_DOORHRON);
			}
			break;
		case TIMER_ANTI_WAIT:


			CANSendDataConst(CAN2,&off_light);
			DelayXms(60);
//			OPEN_ANIT=TRUE;
			CANSendDataConst(CAN2,&cmd_into_security);	 //�����Ƿ�һ��ָ��������
			DelayXms(60);
			CANSendDataConst(CAN2,&cmd_into_security);	
			printf("�������");
//			CarCmdAntiTheft.Data[0]=0x34;//����֮�󣬴򿪷���
//			CANSendDataConst(CAN2,&cmd_into_security);   //�����Ƿ�һ��ָ��������
//			CANSendDataConst(CAN1,&cmd_into_security); 
//			CANSendDataConst(CAN1,&CarCmdAntiTheft);
			KillTimer(TIMER_ANTI_WAIT);
			
			break;
	}
}
/*******************************************************************************
* Function Name  : CheckOnTime
* Description    : ����Զ��嶨ʱ�������¼�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CheckOnTime(void) 
{
  u8 i;
  
  for(i = 0; i < TIMER_END; i++) 
  {
	    if(OnTimeFlag[i] == TRUE) 
		{ //�������¼�
	      OnTimeFlag[i] = FALSE; 
	      OnTimer(i); //��Ӧ�����¼�
	   }
  }
}
/*******************************************************************************
* Function Name  : FLASH_WriteRemoteID
* Description    : �洢ң��IDֵ�����ݵ�ָ��FLASH��ַ
* Input          : remote - ң������������ָ�룬ֻ��ȡ����20Byte����
* Output         : None
* Return         : bool - TRUE, FALSE
*******************************************************************************/
void FLASH_ProgramMoreWord(u32 addr, u32* dwData, u8 len)
{
  u8 i;
  
  for(i = 0; i < len; i++) 
  {
      FLASH_ProgramWord(addr + i * 4, *((u32 *)dwData + i));  //д��FLASH
  }
}
void FLASH_ReadMoreWord(u32 addr, u32* dwData, u8 len) 
{
  u8 i;
  
  for(i = 0; i < len; i++)
  {
    *((u32 *)dwData + i) = *((vu32 *)addr + i); //����FLASH����
  }
}
bool FLASH_WriteRemoteID(u8 ID[RemoteID_Length], u32 key)
{
	u8 Tmp[RemoteID_Length+2] = {0x00};
	u8 i=0;
	u8 j=0;
	u16 temp[RemoteID_Length]={0};
	
	if(key != 0xEE8899AA) 
	{
		return FALSE;
	}
	memcpy(Tmp, ID, RemoteID_Length);
	Tmp[RemoteID_Length] = Cal_crc8(Tmp, RemoteID_Length);
  for(i=0;i<=RemoteID_Length;i=i+2)
	{
		temp[j]=(u16)(((u16)Tmp[i+1]<<8) + Tmp[i]);
		Test_Write(REMOTE_ID_ADDR_HOME+j*2,temp[j]);
		Test_Write(REMOTE_ID_ADDR_HOME+64+j*2,temp[j]);
		j++;
	}
	FLASH_ReadMoreWord(REMOTE_ID_ADDR_HOME,   (u32 *)Tmp, RemoteID_Length>>1);
	if(Tmp[RemoteID_Length] != Cal_crc8(Tmp, RemoteID_Length))//��һ���������ȶ�
	{
		return FALSE; 
	}
	FLASH_ReadMoreWord(REMOTE_ID_ADDR_HOME+64,(u32 *)Tmp, RemoteID_Length>>1);
	
	if(Tmp[RemoteID_Length] != Cal_crc8(Tmp, RemoteID_Length))//�ڶ����������ȶ�
	{
		return FALSE; 
	}
	return TRUE;
}

u32 FLASH_ReadCount(u32 addr) 
{
	u32 tmp = *((vu32 *)addr); //����FLASH����
	return tmp;
}


/*******************************************************************************
* Function Name  : FLASH_ReadConfigflag
* Description    : ��ָ��FLASH��ַ��ȡƥ��洢ֵ
* Input          : configflag - ָ�룬ֻ��ȡ����8Byte����
* Output         : None
* Return         : bool - TRUE, FALSE
*******************************************************************************/
bool FLASH_ReadConfigflag(u8 configflag[8])
{
	u8 Tmp[8<<1]={0};
	
	FLASH_ReadMoreWord(CONFIG_ADD_HOME, (u32 *)Tmp, 8>>1);  //����һ������������
	if(Tmp[8] != Cal_crc8(Tmp, 8)) //����CRC���� 
	{ 
		FLASH_ReadMoreWord(CONFIG_ADD_HOME + 64, (u32 *)Tmp, 8>>1); //���ڶ�������������
		if(Tmp[8] != Cal_crc8(Tmp, 8))//����CRC���� 
		{ 
			return FALSE;
		}
	}
	memcpy(configflag, Tmp, 8);
	memcpy(configflagtemp, Tmp, 8);
	return TRUE;
}
/*******************************************************************************
* Function Name  : FLASH_ReadRemoteID
* Description    : ��ָ��FLASH��ַ��ȡң��IDֵ������
* Input          : remote - ң������������ָ�룬ֻ��ȡ����20Byte����
* Output         : None
* Return         : bool - TRUE, FALSE
*******************************************************************************/
bool FLASH_ReadRemoteID(u8 ID[RemoteID_Length])
{
	u8 Tmp[RemoteID_Length<<1]={0};
	
	FLASH_ReadMoreWord(REMOTE_ID_ADDR_HOME, (u32 *)Tmp, RemoteID_Length>>1);  //����һ������������
	if(Tmp[RemoteID_Length] != Cal_crc8(Tmp, RemoteID_Length)) //����CRC���� 
	{ 
		FLASH_ReadMoreWord(REMOTE_ID_ADDR_HOME + 64, (u32 *)Tmp, RemoteID_Length>>1); //���ڶ�������������
		if(Tmp[RemoteID_Length] != Cal_crc8(Tmp, RemoteID_Length))//����CRC���� 
		{ 
			EventFlag = EVENT_STUDY_KEY;
			return FALSE;
		}
	}
	memcpy(ID, Tmp, RemoteID_Length);
	return TRUE;
}
/*******************************************************************************
* Function Name  : CANSendData
* Description    : ͨ��CAN��������
* Input          : TxMessageTmp - ���������ݻ���
* Output         : None
* Return         : None
*******************************************************************************/
bool CANSendData(CAN_TypeDef* CANx,CanTxMsg *TxMessageTmp) 
{
	
	u8 TransmitMailbox = CAN_Transmit(CANx, TxMessageTmp);	//����CAN����
	
	#ifndef __DEBUG_PRO
	u32 i = 0;
	while(CAN_TransmitStatus(CANx, TransmitMailbox) != CANTXOK) //�ȴ����ͳɹ�
	{
		if(++i > 0x10000L) //���ͳ�ʱ����λMCU
		{
			/*OnResetMCU();*/
			return FALSE;
		}
	}
	#endif	
	return TRUE;
}  
bool CANSendDataConst(CAN_TypeDef* CANx,CanTxMsg const *TxMessageTmp) 
{
  memcpy((u8 *)&CanTxTmp.StdId, (u8 *)TxMessageTmp, 19);
  return CANSendData(CANx,&CanTxTmp);
}

/*******************************************************************************
* Function Name : WaitSendCANLamp
* Description   : ���յ�lampCAN���ݺ���������˫������ʾ
* Input         : count Ϊ������
* Output        : None
* Return        : None
*******************************************************************************/
void WaitSendCANLamp(u8 count) 
{
	u16 i;

	i=count*0x20;
	CanCmdLamp1.Data[2] = (u8)(i>>8);
	CanCmdLamp1.Data[3] = (u8)i;
	
	CANSendDataConst(CAN2,&ConstCanCmdLamp);	//����˫����
	CANSendData(CAN2,&CanCmdLamp1);	//������˸ʱ��
}

/*******************************************************************************
* Function Name : LFSendData
* Description 	: ��Ƶ���뷢������㺯��
* Input         : lfdata - ���͵�����ָ��
                  lfcount - ���͵������ֽ���max
                  outchannel - ѡ���͵�ͨ����
* Output        : None
* Return        : None
*******************************************************************************/
uc8 BasePlus[50] = {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,/*1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,*/1,1,1,0,0,0,1,0,1,1,0,0,1,1,0,0,1,0};	//LF�ź�����
//uc8 BasePlus[50] = {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,0,0,0,1,0,1,1,0,0,1,1,0,0,1,0};	//LF�ź�����
u8  DataPlus[426];
void LFSendData(u8 *lfdata, u16 lfcount, u8 FirstFlag) 
{
	u16 index = 0;
	u16 Offset = 0;
	u16 t2cycle = TIM2_xus(128);

	if(door != DOOR_UPWINING)
	{
		CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);//��ָֹ����CAN�ж�
	}
	RTCDisableFlag = TRUE;
	lfcount = lfcount << 4;
	if(FirstFlag) //����ͷ
	{ 
		Offset = 34;
		memcpy(DataPlus, BasePlus, 34);
	}
	lfcount += Offset; 
	
	for(index = 0; index < lfcount; ) //�����͵����ݣ�ת���ɱ���������
	{	
		if((lfdata[index >> 4] & (1 << (7 - ((index >> 1) & 0x07)))) != 0) 
		{
		  DataPlus[index+Offset] = 0;
		  index++;
		  DataPlus[index+Offset] = 1;
		  index++;
		}
		else 
		{
		  DataPlus[index+Offset] = 1;
		  index++;
		  DataPlus[index+Offset] = 0;
		  index++;
		}
	}
	TIM2flag = TRUE;
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(LF125K_TIMx, ENABLE);  
	TIM_SetCounter(TIM2, 0);//���� 
	TIM4->CCER |=  0x0111;
	//TIM_CtrlPWMOutputs(LF125K_TIMx, ENABLE);	//������ʱ������
	while(TIM_GetCounter(TIM2) < t2cycle && TIM2flag == TRUE) 
	{
	}
	t2cycle += TIM2_xus(128);	//2*64=128us
	if(DataPlus[0] == 0)
	{
		DataPlus[0] = 1;
	}
	TIM4->CCER &= 0xFEEE;
	for(index = 1; index < lfcount; index++)//������������
	{	
		if(DataPlus[index] == 0)
		{
			TIM4->CCER &= 0xFEEE;
		}
		else if(DataPlus[index - 1] == 0)
		{
			TIM4->CCER |=  0x0111; //��������1���м䲻����������PWM����ֹ���λ��� 
		}
		while(TIM_GetCounter(TIM2) < t2cycle && TIM2flag == TRUE) 
		{
		}
		t2cycle += TIM2_xus(128);	//2*64=128us
	}
	RTCDisableFlag = FALSE;
	TIM4->CCER &=  0xFEEE;
	TIM_Cmd(TIM2, DISABLE);
	TIM_Cmd(LF125K_TIMx, DISABLE);
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);//ʹ��ָ����CAN�ж�
	TIM2flag = FALSE;
}
void LFSendData1(u8 *lfdata, u16 lfcount, u8 FirstFlag) 
{
	u16 index = 0;
	u16 Offset = 0;
	u16 t2cycle = TIM2_xus(128);

	if(door != DOOR_UPWINING)
	{
		CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);//��ָֹ����CAN�ж�
	}
	RTCDisableFlag = TRUE;
	lfcount = lfcount << 4;
	if(FirstFlag) //����ͷ
	{ 
		Offset = 34;
		memcpy(DataPlus, BasePlus, 34);
	}
	lfcount += Offset; 

	for(index = 0; index < lfcount; ) //�����͵����ݣ�ת���ɱ���������
	{	
		if((lfdata[index >> 4] & (1 << (7 - ((index >> 1) & 0x07)))) != 0) 
		{
			DataPlus[index+Offset] = 0;
			index++;
			DataPlus[index+Offset] = 1;
			index++;
		}
		else 
		{
			DataPlus[index+Offset] = 1;
			index++;
			DataPlus[index+Offset] = 0;
			index++;
		}
	}
	TIM2flag = TRUE;
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(LF125K_TIMx, ENABLE);  
	TIM_SetCounter(TIM2, 0);//���� 
	TIM4->CCER |=  0x0001;
	//TIM_CtrlPWMOutputs(LF125K_TIMx, ENABLE);	//������ʱ������
	while(TIM_GetCounter(TIM2) < t2cycle && TIM2flag == TRUE) 
	{
	}
	t2cycle += TIM2_xus(128);	//2*64=128us
	if(DataPlus[0] == 0)
	{
		DataPlus[0] = 1;
	}
	TIM4->CCER &= 0xFFEE;
	for(index = 1; index < lfcount; index++)//������������
	{	
		if(DataPlus[index] == 0)
		{
			TIM4->CCER &= 0xFFEE;
		}
		else if(DataPlus[index - 1] == 0)
		{
			TIM4->CCER |=  0x0001; //��������1���м䲻����������PWM����ֹ���λ��� 
		}
		while(TIM_GetCounter(TIM2) < t2cycle && TIM2flag == TRUE) 
		{
		}
		t2cycle += TIM2_xus(128);	//2*64=128us
	}
	RTCDisableFlag = FALSE;
	TIM4->CCER &=  0xFFEE;
	TIM_Cmd(TIM2, DISABLE);
	TIM_Cmd(LF125K_TIMx, DISABLE);
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);//ʹ��ָ����CAN�ж�
	//CAN_ITConfig(CAN2, CAN_IT_FMP1, ENABLE);//ʹ��ָ����CAN�ж� 
	TIM2flag = FALSE;
}
void LFSendData2(u8 *lfdata, u16 lfcount, u8 FirstFlag) 
{
	u16 index = 0;
	u16 Offset = 0;
	u16 t2cycle = TIM2_xus(128);

	if(door != DOOR_UPWINING)
	{
		CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);//��ָֹ����CAN�ж�
		//CAN_ITConfig(CAN2, CAN_IT_FMP1, DISABLE);//��ָֹ����CAN�ж�
	}
	RTCDisableFlag = TRUE;
	lfcount = lfcount << 4;
	if(FirstFlag) //����ͷ
	{ 
		Offset = 34;
		memcpy(DataPlus, BasePlus, 34);
	}
	lfcount += Offset; 

	for(index = 0; index < lfcount; ) //�����͵����ݣ�ת���ɱ���������
	{	
		if((lfdata[index >> 4] & (1 << (7 - ((index >> 1) & 0x07)))) != 0) 
		{
			DataPlus[index+Offset] = 0;
			index++;
			DataPlus[index+Offset] = 1;
			index++;
		}
		else 
		{
			DataPlus[index+Offset] = 1;
			index++;
			DataPlus[index+Offset] = 0;
			index++;
		}
	}
	TIM2flag = TRUE;
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(LF125K_TIMx, ENABLE);  
	TIM_SetCounter(TIM2, 0);//���� 
	TIM4->CCER |=  0x0010;
	//TIM_CtrlPWMOutputs(LF125K_TIMx, ENABLE);	//������ʱ������
	while(TIM_GetCounter(TIM2) < t2cycle && TIM2flag == TRUE) 
	{
	}
	t2cycle += TIM2_xus(128);	//2*64=128us
	if(DataPlus[0] == 0)
	{
		DataPlus[0] = 1;
	}
	TIM4->CCER &= 0xFFEE;
	for(index = 1; index < lfcount; index++)//������������
	{	
		if(DataPlus[index] == 0)
		{
			TIM4->CCER &= 0xFFEE;
		}
		else if(DataPlus[index - 1] == 0)
		{
			TIM4->CCER |=  0x0010; //��������1���м䲻����������PWM����ֹ���λ��� 
		}
		while(TIM_GetCounter(TIM2) < t2cycle && TIM2flag == TRUE) 
		{
		}
		t2cycle += TIM2_xus(128);	//2*64=128us
	}
	RTCDisableFlag = FALSE;
	TIM4->CCER &=  0xFFEE;
	TIM_Cmd(TIM2, DISABLE);
	TIM_Cmd(LF125K_TIMx, DISABLE);
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);//ʹ��ָ����CAN�ж�
	//CAN_ITConfig(CAN2, CAN_IT_FMP1, ENABLE);//ʹ��ָ����CAN�ж� 
	TIM2flag = FALSE;
}
void LFSendData3(u8 *lfdata, u16 lfcount, u8 FirstFlag) 
{
	u16 index = 0;
	u16 Offset = 0;
	u16 t2cycle = TIM2_xus(128);

	if(door != DOOR_UPWINING)
	{
		CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);//��ָֹ����CAN�ж�
		//CAN_ITConfig(CAN2, CAN_IT_FMP1, DISABLE);//��ָֹ����CAN�ж�
	}
	RTCDisableFlag = TRUE;
	lfcount = lfcount << 4;
	if(FirstFlag) //����ͷ
	{ 
		Offset = 34;
		memcpy(DataPlus, BasePlus, 34);
	}
	lfcount += Offset; 

	for(index = 0; index < lfcount; ) //�����͵����ݣ�ת���ɱ���������
	{	
		if((lfdata[index >> 4] & (1 << (7 - ((index >> 1) & 0x07)))) != 0) 
		{
			DataPlus[index+Offset] = 0;
			index++;
			DataPlus[index+Offset] = 1;
			index++;
		}
		else 
		{
			DataPlus[index+Offset] = 1;
			index++;
			DataPlus[index+Offset] = 0;
			index++;
		}
	}
	TIM2flag = TRUE;
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(LF125K_TIMx, ENABLE);  
	TIM_SetCounter(TIM2, 0);//���� 
	TIM4->CCER |=  0x0100;
	//TIM_CtrlPWMOutputs(LF125K_TIMx, ENABLE);	//������ʱ������
	while(TIM_GetCounter(TIM2) < t2cycle && TIM2flag == TRUE) 
	{
	}
	t2cycle += TIM2_xus(128);	//2*64=128us
	if(DataPlus[0] == 0)
	{
		DataPlus[0] = 1;
	}
	TIM4->CCER &= 0xFEEE;
	for(index = 1; index < lfcount; index++)//������������
	{	
		if(DataPlus[index] == 0)
		{
			TIM4->CCER &= 0xFEEE;
		}
		else if(DataPlus[index - 1] == 0)
		{
			TIM4->CCER |=  0x0010; //��������1���м䲻����������PWM����ֹ���λ��� 
		}
		while(TIM_GetCounter(TIM2) < t2cycle && TIM2flag == TRUE) 
		{
		}
		t2cycle += TIM2_xus(128);	//2*64=128us
	}
	RTCDisableFlag = FALSE;
	TIM4->CCER &=  0xFEEE;
	TIM_Cmd(TIM2, DISABLE);
	TIM_Cmd(LF125K_TIMx, DISABLE);
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);//ʹ��ָ����CAN�ж�
	//CAN_ITConfig(CAN2, CAN_IT_FMP1, ENABLE);//ʹ��ָ����CAN�ж� 
	TIM2flag = FALSE;
}

/*******************************************************************************
* Function Name : ValueRange
* Description 	: �ж�ĳ��ֵ�Ƿ��ڸ�����ֵ֮��
* Input         : compare - ���ж���ֵ
                  original - �����Ƚϵ�ֵ
                  power - �ж����ȣ�1--50%�ڣ�2--25%�ڣ�3--12.5%��
* Output        : None
* Return        : bool - ��ָ����Χ��TRUE
*******************************************************************************/
bool ValueRange(u16 compare, u16 original, u8 power) 
{
	u16 finally = 0;
#if TIM2_xus(128) == 64
	original = original >> 1;	//��ʱ��TIM2Ϊ2us������λ����40ms = 20000  
#endif
	finally = original >> power;
	if(compare > (original + finally) || compare < (original - finally)) 
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}
bool ValueVGARange(u16 compare,u16 original)
{
	//if((compare>2100)&&(compare<3100))
	if(compare>=original)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


u8 UhfRevData_First(u8 *uhfdata, u16 checktime) 
{
	u8 index = 0;
	u16 risingvalue = U16_MAX - checktime * TIM2_xus(1000);	//2us����ʱ��    ���ʱ�����Ϊ6.5ms
	u16 fallingvalue = risingvalue;
	u8 step =0;
  u16 uhfpluse[300]={0};
	u8 i=0;
 
	RTCDisableFlag = TRUE;
	if(door != DOOR_UPWINING) 
	{
		CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);//��ָֹ����CAN�ж�
		CAN_ITConfig(CAN2, CAN_IT_FMP0, DISABLE);//��ָֹ����CAN�ж�
	}
	TIM2flag = TRUE;
	TIM_Cmd(TIM2, ENABLE);
	TIM_SetCounter(TIM2, risingvalue);	//���ö�ʱ����ʼ��ʱֵ
	if(ChangeRevMode == TRUE)//ʹ�ø��Ͻ��ĸ�Ƶ�����ж�
	{ 
		while(1) 
		{
			//TESTOUT;/////////////////////////
			while(READ_UHF_IN == Bit_RESET && TIM2flag == TRUE) 
			{
				
				
			}

			while(READ_UHF_IN == Bit_SET && TIM2flag == TRUE) 
			{
			
			}
			fallingvalue = TIM_GetCounter(TIM2);
			uhfpluse[step]=fallingvalue-risingvalue;
			risingvalue=fallingvalue;
			step++;
			//��ʱ����ʱ������ 
			if(TIM2flag == FALSE) 
			{
				index = 0;
				break;
			}	
		} 
	}
	else //ʹ�ýϿ��ɵĸ�Ƶ�����ж�
	{  
	

	}
	//0   1  2   3  4  5  6   7  8  9  10 11 12 13 14 15  16 17 18 19  20
	//184 52 52 52 52 52 52 100 100 52 77 77 52 52 100 52 52 77 52 157
	for(i=0;i<step;i++)
	{
		if(index==0)
		{
			start:	if(ValueRange(uhfpluse[i], 100, 2))   //�������ݵ���ʼλ��
			{
				index++;
			}
			else
			{
				index=0;
			}
		}
		else if(index==2||index==5/*||index==6||index==8||index==9||index==10||index==11*/)
		{
			if(TRUE == ValueRange(uhfpluse[i], 52, 2))
			{
				index++;
			}
			else
			{
					index=0;
					goto start;  //������Ϊ�˷�ֹ���������������ִ���52������
			}
		}
		else if(/*index==7||*/index==1)
		{
			if(TRUE == ValueRange(uhfpluse[i], 100, 2))
			{
				index++;
			}
			else
			{
				index=0;
			}
		}
		else if(index==3||index==4/*||index==12*/)
		{
			if(TRUE == ValueRange(uhfpluse[i], 77, 2))
			{
				index++;
			}
			else
			{
				index=0;
			}
		}	
	}
	if(index!=6)  //index>=19 ��ʾ������������
	{
		index=0;
	}
	RTCDisableFlag = FALSE;
	TIM_Cmd(TIM2, DISABLE);
	TIM2flag = FALSE;
		
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);//ʹ��ָ����CAN�ж�
	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);//ʹ��ָ����CAN�ж�
	return index;
}
/*******************************************************************************
* Function Name : OnEventCarLFSend
* Description   : ԭ����Ƶ�����¼�����
* Input         : None
* Output        : None
* Return        : None
*******************************************************************************/
u8 CarFindRemoteFlag=0xFF;
void OnEventCarLFSend(void)
{
	DelayXms(100);
	//CANSendDataConst(CAN2,&ConstCanCmdCarLFSEND);	//����ԭ����Ƶ��Կ��
	CANSendData(CAN2,&ConstCanCmdCarLFSEND);
	//CAN_Transmit(CAN1,&ConstCanCmdCarLFSEND);
	CarFindRemoteFlag=0x00;
	CarLFFlagBcak =FALSE;
	
	DelayXms(600);
	
}
/*******************************************************************************
* Function Name : OnEvnetLFSend
* Description   : ��Ƶ�����¼�����
* Input         : None
* Output        : None
* Return        : None
*******************************************************************************/
u8 FindRemoteFlag = 0xFF;
u32 lfsendcount=0;
u8  carlfcnt=0;
void OnEvnetLFSend(void) 
{
  u8 FirstUnlamp = 0, FirstLamp = 0,SecondLamp=0;
  
  if(CarPower == CAR_POWER_ACC) 
  {
  	  return; //��������״̬��ֹͣLF��Ѱң���� 
  }
  
  if(Disable_lf_flag == TRUE) //�ֶ�ң��LF ��ģʽ
  {	
	  return;
  }	
	if(Disable_lf ==TRUE)      //����Ϊ��Ƶ��ģʽ
	{
		return;
	}
	if(Disable_lf_unlock==TRUE)     //��ԭ������������ģ��
	{
		return;
	}
	if(Disable_lf_car_flag)       //����ȫ��ʱ��ԭ���ҵ�Կ�ף������ص���Ƶ��Կ��
	{
		return;
	}
	if(DoorOpenLFsleep==LF_SLEEPED)//����״̬�������Сʱ�ҵ�Կ���Զ��رյ�Ƶ
	{
		return;
	}
	if(LFPowerOff_doorclose==TRUE)   //Ϩ��ʱ��û�ж�����ȫ�أ�
	{
		return;
	}
  if(RTCValue > _72H_STOP_LF_TIME) 
  {
  	  return;  //72Сʱ����
  }
  NXP_LED_ON;
	if(door==DOOR_OPEN)      //���ſ���ԭ��Կ��
  {
		if(CarDoorOpenFlag==TRUE)    //
		{
			LFPrvdoordata=CarDoorOpenFlag;
			NXP_LED_OFF;
			return;
		}
		else if(LFPrvdoordata!=CarDoorOpenFlag)
		{  
			if(carlfcnt==2)
			{	
				FindRemoteFlag = 0xFF;    //������������߱�־
				OnEventCarLFSend();
				LFPrvdoordata=CarDoorOpenFlag;
				carlfcnt=1;
				NXP_LED_OFF;
				return;
			}
			else if(carlfcnt<2)
			{
				carlfcnt++;
				//return;
			}
			else    //���в�����ȥ
			{
				UnfindRemoteCount=0;
				carlfcnt=1;
			}
		}
  }
  FindRemoteFlag = 0;   //����Ѱ��ң����
	if(BACKLFOPENFLAG)
	{
		vInitRadio(1);
		bGoRx();
		LFSendData(LFData_Open, RemoteID_Length, TRUE);  //����ͨ�û��ѲⳡǿLF
		DelayXms(2);
		lfsendcount++;
		FirstLamp = UhfRevData_First(Data,20);  //����Ƿ���ң�ػ���
		//FirstLamp= UhfRevData_HIGH(Data,10);  //����Ƿ���ң�ػ���    7 20
		bGoStandby();
		bGoSleep();
		if(FirstUnlamp)//�и�Ƶ����
		{
			NXP_LED_OFF;
			return; 
		} 
		if(FirstLamp) //����ң��1
		{  
			FindRemoteFlag = 1; 
			NXP_LED_OFF; 
			return;
		}
		vInitRadio(0);
		bGoRx();
		LFSendData(LFData_Open, RemoteID_Length, TRUE); //���ͷ���ͻLF�ҵڶ�ң��
		DelayXms(2);
		SecondLamp = UhfRevData_First(Data,20);  //����Ƿ���ң�ػ���
		bGoStandby();
		bGoSleep();
		if(SecondLamp) //����ң��
		{ 
			FindRemoteFlag = 1; 
			NXP_LED_OFF; 
			return;
		}
	}
	else
	{
		vInitRadio(1);
		LFSendData1(LFData_Open, RemoteID_Length, TRUE);  //����ͨ�û��ѲⳡǿLF ��������λ
		DelayXms(1);
		lfsendcount++;
		FirstLamp = UhfRevData_First(Data,20);  //����Ƿ���ң�ػ���
		if(FirstLamp) //����ң��1
		{  
			FindRemoteFlag = 1; 
			bGoStandby();          //�ҵ�Կ�׽�������
			bGoSleep();
			NXP_LED_OFF; 
			return;
	  	}
		if(FindRMLFFlag)    //��������������Կ�� 
		{
			DelayXms(2);
			//	vInitRadio(1);       //�����Ѿ���ʼ����û���ҵ�Կ�ײ��Һ�������
			LFSendData2(LFData_Open, RemoteID_Length, TRUE);  //����ͨ�û��ѲⳡǿLF    ���Ҹ���λ
			DelayXms(1);
			FirstLamp = UhfRevData_First(Data,20);  //����Ƿ���ң�ػ���
			if(FirstLamp) //����ң��
			{ 
				FindRemoteFlag = 1; 
				bGoStandby();            //�ҵ�Կ�׽�������
				bGoSleep();
				NXP_LED_OFF; 
				return;
			 }
		}
		//���ҵڶ���Ƶ��314��6M
		DelayXms(2);
		vInitRadio(0);               //��һ��Ƶ��û�У�������Ƶ���ҵڶ���Ƶ��
		LFSendData1(LFData_Open, RemoteID_Length, TRUE);  //����ͨ�û��ѲⳡǿLF   ��������λ
		DelayXms(1);
		SecondLamp = UhfRevData_First(Data,20);  //����Ƿ���ң�ػ���
		if(SecondLamp) //����ң��
		{ 
			FindRemoteFlag = 1; 
			bGoStandby();
			bGoSleep();
			NXP_LED_OFF; 
			return;
	  	}
		if(FindRMLFFlag)    //��������������Կ�� 
		{
			DelayXms(2);
			//vInitRadio(0);
			LFSendData2(LFData_Open, RemoteID_Length, TRUE);  //����ͨ�û��ѲⳡǿLF  ,�Ҹ���λ
			DelayXms(1);
			SecondLamp = UhfRevData_First(Data,20);  //����Ƿ���ң�ػ���
			if(SecondLamp) //����ң��
			{ 
				FindRemoteFlag = 1; 
				bGoStandby();
				bGoSleep();
				NXP_LED_OFF; 
				return;
		  	}
		}
	}
	if(door==DOOR_CLOSED)      //���ſ���ԭ��Կ��
	{
		if(ENCarLFFlag)
		OnEventCarLFSend();
	}

	bGoStandby();
	bGoSleep();
	FindRemoteFlag = 0;
	NXP_LED_OFF;
	return;
}
/*******************************************************************************
* Function Name : CheckLfRemote
* Description   : ����Ƶ�Ƿ���յ���Чң����
* Input         : None
* Output        : None
* Return        : None
*******************************************************************************/

void CheckLfRemote(void)
{
	if(DoorOpenLFsleep == LF_SLEEPED||Disable_lf == TRUE)return;  //����ң��Ҫ�˳������¼��仯����ֹ�Զ�����
	if((CarFindRemoteFlag!=0xFF)&&(CarLFFlagBcak==TRUE))     //ԭ������Կ���ҵ�
	{
		if(CarFindRemoteFlag ==1)
		{
			CarFindRemoteFlag=0xFF;
//			if(CarDoorOpenFlag==FALSE)     //����ȫ��  ��ͣ��Կ��
//			{
				Disable_lf_car_flag=TRUE;
//			}
			if(ENCarLFFlag)      //���伤��ԭ���ң��ҵ�һ�ξͿ���
			{
				EventFlag = EVENT_REMOTE_OPEN;
				return;
			}
		}	
	}
	if(FindRemoteFlag != 0xFF)//������һ�β���ң�������¼�
	{ 
		if(FindRemoteFlag == 1) //�ҵ���ң����
		{ 
			if(door != DOOR_OPEN)//ԭ����������������״̬���߷���״̬
			{ 
				FindRemoteCount++;
				if(FindRemoteCount > RevIndexTotal) 
				{
					EventFlag = EVENT_REMOTE_OPEN;  //�����¼�
					lfsendcount=0x00;
					CloseCount=0;
					if(BACKLFOPENFLAG==FALSE)    //�򿪺����߿���
					{
						FindRMLFFlag =TRUE;
						SetTimer(TIMER_BACKLFFIND, _FIND_BACKRM_LF_TIME);
					}
				}
				else 
				{
					SetTimer(TIMER_LF, _FIND_FAST_LF_TIME);  //���ٽ���2�μ��
				}
			}
			else //ԭ���ǽ���״̬
			{  
				if(UnfindRemoteCount) 
				{
					SetTimer(TIMER_LF, _FIND_SLOW_LF_TIME);  //��δ�ҵ�ң�صļ�¼���ָ���LF���
				}
				DoorOpenLfcount++;
				CloseCount=0;
				if(DoorOpenLfcount>=120&&lfsendcount>=120)    //60 2���� 150 5���� ���ڽ���״̬������ң�����ݴﵽ5���ӣ�������Ƶ����ģʽ
				{
					DoorOpenLFsleep = LF_SLEEPED;  
					PrvCarDoorOpenFlag=CarDoorOpenFlag;
					WaitSendCANLamp(2);     //˫������˸
				}
			}
			UnfindRemoteCount = 0;  //��δ�ҵ�ң�����ۻ�
			DoorOpenNoLfcount =0;
		}
		else //δ�ҵ�ң����
		{ 
			if((CarIsLockFlag==FALSE)&&(Lockedf==0))
			{
				door=DOOR_OPEN;
			}
			DoorOpenNoLfcount++;
			if(door == DOOR_OPEN) //ԭ���ǽ���״̬
			{ 
				if(!UnfindRemoteCount)
				{
					SetTimer(TIMER_LF, _FIND_MID_LF_TIME);  //��һ��δ�ҵ�ң�أ��ӿ�LF���
				}
				UnfindRemoteCount++;
				if(UnfindRemoteCount > 1)//�����Ĵκ��ж�Ϊ��Ч��ң���뿪����
				{
					CloseCount++;
					if(CloseCount<=5)
					{
						EventFlag = EVENT_REMOTE_CLOSE;
					}
					if(BACKLFOPENFLAG==FALSE)    //�򿪺����߿���
					{
						FindRMLFFlag =TRUE;
						SetTimer(TIMER_BACKLFFIND, _FIND_BACKRM_LF_TIME);
					}
				}
				else//δ�ҵ�ң���ۻ����㣬������
				{
					//SetTimer(TIMER_LF, _FIND_FAST_LF_TIME);  //���ٽ���2�μ��
				}
			}
			else//ԭ��������������״̬
			{  
				if(FindRemoteCount)
				{
					SetTimer(TIMER_LF, _FIND_MID_LF_TIME);
				}
			}
			FindRemoteCount = 0;  //���ҵ�ң�����ۻ�
      if(DoorOpenNoLfcount>3&&DoorOpenLfcount)
      {
				DoorOpenLfcount = 0;
      }
		}
		FindRemoteFlag = 0xFF;
		if(CarLFFlagBcak==TRUE)
		CarFindRemoteFlag=0xFF;
	}
}

/*******************************************************************************
* Function Name : CheckconfigState
* Description   : ���������Ϣ�Ƿ��иı䣬���򱣴�
* Input         : None
* Output        : None
* Return        : None
*******************************************************************************/
void CheckconfigState(void)
{
		u16 tmp;
	u8 Tmp[10]={0};
	u8 i,j;
	
	
	for(i=0;i<8;i++)
	{
		if(configflagtemp[i]!=configflag[i])
		{
			break;
		}
	}
	
	if(i!=8)
	{
		//WriteConfigToFlash(CONFIG_ADD_HOME, config_flag);
		memcpy(Tmp, configflag, 8);
		Tmp[8] = Cal_crc8(Tmp, 8);
		for(i=0,j=0;i<=8;i=i+2)
		{
			tmp=(u16)(((u16)Tmp[i+1]<<8) + Tmp[i]);
			Test_Write(CONFIG_ADD_HOME+j*2,tmp);
			Test_Write(CONFIG_ADD_HOME+64+j*2,tmp);
			j++;
		}
		FLASH_ReadMoreWord(CONFIG_ADD_HOME,   (u32 *)Tmp, 8>>1);
		if(Tmp[8] != Cal_crc8(Tmp, 8))//��һ���������ȶ�
		{
			return ; 
		}
		FLASH_ReadMoreWord(CONFIG_ADD_HOME+64,(u32 *)Tmp, 8>>1);
	
		if(Tmp[8] != Cal_crc8(Tmp, 8))//�ڶ����������ȶ�
		{
			return ; 
		}
		for(i=0;i<8;i++)
		{
			configflagtemp[i]=configflag[i];
		}
		return ;

	}
}

/*******************************************************************************
* Function Name : OnEventRemoteOpen
* Description   : ���������¼�����
* Input         : None
* Output        : None
* Return        : None
*******************************************************************************/
void OnEventRemoteOpen(void) 
{
  NXP_LED_ON;
	KillTimer(TIMER_UPWIN);    //�ص����ڳĶ�ʱ��
	KillTimer(TIMER_ANTI_WAIT);
	KillTimer(TIMER_POWEROFF);
		
	OPEN_ANIT=FALSE;
	DoorHron_flag = FALSE;
	TboxCnt=0;   //β���ۼ�����
	ENCarLFFlag = FALSE;       //�������ԭ����Ƶ���߱�־
	CarCmdAntiTheft.Data[0]=0x82;
	Lockedf=0;
	CANSendData(CAN1, &CarCmdAntiTheft);   //�������,����������
	CANSendDataConst(CAN1,&CanCmdCart);//����ԭ��CAN����
	DelayXms(60);
	CarCmdAntiTheft.Data[0]=0x82;//����֮�󣬴򿪷���
	CANSendDataConst(CAN1,&CarCmdAntiTheft);
	DelayXms(40);
	CarCmdAntiTheft.Data[0]=0x82;//����֮�󣬴򿪷���
	CANSendDataConst(CAN1,&CarCmdAntiTheft);
	DelayXms(40);
	CANSendDataConst(CAN2,&cmd_outo1_security);
	AlarmOpen_flag=FALSE;
	CANSendDataConst(CAN2,&cmd_outo1_security);   	//���ͷ����˳�
	CANSendData(CAN1, &CarCmdAntiTheft);   //�������,����������
	CANSendData(CAN1, &CarCmdAntiTheft);   //�������,����������
	CANSendDataConst(CAN2,&ConstCarCmdDoorPWR);
	CANSendDataConst(CAN2,&ConstCarCmdDoorPWR);    //����ԭ���ſ���ģ���Դ���ſ�����ָ����ܴ���ԭ��Կ������
	DelayXms(60);
	if(Disableglass==FALSE)
	{
		CANSendDataConst(CAN2,&ConstCanCmdOpenGlass);	//�򿪺��Ӿ�
	}
	DelayXms(800);
	if(LOCKMODEFLAG==TRUE)
	{

		CANSendDataConst(CAN2,&ConstCarCmdDoorPWR);
		CANSendDataConst(CAN2,&ConstCanCmdUnLock);	//��������
	}
	if(Disableglass==FALSE)
	{
		DelayXms(50);
		CANSendDataConst(CAN2,&ConstCanCmdOpenGlass);	//�򿪺��Ӿ�
	}
	DelayXms(200);
	
	NXP_LED_OFF;
 if(CarIsLockFlag==FALSE)
	{
		door = DOOR_OPEN;
		DelayXms(200);
		WaitSendCANLamp(3);//˫��2s
		SetTimer(TIMER_LF, _FIND_SLOW_LF_TIME);
		DelayXms(50);
		CANSendDataConst(CAN2,&ConstCanCmdlight);
		CANSendDataConst(CAN2,&CanCmdlight);
	}
}
/*******************************************************************************
* Function Name : OnEventRemoteClose
* Description   : ���������¼�����
* Input         : None
* Output        : None
* Return        : None
*******************************************************************************/
void OnEventRemoteClose(void) 
{
  NXP_LED_ON;
	if(CarIsLockFlag)       
	{
		if(Lockedf==0)    //˵����ԭ��Կ������
		{
			door = DOOR_CLOSE;
			return;
		}
	}
	CANSendDataConst(CAN2,&cmd_lock_data);   //����
	DelayXms(20);
	CANSendDataConst(CAN2,&cmd_lock_data);   //����

	DelayXms(50);
	if(Disable_win==FALSE)
	{
		CANSendDataConst(CAN2,&ConstCanCmdUpWin_F);	//��ǰ��
		DelayXms(10);
		CANSendDataConst(CAN2,&ConstCarCMDOBDSKY1);	//���촰
		CANSendDataConst(CAN2,&ConstCarCMDOBDSKY2);	
	}
	if(Disableglass==FALSE)
	{
		DelayXms(60);
		CANSendDataConst(CAN2,&ConstCanCmdCloseGlass);	//�۵����Ӿ�
		DelayXms(60);

		CANSendDataConst(CAN2,&ConstCanCmdCloseGlass);	//�۵����Ӿ�
	}
	Lockedf=1;
	DelayXms(20);
	CANSendDataConst(CAN2,&ConstCanCmdLock);	//����ָ��
	
	if(/*(DisablePoweroff==FALSE)&&(*/DisablePoweroffOncharg==FALSE)   //�����ϵ�ط�Χ��
	{
		SetTimer(TIMER_POWEROFF, _WAIT_POWEROFF_TIME);
	}
	NXP_LED_OFF;
	DelayXms(200);
	if(CarIsLockFlag)       //20181030 add
	{
		door = DOOR_CLOSE;
		WaitSendCANLamp(1);
		DelayXms(100);
//		if(Disable_hron == FALSE&&CarDoorOpenFlag==FALSE)
//		{
//			if(HronSelFlag)
//				CANSendDataConst(CAN2,&ConstCanCmdHorn1);  //Ĭ������������һ��
//			else
				CANSendDataConst(CAN2,&ConstCanCmdHorn0);  //��������Сһ��
//		}
	}
//	SetTimer(TIMER_ANTI_WAIT, _WAIT_ANTITHEFH_TIME);  //���������򿪶�ʱ��

}
/*******************************************************************************
* Function Name : OnEventStudy
* Description   : ѧϰң����ID�¼�����
* Input         : None
* Output        : None
* Return        : None
*******************************************************************************/
void OnEventStudy(void) 
{
	u32 Timeout = StudyTime + RTCValue;
	u8 i;
	u8 j;
	u8 k=RemoteID_Length;
	u8 count;
	bool res;

	USART_Cmd(UART4, ENABLE);	
	NXP_POWER_ON;
	NXP_NXP_KEYOPEN_ON;
	NXP_LED_ON;
	for(i = 0; ; i++) 
	{
		while(USART_GetFlagStatus(UART4, USART_FLAG_RXNE) == RESET) 
		{
			#ifndef __DEBUG_PRO
				IWDG_ReloadCounter();	//ι��
			#endif
			Drv_UartQueSever();
		}
		if(i == 64) 
		{
			i = 0;
		}
		Data[i] = USART_ReceiveData(UART4);
		if(Debug_flag)
		{
			printf("%02X ",Data[i]);
		}
		if(i>RemoteID_Length+1)
		{
    //                       //data[1]               //data0                                  //data4
			if((Data[i]==Data[i-k])&&(Data[i-1]==Data[i-k-1])&&(Data[i-2]==Data[i-k-2]))//��ȷƥ��Կ��
			{
				if(Debug_flag)
				{
					printf("��Ƶ����");
					for(j = 0; j < k; j++)
					{
							LFData_Open[j] = Data[i-(k+2)+j];
							printf("0x");
							printf("%02X ",LFData_Open[j]);
					}
					printf("����");
				}
				for(j = 0; j < k; j++)
				{
					LFData_Open[j] = Data[i-(k+2)+j];
				}
				res = FLASH_WriteRemoteID(LFData_Open, 0xEE8899AA);  
				if(res != TRUE) 
				{
					res=FLASH_WriteRemoteID(LFData_Open, 0xEE8899AA);
				}
				if(res != TRUE)
				{
					i = 0;
				}
				else         
				{
					NXP_LED_OFF;
					NXP_NXP_KEYOPEN_OFF;
					CANSendDataConst(CAN2,&ConstCanCmdHorn1);
					for(count = 0; count < 17; count++) 
					{
						DelayXms(100);
					}
					CANSendDataConst(CAN2,&ConstCanCmdHorn1);
//					CANSendDataConst(CAN2,&cmd_Set_light);
//					DelayXms(100);
//					CANSendDataConst(CAN2,&cmd_Set_light0);
//					DelayXms(100);
//					CANSendDataConst(CAN2,&cmd_Set_light);
//					DelayXms(100);
//					CANSendDataConst(CAN2,&cmd_Set_light0);
//		    		WaitSendCANLamp(4);	//˫������˸����
					DelayXms(100);
			  		NVIC_SystemReset();//��λ
			  		DelayXms(5);
					NXP_POWER_OFF;
					NXP_NXP_KEYOPEN_OFF;
				}
			}
		}
	}
	
}
void CAN2IrqFuc(void)
{
	CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);	//��ȡ������
	if(RxMessage.StdId== CarGetlonkmode.StdId)
	{
		if((RxMessage.Data[0]&0x01)==0x01)
		{
			LOCKMODEFLAG=TRUE;
			configflag[4]=1;
		}
		else if((RxMessage.Data[0]&0x02)==0x02)
		{
			LOCKMODEFLAG=FALSE;
			configflag[4]=0;
		}
		else
		{
		}
		return;
	}
		if(RxMessage.StdId==CarChargingState.StdId)     //modify  20180912 
	{
		if(RxMessage.Data[6]==0x05)    //���
		{
//			DisablePoweroff=TRUE;    //��ȥ�ϵ�
			DisablePoweroffOncharg = TRUE;
		}
		else    //δ��
		{
//			DisablePoweroff=FALSE;   //�ϵ�
			DisablePoweroffOncharg =FALSE;
		}
		return;
	}
	if(RxMessage.StdId==ConstCanCarLFBACK.StdId)
	{
		if(RxMessage.Data[3]==ConstCanCarLFBACK.Data[3]&&RxMessage.Data[4]==ConstCanCarLFBACK.Data[4]&&
			RxMessage.Data[5]==ConstCanCarLFBACK.Data[5]&&RxMessage.Data[6]==ConstCanCarLFBACK.Data[6])
		{
			CarLFFlagBcak=TRUE;     //�յ������������·���
			if((RxMessage.Data[7]&0x0F)==0x0F)      //δ�ҵ�Կ��
			{
				CarFindRemoteFlag=0x00;
			}
			else        //�ҵ�
			{
				CarFindRemoteFlag=0x01;
			}
		}
		return;
	}
	if(RxMessage.StdId== CanGetDoorstate.StdId)   //��ȡ����״̬�����š�����ǡ�β�䣩
	{
		doordata=(RxMessage.Data[2]&0x0F);
		doordata=RxMessage.Data[1] + doordata;  //20180828 midfy
		if(doordata==0)
		{
			CarDoorOpenFlag = FALSE;
		}
		else
		{
			CarDoorOpenFlag = TRUE;
		}

		if(PrvCarDoorOpenFlag!=CarDoorOpenFlag)    //����״̬��Կ��������5��������״̬�ָ�
		{
				LFPowerOff_doorclose=FALSE;            //�ָ���Կ��
				if(DoorOpenLFsleep ==LF_SLEEPED)
				{
					DoorOpenLFsleep = LF_NO_SLEEP;
					WaitSendCANLamp(1);
				}
				if(doordata==0)
				{
						UnfindRemoteCount=0;
						SetTimer(TIMER_LF, _FIND_MID_LF_TIME);   //�Źغÿ�ʼ�µ���ʱ
				}
				else
				{ 

				}
				carlfcnt=0;
				DoorOpenLfcount=0;
				PrvCarDoorOpenFlag=CarDoorOpenFlag;
				if(BACKLFOPENFLAG==FALSE)
				{
					FindRMLFFlag=TRUE;     //���������߲���Կ��
					SetTimer(TIMER_BACKLFFIND, _FIND_BACKRM_LF_TIME);
				}
					Disable_lf_car_flag=FALSE;
			}
		if(OPEN_ANIT==TRUE&&RxMessage.Data[0]==0x82)  //��״̬û�п�������ֱ�ӷ�������ָ��
		{
			if(Disable_alarm)   //3�ż����ƿ��ط�����Ĭ�Ͽ�
			{
				CarCmdAntiTheft.Data[0] = 0x84;
				CANSendData(CAN1,&CarCmdAntiTheft);   //����ԭ������
			}
		}
		return;
	}
}

/*******************************************************************************
* Function Name : CAN1IrqFuc
* Description   : CAN���ݽ����ж���Ӧ����
* Input         : None
* Output        : None
* Return        : None
*******************************************************************************/
void CAN1IrqFuc(void)
{
	u16 Tmp_button;
	
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);	//��ȡ������
	
	if(RxMessage.StdId==CarGetWindowSKY.StdId)  //��ȡ�촰״̬
	{
		if(RxMessage.Data[0]==0x00&&RxMessage.Data[3]==0x00) //��
		{
			IsSKYWinOpen = FALSE;
		}
		else
		{
			IsSKYWinOpen =TRUE ;
		}
		return;
	}
	if(RxMessage.StdId==CarFd.StdId)   //��ȡ����״̬
	{
		if(RxMessage.Data[1]==0xF1)
		{
			FD_OPEN_flag=TRUE;
		}
		else
		{
			FD_OPEN_flag=FALSE;
		}
		return;
	}
	if(RxMessage.StdId==CarGetRemote.StdId)   //��ȡ�ֶ�����ң��������
	{
		if((RxMessage.Data[2] & 0x04) == 0x04)    //lock
		{

			OPEN_ANIT =FALSE;    //ԭ��Կ������������Ҫ�ֶ�������
			q_Remotedata=0x04;
			q_Remoteunlockcnt=0;
			KillTimer(TIMER_ANTI_WAIT);
		}
		else if((RxMessage.Data[2]&0x01)==0x01)    //unlock
		{
			if(Disable_lf_flag==FALSE)
			{
				WaitSendCANLamp(2);        //20181029 add
				Disable_lf_flag = TRUE;
				KillTimer(TIMER_UPWIN);
			}
			q_Remotedata=0x01;
		}
		else if(RxMessage.Data[2]==0x00)
		{
			Disable_lf_flag = FALSE;
			if(q_Remotedata==0x01)
			{
				q_Remoteunlockcnt++;
				switch(q_Remoteunlockcnt)
				{
					case 3:
					if((doordata&0x01)==0x01)  //�����Ŵ򿪹رյ�Ƶ
					{
						CANSendDataConst(CAN2,&ConstCanCmdHorn1);  //Ĭ������������һ��     //������
						Disable_lf = TRUE;
						configflag[0]=1;
					}
					else
					{
						CANSendDataConst(CAN2,&ConstCanCmdHorn2);  //Ĭ������������һ��     //
						Disable_lf = FALSE;
						configflag[0]=0;
					}
					q_Remoteunlockcnt=0;
					break;
					case 4:
					if((doordata&0x01)==0x01)  //������ʾ�������ܴ򿪹ر�
					{
						CANSendDataConst(CAN2,&ConstCanCmdHorn1);     //������
						Disable_win =TRUE;
						configflag[1]=1;
						
					}
					else
					{
						CANSendDataConst(CAN2,&ConstCanCmdHorn2);       //��һ��
						Disable_win = FALSE;
						configflag[1]=0;
					}
					q_Remoteunlockcnt=0;
					break;
					case 5:
					if((doordata&0x01)==0x01)//��ݼ�4
					{
							Disableglass=TRUE;   //  ���غ��Ӿ�
							configflag[5]=1;
							CANSendDataConst(CAN2,&ConstCanCmdHorn1);       //������
					}
					else
					{
							Disableglass=FALSE;   //�غ��Ӿ�
							configflag[5]=0;
							CANSendDataConst(CAN2,&ConstCanCmdHorn0);       //��һ��

					}
					q_Remoteunlockcnt=0;
					break;	
					case 6:
					q_Remoteunlockcnt=0;
					break;
					default:
					break;
				}
			}
			q_Remotedata=0;
		}
		else
		{
			q_Remoteunlockcnt=0;
		}
		return;
	}
	if(RxMessage.StdId==CarGetWindowLQ.StdId)//����ǰ����״̬
	{
		if(RxMessage.Data[0]==0x00)
		{
			IsLQWinOpen = FALSE;
		}
		else
		{
			IsLQWinOpen =TRUE;
		}
		return;
	}
	if(RxMessage.StdId==CarGetWindowRQ.StdId) //����ǰ����״̬
	{
		if(RxMessage.Data[0]==0x00)
		{
			IsRQWinOpen = FALSE;
		}
		else
		{
			IsRQWinOpen = TRUE;
		}
		return;
	}
	if(RxMessage.StdId==CarGetWindowLH.StdId) //���ݺ󳵴�״̬
	{
		if(RxMessage.Data[0]==0x00)
		{
			IsLHWinOpen = FALSE;
		}
		else
		{
			IsLHWinOpen = TRUE;
		}
		return;
	}
	if(RxMessage.StdId==CarGetWindowRH.StdId)  //���ݺ󳵴�״̬
	{
		if(RxMessage.Data[0]==0x00)
		{
			IsRHWinOpen = FALSE;
		}
		else
		{
			IsRHWinOpen =TRUE ;
		}
		return;
	}
	if(RxMessage.StdId==CanGetAccState.StdId)  //����״̬
	{
		if(((RxMessage.Data[2]&0x0F)==0x08)||((RxMessage.Data[2]&0x0F)==0x09))
		{
			if(CarPower==CAR_POWER_ACC)      //Ϩ��
			{
//				 CANSendDataConst(CAN2,&ConstCarCMDOBDSKY1);	//���촰
//				 CANSendDataConst(CAN2,&ConstCarCMDOBDSKY2);	
//				 SetTimer(TIMER_UPWIN, _UPWIN_TIME);  //��ҪΪ���ڳ�
				if(CarDoorOpenFlag==FALSE)
				{
					LFPowerOff_doorclose=TRUE;
				}
			}

			UnfindRemoteCount=0;
			CarPower = CAR_POWER_OFF;//CAR_POWER_ON;
		}
		else if((RxMessage.Data[2]&0x0F)==0x0A)
		{
			if(CarPower!=CAR_POWER_ACC)
			{
				if(Disableglass==FALSE)
				{
					CANSendDataConst(CAN2,&ConstCanCmdOpenGlass);	//�򿪺��Ӿ�
				}
				KillTimer(TIMER_UPWIN);
				Disable_lf_unlock=FALSE;
				SetTimer(TIMER_RESET, _RESET_WAIT_TIME); //����������5�붨ʱ��ϵͳ��λ
			}
			CarPower = CAR_POWER_ACC;   //ACC\ON\START
			if(DoorOpenLFsleep ==LF_SLEEPED)
			{
				DoorOpenLFsleep = LF_NO_SLEEP;  //����״̬��Կ��������5��������״̬�ָ�
				WaitSendCANLamp(1);
			}
			DoorOpenLfcount = 0;
			
		}
		else if((RxMessage.Data[2]&0x0F)==0x06)
		{
//			if(CarPower==CAR_POWER_ACC)      //Ϩ��
//			{
//				 CANSendDataConst(CAN2,&ConstCarCMDOBDSKY1);	//���촰
//				 CANSendDataConst(CAN2,&ConstCarCMDOBDSKY2);	
//				 SetTimer(TIMER_UPWIN, _UPWIN_TIME);  //��ҪΪ���ڳ�
//			}
			KillTimer(TIMER_RESET); //ֹͣ��ʱ��ϵͳ��λ
			CarPower = CAR_POWER_OFF;
			
		}
		return;
	}
	if(RxMessage.StdId==CarGetCarLockState.StdId)  //��ȡ����״̬��Ϣ
	{
		if(RxMessage.Data[0]==0x88)return; //�޲���

		if((RxMessage.Data[0]&0x0F)==0x01)   //���Ž���
			{
				if(LOCKMODEFLAG&&((RxMessage.Data[0]&0xF0)==0x10))//11 ���Ž���  21 ���ݽ���    33 ң������    
				{
					CarIsLockFlag = FALSE;
				}
				else if(LOCKMODEFLAG==FALSE)
				{
					CarIsLockFlag = FALSE;
				}
			}
			else
			{
				CarIsLockFlag = TRUE;
			}

		return;
	}
	if(RxMessage.StdId == CarGetCarTbox.StdId)
	{
		if(RxMessage.Data[5]&0x40)
		{
			if(TboxCnt++ == 15)
			{
			 door = DOOR_CLOSED;       //Կ�ױ������ڣ�β�俪��
			 Disable_lf_car_flag=FALSE;
			 Disable_lf_flag=FALSE;
			 if(DoorOpenLFsleep ==LF_SLEEPED)
			 {
					DoorOpenLFsleep = LF_NO_SLEEP;
					WaitSendCANLamp(1);
			 }
			 FindRMLFFlag=TRUE;     //���������߲���Կ��
			 EventFlag = EVENT_LFSEND;   //���Ͼʹ���һ����Կ��
			 ENCarLFFlag =TRUE;
			 SetTimer(TIMER_BACKLFFIND, _FIND_BACKRM_LF_TIME);
		 }
		}
		return;
	}
//	if(RxMessage.StdId == CanGetNumKey.StdId)      //��ȡ���ְ���
//	{
//		if((doordata&0x01)!=0x01)
//		{
//			return;
//		}
//		Tmp_button = (RxMessage.Data[0]<<8) + RxMessage.Data[1];
//		//0100 0400 1000 4000 0001 0004 0010 0040
//		if(Tmp_button == prvTmp_button)
//		{
//			if(button_counter ++ == 70)
//			{
//				if(Tmp_button == 0x0100)//��ݼ�1   ����
//				{
//					if(Disable_lf == TRUE)
//					{
//						WaitSendCANLamp(1);       //��һ��
//						Disable_lf = FALSE;
//						configflag[0]=0;
//					}
//					else
//					{
//						WaitSendCANLamp(2);     //������
//						Disable_lf = TRUE;
//						configflag[0]=1;
//					}
//				}
//				else if(Tmp_button == 0x0400)//��ݼ�2    �ر�������
//				{
//					if(Disable_hron == TRUE)
//					{
//						WaitSendCANLamp(1);       //��һ��
//						Disable_hron = FALSE;
//						configflag[1]=0;
//					}
//					else
//					{
//						WaitSendCANLamp(2);     //������
//						Disable_hron =TRUE;
//						configflag[1]=1;
//					}
//				}

//				else if(Tmp_button == 0x1000)//��ݼ�3    δ������ʾ�ķ�����
//				{
//					if(Disable_alarm==TRUE)
//					{
//						Disable_alarm=FALSE;
//						//config_flag &=~(1<<3);
//						configflag[3]=0;
//						WaitSendCANLamp(1);       //������
//					}
//					else
//					{
//						Disable_alarm=TRUE;
//						//config_flag |=(1<<3);
//						configflag[3]=1;
//						WaitSendCANLamp(2);       //��һ��
//					}
//				}

//				else if(Tmp_button == 0x4000)//��ݼ�4
//				{
//					if(DisablePoweroff==TRUE)
//					{
//						DisablePoweroff=FALSE;   //ȥ�ϵ�
//						//config_flag &=~(1<<5);
//						configflag[5]=0;
//						WaitSendCANLamp(1);       //������
//					}
//					else
//					{
//						DisablePoweroff=TRUE;   //��δ���ű���
//						//config_flag |=(1<<5);
//						configflag[5]=1;
//						WaitSendCANLamp(2);       //��һ��
//					}
//				}

//				else if(Tmp_button == 0x0001)//��ݼ�5     δ������ʾ�Ĵ�������
//				{
//					if(Disable_doorhron==TRUE)
//					{
//						Disable_doorhron=FALSE;
//						//config_flag &=~(1<<2);
//						configflag[2]=0;
//						WaitSendCANLamp(2);       //������
//					}
//					else
//					{
//						Disable_doorhron=TRUE;
//						//config_flag |=(1<<2);
//						configflag[2]=1;
//						WaitSendCANLamp(1);       //��һ��
//					}
//				}

//				else if(Tmp_button == 0x0004)//��ݼ�6       //����������С����
//				{
//					if(HronSelFlag==TRUE)
//					{
//						HronSelFlag=FALSE;
//						//config_flag &=~(1<<6);
//						configflag[6]=0;
//						WaitSendCANLamp(2);       //������
//					}
//					else
//					{
//						HronSelFlag=TRUE;
//						//config_flag |=(1<<6);
//						configflag[6]=1;
//						WaitSendCANLamp(1);       //��һ��
//					}
//				}

//				else if(Tmp_button == 0x0010)//��ݼ�7     ���ú���������
//				{
//					if(BACKLFOPENFLAG==TRUE)
//					{
//						BACKLFOPENFLAG=FALSE;
//						//config_flag &=~(1<<7);
//						configflag[7]=0;
//						WaitSendCANLamp(2);       //������
//					}
//					else
//					{
//						BACKLFOPENFLAG=TRUE;
//						//config_flag |=(1<<7); 
//						configflag[7]=1;
//						WaitSendCANLamp(1);       //��һ��
//					}
//				}

//				else if(Tmp_button == 0x0040)//��ݼ�8     ���õ�Ƶ����
//				{
//					
//				}
//				//���ñ��浽flash
//        //WriteConfigToFlash(CONFIG_ADD_HOME, config_flag);
//			}
//		}
//		else
//		{
//			button_counter=0;
//		}
//		prvTmp_button = Tmp_button;
//		return;
//	}
	if(RxMessage.StdId == CanCmdCart.StdId)
	{
		ConnectFlag = TRUE;
		if((RxMessage.Data[7] == Cal_crc8(MCUID, 4)) &&\
		(RxMessage.Data[6] == Cal_crc8(MCUID, 3)) &&\
		(RxMessage.Data[5] == Cal_crc8(MCUID, 2)) &&\
		(RxMessage.Data[4] == Cal_crc8(MCUID, 1))) 
		{
			AllowWriteFlag = TRUE;
		}
		return ;
	}
}
/*******************************************************************************
* Function Name : FlashLedShow
* Description   : ����LED��˸����
* Input         : None
* Output        : None
* Return        : None
*******************************************************************************/
void FlashLedShow(u8 index) 
{
  u8 i;
  while(1) 
  {
    #ifndef __DEBUG_PRO
      IWDG_ReloadCounter();	//ι��
    #endif
    for(i = 0; i < index; i++) 
	{
      NXP_LED_ON; 
	  DelayXms(50);
	  
      NXP_LED_OFF;
	  DelayXms(250);
    }
    DelayXms(2000);
  }
}
/*******************************************************************************
* Function Name : CheckDriveIsOK
* Description   : �������Ժ���
* Input         : None
* Output        : None
* Return        : None
*******************************************************************************/
void CheckDriveIsOK(void) 
{
  u8 res = 0;
  NXP_LED_OFF;
  DelayXms(200);
  if(READ_IS_CLOSE_UPWIN == Bit_RESET || READ_IS_CHANGE_REV == Bit_RESET || READ_IS_REV_INDEX == Bit_RESET) { FlashLedShow(2);}
  REMOTE_UNLOCK_ON;
  DelayXms(10);
  if(READ_IS_CLOSE_UPWIN == Bit_SET) { FlashLedShow(3);}
  REMOTE_UNLOCK_OFF;
  DelayXms(100);
  REMOTE_LOCK_ON;
  DelayXms(10);
  if(READ_IS_CLOSE_UPWIN == Bit_SET) { FlashLedShow(4);}
  REMOTE_LOCK_OFF;

  LFSendData((u8 *)Lavida_LFData_Open, 5, TRUE);  //����ͨ�û��ѲⳡǿLF
	ChangeRevMode = FALSE;
  res = UhfRevData_First(Data, 10); //����Ƿ��и�Ƶ����
  if(res != 12) FlashLedShow(5);
  while(1);
}
void USER_Configuration(void)
{
//	u8 j;

	EventFlag = EVENT_NOP;
	*(u32 *)MCUID = *(vu32 *)(0x1FFFF7F0);

	CANSendData(CAN1,&CanCmdCart);	//��������CAN����
	DelayXms(2000);
	FLASH_ReadConfigflag(configflag);
	if(configflag[0])Disable_lf = TRUE;else Disable_lf = FALSE;
	if(configflag[1])Disable_win= TRUE;else Disable_win = FALSE;
	if(configflag[2])Disable_doorhron =TRUE;else Disable_doorhron = FALSE;
	if(configflag[3])Disable_alarm =TRUE;else Disable_alarm=FALSE;
	if(configflag[4])LOCKMODEFLAG =TRUE;else LOCKMODEFLAG=FALSE;
	if(configflag[5])Disableglass=TRUE;else Disableglass=FALSE;
	if(configflag[6])HronSelFlag=TRUE;else HronSelFlag=FALSE;
	if(configflag[7])BACKLFOPENFLAG=TRUE;else BACKLFOPENFLAG=FALSE;
#ifdef __DEBUG_PRO
	CarPower = CAR_POWER_OFF;
#endif
	FLASH_ReadRemoteID(LFData_Open);   
	if(CarPower == CAR_POWER_OFF && CarIsLockFlag == TRUE) 
	{
		door = DOOR_CLOSED;
	}
	SetTimer(TIMER_LF, _FIND_SLOW_LF_TIME);
	if(RCC_GetFlagStatus(RCC_FLAG_PORRST) == SET) //�ϵ縴λ,����ѧϰң��ID
	{
//		EventFlag = EVENT_STUDY_KEY;
	}
	RCC_ClearFlag();
	vInitRadio(1);  //��ʼ����Ƶ����оƬ������LFData_Open[4]ѡ�����õĸ�ƵƵ��
	if(bGoRx()==TRUE)
	{
	 	CanCmdCart.Data[0]=0x02;    //��ʾ��Ƶ���óɹ�
	 	CANSendData(CAN1,&CanCmdCart);	//��������CAN����
	}
	else
	{
		NXP_POWER_OFF; 
		DelayXms(1000); 
		NVIC_SystemReset();
	}
	SetTimer(TIMER_BACKLFFIND, _FIND_BACKRM_LF_TIME);//��һ����Կ�� 5����
	PowerOffSever();
}


/******************* (C) COPYRIGHT 2011 ECARTEK *****END OF FILE****/
