#include "coding.hpp"

bool program_run = false;
uint16_t eeprom_address = 0;
uint16_t instructionRegister = 0;
// int16_t reg[4] = {0, 0, 0, 0};
REGISTER reg[4];

struct Stack{
    #define STACK_SIZE 16
    uint8_t pointer;
    uint16_t stack[STACK_SIZE];

    void PUSH(uint16_t data){
        if(pointer != STACK_SIZE)
            ++pointer;

        stack[pointer] = data;
    }
    uint16_t POP(){
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
    register uint8_t regAdr = instruction & 0x03;

    // UART_print("Instruction register adr: ");
    // UART_println(instructionRegister-1);

    // UART_print("Instruction: ");
    // UART_println(instruction);

    // UART_print("Register: ");
    // UART_println(regAdr);

    // UART_print("Data in reg: ");
    // UART_println(reg[regAdr]);
    // UART_println("");
    
    switch(instruction){
        case NOP:  return;
        case END:{ program_run = false; } return;

        case GO_SLEEP:{
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
            if(program_run){
                reg[regAdr].U = EEPROM_read(eeprom_address);
            }else{
                UART_print("Address: ");
                UART_print(eeprom_address);
                UART_print("\tdata: ");
                uint8_t data = EEPROM_read(eeprom_address);
                UART_println(data, 10);
            }

            --eeprom_address;
        }  return;

        case I2C_EEPROM_WRITE...(I2C_EEPROM_WRITE+3):{
            EEPROM_I2C_write(eeprom_address, reg[regAdr].Byte[LOW]);
            EEPROM_I2C_write(eeprom_address, reg[regAdr].Byte[HIGH]);
            ++eeprom_address;
        }return;
        case I2C_EEPROM_READ...(I2C_EEPROM_READ+3):{
            reg[regAdr].Byte[HIGH] = EEPROM_I2C_read(eeprom_address);
            // reg[regAdr] <<= 8;
            --eeprom_address;
            reg[regAdr].Byte[LOW] = EEPROM_I2C_read(eeprom_address); 
            --eeprom_address;
        }return;
        case SET_EEPROM_ADR...(SET_EEPROM_ADR+3):{
            eeprom_address = reg[regAdr].U;
        };



        case LDR...(LDR+3):{
            if(program_run){
                reg[regAdr].Byte[HIGH] = fetch();
                // reg[regAdr] <<= 8;
                reg[regAdr].Byte[LOW] = fetch();
            } else {
                reg[regAdr].U = find_int(1);
            }
        } return;

        case JUMP_IF...(JUMP_IF+3):{
            if(reg[regAdr].U){
                register uint16_t temp = fetch() << 8;
                temp |= fetch();
                instructionRegister = temp;
            } else {
                instructionRegister += 2;
            }
        } return;
        
        case JUMP_IF_NOT...(JUMP_IF_NOT+3):{
            if(!reg[regAdr].U){
                register uint16_t temp = fetch() << 8;
                temp |= fetch();
                instructionRegister = temp;
            } else {
                instructionRegister += 2;
            }
        } return;

        case JUMP_IF_LOW...(JUMP_IF_LOW+3):{
            register int16_t data = fetch() << 8;
            data |= fetch();
            if(reg[regAdr].S < data){
                register uint16_t temp = fetch() << 8;
                temp |= fetch();
                instructionRegister = temp;
            } else {
                instructionRegister += 2;
            }
        } return;
        case JUMP_IF_GREAT...(JUMP_IF_GREAT+3):{
            register int16_t data = fetch() << 8;
            data |= fetch();
            if(reg[regAdr].S > data){
                register uint16_t temp = fetch() << 8;
                temp |= fetch();
                instructionRegister = temp;
            } else {
                instructionRegister += 2;
            }
        } return;

        case JUMP_IF_LOW_E...(JUMP_IF_LOW_E+3):{
            register int16_t data = fetch() << 8;
            data |= fetch();
            if(reg[regAdr].S <= data){
                register uint16_t temp = fetch() << 8;
                temp |= fetch();
                instructionRegister = temp;
            } else {
                instructionRegister += 2;
            }
        } return;
        case JUMP_IF_GREAT_E...(JUMP_IF_GREAT_E+3):{
            register int16_t data = fetch() << 8;
            data |= fetch();
            if(reg[regAdr].S >= data){
                register uint16_t temp = fetch() << 8;
                temp |= fetch();
                instructionRegister = temp;
            } else {
                instructionRegister += 2;
            }
        } return;



        case NOT...(NOT+3):{
            reg[regAdr].U = !reg[regAdr].U;
        } return;

        case ADD...(ADD+15):{
            reg[(instruction & (0x0C)) >> 2].S = reg[(instruction & (0x0C)) >> 2].S + reg[regAdr].S;
        } return;

        case SUB...(SUB+15):{
            reg[(instruction & (0x0C)) >> 2].S = reg[(instruction & (0x0C)) >> 2].S - reg[regAdr].S;
        } return;

        case OR...(OR+15):{
            reg[(instruction & (0x0C)) >> 2].U = reg[(instruction & (0x0C)) >> 2].U | reg[regAdr].U;
        } return;

        case AND...(AND+15):{
            reg[(instruction & (0x0C)) >> 2].U = reg[(instruction & (0x0C)) >> 2].U & reg[regAdr].U;
        } return;

        case XOR...(XOR+15):{
            reg[(instruction & (0x0C)) >> 2].U = reg[(instruction & (0x0C)) >> 2].U ^ reg[regAdr].U;
        } return;

        case SHIFT_LEFT...(SHIFT_LEFT+3):{
            reg[regAdr].U <<= 1;
        } return;
        case SHIFT_RIGHT...(SHIFT_RIGHT+3):{
            reg[regAdr].U >>= 1;
        } return;

        case SHIFT_8LEFT...(SHIFT_8LEFT+3):{
            reg[regAdr].U <<= 8;
        } return;
        case SHIFT_8RIGHT...(SHIFT_8RIGHT+3):{
            reg[regAdr].U >>= 8;
        } return;

        case INC...(INC+3):{
            ++reg[regAdr].U;
        } return;

        case PUSH...(PUSH+3):{
            stack.PUSH(reg[regAdr].U);
        } return;
        case POP...(POP+3):{
            reg[regAdr].U = stack.POP();
        } return;

        case JUMP:{
            register uint16_t temp = fetch() << 8;
            temp |= fetch();
            instructionRegister = temp;
        } return;
        case CALL:{
            stack.PUSH(instructionRegister);
            register uint16_t temp = fetch() << 8;
            temp |= fetch();
            instructionRegister = temp;
        } return;
        case RET:{
            instructionRegister = stack.POP();
            ++instructionRegister;
        } return;

        case ACC_READ...(ACC_READ+3):{
            reg[regAdr].Byte[LOW] = ACC_readRegister(reg[regAdr].Byte[LOW]);
        }return;
        case ACC_READ_AXIS...(ACC_READ_AXIS+3):{
            reg[regAdr].S = ACC_readAxis(reg[regAdr].Byte[LOW]);
        }return;
        case ACC_WRITE...(ACC_WRITE+3):{
            ACC_writeToRegister(reg[regAdr].Byte[HIGH], reg[regAdr].Byte[LOW]);
        }


        case ENGINE_FORWARD:{
            move_forward(true);
        }return;
        case ENGINE_BACKWARD:{
            move_forward(false);
        }return;
        case ENGINE_STOP:{
            move_stop();
        }return;

        case ENGINE_LEFT_FORWARD:{
            LEFT_forward(true);
        }return;
        case ENGINE_RIGHT_FORWARD:{
            RIGHT_forward(true);
        }return;
        case ENGINE_LEFT_BACKWARD:{
            LEFT_forward(false);
        }return;
        case ENGINE_RIGHT_BACKWARD:{
            RIGHT_forward(false);
        }return;
        
        case ENGINE_ROTATE...(ENGINE_ROTATE+3):{
            move_rotate(reg[regAdr].Byte[LOW]);
        }return;

        case WAIT...(WAIT+3):{
            TIMER_wait_ms(reg[regAdr].Byte[LOW]);
        }return;

        case LED...(LED+3):{
            if(reg[regAdr].U == 0xFFFF){
                LED_PORT ^= LED_PIN_num;
            } else if(reg[regAdr].Byte[HIGH] & (0xFF)){
                reg[regAdr].U = (LED_PIN & LED_PIN_num);
            } else if(reg[regAdr].Byte[LOW] & 0xFF){
                LED_PORT |= LED_PIN_num;
            } else {
                LED_PORT &= ~(LED_PIN_num);
            }
        }


        case ULTRASONIC_MEASURE...(ULTRASONIC_MEASURE+3):{
            reg[regAdr].U = SONIC_measure();
        }return;
        case ULTRASONIC_ROTATE...(ULTRASONIC_ROTATE+3):{
            PWM_setDuty((reg[regAdr].Byte[LOW] * 0.3333F) + PWM_ANGLE_OFFSET);
        }return;



        case UART_READ...(UART_READ+3):{
            reg[regAdr].U = UART_read_char();
        }return;
        
        case UART_SEND...(UART_SEND+3):{
            UART_print_char(fetch());
        } return;
        case UART_SEND_INT...(UART_SEND_INT+3):{
            UART_println(reg[regAdr].S, 10);
        }return;

        case DEVICE_ENABLE...(DEVICE_ENABLE+3):{
            if(reg[regAdr].U & ENGINE){
                ENGINE_ENABLE();
            }else{
                ENGINE_DISABLE();
            }

            if(reg[regAdr].U & ACC){
                ACC_Init();
            }else{
                ACC_RESET();
            }

            SONIC_run = reg[regAdr].U & ULTRASONIC;
            // BT_enable(reg[regAdr].U & BLUETOOTH);

            if(reg[regAdr].U & PWM){
                PWM_start();
            }else{
                PWM_stop();
            }

        }return;
    }
}