import turtle
import geometry
import numpy as np
from azor import Azor

class Map:
    def __init__(self, x, y, width, height):
        self.map = turtle.Turtle()
        self.geometry = geometry.geometry()
        self._heading = 0
        self.pointer = turtle.Turtle()
        self.azorTurtle = turtle.Turtle()
        
        self.map.color("white")
        self.map.hideturtle()
        self.map.speed(100)

        self.pointer.hideturtle()
        self.pointer.speed(100)

        self.azorTurtle.penup()
        self.azorTurtle.color("blue")
        self.azorTurtle.left(90)
        self.azorTurtle.goto(x + Azor.seeDistance*0.08, y)

        self.geometry.x = x
        self.geometry.y = y
        self.geometry.width = width
        self.geometry.height = height

        self.font = "Font/DejaVuSansMono.ttf"

    def move(self, x, y):
        if -1 < x < 1 and -1 < y < 1:
            x = x*turtle.window_width()
            y = y*turtle.window_height()
        self.geometry.x = x
        self.geometry.y = y

    def resize(self, width, height):
        if 0 < width < 1:
            width = width * geometry.geometry().width

        if 0 < height < 1:
            height = height * geometry.geometry().height

        self.geometry.width = width
        self.geometry.height = height


    def home(self, dx = 0, dy = 0):
        self.map.penup()
        self.map.goto(self.geometry.x + dx, self.geometry.y + dy)
        self.map.setheading(self._heading)
        self.map.pendown()


    def color(self, color :str):
        self.map.color(color)


    def getGeometry(self):
        return self.geometry

    
    def write(self, value, align="left"):
        self.map.penup()
        self.map.right(90)
        self.map.forward(30)
        self.map.write(value, font=self.font, align=align)
        self.map.forward(-30)
        self.map.left(90)
        self.map.pendown()
        

    def draw(self):
        self.map.clear()
        self.home(-1, -1)

        defaultSize = self.map.width()
        thickSize = 3

        hLabels = [[0, "center"], [50, "center"], [100, "center"], [150, "center"], [200, "center"], 
                   [250, "center"], [300, "center"], [350, "center"], [400, "center"]]
        vLabels = [[0, "right"], [50, "right"], [100, "right"], [150, "right"], [200, "right"], [250, "right"], [300, "right"]]
        self.map.width(thickSize)
        self.map.pendown()
        self.map.forward(self.geometry.width + 2)
        self.map.left(90)
        for i in range(6):
            self.write(*vLabels[i])
            self.map.width(thickSize)
            self.map.forward((self.geometry.height + 2)/6)
            self.map.width(defaultSize)
            self.map.left(90)
            self.map.forward(self.geometry.width + 2)
            self.map.left(90)
            self.write(vLabels[i + 1][0])
            self.map.right(90)
            self.map.forward(-self.geometry.width - 2)
            self.map.right(90)

        self.write(*vLabels[-1])
        self.map.left(90)
        self.map.width(thickSize)
        self.map.forward(self.geometry.width + 2)
        self.map.left(90)
        self.map.forward(self.geometry.height + 2)
        self.map.width(defaultSize)
        self.write(vLabels[0][0])

        self.map.left(90)
        for i in range(8):
            self.write(*hLabels[i])
            self.map.forward((self.geometry.width + 2)/8)
            self.map.left(90)
            self.map.forward(self.geometry.height + 2)
            self.map.forward(-self.geometry.height - 2)
            self.map.right(90)
        self.write(*hLabels[-1])
        
    def forward(self, distance):
        angle = self.azorTurtle.heading()
        dx = distance*np.cos(angle*np.pi/180)
        dy = distance*np.sin(angle*np.pi/180)
        x = self.azorTurtle.xcor()
        y = self.azorTurtle.ycor()
        if (
            (self.geometry.x < x + dx < self.geometry.x + self.geometry.width) and 
            (self.geometry.y < y + dy < self.geometry.y + self.geometry.height)
            ):
            self.azorTurtle.forward(distance)
            return True
        else:
            return False
        
    def backward(self, distance):
        angle = self.azorTurtle.heading() + 180
        dx = distance*np.cos(angle*np.pi/180)
        dy = distance*np.sin(angle*np.pi/180)
        x = self.azorTurtle.xcor()
        y = self.azorTurtle.ycor()
        if (
            (self.geometry.x < x + dx < self.geometry.x + self.geometry.width) and 
            (self.geometry.y < y + dy < self.geometry.y + self.geometry.height)
            ):
            self.azorTurtle.forward(distance)
            return True
        else:
            return False
        
    def right(self, angle):
        self.azorTurtle.right(angle)

    def left(self, angle):
        self.azorTurtle.left(angle)

    def measure(self, distance, angle):
        maxDistance = Azor.seeDistance        # milimetry
        coefficient = self.geometry.width/400   # [cm]; liczba pikseli/4 metry, zmienić jeśli zmienimy wymiary mapy w metrach
        angleOffset = 90        # dostosować do wartości przyjmowanych przez Azora, 90 jest dla zakresu 0-180
        limit = maxDistance/10*coefficient

        if distance > maxDistance:
            distance = maxDistance
        distance = distance/10

        self.pointer.penup()
        self.pointer.goto(self.azorTurtle.position())
        self.pointer.setheading(self.azorTurtle.heading() - angleOffset + angle)
        self.pointer.pendown()

        limitx = limit*np.cos((self.azorTurtle.heading() - angleOffset + angle)*np.pi/180)
        limity = limit*np.sin((self.azorTurtle.heading() - angleOffset + angle)*np.pi/180)

        leftUpAngle = np.arctan2(self.geometry.y + self.geometry.height - self.pointer.position()[1], 
                                 self.geometry.x - self.pointer.position()[0])*180/np.pi        # tak chyba będzie lepiej

        leftDownAngle = np.arctan2(self.geometry.y - self.pointer.position()[1], 
                                   self.geometry.x - self.pointer.position()[0])*180/np.pi

        rightUpAngle = np.arctan2(self.geometry.y + self.geometry.height - self.pointer.position()[1], 
                                  self.geometry.x + self.geometry.width - self.pointer.position()[0])*180/np.pi

        rightDownAngle = np.arctan2(self.geometry.y - self.pointer.position()[1], 
                                    self.geometry.x + self.geometry.width - self.pointer.position()[0])*180/np.pi


        angles = [leftUpAngle, leftDownAngle, rightUpAngle, rightDownAngle]

        for i in range(4):
            if angles[i] < 0:
                angles[i] += 360

        while self.azorTurtle.heading() - angleOffset + angle >= 360:
            angle -= 360
        # print(angles, "\t", angle, "\t", azorTurtle.heading() - angleOffset + angle)

        if (
                self.pointer.position()[0] + limitx <= self.geometry.x and      # tu chyba tez będzie lepiej
                (self.azorTurtle.heading() - angleOffset + angle >= angles[0] and 
                self.azorTurtle.heading() - angleOffset + angle <= angles[1])
            ):
            limitx = self.pointer.position()[0] - self.geometry.x
            limity = np.tan((self.azorTurtle.heading() - angleOffset + angle)*np.pi/180)*limitx
            limit = np.sqrt(limitx**2 + limity**2)
            # print(limitx, "\t", limity, "\t", limit)
        elif (
                self.pointer.position()[0] + limitx >= self.geometry.x + self.geometry.width and 
                ((self.azorTurtle.heading() - angleOffset + angle <= angles[2] and 
                self.azorTurtle.heading() - angleOffset + angle >= 0) or 
                (self.azorTurtle.heading() - angleOffset + angle <= 360 and 
                self.azorTurtle.heading() - angleOffset + angle >= angles[3]) or 
                (self.azorTurtle.heading() - angleOffset + angle <= 0 and 
                self.azorTurtle.heading() - angleOffset + angle >= angles[3] - 360))
            ):
            limitx = self.geometry.x + self.geometry.width - self.pointer.position()[0]
            limity = np.tan((self.azorTurtle.heading() - angleOffset + angle)*np.pi/180)*limitx
            limit = np.sqrt(limitx**2 + limity**2)
            # print(limitx, "\t", limity, "\t", limit)
        elif (
                self.pointer.position()[1] + limity <= self.geometry.y and 
                ((self.azorTurtle.heading() - angleOffset + angle >= angles[1] and 
                self.azorTurtle.heading() - angleOffset + angle <= angles[3]) or 
                (self.azorTurtle.heading() - angleOffset + angle <= 0))
            ):
            limity = self.pointer.position()[1] - self.geometry.y
            limitx = limity/np.tan((self.azorTurtle.heading() - angleOffset + angle)*np.pi/180)
            limit = np.sqrt(limitx**2 + limity**2)
            # print(limitx, "\t", limity, "\t", limit)
        elif (
                self.pointer.position()[1] + limity >= self.geometry.y + self.geometry.height and 
                (self.azorTurtle.heading() - angleOffset + angle >= angles[2] and 
                self.azorTurtle.heading() - angleOffset + angle <= angles[0])
            ):
            limity = self.geometry.y + self.geometry.height - self.pointer.position()[1]
            limitx = limity/np.tan((self.azorTurtle.heading() - angleOffset + angle)*np.pi/180)
            limit = np.sqrt(limitx**2 + limity**2)
            # print(limitx, "\t", limity, "\t", limit)
        
        if distance*coefficient <= limit:
            self.pointer.color("green")
            self.pointer.forward(distance*coefficient)
            self.pointer.color("red")
            self.pointer.forward(limit - distance*coefficient)
        else:
            self.pointer.color("green")
            self.pointer.forward(limit)


if __name__ == "__main__":
    screen = turtle.Screen()
    screen.setup(900, 900)
    screen.bgcolor(30/255, 30/255, 31/255)
    screen.tracer(0)

    map = Map(-300, -150, 600, 450)

    map.draw()

    screen.update()
    turtle.exitonclick()