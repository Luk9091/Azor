#include "engine.hpp"
#include "util/delay.h"
#include "accelerometer.hpp"



uint16_t calculate_distance(){
    _delay_ms(10);
    // MOTION_DETECT_INT_ON();
    return 1;
}

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

void ENGINE_enable(bool enable){
    if(enable){
        PORTD |=  1 << ENGINE_ENABLE_PIN;
    }else{
        PORTD &= ~(1 << ENGINE_ENABLE_PIN);
    }
}

void move_forward(bool direction, bool enable){
    ENGINE_DISABLE();
    if(direction){
        PORTB |= 1 << ENGINE_LEFT_UP_PIN | 1 << ENGINE_RIGHT_UP_PIN;
    }else{
        PORTB |= 1 << ENGINE_LEFT_DOWN_PIN | 1 <<ENGINE_RIGHT_DOWN_PIN;
    }
    if(enable)
    ENGINE_ENABLE();
}

void move_rotate(int8_t angle){
    uint8_t measure = 0;
    ENGINE_DISABLE();
    if(angle >= 0){
        PORTB |= 1 << ENGINE_LEFT_DOWN_PIN | 1 << ENGINE_RIGHT_UP_PIN;
    }else{
        PORTB |= 1 << ENGINE_LEFT_UP_PIN | 1 << ENGINE_RIGHT_DOWN_PIN;
        angle = ~angle + 1;
    }
    ENGINE_ENABLE();

    while(angle){
        measure += calculate_distance();
        if(angle == measure){
            move_stop();
            break;
        }
    }
}


void move_stop(){
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