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


  uint16_t Kp = 1;
	uint16_t Ki = 1;
	uint16_t Kd = 1;
	uint16_t ek;
	uint16_t ek_1;
	uint16_t ek_2;

extern __IO uint16_t  ctrlSoftTune;
extern __IO uint8_t rd[19];
extern __IO uint16_t CurrentMaxim;
extern __IO uint16_t CurrentMinum;
extern __IO uint16_t CurrentTuneValue;
extern __IO uint32_t Current_Tune_ADDRESS;
extern __IO uint32_t TECTuneValue;
extern __IO uint32_t TEC_Tune_ADDRESS;
extern __IO uint16_t ADC_Data[45];

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
  TECTuneValue = (*(__IO uint32_t*)(TEC_Tune_ADDRESS));
  delay_ms(10);
  ctrlSoftTune = 1395;
  DAC_Tune();
  delay_ms(10);
  Enable_On();

  if(ADC_Data[0]>=TECTuneValue)//0x0C07
	{
		ctrlSoftTune = 0;
	}
	else
	{
		ek_2	= ADC_Data[1]	-TECTuneValue;
		ek_1	=	ADC_Data[21]-TECTuneValue;
		ek		= ADC_Data[41]-TECTuneValue;
		ctrlSoftTune = (Kp*(ek-ek_1) + Ki*(ek) + Kd*(ek-2*ek_1+ek_2));
	}
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
  Enable_Off();
}

void Tune(void)
{
  WR_EEPROM(CurrentTuneValue,Current_Tune_ADDRESS);
  delay_ms(10);
  if((CurrentTuneValue < CurrentMaxim)&&(CurrentTuneValue > CurrentMinum))
  {
    
    while((CurrentTuneValue > ADC_Data[3]+0x0004) || (CurrentTuneValue < ADC_Data[3]-0x0004))
    {
       if((CurrentTuneValue < ADC_Data[3]) && (ctrlSoftTune > 1395))
       {
         ctrlSoftTune --;
         delay_ms(2);
         DAC_Tune();
       }
       else if((CurrentTuneValue > ADC_Data[3]) && (ctrlSoftTune < 4095))
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

void TEC(void)
{
  
  WR_EEPROM(TECTuneValue,TEC_Tune_ADDRESS);
  delay_ms(10);

  if(ADC_Data[0]>=TECTuneValue)//0x0C07
	{
		ctrlSoftTune = 0;
	}
	else
	{
		ek_2	= ADC_Data[1]	-TECTuneValue;
		ek_1	=	ADC_Data[21]-TECTuneValue;
		ek		= ADC_Data[41]-TECTuneValue;
		ctrlSoftTune = (Kp*(ek-ek_1) + Ki*(ek) + Kd*(ek-2*ek_1+ek_2));
	}
}

	

