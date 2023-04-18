#ifndef __I2C_EEPROM__HPP
#define __I2C_EEPROM__HPP

#include <avr/io.h>
#include "I2C.hpp"

#define EEPROM_I2C_ADDRESS 0x50
#define EEPROM_I2C_WRITE (EEPROM_I2C_ADDRESS << 1) + WRITE
#define EEPROM_I2C_READ  (EEPROM_I2C_ADDRESS << 1) + READ



void EEPROM_I2C_write(uint16_t address, uint8_t data);
uint8_t EEPROM_I2C_read(uint16_t address);


#endif