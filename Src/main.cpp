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
#include "I2C.hpp"


int main(){
    TIMER_Init();

    UART_Init(9600, true);
    PWM_Init(true, 42);
    SONIC_Init();

    ENGINE_Init();
    ENGINE_enable(true);
    UART_println("Hello world!");

    I2C_Init();


    sei();

    while (1)
    {
        if(readSize != 0){
            UART_print(string);
            char time_c[3];
            for(uint8_t i = 0; i < 16; i++){
                if(string[i] >= '0' && string[i] <= '9'){
                    time_c[0] = string[i];
                    time_c[1] = string[i+1];
                    time_c[2] = string[i+2];
                    break;
                }
            }
            int8_t time = atoi(time_c);

            // _delay_ms(10);
            switch(string[0]){
                case 'l':{
                    // UART_print("Turn on LEFT on: ");
                    UART_println(time);
                    LEFT_forward(time);
                }break;;
                case 'r':{
                    // UART_print("Turn on RIGHT on: ");
                    UART_println(time);
                    RIGHT_forward(time);
                }break;
                case 'f':{
                    // UART_print("Turn on FORWARD on: ");
                    UART_println(time);
                    move_forward(time);
                }break;
                case 'R':{
                    // UART_print("Rotate in: ");
                    UART_println(time);
                    move_rotate(time);
                } break;

                case 'h':{
                    UART_print("Head move: ");
                    UART_println(time);
                    PWM_setDuty(time);
                }break;

                case 'd':{
                    UART_print("Distance: ");
                    itoa(SONIC_measure(), string, 10);
                    UART_println(string);
                } break;

                case 'a':{
                    I2C_beginTransition(0x1C, WRITE);
                    I2C_write(time);
                    I2C_beginTransition(0x1C, READ);
                    time = I2C_read(i);
                    I2C_endTransition();

                    UART_print("0x");
                    if(time < 16)
                        UART_print_char('0');
                    UART_println(time, 16);
                }break;
            }

            _delay_ms(100);
            for(uint8_t i = 0; i <= readSize; i++)
                string[i] = '\0';
            readSize = 0;
            time = 0;
        }
        _delay_ms(10);
    }
    return 0;
}