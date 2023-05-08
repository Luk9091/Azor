#!/usr/bin/python3
import sys

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
    "JUMP_IF"               : [0b0001 <<4 | 0b0100, 1, 1],
    "JUMP_IF_NOT"           : [0b0001 <<4 | 0b1000, 1, 1],

    "JUMP_IF_LOW"           : [16, 1, 2],
    "JUMP_IF_GREAT"         : [16, 1, 2],
    "JUMP_IF_LOW_E"         : [16, 1, 2],
    "JUMP_IF_GREAT_E"       : [16, 1, 2],


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

    "JUMP"                  : [0b0111 <<4 | 0b1000, 0, 1],
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
    "UART_SEND"             : [0b1111 <<4 | 0b0100, 0, 1],
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

path = ""


def writeToFile(number):
    file.write(str(number)+"\n")

def addFunction(line = 0, cmd = ""):
    cmd = cmd [:-1]
    if cmd in FUNCTION_POINTER:
        print(f"Overwrite function name in line: {line} and {FUNCTION_POINTER[cmd]}")
        return -1

    FUNCTION_POINTER[cmd] = memoryAddress
    return 0

def changeAddress(newAddress):
    file.write(str(newAddress)+'\n')

def cal(line = 0, cmd = ""):
    if len(cmd) <= 0:
        print(f"Dont find any instruction in line: {line}")
        return -1
    cmd = cmd.split(' ')

    ins = cmd[0].upper()
    if ins == "STOP":
        return -2

    reg = ["R0", "R0"]
    data = list()

    if not ins in INSTRUCTION:
        print(f"Error in line: {line}, dont find key word: {cmd[0]}")
        return -1
    
    asm = INSTRUCTION[ins][0]

    for i in range(1, INSTRUCTION[ins][1]+1):
        if len(cmd) <= i:
            print(f"Invalid number of args in line: {line}.\nExpect {INSTRUCTION[ins][1]+INSTRUCTION[ins][2]} registers not {i-1}.")
            return -1
        cmd[i] = cmd[i].upper()
        if not cmd[i][0] == 'R':
            print(f"Invalid argument in line: {line}.\nExpect REGISTER")
            return -1
        else:
            if not cmd[i] in REGISTER:
                print(f"Invalid number of register in line: {line}.\nExpect REGISTER from 0 to 3")
                return -1
        reg[i-1] = cmd[i]
        asm = asm + (REGISTER[cmd[i]] << (2*(INSTRUCTION[ins][1] - i)))
    
    # asm = asm + (REGISTER[reg[0]]) + (REGISTER[reg[1]] * 4)

    for i in range(1+INSTRUCTION[ins][1], INSTRUCTION[ins][2]+1+INSTRUCTION[ins][1]):
        if len(cmd) <= i:
            print(f"Invalid number of args in line: {line}.\nExpect {INSTRUCTION[ins][1]+INSTRUCTION[ins][2]} not {i-1}.")
            return -1
        
        if cmd[i] in REGISTER:
            print(f"Invalid argument in line: {line}.\nExpect VALUE")
            return -1
        mux = 1
        if cmd[i][0] == '-':
            mux = -1
            cmd[i] = cmd[i][1:]

        if not cmd[i].isdigit():
            if cmd[i][0] == """'""":
                data.append(ord(cmd[i][1]))
                continue
            elif cmd[i] in FUNCTION_POINTER:
                cmd[i] = FUNCTION_POINTER[cmd[i]]
            else:
                print(f"Invalid name function.\nIn line: {line}")
                return -1
        else:
            base = 10
            if cmd[i][0:2] == "0B":
                base = 2
                cmd[i] = cmd[i][2:]
            elif cmd[i][0:2] == "0X":
                base = 16
                cmd[i] = cmd[i][2:]
            cmd[i] = int(int(cmd[i], base)*mux)
        
        # if INSTRUCTION[ins][2] == 2:
        data.append((cmd[i] & 0xFF00) >> 8)

        data.append((cmd[i] & 0x00FF))

    writeToFile(asm)
    for i in range(0, len(data)):
        writeToFile(data[i])

    else:
        return 1+len(data)




    # if cmd.split(' ').len


# def main():
args = sys.argv

print(args)
args = args[1:]

while len(args) > 0:
    if args[0] == '-o':
        path = args[1]
        args = args[2:]
    else:
        inputPath = args[0]
        args = args[1:]

inputPath = "/home/lukasz/Dokumenty/GitHub/Projekt_mikroprocki/Assembler/main.asm"

if path == "":
    path = inputPath[0:-4] + ".dec"



inputFile = open(inputPath, "r")
file = open(path, 'w')
file.write("")
file.close()


file = open(path, "+a")
line = 0
while file.readable():
    # data = input(f"{memoryAddress}: ")
    data = inputFile.readline()
    if(data == "\n"):
        continue
    if(data == ""):
        break
    
    while (data[0] == ' '):
        data = data[1:]

    if(data[0] == ";"):
        continue

    if data[-1] == '\n':
        data = data[:-1]
    # if data[0] == '\t':
    #     data == data [1:]

    if data[-1] == ':':
        data = addFunction(line, data)
    elif data[0] == '.':
        changeAddress(data)
        continue
    else:
        data = cal(line, data)
    
    if data == -1:
        break
    elif data == 0:
        # memoryAddress = memoryAddress + 1
        continue
    elif data == -2:
        memoryAddress = memoryAddress + 1
        break

    memoryAddress = memoryAddress + data
    line = line + 1
    # print(data)
file.close()
inputFile.close()

if data != -1:
    print(f"Memory use: {memoryAddress}/{MAX_EEPROM_SPACE}: {round(memoryAddress/MAX_EEPROM_SPACE*100)}%")