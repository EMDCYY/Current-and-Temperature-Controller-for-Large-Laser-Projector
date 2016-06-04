/**************** (C) COPYRIGHT 2015 LiuXin Academy of Optic-Electronic***********
 * File			:	enable.c
 * Description  :	Set/reset enable pin to turn on/off driver IC.          
 * Platform		:	stm32f051
 * Producer		:	Liu Xin
 * Web			:	www.emdcyy.com
**********************************************************************************/

#include "enable.h"
#include "state.h"
#include "delay.h"
#include "dac.h"
#include "converter.h"
#include <stdbool.h>

extern __IO uint16_t  ctrlSoftTune;
extern __IO uint8_t rd[19];
extern __IO uint16_t CurrentMaxim;
extern __IO uint16_t CurrentMinum;
extern __IO uint16_t CurrentTuneValue;
extern __IO uint16_t ADC_Data[6];

/**********************************************/
/* Function		:	Initial Enable Pin        		*/
/* Parameter	:	None                      		*/
/**********************************************/
void Enable_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin   = Enable_PIN;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
// 	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_Init(Enable_PORT, &GPIO_InitStruct);
	GPIO_ResetBits(Enable_PORT, Enable_PIN );
}

/**********************************************/
/* Function		:	Enable Pin Off            */
/* Parameter	:	None                      */
/**********************************************/
void Enable_On(void)
{
	 GPIO_SetBits(Enable_PORT, Enable_PIN );
}

/**********************************************/
/* Function		:	Enable Pin On             */
/* Parameter	:	None                      */
/**********************************************/
void Enable_Off(void)
{
	GPIO_ResetBits(Enable_PORT, Enable_PIN );
}

/**********************************************/
/* Function		:	Enable Pin Toggle         */
/* Parameter	:	None                      */
/**********************************************/
void Enable_Toggle(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_8, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_8))));
}


void StartUp(void)
{
	delay_ms(10);
//   DAC_On();
  DAC_On();
	delay_ms(10);
	Enable_On();
}

void ShutDown(void)
{
	int i;
  delay_ms(10);
  DAC_Off();
  for(i = 0; i < 6; i++)
  {
    delay_ms(1000);
    State_Toggle();
  }
	Enable_Off();
}

void Tune(void)
{
  
  if((CurrentTuneValue < CurrentMaxim)&&(CurrentTuneValue > CurrentMinum))
  {
    
    while((CurrentTuneValue > ADC_Data[5]+0x0004) || (CurrentTuneValue < ADC_Data[5]-0x0004))
    {
       if((CurrentTuneValue < ADC_Data[5]) && (ctrlSoftTune > 1395))
       {
         ctrlSoftTune --;
         delay_ms(2);
         DAC_Tune();
       }
       else if((CurrentTuneValue > ADC_Data[5]) && (ctrlSoftTune < 4095))
       {
        ctrlSoftTune ++;
        delay_ms(2);
        DAC_Tune();
       }
    }
  }
}
