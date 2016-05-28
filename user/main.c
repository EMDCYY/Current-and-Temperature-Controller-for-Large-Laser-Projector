/*****(C) COPYRIGHT 2016 LiuXin Academy of Optic-Electronic******
 * File					:	main.c
 * Description  :	LD Power Driver Control          
 * Platform			:	stm32f051
 * Producer			:	Liu Xin
 * Web					:	www.emdcyy.com
*****************************************************************/

#include "stm32f0xx.h"
#include <stdio.h>



/* Basic State Setup--------------------------------------------*/
#include "state.h"
#include "enable.h"
#include "delay.h"

/* Fault Report Intial------------------------------------------*/
// #include "fault.h"

/* UART RS485 Header--------------------------------------------*/
#include "dir.h"	//The Direction of RS485 -- Recive or Transmit
#include "uart.h"	//RS485 Configuration

/* DAC Header---------------------------------------------------*/
#include "dac.h"

/* ADC for NTC or ISMON or IVINMON Header-----------------------*/
// #include "ntc.h"

/* Protocol Header----------------------------------------------*/
#include "protocol.h"

/* Other Function-----------------------------------------------*/
// #include "ismon.h"
// #include "PWM.h"



int main(void)
{
  SystemInit();
	
/* Basic State Setup----------------------------------------------------*/
	Enable_Init();	//Enable LT3763
	State_Init(); 	//LED Show

/* Fault Report Intial--------------------------------------------------*/
// 	Fault_Init();
	
/* PWM Intial-----------------------------------------------------------*/
//   PWM_Init();
  
/* DAC output for Ctrl Pin input of LT3763 for soft start---------------*/	
// 	DAC_exInit();
	
/* UART RS485 Initial---------------------------------------------------*/
	Dir_Init();							//The Direction of RS485 -- Recive or Transmit
	delay_ms(10);
	UART_Configuration();	  //RS485 Configuration
	delay_ms(10);
 	Dir_Receive();

//   delay_ms(10);
// 	State_Off();
// 	Fault_Init();
//   GPIO_ResetBits(GPIOB, GPIO_Pin_3);

// 	delay_ms(10);
//   State_Off();
 

//看门狗  
  while (1)
  {
    if (Problem ==Short)
    {
    	State_Short();
    }
    else if(Problem = Vmode)
    {
    	State_Vmode();
    }
    else
    {
    	State_Toggle();
		delay_ms(500);
    }
  }
}	







