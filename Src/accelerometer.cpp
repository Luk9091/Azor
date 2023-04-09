#include "accelerometer.hpp"

uint8_t turnOffINT = 0;

ISR(INT0_vect){
    // UART_println("Motion detect!");

    uint8_t read = ACC_readRegister(0x16);
    // if(read & (1 << 1)){
    //     UART_println("Motion detect");
    // }
    if(read & (1 << 0)){
        UART_print_char('N');
    } else {
        UART_print_char('P');
    }
    UART_print_char('\n');

}



void ACC_Init(){
    MCUCR |= (3 << ISC00);  //  zbocze narastające na INT0 wymusza przerwanie
    GICR  |= (1 << INT0);
    ACC_RESET();
    ACC_WAIT_FOR_RESET();

    ACC_writeToRegister(ACC_ADR_RANGE_REG, RESOLUTION);

    ACC_writeToRegister(ACC_ADR_CONTROL_REG1, 0x0);

    // ACC_writeToRegister(ACC_ADR_X_OFFSET_REG, 100);
    // ACC_writeToRegister(ACC_ADR_Y_OFFSET_REG, 100);
    // ACC_writeToRegister(ACC_ADR_Z_OFFSET_REG, 100);

    ACC_writeToRegister(ACC_ADR_FREE_FALL_MOTION_CONTROL_REG, (1 << 6) | (0 << 5) | (0 << 4) | (1 << 3));
    ACC_writeToRegister(ACC_ADR_CONTROL_REG3, 0x0a);
    ACC_writeToRegister(ACC_ADR_CONTROL_REG4, 0x04);
    ACC_writeToRegister(ACC_ADR_CONTROL_REG5, 0x04);
    ACC_writeToRegister(ACC_ADR_FREE_FALL_MOTION_THRESHOLD_REG, 0x06);

    // ACC_writeToRegister(ACC_ADR_FILTER_REG, 3 << 0);

    // _delay_ms(10);  
    ACC_writeToRegister(ACC_ADR_CONTROL_REG1, 0x05);
}


uint8_t ACC_readRegister(uint8_t address){
    uint8_t data;
    I2C_beginTransition(I2C_ACCELEROMETER_WRITE);
    I2C_write(address);
    I2C_beginTransition(I2C_ACCELEROMETER_READ);
    data = I2C_read_NAK();
    I2C_endTransition();

    return data;
}

int16_t ACC_readAxis(uint8_t axis){
    int16_t data = 0;

    // ACC_writeToRegister(ACC_ADR_CONTROL_REG1, 0x01);
    I2C_beginTransition(I2C_ACCELEROMETER_WRITE);
    I2C_write(axis);
    I2C_beginTransition(I2C_ACCELEROMETER_READ);
    data = I2C_read_AK() << 8;
    data |= I2C_read_NAK();

#if RESOLUTION == 0
    data >>= 2;
#elif RESOLUTION == 1
    // NOTHING
#elif RESOLUTION == 2
    data <<= 2;
#endif

    return float(data)/STD_DEVIDER_ACC*STD_G*1000;
    // return data;
}

void ACC_writeToRegister(uint8_t address, uint8_t data){
    I2C_beginTransition(I2C_ACCELEROMETER_WRITE);
    I2C_write(address);
    I2C_write(data);
    I2C_endTransition();

    // _delay_ms(10);
}



int16_t ACC_calculateToACC(uint16_t readValue){
    
}