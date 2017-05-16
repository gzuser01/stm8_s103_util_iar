
/******
 * 接 3.3V
 * PD1 -> SCLK , PD2 -> IO , PD3 -> RST
 * 
 */


#include "ds1302.h"
#include <stm8s_gpio.h>
#include <stdio.h>

u8 bcd_to_dec(u8 bcdByte) {
  return (((bcdByte & 0xF0) >> 4) * 10) + (bcdByte & 0x0F);
}

u8 dec_to_bcd(u8 decByte) {
  return (((decByte / 10) << 4) | (decByte % 10));
}

void delay(unsigned int t)
{
  unsigned int c;
  while(t>0)        
  {
    t--;
    c = 100;
    while(c > 0)
    {
      c --;
    }
  }
}
//
void DS1302_Init (void)
{
  GPIO_Init(DS1302_PORT, DS1302_SCK_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(DS1302_PORT, DS1302_IO_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(DS1302_PORT, DS1302_CE_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  //
  DS1302_WriteByte(WrControl,0x00);
  delay(2);
  if(DS1302_ReadByte(RdTrickleCharge) != 0xA6)
  {
    delay(2);
    DS1302_WriteByte(WrTrickleCharge,0xA6);
  }        
  delay(2);
  DS1302_WriteByte(WrControl,0x80); 
}

//
void DS1302_WriteBits(u8 code)
{
  u8 i;
  DS1302_IO_OUT();
  DS1302_CLRSCK();
  for(i=0;i<8;i++)
  {
    delay(2);
    if(code&0x01) (DS1302_SETIO());
    else (DS1302_CLRIO());
    delay(2);
    DS1302_SETSCK();
    delay(2);
    DS1302_CLRSCK();
    code = code >> 1;
  }
}

//
u8 DS1302_ReadBits(void)
{
  u8 i,code;
  DS1302_IO_IN();
  code=0;
  DS1302_CLRSCK();
  delay(2);
  for(i=0;i<8;i++)
  {
    code = code >>1;
    if(DS1302_READ_SDA())
    {
      code = code|0x80;
    }
    delay(2);
    DS1302_SETSCK();
    delay(2);
    DS1302_CLRSCK(); 
    delay(2);
  }
  return code;
}

//
u8 DS1302_ReadByte(u8 con)
{
  u8 code;
  DS1302_CLRCE();
  delay(2);       
  DS1302_CLRSCK();
  delay(2);
  DS1302_SETCE();
  delay(2);
  DS1302_WriteBits(con); 
  code=DS1302_ReadBits(); 
  delay(2);
  DS1302_SETSCK();
  delay(2);
  DS1302_CLRCE(); 
  return code;
}

//
void DS1302_WriteByte(u8 con,u8 code)
{
  DS1302_CLRCE();         
  delay(2);
  DS1302_CLRSCK();
  delay(2);
  DS1302_SETCE();          
  delay(2);
  DS1302_WriteBits(con);     
  DS1302_WriteBits(code); 
  delay(2);
  DS1302_SETSCK();
  delay(2);
  DS1302_CLRCE();          

}

/**
 * 写入时间
 */
void DS1302_WriteTime(TIME_TypeDef* time)
{
  DS1302_WriteByte(WrControl,0x00);
          
  DS1302_WriteByte(WrYear,dec_to_bcd(time->year));
  DS1302_WriteByte(WrMonth,dec_to_bcd(time->month));
  DS1302_WriteByte(WrDate,dec_to_bcd(time->date));
  DS1302_WriteByte(WrWeek,dec_to_bcd(time->week));
  DS1302_WriteByte(WrHour,dec_to_bcd(time->hour));
  DS1302_WriteByte(WrMin,dec_to_bcd(time->min));
  DS1302_WriteByte(WrSec,dec_to_bcd(time->sec));
  
  DS1302_WriteByte(WrControl,0x80);
}

/**
 * 读出时间
 */
void DS1302_ReadTime(TIME_TypeDef* time)
{
  time->sec = bcd_to_dec(DS1302_ReadByte(RdSec));
  time->min = bcd_to_dec(DS1302_ReadByte(RdMin));
  time->hour = bcd_to_dec(DS1302_ReadByte(RdHour));
  
  time->year = bcd_to_dec(DS1302_ReadByte(RdYear));
  time->month = bcd_to_dec(DS1302_ReadByte(RdMonth));        
  time->date = bcd_to_dec(DS1302_ReadByte(RdDate));
  time->week = bcd_to_dec(DS1302_ReadByte(RdWeek));

}