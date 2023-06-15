import turtle
import arrow
import geometry
import circle


forward   = arrow.Big    (0, 0, 200, 200)
left      = arrow.Big    (0, 0, 200, 200)
right     = arrow.Big    (0, 0, 200, 200)
backward  = arrow.Big    (0, 0, 200, 200)
rightDown = arrow.BigDown(0, 0, 200, 200)
leftDown  = arrow.BigDown(0, 0, 200, 200)
button    = circle.Circle(0, 0, 100)


left.rotate(90)
right.rotate(-90)
backward.flip()
rightDown.flipX()


    
    



def resize(width, height):
    forward.resize(width, height)
    left.resize(width, height)
    right.resize(width, height)
    backward.resize(width, height)

    rightDown.resize(width, height)
    leftDown.resize(width, height)

    button.resize(width)

def move(x, y):
    if -1 < x < 1 and -1 < y < 1:
        x = x*turtle.window_width()
        y = y*turtle.window_height()

    geometry = forward.getGeometry()
    forward.move (x, y + 2*geometry.height)
    left.move    (x - 2*geometry.width, y)
    right.move   (x + 2*geometry.width, y)
    backward.move(x, y - 2*geometry.height)

    leftDown.move (x - 2*geometry.width, y + 2*geometry.height)
    rightDown.move(x + 2*geometry.width, y + 2*geometry.height)

    button.move(x, y)



def draw():
    forward.draw()
    left.draw()
    right.draw()
    backward.draw()
    rightDown.draw()
    leftDown.draw()
    button.draw()


def onClick(x, y):
    forward.onClick(x, y)
    backward.onClick(x, y)
    left.onClick(x, y)
    right.onClick(x, y)
    leftDown.onClick(x, y)
    rightDown.onClick(x, y)
    button.onClick(x, y)


if __name__=="__main__":
    screen = turtle.Screen()
    screen.setup(900, 900)
    screen.bgcolor(30/255, 30/255, 31/255)
    screen.tracer(0)
    
    resize(0.25, 0.25)
    move(0, 0)
    draw()
    
    screen.update()
    turtle.exitonclick()