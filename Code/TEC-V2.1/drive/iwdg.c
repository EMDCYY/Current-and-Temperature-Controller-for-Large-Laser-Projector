#include "iwdg.h"

void IWDG_Configuration(void)
{
  
  /* 使能写访问IWDG_PR and IWDG_RLR 寄存器 */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  
  /* IWDG计数器的时钟: LSI/32 */
  IWDG_SetPrescaler(IWDG_Prescaler_64); // 40K / 64 = 0.625K = 1.6ms
  
  /* 设置重导计数器的值 获得3s的IWDG超时可以按下面算式计算：  
     Counter Reload Value = 3s/IWDG counter clock period
                          = 3s / 1/(LSI/64)
                          = 3s / 1/(LsiFreq/64)
                          = 3s / 0.0016
                          = 1875
  
  */
  IWDG_SetReload(3750); // 1.6ms * 1875 = 3000ms
   /* 从新导入IWDG计数器 */
  IWDG_ReloadCounter();

  /* 使能 IWDG (LSI 被硬件使能) */
  IWDG_Enable();
}
