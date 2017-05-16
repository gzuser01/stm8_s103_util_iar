
#include "uart1.h"
#include "ds1302.h"

void Time_Cmd_Processor(uint8_t * chars,uint8_t len)
{
  TIME_TypeDef t;
 
  if(len < 1)
  {
    return;
  }
  //0x73 0x74
  if(chars[0] == 's' && chars[1] == 't')
  {
    
    DS1302_ReadTime(&t);
    UART1_Send(t.year);
    UART1_Send(t.month);
    UART1_Send(t.date);
    UART1_Send(t.hour);
    UART1_Send(t.min);
    UART1_Send(t.sec);
    UART1_Send('\n');
  }
  //0x75 0x74
  if(chars[0] == 'u' && chars[1] == 't')
  {
    if(len < 8)
    {
      return;
    }
  
    t.year = chars[2];
    t.month = chars[3];
    t.date = chars[4];
    t.hour = chars[5];
    t.min = chars[6];
    t.sec = chars[7];
    DS1302_WriteTime(&t);
    UART1_Send('o');
    UART1_Send('k');
    UART1_Send('\n');
  }
}


void main(void)
{
  

  DS1302_Init();
  Register_UART1_Callback(Time_Cmd_Processor);
  Init_UART1();
  enableInterrupts(); 
  
  
  
}
