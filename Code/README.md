##Project##
**Red Laiser Diode** (RLD) Power Drive Code

##Introduction##
Since the laiser diodes are very sensitive to voltage or current ripples, even can be damaged by them, meanwhile, considering the voltage-ampere charastic of diodes, we choose constant current source as their power drive.

**The power drive has several requests as below:**   
1.Input is 12~24V   
2.Output is 30V, 1~2A   
3.SNR(Output/Ripple) is above 1000  
4.Protection for short or open circuit   
5.Communicate with upper monitor   

On the PCB, there are two chips: LTC3796, the constant current source driver, and STM32F051, used for monitor and communication.

This code is run in the STM32F051.


##Code Tree    
* [source](#1)
 	* [startup_stm32f0xx.s](#1.1)
* [inc](#2)  
 	* [system_stm32f0xx.c](#2.1)
* [user](#3) 
  	* [main.c](#3.1)
* [lib](#4) 
	* [stm32f0xx_rcc.c](#4.1)
	* [stm32f0xx_gpio.c](#4.2)
	* [stm32f0xx_it.c](#4.3)
	* [stm32f0xx_usart.c](#4.4)
	* [stm32f0xx_adc.c](#4.5)
	* [stm32f0xx_dac.c](#4.6)
	* [stm32f0xx_dma.c](#4.7)
	* [stm32f0xx_flash.c](#4.8)
	* [stm32f0xx_tim.c](#4.9)
	* [stm32f0xx_iwdg.c](#4.10)
	* [stm32f0xx_exti.c](#4.11)
	* [stm32f0xx_misc.c](#4.12)
	* [stm32f0xx_syscfg.c](#4.13)
* [driver](#5) 
	* [enable.c](#5.1)
	* [state.c](#5.2)
	* [uart.c](#5.3)
	* [dir.c](#5.4)
	* [fault.c](#5.5)
	* [delay.c](#5.6)
	* [adc.c](#5.7)
	* [dac.c](#5.8)
	* [protocol.c](#5.9)
	* [converter.c](#5.10)
	* [iwdg.c](#5.11)
	* [eerprom_emulation.c](#5.12)
	* [pwm.c](#5.13)
	

##Flow Chart##

##Diary##
2015.7　***enable.c, state.c*** for power on   
2015.8　***fault.c*** for protection    
2015.9　***dac.c*** for soft-start    
2015.10 ***usart.c*** for communication  
2016.1　***ntc.c*** for temperature control  
2016.5　***USART1_IRQHandler() in stm32f0xx_it.c, protocol.c*** for communication with host computer  
2016.6	***eeprom_emulation.c, Converter.c, StartUp(), ShutDown(), Tune()*** and other logic control specifics

<font color=Green>**PAY ATTENTION!**</font>   
***Error 1:***  
The Difference between **USART\_GetITStatus** and **USART\_GetFlagStatus** in the stm32f0xx_it.c  
When you hope to get the interrupt of USART, if you write the program as below, they are all right:  
 
	void USART1_IRQHandler(void)  
	{  
		if(USART_GetITStatus(USART1, USART_IT_RXNE)!=RESET)  
		....

or
  
	void USART1_IRQHandler(void)  
	{  
		if(USART_GetFlagStatus(USART1, USART_Flag_RXNE)!=RESET)	

If you write the program as below, it is wrong:  

	void USART1_IRQHandler(void)  
	{  
		if(USART_GetFlagStatus(USART1, USART_IT_RXNE)!=RESET)	