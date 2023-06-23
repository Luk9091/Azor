import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

def pochodna(x, y):
    diff = []

    for i in range(len(x)):
        if i == 0:
            diff.append((y[i + 1] - y[i]) / (x[i + 1] - x[i]))
        elif i == len(x) - 1:
            diff.append((y[i] - y[i - 1]) / (x[i] - x[i -1]))
        else:
            diff.append((y[i + 1] - y[i - 1]) / (x[i + 1] - x[i - 1]))
            
    return diff


data = pd.read_csv("measures2.csv")

angle = data["angle"].values
distance = data["value"].values

diff = pochodna(angle, distance)
diff2 = pochodna(angle, diff)


fix = []
fix2 = []

fix.append(distance[0])

for i in range(1, len(angle)-1):
    if diff[i] == 0:
        fix.append((distance[i-1] + distance [i+1])/2)
    else:
        fix.append(distance[i])

fix.append(distance[len(angle)-1])




plt.plot(angle, distance)
plt.plot(angle, diff)

plt.plot(angle, fix)


plt.grid()
plt.show()
