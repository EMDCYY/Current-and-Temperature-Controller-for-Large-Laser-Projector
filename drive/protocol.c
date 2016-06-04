/**************** (C) COPYRIGHT 2015 LiuXin Academy of Optic-Electronic***********
 * File					:	protocol.c
 * Description  :	Communication Protocol of LD Power Driver with Uppermonitor           
 * Platform			:	stm32f051
 * Producer			:	Liu Xin
 * Web					:	www.emdcyy.com
**********************************************************************************/

#include "protocol.h"
#include "enable.h"
#include "state.h"
#include "uart.h"
#include "dir.h"
#include "delay.h"
#include "converter.h"
#include "stm32f0xx.h"
#include "string.h"
#include "stdbool.h"

#define ID			0x0005
#define COLOR		0x0A   //0x0A - Red; 0x0B - Green; 0x0C - Blue; 0x00 - Check
#define	FrameHead	0xAB
#define	FrameTail	0x16
#define Rsvserved	0xFF
#define DataLength_in_Response 	0x0A

uint8_t	ID_H = (ID & 0xFF00)>>8;
uint8_t	ID_L = (ID & 0x00FF);


extern	__IO	uint8_t rd[19];
extern	__IO	uint8_t Error;
__IO  uint16_t  CurrentMaxim;
__IO  uint16_t  CurrentMinum;
__IO  uint16_t  CurrentTuneValue;
uint8_t	wd[19];


extern __IO uint8_t  Problem;
extern __IO uint16_t ADC_Data[6];



//extern __IO uint16_t  COLOR;	//0x0A - Red; 0x0B - Green; 0x0C - Blue; 0x00 - Check
uint8_t Operate	= 0x00;		//0x01 - Enable_On(); 0x02 - Enable_Off(); 0x03 - Check(); 0x04 - Current; 0x05 - TEC; 0x06 - ID; 
uint8_t DataLen = 0x04;
uint8_t ErroRpt	= 0x00;
uint8_t nCurtH	= 0x00;
uint8_t InCurtL	= 0x00;
uint8_t OtCurtH	= 0x00;
uint8_t OtCurtL	= 0x00;
uint8_t NTC1_H	= 0x00;
uint8_t NTC1_L	= 0x00;
uint8_t NTC2_H	= 0x00;
uint8_t NTC2_L	= 0x00;
uint8_t Check	  = 0x00;



void First_Check_ID	(void);
void Second_Check_Frame(void);
void Third_Check_Length	(void);
void Fourth_Check_Bit	(void);
void Fifth_Check_Rsvserved	(void);
void Sixth_Check_Color	(void);
void rd_Operate	(void);
void wr_Enable	(void);
void rd_Response(void);
void wr_Current	(void);
void wr_TEC		(void);
void wr_ID		(void);
void Response 	(uint8_t Problem);


void protocol(void)
{
	First_Check_ID();
}

void First_Check_ID(void)
{
	ID_H =	(ID & 0xFF00)>>8;
	ID_L =	(ID & 0x00FF);
  if (rd[1]==ID_H && rd[2]==ID_L)
	{
		Second_Check_Frame();
	}
	else
	{
//     Response(0xE1);
	}
}

void Second_Check_Frame(void)
{
	if (rd[0] == FrameHead && (rd[12] == FrameTail || rd[18] == FrameTail))
	{
		Third_Check_Length();
	}
	else
	{
    Response(0xE0);
	}
}



void Third_Check_Length(void)
{
	if((rd[12]==FrameTail && rd[6]==4) || (rd[18]==FrameTail && rd[6]==10))
	{
		Fourth_Check_Bit();
	}
	else
	{
    Response(0xE6);
	}
}



void Fourth_Check_Bit(void)
{
	int ByteNum;
	uint8_t  CheckSum = 0x00;
	for(ByteNum=1; ByteNum<rd[6]+7; ByteNum++)
	{
		CheckSum = rd[ByteNum]^CheckSum;
	}
	if(rd[11] == CheckSum || rd[17] == CheckSum)
  	{
  		Fifth_Check_Rsvserved();
  		ByteNum = 0;
  		CheckSum = 0x00;
  	}
	else
	{
    Response(0xCD);
	}
}


void Fifth_Check_Rsvserved(void)
{
	if (rd[3]==Rsvserved)
	{
		Sixth_Check_Color();
	}
	else
	{
    Response(0xE3);
	}
}

void Sixth_Check_Color(void)
{
	if (rd[4]==COLOR)
	{
		rd_Operate();
	}
	else if (rd[4]==0x00)
	{
		Response(Error);
	}
	else
	{
    Response(0xE4);
	}
}

void rd_Operate(void)
{
	if ((rd[5]==0x01 || rd[5]==0x02) && rd[7]==0x00 && rd[8]==0x00 && rd[9]==0x00 && rd[10]==0x00 )
	{
		wr_Enable();
	}
	
	else if (rd[5]==0x03 && rd[7]==0x00 && rd[8]==0x00 && rd[9]==0x00 && rd[10]==0x00 )
	{
		rd_Response();
	}
		
	else if (rd[5]==0x04 && rd[7]==0x00 && rd[8]==0x00)
	{
		wr_Current();
	}
	
	else if (rd[5]==0x05)
	{
		wr_TEC();
	}
	
	else if (rd[5]==0x06)
	{
		wr_ID();
	}	
	
	else
	{
    Response(0xE5);
	}
}

void wr_Enable(void)
{
  Response(Error);
}

void rd_Response(void)
{
  Response(Error);
}

void wr_Current(void)
{
	if(rd[4] == 0x0A)
  {
     CurrentMaxim = 0x4D9; 
  }
  else if(rd[4] == 0x0C)
  {
     CurrentMaxim = 0xC1F;
  }
  CurrentMinum = 0x0011;
  CurrentTuneValue = dec2hex(rd[9], rd[10]);
  if( (CurrentTuneValue < CurrentMaxim) && (CurrentTuneValue > CurrentMinum) )
  {
    Response(Error);
  }
  else
  {
    Response(0x5C);
  }
}

void wr_TEC(void)
{
	//
	Response(Error);
}

void wr_ID(void)
{
	//FLASH
  Response(Error);
}

void Response(uint8_t Error)
{
	int ByteNum = 0;
	uint8_t  CheckSum;
  uint16_t ConverterData;
  
  wd[0] = FrameHead;
	wd[1] = ID_H;
	wd[2] = ID_L;
	wd[3] = Rsvserved;
	wd[4] = COLOR;
	wd[5] = rd[5];
	wd[6] = DataLength_in_Response;
	
  if(ADC_Data[5] <0x0010 || ADC_Data[5] ==0x0010)
  {
    wd[7] = 0x00;
  }
  else 
  {
    wd[7] = 0x10;
  }

	wd[8] = Error;//
	wd[9] = (ADC_Data[4] & 0xFF00)>>8;
	wd[10]= (ADC_Data[4] & 0x00FF);
  
// 	wd[11]= (ADC_Data[5] & 0xFF00)>>8;
// 	wd[12]= (ADC_Data[5] & 0x00FF);
  ConverterData = hex2dec(ADC_Data[5]);
  wd[11]= (ConverterData & 0xFF00)>>8;
	wd[12]= (ConverterData & 0x00FF);
  
  wd[13]= (CurrentTuneValue & 0xFF00)>>8;
	wd[14]= (CurrentTuneValue & 0x00FF);
//   wd[13]= (ADC_Data[2] & 0xFF00)>>8;
//   wd[14]= (ADC_Data[2] & 0x00FF);
  
	wd[15]= (ADC_Data[3] & 0xFF00)>>8;
	wd[16]= (ADC_Data[3] & 0x00FF);

	for(ByteNum=1; ByteNum < DataLength_in_Response + 7; ByteNum++)
	{
		CheckSum = wd[ByteNum]^CheckSum;
	}
	wd[17]= CheckSum;
	wd[18]= FrameTail;
  
//-Another Way for Transmit Continounsly-
//   for(i=0; i<19; i++)	
//   {
//     Dir_Transmit();
//     delay_ms(1);		
//     USART_SendData(USART1, wd[i]);
//   }
//---------------------------------------
  Dir_Transmit();
  delay_ms(1);
  UART_Send(wd,19);
  delay_ms(2);	
  Dir_Receive();
  
}
