#include "state.h"
#include "delay.h"
/**********************************************/
/* Function		:	Initial Enable Pin        */
/* Parameter	:	None                      */
/**********************************************/

#define State_PIN      GPIO_Pin_8
#define State_PORT     GPIOA

void State_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin   = State_PIN ;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
 	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_Init(State_PORT, &GPIO_InitStruct);
	GPIO_ResetBits(State_PORT, State_PIN);
}


/**********************************************/
/* Function		:	Enable Pin Off            */
/* Parameter	:	None                      */
/**********************************************/
void delay_state(int k)
{
	int i, j;
  for(i=0;i<k;i++)
	  {
			for(j=0;j<48000;j++);
		}
}		

void State_On(void)
{
	 GPIO_ResetBits(State_PORT, State_PIN );
}

/**********************************************/
/* Function		:	Enable Pin On             */
/* Parameter	:	None                      */
/**********************************************/
void State_Off(void)
{
	GPIO_SetBits(State_PORT, State_PIN );
}

/**********************************************/
/* Function		:	Enable Pin Toggle         */
/* Parameter	:	None                      */
/**********************************************/
void State_Toggle(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_8, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_8))));
}

void State_Short(void)
{
			GPIO_ResetBits(State_PORT, State_PIN );
			delay_ms(200);
			GPIO_SetBits(State_PORT, State_PIN );
			delay_ms(200);
			GPIO_ResetBits(State_PORT, State_PIN );
			delay_ms(200);
			GPIO_SetBits(State_PORT, State_PIN );
			delay_ms(200);
}

void State_Vmode(void)
{
			GPIO_ResetBits(State_PORT, State_PIN );
			delay_ms(200);
			GPIO_SetBits(State_PORT, State_PIN );
			delay_ms(1000);
			GPIO_ResetBits(State_PORT, State_PIN );
			delay_ms(200);
			GPIO_SetBits(State_PORT, State_PIN );
			delay_ms(200);
}


