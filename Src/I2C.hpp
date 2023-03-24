#ifndef __I2C__HPP
#define __I2C__HPP

// Nie każdy procesor jest w stanie odbierać kilka bajtów :C
#define MULTI_READ 0

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/twi.h>
#include "uart.hpp"

#define READ 1
#define WRITE 0

#define I2C_ADR_ACCELEROMETER 0x1C
#define I2C_ACCELEROMETER_WRITE I2C_ADR_ACCELEROMETER << 1 + WRITE
#define I2C_ACCELEROMETER_READ  I2C_ADR_ACCELEROMETER << 1 + READ


void I2C_Init();

void I2C_beginTransition(uint8_t address);
void I2C_endTransition();

void I2C_write(uint8_t data);

#if MULTI_READ
uint8_t I2C_read(uint8_t count = 0);

//  ATmega8 nie jest w stanie czytać kilku bajtów na raz! 
// Więc za każdym razem odwołujemy się do rejestru!
uint8_t I2C_read_AK();
uint8_t I2C_read_NAK();

#else
uint8_t I2C_read();
#endif




#endif