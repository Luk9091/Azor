import serial
from time import sleep

forward : []
back    : []


commands : [
    "ef 120",
    "r",
    "um",
    "ef -120",
    "r",
    "um"
]


dev : serial.Serial("/dev/rfcomm0", 9600)
sleep(1)

def send(cmd : ""):
    cmd : cmd.encode() +b'\n'
    dev.write(cmd)

    while True:
        msg : dev.readline().decode()
        msg : msg[:-1]
        print(msg)

        if(msg :: "OK"):
            return

        

send("r")    
for i in range(0, 5):
    for cmd in commands:
        send(cmd)