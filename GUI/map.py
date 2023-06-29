import turtle
import geometry
import numpy as np
from azor import Azor

from dataclasses import dataclass

@dataclass
class Point:
    distance    : int = 0
    angle       : int = 0

    @property
    def x(self) -> int:
        x = self.distance*np.cos(self.angle*np.pi/180)
        return x

    @property
    def y(self) -> int:
        y = self.distance*np.sin(self.angle*np.pi/180)
        return y


class Map:
    drawLimit = 420

    def __init__(self, x_axis = 200, y_axis = 200, numberOfTics = 8):
        self.map = turtle.Turtle()
        self.geometry = geometry.geometry()
        self._heading = 0
        self.azorTurtle = turtle.Turtle()

        self.numOfTic = numberOfTics
        self.x_axis = x_axis
        self.y_axis = y_axis
        
        self.map.color("white")
        self.map.hideturtle()


        self.pointer = turtle.Turtle()
        self.pointer.color("red")
        self.pointer.width(2)
        self.pointer.hideturtle()
        self.pointer.penup()
        # self.pointer.speed(100)

        self.azorTurtle.penup()
        self.azorTurtle.color("blue")
        self.azorTurtle.left(90)
        self.azorTurtle.speed(1)

        self.azorPosition = geometry.geometry()


        # wallPadding = 25
        # self.azorPosition.x = wallPadding
        # self.azorPosition.y = wallPadding - self.geometry.y
        self.azorPosition.x = x_axis/2
        self.azorPosition.y = y_axis/2
        self.azorTurtle.goto(self.azorPosition.x, self.azorPosition.y)

        self.font = "Font/DejaVuSansMono.ttf"

    def move(self, x, y):
        if -1 < x < 1 and -1 < y < 1:
            x = x*turtle.window_width()
            y = y*turtle.window_height()
        self.geometry.x = x
        self.geometry.y = y

        self.goto(self.azorPosition.x, self.azorPosition.y)

    def resize(self, width, height):
        if 0 < width <= 1:
            width = width * turtle.window_width()

        if 0 < height <= 1:
            height = height * turtle.window_height()

        self.geometry.width = width
        self.geometry.height = height
        
        self.goto(self.azorPosition.x, self.azorPosition.y)


    def goto(self, x = 0, y = 0):
        self.azorTurtle.goto(
            self.geometry.x + x*self.geometry.width/self.x_axis, 
            self.geometry.y + y*self.geometry.height/self.y_axis
        )
        self.azorTurtle.setheading(90)
        self.azorPosition.x = 0 + x
        self.azorPosition.y = 0 + y

    def pointerGoto(self, x, y):
        # if x < 0:
        #     x = 0
        # elif x > self.x_axis:
        #     x = self.x_axis

        # if y < 0:
        #     y = 0
        # elif y > self.y_axis:
        #     y = self.y_axis

        self.pointer.goto(
            self.geometry.x + (self.azorPosition.x + x)*self.geometry.width/self.x_axis, 
            self.geometry.y + (self.azorPosition.y + y)*self.geometry.height/self.y_axis
        )


    def color(self, color :str):
        self.map.color(color)


    def getGeometry(self):
        return self.geometry

    
    def write(self, value, align="left"):
        self.map.right(90)
        self.map.forward(10)
        self.map.penup()
        self.map.forward(20)
        self.map.write(value, font=self.font, align=align)
        self.map.forward(-30)
        self.map.left(90)
        self.map.pendown()
        

    def draw(self):
        self.map.goto(self.geometry.x, self.geometry.y)
        self.map.setheading(0)
        self.map.clear()

        for dx in range(self.numOfTic):
            self.write(str(round(dx*self.x_axis/self.numOfTic)), "center")
            self.map.forward(self.geometry.width/self.numOfTic)
            self.map.left(90)
            self.map.forward(self.geometry.height)
            self.map.backward(self.geometry.height)
            self.map.right(90)
        self.write(str(round(self.x_axis)), "center")

        self.map.left(90)
        for dy in range(self.numOfTic):
            self.write(str(round(dy*self.y_axis/self.numOfTic)))
            self.map.forward(self.geometry.height/self.numOfTic)
            self.map.left(90)
            self.map.forward(self.geometry.width)
            self.map.backward(self.geometry.width)
            self.map.right(90)
        self.write(str(round(self.y_axis)))

        self.map.left(90)
        self.map.forward(self.geometry.width)
        self.map.left(90)

        for dy in range(self.numOfTic):
            self.write(str(round((self.numOfTic-dy)*self.y_axis/self.numOfTic)), "right")
            self.map.forward(self.geometry.height/self.numOfTic)
        self.write(str(round(0)))





        
    # get distance in mm
    def forward(self, distance):
        angle = self.azorTurtle.heading()
        dx = distance*np.cos(angle*np.pi/180)
        dy = distance*np.sin(angle*np.pi/180)
        x = self.azorPosition.x
        y = self.azorPosition.y
        if (
            (0 < x + dx < self.x_axis) and 
            (0 < y + dy < self.y_axis)
            ):
            self.azorTurtle.goto(
                self.azorTurtle.xcor() + dx*self.geometry.width/self.x_axis,
                self.azorTurtle.ycor() + dy*self.geometry.height/self.y_axis,
            )
            self.azorPosition.x = self.azorPosition.x + dx
            self.azorPosition.y = self.azorPosition.y + dy
            return True
        else:
            return False
        
    def backward(self, distance):
        angle = self.azorTurtle.heading() + 180
        dx = distance*np.cos(angle*np.pi/180)
        dy = distance*np.sin(angle*np.pi/180)
        x = self.azorPosition.x
        y = self.azorPosition.y
        if (
            (0 < x + dx < self.x_axis) and 
            (0 < y + dy < self.y_axis)
            ):
            self.azorTurtle.goto(
                self.azorTurtle.xcor() + dx*self.geometry.width/self.x_axis,
                self.azorTurtle.ycor() + dy*self.geometry.height/self.y_axis,
            )
            self.azorPosition.x = self.azorPosition.x + dx
            self.azorPosition.y = self.azorPosition.y + dy
            return True
        else:
            return False
        
    def right(self, angle):
        self.azorTurtle.right(angle)

    def left(self, angle):
        self.azorTurtle.left(angle)

    # get radius and angle of 2 point,
    # both value are depend of position of Azor
    def drawWall(self, points: list):
        for i in range(len(points)):
            points[i] = Point(points[i], i*3)

        self.pointerGoto(points[0].x, points[0].y)
        
        for i in range(0, len(points)-1):
            self.pointer.pendown()
            if (
                abs(points[i+1].distance - points[i].distance) >= 0.1* points[i].distance
                or
                points[i].distance >= 3/4 * Azor.seeDistance
            ):
                self.pointer.penup()
                i = i + 1
            self.pointerGoto(points[i].x, points[i].y)

        self.pointer.penup()
            

    def clear(self):
        self.pointer.clear()
        return True


        

def draw():
    screen.tracer(0)
    map.resize(0.8, 0.8)
    map.move(-0.4, -0.4)
    map.draw()
    
    screen.update()
    screen.tracer(1)
    

def resize(event):
    draw()

if __name__ == "__main__":
    from time import sleep
    screen = turtle.Screen()
    screen.setup(900, 900)
    screen.bgcolor(30/255, 30/255, 31/255)
    
    map = Map(2000, 2000)
    
    screen.getcanvas().bind("<Configure>", resize)

    draw()

    map.goto(250, 250)
    sleep(2)
    map.forward(1000)
    # map.right(45)
    # map.forward(250*np.sqrt(2))1

    # map.drawWall((200, 180), (210, 180-15))

    turtle.exitonclick()