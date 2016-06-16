#include "delay.h" // 6 MHz

void delay_us(int k)
{
	int i, j;
  for(i=0;i<k;i++)
	  {
			for(j=0;j<6;j++);
		}
}
void delay_ms(int k)
{
	int i, j;
  for(i=0;i<k;i++)
	  {
			for(j=0;j<5999;j++);
		}
}
