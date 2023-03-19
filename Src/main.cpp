// Schemat połączeń:
//  Diody:
//      PD6 -- BLINK LED -- gnd
// 
//  Ultrasonic Sensor
//      PD4 -- Trig
//(INT1)PD3 -- Echo
//      PB3 -- PWM servo
// 
//  H-Bridge:
//      PB0 -- E1
//      PB1 -- E1
//      PB2 -- E2
//      PB4 -- E2
//      PD7 -- Enable
// 
//  Bluetooth
//      PD0 -- RxD
//      PD1 -- TxD
// 
//  Akcelerometr
//(INT0)PD2 -- int
//      PC5 -- SCL
//      PC4 -- SDA


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "uart.hpp"
#include "PWM.hpp"
#include "sonic.hpp"
#include "engine.hpp"
#include "timer.hpp"




int main(){
    TIMER_Init();

    UART_Init(9600, true);
    PWM_Init(true, 40);
    SONIC_Init();

    ENGINE_Init();
    ENGINE_enable(true);
    UART_println("Hello world!");


    sei();

    while (1)
    {
        if(readSize != 0){

            UART_print("Move: ");
            UART_print(string);
            PWM_setDuty(atoi(string));
            UART_print("\tOK\n");

            _delay_ms(100);

            UART_print("Distance: ");
            itoa(SONIC_measure(), string, 10);
            UART_print(string);
            UART_print("\n\n");

            for(uint8_t i = 0; i < 16; i++)
                string[i] = '\0';
            readSize = 0;

            // move_forward(10);
            LEFT_move(100);
        }
        _delay_ms(10);
    }
    return 0;
}