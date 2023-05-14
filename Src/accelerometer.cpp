#include "accelerometer.hpp"

int16_t ACC_FIFO[8];
uint16_t ACC_time[8];
uint8_t FIFO_address = 0;


#if ACC_INTERRUPT_ENABLE
    ISR(INT0_vect){
        #if ACC_FIFO_ENABLE
            ACC_FIFORead();
            UART_print_char('\n');
        #endif
        if(FIFO_address < (uint8_t)sizeof(ACC_FIFO)/2){
            ACC_FIFO[FIFO_address] = ACC_readAxis(X_AXIS_REG);
            ACC_time[FIFO_address] = TCNT1;
            // UART_print(FIFO_address);
            // UART_print(" touch: ");
            // UART_println(ACC_FIFO[FIFO_address]);
            // uint8_t tmp = SREG;
            ++FIFO_address;
            // SREG = tmp;
        } 
        if (FIFO_address == (uint8_t)sizeof(ACC_FIFO)/2) {
            MOTION_DETECT_INT_OFF();
        }

    }
#endif



void ACC_Init(){
    #if ACC_INTERRUPT_ENABLE
        MCUCR |= (3 << ISC00);  //  zbocze narastające na INT0 wymusza przerwanie
        GICR  |= (1 << INT0);
    #endif

    ACC_RESET();
    ACC_WAIT_FOR_RESET();

    ACC_writeToRegister(ACC_ADR_RANGE_REG, RESOLUTION);
    ACC_writeToRegister(ACC_ADR_CONTROL_REG1, 0x0);

    // ACC_writeToRegister(ACC_ADR_Y_OFFSET_REG, 100);
    // ACC_writeToRegister(ACC_ADR_Z_OFFSET_REG, 100);

    ACC_writeToRegister(ACC_ADR_FREE_FALL_MOTION_CONTROL_REG, (1 << 6) | (0 << 5) | (0 << 4) | (1 << 3));
    ACC_writeToRegister(ACC_ADR_CONTROL_REG3, 0x0a);
    ACC_writeToRegister(ACC_ADR_CONTROL_REG4, 0x04);
    ACC_writeToRegister(ACC_ADR_CONTROL_REG5, 0x04);
    ACC_writeToRegister(ACC_ADR_FREE_FALL_MOTION_THRESHOLD_REG, 0x06);

    #if ACC_FIFO_ENABLE
        // FIFO:
        ACC_writeToRegister(ACC_ADR_FIFO_SETUP_REG, 3 << 6);
    #endif

    // _delay_ms(10);  
    ACC_writeToRegister(ACC_ADR_CONTROL_REG1, 0x05);
    
    ACC_writeToRegister(ACC_ADR_X_OFFSET_REG, ACC_readAxis(X_AXIS_REG));
    
    ACC_writeToRegister(ACC_ADR_CONTROL_REG1, 0x00);
    _delay_ms(20);
    ACC_writeToRegister(ACC_ADR_CONTROL_REG1, 0x05);
}


// uint8_t ACC_readRegister(uint8_t address){
//     uint8_t data;
//     I2C_beginTransition(I2C_ACCELEROMETER_WRITE);
//     I2C_write(address);
//     I2C_beginTransition(I2C_ACCELEROMETER_READ);
//     data = I2C_read_NAK();
//     I2C_endTransition();

//     return data;
// }

int16_t ACC_readAxis(uint8_t axis){
    int16_t data = I2C_read2Byte(I2C_ADR_ACCELEROMETER << 1, axis);
    // int16_t data;
    // ACC_writeToRegister(ACC_ADR_CONTROL_REG1, 0x01);
    // I2C_beginTransition(I2C_ACCELEROMETER_WRITE);
    // I2C_write(axis);
    // I2C_beginTransition(I2C_ACCELEROMETER_READ);
    // data = I2C_read_AK() << 8;
    // data |= I2C_read_NAK();


    return float(data)/STD_DEVIDER_ACC*STD_G*1000;
    // return data;
}

#if ACC_FIFO_ENABLE
    void ACC_FIFORead(){
        int16_t data;
        I2C_beginTransition(I2C_ACCELEROMETER_WRITE);
        I2C_write(0x01);
        I2C_beginTransition(I2C_ACCELEROMETER_READ);
        
        #define SAMPLE 16
        for(uint8_t i = 0; i < SAMPLE; i++){
            data = 0;
            data = I2C_read_AK() << 8;

            if(i == SAMPLE-1){
                data |= I2C_read_NAK();
            } else {
                data |= I2C_read_AK();
            }

            // if(i % 3 == 0){
            data = float(data)/STD_DEVIDER_ACC*STD_G*1000;
            UART_println(data);
            // }
        }
    }
#endif

// void ACC_writeToRegister(uint8_t address, uint8_t data){
//     I2C_beginTransition(I2C_ACCELEROMETER_WRITE);
//     I2C_write(address);
//     I2C_write(data);
//     I2C_endTransition();
// }



int16_t getVelocity(){
    // Test dla próbek:    
    // 500 0 0 0 0 0 ...
    FIFO_address = 0;
    while (GICR & (1 << INT0)); // czekaj na zebranie pomiarów
    
    int16_t velocity = ACC_FIFO[0]*ACC_time[0];
    for(uint8_t i = 1; i <= FIFO_address; i++){
        if((ACC_FIFO[i] & 0x80) != (ACC_FIFO[i-1] & 0x80) || ACC_FIFO[i] == 0)
            break;
        velocity += (int32_t)(ACC_FIFO[i]*(ACC_time[i]));//*(1000/F_CPU);
    }

    // UART_println(velocity);


    FIFO_address = 0;
    MOTION_DETECT_INT_ON();
    return velocity;
}