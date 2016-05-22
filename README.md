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


##Code Tree##
* [inc](#1)
* [main](#2) 
* [app](#3) 
	* [shut](#3.1)
	* [monitor](#3.2)
	* [communicate](#3.3)
* [lib](#4)
* [driver](#5) 
	* [enable](#5.1)
	* [state](#5.2)
	* [protocol](#5.3)
	

##Flow Chart##

##Diary##
2015.7　***enable.c, state.c*** for power on   
2015.8　***fault.c*** for protection    
2015.9　***dac.c*** for soft-start    
2015.10 ***usart.c*** for communication  
2016.1　***ntc.c*** for temperature control  
2016.5　***USART1_IRQHandler() in stm32f0xx_it.c, protocol.c*** for communication with host computer　　

<font color=Green>**PAY ATTENTION!**</font>   
The Difference between **USART\_GetITStatus** and **USART\_GetFlagStatus** in the stm32f0xx_it.c  



