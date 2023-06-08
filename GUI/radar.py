import turtle
import communication


turtle.setup(900, 900)
turtle.bgcolor(30/255, 30/255, 31/255)
font = "Font/DejaVuSansMono.ttf"


def write(turtle, value, align="left"):
    turtle.penup()
    turtle.right(90)
    turtle.forward(20)
    turtle.write(value, font=font, align=align)
    turtle.forward(-20)
    turtle.left(90)
    turtle.pendown()



def draw(radius = 400, position = (0, 0)):
    radar = turtle.Turtle()
    radar.speed(50)
    radar.color("white")
    radar.penup()
    radar.setposition(position)
    radar.pendown()

    radar.forward(radius/2)
    write(radar, "20")
    radar.forward(radius/2)
    write(radar, "40")
    radar.left(90)
    radar.circle(radius, extent=180)
    radar.left(90)
    write(radar, "40", "right")
    radar.forward(radius/2)
    write(radar, "20", "right")
    radar.right(90)
    radar.circle(radius/2, extent=-180)
    radar.left(90)
    radar.forward(radius)
    radar.goto(position)

    radar.setheading(0)
    for i in [[30, "left"], [60, "left"], [90, "center"], [120, "right"], [150, "right"]]:
        radar.left(i[0])
        radar.forward(radius)
        radar.write(i[0], font=font, align=i[1])
        radar.goto(position)
        radar.setheading(0)
    radar.left(90)


pointer = turtle.Turtle()
pointer.hideturtle()
pointer.speed(100)
pointer.width(2)

def measure(step = 3, size = 400, position = (0, 0)):
    measure = 400
    limit = 400
    pointer.clear()

    pointer.penup()
    pointer.goto(position)
    pointer.pendown()

    measure = -10
    for i in range(0, 181, step):
        while measure == -10:
            measure = float(communication.measure(str(i)))/10
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
        pointer.goto(position)
        pointer.setheading(0)
        measure = -10
