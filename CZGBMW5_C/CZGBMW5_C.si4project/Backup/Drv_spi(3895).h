/***************************************************************				
File name       £ºdrv_spi.h
Description     £º
author          £ºkxajd
version/date    £ºV1.0 2018/3/15
copy            £ºECARTEK
-----------------------------------------------------------------
modify:
date 		 version		 Modifier   	 Update

*****************************************************************/
#ifndef _DRV_SPI_H
#define _DRV_SPI_H 

#include "stm32f10x.h"

#ifndef byte
	typedef u8  byte;
#endif

#ifndef word
	typedef u16  word;
#endif

#define SPI3_SPEED	2 		//the bigger the slow for 2219




#define GPIO_RF_GPIO1           GPIOB
#define GPIO_RF_GPIO1Pin        GPIO_Pin_9
#define GPIO_RF_GPIO3           GPIOB
#define GPIO_RF_GPIO3Pin        GPIO_Pin_5
#define GPIO_RF_FCSB            GPIOB
#define GPIO_RF_FCSBPin         GPIO_Pin_8  //61
#define GPIO_RF_CSB             GPIOC
#define GPIO_RF_CSBPin          GPIO_Pin_0  //8
#define GPIO_RF_SCLK            GPIOC
#define GPIO_RF_SCLKPin         GPIO_Pin_2  //10
#define GPIO_RF_SDIO         		GPIOC
#define GPIO_RF_SDIOPin      		GPIO_Pin_1  //9
/*	
#define RF_PWRDN_HIGH  			    GPIO_SetBits(GPIO_RF_PWRDN, GPIO_RF_PWRDNPin)
#define RF_PWRDN_LOW   			    GPIO_ResetBits(GPIO_RF_PWRDN, GPIO_RF_PWRDNPin)
#define RF_NSEL_HIGH   			    GPIO_SetBits(GPIO_RF_NSEL, GPIO_RF_NSELPin)
#define RF_NSEL_LOW    			    GPIO_ResetBits(GPIO_RF_NSEL, GPIO_RF_NSELPin)
#define RF_MOSI_HIGH    		    GPIO_SetBits(GPIO_RF_MOSI, GPIO_RF_MOSIPin)
#define RF_MOSI_LOW     		    GPIO_ResetBits(GPIO_RF_MOSI, GPIO_RF_MOSIPin)
#define RF_SCLK_HIGH    		    GPIO_SetBits(GPIO_RF_SCLK, GPIO_RF_SCLKPin)
#define RF_SCLK_LOW     		    GPIO_ResetBits(GPIO_RF_SCLK, GPIO_RF_SCLKPin)
#define RF_MISO_IN      		    GPIO_ReadInputDataBit(GPIO_RF_MISO_IN, GPIO_RF_MISO_INPin)
#define RF_RX_IN					GPIO_ReadInputDataBit(GPIO_UHF_RX, GPIO_PIN_UHF_RX)
*/

#define SetCSB()						GPIO_SetBits(GPIO_RF_CSB, GPIO_RF_CSBPin)
#define ClrCSB()						GPIO_ResetBits(GPIO_RF_CSB, GPIO_RF_CSBPin)

#define SetFCSB()						GPIO_SetBits(GPIO_RF_FCSB, GPIO_RF_FCSBPin)
#define ClrFCSB()						GPIO_ResetBits(GPIO_RF_FCSB, GPIO_RF_FCSBPin)

#define SetSDCK()						GPIO_SetBits(GPIO_RF_SCLK, GPIO_RF_SCLKPin)
#define ClrSDCK()						GPIO_ResetBits(GPIO_RF_SCLK, GPIO_RF_SCLKPin)

#define SetSDIO()						GPIO_SetBits(GPIO_RF_SDIO, GPIO_RF_SDIOPin)
#define ClrSDIO()						GPIO_ResetBits(GPIO_RF_SDIO, GPIO_RF_SDIOPin)

#define SDIO_H()						(GPIO_ReadInputDataBit(GPIO_RF_SDIO, GPIO_RF_SDIOPin)&1)
#define SDIO_L()						(GPIO_ReadInputDataBit(GPIO_RF_SDIO, GPIO_RF_SDIOPin)&1==0)

//class spi3Class
//{
 //public:
	void vSpi3Init(void); 			/** initialize software SPI-3 **/ 
	void vSpi3Write(word dat);			/** SPI-3 send one word **/
	void vSpi3WriteCmd(byte dat); 	/*	SPI-3 send one byte to Cmd mode*/
	void vSpi3WriteTreable(byte addr, word dat);	/** SPI-3 send one address, two value, 3byte**/

	byte bSpi3Read(byte addr);			/** SPI-3 read one byte **/
	word wSpi3ReadTreable(byte addr); /** SPI-3 read word mode **/	

	void vSpi3WriteFIFO(byte dat);		/** SPI-3 send one byte to FIFO **/
	byte bSpi3ReadFIFO(void); 		/** SPI-3 read one byte from FIFO **/
	void vSpi3BurstWriteFIFO(byte ptr[], byte length);			/** SPI-3 burst send N byte to FIFO**/
	void vSpi3BurstReadFIFO(byte ptr[], byte length); 		/** SPI-3 burst read N byte to FIFO**/
	void vSpi3BurstWriteCmd(byte addr, byte ptr[], byte length);
	void vSpi3BurstReadCmd(byte addr, byte ptr[], byte length);

//private:
		void vSpi3WriteByte(byte dat, bool sda_in); 	/** SPI-3 send one byte **/
		byte bSpi3ReadByte(void); 		/** SPI-3 read one byte **/
	void vSpi3SendByte(byte dat);
	byte bSpi3RecvByte(void);

//public:
	void vSpi3WriteBytes(byte addr,byte* pInBuf,word len);
	void vSpi3ReadBytes(byte addr,byte* pOutBuf,word len);

//};

void _delay_us(u32 n) ;


#endif

/******************* (C) COPYRIGHT 2018 ECARTEK *****END OF FILE****/

