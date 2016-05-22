/**************** (C) COPYRIGHT 2015 LiuXin Academy of Optic-Electronic***********
 * File					:	main.c
 * Description  :	LD Power Driver Control          
 * Platform			:	stm32f051
 * Producer			:	Liu Xin
 * Web					:	www.emdcyy.com
**********************************************************************************/

#include "stm32f0xx.h"
#include <stdio.h>

#include "state.h"
#include "enable.h"

#include "dac.h"
#include "fault.h"

#include "uart.h"
#include "dir.h"
#include "ntc.h"
// #include "ismon.h"
// #include "PWM.h"

extern __IO uint16_t  Tep_Data[10];
uint32_t Tep_Mean;

void delay(int k)
{
	int i, j;
	for(i=0; i<k; i++)
	{
		for(j=0; j<48000; j++);
	}
}

int main(void)
{
// 	int i;
	SystemInit();
	State_Init();
	Enable_Init();
	DAC_exInit();
	Fault_Init();
	USART_Configuration();
	Dir_Init();
<<<<<<< HEAD
//	ADC1_Config1();
//	ADC1_Config2();
//	NTC_ADC1_DMA_Init();
=======
	ADC1_Config1();
	ADC1_Config2();
	NTC_ADC1_DMA_Init();
>>>>>>> 3fa4ac42ffe091db06dc90f3116ae795573dc734
// 	Ismon_ADC1_DMA_Init();
// 	PWM_Init();
	
	delay(100);
	State_Off();
	
	delay(1000);
	State_On();
	DAC_On(); 

	delay(50);
	State_Off();
	
	delay(1);
	State_On();
	Enable_On();
	
	delay(200);

	Dir_Off();
	
  while (1)
  {
		State_Toggle();
		delay(200);
//   	Tep_Mean = 0;
//   	for(i=0; i<10; i++)
//   	{
//   		Tep_Mean = Tep_Mean + Tep_Data[i];
//   	}
// 			Tep_Mean = Tep_Mean / 10;
	
	}
}	





