/* Includes ------------------------------------------------------------------*/
#include "AudiEncoding.h"
#include "UHF_125K.h"
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
const unsigned char dec_num[] = {4,0,3,1,2};
const unsigned char dec_array[] = {0x56,0x4f,0x4c,0x4b,0x53};
//有用的是 0 3 6 9 c 几个数
const unsigned char pic_num[] ={4,0,3 ,0,1,3 ,3,0,4 ,1,3,0 ,2,1,3};
//有用的是 0 3 6 9 c 几个数
const unsigned char pic_array[] = 
{0x56,0x27,0x38 ,0x4f,0xab,0x28 ,0x4c,0x90,0xdf ,0x4b,0x10,0xa3 ,0x53,0x50,0x9a};//{0x56,0x4f,0x4c,0x4b,0x53}

unsigned char dec_data[4][7] = {
  {0x08,0x5C,0x07,0xCB,0xF6,0x18},
  {0xEF,0x22,0x69,0x8A,0x85,0x11},
  {0x42,0xA0,0xE2,0xA4,0x4E,0x14},
  {0x61,0x7F,0x74,0xB5,0x7C,0x19}
};
unsigned char redec_data[4][7] = {
	{0x60,0xBF,0xE1,0x12,0x49,0x18},
	{0x60,0xBF,0xE1,0x12,0x4A,0x11},
	{0x60,0xBF,0xE1,0x12,0x4B,0x14},
	{0x60,0xBF,0xE1,0x12,0x4C,0x19}
};
const unsigned char Key[32] = {1,0,1,0,1,0,1,1,0,1,1,0,0,1,0,1,1,1,1,0,0,0,0,1,1,0,0,1,0,1,1,0};	//发布版KEY
/* Private functions ---------------------------------------------------------*/
void decrypt(unsigned char *pt)
{
  modle_union x, y;
  unsigned char f16, f15, cou;
  unsigned char c4, c0, c1, c2, c3, cc;
  int i;
  f16 = pt[0] ^ pt[1] ^ pt[2] ^ pt[3] ^ pt[4];
  f15 = (f16 >> 4);
  pt[5] = (f16 ^ f15) & 0x0f;
  pt[5] = pt[5] | 0x10;
  c4 = 4;
  c0 = 0;
  c1 = 1;
  c2 = 2;
  c3 = 3;
  for(i = 4; i >= 0; i--)
  {
    f16 = pt[i] - dec_array[i];
    f15 = pt[dec_num[i]] & 0x07;
    x.bytes = f16;
    for(cou = 0; cou < f15; cou++)
    {
      y.bits.d7 = x.bits.d7;
      x.bytes = (x.bytes << 1);
      x.bits.d0 = y.bits.d7;
    }
    f16 = x.bytes;
    pt[c4] = f16^ pt[c0] ^ pt[c1] ^ pt[c2] ^ pt[c3];
    cc = c4;
    c4 = c3;
    c3 = c2;
    c2 = c1;
    c1 = c0;
    c0 = cc;
  }
}
void chella(unsigned char *pt)
{
	modle_union x, y;
	unsigned char f16, f15, cou;
	unsigned char i;
	for(i = 0; i < 5; i++)
	{
		f16 = pt[0] ^ pt[1] ^ pt[2] ^ pt[3] ^ pt[4];
		f15 = pt[pic_num[i * 3]] & 0x07;
		x.bytes = f16;
		for(cou = 0; cou < f15; cou++)
		{
			y.bits.d0 = x.bits.d0;
			x.bytes = x.bytes >> 1;
			x.bits.d7 = y.bits.d0;
		}
		f16 = x.bytes;
		pt[i] = f16 + pic_array[i * 3];
	}
	f16 = pt[0] ^ pt[1] ^ pt[2] ^ pt[3] ^ pt[4];
  f15 = f16 >> 4;
  pt[5] = (f16 ^ f15) & 0x0f;
  pt[5] = pt[5] | 0x10;
}
unsigned char Cal_crc8(unsigned char *ptr, unsigned char len)
{
  unsigned  char i;
  unsigned char crc = 0;
  while(len-- != 0)
  {
    for(i=0x80; i!=0; i/=2)
    {
      if((crc&0x80)!=0)
      {
        crc *= 2;
        crc ^= 0x007;
      }
      else crc *= 2;
      if((*ptr&i) != 0)
        crc ^= 0x07;
    }
    ptr++;
  }
  return(crc);
}
unsigned long ec_ChangUp(unsigned long dwData)
{
  kee_union kee;
  unsigned long tmp;
  kee.word = dwData;
  for(unsigned char i = 0; i < 32; i++){
    tmp = kee.bits.d11 ^ kee.bits.d22 ^ kee.bits.d36 ^ kee.bits.d47 ^ (Key[i]&0x01);
    kee.word <<= 1;
    kee.word |= tmp;
  }
  return kee.word;
}
unsigned long ec_ChangDown(unsigned long dwData)
{
  kee_union kee;
  unsigned long tmp;
  kee.word = dwData;
  for(unsigned char i = 0; i < 32; i++){
    tmp = kee.word & 0x1;
    kee.word >>= 1;
    tmp = kee.bits.d11 ^ kee.bits.d22 ^ kee.bits.d36 ^ tmp ^ (Key[31-i]&0x01);
    kee.word |= (tmp << 31);
  }
  return kee.word;
}