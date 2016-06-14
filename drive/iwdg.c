#include "iwdg.h"

void IWDG_Configuration(void)
{
  
  /* ʹ��д����IWDG_PR and IWDG_RLR �Ĵ��� */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  
  /* IWDG��������ʱ��: LSI/32 */
  IWDG_SetPrescaler(IWDG_Prescaler_64); // 40K / 64 = 0.625K = 1.6ms
  
  /* �����ص���������ֵ ���3s��IWDG��ʱ���԰�������ʽ���㣺  
     Counter Reload Value = 3s/IWDG counter clock period
                          = 3s / 1/(LSI/64)
                          = 3s / 1/(LsiFreq/64)
                          = 3s / 0.0016
                          = 1875
  
  */
  IWDG_SetReload(3750); // 1.6ms * 1875 = 3000ms
   /* ���µ���IWDG������ */
  IWDG_ReloadCounter();

  /* ʹ�� IWDG (LSI ��Ӳ��ʹ��) */
  IWDG_Enable();
}
