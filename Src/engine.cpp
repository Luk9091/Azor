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

uint16_t calculate_rotation(){
    _delay_ms(10);
    return 1;
}

void ENGINE_Init(){
    DDRB |= 1 << LEFT_DOWN_PIN | 1 << LEFT_UP_PIN 
         | 1 << RIGHT_DOWN_PIN | 1 << RIGHT_UP_PIN;
    DDRD |= 1 << ENABLE_PIN;
}

void ENGINE_enable(bool enable){
    if(enable){
        PORTD |=  1 << ENABLE_PIN;
    }else{
        PORTD &= ~1 << ENABLE_PIN;
    }
}

void move_forward(int8_t distance){
    uint8_t measure = 0;
    if(distance >= 0){
        PORTB |= 1 << LEFT_UP_PIN | 1 << RIGHT_UP_PIN;
    }else{
        PORTB |= 1 << LEFT_DOWN_PIN | 1 <<RIGHT_DOWN_PIN;
        distance = ~distance+1;
    }
    UART_println(distance);

    while(distance){
        measure += calculate_distance();
        if(distance == measure){
            move_stop();
            break;
        }
    }
}

void move_rotate(int8_t angle){
    uint8_t measure = 0;
    if(angle >= 0){
        PORTB |= 1 << LEFT_DOWN_PIN | 1 << RIGHT_UP_PIN;
    }else{
        PORTB |= 1 << LEFT_UP_PIN | 1 << RIGHT_DOWN_PIN;
        angle = ~angle + 1;
    }

    while(angle){
        measure += calculate_distance();
        if(angle == measure){
            move_stop();
            break;
        }
    }
}


void move_stop(){
    PORTB &= ~(1 << LEFT_UP_PIN | 1 << LEFT_DOWN_PIN
            | 1 << RIGHT_UP_PIN | 1 << RIGHT_DOWN_PIN);
}


void LEFT_forward(int8_t distance){
    uint8_t measure = 0;
    if(distance >= 0){
        PORTB |= 1 << LEFT_UP_PIN;
    }else{
        PORTB |= 1 << LEFT_DOWN_PIN;
        distance = ~distance+1;
    }

    while (distance)
    {
        measure += calculate_distance();
        if(distance == measure){
            move_stop();
            break;
        }
    }   
}

void RIGHT_forward(int8_t distance){
    uint8_t measure = 0;
    if(distance >= 0){
        PORTB |= 1 << RIGHT_UP_PIN;
    }else{
        PORTB |= 1 << RIGHT_DOWN_PIN;
        distance = ~distance+1;
    }

    while (distance)
    {
        measure += calculate_distance();
        if(distance == measure){
            move_stop();
            break;
        }
    }   
}