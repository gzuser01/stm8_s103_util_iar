
#ifndef __DS1302_H
#define __DS1302_H

#include "stm8s.h"

typedef struct
{
        u8 year;
        u8 month;
        u8 date;
        u8 week;
        u8 hour;
        u8 min;
        u8 sec;
}TIME_TypeDef;    

#define   RdSec  0x81
#define   RdMin  0x83
#define   RdHour  0x85
#define   RdDate  0x87
#define   RdMonth  0x89
#define   RdWeek  0x8b
#define   RdYear  0x8d
#define   RdControl          0x8f
#define   RdTrickleCharge 0x91
#define   RdClockBurst  0xbf
#define   WrSec  0x80
#define   WrMin  0x82
#define   WrHour  0x84
#define   WrDate  0x86
#define   WrMonth  0x88
#define   WrWeek  0x8a
#define   WrYear  0x8c
#define   WrControl         0x8e
#define   WrTrickleCharge 0x90
#define   WrClockBurst  0xbe


#define DS1302_PORT GPIOD

#define DS1302_SCK_PIN GPIO_PIN_1 
#define DS1302_IO_PIN GPIO_PIN_2
#define DS1302_CE_PIN GPIO_PIN_3

#define DS1302_CLRSCK() GPIO_WriteLow(DS1302_PORT, DS1302_SCK_PIN)
#define DS1302_SETSCK() GPIO_WriteHigh(DS1302_PORT, DS1302_SCK_PIN)

#define DS1302_CLRIO() GPIO_WriteLow(DS1302_PORT, DS1302_IO_PIN)
#define DS1302_SETIO() GPIO_WriteHigh(DS1302_PORT, DS1302_IO_PIN)

#define DS1302_CLRCE() GPIO_WriteLow(DS1302_PORT, DS1302_CE_PIN)
#define DS1302_SETCE() GPIO_WriteHigh(DS1302_PORT, DS1302_CE_PIN)

#define DS1302_IO_IN()  GPIO_Init(DS1302_PORT, DS1302_IO_PIN, GPIO_MODE_IN_FL_NO_IT)
#define DS1302_IO_OUT() GPIO_Init(DS1302_PORT, DS1302_IO_PIN, GPIO_MODE_OUT_PP_HIGH_FAST) 

#define DS1302_READ_SDA()    (GPIO_ReadInputPin(DS1302_PORT, DS1302_IO_PIN))

    


extern void DS1302_Init(void);

extern u8 DS1302_ReadByte(u8 con);
extern void DS1302_WriteByte(u8 con,u8 code);

extern void DS1302_WriteTime(TIME_TypeDef* time);
extern void DS1302_ReadTime(TIME_TypeDef* time);


#endif