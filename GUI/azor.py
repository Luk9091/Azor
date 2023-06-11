import serial
import serial.tools.list_ports as serialInfo
import sys

class Communication:
    autoConnect = "/dev/rfcomm0"
    baud = 9600


    def __init__(self) -> None:
        try:
            self.serial = serial.Serial(self.autoConnect, self.baud, 8, 'N', 1)
        except:
            self.showDevices()

    def showDevices(self):
        
        while True:
            ports = serialInfo.comports()

            print("0.\tDont try connected")
            for (i, port) in enumerate(ports):
                print(f"{i+1}.\t{port.device} -- {port.name}")
            
            print(f"{ports.count()+1}.\tScan again")

            choose = -1
            while choose == -1:
                try:
                    choose = int(input("?: "))
                except:
                    print("Choice only number!")

                if choose > ports.count()+1:
                    choose = -1
                    print("This port dont exist!")
                    continue

            if choose == 0:
                sys.exit(-1)
            elif choose == ports.count()+1:
                continue
            break

        self.serial = serial.Serial(ports[choose].name, self.baud, 8, 'N', 1)


    def cmd(self, cmd):
        self.send(cmd)
        return self.read(cmd)


        

    def send(self, data : str):
        if data[-1] != '\n':
            data = data + '\n'

        data = data.encode("utf-8")
        
        self.serial.write(data)
        


    def read(self, expectFunction : str):
        data = []
        line = ""
        saveData = False

        if expectFunction[-1] == '\n':
            expectFunction = expectFunction[:-1]
        while line != "OK":
            line = self.serial.readline().decode("utf-8")
            line = line[0:-1]

            if not saveData:
                if line == expectFunction:
                    saveData = True
                else:
                    continue
            
            if line == "!OK":
                return 0

            data.append(line)
            
        if(data.count == 0):
            return 1
        
        return data[1:-1]





class Head:
    def __init__(self, device : Communication) -> None:
        self.device = device

    def rotate(self, angle = 90):
        cmd = "ur " + str(angle)
        self.device.cmd(cmd)
        
    def measure(self):
        data = self.device.cmd("um")
        data = str(data[0])
        data = data[data.find('Distance: '):]
        data = data[data.find(':')+2:]

        return int(data)

class Position:
    def __init__(self, device : Communication) -> None:
        self.device = device
        
    def azimuth(self):
        data = self.device.cmd("ca")
        data = str(data[0])
        data = data[data.find(":")+2:]
        return data

    def magneticField(self):
        axis = {"x" : 0, "y" : 0, "z" : 0 }

        data = str(self.device.cmd("cx")[0])
        axis["x"] = int(data[data.find(":")+2:])

        data = str(self.device.cmd("cy")[0])
        axis["y"] = int(data[data.find(":")+2:])

        data = str(self.device.cmd("cz")[0])
        axis["z"] = int(data[data.find(":")+2:])

        return axis

    def acceleration(self):
        axis = {"x" : 0, "y" : 0, "z" : 0 }

        data = str(self.device.cmd("ax")[0])
        axis["x"] = int(data[data.find(":")+2:])

        data = str(self.device.cmd("ay")[0])
        axis["y"] = int(data[data.find(":")+2:])

        data = str(self.device.cmd("az")[0])
        axis["z"] = int(data[data.find(":")+2:])

        return axis
        

# ! TODO: Nie jest napisane w Azorze
class Memory:
    def __init__(self, device : Communication) -> None:
        self.device = device

    def read(self):
        pass
    def write(self):
        pass




class Azor:
    distanceMux = 100
    distancePerTic = 64


    def __init__(self) -> None:
        self.device = Communication()

        self.Head = Head(self.device)
        self.Position = Position(self.device)
        self.Memory = Memory(self.device)

    def __del__(self) -> None:
        self.disconnect()

    def forward(self, distance = 10):
        distance = distance * self.distanceMux
        cmd = "ef " + str(distance)
        self.device.cmd(cmd)

    def backward(self, distance = 10):
        distance = distance * self.distanceMux
        cmd = "eb " + str(distance)
        self.device.cmd(cmd)

    def turnLeft(self, angle = 90):
        cmd = "ea " + str(angle)
        self.device.cmd(cmd)

    def turnRight(self, angle = -90):
        cmd = "ea " + str(angle)
        self.device.cmd(cmd)

    def stop(self):
        self.device.send("es")

    def onRoad(self):
        data = str(self.device.cmd("ew")[0])
        data = int(data[data.find(':')+2:])
        return data



    def getDistance(self):
        data = str(self.device.cmd("ec")[0])
        data = int(data[data.find(":")+2:])
        data = (data*self.distancePerTic)/self.distanceMux
        return data
        
    def getTime(self):
        data = str(self.device.cmd("et")[0])
        data = int(data[data.find(":")+2:], 16)
        return data/8

    def getVelocity(self):
        distance = self.getDistance()
        time = self.getTime()
        velocity = (distance/time)*10
        return round(velocity, 2)




    def connect(self):
        if self.device.serial.is_open:
            self.device.serial.open()
    def disconnect(self):
        self.stop()
        self.Head.rotate(90)

        self.device.serial.close()




if __name__=="__main__":
    azor = Azor()

    # while True:
    #     print(azor.Head.measure())


    print("Magnetic field:")
    print(azor.Position.magneticField())
    print(f"Azimuth: {azor.Position.azimuth()}")

    print("Acceleration:")
    print(azor.Position.acceleration())

    print("Sonic sensor:")
    data = {}
    for i in [0, 30, 60, 90, 120, 150, 180]:
        azor.Head.rotate(i)
        data[str(i)] = azor.Head.measure()
    print(data)


    print("Move forward by 10cm: ")
    azor.forward(10)

    while azor.onRoad():
        pass

    print(f"Distance: {azor.getDistance()}cm")
    print(f"Time:     {azor.getTime()}ms")
    print(f"Velocity: {azor.getVelocity()}m/s")