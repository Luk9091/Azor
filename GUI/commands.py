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

            "radar":      {"minimumArgs": 0, "handler": self.radarFun},
        
        
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
        elif args[0] == "set":
            self.Azor.Head.rotateTo(int(args[0]))
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
        value = []

        self.Radar.clear()

        for i in range(0, 181, 3):
            self.Azor.Head.rotateTo(i)
            measure = self.Azor.Head.measure()
            self.Radar.rotate(i)
            self.Radar.measure(measure, i)
            
            value.append(measure)

        for i in range(180, -1, -3):
            self.Azor.Head.rotateTo(i)
            measure = self.Azor.Head.measure()
            self.Radar.rotate(i)
            self.Radar.measure(measure, i)

            value[int(i/3)] = (value[int(i/3)] + measure)/2

        # for i in range(0, 181, 3):
        #     self.Azor.Head.rotateTo(i)
        #     measure = self.Azor.Head.measure()
        #     self.Radar.rotate(i)
        #     self.Radar.measure(measure, i)
            
        #     value[int(i/3)] = (value[int(i/3)] + measure)/2
        # self.Azor.Head.rotate(0)

        
        for i in range(1, 59):
            dif = value[i-1] - value[i+1]
            if dif == 0:
                value[i] = value[i-1]

        for i in range(0, 181, 3):
            val = value[int(i/3)]
            self.Map.setPoint(round(val), i)
            # print(f"{i}.\t{val}")
        self.Map.pointer.penup()
        
        return True
            

    def exit(self):
        sys.exit()

    


if __name__ == "__main__":
    azor = Azor()
    radar = Radar()
    cli = CLI(azor, radar)
    while True:
        data = input("?: ")
        
        print(cli.execute(data))
