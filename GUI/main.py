import turtle
import radar
import GUI
import map
from azor import Azor
import _thread
import sys

screen = turtle.Screen()
screen.setup(900, 900)
screen.bgcolor(30/255, 30/255, 31/255)
azorTurtle = turtle.Turtle()
mapedArea = map.Map(-300, -85, 600, 450)
# value = 1

# azor = Azor()                                 # UWAGA zakomentowalem Azora !!!


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

    mapedArea.draw()

    azorTurtle.penup()
    azorTurtle.color("blue")
    azorTurtle.left(90)
    azorTurtle.goto(-300 + 150*0.3, -80)
    
    screen.update()
    screen.tracer(1)


def resize(event):
    draw()

def forwClick():
    # if GUI.buffValue <= 0:
    #     azor.forward()
    #     azorTurtle.forward(150*0.1)
    # else
    #     azor.forward(GUI.buffValue)
    #     azorTurtle.forward(150*GUI.buffValue/100)
    print(f"Forward {GUI.buffValue}")
    GUI.buffValue = 0

def backClick():
    # if GUI.buffValue <= 0:
    #     azor.backward()
    #     azorTurtle.forward(-150*0.1)
    # else
    #     azor.backward(GUI.buffValue)
    #     azorTurtle.forward(-150*GUI.buffValue/100)
    print(f"Backward {GUI.buffValue}")
    GUI.buffValue = 0

def leftClick():
    # if GUI.buffValue <= 0:
    #     azor.turnLeft()
    #     azorTurtle.left(90)
    # else
    #     azor.turnLeft(GUI.buffValue)
    #     azorTurtle.left(GUI.buffValue)
    print(f"Left {GUI.buffValue}")
    GUI.buffValue = 0

def rightClick():
    # if GUI.buffValue <= 0:
    #     azor.turnRight()
    #     azorTurtle.right(90)
    # else
    #     azor.turnRight(GUI.buffValue)
    #     azorTurtle.right(GUI.buffValue)
    print(f"Right {GUI.buffValue}")
    GUI.buffValue = 0

def leftHeadClick():
    # if GUI.buffValue <= 0:
    #     azor.Head.rotate()                # czy dobrze jest dobrany znak?
    # else
    #     azor.Head.rotate(GUI.buffValue)
    print(f"Left head {GUI.buffValue}")
    GUI.buffValue = 0

def rightHeadClick():
    # if GUI.buffValue <= 0:
    #     azor.Head.rotate(-90)             # czy dobrze jest dobrany znak?
    # else
    #     azor.Head.rotate(-GUI.buffValue)
    print(f"Right head {GUI.buffValue}")
    GUI.buffValue = 0

def measureClick():
    # distance = azor.Head.measure()    # pomiar manualny
    # measure = -10                     # pomiar automatyczny
    # for i in range(0, 181, 3):
    #     azor.Head.rotate(i)           # czy dobrze dobrany kąt???     Czy potrzebny delay??
    #     while measure == -10:
    #         measure = azor.Head.measure()
    print("Measure")

def sayHello():
    print("Hello word!")

def cmdControl():
    # while True:
    while turtle.TurtleScreen._RUNNING:
        # turtle.done()

        read = input("?: ")
        if read.lower() == "exit":
            sys.exit(0)
        elif read.isnumeric():
            GUI.buffValue = abs(int(read))
            print(GUI.buffValue)
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
        mapedArea.measure(azorTurtle, distance[i], angle[i])

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

    
    GUI.forward.setFunctionHandlerOnClick(forwClick)
    GUI.backward.setFunctionHandlerOnClick(backClick)
    GUI.left.setFunctionHandlerOnClick(leftClick)
    GUI.right.setFunctionHandlerOnClick(rightClick)
    GUI.leftDown.setFunctionHandlerOnClick(leftHeadClick)
    GUI.rightDown.setFunctionHandlerOnClick(rightHeadClick)
    GUI.button.setFunctionHandlerOnClick(measureClick)

    screen.onclick(GUI.onClick)

    # azorTurtle.left(80)
    # azorTurtle.forward(20)
    # measureTest()
    # azorTurtle.right(170)
    
    # azorTurtle.forward(550)
    # azorTurtle.left(90)
    # azorTurtle.forward(400)
    # measureTest()
    mapTest()
    # azorTurtle.right(90)
    # azorTurtle.forward(520)
    # azorTurtle.left(90)
    # azorTurtle.forward(30)
    # azorTurtle.right(135)
    # measureTest()
    cmdControl()
    # while True:
    # _thread.start_new_thread(cmdControl())
    # _thread.start_new_thread(screen.mainloop())

    # print(runGUI)


    
