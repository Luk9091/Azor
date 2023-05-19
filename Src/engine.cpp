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

void move_forward(bool direction, bool enable){
    ENGINE_DISABLE();
    TIMER_set(255);
    COUNTER_clear();
    TIMER_start();
    if(direction){
        PORTB |= 1 << ENGINE_LEFT_UP_PIN | 1 << ENGINE_RIGHT_UP_PIN;
    }else{
        PORTB |= 1 << ENGINE_LEFT_DOWN_PIN | 1 <<ENGINE_RIGHT_DOWN_PIN;
    }
    if(enable)
    ENGINE_ENABLE();
}

void move_rotate(int16_t angle){
    int16_t measure = COMPASS_getAzimuth();
    if(angle >= 0){
        PORTB |= 1 << ENGINE_LEFT_UP_PIN | 1 << ENGINE_RIGHT_DOWN_PIN;
    }else{
        PORTB |= 1 << ENGINE_LEFT_DOWN_PIN | 1 << ENGINE_RIGHT_UP_PIN;
        // angle = -angle;
    }
    angle = abs(angle);

    while(abs(COMPASS_getAzimuth()-measure) >= angle){
        ENGINE_ENABLE();
        _delay_ms(10);
        ENGINE_DISABLE();
    }
    move_stop();
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