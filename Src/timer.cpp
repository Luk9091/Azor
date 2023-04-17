#include "timer.hpp"

// NIE DZIAŁA PRZEPISAĆ DO STAREJ WERSJI I NAPRAWIĆ PRZEKROCZENIE LIMITU!

uint8_t _TIMER_counter = 0;
uint8_t _TIMER_limit = 1;
bool *_TIMER_overflow;
bool TIMER_overflow = false;

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

    TIMSK |= (1 << TOIE1);
}

void TIMER_set(uint8_t limit, bool *overflow){
    _TIMER_overflow = overflow;
    _TIMER_limit = limit;
    _TIMER_counter = 0;
    TCNT1 = 0;
}

bool TIMER_wait_ms(uint8_t delay){

    for(delay; delay > 0; delay--){
        _delay_ms(1);
    }
    return true;
}

uint32_t TIMER_getValue(){
    uint32_t value = (_TIMER_counter *65536) | TCNT1;
    return value;
}