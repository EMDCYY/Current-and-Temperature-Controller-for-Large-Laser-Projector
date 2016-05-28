/**************** (C) COPYRIGHT 2015 LiuXin Academy of Optic-Electronic***********
 * File			:	enable.c
 * Description  :	Set/reset enable pin to turn on/off driver IC.          
 * Platform		:	stm32f051
 * Producer		:	Liu Xin
 * Web			:	www.emdcyy.com
**********************************************************************************/

#include "rdctrl.h"
/**********************************************/
/* Function		:	Initial Enable Pin        */
/* Parameter	:	None                      */
/**********************************************/
void RdCtrl_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStruct; 
//   	EXTI_InitTypeDef EXTI_InitStruct;
	  NVIC_InitTypeDef NVIC_InitStruct;
	 
	/* config the extiline(PA6) clock and AFIO clock */
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    /* Configyre P[A|B|C|D|E]0  NIVC  */
		NVIC_InitStruct.NVIC_IRQChannel = EXTI4_15_IRQn;
		NVIC_InitStruct.NVIC_IRQChannelPriority = 0x01;
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStruct);
	
	  	/* EXTI line gpio config(PA5) */	
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5; 
	  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_2;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; // 上拉输入
		GPIO_Init(GPIOA, &GPIO_InitStruct);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	
		/* EXTI line(PA5) mode config */
// 		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource5);
// 		EXTI_InitStruct.EXTI_Line = EXTI_Line5;
// 		EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
// 		EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
// 		EXTI_InitStruct.EXTI_LineCmd = ENABLE;
// 		EXTI_Init(&EXTI_InitStruct); 
}
