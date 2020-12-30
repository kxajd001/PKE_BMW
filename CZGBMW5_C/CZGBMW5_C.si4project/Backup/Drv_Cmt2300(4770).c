/***************************************************************				
File name 	  ：Drv_cmt2300.c
Description     ：
author          ：kxajd
version/date    ：V1.0 2018/3/15
copy            ：ECARTEK
-----------------------------------------------------------------
modify:
date 		 version		 Modifier   	 Update

*****************************************************************/
#include "stm32f10x.h"
#include "platform_config.h"

bool	FixedPktLength; 					//false: for contain packet length in Tx message, the same mean with variable lenth
																//true : for doesn't include packet length in Tx message, the same mean with fixed length
byte	PayloadLength;						//unit: byte	range: 1-255																		 
byte	PktRssi;								//with Sign
byte	PktRssiCode;
bool	PktRevError;
bool	RssiTrig;
bool	FhssLockTrig;
byte	FhssLockChannel;
signed char	RssiPeakLock;
byte	FhssPeakChannel;
byte	FhssChannelRange;
byte	FhssChannel;
signed int	FhssRssiAvg;
signed char	FhssRssiTH;

	//***************Add in SLPDriver as below*************************** 				
//********************Duty-Cycle*************************************
word	 SleepTimerM;
byte	 SleepTimerR;
word	 RxTimerT1M;
byte	 RxTimerT1R;
word	 RxTimerT2M;
byte	 RxTimerT2R;
byte	 TxPktNum;
byte	 TxPktGap;

pjdwinType			PJDSel;
dutycycelType 	DutyCycleMode;
signaldetType 	DetectMode;
rxextendType	RxExtendMode;
exitstateType 	RxExitState;				//only for RX_SEMIAUTO_MODE to use
exitstateType 	TxExitState;				//only for TX_SEMIAUTO_MODE to use
byte	 RssiTrigLevel; 											//Range: 1~127
byte	 PreambleRxDetect;							//unit: byte	range: 0-31


unsigned char FreqString[]   = {"Freq: 433.20MHz"};                                
unsigned char BRnDevString[] = {"Dev:10K BR:1.2K"}; 
unsigned char PowerString[]  = {"Pwr: +23dBm     "};

u16 CMTBank_OOK[12] = {            
	0x0000,                      
	0x0166,                      
	0x02EC,                      
	0x031C,                      
	0x0430,                      
	0x0580,                      
	0x0614,                      
	0x0708,                      
	0x0891,                      
	0x0902,                      
	0x0A02,                      
	0x0BD0                       
 };                            
				                       
u16 SystemBank_OOK[12] = {         
	0x0CAE,                      
	0x0DE0,                      
	0x0E35,                      
	0x0F00,                      
	0x1000,                      
	0x11F4,                      
	0x1210,                      
	0x13E2,                      
	0x1442,                      
	0x1520,//1520                
	0x1600,                      
	0x1781                       
};                             
                               
u16 FrequencyBank_OOK_433_20[8] = {
	0x1842,                      
	0x19BB,                      
	0x1A08,                      
	0x1B1B,                      
	0x1C42,                      
	0x1DA5,                      
	0x1E56,                      
	0x1F1A                       
};                             
                               
                               
						                   
u16 DataRateBank_OOK[24] = {       
	 0x204C,                     
	 0x2193,                     
	 0x22B1,                     
	 0x2333,                     
	 0x2400,                     
	 0x2500,                     
	 0x2600,                     
	 0x2700,                     
	 0x2800,                     
	 0x2900,                     
	 0x2A00,                     
	 0x2B29,                     
	 0x2CC0,                     
	 0x2D8A,                     
	 0x2E02,                     
	 0x2F6B,                     
	 0x3017,                     
	 0x3100,                     
	 0x3250,                     
	 0x3315,                     
	 0x3400,                     
	 0x3501,                     
	 0x3605,                     
	 0x3705                      
}; 	                           
								               
u16 BasebandBank_OOK[29] = {       
	0x3812, //3810               
	0x3908,                      
	0x3A00,                      
	0x3BAA,  //3baa              
	0x3C02,                      
	0x3D00,                      
	0x3E00,                      
	0x3F00,                      
	0x4000,                      
	0x4100,                      
	0x4200,                      
	0x43D4,   //43d4             
	0x442D,   //442d             
	0x4500,                      
	0x461F,                      
	0x4700,                      
	0x4800,                      
	0x4900,                      
	0x4A00,                      
	0x4B00,                      
	0x4C00,                      
	0x4D00,                      
	0x4E00,                      
	0x4F60,                      
	0x50FF,                      
	0x5100,                      
	0x5200,                      
	0x531F,                      
	0x5410                       
}; 	                           
								               
u16 TXBank_OOK[11] = {             
	0x5541, //5554              
	0x5626,                     
	0x5703,                     
	0x5800,                     
	0x5900,                     
	0x5A30,                     
	0x5B00,                     
	0x5C37,                     
	0x5D0A,                     
	0x5E3F,                     
	0x5F7F                      
};                             
                               
u16 FrequencyBank_OOK434_65[8] = { 
	0x1842 ,                      
	0x199B ,                      
	0x1A9D ,                      
	0x1B1E ,                      
	0x1C42 ,                      
	0x1D85 ,                      
	0x1EEB ,                      
	0x1F1D                        
};                             
                               


	
u16 CMTBank[12] = {
	0x0002,
	0x0166,
	0x02EC,
	0x031C,
	0x04F0,
	0x0580,
	0x0614,
	0x0708,
	0x0891,
	0x0902,
	0x0A02,
	0x0BD0
 };
				   
u16 SystemBank[12] = {
	0x0CAE,
	0x0DE0,
	0x0E70,
	0x0F00,
	0x1000,
	0x11F4,
	0x1210,
	0x13E2,
	0x1442,
	0x1530,//0x1520
	0x1600,
	0x1781
};

u16 FrequencyBank_433_20[8] = {
	0x1842,
	0x19BB,
	0x1A08,
	0x1B1B,
	0x1C42,
	0x1DA5,
	0x1E56,
	0x1F1A
};
						 
u16 DataRateBank[24] = {
	 0x202A, 
	 0x2183, 
	 0x2211, 
	 0x2310, 
	 0x24A2, 
	 0x2513, 
	 0x260C, 
	 0x2705, 
	 0x288F, 
	 0x295A, 
	 0x2A29, 
	 0x2B28, 
	 0x2CC0, 
	 0x2DA5, 
	 0x2E02, 
	 0x2F53, 
	 0x3018, 
	 0x3100, 
	 0x32B4, 
	 0x3300, 
	 0x3400, 
	 0x3501, 
	 0x3600, 
	 0x3700  
}; 	 
								 
u16 BasebandBank[29] = {
	0x3810, 
	0x3908, 
	0x3A00, 
	0x3BAA,  //3baa
	0x3C02, 
	0x3D00, 
	0x3E00, 
	0x3F00, 
	0x4000, 
	0x4100, 
	0x4200, 
	0x43D4,   //43d4
	0x442D,   //442d
	0x4500, 
	0x461F, 
	0x4700, 
	0x4800, 
	0x4900, 
	0x4A00, 
	0x4B00, 
	0x4C00, 
	0x4D00, 
	0x4E00, 
	0x4F60, 
	0x50FF, 
	0x5101, 
	0x5200, 
	0x531F, 
	0x5410  
}; 	 
								 
u16 TXBank[11] = {
	 0x5554, //5554
	 0x5619, 
	 0x570C, 
	 0x5800, 
	 0x5903, 
	 0x5A50, 
	 0x5B00, 
	 0x5C01, 
	 0x5D01, 
	 0x5E3F, 
	 0x5F7F  
};  
u16 FrequencyBank434_65[8] = {
	0x1842,               
	0x1927,               
	0x1A94,               
	0x1B1E,               
	0x1C42,               
	0x1D11,               
	0x1EE2,               
	0x1F1D                
};  
   
void	vSoftReset(void);
bool	bGoSleep(void); 
bool	bGoStandby(void); 
bool	bGoTx(void);
bool	bGoRx(void);
bool	bGoSwitch(void);
byte	bReadStatus(void);
byte	bReadRssi(bool unit_dbm);
void	vGpioFuncCfg(byte io_cfg);
void	vIntSrcCfg(byte int_1, byte int_2);
void	vInt1SrcCfg(byte int_1);
void	vInt2SrcCfg(byte int_2);				
void	vIntSrcEnable(byte en_int);
byte	bIntSrcFlagClr(void);
void	vEnableAntSwitch(byte mode);			
void	vTxDinEn(byte flag);
void vTxDinSel(byte flag);		
void	vClearFifo(void);
byte	bReadFifoFlag(void);
word	wReadIntFlag(void);
void	vEnableRdFifo(void);
void	vEnableWrFifo(void);
void	vSetPayloadLength(bool mode, byte length);
void	vAckPacket(bool en);
void	vWriteFifo(byte dat);
byte	bReadFifo(void);
void	vInit(void);		
void	vCfgBank(word cfg[], byte length);
void	vAfterCfg(void);
byte	bGetMessage(byte msg[]);
byte	bGetMessageByFlag(byte msg[]);
void	vSendMessage(byte msg[], byte length);
bool	bSendMessageByFlag(byte msg[], byte length);
void	vSetChannelOffset(word interval);
void	vSetChannel(word channel);
void	vSetTxPreamble(word length);
byte	bFHSSDetect(void);		



void	 vSuperLowPowerCfg(void);
void	 vRxDutyCycleOff(void);
void	 vRxDutyCycleOn(void);
void	 vTxDutyCycleOff(void);
void	 vTxDutyCycleOn(void);
void	 vDutyCycleGoOn(void);
void	 vDutyCyclePause(void);
void	 vSetFifoInStby(byte msg[], byte length);




extern void DelayXms(vu32 nTime);
/**********************************************************
State Ctrl
**********************************************************/
/**********************************************************
**Name:     vSoftReset
**Function: Software reset Chipset
**Input:    none
**Output:   none
**********************************************************/
void vSoftReset(void)
{
 vSpi3Write(((word)CUS_SOFTRST<<8)+0xFF); 
 DelayXms(50);				
}

/**********************************************************
**Name:     bGoSleep
**Function: Entry Sleep Mode
**Input:    none
**Output:   none
**********************************************************/
bool bGoSleep(void)
{
 byte tmp;
 
 vSpi3Write(((word)CUS_MODE_CTL<<8)+MODE_GO_SLEEP);	
 _delay_us(100);		
 tmp = (MODE_MASK_STA & bSpi3Read(CUS_MODE_STA));	
 if(tmp==MODE_STA_SLEEP)
 	return(TRUE);
 else
 	return(FALSE);
}

/**********************************************************
**Name:     bGoStandby
**Function: Entry Standby Mode
**Input:    none
**Output:   none
**********************************************************/
bool bGoStandby(void)
{
 byte tmp, i;	
 
 RssiTrig = FALSE;
 PktRevError = FALSE;
 
 vSpi3Write(((word)CUS_MODE_CTL<<8)+MODE_GO_STBY);	
 for(i=0; i<50; i++)
 	{
 	_delay_us(50);	
	tmp = (MODE_MASK_STA & bSpi3Read(CUS_MODE_STA));	
	if(tmp==MODE_STA_STBY)
		break;
	}
 if(i>=50)
 	return(FALSE);
 else
 	return(TRUE);
}


/**********************************************************
**Name:     bGoTx
**Function: Entry Tx Mode
**Input:    none
**Output:   none
**********************************************************/
bool bGoTx(void)
{
 byte tmp, i;
 
 vSpi3Write(((word)CUS_MODE_CTL<<8)+MODE_GO_TFS);	
 for(i=0; i<50; i++)
 	{
 	_delay_us(50);	
	tmp = (MODE_MASK_STA & bSpi3Read(CUS_MODE_STA));	
	if(tmp==MODE_STA_TFS)
		break;
	}
 if(i>=50)
 	return(FALSE);
  
 vSpi3Write(((word)CUS_MODE_CTL<<8)+MODE_GO_TX);		
 for(i=0; i<50; i++)
 	{
 	_delay_us(50);	
	tmp = (MODE_MASK_STA & bSpi3Read(CUS_MODE_STA));	
	if(tmp==MODE_STA_TX)
		break;
	}
 if(i>=50)
 	return(FALSE);
 else
 	return(TRUE);
}

/**********************************************************
**Name:     bGoRx
**Function: Entry Rx Mode
**Input:    none
**Output:   none
**********************************************************/
bool bGoRx(void)
{
 byte tmp, i;

 RssiTrig = FALSE;
 PktRevError = FALSE;
 
 vSetPayloadLength(FALSE, PayloadLength);		//Set traget reveive length
 
 vEnableRdFifo();								//when FIFO Merge Active, set fifo to Rd 

 vSpi3Write(((word)CUS_MODE_CTL<<8)+MODE_GO_RFS);	
 for(i=0; i<50; i++)
 	{
 	_delay_us(50);	
	tmp = (MODE_MASK_STA & bSpi3Read(CUS_MODE_STA));	
	if(tmp==MODE_STA_RFS)
		break;
	}
 if(i>=50)
 	return(FALSE);
  
 vSpi3Write(((word)CUS_MODE_CTL<<8)+MODE_GO_RX);		
 for(i=0; i<50; i++)
 	{
 	_delay_us(50);	
	tmp = (MODE_MASK_STA & bSpi3Read(CUS_MODE_STA));	
	if(tmp==MODE_STA_RX)
		break;
	}
 if(i>=50)
 	return(FALSE);
 else
 	return(TRUE);
}

/**********************************************************
**Name:     bGoSwitch
**Function: Tx to Rx  or  Rx to Tx, use for quick switch 
**Input:    none
**Output:   none
**********************************************************/
bool bGoSwitch(void)
{
 byte tmp, i, z;

 tmp = (MODE_MASK_STA & bSpi3Read(CUS_MODE_STA));	
 if((tmp!=MODE_STA_RX)&&(tmp!=MODE_STA_TX))
 	return(FALSE);
 
 vSpi3Write(((word)CUS_MODE_CTL<<8)+MODE_GO_SWITCH);	
 for(i=0; i<50; i++)
 	{
 	_delay_us(50);		
 	z = (MODE_MASK_STA & bSpi3Read(CUS_MODE_STA));
 	if(tmp==MODE_STA_RX)
 		{
	 	if(z==MODE_STA_TX)
 			break;
 		}
 	else
 		{
 		if(z==MODE_STA_RX)	
 			break;
 		}
 	}
 if(i>=50)
 	return(FALSE);
 else
 	return(TRUE);
}

/**********************************************************
**Name:     bReadStatus
**Function: read chipset status
**Input:    none
**Output:   none
**********************************************************/
byte bReadStatus(void)
{
 return(MODE_MASK_STA & bSpi3Read(CUS_MODE_STA));		
}

/**********************************************************
**Name:     bReadRssi
**Function: Read Rssi
**Input:    true------dBm;
            false-----Code;
**Output:   none
**********************************************************/
byte bReadRssi(bool unit_dbm)
{
 if(unit_dbm)
 	return(bSpi3Read(CUS_RSSI_DBM)-128);		//with sign
 else		
 	return(bSpi3Read(CUS_RSSI_CODE));
}

/**********************************************************
GPIO & Interrupt CFG
**********************************************************/
/**********************************************************
**Name:     vGpioFuncCfg
**Function: GPIO Function config
**Input:    none
**Output:   none
**********************************************************/
void vGpioFuncCfg(byte io_cfg)
{
 vSpi3Write(((word)CUS_IO_SEL<<8)+io_cfg);
}

/**********************************************************
**Name:     vIntSrcCfg
**Function: config interrupt source  
**Input:    int_1, int_2
**Output:   none
**********************************************************/
void vIntSrcCfg(byte int_1, byte int_2)
{
 byte tmp;
 tmp = INT_MASK & bSpi3Read(CUS_INT1_CTL);
 vSpi3Write(((word)CUS_INT1_CTL<<8)+(tmp|int_1));
 
 tmp = INT_MASK & bSpi3Read(CUS_INT2_CTL);
 vSpi3Write(((word)CUS_INT2_CTL<<8)+(tmp|int_2));
}

/**********************************************************
**Name:     vInt1SrcCfg
**Function: config interrupt source 1
**Input:    int_1
**Output:   none
**********************************************************/
void vInt1SrcCfg(byte int_1)
{
 byte tmp;
 tmp = INT_MASK & bSpi3Read(CUS_INT1_CTL);
 vSpi3Write(((word)CUS_INT1_CTL<<8)+(tmp|int_1));
}

/**********************************************************
**Name:     vInt2SrcCfg
**Function: config interrupt source 2 
**Input:    int_2
**Output:   none
**********************************************************/
void vInt2SrcCfg(byte int_2)
{
 byte tmp;
 tmp = INT_MASK & bSpi3Read(CUS_INT2_CTL);
 vSpi3Write(((word)CUS_INT2_CTL<<8)+(tmp|int_2));
}

/**********************************************************
**Name:     vIntSrcEnable
**Function: enable interrupt source 
**Input:    en_int
**Output:   none
**********************************************************/
void vIntSrcEnable(byte en_int)
{
 vSpi3Write(((word)CUS_INT_EN<<8)+en_int);				
}

/**********************************************************
**Name:     vIntSrcFlagClr
**Function: clear flag
**Input:    none
**Output:   equ CMT23_INT_EN
**     .7      .6     .5        .4       .3       .2       .1       .0
**   SL_TMO  RX_TMO  TX_TMO  PREAM_OK  SYNC_OK  NODE_OK  CRC_OK  PKT_DONE
**********************************************************/
byte bIntSrcFlagClr(void)
{
 byte tmp;
 byte int_clr2 = 0;
 byte int_clr1 = 0;
 byte flg = 0;
 
 tmp = bSpi3Read(CUS_INT_FLAG);
 if(tmp&LBD_FLG)				//LBD_FLG_Active
 	int_clr2 |= LBD_CLR;
 
 if(tmp&PREAM_OK_FLG)			//Preamble Active
 	{
 	int_clr2 |= PREAM_OK_CLR;
 	flg |= PREAM_OK_EN;
	}
 if(tmp&SYNC_OK_FLG)			//Sync Active
 	{
 	int_clr2 |= SYNC_OK_CLR;		
 	flg |= SYNC_OK_EN;		
 	}
 if(tmp&NODE_OK_FLG)			//Node Addr Active
 	{
 	int_clr2 |= NODE_OK_CLR;	
 	flg |= NODE_OK_EN;
 	}
 if(tmp&CRC_OK_FLG)				//Crc Pass Active
 	{
 	int_clr2 |= CRC_OK_CLR;
 	flg |= CRC_OK_EN;
 	}
 if(tmp&PKT_OK_FLG)				//Rx Done Active
 	{
 	int_clr2 |= PKT_DONE_CLR;
 	flg |= PKT_DONE_EN;
 	}
 	
 if(tmp&COL_ERR_FLG)			//这两个都必须通过RX_DONE清除
 	int_clr2 |= PKT_DONE_CLR;
 if(tmp&PKT_ERR_FLG)
 	int_clr2 |= PKT_DONE_CLR;
 	
 vSpi3Write(((word)CUS_INT_CLR2<<8)+int_clr2);	//Clear flag
 
 
 tmp = bSpi3Read(CUS_INT_CLR1);
 if(tmp&TX_DONE_FLG)
 	{
 	int_clr1 |= TX_DONE_CLR;
 	flg |= TX_DONE_EN;
 	}	
 if(tmp&SL_TMO_FLG)
 	{
 	int_clr1 |= SL_TMO_CLR;
 	flg |= SL_TMO_EN;
 	} 
 if(tmp&RX_TMO_FLG)
 	{
 	int_clr1 |= RX_TMO_CLR;
 	flg |= RX_TMO_EN;
 	}	
 vSpi3Write(((word)CUS_INT_CLR1<<8)+int_clr1);	//Clear flag 
 
 return(flg);
}

/**********************************************************
**Name:     vEnableAntSwitch
**Function:  
**Input:    
**Output:   none
**********************************************************/
void vEnableAntSwitch(byte mode)
{
 byte tmp;
 tmp = bSpi3Read(CUS_INT1_CTL);
 tmp&= 0x3F;
 switch(mode)
 	{
 	case 1:
 		tmp |= RF_SWT1_EN; break;		//GPO1=RxActive; GPO2=TxActive	
 	case 2:
 		tmp |= RF_SWT2_EN; break;		//GPO1=RxActive; GPO2=!RxActive
 	case 0:
 	default:
 		break;							//Disable	
 	}
 vSpi3Write(((word)CUS_INT1_CTL<<8)+tmp);
}

/**********************************************************
Fifo Packet Handle
**********************************************************/
/**********************************************************
**Name:     vClearFIFO
**Function: clear FIFO buffer
**Input:    none
**Output:   none
**********************************************************/
void vClearFifo(void)
{
 vSpi3Write(((word)CUS_FIFO_CLR<<8)+FIFO_CLR_RX+FIFO_CLR_TX);
}

/**********************************************************
**Name:     bReadFifoFlag
**Function: read fifo state flag
**Input:    none
**Output:   FIFO state
**********************************************************/
byte bReadFifoFlag(void)
{
 return(bSpi3Read(CUS_FIFO_FLAG));
}

/**********************************************************
**Name:     bReadIntFlag
**Function: read interrupt flag
**Input:    none
**Output:   interrupt flag
**********************************************************/
word wReadIntFlag(void)
{
 word tmp;
 tmp = bSpi3Read(CUS_INT_CLR1);
 tmp<<=8;
 tmp|= bSpi3Read(CUS_INT_FLAG);
 return(tmp);	
}

/**********************************************************
**Name:     vEnableRdFifo
**Function: set Fifo for read
**Input:    none
**Output:   none
**********************************************************/
void vEnableRdFifo(void)
{
 byte tmp;
 tmp = bSpi3Read(CUS_FIFO_CTL);
 tmp &= (~(SPI_FIFO_RD_WR_SEL|FIFO_RX_TX_SEL));
 if(PayloadLength>31)
 	tmp |= FIFO_MERGE_EN;
 else
 	tmp &= (~FIFO_MERGE_EN); 
 vSpi3Write(((word)CUS_FIFO_CTL<<8)+tmp);
}

/**********************************************************
**Name:     vEnableWrFifo
**Function: set Fifo for wirte
**Input:    none
**Output:   none
**********************************************************/
void vEnableWrFifo(void)
{
 byte tmp;
 tmp = bSpi3Read(CUS_FIFO_CTL);
 tmp |= (SPI_FIFO_RD_WR_SEL|FIFO_RX_TX_SEL);
 
 if(PayloadLength>31)
 	tmp |= FIFO_MERGE_EN;
 else
 	tmp &= (~FIFO_MERGE_EN); 

 vSpi3Write(((word)CUS_FIFO_CTL<<8)+tmp);
}

/**********************************************************
**Name:     vSetPayloadLength
**Function: set Fifo length to used
**Input:    none
**Output:   none
**********************************************************/
void vSetPayloadLength(bool mode, byte length)
{
 byte tmp;	
 byte len;
 bGoStandby();
 
 tmp = bSpi3Read(CUS_PKT14);
 tmp&= 0x8F;				//length<256
 tmp&= (~0x01);				//Packet Mode
 if(length!=0)
 	{
 	if(mode)                //Tx
 		{
 		if(FixedPktLength)
			len = length-1;
 		else
			len = length;
 		}
 	else					//Rx
		len = length - 1;
 		
 	if(FixedPktLength)
		tmp |= PKT_TYPE_FIXED;
 	else
		tmp |= PKT_TYPE_VARIABLE;
	}
 else
 	return;

 vSpi3Write(((word)CUS_PKT14<<8)+tmp);
 vSpi3Write(((word)CUS_PKT15<<8)+(byte)len);	//Payload length
}

/**********************************************************
**Name:     vAckPacket
**Function: Ack Packet
**Input:    [1] Enable; [0] Disable
**Output:   none
**********************************************************/
void vAckPacket(bool en)
{
 byte tmp;		
 tmp = bSpi3Read(CUS_PKT14);
 if(en)
   	tmp|= AUTO_ACK_EN;
 else
 	tmp&= (~AUTO_ACK_EN);
 vSpi3Write(((word)CUS_PKT14<<8)+tmp);
}

/**********************************************************
**函数名称：bReadFifo
**函数功能：接收长包用的读取Fifo
**输入参数：none
**输出参数：none
**Note：    should be call vEnableRdFifo() at first
**********************************************************/
byte bReadFifo(void)
{
 return(bSpi3ReadFIFO());
}

/**********************************************************
**函数名称：vWriteFifo
**函数功能：接收长包用的读取Fifo
**输入参数：none
**输出参数：none
**Note：    should be call  vEnableWrFifo() at first
**********************************************************/
void vWriteFifo(byte dat)
{
 vSpi3WriteFIFO(dat);
}	


/**********************************************************
CFG
**********************************************************/
/**********************************************************
**Name:     vInit
**Function: Init. CMT2300A
**Input:    none
**Output:   none
**********************************************************/
void vInit(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;

	vSpi3Init();

	GPIO_InitStructure.GPIO_Pin = GPIO_RF_GPIO1Pin|GPIO_RF_GPIO3Pin; //UHF_RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//悬浮输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_RF_GPIO1, &GPIO_InitStructure); 

	vSoftReset();
  bGoStandby();
}

void vCfgBank(word cfg[], byte length)
{
 byte i;
 
 if(length!=0)
 	{	
 	for(i=0; i<length; i++)	
 		vSpi3Write(cfg[i]);
 	}	
}

void vAfterCfg(void)            //call after vCfgBank
{
 byte tmp;	
 tmp = bSpi3Read(CUS_MODE_STA);
 tmp|= CFG_RETAIN;
 tmp&= (~RSTN_IN_EN);						//Disable RstPin	
 vSpi3Write(((word)CUS_MODE_STA<<8)+tmp);

 tmp = bSpi3Read(CUS_SYS2);			//Enable Calibration
 tmp|= (LFOSC_RECAL_EN|LFOSC_CAL1_EN|LFOSC_CAL2_EN);
 vSpi3Write(((word)CUS_SYS2<<8)+tmp);

 bIntSrcFlagClr();
 
 FhssChannelRange = 0;
 FhssChannel      = 0;
 FhssRssiAvg      = 0;
 FhssRssiTH       = 0;
}


/**********************************************************
appliciation
**********************************************************/
/**********************************************************
**函数名称：bGetMessage
**函数功能：接收一包数据
**输入参数：无
**输出参数：非0——接收成功
**            0——接收失败
**备注:     需要与GPO的中断搭配使用，适用于MCU中断场合
**********************************************************/
byte bGetMessage(byte msg[])
{
 byte i;	
 
 if(FixedPktLength)
 	{
  	vSpi3BurstReadFIFO(msg, PayloadLength);
	i = PayloadLength;
	}
 else
 	{
	i = bSpi3ReadFIFO();	
 	vSpi3BurstReadFIFO(msg, i);
 	}
 return(i);
}

/**********************************************************
**函数名称：bGetMessageByFlag
**函数功能：接收一包数据
**输入参数：无
**输出参数：非0——接收成功
**            0——没接收到数据
**           FF——接收到错误
**备注:     查询标识机制，节省GPO中断
**********************************************************/
byte bGetMessageByFlag(byte msg[])
{
 byte tmp;
 byte tmp1;
 byte rev = 0;
 tmp = bSpi3Read(CUS_INT_FLAG);
 if((tmp&SYNC_OK_FLG)&&(!RssiTrig))
 	{
 	PktRssi = bReadRssi(TRUE);
 	PktRssiCode = bReadRssi(FALSE);
 	RssiTrig = TRUE;
 	}
 
 tmp1 = bSpi3Read(CUS_PKT21);

 if(tmp1&CRC_EN)			//Enable CrcCheck
 	{
 	if(tmp&CRC_OK_FLG)
 		{
 		if(FixedPktLength)
 			{
  			vSpi3BurstReadFIFO(msg, PayloadLength);
			rev = PayloadLength;
			}
 		else
 			{	
			rev = bSpi3ReadFIFO();	
 			vSpi3BurstReadFIFO(msg, rev);
 			}
 		RssiTrig = FALSE;
 		PktRevError = FALSE;
 		}
 	}
 else
 	{
	if(tmp&PKT_OK_FLG) 		
		{
 		if(FixedPktLength)
 			{
  			vSpi3BurstReadFIFO(msg, PayloadLength);
			rev = PayloadLength;
			}
 		else
 			{	
			rev = bSpi3ReadFIFO();	
 			vSpi3BurstReadFIFO(msg, rev);
 			}	
 		RssiTrig = FALSE;		
		PktRevError = FALSE;
		}
 	}
 
 if((tmp&COL_ERR_FLG)||(tmp&PKT_ERR_FLG))	//错误处理
	{
	PktRevError = TRUE;	
	rev = 0;
	}
 return(rev);
}

/**********************************************************
**函数名称：vSendMessage
**函数功能：发射一包数据
**输入参数：none
**输出参数：none
**备注    ：需要与GPO的中断搭配使用，适用于MCU中断场合          
**********************************************************/
void vSendMessage(byte msg[], byte length)
{
 //mode1
 //vSetPayloadLength(true, length);
 //bGoStandby();
 //vEnableWrFifo();	
 //vClearFifo();
 //Spi3.vSpi3BurstWriteFIFO(msg, length);
 //bGoTx();
 
 //mode2
 bIntSrcFlagClr();
 vSetPayloadLength(TRUE, length);
 vEnableWrFifo();	
 vClearFifo();
 bGoTx();
 vSpi3BurstWriteFIFO(msg, length);
}

/**********************************************************
**函数名称：bSendMessageByFlag
**函数功能：发射一包数据
**输入参数：none
**输出参数：none
**备注    ：查询标识机制，节省GPO中断         
**********************************************************/
bool bSendMessageByFlag(byte msg[], byte length)
{
 byte tmp;
 word overtime;
 	
 //mode1
 //vSetPayloadLength(true, length);
 //bGoStandby();
 //vEnableWrFifo();	
 //vClearFifo();
 //Spi3.vSpi3BurstWriteFIFO(msg, length);
 //bGoTx();
 
 //mode2
 bIntSrcFlagClr();
 vSetPayloadLength(TRUE, length);
 vEnableWrFifo();	
 vClearFifo();
 bGoTx();
 vSpi3BurstWriteFIFO(msg, length);
 
 overtime = 0;
 do 
 	{
 	_delay_us(100);
	overtime++;
 	tmp = bSpi3Read(CUS_INT_CLR1);
 	}while(((tmp&TX_DONE_FLG)==0)||(overtime>10000));
 if(overtime>=10000)
 	return(FALSE);
 else
 	return(TRUE);
}

/**********************************************************
**函数名称：vSetChannelOffset
**函数功能：设置跳频间隔
**输入参数：interval  unit:KHz
**输出参数：none
**备注    ：
**********************************************************/
void vSetChannelOffset(word interval)
{
 byte offset;
 offset = (interval<<1)/5;			//unit:2.5KHz
 vSpi3Write(((word)CUS_FREQ_OFS<<8)+offset);
}

/**********************************************************
**函数名称：vSetChannel
**函数功能：设置频道
**输入参数：channel
**输出参数：none
**备注    ：
**********************************************************/
void vSetChannel(word channel)
{
 vSpi3Write(((word)CUS_FREQ_CHNL<<8)+channel);
}

/**********************************************************
**函数名称：vSetTxPreamble
**函数功能：设置发射的Preamble长度
**输入参数：length
**输出参数：none
**备注    ：
**********************************************************/
void vSetTxPreamble(word length)
{
 vSpi3Write(((word)CUS_PKT3<<8)+(byte)(length>>8));
 vSpi3Write(((word)CUS_PKT2<<8)+(byte)length);
}

/**********************************************************
**函数名称：bFHSSDetect
**函数功能：Channel detect
**输入参数：range: channel range
            thres: threshold value
**输出参数：0xFF--no detect
            other-channel lock(in channel range)
**备注    ：
**********************************************************/
byte bFHSSDetect(void)
{
 signed char rssi;
 signed int  rssi_cal;
 byte i;
 
 bGoStandby();
 vSetChannel(FhssChannel);
 bGoRx();
 _delay_us(500);		
 rssi_cal = bReadRssi(TRUE);
 for(i=0; i<3; i++)
	{
 	_delay_us(100);		
 	rssi_cal+= bReadRssi(TRUE);
	}
 rssi = rssi_cal>>2;
 bGoStandby();

 
 if(!FhssLockTrig)
 	{
 	if((rssi>FhssRssiAvg)&&((rssi-FhssRssiAvg)>=FhssRssiTH))
 		{
 		FhssLockTrig = TRUE;
		RssiPeakLock    = rssi;
		FhssLockChannel = FhssChannel;
		FhssPeakChannel = FhssChannel;
		}
    else
    	{
    	rssi_cal = FhssRssiAvg+rssi;
        FhssRssiAvg = rssi_cal>>1; 		
    	}
 	FhssChannel++;
 	if(FhssChannel>=FhssChannelRange)	
 		FhssChannel = 0;
 	return(0xFF);
 	}
 else
 	{
	if(FhssChannel==FhssLockChannel)
 		{
 		FhssLockTrig = FALSE;
 		return(FhssPeakChannel);
 		}
 	if(RssiPeakLock<=rssi)
 		{
 		RssiPeakLock    = rssi;
		FhssPeakChannel = FhssChannel;
 		}
 	FhssChannel++;	
 	if(FhssChannel>=FhssChannelRange)	
 		FhssChannel = 0; 	
 	return(0xFF);	
	}
}

/******************************************************************************
Duty-Cycle & Super Low Power
******************************************************************************/
void vSuperLowPowerCfg(void)
{
 byte tmp;
 byte tmp2;
 
 bGoStandby();

 vSpi3Write(((word)CUS_SYS4<<8)+((byte)SleepTimerM));
 vSpi3Write(((word)CUS_SYS5<<8)+((byte)(SleepTimerM>>4)&0x70)+(SleepTimerR&0x0F));
 
 vSpi3Write(((word)CUS_SYS6<<8)+((byte)RxTimerT1M));
 vSpi3Write(((word)CUS_SYS7<<8)+((byte)(RxTimerT1M>>4)&0x70)+(RxTimerT1R&0x0F));

 vSpi3Write(((word)CUS_SYS8<<8)+((byte)RxTimerT2M));
 vSpi3Write(((word)CUS_SYS9<<8)+((byte)(RxTimerT2M>>4)&0x70)+(RxTimerT2R&0x0F)); 
 
 tmp  =bSpi3Read(CUS_SYS2);
 tmp &= 0x70;
 tmp2 = bSpi3Read(CUS_SYS3);
 switch(DutyCycleMode)
 	{

 	case SLEEP_TO_STBY_MODE: 		//GoSleep->Sleep->Standby->McuCtrl
 		tmp |= SLEEP_TIMER_EN;	
 		break;		
 	case SLEEP_TO_RX_MODE:			//GoSleep->Sleep->Rx->McuCtrl
 		tmp |= (SLEEP_TIMER_EN+RX_DC_EN);
 		break;
 	case RX_SEMIAUTO_MODE:
 		tmp |= (SLEEP_TIMER_EN+RX_TIMER_EN);
 		tmp2&= 0xFC; 		
 		switch(RxExitState)
 			{
 		 	case EXIT_TO_STBY:  tmp2 |= RX_EXIT_STBY;  break;
 			case EXIT_TO_RFS:   tmp2 |= RX_EXIT_FS;    break;
 			case EXIT_TO_SLEEP: 
 			default:            tmp2 |= RX_EXIT_SLEEP; break;
 			}
 		break;
 	case RX_FULLAUTO_MODE:
 		tmp |= (SLEEP_TIMER_EN+RX_TIMER_EN+RX_DC_EN);
 		tmp2&= 0xFC; 		
 		tmp2|= RX_EXIT_SLEEP;
 		break;
 	case TX_SEMIAUTO_MODE:
 		tmp |= SLEEP_TIMER_EN;
 		tmp2&= 0xF3; 		
 		switch(TxExitState)
 			{
 			case EXIT_TO_STBY:  tmp2 |= TX_EXIT_STBY;  break;
 			case EXIT_TO_TFS:   tmp2 |= TX_EXIT_FS;    break;
 			case EXIT_TO_SLEEP: 
 			default:            tmp2 |= TX_EXIT_SLEEP; break;
 			}
 		break;	
 	case TX_FULLAUTO_MODE:
 		tmp |= (SLEEP_TIMER_EN+TX_DC_EN);
 		tmp2&= 0xF3; 		
 		tmp2|= TX_EXIT_SLEEP;
 		break;			
 	case DC_OFF_MODE:
 	default:                  
 		break; 						//all disable
 	}
 vSpi3Write(((word)CUS_SYS2<<8)+tmp);
 vSpi3Write(((word)CUS_SYS3<<8)+tmp2);

 if(TX_FULLAUTO_MODE==DutyCycleMode)
    {
    tmp  = bSpi3Read(CUS_PKT29);
 	tmp |= FIFO_AUTO_RES_EN;
 	vSpi3Write(((word)CUS_PKT29<<8)+tmp);
    }

 tmp = bSpi3Read(CUS_SYS10);
 tmp&= 0xF0;
 tmp|= (byte)RxExtendMode;
 vSpi3Write(((word)CUS_SYS10<<8)+tmp);


 tmp = bSpi3Read(CUS_SYS11);
 tmp&= 0x9F;
 switch(DetectMode)
 	{
 	case ONLY_RSSI: tmp |= CCA_INT_ONLY_RSSI; break;
 	case ONLY_PJD:  tmp |= CCA_INT_ONLY_PJD;  break;
 	case RSSI_PJD:
 	default:        tmp |= CCA_INT_PJD_RSSI;  break;
 	}
 vSpi3Write(((word)CUS_SYS11<<8)+tmp); 	
 
 tmp = bSpi3Read(CUS_SYS12);
 tmp&= 0x3F;
 tmp|= (byte)PJDSel;
 vSpi3Write(((word)CUS_SYS12<<8)+tmp);
 
 tmp  = bSpi3Read(CUS_PKT1);
 tmp &= 0x07;
 tmp |= (PreambleRxDetect<<RX_PREAM_SIZE_SHIFT);
 vSpi3Write(((word)CUS_PKT1<<8)+tmp);
 
 if((RssiTrigLevel!=0)&&(RssiTrigLevel<128))
 	{
    tmp = RssiTrigLevel;
    vSpi3Write(((word)CUS_OOK3<<8)+tmp);
    }
 
 if(TxPktNum!=0)
 	{
 	tmp  = bSpi3Read(CUS_PKT29);
 	tmp |= FIFO_AUTO_RES_EN;
 	vSpi3Write(((word)CUS_PKT29<<8)+tmp);
 	
 	vSpi3Write(((word)CUS_PKT27<<8)+TxPktNum);
 	vSpi3Write(((word)CUS_PKT28<<8)+TxPktGap);
 	}
}

void vDutyCyclePause(void)
{
 byte tmp;
 tmp = bSpi3Read(CUS_SYS2);
 tmp|= DC_PAUSE;
 vSpi3Write(((word)CUS_SYS2<<8)+tmp);
}

void vDutyCycleGoOn(void)
{
 byte tmp;
 tmp = bSpi3Read(CUS_SYS2);
 tmp&= (~DC_PAUSE);
 vSpi3Write(((word)CUS_SYS2<<8)+tmp);
}

void vRxDutyCycleOff(void)
{
 byte tmp;
 
 tmp = bSpi3Read(CUS_SYS2);
 tmp&= (~RX_DC_EN);
 vSpi3Write(((word)CUS_SYS2<<8)+tmp);
}

void vRxDutyCycleOn(void)
{
 byte tmp;
 
 tmp = bSpi3Read(CUS_SYS2);
 tmp|= RX_DC_EN;
 vSpi3Write(((word)CUS_SYS2<<8)+tmp);
}

void vTxDutyCycleOff(void)
{
 byte tmp;
 
 tmp = bSpi3Read(CUS_SYS2);
 tmp&= (~TX_DC_EN);
 vSpi3Write(((word)CUS_SYS2<<8)+tmp);
}

void vTxDutyCycleOn(void)
{
 byte tmp;
 
 tmp = bSpi3Read(CUS_SYS2);
 tmp|= TX_DC_EN;
 vSpi3Write(((word)CUS_SYS2<<8)+tmp);
}

void vSetFifoInStby(byte msg[], byte length)
{
 bGoStandby();
 vSetPayloadLength(TRUE, length);
 vEnableWrFifo();	
 vSpi3BurstWriteFIFO(msg, length);
}
 
void vTxDinEn(byte flag)
{
 byte tmp;
 
		if(flag == 1)
		{
		  tmp = bSpi3Read(CUS_FIFO_CTL);
		  tmp |= (TX_DIN_EN);
		  vSpi3Write(((word)CUS_FIFO_CTL<<8)+tmp);
		}
		else
		{
		  tmp = bSpi3Read(CUS_FIFO_CTL);
		  tmp &= (~TX_DIN_EN);
		  vSpi3Write(((word)CUS_FIFO_CTL<<8)+tmp);
		}
}

void vTxDinSel(byte flag)
{
	 byte tmp;
 
		if(flag == 3)
		{
		  tmp = bSpi3Read(CUS_FIFO_CTL);
		  tmp |= (TX_DIN_GPIO3);
		  vSpi3Write(((word)CUS_FIFO_CTL<<8)+tmp);
		}
		else if(flag == 2)
		{
		  tmp = bSpi3Read(CUS_FIFO_CTL);
		  tmp |= (TX_DIN_GPIO2);
		  vSpi3Write(((word)CUS_FIFO_CTL<<8)+tmp);
		}
		else 
		{
		  tmp = bSpi3Read(CUS_FIFO_CTL);
		  tmp |= (TX_DIN_GPIO1);
		  vSpi3Write(((word)CUS_FIFO_CTL<<8)+tmp);
		}
}
//bool CMT2300init = FALSE;
extern u8 LFData_Open[27];
void vInitRadio(u8 Frequency)					
{

 	CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);//禁止指定的CAN中断
 	CAN_ITConfig(CAN2, CAN_IT_FMP0, DISABLE);//禁止指定的CAN中断
	vInit();

	vCfgBank(CMTBank, 12);
	vCfgBank(SystemBank, 12);
	if(Frequency)//配置为434.65M
	{
		vCfgBank(FrequencyBank434_65, 8);
	}
	else      //配置为433.92M
	{
		vCfgBank(FrequencyBank_433_20, 8);
	}
	vCfgBank(DataRateBank, 24);
	vCfgBank(BasebandBank, 29);
	vCfgBank(TXBank, 11);
	vAfterCfg();    

	bGoSleep();    //配置生效
	vGpioFuncCfg(GPIO3_DCLK);  //
	bGoRx();
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);//打开指定的CAN中断
	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);//打开指定的CAN中断
}

void vInitOOKRadio(u8 Frequency)					
{
	
 vInit();
 
 vCfgBank(CMTBank_OOK, 12);
 vCfgBank(SystemBank_OOK, 12);
 if(Frequency)//配置为434.65M
 {
		vCfgBank(FrequencyBank_OOK434_65, 8);
 }
 /*else if(LFData_Open[4]==0x39)
 {
 		vCfgBank(FrequencyBank436_1, 8);
 }*/
 else //if(LFData_Open[4]==0x35)//配置为433.20M
 {
 	vCfgBank(FrequencyBank_OOK_433_20, 8);
 }
 /*else 
 {
 	vCfgBank(FrequencyBank_315, 8);
 }*/
 vCfgBank(DataRateBank_OOK, 24);
 vCfgBank(BasebandBank_OOK, 29);
 vCfgBank(TXBank_OOK, 11);
 vAfterCfg();    
 
 //bGoSleep();    //配置生效
 vGpioFuncCfg(GPIO3_DATA);  //
 //bGoRx();

}


/******************* (C) COPYRIGHT 2018 ECARTEK *****END OF FILE****/


