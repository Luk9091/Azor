import sys
from azor import Azor
import serial.tools.list_ports as serialInfo
from radar import Radar
from map import Map
# azor = Azor()

class CLI:
    def __init__(self, azor: Azor, radar : Radar, map : Map) -> None:
        self.Azor = azor
        self.Radar = radar
        self.Map = map
        self.COMMANDS = {
            #  cmd      [[min, max] num of args, parameter args]
            "forward":  {"minimumArgs": 0, "handler" : self.forward},
            "backward": {"minimumArgs": 0, "handler" : self.backward},
            "left":     {"minimumArgs": 0, "handler" : self.turnLeft},
            "right":    {"minimumArgs": 0, "handler" : self.turnRight},

            "head":     {"minimumArgs": 1, "handler" : self.head},
            "acc":      {"minimumArgs": 1, "handler" : self.Azor.Position.acceleration()},
            "magnet":   {"minimumArgs": 1, "handler" : self.Azor.Position.magneticField()},
            "azimuth":  {"minimumArgs": 1, "handler" : self.Azor.Position.azimuth},
        
            "distance": {"minimumArgs": 0, "handler" : self.distance},
            "time":     {"minimumArgs": 0, "handler" : self.Azor.getTime},
            "velocity": {"minimumArgs": 0, "handler" : self.Azor.getVelocity},

            "radar":    {"minimumArgs": 0, "handler": self.radarFun},
            "clear":    {"minimumArgs": 0, "handler": self.Map.clear},
        
        
            "exit":     {"minimumArgs": 0, "handler" : self.exit},
        }




    def execute(self, command: str):
        command = command.lower()
        command = command.split(' ')
        cmd = command[0]
        args = command[1:]
        

        if cmd in self.COMMANDS.keys():
            if len(args) < self.COMMANDS[cmd]["minimumArgs"]:
                arg = self.COMMANDS[cmd]["minimumArgs"]
                return f"Not enough argument. Code expected minium {arg}!"
            if len(args) == self.COMMANDS[cmd]["minimumArgs"]:
                args = args[:self.COMMANDS[cmd]["minimumArgs"]]
            return self.COMMANDS[cmd]["handler"](*args)
        else:
            return "Invalid cmd"

        # print()

    def forward(self, distance = 100):
        if(self.Map.forward(float(distance))):
            return self.Azor.forward(distance)
        else:
            return False

    def backward(self, distance = 100):
        if(self.Map.backward(float(distance))):
            return self.Azor.backward(distance)
        else:
            return False

    def turnLeft(self, angle = 90):
        if(self.Map.left(int(angle))):
            return self.Azor.turnLeft(angle)
        else:
            return False

    def turnRight(self, angle = 90):
        if(self.Map.right(int(angle))):
            return self.Azor.turnRight(angle)
        else:
            return False




    def head(self, *args):

        if args[0] == "left":
            if len(args) < 2:
                return False
            self.Azor.Head.rotate(int(args[1]))
            self.Radar.left(int(args[1]))
            return True
        elif args[0] == "right":
            if len(args) < 2:
                return False
            self.Azor.Head.rotate(-int(args[1]))
            self.Radar.right(int(args[1]))
            return True
        elif args[0] == "measure":
            data = self.Azor.Head.measure()
            self.Radar.measure(data)
            return data
        elif args[0] == "get":
            return self.Azor.Head.getRotate()
        elif args[0] == "set":
            self.Azor.Head.rotateTo(int(args[1]))
            return True
        elif args[0].isdigit() and 0 <= int(args[0]) <= 180:
            self.Azor.Head.rotateTo(int(args[0]))
            return True
        else:
            return False

    def distance(self, *args):
        if len(args) == 0:
            return self.Azor.getDistance()
        elif args[0] == "clear":
            self.Azor.device.send("ecr")
            return True

    def connect(self, *args):
        if len(args) == 0:
            self.Azor.device.showDevices()
        else:
            ports = serialInfo.comports()
            for port in ports:
                if port.device == args[0]:
                    break
            else:
                return "Invalid port name"
            
            self.Azor.device.connect(args[0])


    def radarFun(self, *args):
        self.Azor.Head.rotate(0)
        value = [0] * 61

        self.Radar.clear()

        for i in range(0, 181, 3):
            self.Azor.Head.rotateTo(i)
            measure1 = self.Azor.Head.measure()
            measure2 = self.Azor.Head.measure()
            measure3 = self.Azor.Head.measure()

            self.Radar.rotate(i)
            measure = measure1 + measure2 + measure3
            measure = measure/3
            self.Radar.measure(measure, i)
            
            value[int(i/3)] = measure
            
        self.Azor.Head.rotateTo(90)

        
        for i in range(1, 59):
            dif = value[i-1] - value[i+1]
            if dif == 0:
                value[i] = value[i-1]

        self.Map.drawWall(value)
        
        return True
            

    def exit(self):
        self.Azor.__del__()
        sys.exit()

    


if __name__ == "__main__":
    print("Run main file!")
