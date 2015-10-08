#ifndef __enable_H
#define	__enable_H

#include "stm32f0xx.h"
#define Enable_PIN      GPIO_Pin_8
#define Enable_PORT     GPIOA
void Enable_Init(void);
void Enable_On(void);
void Enable_Off(void);
void Enable_Toggle(void);
#endif /* __Enable_H */
