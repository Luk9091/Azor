#ifndef __CODING__HPP
#define __CODING__HPP

// Instrukcje interpretowane przez uP:
// 0b xxxx yyaa <- 8bitowa liczba
//      xxxx yy -- adres urządzenia
//      aa  -- rejestr do użytkowy
//      yy  -- przy niektórych instrukcjach operujących równolegle na dwóch rejestrach oznacza rejestr pierwszy i docelowy
enum INS{
    // instrukcje specjalne
    NOP             = 0b0000 <<4 | 0b0000,
    END             = 0b0000 <<4 | 0b0001,
    SLEEP           = 0b0000 <<4 | 0b0010,
    PROG_INNER_EEPROM=0b0000 <<4 | 0b0011, //Programowanie odbywa się po UART


    // Operacje na dwóch rejestrach, odpowiednio R0 i R1 lub R2 i R3
    // INNER_EEPROM_WRITE    = 0b0000 <<4 | 0b0100,
    INNER_EEPROM_READ     = 0b0000 <<4 | 0b1100,

    
    I2C_EEPROM_WRITE      = 0b0000 <<4 | 0b0100,
    I2C_EEPROM_READ       = 0b0000 <<4 | 0b1000,


    LDR              = 0b0001 <<4 | 0b0000,
    IF               = 0b0001 <<4 | 0b0100,
    JUMP             = 0b0001 <<4 | 0b1000,
    NOT              = 0b0001 <<4 | 0b1100, 
    
    // LDR_R1          = 0b0001 <<4 | 0b0001,
    // IF_R1           = 0b0001 <<4 | 0b0101,
    // WHILE_R1        = 0b0001 <<4 | 0b1001,
    // NOT_R1          = 0b0001 <<4 | 0b1101, 

    // LDR_R2          = 0b0001 <<4 | 0b0010,
    // IF_R2           = 0b0001 <<4 | 0b0110,
    // WHILE_R2        = 0b0001 <<4 | 0b1010,
    // NOT_R2          = 0b0001 <<4 | 0b1110, 

    // LDR_R3          = 0b0001 <<4 | 0b0011,
    // IF_R3           = 0b0001 <<4 | 0b0111,
    // WHILE_R3        = 0b0001 <<4 | 0b1011,
    // NOT_R3          = 0b0001 <<4 | 0b1111, 


    ADD             = 0b0010 <<4,//|RdRr
    SUB             = 0b0011 <<4,//|RdRr
    
    OR              = 0b0100 <<4,//|RdRr
    AND             = 0b0101 <<4,//|RdRr
    XOR             = 0b0110 <<4,//|RdRr

    // MOV             = 0b0111 <<4,//|RdRr
    PUSH            = 0b0111 <<4 | 0b0000,
    POP             = 0b0111 <<4 | 0b0100,
    PUSH2           = 0b0111 <<4 | 0b1000,
    POP2            = 0b0111 <<4 | 0b1100,



    // Operacje na dwóch rejestrach, odpowiednio R0 i R1 lub R2 i R3
    ACC_READ        = 0b1000 <<4 | 0b0000, // zawsze odczytuje 2 rejestry!
    ACC_WRITE       = 0b1000 <<4 | 0b0100,
    ACC_CAL         = 0b1001 <<4 | 0b0000,

    // ACC_START       = 0b1000 <<4 | 0b1000,
    // ACC_STOP        = 0b1000 <<4 | 0b1100,

    // ENGINE_ENABLE   = 0b1000 <<4 | 0b0000,
    ENGINE_FORWARD  = 0b1100 <<4 | 0b0000,
    ENGINE_LEFT     = 0b1100 <<4 | 0b0100, 
    ENGINE_RIGHT    = 0b1100 <<4 | 0b1000,
    ENGINE_ROTATE   = 0b1100 <<4 | 0b1100,

    TIMER_START     = 0b1101 <<4 | 0b0000, // jako parametr przyjmuje ile maksymalnie może zmierzyć
    TIMER_STOP      = 0b1101 <<4 | 0b0100,
    TIMER_READ      = 0b1101 <<4 | 0b1000,
    WAIT            = 0b1101 <<4 | 0b1100, // delay ms reg

    ULTRASONIC_MEASURE    = 0b1110 <<4 | 0b0100,
    ULTRASONIC_ROTATE     = 0b1110 <<4 | 0b1000,
    BACK_SENSOR_READ      = 0b1110 <<4 | 0b1100,

    BT_ENABLE       = 0b1111 <<4 | 0b0000,
    UART_SEND       = 0b1111 <<4 | 0b0100,
    UART_READ       = 0b1111 <<4 | 0b1000,
};


#endif