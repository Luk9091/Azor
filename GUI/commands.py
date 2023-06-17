import sys
from azor import Azor
import serial.tools.list_ports as serialInfo
# azor = Azor()

class CLI:
    def __init__(self, azor: Azor) -> None:
        self.COMMANDS = {
            #  cmd      [[min, max] num of args, parameter args]
            "forward":  {"minimumArgs": 0, "handler" : azor.forward},
            "backward": {"minimumArgs": 0, "handler" : azor.backward},
            "left":     {"minimumArgs": 0, "handler" : azor.turnLeft},
            "right":    {"minimumArgs": 0, "handler" : azor.turnRight},

            "head":     {"minimumArgs": 1, "handler" : self.head},
            "acc":      {"minimumArgs": 1, "handler" : azor.Position.acceleration()},
            "magnet":   {"minimumArgs": 1, "handler" : azor.Position.magneticField()},
            "azimuth":  {"minimumArgs": 1, "handler" : azor.Position.azimuth},
        
            "distance": {"minimumArgs": 1, "handler" : self.distance},
            "time":     {"minimumArgs": 0, "handler" : azor.getTime},
            "velocity": {"minimumArgs": 0, "handler" : azor.getVelocity},
        
        
            "exit":     {"minimumArgs": 0, "handler" : self.exit},
        }




    def execute(self, command: str):
        command = command.lower()
        command = command.split(' ')
        cmd = command[0]
        args = command[1:]
        
        if cmd in self.COMMANDS.keys():
            return self.COMMANDS[cmd]["handler"](*args)
        else:
            return "Invalid cmd"

        # print()

    def head(self, *args):

        if args[0] == "left":
            azor.Head.rotate(int(args[1]))
            return True
        elif args[0] == "right":
            azor.Head.rotate(-int(args[1]))
            return True
        elif args[0] == "measure":
            return azor.Head.measure()
        elif args[0].isdigit() and 0 <= int(args[0]) <= 180:
            azor.Head.rotateTo(int(args[0]))
            return True

    def distance(self, *args):
        if len(args) == 0:
            return azor.getDistance()
        elif args[0] == "clear":
            azor.device.send("ecr")
            return True

    def connect(self, *args):
        if len(args) == 0:
            azor.device.showDevices()
        else:
            ports = serialInfo.comports()
            for port in ports:
                if port.device == args[0]:
                    break
            else:
                return "Invalid port name"
            
            azor.device.connect(args[0])

    def exit(self):
        sys.exit()

    


if __name__ == "__main__":
    azor = Azor()
    cli = CLI(azor)
    while True:
        data = input("?: ")
        
        print(cli.execute(data))
