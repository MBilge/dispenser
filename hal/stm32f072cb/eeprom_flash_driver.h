#ifndef __EEPROM_FLASH_DRIVER__
#define __EEPROM_FLASH_DRIVER__

// 1K for stm32f072
#define EEPROM_EMU_BANK_SIZE   (FLASH_PAGE_SIZE*1)
#define EEPROM_EMU_FLASH_START (FLASH_BANK1_END + 1 - EEPROM_EMU_BANK_SIZE*2)

#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"

class EepromFlashDriver
{
public:
    enum { BankSize = EEPROM_EMU_BANK_SIZE };

    void erase(uint8_t bank)
    {
        FLASH_EraseInitTypeDef s_eraseinit;
        s_eraseinit.TypeErase   = FLASH_TYPEERASE_PAGES;
        s_eraseinit.PageAddress = EEPROM_EMU_FLASH_START + bank*EEPROM_EMU_BANK_SIZE;
        s_eraseinit.NbPages     = EEPROM_EMU_BANK_SIZE / FLASH_PAGE_SIZE;
        uint32_t page_error = 0;

        HAL_FLASH_Unlock();
        HAL_FLASHEx_Erase(&s_eraseinit, &page_error);
        HAL_FLASH_Lock();
    }

    uint16_t read_u16(uint8_t bank, uint16_t addr)
    {
        void *flash_ptr = (void *)(EEPROM_EMU_FLASH_START + bank*EEPROM_EMU_BANK_SIZE + addr);

        // Access is aligned. Pointers use is safe.
        //uint16_t val;
        //memcpy(&val, flash_ptr, 2);
        //return val;

        return *(uint16_t *)flash_ptr;
    }

    void write_u16(uint8_t bank, uint16_t addr, uint16_t data)
    {
        uint32_t flash_addr = EEPROM_EMU_FLASH_START + bank*EEPROM_EMU_BANK_SIZE + addr;

        HAL_FLASH_Unlock();
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, flash_addr, data);
        HAL_FLASH_Lock();
    }
};

#endif
