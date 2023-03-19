#include "timer.hpp"

uint8_t _TIMER_counter = 0;
uint8_t _TIMER_limit = 1;

ISR(TIMER1_OVF_vect){
    ++_TIMER_counter;
    if(_TIMER_counter >= _TIMER_limit){
        TCCR1B &= ~(7 << CS10);
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
    // UART_print_char('\n');
    // UART_print("Overflow: ");
    // UART_println(_TIMER_counter);
    // UART_print("Register: ");
    // UART_println(TCNT1);

    return (_TIMER_counter * 65535) + TCNT1;
}