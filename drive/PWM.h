#ifndef __pwm_H
#define	__pwm_H

#include "stm32f0xx.h"
#define PWM_PIN      GPIO_Pin_3
#define PWM_PORT     GPIOB
void PWM_Init(void);

#endif /* __PWM_H */
