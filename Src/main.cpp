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
            UART_print(string);

            switch(string[0]){
                case 'L':{
                    char time[3];
                    time[0] = string[1];
                    time[1] = string[2];
                    time[2] = '\n';
                    LEFT_forward(atoi(time));
                }break;;
                case 'R':{
                    char time[3];
                    time[0] = string[1];
                    time[1] = string[2];
                    time[2] = '\n';
                    RIGHT_forward(atoi(time));
                }break;
                case 'F':{
                    char time[3];
                    time[0] = string[1];
                    time[1] = string[2];
                    time[2] = '\n';
                    move_forward(atoi(time));
                }

                case 'H':{
                    char time[4];
                    time[0] = string[1];
                    time[1] = string[2];
                    time[2] = '\n';
                    time[3] = '\0';
                    UART_print("Head move: ");
                    UART_print(time);
                    PWM_setDuty(atoi(time));
                    UART_print("\tOK\n");
                }break;

                case 'D':{
                    UART_print("Distance: ");
                    itoa(SONIC_measure(), string, 10);
                    UART_println(string);
                } break;
            }

            _delay_ms(100);
            for(uint8_t i = 0; i < 16; i++)
                string[i] = '\0';
            readSize = 0;

        }
        _delay_ms(10);
    }
    return 0;
}