
#include "uart1.h"
#include "stm8s.h"

#define UART1_ReceiveBuff_Len 16

/**
 * 串口中断回调函数的指针
 */
void (*_m_uart1_irqhandler_ptr[])(uint8_t *,uint8_t) = {0,0,0,0,0,0,0,0,0,0};

uint8_t UART1_ReceiveBuff_EOF_Len = 1;
uint8_t UART1_ReceiveBuff_EOF[] = {'\n'};

uint8_t UART1_ReceiveBuff_Idx = 0;
uint8_t UART1_ReceiveBuff[UART1_ReceiveBuff_Len];

void Init_UART1(void)
{
  UART1_DeInit();
  UART1_Init((u32)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
	
  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);

  //UART1_Cmd(ENABLE);
}
void UART1_Send(uint8_t dat)
{
  while(( UART1_GetFlagStatus(UART1_FLAG_TXE)==RESET));
  UART1_SendData8(dat);
	
}
/*
void uart1_test(void)
{
  //uint8_t i=0; 
  //CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  Init_UART1();
  enableInterrupts(); 
  while (1)
  {
	
  }
}*/

INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
  uint8_t i;
  uint8_t eof = 1;
  uint8_t len;
  UART1_ClearITPendingBit(UART1_IT_RXNE);
  //UART1_SendData8(UART1_ReceiveData8());
   
  uint8_t dat = UART1_ReceiveData8();
  //加入缓存
  if(UART1_ReceiveBuff_Idx < UART1_ReceiveBuff_Len)
  {
    UART1_ReceiveBuff[UART1_ReceiveBuff_Idx] = dat;
    UART1_ReceiveBuff_Idx ++;
  }
  
  if(UART1_ReceiveBuff_Idx < UART1_ReceiveBuff_EOF_Len)
  {
    return;
  }
  
  //未写满,判断是否有EOF标记
  if(UART1_ReceiveBuff_Idx < UART1_ReceiveBuff_Len)
  {
    for(i =0;i<UART1_ReceiveBuff_EOF_Len;i++)
    {
      if(UART1_ReceiveBuff[UART1_ReceiveBuff_Idx - UART1_ReceiveBuff_EOF_Len + i]
         != UART1_ReceiveBuff_EOF[i])
      {
        eof = 0;
      }
    }
    if(eof == 1)
    {
      len = UART1_ReceiveBuff_Idx;
    }
  }
  else//写满
  {
    len = UART1_ReceiveBuff_Len;
  }
  
  //写满或有结束标记
  if(eof == 1)
  {
    for(i = 0;i<10;i++)
    {
      if(_m_uart1_irqhandler_ptr[i] == 0)
      {
        break;
      }

      (* _m_uart1_irqhandler_ptr[i])(UART1_ReceiveBuff,len);
    }
    UART1_ReceiveBuff_Idx = 0;
  }
   
}




/**
 * 注册中断回调函数
 */
void Register_UART1_Callback(void (* ptr)(uint8_t *,uint8_t))
{
	int i;
	for(i = 0;i<10;i++)
	{
		//已经注册
		if(_m_uart1_irqhandler_ptr[i] == ptr)
		{
			return;
		}
		//序号移动到未设置或未占用
		if(_m_uart1_irqhandler_ptr[i] == 0)
		{
			_m_uart1_irqhandler_ptr[i] = ptr;
			return;
		}
	}
}

/**
 * 通过 Register_UART1_Callback(uart_send_received); 发送收到的字符
 */
void uart_send_received(uint8_t * chars,uint8_t len)
{
  uint8_t i;
  for(i=0;i<len;i++)
  {
    UART1_Send(chars[i]);
  }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif