#ifndef __CODING__HPP
#define __CODING__HPP

#include <avr/io.h>
#include <avr/sleep.h>
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

#define LED_ON()  LED_PORT |= (LED_PIN_num)
#define LED_OFF() LED_PORT &=!(LED_PIN_num)


#define HIGH 1
#define LOW  0
union REGISTER{
    uint16_t U;
    int16_t S;
    uint8_t Byte[2];
};

extern bool program_run;
extern uint16_t instructionRegister;
extern REGISTER reg[4];
extern uint16_t eeprom_address;



// Instrukcje interpretowane przez uP:
// 0b xxxx yyaa <- 8bitowa liczba
//      xxxx yy -- adres urządzenia
//      aa  -- rejestr do użytkowy
//      yy  -- przy niektórych instrukcjach operujących równolegle na dwóch rejestrach oznacza rejestr pierwszy i docelowy
enum INS{
    // instrukcje specjalne
    NOP             = 0b0000 <<4 | 0b0000,
    END             = 0b0000 <<4 | 0b0001,
    GO_SLEEP        = 0b0000 <<4 | 0b0010,

    INNER_EEPROM_READ= 0b0000 <<4 | 0b1100,
    SET_EEPROM_ADR  = 0b0111 <<4 | 0b1100,
    
    I2C_EEPROM_WRITE = 0b0000 <<4 | 0b0100,
    I2C_EEPROM_READ  = 0b0000 <<4 | 0b1000,

    LDR              = 0b0001 <<4 | 0b0000,

    JUMP_IF          = 0b0001 <<4 | 0b0100,
    JUMP_IF_LOW      = 0b0001 <<4 | 0b1000,

    NOT              = 0b0001 <<4 | 0b1100, 

    ADD             = 0b0010 <<4,//|RdRr
    SUB             = 0b0011 <<4,//|RdRr
    
    INC             = 0b1001 <<4 | 0b0000,
    DEC             = 0b1001 <<4 | 0b0100,

    OR              = 0b0100 <<4,//|RdRr
    AND             = 0b0101 <<4,//|RdRr
    XOR             = 0b0110 <<4,//|RdRr

    PUSH            = 0b0111 <<4 | 0b0000,
    POP             = 0b0111 <<4 | 0b0100,

    JUMP            = 0b0111 <<4 | 0b1000,
    CALL            = 0b0111 <<4 | 0b1001,
    RET             = 0b0111 <<4 | 0b1010,


    SHIFT_LEFT      = 0b1000 <<4 | 0b0000,
    SHIFT_RIGHT     = 0b1000 <<4 | 0b0100,
    SHIFT_8LEFT     = 0b1000 <<4 | 0b1000,
    SHIFT_8RIGHT    = 0b1000 <<4 | 0b1100,



    ENGINE_FORWARD  = 0b1100 <<4 | 0b0000,
    ENGINE_BACKWARD = 0b1100 <<4 | 0b0100,  
    ENGINE_STOP     = 0b1100 <<4 | 0b1000,
                        
    ENGINE_ROTATE   = 0b1100 <<4 | 0b1100,
                        
    WAIT            = 0b1101 <<4 | 0b1100, // delay ms reg
                        
    LED             = 0b1110 <<4 | 0b0000,
                        
    ULTRASONIC_MEASURE    = 0b1110 <<4 | 0b0100,
    ULTRASONIC_ROTATE     = 0b1110 <<4 | 0b1000,
    BACK_SENSOR_READ      = 0b1110 <<4 | 0b1100,
                        
    UART_READ       = 0b1111 <<4 | 0b0000,
    UART_SEND       = 0b1111 <<4 | 0b0100,
    UART_SEND_INT   = 0b1111 <<4 | 0b1000,
};



void program(uint8_t data);
void program(uint16_t address, uint8_t data);

uint8_t EEPROM_read(uint16_t address);
uint8_t fetch();

void execute(uint8_t instruction);

#endif