#include "timer.hpp"

// NIE DZIAŁA PRZEPISAĆ DO STAREJ WERSJI I NAPRAWIĆ PRZEKROCZENIE LIMITU!

uint8_t _TIMER_counter = 0;
uint8_t _TIMER_limit = 1;

ISR(TIMER1_OVF_vect){
    ++_TIMER_counter;
    if(_TIMER_counter >= _TIMER_limit){
        TIMER_stop();
        UART_println("Timer overflow");
    }
}


void TIMER_Init(){
    TCCR1A = 0;
    TCCR1B = 0;

    TIMSK |= (1 << TOIE1);
}

void TIMER_set(uint8_t limit){
    _TIMER_limit = limit;
    _TIMER_counter = 0;
    TCNT1 = 0;
}

uint32_t TIMER_getValue(){
    uint32_t value = (_TIMER_counter *65536) | TCNT1;
    return value;
}