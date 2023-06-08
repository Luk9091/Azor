import turtle
import radar

radar_size = 400
radar_position = (0, 0)





compass_turtle = turtle.Turtle()
compass_turtle.hideturtle()
compass_turtle.speed(100)

def arrow(pointer, x):
    # pointer = turtle.Turtle()
    mux = 100
    pointer.down()
    pointer.forward(mux*x)
    pointer.left(135)
    pointer.forward(mux/10*x)
    pointer.up()
    pointer.right(180)
    pointer.forward(mux/10*x)
    pointer.down()
    pointer.right(90)
    pointer.forward(mux/10*x)
    pointer.up()
    
    




def compass(x, y, z):
    compass_turtle.clear()
    compass_turtle.up()

    compass_turtle.color(0, 1, 0)
    compass_turtle.goto(-300, -250)
    compass_turtle.setheading(0)
    compass_turtle.left(0)
    arrow(compass_turtle, x)
    
    compass_turtle.color(1, 0, 0)
    compass_turtle.goto(-300, -250)
    compass_turtle.setheading(0)
    compass_turtle.left(45)
    arrow(compass_turtle, y/1.41)

    compass_turtle.color(0, 0, 1)
    compass_turtle.goto(-300, -250)
    compass_turtle.setheading(0)
    compass_turtle.left(90)
    arrow(compass_turtle, z)



if __name__=="__main__":


    radar.draw(radar_size, radar_position)
    
    run = True
    step = 3
    while run:
        # communication.measure("10")
        radar.measure(step, radar_size, radar_position)
        run = int(input("? "))

    turtle.exitonclick()
