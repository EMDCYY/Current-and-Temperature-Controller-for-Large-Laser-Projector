/**
  ******************************************************************************
  * @file    EXTI/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"
#include "enable.h"
#include "uart.h"
#include "state.h"
#include "dir.h"
extern __IO uint16_t  Tep_Mean;
extern __IO uint16_t  Tep_Data[20];
extern __IO uint16_t  Ismon_Data[20];


/** @addtogroup STM32F0_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup EXTI_Example
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void delay_ms(int k)
{
	int i, j;
<<<<<<< HEAD
  for(i=0;i<k;i++)
	  {
			for(j=0;j<48000;j++);
=======
  for(i=0; i<k; i++)
	  {
			for(j=0; j<48000; j++);
>>>>>>> 3fa4ac42ffe091db06dc90f3116ae795573dc734
		}
}

void EXTI4_15_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line5) != RESET)
  {
		Enable_Off();
		while(1)
		{
			State_Short();
		}
// DON'T EXIT INTERUPT!!!
// EXTI_ClearITPendingBit(EXTI_Line5);
  }
	else if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		Enable_Off();
		while(1)
		{
			State_Vmode();
		}
// DON'T EXIT INTERUPT!!!
// EXTI_ClearITPendingBit(EXTI_Line6);
	}
}


void NMI_Handler(void)
{
}

<<<<<<< HEAD

void USART1_IRQHandler(void)
{
  uint16_t temp = 0x0000;
=======
/*
//Transfer 1 channel ADC data
void USART1_IRQHandler(void)
{
	uint16_t temp = 0x0000;
>>>>>>> 3fa4ac42ffe091db06dc90f3116ae795573dc734
	if(USART_GetFlagStatus(USART1, USART_IT_RXNE)!=RESET)
	{
		if(USART_ReceiveData(USART1) == 0x00af)
		{
			Enable_Toggle();
			delay_ms(3);
			temp = ADC_GetConversionValue(ADC1) >> 8;
			USART_SendData(USART1,temp);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
			USART_SendData(USART1, ADC_GetConversionValue(ADC1) );
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
		}
	}
	if(USART_GetITStatus(USART1, USART_IT_RXNE)!= RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}
<<<<<<< HEAD

// /*
=======
/*

/*
// 
>>>>>>> 3fa4ac42ffe091db06dc90f3116ae795573dc734
// void USART1_IRQHandler(void)
// {
// 	uint16_t temp = 0x0000;
// 	if(USART_GetFlagStatus(USART1, USART_IT_RXNE)!=RESET)
// 	{
// 		
// 		Dir_On();
// 		delay_ms(1);
// 		temp = ADC_GetConversionValue(ADC1) >> 8;
// 		USART_SendData(USART1,temp);
// 		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
// 		USART_SendData(USART1, ADC_GetConversionValue(ADC1) );
// 		delay_ms(1);
// 		Dir_Off();
// 		State_Toggle();
// 		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
// 		USART_ClearITPendingBit(USART1, USART_IT_TC);
// 		USART1->USART_RQR_RXFRQ |=0x00000008;
// 		USART_Cmd(USART1, DISABLE);
// 		USART_Cmd(USART1, ENABLE);
// 	}
// }
<<<<<<< HEAD
// */
=======
*/
>>>>>>> 3fa4ac42ffe091db06dc90f3116ae795573dc734

// void USART1_IRQHandler(void)
// {	
// 	int i;
// 	uint16_t temp = 0x0000;
// 	if(USART_GetFlagStatus(USART1, USART_IT_RXNE)!=RESET)
// 	{
// 		for(i=0; i<40 ;i++)
// 		{
// 		Dir_On();
// 		delay_ms(1);
// 		temp = Tep_Data[i] >> 8;
// 		USART_SendData(USART1,temp);
// 		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
// 		USART_SendData(USART1, Tep_Data[i] );
// 		delay_ms(1);
// 		}
// 		
// // 		for(i=0; i<20 ;i++)
// // 		{
// // 		Dir_On();
// // 		delay_ms(1);
// // 		temp = Ismon_Data[i] >> 8;
// // 		USART_SendData(USART1,temp);
// // 		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
// // 		USART_SendData(USART1, Ismon_Data[i] );
// // 		delay_ms(1);
// // 		}

// 		Dir_On();
// 		delay_ms(1);
// 		temp = Tep_Mean >> 8;
// 		USART_SendData(USART1,temp);
// 		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
// 		USART_SendData(USART1, Tep_Mean);
// 		delay_ms(1);
// 		
// 		Dir_Off();
// 		State_Toggle();
// 		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
// 		USART_ClearITPendingBit(USART1, USART_IT_TC);
// // 		USART1->USART_RQR_RXFRQ |=0x00000008;
// 		USART_Cmd(USART1, DISABLE);
// 		USART_Cmd(USART1, ENABLE);
// 	}
// }



/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */


/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
