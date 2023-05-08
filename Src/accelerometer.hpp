#ifndef __ACCELEROMETER__HPP
#define __ACCELEROMETER__HPP

#define ACC_INTERRUPT_ENABLE true
#define ACC_FIFO_ENABLE false




#define I2C_ADR_ACCELEROMETER 0x1C
#define I2C_ACCELEROMETER_WRITE (I2C_ADR_ACCELEROMETER << 1) + WRITE
#define I2C_ACCELEROMETER_READ  (I2C_ADR_ACCELEROMETER << 1) + READ

#define X_AXIS_REG 1 // Nazwy na podstawie LSB reg
#define Y_AXIS_REG 3
#define Z_AXIS_REG 5

#define STD_G 9.80665F


// Resolution value:
//      0 == ±2g
//      1 == ±4g
//      2 == ±8g
#define RESOLUTION 1
#define ACC_ADR_RANGE_REG 0x0E

// Resolution devider:
#if   RESOLUTION == 0
    #define STD_DEVIDER_ACC 32768.0F
#elif RESOLUTION == 1
    #define STD_DEVIDER_ACC 8192.0F
#elif RESOLUTION == 2
    #define STD_DEVIDER_ACC 2048.0F
#endif

// Offset reg:
#define ACC_ADR_X_OFFSET_REG 0x2F
#define ACC_ADR_Y_OFFSET_REG 0x30
#define ACC_ADR_Z_OFFSET_REG 0x31


#define ACC_ADR_CONTROL_REG1 0x2A
#define ACC_ADR_CONTROL_REG2 0x2B

// Interrupt could wake up register
#define ACC_ADR_CONTROL_REG3 0x2C

// Interrupt source register
#define ACC_ADR_CONTROL_REG4 0x2D

// Interrupt pin register
#define ACC_ADR_CONTROL_REG5 0x2E

// 6bit -- FreeFall/MotionDetect
// 5bit -- Z axis
// 4bit -- Y axis
// 3bit -- X axis
#define ACC_ADR_FREE_FALL_MOTION_CONTROL_REG 0x15
#define ACC_ADR_FREE_FALL_MOTION_THRESHOLD_REG 0x17

// FIFO:

// FIFO setup register
//  7:6b -- FIFO mode
//      00 -- disable
//      01 -- circular buffer
//      10 -- stop accept new sample when overflow
//      11 -- trigger mode
//  5:0 -- max value of sample
#define ACC_ADR_FIFO_SETUP_REG 0x09

// READ ONLY
// Rejestr stanu FIFO
//  7bit -- overflow
//  6bit -- watermark -- cokolwiek to znaczy
//  5:0b -- FIFO pointer -- start 0x01 end 0x20
#define ACC_ADR_FIFO_STATUS_REG 0x00

#define ACC_ADR_FILTER_REG 0x0F

#define ACC_RESET() ACC_writeToRegister(ACC_ADR_CONTROL_REG2, 0x04)
#define ACC_WAIT_FOR_RESET() while(ACC_readRegister(ACC_ADR_CONTROL_REG2) & 0x40)
#define ACC_STOP_AND_CLEAR() ACC_writeToRegister(ACC_ADR_CONTROL_REG1, 0)

#define MOTION_DETECT_INT_ON()   GICR  |= (1 << INT0)
#define MOTION_DETECT_INT_OFF()  GICR  &=~(1 << INT0)

#include <avr/io.h>
#include "I2C.hpp"
#include "uart.hpp"
#include "timer.hpp"

void ACC_Init();

uint8_t ACC_readRegister(uint8_t address);

int16_t ACC_readAxis(uint8_t axis);
int16_t ACC_calculateToACC(int16_t readValue);


void ACC_writeToRegister(uint8_t address, uint8_t data);
#if ACC_FIFO_ENABLE
    void ACC_FIFORead();
#endif



#endif