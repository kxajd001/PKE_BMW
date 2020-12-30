/***************************************************************				
File name 	  ：Drv_cmt2300.h
Description     ：
author          ：kxajd
version/date    ：V1.0 2018/3/15
copy            ：ECARTEK
-----------------------------------------------------------------
modify:
date 		 version		 Modifier   	 Update

*****************************************************************/
#ifndef _DRV_CMT2300_H_
#define _DRV_CMT2300_H_

#include "stm32f10x.h"

//*********************************************************
	//System Bank Register
	//*********************************************************
	#define CUS_SYS1				0x0C	//LMT_VTR | MIXER_BIAS | LNA_MODE | LNA_BIAS 
	
	#define CUS_SYS2				0x0D	//About DutyCycle Setting	
		#define LFOSC_RECAL_EN		(1<<7)	//
		#define LFOSC_CAL1_EN		(1<<6)	//
		#define LFOSC_CAL2_EN		(1<<5)	//
		#define RX_TIMER_EN			(1<<4)	//RxTimer Enable
		#define	SLEEP_TIMER_EN		(1<<3)	//SleepTimer Enable
		#define TX_DC_EN			(1<<2)	//Tx DutyCycle Enable
		#define RX_DC_EN			(1<<1)	//Rx DutyCycle Enable
		#define DC_PAUSE			(1<<0)	//DutyCycle Pause
		
	#define CUS_SYS3				0x0E	//XTAL_STB_TIME | TX_EXIT_STATE | RX_EXIT_STATE
		#define	SLEEP_BYPASS_EN		(1<<7)	
		
		#define XTAL_STB_20US		(0<<4)
		#define XTAL_STB_40US		(1<<4)
		#define XTAL_STB_80US		(2<<4)
		#define XTAL_STB_160US		(3<<4)	//[default]
		#define XTAL_STB_320US		(4<<4)
		#define XTAL_STB_640US		(5<<4)
		#define	XTAL_STB_1MS		(6<<4)
		#define XTAL_STB_2MS		(7<<4)
		
		#define TX_EXIT_SLEEP		(0<<2)	//TxDutyCycle Exit to Sleep	
		#define TX_EXIT_STBY		(1<<2)	//TxDutyCycle Exit to Standby
		#define TX_EXIT_FS			(2<<2)	//TxDutyCycle Exit to TFS
		
		#define RX_EXIT_SLEEP		(0<<0)	//RxDutyCycle Exit to Sleep	
		#define RX_EXIT_STBY		(1<<0)	//RxDutyCycle Exit to Standby
		#define RX_EXIT_FS			(2<<0)	//RxDutyCycle Exit to RFS

	#define CUS_SYS4				0x0F	//SLEEP_TIMER_M[7:0]
	#define CUS_SYS5				0x10	//(SLEEP_TIMER_M[10:8]<<4) | SLEEP_TIMER_R[3:0]
	#define CUS_SYS6				0x11	//RX_TIMER_T1_M[7:0]
	#define CUS_SYS7				0x12	//(RX_TIMER_T1_M[10:8]<<4) | RX_TIMER_T1_R[3:0]
	#define CUS_SYS8				0x13	//RX_TIMER_T2_M[7:0]
	#define CUS_SYS9				0x14	//(RX_TIMER_T2_M[10:8]<<4) | RX_TIMER_T2_R[3:0]
	
	#define CUS_SYS10				0x15
		#define COL_DET_EN			(1<<7)
		#define COL_OFS_SEL			(1<<6)
		#define RX_AUTO_EXIT_DIS	(1<<5)	//[Should be set 1]
		#define DOUT_MUTE			(1<<4)	
		
		//14种模式用于SLP
		#define RX_EXT_NONE			(0<<0)	//Rx extend mode disable
		//T1窗口检测：检测到符合条件，退出T1
		#define RX_EXIT_T1_RSSI		(1<<0)	//Rx exit T1 by Rssi
		#define RX_EXIT_T1_PREAM	(2<<0)	//Rx exit T1 by Preamble
		#define RX_EXIT_T1_RP		(3<<0)	//Rx exit T1 by Rssi & Preamble
		#define RX_EXIT_T1_ARSSI	(4<<0)	//Rx exit T1 by Rssi, and hold by Rssi
		//T2延收：检测到符合T1条件，进入T2；T2计时退出，不延长接收；
		#define RX_EXT_T2_RSSI		(5<<0)	//Rx extend to T2 by Rssi
		#define RX_EXT_T2_PREAM		(6<<0)	//Rx extend to T2 by Rssi
		#define RX_EXT_T2_RP		(7<<0)	//Rx extend to T2 by Rssi & Preamble
		#define RX_EXT_T2_PorS		(8<<0)	//Rx extend to T2 by Preamble or Sync
		#define RX_EXT_T2_PorN		(9<<0)	//Rx extend to T2 by Preamble or Node
		#define RX_EXT_T2_PSN		(10<<0)	//Rx extend to T2 by Preamble or Sync or Node
		//T2窗口检测：检测到符合T1条件，进入T2；T2检测到Sync，退出T2
		#define RX_EXIT_T2_RSSI		(11<<0)	//Rx exit T1 by Rssi, and exit T2 by Sync
		#define RX_EXIT_T2_PREAM	(12<<0)	//Rx exit T1 by Preamble, and exit T2 by Sync
		#define RX_EXIT_T2_RP		(13<<0)	//Rx exit T1 by Rssi & Preamble , and exit T2 by Sync
	
	#define CUS_SYS11				0x16
		#define PJD_TH_SEL			(0<<7)	//[Should be set 0]
		
		//CCA INT for RSSI_VLD [for FSK used]
		#define CCA_INT_ONLY_PJD	(0<<5)	//RSSI_VLD==PJD
		#define CCA_INT_ONLY_RSSI	(1<<5)	//RSSI_VLD==RSSI [when used OOK mode, should be set 1]		
		#define CCA_INT_PJD_RSSI	(2<<5)	//RSSI_VLD==PJD+RSSI
		
		#define RSSI_DET_ALWAYS		(0<<3)	//RSSI always detect
		#define RSSI_DET_PREAM		(1<<3)	//RSSI detect when Preamble ok
		#define RSSI_DET_SYNC		(2<<3)	//RSSI detect when Sync ok
		#define RSSI_DET_PKT		(3<<3)	//RSSI detect when packet ok
		
		#define RSSI_AVG_DISABLE	(0<<0)	//RSSI detect average filter step
		#define RSSI_AVG_4Step		(1<<0)
		#define RSSI_AVG_8Step		(2<<0)
		#define RSSI_AVG_16Step		(3<<0)
		#define RSSI_AVG_32Step		(4<<0)

	#define CUS_SYS12				0x17
		//select for PJD 
		#define PJD_WIN_4			(0<<6)	//4 times phase jump
		#define PJD_WIN_6			(1<<6)
		#define PJD_WIN_8			(2<<6)	//[default]
		#define PJD_WIN_10			(3<<6)	
		
		#define CLKOUT_EN			(1<<5)
		
		#define CLKOUT_DIV_64		(31<<0)
		#define CLKOUT_DIV_32		(15<<0)
		#define CLKOUT_DIV_16		(7<<0)
		#define CLKOUT_DIV_8		(3<<0)
		#define CLKOUT_DIV_4		(1<<0)
		#define CLKOUT_DIV_2		(0<<0)

	//*********************************************************
	//Frequency Bank Register
	//*********************************************************
	#define	CUS_RF1					0x18	//Rx Frequency N[7:0]
	#define CUS_RF2					0x19	//Rx Frequency K[7:0]
	#define CUS_RF3					0x1A	//Rx Frequency K[15:8]
	#define CUS_RF4					0x1B	//FREQ_PALDO_SEL | FREQ_DIVX_CODE | FREQ_RX_K[19:16]
	#define CUS_RF5					0x1C	//Tx Frequency N[7:0]
	#define CUS_RF6					0x1D	//Tx Frequency K[7:0]
	#define CUS_RF7					0x1E	//Tx Frequency K[15:8]
	#define CUS_RF8					0x1F	//FSK_SWT | FREQ_VCO_BANK | FREQ_TX_K[19:16]	
	
	//*********************************************************
	//BitRate Bank Register: about BitRate & RxModulation
	//*********************************************************
	#define CUS_RF9					0x20	//BRX64_DIV[7:0]
	#define CUS_RF10				0x21	//BRX64_DIV[15:8]	
	#define CUS_RF11				0x22	//RX_FSK_OOK_SEL | MIXER_BW_SEL | FLT_BW_SEL | BRX64_DIV[19:16]
	#define CUS_RF12 				0x23	
	#define CUS_FSK1				0x24
	#define CUS_FSK2				0x25	
	#define CUS_FSK3				0x26
	#define CUS_FSK4				0x27
	#define CUS_FSK5				0x28
	#define CUS_FSK6				0x29
	#define CUS_FSK7				0x2A
	#define CUS_CDR1				0x2B
		#define CDR_AVG_SEL_SHIFT   4      	//[6:4]
		#define CDR_RANGE_SEL_SHIFT 2       //[3:2]
		#define CDR_MODE_SHIFT      0       //[1:0]
		
		#define CDR_TRACING_MODE    0
		#define CDR_COUNTING_MODE   1
		#define CDR_MANCHESTER_MODE 2
		#define CDR_RAWDATA_MODE    3
		
	#define CUS_CDR2				0x2C
		#define CDR_DET_SEL         (1<<7)	//
		#define CDR_3RD_EN          (1<<6)	//
		#define CDR_4TH_EN			(1<<5)	//
		
	#define CUS_CDR3				0x2D
	#define CUS_CDR4				0x2E
	#define CUS_AGC1				0x2F
	#define	CUS_AGC2				0x30
	#define CUS_AGC3				0x31
	#define	CUS_AGC4				0x32
	#define CUS_OOK1				0x33
	#define CUS_OOK2				0x34
	#define	CUS_OOK3				0x35    //AUTO_ABS_TH (OOK) / RSSI_TRIG_TH (FSK) [7:0]
	#define	CUS_OOK4				0x36
	#define CUS_OOK5				0x37

	//*********************************************************
	//Base Bank Register
	//*********************************************************
	#define CUS_PKT1				0x38
		#define DIRECT_MODE			(0<<0)
		#define BUFFER_MODE			(1<<0)
		#define PACKET_MODE			(2<<0)	//[default]
		
		#define PREAM_UNIT_BYTE		(0<<2)	//Preamble unit 8bit [default]
		#define PREAM_UNIT_NIBBLE	(1<<2)	//Preamble unit 4bit
		
		#define	RX_PREAM_SIZE_SHIFT	3		//[4:0]
		
	#define CUS_PKT2				0x39	//TX_PREAM_SIZE[7:0]
	#define CUS_PKT3				0x3A	//TX_PREAM_SIZE[15:8]
	#define CUS_PKT4				0x3B	//Preamble Value [0xAA or 0x55 is better]
	#define CUS_PKT5				0x3C
		#define SYNC_TOL_NONE		(0<<4)	
		#define SYNC_TOL_0			(0<<4)
		#define SYNC_TOL_1			(1<<4)
		#define SYNC_TOL_2			(2<<4)
		#define SYNC_TOL_3			(3<<4)
		#define SYNC_TOL_4			(4<<4)
		#define SYNC_TOL_5			(5<<4)
		#define SYNC_TOL_6			(6<<4)
		#define SYNC_TOL_7			(7<<4)
		
		#define	SYNC_SIZE_1			(0<<1)
		#define SYNC_SIZE_2			(1<<1)
		#define SYNC_SIZE_3			(2<<1)
		#define SYNC_SIZE_4			(3<<1)
		#define SYNC_SIZE_5			(4<<1)
		#define SYNC_SIZE_6			(5<<1)
		#define SYNC_SIZE_7			(6<<1)
		#define SYNC_SIZE_8			(7<<1)
		
		#define SYNC_MAN_EN			(1<<0)	//enable Sync manchester encode, when manchester code enable
		#define SYNC_MAN_DIS		(0<<0)	
		
	#define CUS_PKT6				0x3D	//SYNC_VALUE[7:0]
	#define CUS_PKT7				0x3E	//SYNC_VALUE[15:8]
	#define CUS_PKT8				0x3F	//SYNC_VALUE[23:16]	
	#define CUS_PKT9				0x40	//SYNC_VALUE[31:24]	
	#define CUS_PKT10				0x41	//SYNC_VALUE[39:32]	
	#define CUS_PKT11				0x42	//SYNC_VALUE[47:40]	
	#define CUS_PKT12				0x43	//SYNC_VALUE[55:48]	
	#define CUS_PKT13				0x44	//SYNC_VALUE[63:56]	

	#define CUS_PKT14				0x45	
		#define PAYLOAD_LENG_SHIFT	4		//PAYLOAD_LENG[11:8]
		
		#define AUTO_ACK_EN			(1<<3)
		#define AUTO_ACK_DIS		(0<<3)
		
		#define NODE_LENG_POS_HEAD	(0<<2)
		#define NODE_LENG_POS_NODE	(1<<2)
		
		#define PAYLOAD_BIT_ORDER_MSB	(0<<1)	//Normal used [default]	
		#define PAYLOAD_BIT_ORDER_LSB	(1<<1)
		
		#define PKT_TYPE_FIXED		(0<<0)	//固定包长
		#define PKT_TYPE_VARIABLE	(1<<0)	//可变包长
		
	#define CUS_PKT15				0x46	//PAYLOAD_LENG[7:0]
		
	#define CUS_PKT16				0x47	
		#define NODE_FREE_EN		(1<<5)
		#define NODE_ERR_MASK		(1<<4)
		
		#define NODE_SIZE_1			(0<<2)
		#define	NODE_SIZE_2			(1<<2)
		#define NODE_SIZE_3			(2<<2)
		#define NODE_SIZE_4			(3<<2)
		
		#define NODE_DET_DISABLE	(0<<0)
		#define NODE_DET_NODE		(1<<0)	//detect NodeID only
		#define NODE_DET_NODE_ZERO	(2<<0)	//detect NodeID or Zero
		#define NODE_DET_ALL		(3<<0)	//detect NodeID or Zero or All'1'
		
	#define CUS_PKT17				0x48	//NODE_VALUE[7:0]
	#define CUS_PKT18				0x49	//NODE_VALUE[15:8]
	#define CUS_PKT19				0x4A	//NODE_VALUE[23:16]	
	#define CUS_PKT20				0x4B	//NODE_VALUE[31:24]

	#define CUS_PKT21				0x4C
		#define FEC_TYPE_A			(0<<7)	//FEC=X^3+X+1
		#define FEC_TYPE_B			(1<<7)	//FEC=X^3+X^2+1
		
		#define FEC_DIS				(0<<6)
		#define FEC_EN				(1<<6)
		
		#define CRC_BYTE_SWAP		(1<<5)	//[15:8]<->[7:0]
		
		#define CRC_BIT_INV			(1<<4)	//CRC result invert
		
		#define CRC_RANGE_ONLY_DATA	(1<<3)	//[0]:Full Payload; [1]:Only Data
		
		#define CRC_TYPE_CCITT		(0<<1)
		#define CRC_TYPE_IBM		(1<<1)
		#define CRC_TYPE_ITU		(2<<1)
		#define CRC_TYPE_NA			(3<<1)
		
		#define CRC_EN				(1<<0)
		#define CRC_DIS				(0<<0)
	
	#define CUS_PKT22				0x4D	//CRC Seed[7:0]
	#define CUS_PKT23				0x4E	//CRC Seed[15:8]
	
	#define CUS_PKT24				0x4F	
		#define CRC_BIT_ORDER		(1<<7)	//[0]:MSB-LSB, Normally; [1]:LSB-MSB
		
		#define WHITEN_SEED_SHIFT	6
		#define WHITEN_SEED_TYPE	(1<<5)	//[0]:as A7139; [1]:by whiten_seed value;  when PN7 active
		
		#define WHITEN_TYPE_CCITT	(0<<3)
		#define WHITEN_TYPE_IBM		(1<<3)
		#define WHITEN_TYPE_PN7		(2<<3)
		
		#define WHITEN_EN			(1<<2)
		
		#define MANCH_TYPE_A		(0<<1)	//Logic0=10; Logic1=01; Normally
		#define MANCH_TYPE_B		(1<<1)	//Logic0=01; Logic1=10;
		
		#define MANCH_EN			(1<<0)
		
	#define CUS_PKT25				0x50	//WHITEN_SEED[7:0]
	
	#define CUS_PKT26				0x51	//
		#define TX_PREFIX_ZERO		(0<<0)
		#define TX_PREFIX_HIGH		(1<<0)
		#define TX_PREFIX_PREAM		(2<<0)	//
	
	#define CUS_PKT27				0x52	//TX_PKT_NUM[7:0]
	#define CUS_PKT28				0x53	//TX_PKT_GAP[7:0]
	#define CUS_PKT29				0x54	//FIFO_TH[6:0]
		#define FIFO_AUTO_RES_EN	(1<<7)	

	//*********************************************************
	//Tx Bank Register
	//*********************************************************
	#define CUS_TX1					0x55	
		#define BT_SEL_3			(0<<6)
		#define BT_SEL_5			(1<<6)
		#define BT_SEL_8			(2<<6)
		#define BT_SEL_10			(3<<6)
		#define GAUS_EN				(1<<5)
		
		#define TX_DIN_FIFO			(0<<2)	//TxData from Fifo
		#define TX_DIN_GPIO			(1<<2)	//TxData from GPIO
	
	#define CUS_TX2					0x56	//TX_DEV[7:0]
	#define CUS_TX3					0x57	//TX_DEV[15:8]	
	#define CUS_TX4					0x58	//RAMP_STEP[11:8] | TX_DEV[19:16]
	#define CUS_TX5					0x59	//RAMP_STEP[7:0]
	#define CUS_TX6					0x5A	//PA_RAMP_RSEL[2:0] | FSK_MOD_DLY[4:0]
	#define CUS_TX7					0x5B	//OOK_MOD_DLY[7:0]
	#define CUS_TX8					0x5C	//POWER_CODE[7:0]	
	#define CUS_TX9					0x5D	//LBD_COMP_OFFSET[2:0] | PCODE_ADJ[4:0]
	#define CUS_TX10				0x5E	//TX_PLL_BW_CODE[1:0] | IDAC_CODE[5:0]
		
		#define TX_PLL_75K			(0<<6)	//<150Kbps
		#define TX_PLL_100K			(1<<6)	//<200Kbps
		#define TX_PLL_150K         (2<<6)	//<300Kbps
		#define TX_PLL_225K         (3<<6)	//<450Kbps
		
	#define CUS_LBD					0x5F	//LBD_TH[7:0]

	//*********************************************************
	//Control Bank1 Register
	//*********************************************************
	#define CUS_MODE_CTL			0x60
		#define	MODE_GO_EEPROM		(1<<0)	//0x01
		#define	MODE_GO_STBY		(1<<1)	//0x02
		#define	MODE_GO_RFS			(1<<2)	//0x04
		#define	MODE_GO_RX			(1<<3)	//0x08
		#define	MODE_GO_SLEEP		(1<<4)	//0x10
		#define	MODE_GO_TFS			(1<<5)	//0x20
		#define	MODE_GO_TX			(1<<6)	//0x40
		#define	MODE_GO_SWITCH		(1<<7)	//0x80			
	
	#define CUS_MODE_STA			0x61
		#define RSTN_IN_EN			(1<<5)	//Enable GPIO4 for Reset Pin 
		#define RSTN_IN_DIS			(0<<5)	
		#define	CFG_RETAIN			(1<<4)	//does not clear when reset
	
		#define	MODE_MASK_STA		0x0F
		
		#define	MODE_STA_IDLE		0x00
		#define	MODE_STA_SLEEP		0x01
		#define	MODE_STA_STBY		0x02
		#define	MODE_STA_RFS		0x03
		#define MODE_STA_TFS		0x04
		#define	MODE_STA_RX			0x05
		#define	MODE_STA_TX			0x06
		#define	MODE_STA_EEPROM		0x07
		#define	MODE_STA_TUNE		0x08			
		
	#define CUS_EN_CTL				0x62
		#define UNLOCK_STOP_EN		(1<<5)	//
		#define LBD_STOP_EN			(1<<4)	//
	
	#define CUS_FREQ_CHNL			0x63	//FHSS_CHANNEL[7:0]
	#define CUS_FREQ_OFS			0x64	//FHSS_OFFSET[7:0]
	
	#define CUS_IO_SEL				0x65
		#define	GPIO1_DATA			(0<<0)	//DOUT/DIN
		#define GPIO1_INT1			(1<<0)	//
		#define	GPIO1_INT2			(2<<0)	//
		#define GPIO1_DCLK			(3<<0)	//
		
		#define	GPIO2_INT1			(0<<2)	//
		#define	GPIO2_INT2			(1<<2)	//
		#define	GPIO2_DATA			(2<<2)	//DOUT/DIN
		#define	GPIO2_DCLK			(3<<2)	//
		
		#define	GPIO3_CLKO			(0<<4)	//CLK output
		#define GPIO3_DATA			(1<<4)	//DOUT/DIN
		#define	GPIO3_INT2			(2<<4)	//
		#define	GPIO3_DCLK			(3<<4)	//
		
		#define	GPIO4_NRST			(0<<6)	//
		#define	GPIO4_INT1			(1<<6)	//
		#define	GPIO4_DOUT			(2<<6)	//Only DOUT
		#define	GPIO4_DCLK			(3<<6)	//	
	
	#define CUS_INT1_CTL			0x66
		#define	RF_SWT1_EN 			(1<<7)	//GPIO1 会输出 RX_ACTIVE，GPIO2 会输出 TX_ACTIVE
		#define RF_SWT2_EN          (1<<6)	//GPIO1 会输出 RX_ACTIVE， GPIO2 会输出 RX_ACTIVE 取反，即完全差分
		#define	INT_POLAR			(1<<5)

	#define CUS_INT2_CTL			0x67
		#define	TX_DIN_INV			(1<<5)	//Tx Data invert

		#define	INT_MASK			0xE0	
		#define	INT_RX_ACTIVE		0x00	
		#define	INT_TX_ACTIVE		0x01
		#define	INT_RSSI_VLD		0x02
		#define	INT_PREAM_OK		0x03
		#define	INT_SYNC_OK			0x04
		#define	INT_NODE_OK			0x05
		#define	INT_CRC_OK			0x06
		#define	INT_PKT_OK			0x07
		#define	INT_SL_TMO			0x08
		#define	INT_RX_TMO			0x09
		#define	INT_TX_DONE			0x0A
		#define	INT_RX_FIFO_NMTY	0x0B
		#define INT_RX_FIFO_TH		0x0C
		#define	INT_RX_FIFO_FULL	0x0D
		#define	INT_RX_FIFO_WBYTE	0x0E
		#define	INT_RX_FIFO_OVF		0x0F
		#define	INT_TX_FIFO_NMTY	0x10
		#define	INT_TX_FIFO_TH		0x11
		#define	INT_TX_FIFO_FULL	0x12
		#define	INT_STATE_IS_STBY	0x13
		#define	INT_STATE_IS_FS		0x14
		#define	INT_STATE_IS_RX		0x15
		#define	INT_STATE_IS_TX		0x16
		#define	INT_LBD				0x17
		#define	INT_TRX_ACTIVE		0x18		
		#define INT_PKT_DONE		0x19	//PKT_OK | COL_ERR | PKT_ERR
	
	#define CUS_INT_EN				0x68
		#define	SL_TMO_EN			(1<<7)	//
		#define	RX_TMO_EN			(1<<6)	//
		#define	TX_DONE_EN			(1<<5)	//
		#define	PREAM_OK_EN			(1<<4)	//
		#define	SYNC_OK_EN			(1<<3)	//
		#define	NODE_OK_EN			(1<<2)	//
		#define	CRC_OK_EN			(1<<1)	//
		#define	PKT_DONE_EN			(1<<0)	//
	
	#define CUS_FIFO_CTL			0x69
		#define	TX_DIN_EN			(1<<7)	//[0]:DOUT, for Rx; [1]:DIN, for Tx 
		
		#define TX_DIN_GPIO1		(0<<5)
		#define TX_DIN_GPIO2  		(1<<5)
		#define TX_DIN_GPIO3		(2<<5)
		#define TX_DIN_HIGH			(3<<5)
				
		#define	FIFO_AUTO_CLR_DIS	(1<<4)	//0: Entry Rx, auto clear fifo
		#define	FIFO_TX_RD_EN		(1<<3)	//0: Tx FIFO can not be read
		#define	FIFO_RX_TX_SEL		(1<<2)	//when "FIFO_SHARE_EN=1" active, 0=use for Rx; 1=use for Tx
		#define	FIFO_MERGE_EN		(1<<1)	//0: not share, both 32Byte for Tx/Rx FIFO; 1: total 64Byte
		#define	SPI_FIFO_RD_WR_SEL	(1<<0)	//0: SPI use to read FIFO;  1: SPI use to write FIFO;
	
	#define CUS_INT_CLR1			0x6A
		#define	SL_TMO_FLG			(1<<5)	//
		#define	RX_TMO_FLG			(1<<4)	//
		#define	TX_DONE_FLG			(1<<3)	//
		#define	TX_DONE_CLR			(1<<2)	//
		#define	SL_TMO_CLR			(1<<1)	//
		#define	RX_TMO_CLR			(1<<0)	//			


	//*********************************************************
	//Control Bank2 Register
	//*********************************************************
	#define CUS_INT_CLR2			0x6B
		#define	LBD_CLR				(1<<5)	//
		#define	PREAM_OK_CLR		(1<<4)	//
		#define	SYNC_OK_CLR			(1<<3)	//
		#define	NODE_OK_CLR			(1<<2)	//
		#define	CRC_OK_CLR			(1<<1)	//
		#define	PKT_DONE_CLR		(1<<0)	//
	
	#define CUS_FIFO_CLR			0x6C
		#define	FIFO_RESTORE		(1<<2)	//
		#define	FIFO_CLR_RX			(1<<1)	//
		#define	FIFO_CLR_TX			(1<<0)	//
	
	#define CUS_INT_FLAG			0x6D	//Read Only
		#define	LBD_FLG				(1<<7)	//
		#define	COL_ERR_FLG			(1<<6)	//
		#define	PKT_ERR_FLG			(1<<5)	//
		#define	PREAM_OK_FLG		(1<<4)	//
		#define	SYNC_OK_FLG			(1<<3)	//
		#define	NODE_OK_FLG			(1<<2)	//
		#define	CRC_OK_FLG			(1<<1)	//
		#define	PKT_OK_FLG			(1<<0)	//
	
	#define CUS_FIFO_FLAG			0x6E	//Read Only
		#define RX_FIFO_FULL_FLG	(1<<6)	//Rx Fifo Full
		#define	RX_FIFO_NMTY_FLG	(1<<5)	//Rx Fifo Not Empty
		#define	RX_FIFO_TH_FLG		(1<<4)	//Rx Fifo Threshold
		#define	RX_FIFO_OVF_FLG		(1<<3)	//Rx Fifo Overflow
		#define	TX_FIFO_FULL_FLG	(1<<2)	//Tx Fifo Full
		#define	TX_FIFO_NMTY_FLG	(1<<1)	//Tx Fifo Not Empty
		#define	TX_FIFO_TH_FLG		(1<<0)	//Tx Fifo Threshold


	#define CUS_RSSI_CODE			0x6F	//RSSI_Code[7:0]  Read Only
	#define CUS_RSSI_DBM			0x70	//RSSI_dBm[7:0]   Read Only
	#define CUS_LBD_RESULT			0x71	//LBD_RESULT[7:0] Read Only	

	#define	CUS_SOFTRST				0x7F	//SoftReset
/*
	#define _GPO1    0x01       // 0          0          0
		
		//PORTD
	#define _GPO2    0x80       // 0          0          0
	#define _GPO3    0x40       // 0          0          0
	#define _GPO4    0x20       // 0          0          0 
		
	#define	GPO1In()	(DDRB &= (~_GPO1))
	#define GPO1_H() 	(PINB&_GPO1)	
	#define GPO1_L()	((PINB&_GPO1)==0)
	
	#define	GPO2In()	(DDRD &= (~_GPO2))
	#define GPO2_H() 	(PIND&_GPO2)	
	#define GPO2_L()	((PIND&_GPO2)==0)
	
	#define	GPO3In()	(DDRD &= (~_GPO3))
	#define GPO3_H() 	(PIND&_GPO3)	
	#define GPO3_L()	((PIND&_GPO3)==0)
	
	#define	GPO4In()	(DDRD &= (~_GPO4))
	#define GPO4_H() 	(PIND&_GPO4)	
	#define GPO4_L()	((PIND&_GPO4)==0)*/
		
typedef enum{
			DC_OFF_MODE,			//Duty-Cycle-Off
			SLEEP_TO_STBY_MODE, 	//SleepTimer wakeup mcu, and to standby
			SLEEP_TO_RX_MODE, 		//SleepTimer wakeup mcu, and to rx
			RX_SEMIAUTO_MODE, 			//Rx->STBY/Sleep->mcu, semi-auto mode by mcu
			RX_FULLAUTO_MODE, 		//full auto
			TX_SEMIAUTO_MODE, 	
		TX_FULLAUTO_MODE
}dutycycelType;
		
typedef enum  {
			 ONLY_RSSI,
			 ONLY_PJD, 
			 RSSI_PJD
 }signaldetType;
		
typedef enum  {
			EXT_DISABLE		= 0,	// 
			EXIT_T1_RSSI 		= 1,
			EXIT_T1_PREAM			= 2, 
			EXIT_T1_RSSInPREAM = 3,	
			EXT_RSSI_AWAYS 	= 4, 
			EXT_T2_RSSI		= 5, 
			EXT_T2_PREAM 	= 6,
			EXT_T2_RSSInPREAM	= 7, 
			EXT_T2_PREAMorSYNC = 8, 
			EXT_T2_PREAMorNode = 9,
			EXT_T2_PoSoN 	=10, 
			EXIT_T2_RSSI 	=11, 
			EXIT_T2_PREAM		=12, 
			EXIT_T2_RSSInPREAM =13 
}rxextendType; 
		
typedef enum  {
			 EXIT_TO_SLEEP, 
			 EXIT_TO_STBY, 
			 EXIT_TO_TFS, 
			 EXIT_TO_RFS, 
}exitstateType;
										 
typedef enum  {
			 JUMP_4 = 0, 
			 JUMP_6 = 1, 
			 JUMP_8 = 2, 
			 JUMP_10= 3
}pjdwinType; 								
		


void vInitRadio(u8 Frequency);
void vInitOOKRadio(u8 Frequency);
bool bGoSleep(void);
bool bGoRx(void);
bool	bGoStandby(void); 
//		};

#endif

/******************* (C) COPYRIGHT 2018 ECARTEK *****END OF FILE****/

