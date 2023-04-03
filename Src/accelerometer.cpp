#include "accelerometer.hpp"

uint8_t turnOffINT = 0;

ISR(INT0_vect){
    cli();
    // if(turnOffINT){
    int16_t X_axis = ACC_readAxis(X_AXIS_REG);
        MOTION_DETECT_INT_OFF();
    UART_print(X_axis, 16);
    UART_println(" x axis");
    // }
    sei();
}



void ACC_Init(){
    MCUCR |= (3 << ISC00);  //  zbocze narastające na INT0 wymusza przerwanie
    ACC_RESET();
    ACC_WAIT_FOR_RESET();
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

uint16_t ACC_readAxis(uint8_t axis){
    if(axis > 2)
        return 0;

    axis = (axis + 1)*2;//(0+1)*2 = 2   
    uint16_t data = 0;

    // data = ACC_readRegister(axis-1) << 8; // ADR = 2-1 = 1
    // data += ACC_readRegister(axis); // ADR = 2

    I2C_beginTransition(I2C_ACCELEROMETER_WRITE);
    I2C_write(axis-1);
    I2C_beginTransition(I2C_ACCELEROMETER_READ);
    data = I2C_read_AK() << 8;
    data = I2C_read_NAK();

    data >>= 2;
    // if(data < 0)
    //     data = data * (-1);

    return data;
}

void ACC_writeToRegister(uint8_t address, uint8_t data){
    I2C_beginTransition(I2C_ACCELEROMETER_WRITE);
    I2C_write(address);
    I2C_write(data);
    I2C_endTransition();
}



int16_t ACC_calculateToACC(uint16_t readValue){
    
}