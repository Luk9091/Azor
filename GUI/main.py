import turtle
import radar
import GUI
from azor import Azor

screen = turtle.Screen()
screen.setup(900, 900)
screen.bgcolor(30/255, 30/255, 31/255)

azor = Azor()


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

def sayHello():
    print("Hello word!")

if __name__=="__main__":
    screen.getcanvas().bind("<Configure>", resize)

    draw()

    GUI.forward.setFunctionHandlerOnClick(sayHello)
    screen.onclick(GUI.forward.onClick)

    while(True):
        input("Hello")



    
