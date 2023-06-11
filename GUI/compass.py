import turtle
import numpy as np
import geometry

font = "Font/DejaVuSansMono.ttf"

compass = turtle.Turtle()

txt = turtle.Turtle()
txt.color("white")
txt.hideturtle()
txt.penup()

compass.pensize(3)
compass.hideturtle()
colors = ["blue", "red", "white"]

_g = geometry.geometry(0, 0, 50, 80)
rotate = 0

def home():
    compass.penup()
    compass.goto(_g.x, _g.y)
    compass.setheading(-rotate)
    compass.pendown()


def move(x, y):
    if -1 < x < 1 and -1 < y < 1:
        x = x*turtle.window_width()
        y = y*turtle.window_height()
    _g.x = x
    _g.y = y


def resize(radius):
    if 0 < radius < 1:
        radius = radius * 200
    _g.radius = abs(radius)

def draw():
    compass.clear()
    home()
    
    compass.left(90)
    compass.penup()
    compass.forward(16*2)
    compass.pendown()
    compass.right(90)
    
    compass.begin_fill()
    compass.color("blue")
    compass.left(30)
    compass.forward(_g.radius)
    compass.left(90)
    compass.forward(_g.radius*np.sqrt(3))
    compass.left(120)
    compass.forward(_g.radius*np.sqrt(3))
    compass.left(90)
    compass.forward(_g.radius)


    compass.color("red")
    compass.end_fill()
    

def textDraw():
    txt.goto(_g.x, _g.y)
    txt.write("Current azi", align="center", font=(font, 16, "normal"))



if __name__ == "__main__":
    from time import sleep
    screen = turtle.Screen()
    screen.setup(900, 900)
    screen.bgcolor(30/255, 30/255, 31/255)
    # screen.tracer(0)
    # move(0.3, 0.3)
    resize(50)
    # rotate = 90
    draw()

    # sleep(1)
    # for i in range(0, 360):
    #     rotate = i
    #     draw()
    #     screen.update()
    #     sleep(0.1)


    turtle.exitonclick()
