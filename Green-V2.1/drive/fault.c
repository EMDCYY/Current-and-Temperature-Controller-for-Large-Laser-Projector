/**************** (C) COPYRIGHT 2015 LiuXin Academy of Optic-Electronic***********
 * File			:	enable.c
 * Description  :	Set/reset enable pin to turn on/off driver IC.          
 * Platform		:	stm32f051
 * Producer		:	Liu Xin
 * Web			:	www.emdcyy.com
**********************************************************************************/

#include "fault.h"
/**********************************************/
/* Function		:	Initial Enable Pin        */
/* Parameter	:	None                      */
/**********************************************/
void Fault_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStruct; 
  	EXTI_InitTypeDef EXTI_InitStruct;
	  NVIC_InitTypeDef NVIC_InitStruct;

	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

		NVIC_InitStruct.NVIC_IRQChannel 				= EXTI0_1_IRQn;
		NVIC_InitStruct.NVIC_IRQChannelPriority = 0x00;
		NVIC_InitStruct.NVIC_IRQChannelCmd 			= ENABLE;
		NVIC_Init(&NVIC_InitStruct);

		GPIO_InitStruct.GPIO_Pin 		= GPIO_Pin_1; 
	  GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_IN;
		GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_Level_1;
    GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_NOPULL; // 上拉输入
		GPIO_Init(GPIOB, &GPIO_InitStruct);
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);

		/* EXTI line(PB1) mode config */
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);
		EXTI_InitStruct.EXTI_Line			= EXTI_Line1;
		EXTI_InitStruct.EXTI_Mode			= EXTI_Mode_Interrupt;
		EXTI_InitStruct.EXTI_Trigger	= EXTI_Trigger_Falling; //下降沿中断
		EXTI_InitStruct.EXTI_LineCmd	= ENABLE;
		EXTI_Init(&EXTI_InitStruct); 
}


