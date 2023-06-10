#include "sonic.hpp"
#include "uart.hpp"

bool SONIC_done = false;
uint16_t SONIC_value;


ISR(INT1_vect){
    if(PIND & (1 << ECHO_PIN)){
        TIMER_start();
        SONIC_value = TIMER_read();
    } else {
        // TIMER_stop();
        SONIC_value = TIMER_read() - SONIC_value;
        SONIC_done = true;
    }
}



void SONIC_Init(){
    TRIG_DDR  |= (1 << TRIG_PIN);
    ECHO_DDR  &= ~(1<< ECHO_PIN);

    EICRA |= (1 << ISC10); // wyzwolenie zmianą stanu logicznego
    EIMSK |= (1 << INT1);
}


uint16_t SONIC_measure(){
    SONIC_done = false;
    // TIMER_set(0);
    TRIG_PORT |= (1 << TRIG_PIN);
    _delay_us(15);
    TRIG_PORT &= ~(1<< TRIG_PIN);

    while(!SONIC_done){
        _delay_ms(1);
    }

    // return TIMER_getValue()*SOUND_V*100/F_CPU;
    // uint16_t value = ((TIMER_getValue() >> 3));
    uint16_t value = SONIC_value/8;


    // if(value >= 2449)
    //     value = 4200;
    if(value >= 4198)
        value = 7200;
    else if(value <= 117)
        value = 0;
    else
        value = value * SOUND_V;

    return value;

}