#include "sonic.hpp"
#include "uart.hpp"

volatile uint8_t run = 0;
uint8_t done = 0;


ISR(INT1_vect){
    cli();
    if(run && !done)
        if(PIND & (1 << ECHO_PIN)){
            TIMER_start();
        } else {
            TIMER_stop();
            done = 1;
        }
    sei();
}



void SONIC_Init(){
    DDRD  |= (1 << TRIG_PIN);
    DDRD  &= ~(1<< ECHO_PIN);

    MCUCR |= (1 << ISC10); // wyzwolenie zmianą stanu logicznego
    GICR  |= (1 << INT1);
    
    run = 1;
}


uint16_t SONIC_measure(){
    if(run == 0){
        UART_println("Sonic is dis");
        return 0;
    }

    done = 0;
    
    TIMER_set(0, &done);
    PORTD |= (1 << TRIG_PIN);
    _delay_us(15);
    PORTD &= ~(1<< TRIG_PIN);

    while(!done){
        _delay_ms(10);
    }

    // return TIMER_getValue()*SOUND_V*100/F_CPU;
    uint32_t value = TIMER_getValue();

    if(value >= 2449)
        value = 4200;
    else
        value = value*SOUND_V;

    return value;

}