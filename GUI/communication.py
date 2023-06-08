import serial
from time import sleep

dev = serial.Serial("/dev/rfcomm0", 9600)

def readData(dataLine = 0):
    line = 0
    returnData = ""
    data = ""
    while data != "OK":
        data = dev.readline().decode()
        data = data[:-1]
        # print(data)
        if(data == "!OK"):
            return 0

        line = line + 1
        if(line == dataLine):
            returnData = data

    sleep(0.005)
    if dataLine == 0:
        returnData = data
    return returnData



def measure(angle):
    if(int(angle) > 180):
        angle = 180
    if(int(angle) < 0):
        angle = 0

    angle = str(angle)
    msg = "ur " + angle + "\n"

    dev.write(msg.encode())
    readData()
    
    dev.write("um\n".encode())
    data = readData(2)
    # checkAngle = data[7:10]
    # if checkAngle[-1] == "\t":
    #     checkAngle = checkAngle[:-1]

    # if checkAngle != angle:
    #     return 0
    distance = data[19:]

    while(not distance[0].isdigit()):
        distance = distance[1:]


    return int(distance)




if __name__ == "__main__":
    while(1):
        print(measure(input("? ")))