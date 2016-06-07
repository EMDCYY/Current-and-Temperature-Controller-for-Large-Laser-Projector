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
#include "protocol.h"
#include "dir.h"
#include "delay.h"
#include "stdarg.h"
#include "stdio.h"

__IO uint8_t  Error = NoWarn;

// extern __IO uint16_t  Tep_Mean;
// extern __IO uint16_t  Tep_Data[20];
// extern __IO uint16_t  Ismon_Data[20];
__IO uint8_t  rd[19];
extern __IO uint8_t  wd[18];
uint16_t cd[12];
int k = 0;
int i = 0;

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
int count ;	
int lastcount ;
int flag;


void EXTI4_15_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line6) != RESET)
  {

		Error = Short;
    Enable_Off();
    EXTI_ClearITPendingBit(EXTI_Line6);
  }
	else if(EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
		Error = Vmode;
    Enable_Off();
    EXTI_ClearITPendingBit(EXTI_Line7);
	}
}


void NMI_Handler(void)
{
}

/**
  * @brief  This function handles TIM14 global interrupt request.
  * @param  None
  * @retval None
  */
// void TIM14_IRQHandler(void)
// {
//   if (TIM_GetITStatus(TIM14, TIM_IT_CC1) != RESET)
//   {    
//     if(CaptureNumber == 0)
//     {
//       /* 获得输入比较的值 */
//       IC1ReadValue1 = TIM_GetCapture1(TIM14);
//     }
//     else if(CaptureNumber == 1)
//     {
//       /* 获得输入比较的值 */
//       IC1ReadValue2 = TIM_GetCapture1(TIM14); 
//       
//       /* 比较2次的值 */
//       if (IC1ReadValue2 > IC1ReadValue1)
//       {
//         Capture = (IC1ReadValue2 - IC1ReadValue1); 
//       }
//       else
//       {
//         Capture = ((0xFFFF - IC1ReadValue1) + IC1ReadValue2); 
//       }
//       /*  频率计算*/ 
//       LsiFreq = (uint32_t) SystemCoreClock / Capture;
//       LsiFreq *= 8;
//     }
//     
//     CaptureNumber++;
//     
//     /* 清除TIM4捕获比较中断位*/
//     TIM_ClearITPendingBit(TIM14, TIM_IT_CC1);
//   }
// }

//---------------EXAMPLE 1---------------------//
// void USART1_IRQHandler(void)
// {
// 	if(USART_GetFlagStatus(USART1, USART_IT_RXNE)!=RESET)
// 	{
// 		Dir_Transmit();
// 		delay_us(800);		
// 		USART_SendData(USART1, 0xAA37);
// 		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
// 		USART_SendData(USART1, 0xAA92);
// 		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
// 		delay_us(900);
// 		USART_Cmd(USART1, DISABLE);
// // 		delay_us(100);
// 		USART_Cmd(USART1, ENABLE);
// 	}
// 		Dir_Receive();
// 		USART_ClearITPendingBit(USART1, USART_IT_TC);
// 		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
// }
//--------------------------------------------------

// void USART1_IRQHandler(void) 
// { 
//     uint16_t i; 
//      
//     if(USART_GetFlagStatus(USART1,USART_IT_RXNE)==SET) 
//     {               
//         i = USART_ReceiveData(USART1); 
//         USART_SendData(USART1,i); 
//         while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) 
//         { 
//         }                
//   0  }
//     if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
//     { 
//          
//         USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//     }
// }


//---------------EXAMPLE 2---------------------//
// void USART1_IRQHandler(void)
// {
// 	if(USART_GetFlagStatus(USART1, USART_IT_RXNE)!=RESET)
// 	{
//     Dir_Transmit();
// 		delay_ms(1);		
// 		USART_SendData(USART1, USART_ReceiveData(USART1));
// 		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
//     delay_ms(1);
// 		USART_SendData(USART1, 0x92);
// 		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
// 		delay_ms(1);
// 		USART_Cmd(USART1, DISABLE);
// 		USART_Cmd(USART1, ENABLE);
// 	}
// 		Dir_Receive();
// // 		USART_ClearITPendingBit(USART1, USART_IT_TC);
// 		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
// }


//---------------EXAMPLE 3---------------------//
// void USART1_IRQHandler(void)
// {

// 		cd[0] = 0xABCD;
//     Dir_Transmit();
// 		delay_ms(1);		
//     USART_SendData(USART1, cd[0]);
// 		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
// 		USART_SendData(USART1, cd[0]>>8);
// 		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);

// 		delay_ms(1);
// 		USART_Cmd(USART1, DISABLE);
// 		USART_Cmd(USART1, ENABLE);
// 		Dir_Receive();
// }

//--------------------------------------------------

// //---------------EXAMPLE 3.1---------------------//
// void USART1_IRQHandler(void)
// {
// 	if(USART_GetFlagStatus(USART1, USART_IT_RXNE)!=RESET)
// 	{
//     rd[0] = USART_ReceiveData(USART1);
//     
//     if (rd[0] == 0xDD)
//     {
//       Dir_Transmit();
//       delay_ms(1);		
//       USART_SendData(USART1, rd[0]);
//       while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
//       delay_ms(1);
//       USART_SendData(USART1, 0x92);
//       while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
//       
//     }
//     delay_ms(1);
// 		USART_Cmd(USART1, DISABLE);
// 		USART_Cmd(USART1, ENABLE);
// 	}
// 		Dir_Receive();
// // 		USART_ClearITPendingBit(USART1, USART_IT_TC);
// // 		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
// }

//--------------------------------------------------

//---------------EXAMPLE 3.1---------------------//

// void USART1_IRQHandler(void)
// {
//   if(USART_GetITStatus(USART1, USART_IT_RXNE)!=RESET)	
//   {
//     rd[0] = USART_ReceiveData(USART1); 
//     Dir_Transmit();
//     delay_ms(1);	
//     USART_SendData(USART1, rd[0]);
//     delay_ms(1);	
//     Dir_Receive();
//   }
//   
//   if (USART_GetITStatus(USART1, USART_IT_TC) != RESET)
//   {
//     USART_ClearITPendingBit(USART1, USART_IT_TC);
//   }
// } 

//---------------EXAMPLE 3.2---------------------//

void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE)!=RESET)	
  {
    rd[k++] = USART_ReceiveData(USART1); 
    if(rd[k-1]==0x16)
    {
//       for(i=0; i<k; i++)
//       {
//         Dir_Transmit();
//         delay_ms(1);	
//         USART_SendData(USART1, rd[i]);
//       }
//       delay_ms(2);	
//       Dir_Receive();
      k = 0;
      protocol();
    }

//    if (USART_GetITStatus(USART1, USART_IT_TC) != RESET)
//    {
//       USART_ClearITPendingBit(USART1, USART_IT_TC);
//    }
  }
} 


//---------------EXAMPLE 4---------------------//
// void USART1_IRQHandler(void)
// {
// 		
//   rd[0] = USART_ReceiveData(USART1);
//   if (rd[0] == 0xAA)
//   {
//     Dir_Transmit();
//     delay_us(10);		
//     USART_SendData(USART1, rd[0]);
//     while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
//     USART_SendData(USART1, 0x92);
//     while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
//   }
//     
//   delay_us(1000);
// 	USART_Cmd(USART1, DISABLE);
// 	delay_us(10);
// 	USART_Cmd(USART1, ENABLE);
// 	Dir_Receive();
// } 

//---------------EXAMPLE 5---------------------//
// void USART1_IRQHandler(void)
// {
// 	
//   rd[0] = USART_ReceiveData(USART1);
//   if (rd[0] == 0xAA)
//   {
//     count = 0;
//     flag =1;
//   }
//   else if(rd[0] == 0xDD && count > 0)
//   {
//     
//     Dir_Transmit();
//     delay_ms(1);		
//     USART_SendData(USART1, rd[count]);
//     while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
//     count --;
//     if (count == 0) flag = 0;
//   }
//   else if( flag == 1 && rd[0] != 0xDD && rd[0] != 0xAA)
//   {
//     count++;
//     rd[count] = USART_ReceiveData(USART1);
//   }
//     
//   delay_ms(1);
// 	USART_Cmd(USART1, DISABLE);
// 	USART_Cmd(USART1, ENABLE);
// 	Dir_Receive();
// }  

//---------------EXAMPLE 6---------------------//
// void USART1_IRQHandler(void)
// {
// 	
//   while(1)
// 		{
//       State_On();
//       delay_us(500);
//       GPIO_SetBits(GPIOB, GPIO_Pin_3);
//       State_Off();
//       delay_us(500);
//       GPIO_ResetBits(GPIOB, GPIO_Pin_3);
// 		}
// }  



//--------------------------------------------------

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
