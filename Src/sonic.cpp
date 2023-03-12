#include "sonic.hpp"
#include "uart.hpp"

uint8_t run = 0;
uint16_t counter = 0;
uint8_t done = 0;


ISR(TIMER0_OVF_vect){
    ++counter;
    // UART_write_char('c');

    if(counter >= 10){
        TCCR0 &= ~(7 << CS00);
        done = 1;
        counter = 0;
        TCNT0 = 0;
        UART_write("Error: ");
    }
}


ISR(INT1_vect){
    // UART_write("Change logic\n");
    if(run && !done)
        if(PIND & (1 << ECHO_PIN)){
            TCCR0 |= (1 << CS00);
            PORTB |= 1;
            // UART_write("Run\n");
        } else {
            TCCR0 &= ~(7 << CS00);
            done = 1;
            PORTB &= ~1;
            // UART_write("Stop\n");
        }
}



void SONIC_init(){
    DDRD  |= (1 << TRIG_PIN);
    DDRD  &= ~(1<< ECHO_PIN);
    // PORTD |= (1 << ECHO_PIN);

    // TIMER init
    TCCR0 = 0;
    TCNT0 = 0;
    TIMSK |= (1 << TOIE0);

    // PIN interrupt
    MCUCR |= (1 << ISC10); // wyzwolenie zmianą stanu logicznego
    GICR  |= (1 << INT1);


    run = 1;
}


uint16_t SONIC_measure(){
    if(run == 0)
        return 0;

    TCNT0 = 0;
    counter = 0;
    done = 0;
    
    PORTD |= (1 << TRIG_PIN);
    _delay_us(15);
    PORTD &= ~(1<< TRIG_PIN);

    while(!done){
        _delay_ms(10);
    }

    counter = (counter * 256) + TCNT0;
    return counter;
}