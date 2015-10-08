#include "stm32f0xx_tim.h"

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
uint16_t TimerPeriod = 0;
uint16_t Channel1Pulse = 0, Channel2Pulse = 0, Channel3Pulse = 0, Channel4Pulse = 0;

void TIM_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* ʹ��GPIOʱ�� */
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
  
  /* ����GPIO�ܽŸ���*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_2);

}

void TIM_PWM_Config(void)
{
	/* TIM1 ������ ---------------------------------------------------
   TIM1 ����ʱ��(TIM1CLK) ����Ϊ APB2 ʱ�� (PCLK2)    
    => TIM1CLK = PCLK2 = SystemCoreClock
   TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
   SystemCoreClock Ϊ48 MHz 
   
   ���ǵ�Ŀ����� 4 ·PWM �ź���17.57 KHz:
     - TIM1_Period = (SystemCoreClock / 17570) - 1
   �ŵ�1���õ�ռ�ձ�Ϊ 50%
   �ŵ�2���õ�ռ�ձ�Ϊ 37.5%
   �ŵ�3���õ�ռ�ձ�Ϊ 25%
   �ŵ�4���õ�ռ�ձ�Ϊ 12.5%
   ��ʱ������ļ��㷽ʽ����:
     - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
	*/  
    /*����Ԥ�����ֵ��Ҳ���Ƕ��ٸ�ʱ�Ӽ���Ϊһ������*/
  TimerPeriod = (SystemCoreClock / 17570 ) - 1;
  /*����CCR1 ��תֵ ��ռ�ձ�Ϊ50%ʱ*/
  Channel1Pulse = (uint16_t) (((uint32_t) 5 * (TimerPeriod - 1)) / 10);
 /*����CCR2 ��תֵ ��ռ�ձ�Ϊ37.5%ʱ*/
  Channel2Pulse = (uint16_t) (((uint32_t) 375 * (TimerPeriod - 1)) / 1000);
  /*����CCR3 ��תֵ ��ռ�ձ�Ϊ25%ʱ*/
  Channel3Pulse = (uint16_t) (((uint32_t) 25 * (TimerPeriod - 1)) / 100);
 /*����CCR4��תֵ ��ռ�ձ�Ϊ12.5%ʱ*/
  Channel4Pulse = (uint16_t) (((uint32_t) 125 * (TimerPeriod- 1)) / 1000);

  /* TIM1 ʱ��ʹ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
  
  /* Time ��ʱ��������*/
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /* Time ��ʱ����Ϊ�����ؼ���ģʽ*/
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* Ƶ��1��2��3��4��PWM ģʽ���� */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;//ʹ��Ƶ��1����
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;//ʹ��Ƶ��2����
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = Channel3Pulse;//ʹ��Ƶ��3����
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = Channel4Pulse;//ʹ��Ƶ��4����
  TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	
  /* TIM1 ������ʹ��*/
  TIM_Cmd(TIM1, ENABLE);
	
  /* TIM1 �����ʹ�� */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);	
	}
