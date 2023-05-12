#include "mapping.hpp"



void runAndMeasure(int16_t destination){
    int16_t distance = 0;
    int16_t velocity = 0;
    TIMER_set(255);

    if(destination == 0)
        return;
    move_forward(destination > 0, false);
    destination *= 1000;

    TIMER_start();
    ENGINE_ENABLE();

    velocity = getVelocity()/10;
    if(velocity & 0x8000 != destination & 0x8000 || velocity == 0){
        move_stop();
        UART_print("Er v:");
        UART_println(velocity);
        return;
    }
    UART_print("v:");
    UART_println(velocity);

    velocity = abs(velocity);
    while (distance < destination){
        distance = (TIMER_getValue()/8)*velocity;
    }
    move_stop();
    TIMER_stop();
    
    UART_print("Dis: ");
    UART_println(distance);
    UART_print("T: ");
    UART_println(TIMER_get_us());

    


    // map();
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