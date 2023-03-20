#include "engine.hpp"
#include "util/delay.h"

#define LEFT_UP_PIN     1
#define LEFT_DOWN_PIN   0

#define RIGHT_UP_PIN    2
#define RIGHT_DOWN_PIN  4

#define ENABLE_PIN 7


uint16_t calculate_distance(){
    _delay_ms(10);
    return 1;
}


void ENGINE_Init(){
    DDRB |= 1 << LEFT_DOWN_PIN | 1 << LEFT_UP_PIN 
         | 1 << RIGHT_DOWN_PIN | 1 << RIGHT_UP_PIN;
    DDRD |= 1 << ENABLE_PIN;
}

void ENGINE_enable(bool enable){
    if(enable)
        PORTD |=  1 << ENABLE_PIN;
    else
        PORTD &= ~1 << ENABLE_PIN;
}

void move_forward(uint16_t distance){
    uint16_t measure = 0;
    PORTB |= 1 << LEFT_UP_PIN | 1 << RIGHT_UP_PIN;

    while(distance){
        measure += calculate_distance();
        if(distance == measure)
            move_stop();
            break;
    }
}


void move_stop(){
    PORTB &= ~(1 << LEFT_UP_PIN | 1 << LEFT_DOWN_PIN
            | 1 << RIGHT_UP_PIN | 1 << RIGHT_DOWN_PIN);
}


void LEFT_forward(uint16_t distance){
    uint16_t measure = 0;
    PORTB |= 1 << LEFT_UP_PIN;

    while (distance)
    {
        measure += calculate_distance();
        if(distance == measure){
            move_stop();
            break;
        }
    }   
}

void RIGHT_forward(uint16_t distance){
    uint16_t measure = 0;
    PORTB |= 1 << RIGHT_UP_PIN;

    while (distance)
    {
        measure += calculate_distance();
        if(distance == measure){
            move_stop();
            break;
        }
    }   
}