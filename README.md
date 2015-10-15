##Project##
**Red Laiser Diode** (RLD) Power Drive Code

##Introduction##
Since the laiser diodes are very sensitive to voltage or current ripples, even can be damaged by them, meanwhile, considering the voltage-ampere charastic of diodes, we choose constant current source as their power drive.

**The power drive has several requests as below:**   
1.Input is 24V   
2.Output is 30V, 1A   
3.SNR(Output/Ripple) is above 200  
4.Protection for short or open circuit   
5.Communicate with upper monitor   

I have realized the PCB of power drive. On my PCB, there are two chips: LTC3796, with what the constant current source come true, and STM32F051, what is used for monitor and communication.

This code is run in the STM32F051.


##Code Tree##
* [include](#1)
* [main](#2) 
* [app](#3) 
	* [shut](#3.1)
	* [monitor](#3.2)
	* [communicate](#3.3)
* [drive](#4)
	* [enable](#4.1)
	* [status](#4.2)
	* [current](#4.3)
	* [tempearture](#4.4)
	* [default](#4.5)
* [lib](#5) 