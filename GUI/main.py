import turtle
import radar
import GUI
from azor import Azor
from commands import CLI
import _thread
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
    
    screen.update()
    screen.tracer(1)


def resize(event):
    draw()

def forwardClick():
    print(f"\nForward")


def backClick():
    print(f"\nBackward")


def leftClick():
    print(f"\nLeft")


def rightClick():
    print(f"\nRight")


def leftHeadClick():
    print(f"\nLeft head")


def rightHeadClick():
    print(f"\nRight head")


def measureClick():
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

if __name__=="__main__":
    screen.getcanvas().bind("<Configure>", resize)

    draw()

    azor = Azor()
    cli = CLI(azor)
    
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


    
