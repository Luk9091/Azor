import sys
from azor import Azor
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
            "acc":      {"minimumArgs": 1, "handler" : 0},
            "magnet":   {"minimumArgs": 1, "handler" : 0},
            "azimuth":  {"minimumArgs": 1, "handler" : 0},
            
            "distance": {"minimumArgs": 1, "handler" : self.distance}
        }




    def execute(self, command: str):
        command = command.lower()
        command = command.split(' ')
        cmd = command[0]
        args = command[1:]
        if cmd == "exit":
            # azor.disconnect()
            sys.exit(0)

        # for (index, arg) in enumerate(args):
        #     if arg.isdigit():
        #         args[index] = float(arg)
        
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



if __name__ == "__main__":
    azor = Azor()
    cli = CLI(azor)
    while True:
        data = input("?: ")
        
        print(cli.execute(data))