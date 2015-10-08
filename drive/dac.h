// #ifndef __DAC_H
// #define	__DAC_H

// #include "stm32f0xx.h"

// void DAC_Config(void);

// #endif /* __DAC_H */


#ifndef __dac_H
#define	__dac_H

#include "stm32f0xx.h"
#define DAC_PIN      GPIO_Pin_4
#define DAC_PORT     GPIOA
void DAC_exInit(void);
void DAC_On(void);
void DAC_Off(void);
void DAC_Toggle(void);
#endif /* __DAC_H */
