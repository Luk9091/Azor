import turtle
from radar import Radar
import GUI
import map
from azor import Azor
from commands import CLI
import _thread
import sys



screen = turtle.Screen()
screen.setup(900, 900)
screen.bgcolor(30/255, 30/255, 31/255)
azorTurtle = turtle.Turtle()
mappedArea = map.Map(-300, -85, 600, 450)
radar = Radar(0, 0, 300)
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

    mappedArea.draw()

    azorTurtle.penup()
    azorTurtle.color("blue")
    azorTurtle.left(90)
    azorTurtle.goto(-300 + 150*0.3, -80)
    
    screen.update()
    screen.tracer(1)


def resize(event):
    draw()

def forwardClick():
    if radar.filled:
        radar.clear()
    print(f"\nForward")


def backClick():
    if radar.filled:
        radar.clear()
    print(f"\nBackward")


def leftClick():
    if radar.filled:
        radar.clear()
    print(f"\nLeft")


def rightClick():
    if radar.filled:
        radar.clear()
    print(f"\nRight")


def leftHeadClick():
    print(f"\nLeft head")


def rightHeadClick():
    print(f"\nRight head")


def measureClick():
    measureTest(1)
    print("\nMeasure")

def sayHello():
    print("\nHello word!")

def cmdControl():
    # while True:
    while turtle.TurtleScreen._RUNNING:
        # turtle.done()

        read = input("?: ")
        if read.lower() == "exit" or not turtle.TurtleScreen._RUNNING:
            sys.exit(0)
        else:
            print("Value error")
        #     continue

def measureTest(num = 0):
    import pandas as pd

    data = pd.read_csv("GUI\\Measure1.csv", sep='\t')
    angle = data["phi"]
    distance = data["distance"]
    for i in range(len(distance)):
        if num % 2 == 0:
            distance[i] = 400
        mappedArea.measure(azorTurtle, distance[i], angle[i])
        radar.measure(distance[i], angle[i])

def mapTest():
    # azorTurtle.right(90)
    # azorTurtle.forward(530)
    # azorTurtle.left(90)
    for i in range(7):
        azorTurtle.right(180)
        for j in range(6):
            measureTest(j)
            azorTurtle.left(180)
            azorTurtle.forward(80)
            azorTurtle.right(180)
        # azorTurtle.left(30)
        azorTurtle.left(90)
        azorTurtle.forward(85)
        azorTurtle.right(90)
        azorTurtle.forward(480)
        azorTurtle.left(180)

if __name__=="__main__":
    screen.getcanvas().bind("<Configure>", resize)

    draw()

    azor = Azor()
    cli = CLI(azor, radar)
    cli.radar = radar
    
    if azor.device.serial == None:
        GUI.forward.setFunctionHandlerOnClick(forwardClick)
        GUI.backward.setFunctionHandlerOnClick(backClick)
        GUI.left.setFunctionHandlerOnClick(leftClick)
        GUI.right.setFunctionHandlerOnClick(rightClick)
        GUI.leftDown.setFunctionHandlerOnClick(leftHeadClick)
        GUI.rightDown.setFunctionHandlerOnClick(rightHeadClick)
        GUI.button.setFunctionHandlerOnClick(measureClick)
    else:

        GUI.forward.setFunctionHandlerOnClick(azor.forward)
        GUI.backward.setFunctionHandlerOnClick(azor.backward)
        GUI.left.setFunctionHandlerOnClick(azor.turnLeft)
        GUI.right.setFunctionHandlerOnClick(azor.turnRight)

        GUI.leftDown.setFunctionHandlerOnClick(azor.Head.rotate, 15)
        GUI.rightDown.setFunctionHandlerOnClick(azor.Head.rotate, -15)


    screen.onclick(GUI.onClick)

    # cmdControl()
    
    while turtle.TurtleScreen._RUNNING:
        data = input("?: ")
        print(cli.execute(data))
    
    
    # _thread.start_new_thread(cmdControl())
    # _thread.start_new_thread(screen.mainloop())



    
