#include "converter.h"
#include "stm32f0xx.h"
#include "string.h"
#include "stdbool.h"

int hex2dec(uint16_t data)
{
//   uint16_t DecValue_H;
//   uint16_t DecValue_M;
//   uint16_t DecValue_L;
//   
//   DecValue_H = data * 0x14A / 0xFFF / 0x64; //Minimun Current is 0.01A, Maxmum Current is 3.3A. Make data amplify to 330 can get integra after induce 0xFFF 
//   DecValue_M = data * 0x14A / 0xFFF / 0xA;
//   DecValue_L = data * 0x14A / 0xFFF % 0xA;
  
//   return ((DecValue_H << 8) & 0x0F00) + ((DecValue_M << 4) & 0x00F0) + (DecValue_L & 0x000F);
  
  uint16_t DecValue_H;
  uint16_t DecValue_L;
  
  DecValue_H = data * 0x14A / 0xFFF / 0x64; //Minimun Current is 0.01A, Maxmum Current is 3.3A. Make data amplify to 330 can get integra after induce 0xFFF 
  DecValue_L = data * 0x14A / 0xFFF % 0x64;
  
  return ((DecValue_H << 8) & 0x0F00) + (DecValue_L  & 0x00FF);
}

int dec2hex(uint8_t data_H, uint8_t data_L)
{
  uint16_t HexValue_H;
  uint16_t HexValue_L;
  HexValue_H = data_H * 0x64 * 0xFFF / 0x14A;
  HexValue_L = data_L * 0xFFF / 0x14A; 
  return HexValue_L + HexValue_H;
}
