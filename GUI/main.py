import turtle
import radar
import GUI
from azor import Azor
import _thread

screen = turtle.Screen()
screen.setup(900, 900)
screen.bgcolor(30/255, 30/255, 31/255)

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
    
    screen.update()
    screen.tracer(1)


def resize(event):
    draw()

def forwClick():
    print(f"Forward {GUI.buffValue}")
    GUI.buffValue = 0

def backClick():
    print(f"Backward {GUI.buffValue}")
    GUI.buffValue = 0

def leftClick():
    print(f"Left {GUI.buffValue}")
    GUI.buffValue = 0

def rightClick():
    print(f"Right {GUI.buffValue}")
    GUI.buffValue = 0

def leftHeadClick():
    print(f"Left head {GUI.buffValue}")
    GUI.buffValue = 0

def rightHeadClick():
    print(f"Right head {GUI.buffValue}")
    GUI.buffValue = 0

def measureClick():
    print("Measure")

def sayHello():
    print("Hello word!")

def cmdControl():
    while(True):
        try:
            GUI.buffValue = abs(int(input("Hello")))
            print(GUI.buffValue)
        except:
            print("Only positive integer allowed!")

if __name__=="__main__":
    screen.getcanvas().bind("<Configure>", resize)

    draw()

    
    GUI.forward.setFunctionHandlerOnClick(forwClick)
    GUI.backward.setFunctionHandlerOnClick(backClick)
    GUI.left.setFunctionHandlerOnClick(leftClick)
    GUI.right.setFunctionHandlerOnClick(rightClick)
    GUI.leftDown.setFunctionHandlerOnClick(leftHeadClick)
    GUI.rightDown.setFunctionHandlerOnClick(rightHeadClick)
    # screen.onclick(GUI.backward.onClick)
    # screen.onclick(GUI.forward.onClick)
    screen.onclick(GUI.onClick)

    try:
        _thread.start_new_thread(cmdControl())
        _thread.start_new_thread(screen.mainloop())
    except:
        print("Error occured")

    # while(True):
    #     input("Hello")
        # screen.mainloop()       # multi-threads ???

    
