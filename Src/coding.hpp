#ifndef __CODING__HPP
#define __CODING__HPP

#include <avr/io.h>
// #include <avr/sleep.h>
#include <util/delay.h>

#include "uart.hpp"
#include "sonic.hpp"
#include "PWM.hpp"
#include "engine.hpp"
#include "accelerometer.hpp"
#include "eeprom.hpp"

#define LED_PIN_num 1<<6
#define LED_DDR     DDRD
#define LED_PORT    PORTD
#define LED_PIN     PIND

// Instrukcje interpretowane przez uP:
// 0b xxxx yyaa <- 8bitowa liczba
//      xxxx yy -- adres urządzenia
//      aa  -- rejestr do użytkowy
//      yy  -- przy niektórych instrukcjach operujących równolegle na dwóch rejestrach oznacza rejestr pierwszy i docelowy
enum INS{
    // instrukcje specjalne
    NOP             = 0b0000 <<4 | 0b0000,
    RUN_END         = 0b0000 <<4 | 0b0001,
    SLEEP           = 0b0000 <<4 | 0b0010,
    PROG_INNER_EEPROM=0b0000 <<4 | 0b0011, //Programowanie odbywa się po UART


    // Operacje na dwóch rejestrach, odpowiednio R0 i R1 lub R2 i R3
    // INNER_EEPROM_WRITE    = 0b0000 <<4 | 0b0100,
    INNER_EEPROM_READ     = 0b0000 <<4 | 0b1100,

    
    I2C_EEPROM_WRITE      = 0b0000 <<4 | 0b0100,
    I2C_EEPROM_READ       = 0b0000 <<4 | 0b1000,


    LDR              = 0b0001 <<4 | 0b0000,
    SKIP_IF          = 0b0001 <<4 | 0b0100,
    JUMP             = 0b0001 <<4 | 0b1000,
    NOT              = 0b0001 <<4 | 0b1100, 


    ADD             = 0b0010 <<4,//|RdRr
    SUB             = 0b0011 <<4,//|RdRr
    
    OR              = 0b0100 <<4,//|RdRr
    AND             = 0b0101 <<4,//|RdRr
    XOR             = 0b0110 <<4,//|RdRr

    // MOV             = 0b0111 <<4,//|RdRr
    PUSH            = 0b0111 <<4 | 0b0000,
    POP             = 0b0111 <<4 | 0b0100,

    JUMP_TO_ADR     = 0b0111 <<4 | 0b1000,
    CALL            = 0b0111 <<4 | 0b1001,
    RET             = 0b0111 <<4 | 0b1010,

    // POP2            = 0b0111 <<4 | 0b1100,
    SET_EEPROM_ADR  = 0b0111 <<4 | 0b1100,


    SHIFT_LEFT      = 0b1000 <<4 | 0b0000,
    SHIFT_RIGHT     = 0b1000 <<4 | 0b0100,
    SHIFT_8LEFT     = 0b1000 <<4 | 0b1000,
    SHIFT_8RIGHT    = 0b1000 <<4 | 0b1100,



    // Operacje na dwóch rejestrach, odpowiednio R0 i R1 lub R2 i R3
    ACC_READ        = 0b1001 <<4 | 0b0000,
    ACC_READ_AXIS   = 0b1001 <<4 | 0b0100,
    ACC_WRITE       = 0b1001 <<4 | 0b1000,
    ACC_CAL         = 0b1001 <<4 | 0b1100,

    // ACC_START       = 0b1000 <<4 | 0b1000,
    // ACC_STOP        = 0b1000 <<4 | 0b1100,

    // ENGINE_ENABLE   = 0b1000 <<4 | 0b0000,
    ENGINE_FORWARD  = 0b1100 <<4 | 0b0000,
    ENGINE_LEFT     = 0b1100 <<4 | 0b0100, 
    ENGINE_RIGHT    = 0b1100 <<4 | 0b1000,
    ENGINE_ROTATE   = 0b1100 <<4 | 0b1100,

    // TIMER_START     = 0b1101 <<4 | 0b0000, // jako parametr przyjmuje ile maksymalnie może zmierzyć
    // TIMER_STOP      = 0b1101 <<4 | 0b0100,
    // TIMER_READ      = 0b1101 <<4 | 0b1000,
    WAIT            = 0b1101 <<4 | 0b1100, // delay ms reg

    LED             = 0b1110 <<4 | 0b0000,

    ULTRASONIC_MEASURE    = 0b1110 <<4 | 0b0100,
    ULTRASONIC_ROTATE     = 0b1110 <<4 | 0b1000,
    BACK_SENSOR_READ      = 0b1110 <<4 | 0b1100,

    UART_READ       = 0b1111 <<4 | 0b0000,
    UART_SEND       = 0b1111 <<4 | 0b0100,
    UART_SEND_INT   = 0b1111 <<4 | 0b1000,
    DEVICE_ENABLE   = 0b1111 <<4 | 0b1100,
};

enum DEVICE_ADR{
    ENGINE      = 0b1000 <<4 | 0b0000,
    ACC         = 0b0100 <<4 | 0b0000,
    ULTRASONIC  = 0b0010 <<4 | 0b0000,
    BLUETOOTH   = 0b0001 <<4 | 0b0000,
    // TIMER       = 0b0000 <<4 | 0b1000,
    ADCon       = 0b0000 <<4 | 0b0100,
    PWM         = 0b0000 <<4 | 0b0010,
    EEPROM      = 0b0000 <<4 | 0b0001,
};

// uint8_t reg[4];
extern bool program_run;
extern uint16_t instructionRegister;
extern int16_t reg[4];
extern uint16_t eeprom_address;




void program(uint8_t data);
void program(uint16_t address, uint8_t data);

uint8_t EEPROM_read(uint16_t address);
uint8_t fetch();

void execute(uint8_t instruction);

#endif