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
#include "eeprom_emulation.h"
#include "stm32f0xx_it.h"


#define COLOR		0x0C   //0x0A - Red; 0x0B - Green; 0x0C - Blue; 0x00 - Check

#define RedColor	0x0A
#define BlueColor	0x0C
#define	FrameHead	0xAB
#define	FrameTail	0x16
#define Reserved	0xFF
#define DataLength_in_Response 	0x0A
#define Response_in_Broadcast	0x00
#define PCX 0x00 //PaddingCharacter

#define SN_FrameHead	0
#define SN_ID_H			1
#define SN_ID_L			2
#define SN_Reserved		3
#define SN_Color		4
#define SN_Order		5
#define SN_DataLen		6
#define SN_Data_1		7
#define SN_Data_2		8
#define SN_Data_3		9
#define SN_Data_4		10		
#define SN_CheckBit		11
#define SN_FrameTail	12


#define Order_Enable_On 	0x01
#define Order_Enable_Off 	0x02
#define Order_Response	 	0x03
#define	Order_wr_Current	0x04
#define Order_wr_TEC		0x05
#define Order_wr_ID			0x06	


#define Error_Frame_Head_Tail 	0xE0
#define Error_Check_Reserved	0xE3
#define Error_Check_Color		0xE4
#define Error_rd_Order	0xE5
#define Error_Check_DataLen	0xE6
#define Error_Check_Bit	0xCD
#define Error_wr_Current 0x5C

extern	__IO	uint8_t rd[19];
extern	__IO	uint8_t Error;
extern __IO bool Flag_On;
extern __IO uint16_t ADC_Data[6];
extern __IO uint32_t ID_ADDRESS;

__IO  uint16_t  CurrentMaxim;
__IO  uint16_t  CurrentMinum;
__IO  uint16_t  CurrentTuneValue;
uint8_t	wd[19];
bool Flag_Order = false;


void First_Check_ID			(void);
void Second_Check_Frame		(void);
void Third_Check_DataLen	(void);
void Fourth_Check_Bit		(void);
void Fifth_Check_Reserved	(void);
void Sixth_Check_Color		(void);
void rd_Order	(void);
void wr_Enable	(void);
void rd_Response(void);
void wr_Current	(void);
void wr_TEC		(void);
void wr_ID		(void);
void Response 	(uint8_t Error);


void protocol(void)
{
// 	Error = NoWarn;
	Flag_Order = false;
	First_Check_ID();
}

void First_Check_ID(void)
{
	uint16_t ID;
	uint8_t	ID_H;
	uint8_t	ID_L;

	ID = (*(__IO uint32_t*)(ID_ADDRESS));
	delay_ms(3);
	
	ID_H = (ID & 0xFF00)>>8;
	ID_L = (ID & 0x00FF);
  
	if (rd[SN_ID_H]==ID_H && rd[SN_ID_L]==ID_L)
	{
		Second_Check_Frame();
	}
	else if(rd[SN_ID_H] == PCX && rd[SN_ID_L] == PCX) 
	{
    	Second_Check_Frame();
	}
  	else
	{

	}
}

void Second_Check_Frame(void)
{
	if (rd[SN_FrameHead] == FrameHead && rd[SN_FrameTail] == FrameTail)
	{
		Third_Check_DataLen();
	}
	else
	{
    	Response(Error_Frame_Head_Tail);
	}
}



void Third_Check_DataLen(void)
{
	if(rd[SN_DataLen]==4)
	{
		Fourth_Check_Bit();
	}
	else
	{
    	Response(Error_Check_DataLen);
	}
}



void Fourth_Check_Bit(void)
{
	int ByteNum;
	uint8_t  CheckSum = 0x00;

	for(ByteNum=1; ByteNum<rd[SN_DataLen]+7; ByteNum++)
	{
		CheckSum = rd[ByteNum]^CheckSum;
	}

	if(rd[SN_CheckBit] == CheckSum)
{
  		Fifth_Check_Reserved();
  		ByteNum = 0;
  		CheckSum = 0x00;
  	}
	else
	{
    	Response(Error_Check_Bit);
	}
}


void Fifth_Check_Reserved(void)
{
	if (rd[SN_Reserved]==Reserved)
	{
		if(rd[SN_ID_H] == PCX && rd[SN_ID_L] == PCX && rd[SN_Order] != Order_wr_ID)
    	{
      		Response(Error);
    	}
    	else if(rd[SN_ID_H] == PCX && rd[SN_ID_L] == PCX && rd[SN_Order] == Order_wr_ID)
    	{
      		wr_ID();
    	}
    	else
    	{
      		Sixth_Check_Color();
    	}
	}
	else
	{
    	Response(Error_Check_Reserved);
	}
}

void Sixth_Check_Color(void)
{
	if (rd[SN_Color]==COLOR)
	{
		rd_Order();
	}
	else if (rd[SN_Color] == Response_in_Broadcast) // Response without requirement only with right ID
	{
		Response(Error);
	}
	else
	{
    	Response(Error_Check_Color);
	}
}

void rd_Order(void)
{
	if ((rd[SN_Order]==Order_Enable_On || rd[SN_Order]==Order_Enable_Off) && rd[SN_Data_1]==PCX && rd[SN_Data_2]==PCX && rd[SN_Data_3]==PCX && rd[SN_Data_4]==PCX)
	{
		wr_Enable();
	}
	
	else if (rd[SN_Order]==Order_Response && rd[SN_Data_1]==PCX && rd[SN_Data_2]==PCX && rd[SN_Data_3]==PCX && rd[SN_Data_4]==PCX)
	{
		rd_Response();
	}
		
	else if (rd[SN_Order]==Order_wr_Current && rd[SN_Data_1]==PCX && rd[SN_Data_2]==PCX)
	{
		wr_Current();
	}
	
	else if (rd[5]==Order_wr_TEC)
	{
		wr_TEC();
	}
	
	else if (rd[5]==Order_wr_ID && rd[SN_Data_3]==PCX && rd[SN_Data_4]==PCX)
	{
		wr_ID();
	}	
	
	else
	{
    	Response(Error_rd_Order);
	}
}

void wr_Enable(void)
{
	Response(Error);
	Flag_Order = true;
}

void rd_Response(void)
{
	Response(Error);
}

void wr_Current(void)
{
	if(rd[SN_Color] == RedColor)
  	{
     	CurrentMaxim = 0x04D9; 
  	}
  	else if(rd[SN_Color] == BlueColor)
  	{
     	CurrentMaxim = 0x0953;
  	}
  	
  	CurrentMinum = 0x0011;
  	CurrentTuneValue = dec2hex(rd[9], rd[10]);
  	
  	if((CurrentTuneValue < CurrentMaxim) && (CurrentTuneValue > CurrentMinum) )
  	{
    	Response(Error);
  	}
  	else
  	{
    	Response(Error_wr_Current);
  	}

  	Flag_Order = true;
}

void wr_TEC(void)
{
	//
	Response(Error);
}

void wr_ID(void)
{
	uint16_t ID = 0x0000;
	ID = rd[SN_Data_1] ;
	ID = (ID  & 0x00FF)<<8;
  	ID = ID + (rd[SN_Data_2] & 0x00FF);
  
  	WR_EEPROM(ID,ID_ADDRESS);
  	delay_ms(3);
  	Response(Error);
}

void Response(uint8_t Error)
{
	int ByteNum = 0;
	uint8_t  CheckSum;
  	uint16_t ConverterData;
  
  	uint16_t ID = 0x0000;
  	uint8_t	ID_H = 0x00;
  	uint8_t	ID_L = 0x00;
  	ID = (*(__IO uint32_t*)(ID_ADDRESS));
  	delay_ms(3);
  	ID_H = (ID & 0xFF00)>>8;
  	ID_L = (ID & 0x00FF);
  
  	wd[0] = FrameHead;
	wd[1] = ID_H;
	wd[2] = ID_L;
	wd[3] = Reserved;
	wd[4] = COLOR;
	wd[5] = rd[SN_Order];
	wd[6] = DataLength_in_Response;
	
//   	if(ADC_Data[5] <0x0010 || ADC_Data[5] ==0x0010)
//   	{
//     	wd[7] = 0x00;
//   	}
//   	else 
//   	{
//     	wd[7] = 0x10;
//   	}
  	if(Flag_On == false)
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
  
//   wd[13]= (CurrentTuneValue & 0xFF00)>>8;
// 	wd[14]= (CurrentTuneValue & 0x00FF);
  	wd[13]= (ADC_Data[2] & 0xFF00)>>8;
  	wd[14]= (ADC_Data[2] & 0x00FF);
  
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



bool StartUp_Condition(void)
{
	if(Flag_Order == true)
	{
		if(ADC_Data[5] <=0x0010 && rd[SN_Order] ==0x01)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

}

bool ShutDown_Condition(void)
{
	if(Flag_Order == true)
	{
		if(ADC_Data[5] >0x0010 && rd[SN_Order] ==0x02)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool Tune_Condition(void)
{
	if(Flag_Order == true)
	{
		if(ADC_Data[5] >0x0010 && rd[SN_Order] ==Order_wr_Current && rd[SN_Data_1]==PCX && rd[SN_Data_2]==PCX)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
