#include "compass.hpp"


void COMPASS_Init(){
    // COMPASS_writeToRegister(COMPASS_PERIOD_REG, 0x01);
    COMPASS_writeToRegister(COMPASS_CONTROL_A_REG, 0b01010101);
    // COMPASS_writeToRegister(COMPASS_CONTROL_B_REG, 1<<6);
}

int16_t COMPASS_getAzimuth(){
    int16_t x, y, a;

    x = COMPASS_getAxis(COMPASS_X);
    y = COMPASS_getAxis(COMPASS_Y);
    COMPASS_getAxis(COMPASS_Z);

    // for(uint8_t i = 0; i < 2; i++){
        // x += I2C_read2Byte(I2C_ADR_COMPASS << 1, 0);
        // y += I2C_read2Byte(I2C_ADR_COMPASS << 1, 2);
        // I2C_read2Byte(I2C_ADR_COMPASS << 1, 4);
        // x = x/2;
        // y = y/2;
    // }

    // UART_print("x: ");
    // UART_print(x);
    // UART_print(" y: ");
    // UART_println(y);
    a = (atan2(y, x)*180/PI);
    // return a < 0 ? 360 + a : a;
    return a;
}