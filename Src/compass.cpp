#include "compass.hpp"


void COMPASS_Init(){
    COMPASS_writeToRegister(COMPASS_PERIOD_REG, 0x01);
    COMPASS_writeToRegister(COMPASS_CONTROL_A_REG, 0x1D);
}

int16_t COMPASS_getAzimuth(){
    int16_t x, y, a;

    x = I2C_read2Byte(I2C_ADR_COMPASS << 1, 0);
    y = I2C_read2Byte(I2C_ADR_COMPASS << 1, 2);
    I2C_read2Byte(I2C_ADR_COMPASS << 1, 4);

    a = (atan2(y, x)*180/PI);
    return a < 0 ? 360+a : a;
}