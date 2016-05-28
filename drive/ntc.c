
#include "ntc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define NTC_ADC1_DR_Address      0x40012440
__IO uint16_t Tep_Data[40];
__IO uint16_t InputCurrent;
__IO uint16_t OutputCurrent;
__IO uint16_t Temperature1;
__IO uint16_t Temperature2;

void ADC1_Config1(void)
{
	GPIO_InitTypeDef    GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStruct);				// PC1,输入时不用设置速率
}

void ADC1_Config2(void)
{
	GPIO_InitTypeDef    GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOB, &GPIO_InitStruct);				// PC1,输入时不用设置速率
}

void NTC_ADC1_DMA_Init(void)
{
//   GPIO_InitTypeDef    GPIO_InitStruct;
  ADC_InitTypeDef     ADC_InitStruct;
  DMA_InitTypeDef     DMA_InitStruct;
	
	  
	/* Enable  GPIOA clock */
// 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  /* ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  /* DMA1 clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);

//   GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_2 | GPIO_Pin_3;
//   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
// 	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;
//   GPIO_Init(GPIOA, &GPIO_InitStruct);				// PC1,输入时不用设置速率

  /* ADC1 DeInit */  
  ADC_DeInit(ADC1);
  /* Initialize ADC structure */
  ADC_StructInit(&ADC_InitStruct);
  /* Configure the ADC1 in continous mode withe a resolutuion equal to 12 bits  */
  ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStruct.ADC_ScanDirection = ADC_ScanDirection_Upward;
  ADC_Init(ADC1, &ADC_InitStruct); 
  /* Convert the ADC1 Vref  with 55.5 Cycles as sampling time */ 
  ADC_ChannelConfig(ADC1, ADC_Channel_2, ADC_SampleTime_55_5Cycles); 
	ADC_ChannelConfig(ADC1, ADC_Channel_3, ADC_SampleTime_55_5Cycles);
	ADC_ChannelConfig(ADC1, ADC_Channel_8, ADC_SampleTime_55_5Cycles); 
	ADC_ChannelConfig(ADC1, ADC_Channel_9, ADC_SampleTime_55_5Cycles); 
  //  ADC_VrefintCmd(ENABLE);
  /* ADC Calibration */
  ADC_GetCalibrationFactor(ADC1);
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);     
  /* Wait the ADCEN falg */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN)); 
  /* ADC1 regular Software Start Conv */ 
  ADC_StartOfConversion(ADC1);

  /* DMA1 Channel1 Config */
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)NTC_ADC1_DR_Address;
  DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&Tep_Data;
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStruct.DMA_BufferSize =40;
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStruct.DMA_Priority = DMA_Priority_High;
  DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStruct);
  /* DMA1 Channel1 enable */
  DMA_Cmd(DMA1_Channel1, ENABLE);
  /* ADC DMA request in circular mode */
  ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
  /* Enable ADC_DMA */
  ADC_DMACmd(ADC1, ENABLE);  
}
