#include "engine.hpp"
#include "util/delay.h"

#define LEFT_UP_PIN     5
#define LEFT_UP_DDR     DDRC
#define LEFT_UP_PORT    PORTC
#define LEFT_UP_READ    PINC & (1 << LEFT_UP_PIN)

#define LEFT_DOWN_PIN   6
#define LEFT_DOWN_DDR   DDRC
#define LEFT_DOWN_PORT  PORTC
#define LEFT_DOWN_READ  PINC & (1 << LEFT_DOWN_PIN)


#define RIGHT_UP_PIN    5
#define RIGHT_UP_DDR    DDRC
#define RIGHT_UP_PORT   PORTC
#define RIGHT_UP_READ   PINC & (1 << RIGHT_UP_PIN)

#define RIGHT_DOWN_PIN  7
#define RIGHT_DOWN_DDR  DDRC
#define RIGHT_DOWN_PORT PORTC
#define RIGHT_DOWN_READ PINC & (1 << RIGHT_DOWN_PIN)

uint16_t calculate_distance(){
    _delay_ms(10);
    return 1;
}


void ENGINE_init(){
    LEFT_UP_DDR     |= 1 << LEFT_UP_PIN;
    LEFT_DOWN_DDR   |= 1 << LEFT_DOWN_PIN;
    RIGHT_UP_DDR    |= 1 << RIGHT_UP_PIN;
    RIGHT_DOWN_DDR  |= 1 << RIGHT_DOWN_PIN;
}


void move_forward(uint16_t distance){
    uint16_t measure = 0;
    LEFT_UP_PORT    |= 1 << LEFT_UP_PIN;
    LEFT_DOWN_PORT  |= 1 << RIGHT_UP_PIN;

    while(distance){
        measure += calculate_distance();
        if(distance == measure)
            move_stop();
            break;
    }
}


void move_stop(){
    LEFT_UP_PORT      &= ~(1 << LEFT_UP_PIN);
    LEFT_DOWN_PORT    &= ~(1 << LEFT_DOWN_PIN);
    RIGHT_UP_PORT     &= ~(1 << RIGHT_UP_PIN);
    RIGHT_DOWN_PORT   &= ~(1 << RIGHT_DOWN_PIN);
}