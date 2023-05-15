#ifndef __COMPASS__HPP
#define __COMPASS__HPP

#include <avr/io.h>
#include <util/delay.h>
#include "I2C.hpp"
#include <math.h>
#include "uart.hpp"
#define PI 3.1415926F

#define I2C_ADR_COMPASS 0x0D
#define COMPASS_X 0
#define COMPASS_Y 2
#define COMPASS_Z 4

#define COMPASS_CONTROL_A_REG 0x09
#define COMPASS_CONTROL_B_REG 0x0A
#define COMPASS_PERIOD_REG    0x0B
// #define 


// Read only status register
// bit 1 -- Data Ready Register
// bit 2 -- Overflow flag
// bit 3 -- Ski data for reading
#define COMPASS_STATUS_REG  0x06

// Number of sample to average in output:
// 0b00 - 1
// 0b01 - 2w
// 0b10 - 4
// 0b11 - 8
#define COMPASS_SAMPLING 0b10 << 5

// Measure range [Gauss]:
// 0b000 - ±0.88Ga
// 0b001 - ±1.3Ga (default)
// 0b100 - ±4.0Ga
#define COMPASS_RANGE 0b001 << 5

#define COMPASS_writeToRegister(address, data) I2C_writeToRegister(I2C_ADR_COMPASS << 1 + WRITE, address, data)
// #define COMPASS_readAxis(axis) I2C_read2Byte(I2C_ADR_COMPASS << 1, axis)
// #define COMPASS_read(address) I2C_readByte(I2C_ADR_COMPASS << 1, address)
#define COMPASS_read(address) I2C_readByte(I2C_ADR_COMPASS << 1, address)
// uint8_t COMPASS_read(uint8_t address);



void COMPASS_Init();
int16_t COMPASS_getAzimuth();

// int8_t COMPASS_getNorth();


#endif