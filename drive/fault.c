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
	 
	/* config the extiline(PA6) clock and AFIO clock */
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    /* Configyre P[A|B|C|D|E]0  NIVC  */
		NVIC_InitStruct.NVIC_IRQChannel = EXTI4_15_IRQn;
		NVIC_InitStruct.NVIC_IRQChannelPriority = 0x00;
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStruct);
	
	  	/* EXTI line gpio config(PA6) */	
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 
// 	  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 ;
	  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_2;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; // ��������
		GPIO_Init(GPIOA, &GPIO_InitStruct);
		GPIO_ResetBits(GPIOA, GPIO_Pin_6 | GPIO_Pin_7 );
// 		GPIO_ResetBits(GPIOA, GPIO_Pin_6 );		
		/* EXTI line(PA6) mode config */
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource6 | EXTI_PinSource7);
// 		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource6 );
		EXTI_InitStruct.EXTI_Line = EXTI_Line6 | EXTI_Line7;
// 		EXTI_InitStruct.EXTI_Line = EXTI_Line6 ;
		EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
		EXTI_InitStruct.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStruct); 
}


