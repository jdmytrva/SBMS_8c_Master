#ifndef _EE_
#define _EE_
#include <stm32f1xx.h>
#include "General.h"


uint8_t flash_ready(void);
void flash_erase_page(uint32_t address);
void flash_unlock(void);
void flash_lock();
void flash_write(uint32_t address,uint32_t data);
uint32_t flash_read(uint32_t address, uint32_t MinValue, uint32_t MaxValue);

uint8_t ReadFromFlash(void);
uint8_t DataWhenPowerOffWriteToFlash_CRC(void);
uint8_t SettingsWriteToFlash_CRC(void);
uint8_t CalibrationWriteToFlash_CRC(void);
uint8_t FactoryWriteToFlash_CRC(void);
uint8_t CalcCRC8(uint8_t *Ptr, uint8_t Num, uint8_t CRC1);




void CalibrationWriteToFlash(void);
void CalibrationFactoryWriteToFlash(void);
void CalibrationReadFromFlash(void);
void CalibrationReadFromFlashForVerify(void);
void SettingsWriteToFlash(void);
void SettingsFactoryWriteToFlash(void);
void SettingsReadFromFlash(void);
void SettingsReadFromFlashForVerify(void);
void DataWhenPowerOffWriteToFlash(void);
void DataWhenPowerOff_Factory_WriteToFlash(void);
void DataWhenPowerOffReadFromFlash(void);
void DataWhenPowerOffReadFromFlashForVerify(void);

uint8_t flash_write_block();
uint8_t flash_read_block();









#endif
