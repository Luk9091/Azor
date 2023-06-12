import turtle
import geometry
import numpy as np

class Arrow:
    def __init__(self, x, y, width, height):
        self.arrow = turtle.Turtle()
        self.geometry = geometry.geometry()
        self._heading = 0
        
        self.arrow.color("white")
        self.arrow.hideturtle()

        self.geometry.x = x
        self.geometry.y = y
        self.geometry.width = width
        self.geometry.height = height

        self._functionHandler = None


    def move(self, x, y):
        # if -1 < x < 1 and -1 < y < 1:
        #     x = x*turtle.window_width()
        #     y = y*turtle.window_height()
        self.geometry.x = x
        self.geometry.y = y

    def resize(self, width, height):
        if -1 < width < 1:
            width = width * geometry.geometry.width

        if -1 < height < 1:
            height = height*geometry.geometry.height
        
        self.geometry.width = width
        self.geometry.height = height


    def home(self):
        self.arrow.penup()
        self.arrow.goto(self.geometry.x, self.geometry.y)
        self.arrow.setheading(self._heading)
        self.arrow.pendown()

    def rotate(self, angle = 0):
        self._heading = angle

    def flip(self):
        self._heading = self._heading + 180
    
    def color(self, color :str):
        self.arrow.color(color)

    def getGeometry(self):
        return self.geometry

    def onClick(self, x, y):
        if (
            self._functionHandler != None and
            self.geometry.x-self.geometry.width/2 < x < self.geometry.x+self.geometry.width/2 and
            self.geometry.y-self.geometry.height/2 < y < self.geometry.y+self.geometry.height/2
        ):
            self._functionHandler()
    
    def setFunctionHandlerOnClick(self, fun):
        self._functionHandler = fun



class Big(Arrow):
    def __init__(self, x, y, width, height):
        super().__init__(x, y, width, height)

    def draw(self):
        self.arrow.clear()
        self.home()
        # self.arrow.forward(self.geometry.width)
        # self.home()
        # self.arrow.right(90)
        # self.arrow.forward(self.geometry.height)
        # self.home()
        
        self.arrow.begin_fill()
        if(self.geometry.width > self.geometry.height):
            self.arrow.forward(self.geometry.width/(self.geometry.height/self.geometry.width*10))

        self.arrow.penup()
        self.arrow.left(90)
        self.arrow.forward(self.geometry.height/2)
        self.arrow.pendown()
        self.arrow.right(135)       #45 i wczesniejszy obrot o 90
        self.arrow.forward(self.geometry.radius/np.sqrt(2))
        self.arrow.right(135)
        self.arrow.forward(self.geometry.width/4)
        self.arrow.left(90)
        self.arrow.forward(self.geometry.height/2)
        self.arrow.right(90)
        self.arrow.forward(self.geometry.width/2)
        self.arrow.right(90)
        self.arrow.forward(self.geometry.height/2)
        self.arrow.left(90)
        self.arrow.forward(self.geometry.width/4)
        self.arrow.right(135)
        self.arrow.forward(self.geometry.radius/np.sqrt(2))
        self.arrow.end_fill()


class BigDown(Arrow):
    def __init__(self, x, y, width, height, flipX = False):
        super().__init__(x, y, width, height)
        self._Xflip = flipX

    def flipX(self):
        self._Xflip = not self._Xflip
        
    def draw(self):
        self.arrow.clear()
        # self.home()
        # self.arrow.forward(self.geometry.width)
        # self.home()
        # self.arrow.right(90)
        # self.arrow.forward(self.geometry.height)
        self.home()

        self.arrow.begin_fill()
        self.arrow.penup()
        self.arrow.goto(self.geometry.x, self.geometry.y - self.geometry.height/2)
        self.arrow.pendown()
        
        if not self._Xflip:
            self.arrow.left(45)
            self.arrow.forward(self.geometry.radius/np.sqrt(2))
            self.arrow.right(135)
            self.arrow.forward(self.geometry.height/4)
            self.arrow.left(90)
            self.arrow.forward(self.geometry.width/4)
            self.arrow.circle(-self.geometry.radius/4, extent=90)
            self.arrow.forward(self.geometry.height/2)
        
            self.home()
            self.arrow.penup()
            self.arrow.goto(self.geometry.x, self.geometry.y - self.geometry.height/2)
            self.arrow.pendown()
            self.arrow.right(45)
            self.arrow.forward(self.geometry.radius/np.sqrt(2))
            self.arrow.left(135)
            self.arrow.forward(self.geometry.height/4)
            self.arrow.right(90)
            self.arrow.circle(-self.geometry.radius/8, extent=90)
            self.arrow.forward(self.geometry.height/8)
            self.arrow.left(90)
            self.arrow.forward(self.geometry.width*3/8)

        else:
            self.arrow.left(135)
            self.arrow.forward(self.geometry.radius/np.sqrt(2))
            self.arrow.left(135)
            self.arrow.forward(self.geometry.height/4)
            self.arrow.right(90)
            self.arrow.forward(self.geometry.width/4)
            self.arrow.circle(self.geometry.radius/4, extent=90)
            self.arrow.forward(self.geometry.height/2)

            self.home()
            self.arrow.penup()
            self.arrow.goto(self.geometry.x, self.geometry.y - self.geometry.height/2)
            self.arrow.pendown()
            self.arrow.right(135)
            self.arrow.forward(self.geometry.radius/np.sqrt(2))
            self.arrow.right(135)
            self.arrow.forward(self.geometry.height/4)
            self.arrow.left(90)
            self.arrow.circle(self.geometry.radius/8, extent=90)
            self.arrow.forward(self.geometry.height/8)
            self.arrow.right(90)
            self.arrow.forward(self.geometry.width*3/8)
        self.arrow.end_fill()






if __name__ == "__main__":
    screen = turtle.Screen()
    screen.setup(900, 900)
    screen.bgcolor(30/255, 30/255, 31/255)
    screen.tracer(0)
    

    forward = Big(0, 160, 80, 80)
    
    left = Big(160, 0, 80, 80)
    left.rotate(-90)

    right = Big(-160, 0, 80, 80)
    right.rotate(90)

    backward = Big(0, -160, 80, 80)
    backward.flip()


    forward.draw()
    left.draw()
    right.draw()
    backward.draw()

    rightDown = BigDown(-160, 160, 80, 80)
    leftDown  = BigDown(160, 160, 80, 80)
    leftDown.flipX()
    

    rightDown.draw()
    leftDown.draw()

    screen.update()
    turtle.exitonclick()