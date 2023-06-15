import turtle
import geometry
import numpy as np

class Circle:
    def __init__(self, x, y, radius):
        self.circle = turtle.Turtle()
        self.geometry = geometry.geometry()
        self._heading = 0

        self.circle.color("white")
        self.circle.hideturtle()

        self.geometry.radius = radius*np.sqrt(2)        # geometry.radius zwraca wartosc mniejsza o sqrt(2) niz podany radius, dlatego mnozenie przez sqrt(2)
        # self.geometry.width = radius
        # self.geometry.height = radius
        self.geometry.x = x
        self.geometry.y = y

        self._functionHandler = None

    def move(self, x, y):
        self.geometry.x = x
        self.geometry.y = y

    def resize(self, radius):
        if -1 < radius < 1:
            radius = radius * self.geometry.radius
        
        self.geometry.radius = radius*np.sqrt(2)        # analogicznie jak w __init__
        # self.geometry.width = radius
        # self.geometry.height = radius

    def home(self):
        self.circle.penup()
        self.circle.goto(self.geometry.x, self.geometry.y)
        self.circle.setheading(self._heading)
        self.circle.pendown()

    def color(self, color :str):
        self.circle.color(color)

    def getGeometry(self):
        return self.geometry
    
    def onClick(self, x, y):
        if self._functionHandler != None and (x - self.geometry.x)**2 + (y - self.geometry.y)**2 < (self.geometry.radius)**2:
            self._functionHandler()

    def setFunctionHandlerOnClick(self, fun):
        self._functionHandler = fun

    def draw(self):
        self.circle.clear()
        self.home()

        self.circle.penup()
        self.circle.right(90)
        self.circle.forward(self.geometry.radius)
        self.circle.left(90)
        self.circle.pendown()

        self.circle.begin_fill()

        self.circle.circle(self.geometry.radius)

        self.circle.end_fill()


if __name__ == "__main__":
    screen = turtle.Screen()
    screen.setup(900, 900)
    screen.bgcolor(30/255, 30/255, 31/255)
    screen.tracer(0)

    circle = Circle(0, 0, 100)

    circle.draw()

    print(circle.geometry.radius)
    circle.resize(0.5)
    print(circle.geometry.radius)

    screen.update()
    turtle.exitonclick()