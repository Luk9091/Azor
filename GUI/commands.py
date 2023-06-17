import sys
from azor import Azor
import serial.tools.list_ports as serialInfo
from radar import Radar
# azor = Azor()

class CLI:
    def __init__(self, azor: Azor, radar : Radar) -> None:
        self.Azor = azor
        self.radar = radar
        self.COMMANDS = {
            #  cmd      [[min, max] num of args, parameter args]
            "forward":  {"minimumArgs": 0, "handler" : self.Azor.forward},
            "backward": {"minimumArgs": 0, "handler" : self.Azor.backward},
            "left":     {"minimumArgs": 0, "handler" : self.Azor.turnLeft},
            "right":    {"minimumArgs": 0, "handler" : self.Azor.turnRight},

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

    def head(self, *args):

        if args[0] == "left":
            self.Azor.Head.rotate(int(args[1]))
            return True
        elif args[0] == "right":
            self.Azor.Head.rotate(-int(args[1]))
            return True
        elif args[0] == "measure":
            return self.Azor.Head.measure()
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
        for i in range(0, 181, 3):
            self.Azor.Head.rotateTo(i)
            measure = self.Azor.Head.measure()
            self.radar.measure(measure/10, i)
            print(f"{i}.\t{measure}")
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
