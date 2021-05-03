#include "PWM.h"
/**********************************************/
/* Function		:	Initial PWM Pin               */
/* Parameter	:	None                          */
/**********************************************/

#define PWM_PIN      GPIO_Pin_3
#define PWM_PORT     GPIOB

void PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Pin   = PWM_PIN ;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
 	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_Init(PWM_PORT, &GPIO_InitStruct);
	GPIO_ResetBits(PWM_PORT, PWM_PIN);
}
