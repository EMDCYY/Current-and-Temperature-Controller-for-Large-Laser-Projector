#include "adc.h"

#define ADC_DR_Address      0x40012440
__IO uint16_t ADC_Data[45];


void ADC_LP_NTC_UVP_ISMON_Config(void)
{
	GPIO_InitTypeDef    GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  //PA0-LightPower;  PA2-NTC1; PA3-UVP;
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6; 

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStruct);										
}

void ADC_IVINMON_Config(void)
{
	GPIO_InitTypeDef    GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  //PB0-CSOUT; PB1-ISMON;
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0; 

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOB, &GPIO_InitStruct);	
}


void ADC_DMA_Init(void)
{
  ADC_InitTypeDef     ADC_InitStruct;
  DMA_InitTypeDef     DMA_InitStruct;
	  
  /* ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  /* DMA1 clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);

  /* ADC1 DeInit */  
  ADC_DeInit(ADC1);
  /* Initialize ADC structure */
  ADC_StructInit(&ADC_InitStruct);
  /* Configure the ADC1 in continous mode with a resolutuion equal to 12 bits  */
  ADC_InitStruct.ADC_Resolution 					= ADC_Resolution_12b;
  ADC_InitStruct.ADC_ContinuousConvMode 	= ENABLE;
  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStruct.ADC_DataAlign 						= ADC_DataAlign_Right;
  ADC_InitStruct.ADC_ScanDirection 				= ADC_ScanDirection_Upward;
  ADC_Init(ADC1, &ADC_InitStruct); 
  /* ADC CLOCK IS 12MHz*/ 

  //0.05MHz, the minimum sampling rate
  ADC_ChannelConfig(ADC1, ADC_Channel_0, ADC_SampleTime_239_5Cycles); 
  ADC_ChannelConfig(ADC1, ADC_Channel_2, ADC_SampleTime_239_5Cycles); 
  ADC_ChannelConfig(ADC1, ADC_Channel_3, ADC_SampleTime_239_5Cycles); 
  ADC_ChannelConfig(ADC1, ADC_Channel_6, ADC_SampleTime_239_5Cycles); 
	ADC_ChannelConfig(ADC1, ADC_Channel_8, ADC_SampleTime_239_5Cycles);
 
  
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
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)ADC_DR_Address;
  DMA_InitStruct.DMA_MemoryBaseAddr 		= (uint32_t)&ADC_Data;
  DMA_InitStruct.DMA_DIR 								= DMA_DIR_PeripheralSRC;
  DMA_InitStruct.DMA_BufferSize 				=	45;
  DMA_InitStruct.DMA_PeripheralInc 			= DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc 					= DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStruct.DMA_MemoryDataSize 		= DMA_MemoryDataSize_HalfWord;
  DMA_InitStruct.DMA_Mode 							= DMA_Mode_Circular;
  DMA_InitStruct.DMA_Priority 					= DMA_Priority_High;
  DMA_InitStruct.DMA_M2M 								= DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStruct);
  /* DMA1 Channel1 enable */
  DMA_Cmd(DMA1_Channel1, ENABLE);
  /* ADC DMA request in circular mode */
  ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
  /* Enable ADC_DMA */
  ADC_DMACmd(ADC1, ENABLE);  
}

void ADC_exInit(void)
{
  ADC_LP_NTC_UVP_ISMON_Config();
  ADC_IVINMON_Config();
  ADC_DMA_Init();
}
