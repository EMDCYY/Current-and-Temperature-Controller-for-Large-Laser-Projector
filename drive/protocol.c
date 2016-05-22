#include <stdio.h>

__IO uint8_t  rd[15];


void Calculate_Length(void)
{
	int byte_count = 0;
	while( rd[byte_count] £¡= 0x16) 
	{
		byte_count++;
	}
	return byte_count;
}

void Check_Length(void)
{
	int byte_count = Calculate_Length();
	if((byte_count-3) == rd[byte_count-3])
	{
			Check_Bit();
	}
	else
	{
			//error;
	}
}


void Check_Bit(void)
{
	int byte_count;
	__IO uint8_t  checksum;
	for(byte_count = 1; byte_count <  Calculate_Length()-2; byte_count++)
	{
		checksum = rd[byte_count]^checksum;
	}
	if(checksum == rd[Calculate_Length()-3)
	{
		;
	}
	else
	{
		;
	}
}