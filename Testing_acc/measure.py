import os
import pandas as pd
import serial
from time import sleep

max = 100
dev = serial.Serial("/dev/rfcomm0", 9600)
dev.readline()

time = 0.1
delayTime = 3*time*max

x_data = []
y_data = []
z_data = []
g = []

# if(not dev.is_open):
#     dev.open()
X = "ax\n".encode("utf8")
Y = "ay\n".encode("utf8")
Z = "az\n".encode("utf8")



print("Delay...")
print(f"Time to finish: {int(delayTime/60)}:{round(delayTime%60, 0)}")

for i in range(5,0, -1):
    print(i)
    sleep(1)
print("RUN")
print("Measuring...")

for i in range(0, max):
    if(i%(10)):
        print('.', end='', flush=True)
 # X axis read   
    dev.write(X)
    dev.readline()
    x_data.append(((dev.readline()).decode("utf8"))[12:-1])
    sleep(time)

 # Y axis read   
    dev.write(Y)
    dev.readline()
    y_data.append(((dev.readline()).decode("utf8"))[12:-1])
    sleep(time)

 # Z axis read   
    dev.write(Z)
    dev.readline()
    z_data.append(((dev.readline()).decode("utf8"))[12:-1])
    sleep(time)

dev.close()

print()

dataFrame = pd.DataFrame({"X":x_data, "Y": y_data, "Z": z_data})
dataFrame.to_csv("axis.csv")
print("Done!")
os.system("printf 0x007")