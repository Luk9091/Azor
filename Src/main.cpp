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
//  EEPROM
//      PC5 -- SCL
//      PC4 -- SDA



#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "coding.hpp"

#include "uart.hpp"
#include "PWM.hpp"
#include "sonic.hpp"
#include "engine.hpp"
#include "timer.hpp"
#include "accelerometer.hpp"



void readFifo(){
    UART_println("Read FIFO:");

    for(uint8_t innerCounter = 0; innerCounter < FIFO_counter; innerCounter++){
        UART_print(innerCounter);
        UART_print(".\t");
        UART_println(FIFO[innerCounter]);
    }
    
    UART_println("END reading");
    FIFO_counter = 0;
}




int main(){
    LED_DDR |= LED_PIN_num;

    TIMER_Init();
    UART_Init(9600, true);
    PWM_Init(true, 42);
    SONIC_Init(true);
    ENGINE_Init();
    I2C_Init();
    ACC_Init();

    UART_println("Hello world!");
    sei();

    while (1)
    {
        if(readSize != 0){
            switch(string[0]){
            //     case 'e':{
            //         switch (string[1]){
            //             case 'l':{
            //                 LEFT_forward(find_int());
            //             }break;;
            //             case 'r':{
            //                 RIGHT_forward(find_int());
            //             }break;
            //             case 'f':{
            //                 move_forward(find_int());
            //             }break;
            //             case 'a':{
            //                 move_rotate(find_int());
            //             } break;
            //             case 'e':{
            //                 ENGINE_enable(find_int());
            //             }break;
                        
            //             default:{
            //                 UART_println("Invalid cmd!");
            //             }
            //         }
            //     }break;

            //     case 'u':{
            //         switch(string[1]){
            //             case 'r':{
            //                 uint8_t duty = find_int();
            //                 UART_print("Head move: ");
            //                 UART_println(duty);
            //                 PWM_setDuty(duty);
            //                 // _delay_ms(20);
            //                 // PWM_setDuty(0);
            //             }break;

            //             case 'm':{
            //                 UART_print("Distance: ");
            //                 itoa(SONIC_measure(), string, 10);
            //                 UART_println(string);
            //             } break;
                        

            //             default:{
            //                 UART_println("Invalid cmd!");
            //             }
            //         }
            //     }break;


            //     case 'a':{
            //         switch (string[1]){
            //             case 'w':{
            //                 uint8_t address = find_int(0);
            //                 uint8_t data = find_int(1);
            //                 UART_print("Write data: ");
            //                 UART_print(data, 16);
            //                 UART_print(" at address: ");
            //                 UART_println(address, 16);

            //                 ACC_writeToRegister(address, data);
            //             } break;

            //             case 'r':{
            //                 uint8_t address = find_int(0);
            //                 UART_print("Read from address: ");
            //                 UART_print(address, 16);
            //                 UART_println(": ");
            //                 uint8_t data = ACC_readRegister(address);
            //                 UART_print(data, 10);
            //                 UART_print("\t");
            //                 UART_println(data, 16);
            //             }break;

            //             case 'x':{
            //                 UART_print("Axis x acc: ");
            //                 int16_t data = ACC_readAxis(X_AXIS_REG);
            //                 UART_println(data, 10);
            //             }break;
            //             case 'y':{
            //                 UART_print("Axis y acc: ");
            //                 int16_t data = ACC_readAxis(Y_AXIS_REG);
            //                 UART_println(data, 10);
            //             }break;
            //             case 'z':{
            //                 UART_print("Axis z acc: ");
            //                 int16_t data = ACC_readAxis(Z_AXIS_REG);
            //                 UART_println(data, 10);
            //             }break;
                        
            //             default:{
            //                 UART_println("Invalid cmd!");
            //             }
            //         }
            //     }break;

                case 'r':
                    // readFifo();
                    // UART_println("\n\n");
                    UART_print("EEPROM address: ");
                    UART_println(eeprom_address);
                    UART_println("General purpose register:");
                    UART_print("0.\t");
                    UART_println(reg[0].S);
                    UART_print("1.\t");
                    UART_println(reg[1].S);
                    UART_print("2.\t");
                    UART_println(reg[2].S);
                    UART_print("3.\t");
                    UART_println(reg[3].S);
                break;

                case 'p':{
                    readSize = 0;
                    uint16_t eepromAddress = 0;
                    while(1){
                        UART_print(eepromAddress);
                        UART_print(". ");
                        while(readSize == 0) _delay_ms(10);
                        

                        if(string[0] == 'e'){
                            break;
                        }else if(string[0] == '.'){
                            eepromAddress = find_int();
                        }else{
                            program(eepromAddress, find_int());
                            ++eepromAddress;
                        }
                        readSize = 0;
                    }
                    instructionRegister = 0;
                }break;
                case 'x':{
                    // execute(RUN);
                    UART_DISABLE_INTERRUPT_RX;
                    program_run = true;
                    while(program_run){
                        execute(fetch());
                    }
                    instructionRegister = 0;
                    UART_ENABLE_INTERRUPT_RX;
                }break;
                case 's':{
                    UART_DISABLE_INTERRUPT_RX;
                    program_run = true;
                    
                    uint8_t ins = 0;
                    char stopMsg;
                    while(program_run){
                        UART_print("Line: ");
                        UART_print(instructionRegister);
                        UART_print("\tIns: ");
                        ins = fetch();
                        UART_println(ins);
                        execute(ins);

                        UART_print("EEPROM address: ");
                        UART_println(eeprom_address);
                        UART_println("General purpose register:");
                        UART_print("0.\t");
                        UART_println(reg[0].S);
                        UART_print("1.\t");
                        UART_println(reg[1].S);
                        UART_print("2.\t");
                        UART_println(reg[2].S);
                        UART_print("3.\t");
                        UART_println(reg[3].S);

                        UART_println("\n");

                        do{
                            stopMsg = UART_read_char();    
                        }while(stopMsg != '\n');
                        stopMsg = 0;
                    }
                    instructionRegister = 0;
                    UART_ENABLE_INTERRUPT_TX;


                }break;
                case 'w':{
                    execute(find_int());
                } break;


                default:
                    UART_print_char('!');
            }
            UART_println("OK");

            for(uint8_t i = 0; i <= readSize; i++)
                string[i] = '\0';
            readSize = 0;
        }
        _delay_ms(10);
    }
    return 0;
}