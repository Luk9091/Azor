import turtle
from radar import Radar
import GUI
from map import Map
from azor import Azor
from commands import CLI
# import _thread
import sys



screen = turtle.Screen()
screen.setup(900, 900)
screen.bgcolor(30/255, 30/255, 31/255)
# value = 1



# turtle.color("white")
# turtle.hideturtle()

def draw():
    screen.tracer(0)
    GUI.resize(0.25, 0.25)
    GUI.move(0.3, -0.3)
    GUI.draw()
     
    radar.resize(0.75)
    radar.move(-0.2, -0.4)
    radar.draw()

    mappedArea.resize(0.75, 0.5)
    mappedArea.move(-0.4, -0.05)
    mappedArea.draw()
    
    screen.update()
    screen.tracer(1)


def resize(event):
    draw()

if __name__=="__main__":
    screen.getcanvas().bind("<Configure>", resize)

    mappedArea = Map(2000, 2000)
    radar = Radar(0, 0, 900)


    draw()

    azor = Azor()
    cli = CLI(azor, radar, mappedArea)

    
    GUI.forward.setFunctionHandlerOnClick(cli.forward)
    GUI.backward.setFunctionHandlerOnClick(cli.backward)
    GUI.left.setFunctionHandlerOnClick(cli.turnLeft)
    GUI.right.setFunctionHandlerOnClick(cli.turnRight)

    GUI.leftDown.setFunctionHandlerOnClick(cli.head, "left", 15)
    GUI.rightDown.setFunctionHandlerOnClick(cli.head, "right", 15)
    GUI.button.setFunctionHandlerOnClick(cli.head, "measure")




    screen.onclick(GUI.onClick)
    screen.tracer(1)

    while turtle.TurtleScreen._RUNNING:
        data = input("?: ")
        print(cli.execute(data))
    