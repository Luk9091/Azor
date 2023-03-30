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
// #include "I2C.hpp"
#include "accelerometer.hpp"


uint8_t find_int(uint8_t count = 0){
    char value_c[4] = {0, 0, 0, 0};
    uint8_t numer = 0;

    for(uint8_t i = 0; i < 16; i++){
        if(string[i] >= '0' && string[i] <= '9' || string[i] == '-' || string[i] == 'x'){
            if(count == 0){
                value_c[numer] = string[i];
                numer++;
                if(!(string[i+1] >= '0' && string[i+1] <= '9' || string[i+1] == 'x'))
                    break;
            } else if(string[i+1] == ' '){
                --count;
            }
        }
    }

    // UART_println(value_c);
    uint8_t value = atoi(value_c);
    return value;
}


int main(){
    TIMER_Init();

    UART_Init(9600, true);
    PWM_Init(true, 42);
    SONIC_Init();

    ENGINE_Init();
    ENGINE_enable(true);
    UART_println("Hello world!");

    I2C_Init();
    ACC_Init();


    sei();

    while (1)
    {
        if(readSize != 0){
            UART_print(string);
            

            switch(string[0]){
                case 'e':{
                    MOTION_DETECT_INT_ON();
                    // turnOffINT = 1;
                    _delay_ms(5);
                    switch (string[1]){
                        case 'l':{
                            LEFT_forward(find_int());
                        }break;;
                        case 'r':{
                            RIGHT_forward(find_int());
                        }break;
                        case 'f':{
                            move_forward(find_int());
                        }break;
                        case 'a':{
                            move_rotate(find_int());
                        } break;
                        
                        default:{
                            UART_println("Invalid cmd!");
                        }
                    }
                }break;

                case 'u':{
                    switch(string[1]){
                        case 'r':{
                            UART_print("Head move: ");
                            PWM_setDuty(find_int());
                        }break;

                        case 'm':{
                            UART_print("Distance: ");
                            itoa(SONIC_measure(), string, 10);
                            UART_println(string);
                        } break;
                        

                        default:{
                            UART_println("Invalid cmd!");
                        }
                    }
                }break;


                case 'a':{
                    switch (string[1]){
                        case 'w':{
                            uint8_t address = find_int(0);
                            uint8_t data = find_int(1);
                            UART_print("Write data: ");
                            UART_print(data, 16);
                            UART_print(" at address: ");
                            UART_println(address, 16);

                            ACC_writeToRegister(address, data);
                        } break;

                        case 'r':{
                            uint8_t address = find_int(0);
                            UART_print("Read from address: ");
                            UART_print(address, 16);
                            UART_println(": ");
                            uint8_t data = ACC_readRegister(address);
                            UART_print(data, 10);
                            UART_print("\t");
                            UART_println(data, 16);
                        }break;

                        case 'x':{
                            UART_print("Axis x acc: ");
                            int16_t data = ACC_readAxis(X_AXIS_REG);
                            UART_print(data, 10);
                            UART_print(",\t");
                            UART_println(data, 16);
                        }break;
                        case 'y':{
                            UART_print("Axis y acc: ");
                            int16_t data = ACC_readAxis(X_AXIS_REG);
                            UART_print(data, 10);
                            UART_print(",\t");
                            UART_println(data, 16);
                        }break;
                        case 'z':{
                            UART_print("Axis z acc: ");
                            int16_t data = ACC_readAxis(X_AXIS_REG);
                            UART_print(data, 10);
                            UART_print(",\t");
                            UART_println(data, 16);
                        }break;

                        case 'm':{
                            if(string[2] == '1'){
                                // turnOffINT = 1;
                                MOTION_DETECT_INT_ON();
                            }else if( string[2] == '0'){
                                MOTION_DETECT_INT_OFF();
                            }
                        }break;
                        default:{
                            UART_println("Invalid cmd!");
                        }
                    }
                }break;

                default:
                    UART_println("Invalid function");
            }

            _delay_ms(100);
            for(uint8_t i = 0; i <= readSize; i++)
                string[i] = '\0';
            readSize = 0;
        }
        _delay_ms(10);
    }
    return 0;
}