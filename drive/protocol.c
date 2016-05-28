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
#include "stm32f0xx.h"
#include "string.h"
#include ¡°stdbool.h¡±

#define ID			0x0005
#define COLOR		0x0A   //0x0A - Red; 0x0B - Green; 0x0C - Blue; 0x00 - Check
#define	FrameHead	0xAC
#define	FrameTail	0x16
#define Rsvserved	0xFF
#define DataLength_in_Response 	0x0A
#define StartUp_in_Response		0x01
#define ShutDown_in_Response	0x00
#define NoWarning_in_Response 	0x0C

uint8_t	ID_H = (ID & 0xFF00)>>8;
uint8_t	ID_L = (ID & 0x00FF);
uint8_t Problem;

extern	__IO	uint8_t rd[19];
		__IO	uint8_t	wd[19];

extern uint16_t InputCurrent;
extern uint16_t OutputCurrent;
extern uint16_t Temperature1;
extern uint16_t Temperature2;

extern __O 	bool  state;




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



void First_Check_Frame	(void);
void Second_Check_Length(void);
void Third_Check_Bit	(void);
void Fourth_Check_ID	(void);
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
	First_Check_Frame();
}

void First_Check_Frame(void)
{
	if (rd[0] == FrameHead && (rd[12] == FrameTail || rd[18] == FrameTail))
	{
		Second_Check_Length();
	}
	else
	{
    	Response(0xE0);
	}
}

void Second_Check_Length(void)
{
	if((rd[12]==FrameTail && rd[6]==4) || (rd[18]==FrameTail && rd[6]==10))
	{
		Third_Check_Bit();
	}
	else
	{
    	Response(0xE6);
	}
}

void Third_Check_Bit(void)
{
	int ByteNum;
	uint8_t  CheckSum;
	for(ByteNum=1; ByteNum<rd[6]+5; ByteNum++)
	{
		CheckSum = rd[ByteNum]^CheckSum;
	}
	if(rd[11] == CheckSum || rd[17] == CheckSum)
  	{
  		Fourth_Check_ID();
  		ByteNum = 0;
  		CheckSum = 0x00;
  	}
	else
	{
    	Response(0xCD);
	}
}


void Fourth_Check_ID(void)
{
	ID_H =	(ID & 0xFF00)>>8;
	ID_L =	(ID & 0x00FF);
  	if (rd[1]==ID_H && rd[2]==ID_L)
	{
		Fifth_Check_Rsvserved();
	}
	else
	{
    	Response(0xE1);
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
		; //Response without Preconditions
	}
	else
	{
    	Response(0xE4);
	}
}

void rd_Operate(void)
{
	if (rd[5]==0x01 || rd[5]==0x02)
	{
		wr_Enable();
	}
	
	else if (rd[5]==0x03)
	{
		rd_Response();
	}
		
	else if (rd[5]==0x04)
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
		Dir_Transmit();
    	delay_ms(1);	
    	USART_SendData(USART1, 0xE5);
    	delay_ms(2);	
    	Dir_Receive();
    	Response(0xE5);
	}
}

void wr_Enable(void)
{
	if (state == True && rd[5] ==0x01)
	{
		Response(0x0C);
	}
	else if (state == True && rd[5] ==0x02)
	{
		ShutDown();
		Response(0x0C);
	}
	else if (state == False && rd[5] ==0x01)
	{
		ShutDown();
		Response(0x0C);
	}
	else if (state == False && rd[5] ==0x02)
	{
		Response(0x0C);
	}
}

void rd_Response(void)
{
	Response();
}

void wr_Current(void)
{
	if(rd[7]==0x00 && rd[8]==0x00)
	{
		ctrl = (rd[9]*10+rd[10])/200*4095;

	}
	Response();
}

void wr_TEC(void)
{
	//
	Response();
}

void wr_ID(void)
{
	//FLASH
}

void Response(uint8_t Problem)
{
	int ByteNum;
  int i;
	uint8_t  CheckSum;
  wd[0] = FrameHead;
	wd[1] = ID_H;
	wd[2] = ID_L;
	wd[3] = Rsvserved;
	wd[4] = COLOR;
	wd[5] = rd[5];
	wd[6] = DataLength_in_Response;
	wd[7] = StartUp_in_Response;//
	wd[8] = Problem;//
	wd[9] = (InputCurrent & 0xFF00)>>8;
	wd[10]= (InputCurrent & 0x00FF);
	wd[11]= (OutputCurrent & 0xFF00)>>8;
	wd[12]= (OutputCurrent & 0x00FF);
	wd[13]= (Temperature1 & 0xFF00)>>8;
	wd[14]= (Temperature1 & 0x00FF);
	wd[15]= (Temperature2 & 0xFF00)>>8;
	wd[16]= (Temperature2 & 0x00FF);

	for(ByteNum=1; ByteNum < DataLength_in_Response + 5; ByteNum++)
	{
		CheckSum = wd[ByteNum]^CheckSum;
	}
	wd[17]= CheckSum;
	wd[18]= FrameTail;

	Dir_Transmit();
    delay_ms(1);
    for(i=0; i < 19; i++)	
    {
    	USART_SendData(USART1, wd[i]);
    }
    delay_ms(2);	
    Dir_Receive();
}
