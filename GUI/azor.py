import serial
import serial.tools.list_ports as serialInfo
import sys
from simulation import Simulator

class Communication:
    autoConnect = "/dev/rfcomm0"
    baud = 9600


    def __init__(self) -> None:
        ports = serialInfo.comports()
        portsDev = []
        for port in ports:
            portsDev.append(port.device)
        
        if self.autoConnect in portsDev:
            self.connect(self.autoConnect)
        else: 
            self.showDevices()

    def showDevices(self):
        
        while True:
            ports = serialInfo.comports()

            print("0.\tDont try connected")
            for (i, port) in enumerate(ports):
                print(f"{i+1}.\t{port.device} -- {port.name}")
            
            print(f"{len(ports)+1}.\tScan again")

            choose = -1
            while choose == -1:
                try:
                    choose = int(input("?: "))
                except ValueError:
                    print("Choice only number!")

                if choose > len(ports)+1:
                    choose = -1
                    print("This port dont exist!")
                    continue

            if choose == len(ports)+1:
                continue
            break

        if choose != 0:
            self.connect(ports[choose-1].device)
        else:
            self.serial = Simulator()


        
    def connect(self, device):
        print("Connecting...")
        self.serial = serial.Serial(device, self.baud, 8, 'N', 1)

        data = self.cmd(" ")
        if data == 0:
            print("Status: Connected!\n")



    def reconnect(self):
        self.showDevices()


    def cmd(self, cmd: str):
        cmd = cmd.lower()
        self.send(cmd)
        return self.read(cmd)

    def getNumber(self, cmd, base = 10, terminator = ":", lineIndex = 0) -> int:
            data = self.cmd(cmd)
            
            if data == None:
                return 0            

            data = str(data[lineIndex])
            data = int(data[data.find(terminator)+2:], base)
            return int(data)



        

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

            if line == "!OK":
                return None

            if not saveData:
                if line == expectFunction:
                    saveData = True
                else:
                    continue
            

            data.append(line)
            
        if(data.count == 0):
            return 1
        
        
        return data[1:-1]




class Head:
    def __init__(self, device : Communication) -> None:
        self.device = device

    def getRotate(self):
        # currentAng = self.device.cmd("ua")[0]
        # currentAng = int(currentAng[currentAng.find(":")+2:])
        # return currentAng
        return self.device.getNumber("ua")
        

    def rotate(self, angle = 6):
        currentAng = self.getRotate()
        if 0 <= (angle + currentAng) <= 180:
            angle = angle + currentAng
        else:
            angle = currentAng

        self.rotateTo(angle)

    def rotateTo(self, angle = 90):
        cmd = "ur " + str(angle)
        self.device.cmd(cmd)
        
    def measure(self):
        # data = self.device.cmd("um")
        # data = str(data[0])
        # data = data[data.find(': ')+2:]
        # return int(data)
        return self.device.getNumber("um")/10

class Position:
    def __init__(self, device : Communication) -> None:
        self.device = device
        
    def azimuth(self):
        # data = self.device.cmd("ca")
        # data = str(data[0])
        # data = data[data.find(":")+2:]
        # return int(data)
        return self.device.getNumber("ca")

    def magneticField(self):
        axis = {"x" : 0, "y" : 0, "z" : 0 }

        # data = str(self.device.cmd("cx")[0])
        # axis["x"] = int(data[data.find(":")+2:])
        axis["x"] = self.device.getNumber("cx")

        # data = str(self.device.cmd("cy")[0])
        # axis["y"] = int(data[data.find(":")+2:])
        axis["y"] = self.device.getNumber("cy")

        # data = str(self.device.cmd("cz")[0])
        # axis["z"] = int(data[data.find(":")+2:])
        axis["z"] = self.device.getNumber("cz")

        return axis

    def acceleration(self):
        axis = {"x" : 0, "y" : 0, "z" : 0 }

        # data = str(self.device.cmd("ax")[0])
        # axis["x"] = int(data[data.find(":")+2:])
        axis["x"] = self.device.getNumber("ax")

        # data = str(self.device.cmd("ay")[0])
        # axis["y"] = int(data[data.find(":")+2:])
        axis["y"] = self.device.getNumber("ay")

        # data = str(self.device.cmd("az")[0])
        # axis["z"] = int(data[data.find(":")+2:])
        axis["z"] = self.device.getNumber("az")

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
    seeDistance = 720

    def __init__(self) -> None:
        self.device = Communication()

        self.Head = Head(self.device)
        self.Position = Position(self.device)
        self.Memory = Memory(self.device)

    def __del__(self) -> None:
        self.disconnect()

    def forward(self, distance : int = 10):
        distance = abs(int(distance))
        distance = int(distance * self.distanceMux)
        cmd = "ef " + str(distance)
        self.device.cmd(cmd)
        return True

    def backward(self, distance : int = 10):
        distance = abs(int(distance))
        distance = int(distance * self.distanceMux)
        cmd = "eb " + str(distance)
        self.device.cmd(cmd)
        return True

    def turnLeft(self, angle = 90):
        angle = -abs(int(angle))
        cmd = "ea " + str(angle)
        self.device.cmd(cmd)
        return self.Position.azimuth()

    def turnRight(self, angle = 90):
        angle = abs(int(angle))
        cmd = "ea " + str(angle)
        self.device.cmd(cmd)
        return self.Position.azimuth()

    def stop(self):
        self.device.cmd("es")
        return True

    def onRoad(self):
        # data = str(self.device.cmd("ew")[0])
        # data = int(data[data.find(':')+2:])
        # return data
        return self.device.getNumber("ew")



    def getDistance(self):
        # data = str(self.device.cmd("ec")[0])
        # data = int(data[data.find(":")+2:])
        data = self.device.getNumber("ec")
        data = (data*self.distancePerTic)/self.distanceMux
        return data
        
    def getTime(self):
        # data = str(self.device.cmd("et")[0])
        # data = int(data[data.find(":")+2:], 16)
        data = self.device.getNumber("et", base=16)
        return data/100

    def getVelocity(self):
        distance = self.getDistance()
        time = self.getTime()
        if time != 0:
            velocity = (distance/time)*10
        else:
            velocity = 0
        return round(velocity, 2)




    # def connect(self):
    #     if self.device.serial.is_open:
    #         self.device.serial.open()
    def disconnect(self):
        self.stop()
        self.Head.rotateTo(90)

        self.device.serial.close()




if __name__=="__main__":
    azor = Azor()

    if azor.device.serial == None:
        sys.exit()
    

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
        azor.Head.rotateTo(i)
        data[str(i)] = azor.Head.measure()
    print(data)


    print("Move forward by 10cm: ")
    azor.forward(10)

    while azor.onRoad():
        pass

    print(f"Distance: {azor.getDistance()}cm")
    print(f"Time:     {azor.getTime()}ms")
    print(f"Velocity: {azor.getVelocity()}m/s")