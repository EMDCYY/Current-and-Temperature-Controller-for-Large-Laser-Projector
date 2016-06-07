#include "eeprom_emulation.h"

__IO uint32_t PAGE0_BASE_ADDRESS = 0x0800F800;//Page 62: EEPROM emulation start address; after 62KBytes of used Flash memory

uint32_t WR_EEPROM(uint16_t data)
{
  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR |FLASH_FLAG_WRPERR);
  FLASH_ErasePage(PAGE0_BASE_ADDRESS);
  FLASH_ProgramWord(PAGE0_BASE_ADDRESS,data);
  FLASH_Lock();
  return (*(__IO uint32_t*)(PAGE0_BASE_ADDRESS));
}
