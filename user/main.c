/*****(C) COPYRIGHT 2016 LiuXin Academy of Optic-Electronic******
 * File					:	main.c
 * Description  :	LD Power Driver Control          
 * Platform			:	stm32f051
 * Producer			:	Liu Xin
 * Web					:	www.emdcyy.com
*****************************************************************/

#include "stm32f0xx.h"
#include "stdio.h"
#include "stdbool.h"

/* Basic State Setup--------------------------------------------*/
#include "state.h"
#include "enable.h"
#include "delay.h"

/* Fault Report Intial------------------------------------------*/
#include "fault.h"

/* UART RS485 Header--------------------------------------------*/
#include "dir.h"	//The Direction of RS485 -- Recive or Transmit
#include "uart.h"	//RS485 Configuration

/* DAC for Control Current--------------------------------------*/
#include "dac.h"

/* ADC for Light Power or Under Voltage Protection or NTC or ISMON or CSOUT Header-----------------------*/
#include "adc.h"

/* Protocol for Communicate with Monitor------------------------*/
#include "protocol.h"

/* EEPROM-for saving ID & Current Value-------------------------*/
#include "eeprom_emulation.h"

/* Other Function-----------------------------------------------*/
// #include "PWM.h"

/* Interupt-----------------------------------------------------*/
#include "stm32f0xx_it.h"

/* IWDG---------------------------------------------------------*/
#include "iwdg.h"

extern __IO uint8_t  Error;

int main(void)
{ 
  SystemInit();
	
/* Basic State Setup----------------------------------------------------*/
	Enable_Init();	//Enable LT3763
	State_Init(); 	//LED Show

/* Fault Report Intial--------------------------------------------------*/
	Fault_Init();
  
/* IWDG Intial----------------------------------------------------------*/
//   IWDG_Configuration();
 	
// /* PWM Intial-----------------------------------------------------------*/
//   PWM_Init();
  
/* UART RS485 Initial---------------------------------------------------*/
  UART_Init();

/* ADC Intial-----------------------------------------------------------*/
  ADC_exInit();

/* DAC output for Ctrl Pin input of LT3763 for soft start---------------*/  
  DAC_exInit();

  while (1)
  {
//   IWDG_ReloadCounter(); //feed WatchDog
  
    if (Error == Short)
    {
      while(1)
      {
        Enable_Off();
        State_Short();
      }
    }
    else if(Error == Vmode)
    {
      while(1)
      {
        Enable_Off();
        State_Vmode();
      }
    }
    else
    {
    	State_Toggle();
      delay_ms(1000);
      if (StartUp_Condition() == true && Error == NoWarn)
      {
        StartUp();
      }
      else if (ShutDown_Condition() == true && Error == NoWarn)
      {
        ShutDown();	
      }
      else if (Tune_Condition() == true && Error == NoWarn)
      {
        Tune();
      }
    }
  }
}	

