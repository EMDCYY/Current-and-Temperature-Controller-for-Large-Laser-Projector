// #include "iwdg.h"

// __IO uint32_t TimingDelay = 0;

// void TIM14_ConfigForLSI(void)
// {
//   NVIC_InitTypeDef  NVIC_InitStructure;
//   TIM_ICInitTypeDef TIM_ICInitStructure;
//   
//   /* 使能外设时钟 ------------------------------------------------*/
//   RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

//   /* 允许访问的RTC */
//   PWR_BackupAccessCmd(ENABLE);

//   /* 复位RTC */
//   RCC_BackupResetCmd(ENABLE);
//   RCC_BackupResetCmd(DISABLE);
//   
//   /*!LSI 使能 */
//   RCC_LSICmd(ENABLE);
//   
//   /*!< 等到LSI已准备就绪 */
//   while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
//   {}
//   
//   /* 选择RTC时钟源*/
//   RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
//    
//   /* 使能RTC时钟*/
//   RCC_RTCCLKCmd(ENABLE);

//   /* 等待RTC APB寄存器同步*/
//   RTC_WaitForSynchro();
//   
//   /* 使能TIM14时钟 */
//   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);  
//   
//   /* 使能 TIM14 中断 */
//   NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
//   NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);
//   
//   /* 配置TIM14预分频器		*/
//   TIM_PrescalerConfig(TIM14, 0, TIM_PSCReloadMode_Immediate);
//   
//   /* 连接内部的 TM14_CH1 输入捕获到LSI时钟输出*/
//   TIM_RemapConfig(TIM14, TIM14_RTC_CLK);
//   
//   /* TIM14 配置: 输入捕获模式 ---------------------
//      LSI 时钟与 TIM14 CH1通道相连
//      上升沿被用作有效边沿,
//      TIM14 CCR1被用来计算频率值 
//   ------------------------------------------------------------ */
//   TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
//   TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
//   TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//   TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV8;
//   TIM_ICInitStructure.TIM_ICFilter = 0;
//   TIM_ICInit(TIM14, &TIM_ICInitStructure);
//   
//   /* TIM14计数器使能  */
//   TIM_Cmd(TIM14, ENABLE);

//   /* 复位标志 */
//   TIM14->SR = 0;
//     
//   /*  使能CC1中断请求 */  
//   TIM_ITConfig(TIM14, TIM_IT_CC1, ENABLE);  
// }

// void IWDG_Configuration(void)
// {
// #ifdef LSI_TIM_MEASURE
//   /* TIM配置 -------------------------------------------------------*/
//   TIM14_ConfigForLSI();
//   
//   /* 等待当 TIM14 捕获 2 个LSI 边沿 */
//   while(CaptureNumber != 2)
//   {
//   }

//   /* 关闭TIM14 CC1 中断应答 */
//   TIM_ITConfig(TIM14, TIM_IT_CC1, DISABLE);
// #endif
//   
//   
//   /* 使能写访问IWDG_PR and IWDG_RLR 寄存器 */
//   IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
//   /* IWDG计数器的时钟: LSI/32 */
//   IWDG_SetPrescaler(IWDG_Prescaler_32);
//   
//   /* 设置重导计数器的值 获得250ms的IWDG超时可以按下面算式计算：  
//      Counter Reload Value = 250ms/IWDG counter clock period
//                           = 250ms / 1/(LSI/32)
//                           = 0.25s / 1/(LsiFreq/32)
//                           = 0.25s / 0.0008
//                           = 313
//   
//   */
//   IWDG_SetReload(313);
//    /* 从新导入IWDG计数器 */
//   IWDG_ReloadCounter();

//   /* 使能 IWDG (LSI 被硬件使能) */
//   IWDG_Enable();
// }
