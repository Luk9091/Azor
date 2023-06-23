#!/usr/bin/python3

import sys
import serial
from time import sleep

dev = serial.Serial("/dev/rfcomm0", 9600)
# while not dev.readable():
#     sleep(0.1)a
# path = ""
# path = "/home/lukasz/Dokumenty/GitHub/Projekt_mikroprocki/Assembler/test.dec"
args = sys.argv
args = args[1:]

while len(args) > 0:
    path = args[0]
    args = args[1:]


try:
    file = open(path, "r")
except:
    print("File don't exist")
    sys.exit(1)


lineNumber = 0

dev.read_all().decode()
dev.write("p\n".encode())
print(dev.readline().decode())

while(True):
    # if lineNumber%10 == 1:
    #     dev.close()
    #     dev.open()

    line = file.readline()
    if line == "":
        break
    line = line.encode()

    ans = dev.read_until(b" ").decode()
    ans = ans[:-1]
    dev.write(line)
    data = dev.readline().decode()[:-1]
    print(f"{ans} {data}\t\t{line[:-1].decode()}")


    lineNumber = lineNumber + 1

dev.write("e\n".encode())
dev.readline().decode()
dev.close()

print("Done")