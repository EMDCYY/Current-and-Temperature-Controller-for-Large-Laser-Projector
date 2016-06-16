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
#include "eeprom_emulation.h"
#include "dir.h"
#include "uart.h"
#include "stdbool.h"

__IO bool  Flag_On = false;

extern __IO uint16_t  ctrlSoftTune;
extern __IO uint8_t rd[19];
extern __IO uint16_t CurrentMaxim;
extern __IO uint16_t CurrentMinum;
extern __IO uint16_t CurrentTuneValue;
extern __IO uint32_t Current_Tune_ADDRESS;
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
    Flag_On = true;
    delay_ms(10); 
    GPIO_SetBits(Enable_PORT, Enable_PIN );
}

/**********************************************/
/* Function		:	Enable Pin On             */
/* Parameter	:	None                      */
/**********************************************/
void Enable_Off(void)
{
	Flag_On = false;
  delay_ms(10); 
  //这个延时非常重要。Flag_On 和 GPIO 转换虽然是串行执行，但是GPIO速度更快，超过Flag_On;
  //致使速度更快的中断监测到中断信号时，认为此时Flag_On是0xFF,机器仍然在开机，发出错误中断。
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

/*SrartUp to maxim current*/
// void StartUp(void)
// {
// 	delay_ms(10);
//   DAC_On();
// 	delay_ms(10);
// 	Enable_On();
// }

void StartUp(void)
{
  CurrentTuneValue = (*(__IO uint32_t*)(Current_Tune_ADDRESS));
  delay_ms(10);
//   if(CurrentTuneValue>CurrentMinum && CurrentTuneValue<CurrentMaxim)
//   {
    ctrlSoftTune = 1395;
    DAC_Tune();
    delay_ms(10);
    Enable_On();
    while(ADC_Data[5]<CurrentTuneValue)
    {
      if(ctrlSoftTune<4095)
      {
        ctrlSoftTune ++;
        delay_ms(2);
        DAC_Tune();
      }
      else
      {
        ctrlSoftTune = 4095;
        break;
      }
    }
//   }
}



/*ShutDown from maxim current*/
// void ShutDown(void)
// {
// 	int i;
//   delay_ms(10);
//   DAC_Off();
//   for(i = 0; i < 6; i++)
//   {
//     delay_ms(1000);
//     State_Toggle();
//   }
// 	Enable_Off();
//   delay_ms(1);
// }

void ShutDown(void)
{
  if(ADC_Data[5]<=CurrentMinum && Flag_On==false)
  {
    //
  }
  else if(ADC_Data[5]<=CurrentMinum  && Flag_On==true)
  {
    Enable_Off();
  }
  else if(ADC_Data[5]>CurrentMinum)
  {
    while(ADC_Data[5]>CurrentMinum)
    {
      if(ctrlSoftTune>1395)
      {
        ctrlSoftTune --;
        delay_ms(2);
        DAC_Tune();
      }
      else
      {
        Enable_Off();
        ctrlSoftTune = 0;
      }
    }
    Enable_Off();
  }
}

void Tune(void)
{
  WR_EEPROM(CurrentTuneValue,Current_Tune_ADDRESS);
  delay_ms(10);
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
       else
       {
          break;
       }
    }
  }
}
