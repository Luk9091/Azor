#include "mapping.hpp"



void runAndMeasure(int16_t destination){
    int16_t distance = 0;
    int16_t velocity = 0;
    
    TIMER_set(255);

    if(destination == 0)
        return;
    move_forward(destination > 0, false);
    destination = abs(destination);
    uint32_t destinationMul = destination*1000;
    // destination *= 1000;

    MOTION_DETECT_INT_ON();
    COUNTER_clear();
    TIMER_start();

    ENGINE_ENABLE();

    velocity = abs(getVelocity_withACC());
    // if(velocity & 0x8000 != destination & 0x8000 || velocity == 0){
    //     move_stop();
    //     UART_print("Er v:");
    //     UART_println(velocity);
    //     return;
    // }
    UART_print("v:");
    UART_println(velocity);
    // UART_print("Count: ");
    // UART_println(COUNTER_read());

    // velocity = abs(velocity);
    while (COUNTER_read()/4 < destination && velocity*TIMER_getValue()/8 < destinationMul){
        // Jeśli jedna z prędkości wykaże bezedurę to stop
    }
    move_stop();
    
    UART_print("Counter: ");
    UART_println(COUNTER_read());
    UART_print("T: ");
    UART_println(TIMER_getValue()/8);
    COUNTER_clear();
    TIMER_clear();
}


void map(int16_t addressToStore){
    PWM_setAngle(0);
    _delay_ms(50);

    for(uint8_t i = 0; i <= 180; i += 3){
        PWM_setAngle(i);
        _delay_ms(2);
        UART_print(i);
        UART_print("dec: ");
        _delay_ms(10);
        uint16_t data = SONIC_measure();
        UART_println(data);


        if(addressToStore != -1){
            EEPROM_I2C_write(addressToStore, data);
            ++addressToStore;
        }
    }
    PWM_setAngle(90);
}