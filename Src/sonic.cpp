#include "sonic.hpp"
#include "uart.hpp"

bool SONIC_run = false;
bool SONIC_done = false;


ISR(INT1_vect){
    if(SONIC_run && !SONIC_done)
        if(PIND & (1 << ECHO_PIN)){
            TIMER_start();
        } else {
            TIMER_stop();
            SONIC_done = true;
        }
}



void SONIC_Init(bool run){
    DDRD  |= (1 << TRIG_PIN);
    DDRD  &= ~(1<< ECHO_PIN);

    MCUCR |= (1 << ISC10); // wyzwolenie zmianą stanu logicznego
    GICR  |= (1 << INT1);
    
    SONIC_run = run;
}


uint16_t SONIC_measure(){
    if(!SONIC_run){
        return 0;
    }
    SONIC_done = false;
    TIMER_set(8, &SONIC_done);
    PORTD |= (1 << TRIG_PIN);
    _delay_us(15);
    PORTD &= ~(1<< TRIG_PIN);

    while(!SONIC_done){
        _delay_ms(10);
    }

    // return TIMER_getValue()*SOUND_V*100/F_CPU;
    uint32_t value = (TIMER_getValue() >> 3);

    if(value >= 2449)
        value = 4200;
    else
        value = value * SOUND_V;

    return value;

}