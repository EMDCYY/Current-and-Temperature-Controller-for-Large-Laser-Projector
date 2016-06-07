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
#include "fault.h"

/* UART RS485 Header--------------------------------------------*/
#include "dir.h"	//The Direction of RS485 -- Recive or Transmit
#include "uart.h"	//RS485 Configuration

/* DAC Header---------------------------------------------------*/
#include "dac.h"

/* ADC for Light Power or Under Voltage Protection or NTC or ISMON or CSOUT Header-----------------------*/
#include "adc.h"

/* Protocol Header----------------------------------------------*/
#include "protocol.h"

/* Other Function-----------------------------------------------*/
// #include "PWM.h"

/* Interupt-----------------------------------------------------*/
#include "stm32f0xx_it.h"

/* IWDG---------------------------------------------------------*/
// #include "iwdg.h"

/* EEPROM-------------------------------------------------------*/
// #include "eeprom_emulation.h"

extern __IO uint8_t  Error;
extern __IO uint16_t ADC_Data[6];
extern __IO uint8_t rd[19];

int main(void)
{ 

  
  SystemInit();
	
/* Basic State Setup----------------------------------------------------*/
	Enable_Init();	//Enable LT3763
	State_Init(); 	//LED Show

/* Fault Report Intial--------------------------------------------------*/
	Fault_Init();
  
/* IWDG Intial----------------------------------------------------------*/
//   TIM14_ConfigForLSI();
//   IWDG_Configuration();
	
// /* PWM Intial-----------------------------------------------------------*/
//   PWM_Init();
  
/* DAC output for Ctrl Pin input of LT3763 for soft start---------------*/	
	DAC_Config();

	
/* UART RS485 Initial---------------------------------------------------*/
	Dir_Init();							//The Direction of RS485 -- Recive or Transmit
	delay_ms(10);
	UART_Configuration();	  //RS485 Configuration
	delay_ms(10);
 	Dir_Receive();

/* ADC Intial-----------------------------------------------------------*/
  ADC_LP_UVP_NTC_Config();
  ADC_CSOUT_ISMON_Config();
  ADC_DMA_Init();

 
  while (1)
  {
    if (Error == Short)
    {
      Enable_Off();
      State_Short();

    }
    else if(Error == Vmode)
    {
      Enable_Off();
      State_Vmode();
    }
    else
    {
    	State_Toggle();
      delay_ms(1000);
      if (ADC_Data[5] >0x0010 && rd[5] ==0x02 && rd[7]==0x00 && rd[8]==0x00 && rd[9]==0x00 && rd[10]==0x00) //Runing and Want to Shut Down
      {
        ShutDown();
      }
      else if ((ADC_Data[5] <0x0010 || ADC_Data[5] ==0x0010) && rd[5] ==0x01 && rd[7]==0x00 && rd[8]==0x00 && rd[9]==0x00 && rd[10]==0x00) //Shuting Down and Want to Run
      {
        StartUp();	
      }
      else if ( ADC_Data[5] >0x0010 && rd[5] ==0x04 && rd[7]==0x00 && rd[8]==0x00)
      {
        Tune();
      }
      
    }
  }
}	

