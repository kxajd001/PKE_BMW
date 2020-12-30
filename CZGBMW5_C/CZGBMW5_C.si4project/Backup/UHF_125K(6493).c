/***************************************************************				
文件名 	  ：App_public.c
相关文件	  ：
文件实现功能 ：
作者		  ：kxajd
版本		  ：V1.0 20170308
-----------------------------------------------------------------
修改记录:
日  期 		 版本		 修改人 	 修改内容

*****************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "UHF_125K.h"
#include "string.h"
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

extern volatile EventState EventFlag;

vu32 RTCValue = 0;     //当前RTC的值
volatile bool TIM2flag = FALSE; //TIM2定时器工作标志
u32 ElapseTime[TIMER_END]    = {0};  //自用定时器时间步长
vu32 ElapseTimeEnd[TIMER_END] = {0};  //自用定时器时间终点
volatile bool IDEventFlag[TIMER_END]  = {FALSE};  //自用定时器启用
volatile bool OnTimeFlag[TIMER_END]   = {FALSE};  //自用定时器时间到来
volatile bool RTCDisableFlag = FALSE;
DoorState door = DOOR_OPEN;
CarState CarPower = CAR_POWER_ACC;
CanRxMsg RxMessage;   //CAN接收数据
bool AllowCloseUpwinFlag = TRUE;
bool ChangeRevMode = TRUE;
u32 StudyTime = _STUDY_TIME;
u8 RevIndexTotal = 1;
u8 ACCActiveCount = 0;
u8 Data[64] = {0};        //RF接收数据保存
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
bool Disableglass =FALSE;    //关后视镜
bool DisablePoweroffOncharg=FALSE;
bool CarLFFlagBcak =TRUE;
bool FindRMLFFlag =TRUE;
bool BACKLFOPENFLAG =TRUE;    //一直使能后天线
bool ENCarLFFlag =FALSE;
u8 preremote = 0;

u8 MCUID[4];
u8 TboxCnt=0;
u8 doordata=0;
u8 prvdoordata=0;   //20180828 midfy
u8 LFPrvdoordata=0;
u32 button_counter =0;
u16 prvTmp_button =0;
u16 DoorOpenLfcount=0; //解锁状态找遥控计算器
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
u8 configflag[8]={0,0,0,1,1,0,0,1};//{1,1,0,1,1,0,0,0};     厦门要求configflag[7]=0
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


CanTxMsg CanTxTmp             			= {0x07EC,0,CAN_ID_STD,CAN_RTR_DATA,0x04,{0xDD,0xDD,0xDD,0xDD,0x00,0x00,0x00,0x00}};  //CAN发送缓存数组
CanTxMsg CanCmdCart     						= {0x07EC,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0xF1,0x14,0x01,0x01,0x45,0x43,0x41,0x52}};


CanTxMsg const CarGetWindowLQ 			= {0x03B6,0,CAN_ID_STD,CAN_RTR_DATA,0x03,{0x00,0x00,0x00,0x37,0x80,0x00,0x00,0x00}};  //左前车窗位置信息
CanTxMsg const CarGetWindowRQ 			= {0x03B8,0,CAN_ID_STD,CAN_RTR_DATA,0x03,{0x00,0x00,0x00,0x37,0x80,0x00,0x00,0x00}};  //右前车窗位置信息
CanTxMsg const CarGetWindowLH 			= {0x03B7,0,CAN_ID_STD,CAN_RTR_DATA,0x03,{0x00,0x00,0x00,0x37,0x80,0x00,0x00,0x00}};  //左后车窗位置信息
CanTxMsg const CarGetWindowRH 			= {0x03B9,0,CAN_ID_STD,CAN_RTR_DATA,0x03,{0x00,0x00,0x00,0x37,0x80,0x00,0x00,0x00}};  //右后车窗位置信息

CanTxMsg const CarGetlonkmode       = {0x03D5,0,CAN_ID_STD,CAN_RTR_DATA,0x04,{0x29,0x00,0x00,0x37,0x80,0x00,0x00,0x00}};  //单门模式四门模式设定  29 所有车门 2A单门模式
																																												

//CanTxMsg const ConstCanCmdlock_0      = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x40,0x10,0x07,0x31,0x01,0x10,0x01,0x06}}; 
//CanTxMsg const ConstCanCmdlock_01     = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x04,{0x40,0x21,0x06,0xA8,0xAC,0x54,0x02,0x00}}; 
CanTxMsg const ConstCanCmdLock     		= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x07,{0x40,0x05,0x31,0x01,0xAC,0x57,0x02,0x00}};  //OBD 锁车
CanTxMsg const ConstCanCmdUpWin_F   	= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x40,0x06,0x31,0x01,0xA1,0x7F,0x40,0x00}};  //OBD 升前窗
//CanTxMsg const ConstCanCmdUpWin_L   	= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x72,0x06,0x31,0x01,0xA1,0x7F,0x50,0x00}};  //OBD 升后窗
//CanTxMsg const ConstCanCmdCloseGlass 	= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x06,{0x40,0x04,0x2E,0xE4,0x54,0x01,0x00,0x00}};  //OBD 折叠后视镜
CanTxMsg const ConstCanCmdlockAccOFF1 = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x40,0x10,0x07,0x31,0x01,0x10,0x31,0x02}}; 
CanTxMsg const ConstCanCmdlockAccOFF2 = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x04,{0x40,0x21,0x02,0xD9,0xFF,0xFF,0xFF,0xFF}}; 
CanTxMsg const ConstCanCmdHorn   			= {0x5E1,0,CAN_ID_STD,CAN_RTR_DATA,0x07,{0x60,0xF0,0x05,0x01,0x01,0xFF,0xFF,0xFF}};  //OBD 报警声
CanTxMsg const ConstCanCmdHorn1   		= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x07,{0x40,0x05,0x2E,0xD2,0x98,0x00,0x80,0x00}};  //OBD 报警声
CanTxMsg const ConstCanCmdHorn0   		= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x07,{0x40,0x05,0x2E,0xD2,0x98,0x00,0x60,0x00}};  //OBD 报警声
CanTxMsg const ConstCanCmdHorn2   		= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x07,{0x40,0x05,0x2E,0xD2,0x98,0x00,0x40,0x00}};  //OBD 报警声

CanTxMsg const ConstCanCmdALARM       = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x06,{0x56,0x04,0x31,0x01,0xAA,0x70,0xFF,0xFF}};  //OBD 报警声ALARM,持续5S9
CanTxMsg const ConstCanCmdUnLock   		= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x07,{0x40,0x05,0x31,0x01,0xAC,0x57,0x01,0x00}};  //OBD 解锁
//CanTxMsg const ConstCanCmdOpenGlass  	= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x06,{0x40,0x04,0x2E,0xE4,0x54,0x02,0x00,0x00}};  //OBD 打开后视镜
CanTxMsg const ConstCanCmdCloseGlass 	= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x07,{0x40,0x05,0x2E,0xD3,0x24,0x00,0x01,0x00}};  //OBD 折叠后视镜
CanTxMsg const ConstCanCmdOpenGlass  	= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x07,{0x40,0x05,0x2E,0xD3,0x24,0x00,0x02,0x00}};  //OBD 打开后视镜

CanTxMsg const ConstCanCmdLamp  			= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x40,0x10,0x07,0x2E,0xD5,0x42,0x00,0x12}};  //OBD 双闪灯信息
CanTxMsg CanCmdLamp1 									= {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x04,{0x40,0x21,0x00,0x64,0xD5,0x42,0x00,0x12}};  //0064=100 为 闪1s
CanTxMsg const ConstCarCMDOBDSKY1     = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x56,0x10,0x07,0x31,0x01,0xA1,0x86,0xA1}};  //39OBD关天窗
CanTxMsg const ConstCarCMDOBDSKY2     = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x56,0x21,0x64,0x02,0xFF,0xFF,0xFF,0xFF}};  //40OBD关天窗
CanTxMsg const ConstCarCmdCloseSKY  	= {0x026E,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x1B,0x1B,0x1B,0x52,0xFF,0xFF,0xFF,0xFF}};  //关天窗  data1==data2==0x5B 20180509修改
CanTxMsg const ConstCarCmdDoorPWR     = {0x03A3,0,CAN_ID_STD,CAN_RTR_DATA,0x02,{0x08,0xFF,0x00,0x00,0xFF,0xFF,0xFF,0xFF}};  //

CanTxMsg const ConstCanCmdlight  			= {0x06F3,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x40,0x10,0x07,0x2E,0xD5,0x42,0x00,0x4}};  //OBD 开近光灯
CanTxMsg CanCmdlight									= {0x06F3,0,CAN_ID_STD,CAN_RTR_DATA,0x04,{0x40,0x21,0x0B,0xB8,0x00,0x00,0x00,0x00}};  //03E8=1000 为 10s
CanTxMsg const caution_light_data4 		= {0x02B4,0,CAN_ID_STD,CAN_RTR_DATA,0x02,{0x01,0x00,0x0F,0xA0,0xFF,0xFF,0xFF,0xFF}};  //????
CanTxMsg const caution_light_data5 		= {0x02B4,0,CAN_ID_STD,CAN_RTR_DATA,0x02,{0x00,0x00,0x0F,0xA0,0xFF,0xFF,0xFF,0xFF}};  //????
CanTxMsg const off_light					 		= {0x06F3,0,CAN_ID_STD,CAN_RTR_DATA,0x04,{0x40,0x02,0x11,0x01,0xFF,0xFF,0xFF,0xFF}};  //????

		

CanTxMsg const cmd_unlock_data  			= {0x03A3,0,CAN_ID_STD,CAN_RTR_DATA,0x02,{0xC8,0xFF,0x00,0x00,0x00,0x00,0x00,0x00}};  //开锁控制数据	用机械钥匙开锁时采集的数据
CanTxMsg const cmd_lock_data 					= {0x03A3,0,CAN_ID_STD,CAN_RTR_DATA,0x02,{0xD0,0xFF,0x00,0x00,0x00,0x00,0x00,0x00}};  	//上锁控制数据	用机械钥匙上锁时采集的数据

CanTxMsg /*const*/ ConstCanCmdCarLFSEND  	= {0x06F3,0,CAN_ID_STD,CAN_RTR_DATA,0x06,{0x40,0x04,0x31,0x01,0xAC,0x54,0x00,0x00}};  //OBD 激活原车找钥匙
CanTxMsg const ConstCanCarLFBACK     	= {0x0640,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0xF1,0x10,0x0C,0x71,0x01,0xAC,0x54,0x00}};  //OBD 原车找钥匙返回指令

CanTxMsg const CarChargingState     = {0x03B4,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0xFF,0xFF,0xFD,0xFF,0xFF,0xFF,0x05,0xFF}};  //左前车窗位置信息

CanTxMsg  CarGetRemote		 	= {0x023A,0,CAN_ID_STD,CAN_RTR_DATA,0x06,{0x11,0xF3,0x01,0x3F,0xFF,0xF0,0x00,0x00}};  //遥控器信号, Data[2]:0x01-解锁;0x10-后备箱;0x04-锁车;
CanTxMsg  CarGetWindowSKY 				= {0x03BA,0,CAN_ID_STD,CAN_RTR_DATA,0x07,{0x00,0xE1,0x00,0xE4,0x09,0x00,0x00,0x00}};  //data0 为0x00天窗关   天窗状态
CanTxMsg  CanGetDoorstate     		= {0x02FC,0,CAN_ID_STD,CAN_RTR_DATA,0x07,{0x81,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00}};  //车门状态
CanTxMsg  CanGetNumKey    				= {0x00A2,0,CAN_ID_STD,CAN_RTR_DATA,0x02,{0x00,0x40,0x00,0x00,0x8C,0x00,0x00,0x00}};  //中控数字按键
//CanTxMsg  CanGetAccState          = {0x03BB,0,CAN_ID_STD,CAN_RTR_DATA,0x05,{0x02,0xF0,0xFC,0xFF,0xFF,0xFF,0xFF,0xFF}};  //OBD data0==0x00 ACCOFF data0==0x05 accON
CanTxMsg  CanGetAccState          = {0x012F,0,CAN_ID_STD,CAN_RTR_DATA,0x05,{0x02,0xF0,0x86,0xFF,0xFF,0xFF,0xFF,0xFF}};  //OBD data0==0x86 ACCOFF data0==0x8A accON  0x88 accstart

CanTxMsg  CarGetCarLockState		 	= {0x02A0,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x88,0x88,0xF8,0x0C,0xFF,0xFF,0xFF,0xFF}};  //锁车状态信息
CanTxMsg  CarGetCarTbox						= {0x00EC,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0xFF,0xFF,0xFF,0xFF,0x08,0x4F,0xFC,0xFF}};  //尾箱动作
CanTxMsg CarCmdAntiTheft          = {0x02FC,0,CAN_ID_STD,CAN_RTR_DATA,0x07,{0x34,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF}};//车门状态

CanTxMsg const cmd_into_security  = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x56,0x06,0x31,0x01,0xAA,0x79,0x00,0x01}};  //进入防盗控制数据
CanTxMsg const cmd_outo_security  = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x56,0x06,0x31,0x01,0xAA,0x79,0x00,0x00}};  //解除防盗控制数据
CanTxMsg const cmd_outo1_security = {0x23A,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x22,0xF3,0x01,0x3F,0xFC,0xF0,0xFF,0xFF}};  //解除防盗控制数据

CanTxMsg const cmd_Set_light      = {0x3D6,0,CAN_ID_STD,CAN_RTR_DATA,0x03,{0x01,0xF1,0xFF,0x00,0x00,0x00,0x00,0x00}};  //去掉锁车双闪
CanTxMsg const cmd_Set_light0     = {0x3D6,0,CAN_ID_STD,CAN_RTR_DATA,0x03,{0x04,0xF4,0xFF,0x00,0x00,0x00,0x00,0x00}};  //去掉锁车提示音


CanTxMsg const cmd_POWEROFF0      = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x05,{0x12,0x03,0x22,0xf1,0x50,0x00,0x00,0x00}};  //断电指令
CanTxMsg const cmd_POWEROFF1      = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x05,{0x12,0x03,0x22,0x40,0xC3,0x00,0x00,0x00}};  //断电指令
CanTxMsg const cmd_POWEROFF2      = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x04,{0x12,0x30,0x00,0x00,0x00,0x00,0x00,0x00}};  //断电指令
CanTxMsg const cmd_POWEROFF3      = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x05,{0x40,0x03,0x22,0xf1,0x50,0x00,0x00,0x00}};  //断电指令
CanTxMsg const cmd_POWEROFF4      = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x07,{0x40,0x05,0x31,0x01,0xAC,0x51,0x00,0x00}};  //断电指令
CanTxMsg const cmd_POWEROFF5      = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x04,{0xdf,0x02,0x11,0x04,0x00,0x00,0x00,0x00}};  //断电指令
CanTxMsg const cmd_POWEROFF6      = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x05,{0x40,0x03,0x22,0xf1,0x50,0x00,0x00,0x00}};  //断电指令
CanTxMsg const cmd_POWEROFF7      = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x08,{0x40,0x10,0x07,0x31,0x01,0x10,0x01,0x10}};  //断电指令
CanTxMsg const cmd_POWEROFF8      = {0x6F3,0,CAN_ID_STD,CAN_RTR_DATA,0x04,{0x40,0x21,0x10,0x2a,0x00,0x00,0x00,0x00}};  //断电指令


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
* Input          : ptr - 数组地址
                  len - 长度
* Output         : None
* Return         : ECBYTE - 计算结果
*******************************************************************************/
u8 Cal_crc8(u8 *ptr, u8 len) {
  u8 CRCValue = 0;
  u8 i;
  for(i = 0; i < len; i++)
    CRCValue = CrcTable[CRCValue ^ ptr[i]]; //查表方式计算CRC
  return CRCValue;
}

/*******************************************************************************
* Function Name  : SetTimer
* Description    : 启动自用定时器，必须使用KillTimer来关闭，否则一个计时周期后，会继续下一个周期定时
* Input          : nIDEvent - 定时器序号，共TIMER_END
                   nElapse - 定时器定时时长 基数62.5ms
* Output         : None
* Return         : None 
*******************************************************************************/
void SetTimer(u8 nIDEvent, u32 nElapse) { //62.5ms
  OnTimeFlag[nIDEvent] = FALSE; //计时周期结束标志
  ElapseTime[nIDEvent] = nElapse; //计时周期时长
  ElapseTimeEnd[nIDEvent] = RTCValue + nElapse;  //计时周期结束时，RTC时间
  IDEventFlag[nIDEvent] = TRUE; //开启RTC中断时，判断计时周期是否结束
}
/*******************************************************************************
* Function Name  : KillTimer
* Description    : 停止指定序号定时器周期性计时
* Input          : nIDEvent - 定时器序号，共TIMER_END
* Output         : None
* Return         : None 
*******************************************************************************/
void KillTimer(u8 nIDEvent) {
  OnTimeFlag[nIDEvent] = FALSE; //计时周期结束标志
  IDEventFlag[nIDEvent] = FALSE;  //关闭RTC中断时，判断计时周期是否结束
  ElapseTime[nIDEvent] = 0x00L; //清除计时周期时长
  ElapseTimeEnd[nIDEvent] = 0x00L;  //清除计时周期结束时，RTC时间
}

void PowerOffSever(void)
{
		
		CarCmdAntiTheft.Data[0]=0x82;//锁车之后，打开防盗
		CANSendDataConst(CAN1,&CarCmdAntiTheft);
		DelayXms(1);
		CANSendDataConst(CAN2,&ConstCanCmdlockAccOFF1);
			//DelayXms(60);
		CarCmdAntiTheft.Data[0]=0x82;//锁车之后，打开防盗
		CANSendDataConst(CAN1,&CarCmdAntiTheft);
		DelayXms(1);
		CANSendDataConst(CAN2,&ConstCanCmdlockAccOFF2);
		CarCmdAntiTheft.Data[0]=0x82;//锁车之后，打开防盗
		CANSendDataConst(CAN1,&CarCmdAntiTheft);
//		CANSendDataConst(CAN2,&cmd_into_security);
}
/*******************************************************************************
* Function Name  : OnTimer
* Description    : 定时器计时周期结束事件响应
* Input          : nIDEvent - 定时器序号，共TIMER_END
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
		case TIMER_UPWIN:  //升天窗
				CANSendDataConst(CAN2,&ConstCarCMDOBDSKY1);	//升天窗
				DelayXms(5);
				CANSendDataConst(CAN2,&ConstCarCMDOBDSKY2);	
				KillTimer(TIMER_UPWIN);
		break; 
		case TIMER_RESET:
			NVIC_SystemReset(); //复位
			DelayXms(1);
			KillTimer(TIMER_RESET);
		break;
		case TIMER_BACKLFFIND:
			FindRMLFFlag=FALSE;     //关闭后天线查找钥匙
			KillTimer(TIMER_BACKLFFIND);
		break;
		case TIMER_POWEROFF:
			printf("断电");
//			CANSendDataConst(CAN2,&cmd_into_security);   //测试是否一条指令进入防盗
//			DelayXms(60);
			if(secnedflag==0)
			{
				if(IsLQWinOpen||IsLHWinOpen||IsRQWinOpen||IsRHWinOpen||IsSKYWinOpen)
				{
//					if(CarDoorOpenFlag==FALSE)
					{
						CANSendDataConst(CAN2,&ConstCarCMDOBDSKY1);	//升天窗
						DelayXms(2);
						CANSendDataConst(CAN2,&ConstCarCMDOBDSKY2);
						CANSendDataConst(CAN2,&ConstCanCmdUpWin_F);	//升前窗
					}
				}
				else
				{
					if(DisablePoweroffOncharg==FALSE)   //开启断电关氛围灯
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
				if(DisablePoweroffOncharg==FALSE)   //开启断电关氛围灯
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
				CANSendDataConst(CAN2,&ConstCanCmdHorn);  //设置报警声
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
			CANSendDataConst(CAN2,&cmd_into_security);	 //测试是否一条指令进入防盗
			DelayXms(60);
			CANSendDataConst(CAN2,&cmd_into_security);	
			printf("进入防盗");
//			CarCmdAntiTheft.Data[0]=0x34;//锁车之后，打开防盗
//			CANSendDataConst(CAN2,&cmd_into_security);   //测试是否一条指令进入防盗
//			CANSendDataConst(CAN1,&cmd_into_security); 
//			CANSendDataConst(CAN1,&CarCmdAntiTheft);
			KillTimer(TIMER_ANTI_WAIT);
			
			break;
	}
}
/*******************************************************************************
* Function Name  : CheckOnTime
* Description    : 检测自定义定时器周期事件
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
		{ //有周期事件
	      OnTimeFlag[i] = FALSE; 
	      OnTimer(i); //响应周期事件
	   }
  }
}
/*******************************************************************************
* Function Name  : FLASH_WriteRemoteID
* Description    : 存储遥控ID值等数据到指定FLASH地址
* Input          : remote - 遥控器属性数组指针，只获取其中20Byte数据
* Output         : None
* Return         : bool - TRUE, FALSE
*******************************************************************************/
void FLASH_ProgramMoreWord(u32 addr, u32* dwData, u8 len)
{
  u8 i;
  
  for(i = 0; i < len; i++) 
  {
      FLASH_ProgramWord(addr + i * 4, *((u32 *)dwData + i));  //写入FLASH
  }
}
void FLASH_ReadMoreWord(u32 addr, u32* dwData, u8 len) 
{
  u8 i;
  
  for(i = 0; i < len; i++)
  {
    *((u32 *)dwData + i) = *((vu32 *)addr + i); //读出FLASH数据
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
	if(Tmp[RemoteID_Length] != Cal_crc8(Tmp, RemoteID_Length))//第一块数据区比对
	{
		return FALSE; 
	}
	FLASH_ReadMoreWord(REMOTE_ID_ADDR_HOME+64,(u32 *)Tmp, RemoteID_Length>>1);
	
	if(Tmp[RemoteID_Length] != Cal_crc8(Tmp, RemoteID_Length))//第二块数据区比对
	{
		return FALSE; 
	}
	return TRUE;
}

u32 FLASH_ReadCount(u32 addr) 
{
	u32 tmp = *((vu32 *)addr); //读出FLASH数据
	return tmp;
}


/*******************************************************************************
* Function Name  : FLASH_ReadConfigflag
* Description    : 从指定FLASH地址读取匹配存储值
* Input          : configflag - 指针，只获取其中8Byte数据
* Output         : None
* Return         : bool - TRUE, FALSE
*******************************************************************************/
bool FLASH_ReadConfigflag(u8 configflag[8])
{
	u8 Tmp[8<<1]={0};
	
	FLASH_ReadMoreWord(CONFIG_ADD_HOME, (u32 *)Tmp, 8>>1);  //读第一块数据区数据
	if(Tmp[8] != Cal_crc8(Tmp, 8)) //数据CRC错误 
	{ 
		FLASH_ReadMoreWord(CONFIG_ADD_HOME + 64, (u32 *)Tmp, 8>>1); //读第二块数据区数据
		if(Tmp[8] != Cal_crc8(Tmp, 8))//数据CRC错误 
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
* Description    : 从指定FLASH地址读取遥控ID值等数据
* Input          : remote - 遥控器属性数组指针，只获取其中20Byte数据
* Output         : None
* Return         : bool - TRUE, FALSE
*******************************************************************************/
bool FLASH_ReadRemoteID(u8 ID[RemoteID_Length])
{
	u8 Tmp[RemoteID_Length<<1]={0};
	
	FLASH_ReadMoreWord(REMOTE_ID_ADDR_HOME, (u32 *)Tmp, RemoteID_Length>>1);  //读第一块数据区数据
	if(Tmp[RemoteID_Length] != Cal_crc8(Tmp, RemoteID_Length)) //数据CRC错误 
	{ 
		FLASH_ReadMoreWord(REMOTE_ID_ADDR_HOME + 64, (u32 *)Tmp, RemoteID_Length>>1); //读第二块数据区数据
		if(Tmp[RemoteID_Length] != Cal_crc8(Tmp, RemoteID_Length))//数据CRC错误 
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
* Description    : 通过CAN发送数据
* Input          : TxMessageTmp - 待发送数据缓存
* Output         : None
* Return         : None
*******************************************************************************/
bool CANSendData(CAN_TypeDef* CANx,CanTxMsg *TxMessageTmp) 
{
	
	u8 TransmitMailbox = CAN_Transmit(CANx, TxMessageTmp);	//发送CAN数据
	
	#ifndef __DEBUG_PRO
	u32 i = 0;
	while(CAN_TransmitStatus(CANx, TransmitMailbox) != CANTXOK) //等待发送成功
	{
		if(++i > 0x10000L) //发送超时，复位MCU
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
* Description   : 接收到lampCAN数据后立即激活双闪灯提示
* Input         : count 为多少秒
* Output        : None
* Return        : None
*******************************************************************************/
void WaitSendCANLamp(u8 count) 
{
	u16 i;

	i=count*0x20;
	CanCmdLamp1.Data[2] = (u8)(i>>8);
	CanCmdLamp1.Data[3] = (u8)i;
	
	CANSendDataConst(CAN2,&ConstCanCmdLamp);	//激活双闪灯
	CANSendData(CAN2,&CanCmdLamp1);	//设置闪烁时间
}

/*******************************************************************************
* Function Name : LFSendData
* Description 	: 低频编码发送物理层函数
* Input         : lfdata - 发送的数据指针
                  lfcount - 发送的数据字节数max
                  outchannel - 选择发送的通道号
* Output        : None
* Return        : None
*******************************************************************************/
uc8 BasePlus[50] = {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,/*1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,*/1,1,1,0,0,0,1,0,1,1,0,0,1,1,0,0,1,0};	//LF信号引导
//uc8 BasePlus[50] = {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,0,0,0,1,0,1,1,0,0,1,1,0,0,1,0};	//LF信号引导
u8  DataPlus[426];
void LFSendData(u8 *lfdata, u16 lfcount, u8 FirstFlag) 
{
	u16 index = 0;
	u16 Offset = 0;
	u16 t2cycle = TIM2_xus(128);

	if(door != DOOR_UPWINING)
	{
		CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);//禁止指定的CAN中断
	}
	RTCDisableFlag = TRUE;
	lfcount = lfcount << 4;
	if(FirstFlag) //引导头
	{ 
		Offset = 34;
		memcpy(DataPlus, BasePlus, 34);
	}
	lfcount += Offset; 
	
	for(index = 0; index < lfcount; ) //将发送的数据，转化成比特流数组
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
	TIM_SetCounter(TIM2, 0);//清零 
	TIM4->CCER |=  0x0111;
	//TIM_CtrlPWMOutputs(LF125K_TIMx, ENABLE);	//开启定时器计数
	while(TIM_GetCounter(TIM2) < t2cycle && TIM2flag == TRUE) 
	{
	}
	t2cycle += TIM2_xus(128);	//2*64=128us
	if(DataPlus[0] == 0)
	{
		DataPlus[0] = 1;
	}
	TIM4->CCER &= 0xFEEE;
	for(index = 1; index < lfcount; index++)//发送数据脉冲
	{	
		if(DataPlus[index] == 0)
		{
			TIM4->CCER &= 0xFEEE;
		}
		else if(DataPlus[index - 1] == 0)
		{
			TIM4->CCER |=  0x0111; //连续两个1，中间不再重新启动PWM，防止波形畸变 
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
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);//使能指定的CAN中断
	TIM2flag = FALSE;
}
void LFSendData1(u8 *lfdata, u16 lfcount, u8 FirstFlag) 
{
	u16 index = 0;
	u16 Offset = 0;
	u16 t2cycle = TIM2_xus(128);

	if(door != DOOR_UPWINING)
	{
		CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);//禁止指定的CAN中断
	}
	RTCDisableFlag = TRUE;
	lfcount = lfcount << 4;
	if(FirstFlag) //引导头
	{ 
		Offset = 34;
		memcpy(DataPlus, BasePlus, 34);
	}
	lfcount += Offset; 

	for(index = 0; index < lfcount; ) //将发送的数据，转化成比特流数组
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
	TIM_SetCounter(TIM2, 0);//清零 
	TIM4->CCER |=  0x0001;
	//TIM_CtrlPWMOutputs(LF125K_TIMx, ENABLE);	//开启定时器计数
	while(TIM_GetCounter(TIM2) < t2cycle && TIM2flag == TRUE) 
	{
	}
	t2cycle += TIM2_xus(128);	//2*64=128us
	if(DataPlus[0] == 0)
	{
		DataPlus[0] = 1;
	}
	TIM4->CCER &= 0xFFEE;
	for(index = 1; index < lfcount; index++)//发送数据脉冲
	{	
		if(DataPlus[index] == 0)
		{
			TIM4->CCER &= 0xFFEE;
		}
		else if(DataPlus[index - 1] == 0)
		{
			TIM4->CCER |=  0x0001; //连续两个1，中间不再重新启动PWM，防止波形畸变 
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
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);//使能指定的CAN中断
	//CAN_ITConfig(CAN2, CAN_IT_FMP1, ENABLE);//使能指定的CAN中断 
	TIM2flag = FALSE;
}
void LFSendData2(u8 *lfdata, u16 lfcount, u8 FirstFlag) 
{
	u16 index = 0;
	u16 Offset = 0;
	u16 t2cycle = TIM2_xus(128);

	if(door != DOOR_UPWINING)
	{
		CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);//禁止指定的CAN中断
		//CAN_ITConfig(CAN2, CAN_IT_FMP1, DISABLE);//禁止指定的CAN中断
	}
	RTCDisableFlag = TRUE;
	lfcount = lfcount << 4;
	if(FirstFlag) //引导头
	{ 
		Offset = 34;
		memcpy(DataPlus, BasePlus, 34);
	}
	lfcount += Offset; 

	for(index = 0; index < lfcount; ) //将发送的数据，转化成比特流数组
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
	TIM_SetCounter(TIM2, 0);//清零 
	TIM4->CCER |=  0x0010;
	//TIM_CtrlPWMOutputs(LF125K_TIMx, ENABLE);	//开启定时器计数
	while(TIM_GetCounter(TIM2) < t2cycle && TIM2flag == TRUE) 
	{
	}
	t2cycle += TIM2_xus(128);	//2*64=128us
	if(DataPlus[0] == 0)
	{
		DataPlus[0] = 1;
	}
	TIM4->CCER &= 0xFFEE;
	for(index = 1; index < lfcount; index++)//发送数据脉冲
	{	
		if(DataPlus[index] == 0)
		{
			TIM4->CCER &= 0xFFEE;
		}
		else if(DataPlus[index - 1] == 0)
		{
			TIM4->CCER |=  0x0010; //连续两个1，中间不再重新启动PWM，防止波形畸变 
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
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);//使能指定的CAN中断
	//CAN_ITConfig(CAN2, CAN_IT_FMP1, ENABLE);//使能指定的CAN中断 
	TIM2flag = FALSE;
}
void LFSendData3(u8 *lfdata, u16 lfcount, u8 FirstFlag) 
{
	u16 index = 0;
	u16 Offset = 0;
	u16 t2cycle = TIM2_xus(128);

	if(door != DOOR_UPWINING)
	{
		CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);//禁止指定的CAN中断
		//CAN_ITConfig(CAN2, CAN_IT_FMP1, DISABLE);//禁止指定的CAN中断
	}
	RTCDisableFlag = TRUE;
	lfcount = lfcount << 4;
	if(FirstFlag) //引导头
	{ 
		Offset = 34;
		memcpy(DataPlus, BasePlus, 34);
	}
	lfcount += Offset; 

	for(index = 0; index < lfcount; ) //将发送的数据，转化成比特流数组
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
	TIM_SetCounter(TIM2, 0);//清零 
	TIM4->CCER |=  0x0100;
	//TIM_CtrlPWMOutputs(LF125K_TIMx, ENABLE);	//开启定时器计数
	while(TIM_GetCounter(TIM2) < t2cycle && TIM2flag == TRUE) 
	{
	}
	t2cycle += TIM2_xus(128);	//2*64=128us
	if(DataPlus[0] == 0)
	{
		DataPlus[0] = 1;
	}
	TIM4->CCER &= 0xFEEE;
	for(index = 1; index < lfcount; index++)//发送数据脉冲
	{	
		if(DataPlus[index] == 0)
		{
			TIM4->CCER &= 0xFEEE;
		}
		else if(DataPlus[index - 1] == 0)
		{
			TIM4->CCER |=  0x0010; //连续两个1，中间不再重新启动PWM，防止波形畸变 
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
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);//使能指定的CAN中断
	//CAN_ITConfig(CAN2, CAN_IT_FMP1, ENABLE);//使能指定的CAN中断 
	TIM2flag = FALSE;
}

/*******************************************************************************
* Function Name : ValueRange
* Description 	: 判断某数值是否在给定数值之间
* Input         : compare - 待判定的值
                  original - 给定比较的值
                  power - 判定精度，1--50%内，2--25%内，3--12.5%内
* Output        : None
* Return        : bool - 在指定范围内TRUE
*******************************************************************************/
bool ValueRange(u16 compare, u16 original, u8 power) 
{
	u16 finally = 0;
#if TIM2_xus(128) == 64
	original = original >> 1;	//定时器TIM2为2us基本单位，如40ms = 20000  
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
	u16 risingvalue = U16_MAX - checktime * TIM2_xus(1000);	//2us基础时间    检测时间更改为6.5ms
	u16 fallingvalue = risingvalue;
	u8 step =0;
  u16 uhfpluse[300]={0};
	u8 i=0;
 
	RTCDisableFlag = TRUE;
	if(door != DOOR_UPWINING) 
	{
		CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);//禁止指定的CAN中断
		CAN_ITConfig(CAN2, CAN_IT_FMP0, DISABLE);//禁止指定的CAN中断
	}
	TIM2flag = TRUE;
	TIM_Cmd(TIM2, ENABLE);
	TIM_SetCounter(TIM2, risingvalue);	//设置定时器开始计时值
	if(ChangeRevMode == TRUE)//使用更严谨的高频波形判定
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
			//定时器超时，返回 
			if(TIM2flag == FALSE) 
			{
				index = 0;
				break;
			}	
		} 
	}
	else //使用较宽松的高频波形判定
	{  
	

	}
	//0   1  2   3  4  5  6   7  8  9  10 11 12 13 14 15  16 17 18 19  20
	//184 52 52 52 52 52 52 100 100 52 77 77 52 52 100 52 52 77 52 157
	for(i=0;i<step;i++)
	{
		if(index==0)
		{
			start:	if(ValueRange(uhfpluse[i], 100, 2))   //接收数据的起始位置
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
					goto start;  //这里是为了防止丢掉连续两个出现大于52的数据
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
	if(index!=6)  //index>=19 表示数据组接收完成
	{
		index=0;
	}
	RTCDisableFlag = FALSE;
	TIM_Cmd(TIM2, DISABLE);
	TIM2flag = FALSE;
		
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);//使能指定的CAN中断
	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);//使能指定的CAN中断
	return index;
}
/*******************************************************************************
* Function Name : OnEventCarLFSend
* Description   : 原车低频发射事件函数
* Input         : None
* Output        : None
* Return        : None
*******************************************************************************/
u8 CarFindRemoteFlag=0xFF;
void OnEventCarLFSend(void)
{
	DelayXms(100);
	//CANSendDataConst(CAN2,&ConstCanCmdCarLFSEND);	//激活原车低频找钥匙
	CANSendData(CAN2,&ConstCanCmdCarLFSEND);
	//CAN_Transmit(CAN1,&ConstCanCmdCarLFSEND);
	CarFindRemoteFlag=0x00;
	CarLFFlagBcak =FALSE;
	
	DelayXms(600);
	
}
/*******************************************************************************
* Function Name : OnEvnetLFSend
* Description   : 低频发射事件函数
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
  	  return; //汽车开电状态，停止LF找寻遥控器 
  }
  
  if(Disable_lf_flag == TRUE) //手动遥控LF 关模式
  {	
	  return;
  }	
	if(Disable_lf ==TRUE)      //设置为低频关模式
	{
		return;
	}
	if(Disable_lf_unlock==TRUE)     //按原车解锁键休眠模块
	{
		return;
	}
	if(Disable_lf_car_flag)       //车门全关时，原车找到钥匙，立即关掉低频找钥匙
	{
		return;
	}
	if(DoorOpenLFsleep==LF_SLEEPED)//开锁状态连续半个小时找到钥匙自动关闭低频
	{
		return;
	}
	if(LFPowerOff_doorclose==TRUE)   //熄火时门没有动作（全关）
	{
		return;
	}
  if(RTCValue > _72H_STOP_LF_TIME) 
  {
  	  return;  //72小时休眠
  }
  NXP_LED_ON;
	if(door==DOOR_OPEN)      //车门开找原车钥匙
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
				FindRemoteFlag = 0xFF;    //清掉我们找天线标志
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
			else    //运行不到此去
			{
				UnfindRemoteCount=0;
				carlfcnt=1;
			}
		}
  }
  FindRemoteFlag = 0;   //当次寻找遥控器
	if(BACKLFOPENFLAG)
	{
		vInitRadio(1);
		bGoRx();
		LFSendData(LFData_Open, RemoteID_Length, TRUE);  //发送通用唤醒测场强LF
		DelayXms(2);
		lfsendcount++;
		FirstLamp = UhfRevData_First(Data,20);  //检测是否有遥控唤醒
		//FirstLamp= UhfRevData_HIGH(Data,10);  //检测是否有遥控唤醒    7 20
		bGoStandby();
		bGoSleep();
		if(FirstUnlamp)//有高频干扰
		{
			NXP_LED_OFF;
			return; 
		} 
		if(FirstLamp) //发现遥控1
		{  
			FindRemoteFlag = 1; 
			NXP_LED_OFF; 
			return;
		}
		vInitRadio(0);
		bGoRx();
		LFSendData(LFData_Open, RemoteID_Length, TRUE); //发送防冲突LF找第二遥控
		DelayXms(2);
		SecondLamp = UhfRevData_First(Data,20);  //检测是否有遥控唤醒
		bGoStandby();
		bGoSleep();
		if(SecondLamp) //发现遥控
		{ 
			FindRemoteFlag = 1; 
			NXP_LED_OFF; 
			return;
		}
	}
	else
	{
		vInitRadio(1);
		LFSendData1(LFData_Open, RemoteID_Length, TRUE);  //发送通用唤醒测场强LF ，找主驾位
		DelayXms(1);
		lfsendcount++;
		FirstLamp = UhfRevData_First(Data,20);  //检测是否有遥控唤醒
		if(FirstLamp) //发现遥控1
		{  
			FindRemoteFlag = 1; 
			bGoStandby();          //找到钥匙进入休眠
			bGoSleep();
			NXP_LED_OFF; 
			return;
	  	}
		if(FindRMLFFlag)    //如果激活后天线找钥匙 
		{
			DelayXms(2);
			//	vInitRadio(1);       //上面已经初始化，没有找到钥匙才找后面天线
			LFSendData2(LFData_Open, RemoteID_Length, TRUE);  //发送通用唤醒测场强LF    ，找副驾位
			DelayXms(1);
			FirstLamp = UhfRevData_First(Data,20);  //检测是否有遥控唤醒
			if(FirstLamp) //发现遥控
			{ 
				FindRemoteFlag = 1; 
				bGoStandby();            //找到钥匙进入休眠
				bGoSleep();
				NXP_LED_OFF; 
				return;
			 }
		}
		//查找第二种频率314。6M
		DelayXms(2);
		vInitRadio(0);               //第一种频率没有，进行跳频查找第二种频率
		LFSendData1(LFData_Open, RemoteID_Length, TRUE);  //发送通用唤醒测场强LF   ，找主驾位
		DelayXms(1);
		SecondLamp = UhfRevData_First(Data,20);  //检测是否有遥控唤醒
		if(SecondLamp) //发现遥控
		{ 
			FindRemoteFlag = 1; 
			bGoStandby();
			bGoSleep();
			NXP_LED_OFF; 
			return;
	  	}
		if(FindRMLFFlag)    //如果激活后天线找钥匙 
		{
			DelayXms(2);
			//vInitRadio(0);
			LFSendData2(LFData_Open, RemoteID_Length, TRUE);  //发送通用唤醒测场强LF  ,找副驾位
			DelayXms(1);
			SecondLamp = UhfRevData_First(Data,20);  //检测是否有遥控唤醒
			if(SecondLamp) //发现遥控
			{ 
				FindRemoteFlag = 1; 
				bGoStandby();
				bGoSleep();
				NXP_LED_OFF; 
				return;
		  	}
		}
	}
	if(door==DOOR_CLOSED)      //车门开找原车钥匙
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
* Description   : 检测低频是否接收到有效遥控器
* Input         : None
* Output        : None
* Return        : None
*******************************************************************************/

void CheckLfRemote(void)
{
	if(DoorOpenLFsleep == LF_SLEEPED||Disable_lf == TRUE)return;  //不找遥控要退出锁车事件变化，防止自动锁车
	if((CarFindRemoteFlag!=0xFF)&&(CarLFFlagBcak==TRUE))     //原车查找钥匙找到
	{
		if(CarFindRemoteFlag ==1)
		{
			CarFindRemoteFlag=0xFF;
//			if(CarDoorOpenFlag==FALSE)     //车门全关  暂停找钥匙
//			{
				Disable_lf_car_flag=TRUE;
//			}
			if(ENCarLFFlag)      //后备箱激活原车找，找到一次就开锁
			{
				EventFlag = EVENT_REMOTE_OPEN;
				return;
			}
		}	
	}
	if(FindRemoteFlag != 0xFF)//发生了一次查找遥控器的事件
	{ 
		if(FindRemoteFlag == 1) //找到了遥控器
		{ 
			if(door != DOOR_OPEN)//原车是锁车或者升窗状态或者防盗状态
			{ 
				FindRemoteCount++;
				if(FindRemoteCount > RevIndexTotal) 
				{
					EventFlag = EVENT_REMOTE_OPEN;  //开门事件
					lfsendcount=0x00;
					CloseCount=0;
					if(BACKLFOPENFLAG==FALSE)    //打开后天线开关
					{
						FindRMLFFlag =TRUE;
						SetTimer(TIMER_BACKLFFIND, _FIND_BACKRM_LF_TIME);
					}
				}
				else 
				{
					SetTimer(TIMER_LF, _FIND_FAST_LF_TIME);  //快速进行2次检测
				}
			}
			else //原车是解锁状态
			{  
				if(UnfindRemoteCount) 
				{
					SetTimer(TIMER_LF, _FIND_SLOW_LF_TIME);  //有未找到遥控的记录，恢复慢LF检测
				}
				DoorOpenLfcount++;
				CloseCount=0;
				if(DoorOpenLfcount>=120&&lfsendcount>=120)    //60 2分钟 150 5分钟 当在解锁状态连续找遥控数据达到5分钟，则进入低频休眠模式
				{
					DoorOpenLFsleep = LF_SLEEPED;  
					PrvCarDoorOpenFlag=CarDoorOpenFlag;
					WaitSendCANLamp(2);     //双闪灯闪烁
				}
			}
			UnfindRemoteCount = 0;  //清未找到遥控器累积
			DoorOpenNoLfcount =0;
		}
		else //未找到遥控器
		{ 
			if((CarIsLockFlag==FALSE)&&(Lockedf==0))
			{
				door=DOOR_OPEN;
			}
			DoorOpenNoLfcount++;
			if(door == DOOR_OPEN) //原车是解锁状态
			{ 
				if(!UnfindRemoteCount)
				{
					SetTimer(TIMER_LF, _FIND_MID_LF_TIME);  //第一次未找到遥控，加快LF检测
				}
				UnfindRemoteCount++;
				if(UnfindRemoteCount > 1)//连续四次后，判断为有效的遥控离开动作
				{
					CloseCount++;
					if(CloseCount<=5)
					{
						EventFlag = EVENT_REMOTE_CLOSE;
					}
					if(BACKLFOPENFLAG==FALSE)    //打开后天线开关
					{
						FindRMLFFlag =TRUE;
						SetTimer(TIMER_BACKLFFIND, _FIND_BACKRM_LF_TIME);
					}
				}
				else//未找到遥控累积不足，不动作
				{
					//SetTimer(TIMER_LF, _FIND_FAST_LF_TIME);  //快速进行2次检测
				}
			}
			else//原车是锁车或升窗状态
			{  
				if(FindRemoteCount)
				{
					SetTimer(TIMER_LF, _FIND_MID_LF_TIME);
				}
			}
			FindRemoteCount = 0;  //清找到遥控器累积
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
* Description   : 检测配置信息是否有改变，有则保存
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
		if(Tmp[8] != Cal_crc8(Tmp, 8))//第一块数据区比对
		{
			return ; 
		}
		FLASH_ReadMoreWord(CONFIG_ADD_HOME+64,(u32 *)Tmp, 8>>1);
	
		if(Tmp[8] != Cal_crc8(Tmp, 8))//第二块数据区比对
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
* Description   : 解锁控制事件函数
* Input         : None
* Output        : None
* Return        : None
*******************************************************************************/
void OnEventRemoteOpen(void) 
{
  NXP_LED_ON;
	KillTimer(TIMER_UPWIN);    //关掉升内衬定时器
	KillTimer(TIMER_ANTI_WAIT);
	KillTimer(TIMER_POWEROFF);
		
	OPEN_ANIT=FALSE;
	DoorHron_flag = FALSE;
	TboxCnt=0;   //尾箱累加清零
	ENCarLFFlag = FALSE;       //清除激活原车低频天线标志
	CarCmdAntiTheft.Data[0]=0x82;
	Lockedf=0;
	CANSendData(CAN1, &CarCmdAntiTheft);   //解除防盗,并唤醒总线
	CANSendDataConst(CAN1,&CanCmdCart);//唤醒原车CAN总线
	DelayXms(60);
	CarCmdAntiTheft.Data[0]=0x82;//锁车之后，打开防盗
	CANSendDataConst(CAN1,&CarCmdAntiTheft);
	DelayXms(40);
	CarCmdAntiTheft.Data[0]=0x82;//锁车之后，打开防盗
	CANSendDataConst(CAN1,&CarCmdAntiTheft);
	DelayXms(40);
	CANSendDataConst(CAN2,&cmd_outo1_security);
	AlarmOpen_flag=FALSE;
	CANSendDataConst(CAN2,&cmd_outo1_security);   	//发送防盗退出
	CANSendData(CAN1, &CarCmdAntiTheft);   //解除防盗,并唤醒总线
	CANSendData(CAN1, &CarCmdAntiTheft);   //解除防盗,并唤醒总线
	CANSendDataConst(CAN2,&ConstCarCmdDoorPWR);
	CANSendDataConst(CAN2,&ConstCarCmdDoorPWR);    //唤醒原车门控制模块电源，放开这条指令就能打开用原车钥匙锁车
	DelayXms(60);
	if(Disableglass==FALSE)
	{
		CANSendDataConst(CAN2,&ConstCanCmdOpenGlass);	//打开后视镜
	}
	DelayXms(800);
	if(LOCKMODEFLAG==TRUE)
	{

		CANSendDataConst(CAN2,&ConstCarCmdDoorPWR);
		CANSendDataConst(CAN2,&ConstCanCmdUnLock);	//解锁车门
	}
	if(Disableglass==FALSE)
	{
		DelayXms(50);
		CANSendDataConst(CAN2,&ConstCanCmdOpenGlass);	//打开后视镜
	}
	DelayXms(200);
	
	NXP_LED_OFF;
 if(CarIsLockFlag==FALSE)
	{
		door = DOOR_OPEN;
		DelayXms(200);
		WaitSendCANLamp(3);//双闪2s
		SetTimer(TIMER_LF, _FIND_SLOW_LF_TIME);
		DelayXms(50);
		CANSendDataConst(CAN2,&ConstCanCmdlight);
		CANSendDataConst(CAN2,&CanCmdlight);
	}
}
/*******************************************************************************
* Function Name : OnEventRemoteClose
* Description   : 锁车控制事件函数
* Input         : None
* Output        : None
* Return        : None
*******************************************************************************/
void OnEventRemoteClose(void) 
{
  NXP_LED_ON;
	if(CarIsLockFlag)       
	{
		if(Lockedf==0)    //说明是原车钥匙锁车
		{
			door = DOOR_CLOSE;
			return;
		}
	}
	CANSendDataConst(CAN2,&cmd_lock_data);   //锁车
	DelayXms(20);
	CANSendDataConst(CAN2,&cmd_lock_data);   //锁车

	DelayXms(50);
	if(Disable_win==FALSE)
	{
		CANSendDataConst(CAN2,&ConstCanCmdUpWin_F);	//升前窗
		DelayXms(10);
		CANSendDataConst(CAN2,&ConstCarCMDOBDSKY1);	//升天窗
		CANSendDataConst(CAN2,&ConstCarCMDOBDSKY2);	
	}
	if(Disableglass==FALSE)
	{
		DelayXms(60);
		CANSendDataConst(CAN2,&ConstCanCmdCloseGlass);	//折叠后视镜
		DelayXms(60);

		CANSendDataConst(CAN2,&ConstCanCmdCloseGlass);	//折叠后视镜
	}
	Lockedf=1;
	DelayXms(20);
	CANSendDataConst(CAN2,&ConstCanCmdLock);	//锁车指令
	
	if(/*(DisablePoweroff==FALSE)&&(*/DisablePoweroffOncharg==FALSE)   //开启断电关氛围灯
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
//				CANSendDataConst(CAN2,&ConstCanCmdHorn1);  //默认设置声音大一点
//			else
				CANSendDataConst(CAN2,&ConstCanCmdHorn0);  //设置声音小一点
//		}
	}
//	SetTimer(TIMER_ANTI_WAIT, _WAIT_ANTITHEFH_TIME);  //开启防盗打开定时器

}
/*******************************************************************************
* Function Name : OnEventStudy
* Description   : 学习遥控器ID事件函数
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
				IWDG_ReloadCounter();	//喂狗
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
			if((Data[i]==Data[i-k])&&(Data[i-1]==Data[i-k-1])&&(Data[i-2]==Data[i-k-2]))//正确匹配钥匙
			{
				if(Debug_flag)
				{
					printf("低频数据");
					for(j = 0; j < k; j++)
					{
							LFData_Open[j] = Data[i-(k+2)+j];
							printf("0x");
							printf("%02X ",LFData_Open[j]);
					}
					printf("结束");
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
//		    		WaitSendCANLamp(4);	//双闪灯闪烁两下
					DelayXms(100);
			  		NVIC_SystemReset();//复位
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
	CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);	//读取缓存区
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
		if(RxMessage.Data[6]==0x05)    //充电
		{
//			DisablePoweroff=TRUE;    //不去断电
			DisablePoweroffOncharg = TRUE;
		}
		else    //未充
		{
//			DisablePoweroff=FALSE;   //断电
			DisablePoweroffOncharg =FALSE;
		}
		return;
	}
	if(RxMessage.StdId==ConstCanCarLFBACK.StdId)
	{
		if(RxMessage.Data[3]==ConstCanCarLFBACK.Data[3]&&RxMessage.Data[4]==ConstCanCarLFBACK.Data[4]&&
			RxMessage.Data[5]==ConstCanCarLFBACK.Data[5]&&RxMessage.Data[6]==ConstCanCarLFBACK.Data[6])
		{
			CarLFFlagBcak=TRUE;     //收到反馈可以重新发送
			if((RxMessage.Data[7]&0x0F)==0x0F)      //未找到钥匙
			{
				CarFindRemoteFlag=0x00;
			}
			else        //找到
			{
				CarFindRemoteFlag=0x01;
			}
		}
		return;
	}
	if(RxMessage.StdId== CanGetDoorstate.StdId)   //获取车门状态（车门、引擎盖、尾箱）
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

		if(PrvCarDoorOpenFlag!=CarDoorOpenFlag)    //解锁状态，钥匙连续找5分钟休眠状态恢复
		{
				LFPowerOff_doorclose=FALSE;            //恢复找钥匙
				if(DoorOpenLFsleep ==LF_SLEEPED)
				{
					DoorOpenLFsleep = LF_NO_SLEEP;
					WaitSendCANLamp(1);
				}
				if(doordata==0)
				{
						UnfindRemoteCount=0;
						SetTimer(TIMER_LF, _FIND_MID_LF_TIME);   //门关好开始新的延时
				}
				else
				{ 

				}
				carlfcnt=0;
				DoorOpenLfcount=0;
				PrvCarDoorOpenFlag=CarDoorOpenFlag;
				if(BACKLFOPENFLAG==FALSE)
				{
					FindRMLFFlag=TRUE;     //启动后天线查找钥匙
					SetTimer(TIMER_BACKLFFIND, _FIND_BACKRM_LF_TIME);
				}
					Disable_lf_car_flag=FALSE;
			}
		if(OPEN_ANIT==TRUE&&RxMessage.Data[0]==0x82)  //关状态没有开防盗，直接发开防盗指令
		{
			if(Disable_alarm)   //3号键控制开关防盗，默认开
			{
				CarCmdAntiTheft.Data[0] = 0x84;
				CANSendData(CAN1,&CarCmdAntiTheft);   //覆盖原车数据
			}
		}
		return;
	}
}

/*******************************************************************************
* Function Name : CAN1IrqFuc
* Description   : CAN数据接收中断响应函数
* Input         : None
* Output        : None
* Return        : None
*******************************************************************************/
void CAN1IrqFuc(void)
{
	u16 Tmp_button;
	
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);	//读取缓存区
	
	if(RxMessage.StdId==CarGetWindowSKY.StdId)  //获取天窗状态
	{
		if(RxMessage.Data[0]==0x00&&RxMessage.Data[3]==0x00) //关
		{
			IsSKYWinOpen = FALSE;
		}
		else
		{
			IsSKYWinOpen =TRUE ;
		}
		return;
	}
	if(RxMessage.StdId==CarFd.StdId)   //获取防盗状态
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
	if(RxMessage.StdId==CarGetRemote.StdId)   //获取手动操作遥控器动作
	{
		if((RxMessage.Data[2] & 0x04) == 0x04)    //lock
		{

			OPEN_ANIT =FALSE;    //原车钥匙锁车，不需要手动开防盗
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
					if((doordata&0x01)==0x01)  //主驾门打开关闭低频
					{
						CANSendDataConst(CAN2,&ConstCanCmdHorn1);  //默认设置声音大一点     //闪两下
						Disable_lf = TRUE;
						configflag[0]=1;
					}
					else
					{
						CANSendDataConst(CAN2,&ConstCanCmdHorn2);  //默认设置声音大一点     //
						Disable_lf = FALSE;
						configflag[0]=0;
					}
					q_Remoteunlockcnt=0;
					break;
					case 4:
					if((doordata&0x01)==0x01)  //锁车提示声，主架打开关闭
					{
						CANSendDataConst(CAN2,&ConstCanCmdHorn1);     //闪两下
						Disable_win =TRUE;
						configflag[1]=1;
						
					}
					else
					{
						CANSendDataConst(CAN2,&ConstCanCmdHorn2);       //闪一下
						Disable_win = FALSE;
						configflag[1]=0;
					}
					q_Remoteunlockcnt=0;
					break;
					case 5:
					if((doordata&0x01)==0x01)//快捷键4
					{
							Disableglass=TRUE;   //  不关后视镜
							configflag[5]=1;
							CANSendDataConst(CAN2,&ConstCanCmdHorn1);       //闪两下
					}
					else
					{
							Disableglass=FALSE;   //关后视镜
							configflag[5]=0;
							CANSendDataConst(CAN2,&ConstCanCmdHorn0);       //闪一下

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
	if(RxMessage.StdId==CarGetWindowLQ.StdId)//主驾前车窗状态
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
	if(RxMessage.StdId==CarGetWindowRQ.StdId) //副驾前车窗状态
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
	if(RxMessage.StdId==CarGetWindowLH.StdId) //主驾后车窗状态
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
	if(RxMessage.StdId==CarGetWindowRH.StdId)  //副驾后车窗状态
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
	if(RxMessage.StdId==CanGetAccState.StdId)  //车电状态
	{
		if(((RxMessage.Data[2]&0x0F)==0x08)||((RxMessage.Data[2]&0x0F)==0x09))
		{
			if(CarPower==CAR_POWER_ACC)      //熄火
			{
//				 CANSendDataConst(CAN2,&ConstCarCMDOBDSKY1);	//升天窗
//				 CANSendDataConst(CAN2,&ConstCarCMDOBDSKY2);	
//				 SetTimer(TIMER_UPWIN, _UPWIN_TIME);  //主要为升内衬
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
					CANSendDataConst(CAN2,&ConstCanCmdOpenGlass);	//打开后视镜
				}
				KillTimer(TIMER_UPWIN);
				Disable_lf_unlock=FALSE;
				SetTimer(TIMER_RESET, _RESET_WAIT_TIME); //开启、重启5秒定时器系统复位
			}
			CarPower = CAR_POWER_ACC;   //ACC\ON\START
			if(DoorOpenLFsleep ==LF_SLEEPED)
			{
				DoorOpenLFsleep = LF_NO_SLEEP;  //解锁状态，钥匙连续找5分钟休眠状态恢复
				WaitSendCANLamp(1);
			}
			DoorOpenLfcount = 0;
			
		}
		else if((RxMessage.Data[2]&0x0F)==0x06)
		{
//			if(CarPower==CAR_POWER_ACC)      //熄火
//			{
//				 CANSendDataConst(CAN2,&ConstCarCMDOBDSKY1);	//升天窗
//				 CANSendDataConst(CAN2,&ConstCarCMDOBDSKY2);	
//				 SetTimer(TIMER_UPWIN, _UPWIN_TIME);  //主要为升内衬
//			}
			KillTimer(TIMER_RESET); //停止定时器系统复位
			CarPower = CAR_POWER_OFF;
			
		}
		return;
	}
	if(RxMessage.StdId==CarGetCarLockState.StdId)  //获取锁车状态信息
	{
		if(RxMessage.Data[0]==0x88)return; //无操作

		if((RxMessage.Data[0]&0x0F)==0x01)   //主门解锁
			{
				if(LOCKMODEFLAG&&((RxMessage.Data[0]&0xF0)==0x10))//11 四门解锁  21 主驾解锁    33 遥控锁车    
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
			 door = DOOR_CLOSED;       //钥匙被锁车内，尾箱开门
			 Disable_lf_car_flag=FALSE;
			 Disable_lf_flag=FALSE;
			 if(DoorOpenLFsleep ==LF_SLEEPED)
			 {
					DoorOpenLFsleep = LF_NO_SLEEP;
					WaitSendCANLamp(1);
			 }
			 FindRMLFFlag=TRUE;     //启动后天线查找钥匙
			 EventFlag = EVENT_LFSEND;   //马上就触发一次找钥匙
			 ENCarLFFlag =TRUE;
			 SetTimer(TIMER_BACKLFFIND, _FIND_BACKRM_LF_TIME);
		 }
		}
		return;
	}
//	if(RxMessage.StdId == CanGetNumKey.StdId)      //获取数字按键
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
//				if(Tmp_button == 0x0100)//快捷键1   休眠
//				{
//					if(Disable_lf == TRUE)
//					{
//						WaitSendCANLamp(1);       //闪一下
//						Disable_lf = FALSE;
//						configflag[0]=0;
//					}
//					else
//					{
//						WaitSendCANLamp(2);     //闪两下
//						Disable_lf = TRUE;
//						configflag[0]=1;
//					}
//				}
//				else if(Tmp_button == 0x0400)//快捷键2    关闭锁车声
//				{
//					if(Disable_hron == TRUE)
//					{
//						WaitSendCANLamp(1);       //闪一下
//						Disable_hron = FALSE;
//						configflag[1]=0;
//					}
//					else
//					{
//						WaitSendCANLamp(2);     //闪两下
//						Disable_hron =TRUE;
//						configflag[1]=1;
//					}
//				}

//				else if(Tmp_button == 0x1000)//快捷键3    未关门提示的蜂鸣器
//				{
//					if(Disable_alarm==TRUE)
//					{
//						Disable_alarm=FALSE;
//						//config_flag &=~(1<<3);
//						configflag[3]=0;
//						WaitSendCANLamp(1);       //闪两下
//					}
//					else
//					{
//						Disable_alarm=TRUE;
//						//config_flag |=(1<<3);
//						configflag[3]=1;
//						WaitSendCANLamp(2);       //闪一下
//					}
//				}

//				else if(Tmp_button == 0x4000)//快捷键4
//				{
//					if(DisablePoweroff==TRUE)
//					{
//						DisablePoweroff=FALSE;   //去断电
//						//config_flag &=~(1<<5);
//						configflag[5]=0;
//						WaitSendCANLamp(1);       //闪两下
//					}
//					else
//					{
//						DisablePoweroff=TRUE;   //打开未关门报警
//						//config_flag |=(1<<5);
//						configflag[5]=1;
//						WaitSendCANLamp(2);       //闪一下
//					}
//				}

//				else if(Tmp_button == 0x0001)//快捷键5     未关门提示的大喇叭声
//				{
//					if(Disable_doorhron==TRUE)
//					{
//						Disable_doorhron=FALSE;
//						//config_flag &=~(1<<2);
//						configflag[2]=0;
//						WaitSendCANLamp(2);       //闪两下
//					}
//					else
//					{
//						Disable_doorhron=TRUE;
//						//config_flag |=(1<<2);
//						configflag[2]=1;
//						WaitSendCANLamp(1);       //闪一下
//					}
//				}

//				else if(Tmp_button == 0x0004)//快捷键6       //锁车声音大小设置
//				{
//					if(HronSelFlag==TRUE)
//					{
//						HronSelFlag=FALSE;
//						//config_flag &=~(1<<6);
//						configflag[6]=0;
//						WaitSendCANLamp(2);       //闪两下
//					}
//					else
//					{
//						HronSelFlag=TRUE;
//						//config_flag |=(1<<6);
//						configflag[6]=1;
//						WaitSendCANLamp(1);       //闪一下
//					}
//				}

//				else if(Tmp_button == 0x0010)//快捷键7     设置后天线休眠
//				{
//					if(BACKLFOPENFLAG==TRUE)
//					{
//						BACKLFOPENFLAG=FALSE;
//						//config_flag &=~(1<<7);
//						configflag[7]=0;
//						WaitSendCANLamp(2);       //闪两下
//					}
//					else
//					{
//						BACKLFOPENFLAG=TRUE;
//						//config_flag |=(1<<7); 
//						configflag[7]=1;
//						WaitSendCANLamp(1);       //闪一下
//					}
//				}

//				else if(Tmp_button == 0x0040)//快捷键8     设置低频休眠
//				{
//					
//				}
//				//配置保存到flash
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
* Description   : 主板LED闪烁控制
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
      IWDG_ReloadCounter();	//喂狗
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
* Description   : 生产测试函数
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

  LFSendData((u8 *)Lavida_LFData_Open, 5, TRUE);  //发送通用唤醒测场强LF
	ChangeRevMode = FALSE;
  res = UhfRevData_First(Data, 10); //检测是否有高频干扰
  if(res != 12) FlashLedShow(5);
  while(1);
}
void USER_Configuration(void)
{
//	u8 j;

	EventFlag = EVENT_NOP;
	*(u32 *)MCUID = *(vu32 *)(0x1FFFF7F0);

	CANSendData(CAN1,&CanCmdCart);	//唤醒汽车CAN总线
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
	if(RCC_GetFlagStatus(RCC_FLAG_PORRST) == SET) //上电复位,重新学习遥控ID
	{
//		EventFlag = EVENT_STUDY_KEY;
	}
	RCC_ClearFlag();
	vInitRadio(1);  //初始化高频接收芯片，根据LFData_Open[4]选择配置的高频频率
	if(bGoRx()==TRUE)
	{
	 	CanCmdCart.Data[0]=0x02;    //表示高频配置成功
	 	CANSendData(CAN1,&CanCmdCart);	//唤醒汽车CAN总线
	}
	else
	{
		NXP_POWER_OFF; 
		DelayXms(1000); 
		NVIC_SystemReset();
	}
	SetTimer(TIMER_BACKLFFIND, _FIND_BACKRM_LF_TIME);//第一次找钥匙 5分钟
	PowerOffSever();
}


/******************* (C) COPYRIGHT 2011 ECARTEK *****END OF FILE****/
