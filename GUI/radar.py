import turtle
import numpy as np
# import communication
import geometry


font = "Font/DejaVuSansMono.ttf"
radar = turtle.Turtle()

_geo = geometry.geometry()

def move(x, y):
    if -1 < x < 1 and -1 < y < 1:
        x = x*turtle.window_width()
        y = y*turtle.window_height()
    _geo.x = x
    _geo.y = y


def resize(radius):
    if 0 < radius < 1:
        radius = radius * 200
    _geo.radius = abs(radius)


def write(turtle, value, align="left"):
    turtle.penup()
    turtle.right(90)
    turtle.forward(20)
    turtle.write(value, font=font, align=align)
    turtle.forward(-20)
    turtle.left(90)
    turtle.pendown()



def draw():
    radar.home()
    radar.clear()
    # radar.speed(50)
    radar.color("white")
    radar.penup()
    radar.setposition(_geo.x, _geo.y)
    radar.pendown()

    radar.forward(_geo.radius)
    write(radar, "20")
    radar.forward(_geo.radius)
    write(radar, "40")
    radar.left(90)
    radar.circle(_geo.radius*2, extent=180)
    radar.left(90)
    write(radar, "40", "right")
    radar.forward(_geo.radius)
    write(radar, "20", "right")
    radar.right(90)
    radar.circle(_geo.radius, extent=-180)
    radar.left(90)
    radar.forward(_geo.radius*2)
    radar.goto(_geo.x, _geo.y)

    radar.setheading(0)
    for i in [[30, "left"], [60, "left"], [90, "center"], [120, "right"], [150, "right"]]:
        radar.left(i[0])
        radar.forward(_geo.radius*2)
        radar.write(i[0], font=font, align=i[1])
        radar.goto(_geo.x, _geo.y)
        radar.setheading(0)
    radar.left(90)



pointer = turtle.Turtle()
pointer.hideturtle()
pointer.speed(100)
pointer.width(2)

def measure(step = 3, size = 720):
    measure = 720
    limit = _geo.radius
    pointer.clear()

    pointer.penup()
    pointer.goto(_geo.x, _geo.y)
    pointer.pendown()

    measure = -10
    for i in range(0, 181, step):
        while measure == -10:
            # measure = float(communication.measure(str(i)))/10
            if measure > 400:
                measure = 400
            print(measure)
        
        pointer.color("Green")
        pointer.pendown()
        pointer.left(i)
        # measure = 250

        pointer.forward(measure*size/limit)
        if((limit-measure) > 0):
            pointer.color("Red")
            pointer.forward((limit - measure)*size/limit)
        pointer.penup()
        pointer.goto(_geo.x, _geo.y)
        pointer.setheading(0)
        measure = -10
