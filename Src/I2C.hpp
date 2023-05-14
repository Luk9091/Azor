#ifndef __I2C__HPP
#define __I2C__HPP

// Nie każdy procesor jest w stanie odbierać kilka bajtów :C
#define MULTI_READ 1

#include <avr/io.h>
#include <util/twi.h>

#define READ 1
#define WRITE 0

void I2C_Init();

void I2C_writeToRegister(uint8_t deviceAddress, uint8_t registerAddress, uint8_t data);
int8_t I2C_readByte(uint8_t deviceAddress, uint8_t registerAddress);
int16_t I2C_read2Byte(uint8_t deviceAddress, uint8_t registerAddress);


void I2C_beginTransition(uint8_t address);
void I2C_endTransition();

void I2C_write(uint8_t data);

#if MULTI_READ
// void I2C_read(uint8_t *buffer, uint8_t bufferSize = 1);

uint8_t I2C_read_AK();
uint8_t I2C_read_NAK();
#else

uint8_t I2C_read();
#endif





#endif