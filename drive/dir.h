#ifndef __Dir_H
#define	__Dir_H

#include "stm32f0xx.h"
#define Dir_PIN      GPIO_Pin_11
#define Dir_PORT     GPIOA
void Dir_Init(void);
void Dir_Transmit(void);
void Dir_Receive(void);
#endif /* __Dir_H */
