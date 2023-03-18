// Schemat połączeń:
// PB0 -- BLINK LED -- gnd
// 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "uart.hpp"
#include "timer.hpp"
#include "sonic.hpp"
#include "engine.hpp"


#define DDR(port) DDR ## port
#define PIN(port) PIN ## port
#define PORT(port) PORT ## port

#define pinMode(port, pin, mode) (mode) ? (DDR(port) |= _BV(pin)) : (DDR(port &= ~_BV(pin)))
#define writePin(port, pin, state) (state) ? (PORT(port) |= _BV(pin)) : (PORT(port) &= ~_BV(pin))
#define readPin(port, pin) (PIN(port) & _BV(pin))



int main(){
    DDRD |= 1 << 6 | 1 << 7;
    DDRB = 255;

    // PORTD |= 1 << 7;
    // PORTB |= 1 << 0;


    while(1){
        PORTD ^= 1 << 6;
        _delay_ms(500);
    }



    // pinMode(D, 0, 1);
    // pinMode(D, 7, 1);
    
    // UART_Init(9600);
    // UART_ENABLE_INTERRUPT_RX;

    // // PWM_init(true);
    // // SONIC_init();
    
    // ENGINE_init();

    // sei();
    
    // // writePin(B, 0, 0);
    // UART_write("Hello!\n");
    // while(1){
    //     // UART_write_char(UART_read_char());
    //     _delay_ms(10);

    //     if(readSize != 0){
    //         UART_write(string);
    //         PORTB ^= 1;
    //         // UART_write(string);
    //         if(string[0] == '1'){
    //             move_forward(10);
    //         }

    //         for(uint8_t i = 0; i < 16; i++)
    //             string[i] = '\0';
    //         readSize = 0;
    //     }


    //     // if(readSize != 0){            
    //         // UART_write(string);
    //     //     UART_write("Move: ");
    //     //     PWM_setDuty(atoi(string));
    //     //     UART_write("OK\n");

    //     //     _delay_ms(100);

    //     //     UART_write("Time: ");
    //     //     itoa(SONIC_measure(), string, 10);
    //     //     UART_write(string);
    //     //     UART_write("\n\n");

    //     //     for(uint8_t i = 0; i < 16; i++)
    //     //         string[i] = '\0';

    //     //     readSize = 0;
    //     // }
    // }
    

    return 0;
}