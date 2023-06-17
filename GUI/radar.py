import turtle
import numpy as np
# import communication
import geometry
from azor import Azor

class Radar:
    def __init__(self, x, y, radius):
        self.radar = turtle.Turtle()
        self.geometry = geometry.geometry()
        self._heading = 0
        self.pointer = turtle.Turtle()

        self.radar.color("white")
        self.radar.hideturtle()
        self.radar.speed(100)

        self.pointer.hideturtle()
        self.pointer.speed(100)
        self.filled = False

        self.geometry.x = x
        self.geometry.y = y
        self.geometry.radius = radius * np.sqrt(2)

        self.font = "Font/DejaVuSansMono.ttf"

    def move(self, x, y):
        if -1 < x < 1 and -1 < y < 1:
            x = x*turtle.window_width()
            y = y*turtle.window_height()
        self.geometry.x = x
        self.geometry.y = y

    def resize(self, radius):
        if 0 < radius < 1:
            radius = radius * geometry.geometry().radius*np.sqrt(2)

        self.geometry.radius = radius * np.sqrt(2)

    def home(self):
        self.radar.penup()
        self.radar.goto(self.geometry.x, self.geometry.y)
        self.radar.setheading(self._heading)
        self.radar.pendown()

    def write(self, value, align = "left"):
        self.radar.penup()
        self.radar.right(90)
        self.radar.forward(20)
        self.radar.write(value, font=self.font, align=align)
        self.radar.forward(-20)
        self.radar.left(90)
        self.radar.pendown()
    
    def draw(self):
        self.radar.home()
        self.radar.clear()
        self.home()


        self.radar.forward(self.geometry.radius/2)
        self.write(str(round(Azor.seeDistance/2/10)))
        self.radar.forward(self.geometry.radius/2)
        self.write(str(round(Azor.seeDistance/10)))
        self.radar.left(90)
        self.radar.circle(self.geometry.radius, extent=180)
        self.radar.left(90)
        self.write(str(round(Azor.seeDistance/10)), "right")
        self.radar.forward(self.geometry.radius/2)
        self.write(str(round(Azor.seeDistance/2/10)), "right")
        self.radar.right(90)
        self.radar.circle(self.geometry.radius/2, extent=-180)
        self.radar.left(90)
        self.radar.forward(self.geometry.radius)
        self.home()

        for i in [[30, "left"], [60, "left"], [90, "center"], [120, "right"], [150, "right"]]:
            self.radar.left(i[0])
            self.radar.forward(self.geometry.radius)
            self.radar.write(i[0], font=self.font, align=i[1])
            self.radar.goto(self.geometry.x, self.geometry.y)
            self.radar.setheading(0)
        self.radar.left(90)

    def measure(self, distance, angle):
        limit = self.geometry.radius
        self.pointer.penup()
        self.pointer.goto(self.geometry.x, self.geometry.y)
        self.pointer.setheading(0)
        self.pointer.pendown()

        if distance > Azor.seeDistance:
            distance = Azor.seeDistance
        else:
            distance = distance * (limit/Azor.seeDistance)*2
        distance = distance * limit/(2*limit)

        self.pointer.color("green")
        self.pointer.left(angle)
        self.pointer.forward(distance)

        if limit - distance > 0:
            self.pointer.color("red")
            self.pointer.forward(limit - distance)

        self.filled = True

    def clear(self):
        self.pointer.clear()
        self.filled = False


if __name__ == "__main__":
    screen = turtle.Screen()
    screen.setup(900, 900)
    screen.bgcolor(30/255, 30/255, 31/255)
    screen.tracer(0)

    radar2 = Radar(0, 0, 200)
    radar2.draw()

    distance = 0
    for angle in range (0, 181, 3):
        distance = angle * 4
        radar2.measure(distance, angle)


    screen.update()
    turtle.exitonclick()