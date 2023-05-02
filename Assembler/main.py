import serial

MAX_EEPROM_SPACE = 512



INSTRUCTION = {
    #                           INS               , NUM of args in reg, Byte of next memory byte
    "NOP"                   : [0b0000 <<4 | 0b0000, 0, 0],
    "END"                   : [0b0000 <<4 | 0b0001, 0, 0],
    "SLEEP"                 : [0b0000 <<4 | 0b0010, 0, 0],
    "PROG_INNER_EEPROM"     : [0b0000 <<4 | 0b0011, 0, 1], #Programowanie odbywa się po UART

    # INNER_EEPROM_WRITE    : [0b0000 <<4 | 0b0100, 0],
    "INNER_EEPROM_READ"     : [0b0000 <<4 | 0b1100, 0, 0],

    
    "I2C_EEPROM_WRITE"      : [0b0000 <<4 | 0b0100, 1, 0],
    "I2C_EEPROM_READ"       : [0b0000 <<4 | 0b1000, 1, 0],


    "LDR"                   : [0b0001 <<4 | 0b0000, 1, 1],
    "SKIP_IF"               : [0b0001 <<4 | 0b0100, 1, 0],
    "JUMP"                  : [0b0001 <<4 | 0b1000, 0, 1],
    "NOT"                   : [0b0001 <<4 | 0b1100, 1, 0],


    "ADD"                   : [0b0010 <<4,          2, 0],#|RdRr
    "SUB"                   : [0b0011 <<4,          2, 0],#|RdRr

    "INC"                   : [0b1100 <<4 | 0b1000, 1, 0],
    
    "OR"                    : [0b0100 <<4,          2, 0],#|RdRr
    "AND"                   : [0b0101 <<4,          2, 0],#|RdRr
    "XOR"                   : [0b0110 <<4,          2, 0],#|RdRr

    # MOV"                 : [0b0111 <<4, 2],#|RdRr
    "PUSH"                  : [0b0111 <<4 | 0b0000, 1, 0],
    "POP"                   : [0b0111 <<4 | 0b0100, 1, 0],

    "JUMP_TO_ADD"           : [0b0111 <<4 | 0b1000, 0, 1],
    "CALL"                  : [0b0111 <<4 | 0b1001, 0, 1],
    "RET"                   : [0b0111 <<4 | 0b1010, 0, 0],

    # "POP2"                : 0b0111 <<4 | 0b1100,
    "SET_EEPROM_ADR"        : [0b0111 <<4 | 0b1100, 1, 0],


    "SHIFT_LEFT"            : [0b1000 <<4 | 0b0000, 1, 0],
    "SHIFT_RIGHT"           : [0b1000 <<4 | 0b0100, 1, 0],
    "SHIFT_8LEFT"           : [0b1000 <<4 | 0b1000, 1, 0],
    "SHIFT_8RIGHT"          : [0b1000 <<4 | 0b1100, 1, 0],



    # Operacje na dwóch rejestrach, odpowiednio R0 i R1 lub R2 i R3
    "ACC_READ"              : [0b1001 <<4 | 0b0000, 1, 0],
    "ACC_READ_AXIS"         : [0b1001 <<4 | 0b0100, 1, 0],
    "ACC_WRITE"             : [0b1001 <<4 | 0b1000, 1, 0],
    "ACC_CAL"               : [0b1001 <<4 | 0b1100, 1, 0],

    # "ACC_START"           : 0b1000 <<4 | 0b1000,
    # "ACC_STOP"            : 0b1000 <<4 | 0b1100,

    # "ENGINE_ENABLE"       : 0b1000 <<4 | 0b0000,
    "ENGINE_FORWARD"        : [0b1100 <<4 | 0b0000, 0, 0],
    "ENGINE_BACKWARD"       : [0b1100 <<4 | 0b0001, 0, 0],  
    "ENGINE_STOP"           : [0b1100 <<4 | 0b0011, 0, 0],

    "ENGINE_LEFT_FORWARD"   : [0b1100 <<4 | 0b0100, 0, 0], 
    "ENGINE_RIGHT_FORWARD"  : [0b1100 <<4 | 0b0101, 0, 0],
    "ENGINE_LEFT_BACKWARD"  : [0b1100 <<4 | 0b0110, 0, 0], 
    "ENGINE_RIGHT_BACKWARD" : [0b1100 <<4 | 0b0111, 0, 0],

    "ENGINE_ROTATE"         : [0b1100 <<4 | 0b1100, 1, 0],

    # "TIMER_START"         : 0b1101 <<4 | 0b0000, # jako parametr przyjmuje ile maksymalnie może zmierzyć
    # "TIMER_STOP"          : 0b1101 <<4 | 0b0100,
    # "TIMER_READ"          : 0b1101 <<4 | 0b1000,
    "WAIT"                  : [0b1101 <<4 | 0b1100, 1, 0], # delay ms reg

    "LED"                   : [0b1110 <<4 | 0b0000, 1, 0],

    "ULTRASONIC_MEASURE"    : [0b1110 <<4 | 0b0100, 1, 0],
    "ULTRASONIC_ROTATE"     : [0b1110 <<4 | 0b1000, 1, 0],
    "BACK_SENSOR_READ"      : [0b1110 <<4 | 0b1100, 1, 0],

    "UART_READ"             : [0b1111 <<4 | 0b0000, 1, 0],
    "UART_SEND"             : [0b1111 <<4 | 0b0100, 1, 0],
    "UART_SEND_INT"         : [0b1111 <<4 | 0b1000, 1, 0],
    
    "DEVICE_ENABLE"         : [0b1111 <<4 | 0b1100, 1, 0],
}



REGISTER = {
    "R0"            :       0,
    "R1"            :       1,
    "R2"            :       2,
    "R3"            :       3,
}

FUNCTION_POINTER = {}
memoryAddress = 0





def writeToEEPROM(number):
    # print(number)
    pass

def addFunction(line = 0, cmd = ""):
    cmd = cmd [:-1]
    if cmd in FUNCTION_POINTER:
        print(f"Overwrite function name in line: {line} and {FUNCTION_POINTER[cmd]}")
        return -1

    FUNCTION_POINTER[cmd] = memoryAddress
    return 0




def cal(line = 0, cmd = ""):
    if len(cmd) <= 0:
        print(f"Dont find any instruction in line: {line}")
        return -1
    cmd = cmd.split(' ')

    ins = cmd[0]
    reg = ["R0", "R0"]
    expectReg = 0
    data = list()

    if not ins in INSTRUCTION:
        print(f"Error in line: {line}, dont find key word: {cmd[0]}")
        return -1
    
    asm = INSTRUCTION[ins][0]

    for i in range(1, INSTRUCTION[ins][1]+1):
        expectReg = expectReg + 1
        if len(cmd) <= i:
            print(f"Invalid number of args in line: {line}.\nExpect {INSTRUCTION[ins][1]} registers not {i-1}.")
            return -1
        if not cmd[i][0] == 'R':
            print(f"Invalid argument in line: {line}.\nExpect REGISTER")
            return -1
        else:
            if not cmd[i] in REGISTER:
                print(f"Invalid number of register in line: {line}.\nExpect REGISTER from 0 to 3")
                return -1
        reg[i-1] = cmd[i]
    
    asm = asm + (REGISTER[reg[0]]) + (REGISTER[reg[1]] << 2)

    for i in range(1+expectReg, INSTRUCTION[ins][2]+1+expectReg):
        if len(cmd) <= i:
            print(f"Invalid number of args in line: {line}.\nExpect {INSTRUCTION[ins][1]} not {i-1}.")
            return -1
        
        if cmd[i] in REGISTER:
            print(f"Invalid argument in line: {line}.\nExpect VALUE")
            return -1
        if not cmd[i].isdigit():
            if cmd[i] in FUNCTION_POINTER:
                cmd[i] = FUNCTION_POINTER[cmd[i]]
            else:
                print(f"Invalid name function.\nIn line: {line}")
                return -1
        
        base = 10
        if cmd[i][0:2] == "0B":
            base = 2
        elif cmd[i][0:2] == "0X":
            base = 16

    
    data.append((int(cmd[i], base) & 0xFF00) >> 8)
    data.append((int(cmd[i], base) & 0x00FF))

    writeToEEPROM(asm)
    for i in range(0, len(data)):
        writeToEEPROM(data[i])

    return 1+len(data)




    # if cmd.split(' ').len




while True:
    data = input(f"{memoryAddress}: ")
    if data[-1] == ':':
        data = addFunction(memoryAddress, data)
    else:
        data = cal(memoryAddress, data)
    
    if data == -1:
        break

    memoryAddress = memoryAddress + data
    # print(data)

if data != -1:
    print(f"Memory use: {memoryAddress}/{MAX_EEPROM_SPACE}: {round(memoryAddress/MAX_EEPROM_SPACE*100)}")