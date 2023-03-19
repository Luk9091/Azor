#include "sonic.hpp"
#include "uart.hpp"

uint8_t run = 0;
uint8_t done = 0;
uint16_t overflow = 0;


ISR(INT1_vect){
    if(run &&  !done)
        if(PIND & (1 << ECHO_PIN)){
            TCCR1B |= (1 << CS10);
            PORTB |= 1;
        } else {
            TCCR1B &= ~(7 << CS10);
            PORTB &= ~1;
            done = 1;
        }
}



void SONIC_Init(){
    DDRD  |= (1 << TRIG_PIN);
    DDRD  &= ~(1<< ECHO_PIN);

    MCUCR |= (1 << ISC10); // wyzwolenie zmianą stanu logicznego
    GICR  |= (1 << INT1);
    

    run = 1;
}


uint16_t SONIC_measure(){
    if(run == 0)
        return 0;


    done = 0;
    
    TIMER_set(10);
    PORTD |= (1 << TRIG_PIN);
    _delay_us(15);
    PORTD &= ~(1<< TRIG_PIN);

    while(!done && (TCCR1B & 7 << CS10)){
        _delay_ms(10);
    }

    return TIMER_getValue()*SOUND_V*100/F_CPU;
}