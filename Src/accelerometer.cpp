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
    while(ACC_readRegister(ACC_ADR_CONTROL_REG2) & (1 << 6)); // poczekaj na zakończenie resetu

    ACC_writeToRegister(ACC_ADR_FREE_FALL_MOTION_CONTROL_REG, (1 << 6) | (0 << 5) | (0 << 4) | (1 << 3)); // Motion detect, 
    ACC_writeToRegister(ACC_ADR_CONTROL_REG3, (1 << 3)); // when motion wake up
    ACC_writeToRegister(ACC_ADR_CONTROL_REG4, (1 << 2)); // Wyzwolenie przerwania od wykrycia ruchu
    ACC_writeToRegister(ACC_ADR_CONTROL_REG5, (1 << 2)); // Przekierowanie przerwania na INT1

    // Uwaga aby całość działa musi istnieć jakiś threshold
    ACC_writeToRegister(ACC_ADR_FREE_FALL_MOTION_THRESHOLD_REG, 0x08);

    // Ustawienie rejestru FIFO
    // ACC_writeToRegister(ACC_ADR_FIFO_SETUP_REG, (3 << 6) | (1 << 5));

    ACC_writeToRegister(ACC_ADR_FILTER_REG, 0 << 4 | 3 << 0); // Powolny pomiar


    ACC_writeToRegister(ACC_ADR_RANGE_REG, RESOLUTION); // Ustaw zakres
    ACC_writeToRegister(ACC_ADR_CONTROL_REG1,  (0x04 | 0x02 | 0x01)); //Reduce noise, fast read, active mode
}


uint8_t ACC_readRegister(uint8_t address){
    uint8_t data;
    I2C_beginTransition(I2C_ACCELEROMETER_WRITE);
    I2C_write(address);
    I2C_beginTransition(I2C_ACCELEROMETER_READ);
    data = I2C_read();
    I2C_endTransition();

    return data;
}

int16_t ACC_readAxis(uint8_t axis){
    if(axis > 2)
        return 0;

    axis = (axis + 1)*2;//(0+1)*2 = 2

    int16_t data = 0;

    data = ACC_readRegister(axis-1) << 8; // ADR = 2-1 = 1
    data += ACC_readRegister(axis); // ADR = 2
    data >>= 2; 
    data &= 0b0011111111111111;

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