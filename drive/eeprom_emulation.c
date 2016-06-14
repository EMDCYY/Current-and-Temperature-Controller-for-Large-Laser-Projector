#include "eeprom_emulation.h"

__IO uint32_t ID_ADDRESS = 0x08007400;//Page 61: EEPROM emulation start address; after 61KBytes of used Flash memory
__IO uint32_t Current_Tune_ADDRESS = 0x08007800;// Page 62

uint32_t WR_EEPROM(uint16_t data, uint32_t address)
{
  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR |FLASH_FLAG_WRPERR);
  FLASH_ErasePage(address);
  FLASH_ProgramWord(address,data);
  FLASH_Lock();
  return (*(__IO uint32_t*)(address));
}
