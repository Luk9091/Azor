import numpy
import turtle
import geometry

class Map:
    def __init__(self, x, y, width, height):
        self.map = turtle.Turtle()
        self.geometry = geometry.geometry()
        self._heading = 0
        
        self.map.color("white")
        self.map.hideturtle()

        self.geometry.x = x
        self.geometry.y = y
        self.geometry.width = width
        self.geometry.height = height

        self.font = "Font/DejaVuSansMono.ttf"

    def move(self, x, y):
        self.geometry.x = x
        self.geometry.y = y

    def resize(self, width, height):
        if -1 < width < 1:
            width = width * geometry.geometry.width

        if -1 < height < 1:
            height = height * geometry.geometry.height

        self.geometry.width = width
        self.geometry.height = height

    def home(self):
        self.map.penup()
        self.map.goto(self.geometry.x, self.geometry.y)
        self.map.setheading(self._heading)

    def color(self, color :str):
        self.map.color(color)

    def getGeometry(self):
        return self.geometry
    
    def wrirte(self, value, align="left"):
        self.map.penup()
        self.map.right(90)
        self.map.forward(20)
        self.map.write(value, font=self.font, align=align)
        self.map.forward(-20)
        self.map.left(90)
        self.map.pendown()

    def draw(self):
        self.map.home()
        self.map.clear()    #funkcja zaczeta