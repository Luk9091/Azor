#include "coding.hpp"

bool program_run = false;
uint16_t eeprom_address = 0;
uint16_t instructionRegister = 0;
int16_t reg[4] = {0, 0, 0, 0};

struct Stack{
    #define STACK_SIZE 16
    uint8_t pointer;
    uint16_t stack[STACK_SIZE];

    void PUSH(uint8_t data){
        if(pointer != STACK_SIZE)
            ++pointer;

        stack[pointer] = data;
    }
    uint8_t POP(){
        if(pointer != 0)
            --pointer;

        return stack[pointer];
    }
} stack;

void program(uint16_t address, uint8_t data){
    // eeprom_write((uint8_t*)address, data);
    while(EECR & (1 << EEWE));

    EEAR = address;
    EEDR = data;

    EECR |= 1 << EEMWE;
    EECR |= 1 << EEWE;
}

uint8_t EEPROM_read(uint16_t address){
    while(EECR & (1 << EEWE));

    EEAR = address;
    EECR |= (1 << EERE);
    return EEDR;
}



void program(uint8_t data){
    program(instructionRegister, data);
    ++instructionRegister;
}

uint8_t fetch(){
    uint8_t data;
    data = EEPROM_read(instructionRegister);
    ++instructionRegister;
    return data;
}



void execute(uint8_t instruction){
    uint8_t regAdr = instruction & 0x03;

    UART_print("Instruction register adr: ");
    UART_println(instructionRegister, 10);

    UART_print("Instruction: ");
    UART_println(instruction);

    UART_print("Register: ");
    UART_println(regAdr);

    UART_print("Data in reg: ");
    UART_println(reg[regAdr]);
    UART_println("");
    
    switch(instruction){
        case NOP:  return;
        case END:{ program_run = false; } return;
        case RUN:{ program_run = true;  }return;

        case SLEEP:{
            // Sleep function
        } return;

        case PROG_INNER_EEPROM:{
            while(true){
                uint8_t data = find_int();
                if(data == PROG_INNER_EEPROM){
                    break;
                }
                program(data);
            }
            instructionRegister = 0;
        } return;

        case INNER_EEPROM_READ...(INNER_EEPROM_READ+3):{
            reg[regAdr] = EEPROM_read(eeprom_address);
            --eeprom_address;
        }  return;

        case I2C_EEPROM_WRITE...(I2C_EEPROM_WRITE+3):{
            EEPROM_I2C_write(eeprom_address, reg[regAdr]);
            EEPROM_I2C_write(eeprom_address, reg[regAdr]>>8);
            ++eeprom_address;
        }return;
        case I2C_EEPROM_READ...(I2C_EEPROM_READ+3):{
            reg[regAdr] = EEPROM_I2C_read(eeprom_address);
            reg[regAdr] <<= 8;
            --eeprom_address;
            reg[regAdr] |= EEPROM_I2C_read(eeprom_address); 
            --eeprom_address;
        }return;
        case SET_EEPROM_ADR...(SET_EEPROM_ADR+3):{
            eeprom_address = reg[regAdr];
        };



        case LDR...(LDR+3):{
            if(program_run){
                reg[regAdr] = fetch();
                reg[regAdr] <<= 8;
                reg[regAdr] |= fetch();
            } else {
                reg[regAdr] = find_int(1);
            }
        } return;

        case SKIP_IF...(SKIP_IF+3):{
            if(reg[regAdr])
                ++instructionRegister;
        } return;

        case JUMP...(JUMP+3):{
            instructionRegister = reg[regAdr];
        } return;

        case NOT...(NOT+3):{
            reg[regAdr] = !reg[regAdr];
        } return;

        case ADD...(ADD+15):{
            reg[regAdr] = reg[instruction & (0x0C) >> 2] + reg[regAdr];
        } return;

        case SUB...(SUB+15):{
            reg[regAdr] = reg[instruction & (0x0C) >> 2] - reg[regAdr];
        } return;

        case OR...(OR+3):{
            reg[regAdr] = reg[instruction & (0x0C) >> 2] | reg[regAdr];
        } return;

        case AND...(AND+3):{
            reg[regAdr] = reg[instruction & (0x0C) >> 2] & reg[regAdr];
        } return;

        case XOR...(XOR+3):{
            reg[regAdr] = reg[instruction & (0x0C) >> 2] ^ reg[regAdr];
        } return;

        case SHIFT_LEFT:{
            reg[regAdr] <<= 1;
        } return;
        case SHIFT_RIGHT:{
            reg[regAdr] >>= 1;
        } return;

        case SHIFT_8LEFT:{
            reg[regAdr] <<= 8;
        } return;
        case SHIFT_8RIGHT:{
            reg[regAdr] >>= 8;
        } return;


        case PUSH...(PUSH+3):{
            stack.PUSH(reg[regAdr]);
        } return;
        case POP...(POP+3):{
            reg[regAdr] = stack.POP();
        } return;

        case JUMP_TO_ADR:{
            instructionRegister = (fetch() << 8);
            instructionRegister |= fetch();
        } return;
        case CALL:{
            stack.PUSH(instructionRegister);
            instructionRegister = (fetch() << 8);
            instructionRegister |= fetch();
        } return;
        case RET:{
            instructionRegister = stack.POP();
            ++instructionRegister;
        } return;

        case ACC_READ...(ACC_READ+3):{
            reg[regAdr] = ACC_readRegister(reg[regAdr]);
        }return;
        case ACC_READ_AXIS...(ACC_READ_AXIS+3):{
            reg[regAdr] = ACC_readAxis(reg[regAdr]);
        }return;
        case ACC_WRITE...(ACC_WRITE+3):{
            ACC_writeToRegister(reg[regAdr], reg[regAdr+1]);
        }


        case ENGINE_FORWARD...(ENGINE_FORWARD+3):{
            move_forward(reg[regAdr]);
        }return;
        case ENGINE_LEFT...(ENGINE_LEFT+3):{
            LEFT_forward(reg[regAdr]);
        }return;
        case ENGINE_RIGHT...(ENGINE_RIGHT+3):{
            RIGHT_forward(reg[regAdr]);
        }return;
        case ENGINE_ROTATE...(ENGINE_ROTATE+3):{
            move_rotate(reg[regAdr]);
        }return;

        case WAIT...(WAIT+3):{
            TIMER_wait_ms(reg[regAdr]);
        }return;

        case LED...(LED+3):{
            if(reg[regAdr] & (0xFF00)){
                reg[regAdr] = (LED_PIN & LED_PIN_num);
            } else if(reg[regAdr] & 0x00FF){
                LED_PORT |= LED_PIN_num;
            } else {
                LED_PORT &= ~(LED_PIN_num);
            }
        }


        case ULTRASONIC_MEASURE...(ULTRASONIC_MEASURE+3):{
            reg[regAdr] = SONIC_measure();
        }return;
        case ULTRASONIC_ROTATE...(ULTRASONIC_ROTATE+3):{
            PWM_setDuty(((uint8_t)reg[regAdr] * 0.3333F) + 12);
        }return;



        case UART_READ...(UART_READ+3):{
            reg[regAdr] = UART_read_char();
        }return;
        
        case UART_SEND...(UART_SEND+3):{
            UART_print_char(reg[regAdr]);
        } return;
        case UART_SEND_INT...(UART_SEND_INT+3):{
            UART_println(reg[regAdr], 10);
        }return;

        case DEVICE_ENABLE...(DEVICE_ENABLE+3):{
            ENGINE_enable((uint8_t)reg[regAdr] & ENGINE);

            if((uint8_t)reg[regAdr] & ACC){
                ACC_Init();
            }else{
                ACC_RESET();
            }

            SONIC_run = (uint8_t)reg[regAdr] & ULTRASONIC;
            BT_enable((uint8_t)reg[regAdr] & BLUETOOTH);

            if((uint8_t)reg[regAdr] & PWM){
                PWM_start();
            }else{
                PWM_stop();
            }

        }return;
    }
}