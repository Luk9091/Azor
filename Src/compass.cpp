#include "compass.hpp"

COMPASS_AXIS COMPASS_axis;

void COMPASS_Init(){
    COMPASS_writeToRegister(COMPASS_PERIOD_REG, 0x01);
    COMPASS_writeToRegister(COMPASS_CONTROL_A_REG, 0x01|0x0C|0x10|0x00);
    // COMPASS_writeToRegister(COMPASS_CONTROL_B_REG, 1<<6);
}


int16_t COMPASS_getAxis(){
    I2C_beginTransition((I2C_ADR_COMPASS << 1) + WRITE);
    I2C_write(COMPASS_X);
    I2C_beginTransition((I2C_ADR_COMPASS << 1) + READ);
    
    COMPASS_axis.x = (I2C_read_AK()) | (I2C_read_AK() << 8);
    COMPASS_axis.y = (I2C_read_AK()) | (I2C_read_AK() << 8);
    COMPASS_axis.z = (I2C_read_AK()) | (I2C_read_NAK() << 8);

    I2C_endTransition();

#if COMPASS_CALIBRATION
    COMPASS_axis.x = (COMPASS_axis.x - _COMPASS_x_offset)*_COMPASS_x_scale;
    COMPASS_axis.y = (COMPASS_axis.y - _COMPASS_y_offset)*_COMPASS_y_scale;
    COMPASS_axis.z = (COMPASS_axis.z - _COMPASS_z_offset)*_COMPASS_z_scale;
#endif
}


int16_t COMPASS_getAzimuth(){
    int16_t a;
    COMPASS_getAxis();
    
    a = (atan2(COMPASS_axis.y, COMPASS_axis.x) * 180/PI);
    return a < 0 ? 360 + a : a;
}