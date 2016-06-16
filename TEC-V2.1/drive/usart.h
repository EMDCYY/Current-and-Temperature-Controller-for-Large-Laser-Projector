#ifndef __USART_H
#define	__USART_H

#include "stm32f0xx.h"
#include <stdio.h>

void USART_Configuration(void);
int fputc(int ch, FILE *f);
void USART_Send_Byte(uint8_t byte);
void USART_Send(uint8_t *Buffer, uint32_t Length);
uint8_t USART_Recive(void);
void USART_send_receive(void);

#endif /* __USART_H */
