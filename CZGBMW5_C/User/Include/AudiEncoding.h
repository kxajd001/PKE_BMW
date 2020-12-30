#ifndef _AUDI_ENCODING_H
#define _AUDI_ENCODING_H

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef struct{
  unsigned d0 : 1;
  unsigned d1 : 1;
  unsigned d2 : 1;
  unsigned d3 : 1;
  unsigned d4 : 1;
  unsigned d5 : 1;
  unsigned d6 : 1;
  unsigned d7 : 1;
}modle_bit;
typedef union{
  modle_bit bits ;
  unsigned char bytes;
}modle_union;

extern unsigned char dec_data[4][7];
extern unsigned char redec_data[4][7];
/* Private function prototypes -----------------------------------------------*/
void decrypt(unsigned char *pt);
void chella(unsigned char *pt);
unsigned char Cal_crc8(unsigned char *ptr, unsigned char len);
unsigned long ec_ChangUp(unsigned long dwData);
unsigned long ec_ChangDown(unsigned long dwData);
/* Exported macro ------------------------------------------------------------*/
#endif
/******************* (C) COPYRIGHT 2011 ECARTEK *****END OF FILE****/