// #include "dac.h"
// /**********************************************/
// /* Function		:	Initial Enable Pin        */
// /* Parameter	:	None                      */
// /**********************************************/
// void DAC_exInit(void)
// {
// 	GPIO_InitTypeDef GPIO_InitStruct;
// 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
// 	GPIO_InitStruct.GPIO_Pin   = DAC_PIN ;
// 	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
// 	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
// // 	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
// 	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
// 	GPIO_Init(DAC_PORT, &GPIO_InitStruct);
// 	GPIO_ResetBits(DAC_PORT, DAC_PIN );
// }

// /**********************************************/
// /* Function		:	Enable Pin Off            */
// /* Parameter	:	None                      */
// /**********************************************/
// void DAC_On(void)
// {
// 	 GPIO_SetBits(DAC_PORT, DAC_PIN );
// }

// /**********************************************/
// /* Function		:	Enable Pin On             */
// /* Parameter	:	None                      */
// /**********************************************/
// void DAC_Off(void)
// {
// 	GPIO_ResetBits(DAC_PORT, DAC_PIN );
// }

// /**********************************************/
// /* Function		:	Enable Pin Toggle         */
// /* Parameter	:	None                      */
// /**********************************************/
// void DAC_Toggle(void)
// {
// 	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_4))));
// }

#include "stm32f0xx.h"
#include "dac.h"
// #include "exit.h"

#define DAC_DHR12R1_ADDRESS      0x40007408
#define DAC_DHR8R1_ADDRESS       0x40007410

const uint16_t ctrlSoftStart[137] = {
	1395,1415,1435,1455,1475,1495,1515,1535,1555,
	1575,1595,1615,1635,1655,1675,1695,1715,1735,1755,
	1775,1795,1815,1835,1855,1875,1895,1915,1935,1955,
	1975,1995,2015,2035,2055,2075,2095,2115,2135,2155,
	2175,2195,2215,2235,2255,2275,2295,2315,2335,2355,
	2375,2395,2415,2435,2455,2475,2495,2515,2535,2555,
	2575,2595,2615,2635,2655,2675,2695,2715,2735,2755,
	2775,2795,2815,2835,2855,2875,2895,2915,2935,2955,
	2975,2995,3015,3035,3055,3075,3095,3115,3135,3155,
	3175,3195,3215,3235,3255,3275,3295,3315,3335,3355,
	3375,3395,3415,3435,3455,3475,3495,3515,3535,3555,
	3575,3595,3615,3635,3655,3675,3695,3715,3735,3755,
	3775,3795,3815,3835,3855,3875,3895,3915,3935,3955,
	3975,3995,4015,4035,4055,4075,4095};	

const uint8_t Escalator8bit[6] = {0x0, 0x33, 0x66, 0x99, 0xCC, 0xFF};
uint8_t Idx = 0;
extern volatile unsigned char SelectedWavesForm,WaveChange ; 

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

void DAC_exInit(void)
{
  GPIO_InitTypeDef					GPIO_InitStructure;
	DAC_InitTypeDef						DAC_InitStructure;
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	DMA_InitTypeDef           DMA_InitStructure;
	
  /* GPIOA clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  /* Configure PA.04 (DAC_OUT1) as analog */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
// 	GPIO_ResetBits(DAC_PORT, DAC_PIN );
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	 /* DAC Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	DAC_Cmd(DAC_Channel_1, ENABLE);

	/* TIM2 ����ʱ��ʹ��*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  /* ��ʱ���������� */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = 0xffff;          
  TIM_TimeBaseStructure.TIM_Prescaler = 0x1a;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	/* TIM2�������ģʽѡ��  */
  TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
   /* TIM2ʹ�� */
  TIM_Cmd(TIM2, ENABLE);

  /* DMA1 clock enable (to be used with DAC) */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel3); 
	DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1_ADDRESS;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ctrlSoftStart;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = 136;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//   DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel3, &DMA_InitStructure);
          /* ʹ��DMA1ͨ��3 */
  DMA_Cmd(DMA1_Channel3, ENABLE);
          /* DAC ͨ��1��ʼ�� */
          /* ʹ��DAC ͨ��1: �� DAC ͨ��1��ʹ�� , PA.04 �Զ���DACת��������. */

          /* ʹ��DACͨ��1��DMA */
//   DAC_DMACmd(DAC_Channel_1, ENABLE);
	
// 	DAC_Cmd(DAC_Channel_1, DISABLE);
// 	TIM_Cmd(TIM2, DISABLE);
// 	DMA_Cmd(DMA1_Channel3, DISABLE);
// 	DAC_DMACmd(DAC_Channel_1, DISABLE);
}

void DAC_On(void)
{
	/* ʹ��DACͨ��1��DMA */
  DAC_DMACmd(DAC_Channel_1, ENABLE);
}

// void DAC_Off(void)
// {
// 	/* ʹ��DACͨ��1��DMA */
//   DAC_DMACmd(DAC_Channel_1, DISABLE);
// }
