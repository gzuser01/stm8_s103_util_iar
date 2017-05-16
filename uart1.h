
#ifndef __UART1_H
#define __UART1_H

#include "stm8s.h"



void Init_UART1(void);
void UART1_Send(uint8_t dat);
void Register_UART1_Callback(void (* ptr)(uint8_t *,uint8_t));

//”√”⁄≤‚ ‘
void uart_send_received(uint8_t * chars,uint8_t len);
//void uart1_test(void);



#endif