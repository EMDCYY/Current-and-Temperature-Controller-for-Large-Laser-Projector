#ifndef __state_H
#define	__state_H

#include "stm32f0xx.h"
#define State_PIN      GPIO_Pin_12
#define State_PORT     GPIOA
void State_Init(void);
void State_On(void);
void State_Off(void);
void State_Toggle(void);
void State_Vmode(void);
void State_Short(void);
#endif /* __Enable_H */
