#include "timer.hpp"

uint8_t _TIMER_counter = 0;
uint8_t _TIMER_limit = 255;
bool *_TIMER_overflow;
bool TIMER_overflow = false;
uint16_t velocity = 0;

ISR(TIMER1_OVF_vect){
    ++_TIMER_counter;
    if(_TIMER_counter >= _TIMER_limit){
        TIMER_stop();
        *_TIMER_overflow = true;
        // UART_println("Timer overflow");
    }
}


void TIMER_Init(){
    TCCR1A = 0;
    TCCR1B = 0;

    TIMSK1 |= (1 << TOIE1);
}

void TIMER_set(uint8_t limit, bool *overflow){
    _TIMER_overflow = overflow;
    *_TIMER_overflow = false;
    _TIMER_limit = limit;
    _TIMER_counter = 0;
    TCNT1 = 0;
}

void TIMER_wait_ms(uint16_t delay){
    for(delay; delay > 0; delay--){
        _delay_ms(1);
    }
}

void TIMER_clear(){
    _TIMER_counter = 0;
    TCNT1 = 0;
}

uint32_t TIMER_getValue(){
    uint32_t data =  (_TIMER_counter * 65536) | TCNT1;
    return data;
}



void COUNTER_Init(){
    COUNTER_DDR &= ~(1 << COUNTER_PIN);
    COUNTER_PORT |= (1 << COUNTER_PIN);
    TIFR0  |= 1 << OCF0A  | 1 << TOV0;
    TIMSK0 |= 1 << OCIE0A | 1 << TOIE0;

    // TCCR0A = 0;

    COUNTER_start();
    COUNTER_clear();
}

