#include "engine.hpp"



ISR(TIMER0_COMPA_vect){
    ENGINE_stop();
}

ISR(TIMER0_OVF_vect){
    ENGINE_stop();
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

void ENGINE_forward(bool enable){
    ENGINE_stop();

    ENGINE_PORT |= 1 << ENGINE_LEFT_UP_PIN | 1 << ENGINE_RIGHT_UP_PIN;

    if(enable)
        ENGINE_ENABLE();
}

void ENGINE_backward(bool enable){
    ENGINE_stop();
    
    ENGINE_PORT |= 1 << ENGINE_LEFT_DOWN_PIN | 1 <<ENGINE_RIGHT_DOWN_PIN;

    if(enable)
        ENGINE_ENABLE();
}

void move_rotate(int16_t angle){
    TIMSK0 &= ~(3 << TOIE0);
    if(angle >= 0){
        ENGINE_PORT |= 1 << ENGINE_LEFT_UP_PIN | 1 << ENGINE_RIGHT_DOWN_PIN;
    }else{
        ENGINE_PORT |= 1 << ENGINE_LEFT_DOWN_PIN | 1 << ENGINE_RIGHT_UP_PIN;
    }

    int16_t azi = COMPASS_getAzimuth();
    // UART_print("Current azi: ");
    // UART_println(azi);

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
        _delay_ms(10);

        azi = COMPASS_getAzimuth();
        azi += COMPASS_getAzimuth();
        azi /= 2;
    }
    ENGINE_DISABLE();
    ENGINE_stop();
    TIMSK0 |= 3 << TOIE0;

    // UART_print("End azi: ");
    // UART_println(azi);

    // UART_print("Delta azi: ");
    // UART_println(destination - azi);
}

void move_rotateTo(int16_t angle){
    int16_t currentAng = COMPASS_getAzimuth();

    angle = angle - currentAng;    
    move_rotate(angle);
}


void ENGINE_stop(){
    TIMER_stop();
    ENGINE_DISABLE();
    ENGINE_PORT &= ~(1 << ENGINE_LEFT_UP_PIN | 1 << ENGINE_LEFT_DOWN_PIN
            | 1 << ENGINE_RIGHT_UP_PIN | 1 << ENGINE_RIGHT_DOWN_PIN);
}


void LEFT_forward(bool direction){
    if(direction){
        ENGINE_PORT |= 1 << ENGINE_LEFT_UP_PIN;
    }else{
        ENGINE_PORT |= 1 << ENGINE_LEFT_DOWN_PIN;
    }
}

void RIGHT_forward(bool direction){
    if(direction){
        ENGINE_PORT |= 1 << ENGINE_RIGHT_UP_PIN;
    }else{
        ENGINE_PORT |= 1 << ENGINE_RIGHT_DOWN_PIN;
    }
}



void move_forward(int16_t distance){
    COUNTER_compValue = distance/distancePerTic-1;
    COUNTER_clear();
    ENGINE_forward();
}

void move_backward(int16_t distance){
    COUNTER_compValue = distance/distancePerTic-1;
    COUNTER_clear();
    ENGINE_backward();
    
}