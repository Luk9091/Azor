#include "compass.hpp"


void COMPASS_Init(){
    COMPASS_writeToRegister(COMPASS_PERIOD_REG, 0x01);
}

int16_t COMPASS_measureAxis(uint8_t axis){
    int16_t data = I2C_read2Byte(I2C_ADR_COMPASS << 1, axis);
    return data;
}

// uint8_t COMPASS_getNorth()