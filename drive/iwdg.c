// #include "iwdg.h"

// __IO uint32_t TimingDelay = 0;

// void TIM14_ConfigForLSI(void)
// {
//   NVIC_InitTypeDef  NVIC_InitStructure;
//   TIM_ICInitTypeDef TIM_ICInitStructure;
//   
//   /* ʹ������ʱ�� ------------------------------------------------*/
//   RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

//   /* ������ʵ�RTC */
//   PWR_BackupAccessCmd(ENABLE);

//   /* ��λRTC */
//   RCC_BackupResetCmd(ENABLE);
//   RCC_BackupResetCmd(DISABLE);
//   
//   /*!LSI ʹ�� */
//   RCC_LSICmd(ENABLE);
//   
//   /*!< �ȵ�LSI��׼������ */
//   while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
//   {}
//   
//   /* ѡ��RTCʱ��Դ*/
//   RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
//    
//   /* ʹ��RTCʱ��*/
//   RCC_RTCCLKCmd(ENABLE);

//   /* �ȴ�RTC APB�Ĵ���ͬ��*/
//   RTC_WaitForSynchro();
//   
//   /* ʹ��TIM14ʱ�� */
//   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);  
//   
//   /* ʹ�� TIM14 �ж� */
//   NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
//   NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);
//   
//   /* ����TIM14Ԥ��Ƶ��		*/
//   TIM_PrescalerConfig(TIM14, 0, TIM_PSCReloadMode_Immediate);
//   
//   /* �����ڲ��� TM14_CH1 ���벶��LSIʱ�����*/
//   TIM_RemapConfig(TIM14, TIM14_RTC_CLK);
//   
//   /* TIM14 ����: ���벶��ģʽ ---------------------
//      LSI ʱ���� TIM14 CH1ͨ������
//      �����ر�������Ч����,
//      TIM14 CCR1����������Ƶ��ֵ 
//   ------------------------------------------------------------ */
//   TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
//   TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
//   TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//   TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV8;
//   TIM_ICInitStructure.TIM_ICFilter = 0;
//   TIM_ICInit(TIM14, &TIM_ICInitStructure);
//   
//   /* TIM14������ʹ��  */
//   TIM_Cmd(TIM14, ENABLE);

//   /* ��λ��־ */
//   TIM14->SR = 0;
//     
//   /*  ʹ��CC1�ж����� */  
//   TIM_ITConfig(TIM14, TIM_IT_CC1, ENABLE);  
// }

// void IWDG_Configuration(void)
// {
// #ifdef LSI_TIM_MEASURE
//   /* TIM���� -------------------------------------------------------*/
//   TIM14_ConfigForLSI();
//   
//   /* �ȴ��� TIM14 ���� 2 ��LSI ���� */
//   while(CaptureNumber != 2)
//   {
//   }

//   /* �ر�TIM14 CC1 �ж�Ӧ�� */
//   TIM_ITConfig(TIM14, TIM_IT_CC1, DISABLE);
// #endif
//   
//   
//   /* ʹ��д����IWDG_PR and IWDG_RLR �Ĵ��� */
//   IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
//   /* IWDG��������ʱ��: LSI/32 */
//   IWDG_SetPrescaler(IWDG_Prescaler_32);
//   
//   /* �����ص���������ֵ ���250ms��IWDG��ʱ���԰�������ʽ���㣺  
//      Counter Reload Value = 250ms/IWDG counter clock period
//                           = 250ms / 1/(LSI/32)
//                           = 0.25s / 1/(LsiFreq/32)
//                           = 0.25s / 0.0008
//                           = 313
//   
//   */
//   IWDG_SetReload(313);
//    /* ���µ���IWDG������ */
//   IWDG_ReloadCounter();

//   /* ʹ�� IWDG (LSI ��Ӳ��ʹ��) */
//   IWDG_Enable();
// }
