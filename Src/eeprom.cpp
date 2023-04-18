#include "eeprom.hpp"
#include "uart.hpp"

void EEPROM_I2C_write(uint16_t address, uint8_t data){
    I2C_beginTransition(EEPROM_I2C_WRITE);
    I2C_write(address>>8);
    I2C_write(address);
    I2C_write(data);
    I2C_endTransition();
}

uint8_t EEPROM_I2C_read(uint16_t address){
    uint8_t data;
    I2C_beginTransition(EEPROM_I2C_WRITE);
    I2C_write(address>>8);
    I2C_write(address);
    I2C_beginTransition(EEPROM_I2C_READ);
    data = I2C_read_NAK();
    I2C_endTransition();

    return data;
}