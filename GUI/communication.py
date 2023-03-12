import serial
from time import sleep

dev = serial.Serial("/dev/rfcomm0", 9600)


def measure(angle):
    delay = 0.25
    if angle == "10" or angle == "70":
        delay = 1

    angle = angle + "\n"
    dev.write(angle.encode("utf-8"))
    sleep(delay)
    data = dev.read_all().decode("utf-8")
    print(f"{data}Size: {len(data)}")

    time = -1
    if(len(data) > 0 and len(data) < 26):
        time = data.split('\n')
        time = time[2][6:]
        time = int(time, 10)
        time = time * 0.017150

    if(len(data) == 28):
        time = data.split('\n')
        time = time[2][6]
        if time == 'E':
            time = 40


    return time


if __name__ == "__main__":
    # data = ""
    # while data == "":
    #     data = dev.read_all().decode("utf-8")
    # print(f"{data} Size: {len(data)}")
    while(1):
        print(measure(input("?")))