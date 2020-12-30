/***************************************************************				
File name 	  ：Drv_CAN.c
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

void _delay_us(u32 n) 
{
	u32 i;
	for(;n != 0; n--)
		for(i = 0; i < 4; i++);
}


void GPIO_SDIO_Configuration(u8 inoutput)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_RF_SDIOPin;      //RFSDIO
	if(inoutput==0)
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	      //上拉输入
	}
	else
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	      //输出模式
	}
	GPIO_Init(GPIO_RF_SDIO, &GPIO_InitStructure);
}

#define InputSDIO()				 GPIO_SDIO_Configuration(0)
#define OutputSDIO()			 GPIO_SDIO_Configuration(1)




/**********************************************************
**Name: 	vSpi3Init
**Func: 	Init Spi-3 Config
**Note: 	
**********************************************************/
void vSpi3Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_RF_CSBPin|GPIO_RF_SCLKPin|GPIO_RF_SDIOPin;  //SPI-3 for output
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_Init(GPIO_RF_CSB, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_RF_FCSBPin;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_Init(GPIO_RF_FCSB, &GPIO_InitStructure); 
	
	ClrSDCK();
	//SetSDCK();
}

/**********************************************************
**Name: 	vSpi3WriteByte
**Func: 	SPI-3 send one byte
**Input:
**Output:  
**********************************************************/
void vSpi3WriteByte(byte dat, bool sda_in)
{

 	byte bitcnt;	
 
	SetFCSB();				//FCSB = 1;
 
 	OutputSDIO();			//SDA output mode
 	SetSDIO();				//    output 1
 
 	ClrSDCK();				
 	ClrCSB();
 _delay_us(6);
 	for(bitcnt=8; bitcnt!=0; bitcnt--)
 		{
		ClrSDCK();	
		if(dat&0x80)
 			SetSDIO();
 		else
 			ClrSDIO();
		
		#if (SPI3_SPEED!=0)
		    _delay_us(SPI3_SPEED);
		#endif
		
		SetSDCK();
 		dat <<= 1; 		

		#if (SPI3_SPEED!=0)
		    _delay_us(SPI3_SPEED);
		#endif
 		}
 	if(sda_in)
 		{
 		InputSDIO();		//shift to input for read
 		ClrSDCK();
 		}	
 	else
 		{
 		ClrSDCK();	
 		SetSDIO();
 		}
}

/**********************************************************
**Name: 	bSpi3ReadByte
**Func: 	SPI-3 read one byte
**Input:
**Output:  
**********************************************************/
byte bSpi3ReadByte(void)
{
	byte RdPara = 0;
 	byte bitcnt;
 
  ClrSDCK();
 	InputSDIO();			
  ClrCSB(); 
   	
 	_delay_us(SPI3_SPEED);		//Time-Critical
 	_delay_us(SPI3_SPEED);		//Time-Critical   	
   	
 	for(bitcnt=8; bitcnt!=0; bitcnt--)
 		{
 		SetSDCK();
 		RdPara <<= 1;
 		
		#if (SPI3_SPEED!=0)
		    _delay_us(SPI3_SPEED);
		#endif

 		if(SDIO_H())
 			RdPara |= 0x01;
 		else
 			RdPara |= 0x00;
 		ClrSDCK();

		#if (SPI3_SPEED!=0)
		    _delay_us(SPI3_SPEED);
		#endif
 		} 
 	ClrSDCK();
 	//OutputSDIO();
 	//SetSDIO();
 	//SetCSB();			
 	return(RdPara);	
}

/**********************************************************
**Name:	 	vSpi3Write
**Func: 	SPI Write One word
**Input: 	Write word
**Output:	none
**********************************************************/
void vSpi3Write(word dat)
{
 	vSpi3WriteByte(((byte)(dat>>8)&0x7F), FALSE);
 	vSpi3WriteByte(((byte)dat), FALSE);
	_delay_us(6);
 	SetCSB();
}

void vSpi3WriteCmd(byte dat)
{
 	vSpi3WriteByte((dat&0x7F), FALSE);
 	SetCSB();
}

void vSpi3WriteTreable(byte addr, word dat)
{

 	vSpi3WriteByte((addr&0x7F), FALSE);
 	vSpi3WriteByte(((byte)(dat>>8)), FALSE);
 	vSpi3WriteByte(((byte)dat), FALSE);
 	SetCSB();
}

void vSpi3BurstWriteCmd(byte addr, byte ptr[], byte length)
{
  byte i;
	
 	vSpi3WriteByte((addr&0x7F), FALSE);
 	for(i=0; i<length; i++)
 		vSpi3WriteByte(ptr[i], FALSE);
	SetCSB();
}



/**********************************************************
**Name:	 	bSpi3Read
**Func: 	SPI-3 Read One byte
**Input: 	readout addresss
**Output:	readout byte
**********************************************************/
byte bSpi3Read(byte addr)
{
	byte dat;
	
	vSpi3WriteByte((addr|0x80), TRUE);
	dat = bSpi3ReadByte();
	OutputSDIO();
 	SetSDIO();
 	SetCSB();			

	return(dat);
}

word wSpi3ReadTreable(byte addr)
{
	word dat;
	
  vSpi3WriteByte((addr|0x80), TRUE);
 	dat   = bSpi3ReadByte();
 	dat <<= 8;
 	dat  |= bSpi3ReadByte();
	OutputSDIO();
 	SetSDIO();
 	SetCSB();			
 	return(dat);
}

void vSpi3BurstReadCmd(byte addr, byte ptr[], byte length)
{
	byte i;

  vSpi3WriteByte((addr|0x80), TRUE);
 	for(i=0; i<length; i++)
 		ptr[i] = bSpi3ReadByte();
	OutputSDIO();
 	SetSDIO();
 	SetCSB();			
}


/**********************************************************
**Name:	 	vSpi3WriteFIFO
**Func: 	SPI-3 send one byte to FIFO
**Input: 	one byte buffer
**Output:	none
**********************************************************/
void vSpi3WriteFIFO(byte dat)
{
 	byte bitcnt;	
 
 	SetCSB();	
	OutputSDIO();	
	ClrSDCK();
 	ClrFCSB();			//FCSB = 0
	for(bitcnt=8; bitcnt!=0; bitcnt--)
 		{
 		ClrSDCK();
 		
 		if(dat&0x80)
			SetSDIO();		
		else
			ClrSDIO();

		#if (SPI3_SPEED!=0)
		    _delay_us(SPI3_SPEED);
		#endif
			
		SetSDCK();

		#if (SPI3_SPEED!=0)
		    _delay_us(SPI3_SPEED);
		#endif

 		dat <<= 1;
 		}
 	ClrSDCK();	
 	_delay_us(SPI3_SPEED);		//Time-Critical
 	_delay_us(SPI3_SPEED);		//Time-Critical
 	SetFCSB();
	SetSDIO();
 	_delay_us(SPI3_SPEED);		//Time-Critical
 	_delay_us(SPI3_SPEED);		//Time-Critical
}

/**********************************************************
**Name:	 	bSpi3ReadFIFO
**Func: 	SPI-3 read one byte to FIFO
**Input: 	none
**Output:	one byte buffer
**********************************************************/
byte bSpi3ReadFIFO(void)
{
	byte RdPara;
 	byte bitcnt;	

 	SetCSB();
	InputSDIO();
 	ClrSDCK();
	ClrFCSB();
	 	
 	_delay_us(SPI3_SPEED);		//Time-Critical
 	_delay_us(SPI3_SPEED);		//Time-Critical
		
 	for(bitcnt=8; bitcnt!=0; bitcnt--)
 		{

		SetSDCK();

		#if (SPI3_SPEED!=0)
		    _delay_us(SPI3_SPEED);
		#endif

 		RdPara <<= 1;
 		if(SDIO_H())
 			RdPara |= 0x01;		//NRZ MSB
 		else
 		 	RdPara |= 0x00;		//NRZ MSB
 		ClrSDCK();

		#if (SPI3_SPEED!=0)
		    _delay_us(SPI3_SPEED);
		#endif

 		}
 	
 	ClrSDCK();
 	SetFCSB();
	OutputSDIO();
	SetSDIO();
 	return(RdPara);
}

/**********************************************************
**Name:	 	vSpi3BurstWriteFIFO
**Func: 	burst wirte N byte to FIFO
**Input: 	array length & head pointer
**Output:	none
**********************************************************/
void vSpi3BurstWriteFIFO(byte ptr[], byte length)
{
 	byte i;
 	if(length!=0x00)
	 	{
 		for(i=0;i<length;i++)
 			vSpi3WriteFIFO(ptr[i]);
 		}
 	return;
}

/**********************************************************
**Name:	 	vSpiBurstRead
**Func: 	burst wirte N byte to FIFO
**Input: 	array length  & head pointer
**Output:	none
**********************************************************/
void vSpi3BurstReadFIFO(byte ptr[], byte length)
{
	byte i;
 	if(length!=0)
 		{
 		for(i=0;i<length;i++)
 			ptr[i] = bSpi3ReadFIFO();
 		}	
 	return;
}

void vSpi3SendByte(byte dat)
{
	byte bitcnt;	

 	for(bitcnt=0; bitcnt<8; bitcnt++)
	{
		ClrSDCK();	
		if(dat&0x80)
			SetSDIO();
		else
			ClrSDIO();
		
		_delay_us(SPI3_SPEED);
		dat <<= 1; 	
		SetSDCK();				
		_delay_us(SPI3_SPEED);
	}
}

byte bSpi3RecvByte(void)
{
	byte RdPara = 0;
 	byte bitcnt;
   	
 	for(bitcnt=0; bitcnt<8; bitcnt++)
 	{
 		
 		RdPara <<= 1;
		ClrSDCK();		
 		_delay_us(SPI3_SPEED);
		SetSDCK();
 		if(SDIO_H())
 			RdPara |= 0x01;
 		else
 			RdPara |= 0x00;
 		
 		_delay_us(SPI3_SPEED);
 	} 
 		
 	return(RdPara);	
}

void vSpi3WriteBytes(byte addr,byte* pInBuf,word len)
{
	word i;

	OutputSDIO();
	SetSDIO();				//    output 1
 
 	ClrSDCK();				
 	ClrCSB();
	_delay_us(SPI3_SPEED);
	_delay_us(SPI3_SPEED);

	vSpi3SendByte(addr|0x00);

	for(i=0;i<len;i++)
	{
		vSpi3SendByte(pInBuf[i]);
	}

	ClrSDCK();
	_delay_us(SPI3_SPEED);
	_delay_us(SPI3_SPEED);
	SetCSB();
	SetSDIO();
	InputSDIO();
}

void vSpi3ReadBytes(byte addr,byte* pOutBuf,word len)
{
	word i;

	OutputSDIO();
	SetSDIO();				//    output 1
 
 	ClrSDCK();				
 	ClrCSB();
	_delay_us(SPI3_SPEED);
	_delay_us(SPI3_SPEED);

	vSpi3SendByte(addr|0x80);

	SetSDIO();
	InputSDIO();

	for(i=0;i<len;i++)
	{
		pOutBuf[i]=bSpi3RecvByte();
	}

	ClrSDCK();
	_delay_us(SPI3_SPEED);
	_delay_us(SPI3_SPEED);
	SetCSB();
	SetSDIO();
	InputSDIO();
}


/******************* (C) COPYRIGHT 2018 ECARTEK *****END OF FILE****/
