#include "engine.hpp"
#include "util/delay.h"
#include "accelerometer.hpp"



uint16_t calculate_rotation(){
    _delay_ms(10);
    return 1;
}

void ENGINE_Init(){
    DDRB |= 1 << ENGINE_LEFT_DOWN_PIN | 1 << ENGINE_LEFT_UP_PIN 
         | 1 << ENGINE_RIGHT_DOWN_PIN | 1 << ENGINE_RIGHT_UP_PIN;
    DDRD |= 1 << ENGINE_ENABLE_PIN;

    ENGINE_DISABLE();
}

void move_forward(bool enable){
    move_stop();

    PORTB |= 1 << ENGINE_LEFT_UP_PIN | 1 << ENGINE_RIGHT_UP_PIN;

    if(enable)
        ENGINE_ENABLE();
}

void move_backward(bool enable){
    move_stop();
    
    PORTB |= 1 << ENGINE_LEFT_DOWN_PIN | 1 <<ENGINE_RIGHT_DOWN_PIN;

    if(enable)
        ENGINE_ENABLE();
}

void move_rotate(int16_t angle){
    // int16_t startAzi = COMPASS_getAzimuth();
    // startAzi += COMPASS_getAzimuth();
    // startAzi /= 2;
    if(angle >= 0){
        PORTB |= 1 << ENGINE_LEFT_UP_PIN | 1 << ENGINE_RIGHT_DOWN_PIN;
    }else{
        PORTB |= 1 << ENGINE_LEFT_DOWN_PIN | 1 << ENGINE_RIGHT_UP_PIN;
        // angle = -angle;
    }

    int16_t azi = COMPASS_getAzimuth();
    UART_print("Current azi: ");
    UART_println(azi);

    // int16_t overcome = 0;
    int16_t destination = azi + angle;
    if(destination >= 360)
        destination -= 360;
    else if(destination < 0)
        destination += 360;
    

    // Teoretycznie można spróbować z takim ustawieniem
    // int16_t padding = destination*1/100;
    // int16_t destinationMin = destination - padding;
    // int16_t destinationMax = destination + padding;

    ENGINE_ENABLE();
    // while(!(azi >= destinationMin && azi <= destinationMax)){
    while (azi != destination){
        _delay_ms(50);

        azi = COMPASS_getAzimuth();
        azi += COMPASS_getAzimuth();
        azi /= 2;

        // overcome = azi - startAzi;
        // UART_print("Azi: ");
        // UART_println(azi);
    }
    ENGINE_DISABLE();
    move_stop();

    UART_print("End azi: ");
    UART_println(azi);

    UART_print("Delta azi: ");
    UART_println(destination - azi);
}


void move_stop(){
    TIMER_stop();
    ENGINE_DISABLE();
    PORTB &= ~(1 << ENGINE_LEFT_UP_PIN | 1 << ENGINE_LEFT_DOWN_PIN
            | 1 << ENGINE_RIGHT_UP_PIN | 1 << ENGINE_RIGHT_DOWN_PIN);
}


void LEFT_forward(bool direction){
    if(direction){
        PORTB |= 1 << ENGINE_LEFT_UP_PIN;
    }else{
        PORTB |= 1 << ENGINE_LEFT_DOWN_PIN;
    }
}

void RIGHT_forward(bool direction){
    if(direction){
        PORTB |= 1 << ENGINE_RIGHT_UP_PIN;
    }else{
        PORTB |= 1 << ENGINE_RIGHT_DOWN_PIN;
    }
}