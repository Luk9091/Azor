import pandas as pd
import numpy as np

div = 4*2048
gravity = 9.80665


df = pd.read_csv("axis.csv")
average_X = 0
average_Y = 0
average_Z = 0

for i in range(0, len(df)):
    average_X = average_X + df["X"].values[i]
    average_Y = average_Y + df["Y"].values[i]
    average_Z = average_Z + (df["Z"].values[i] - div)

print(f"X ave: {average_X}")
print(f"Y ave: {average_Y}")
print(f"Z ave: {average_Y}")

average_X = average_X/len(df)
average_Y = average_Y/len(df)
average_Z = average_Z/len(df)


print(f"X ave data: {average_X}")
print(f"Y ave data: {average_Y}")
print(f"Z ave data: {average_Y}")

print(f"g_X: {average_X/div*gravity}")
print(f"g_Y: {average_Y/div*gravity}")
print(f"g_Z: {average_Z/div*gravity}")

print(df["Z"].values[0]/div*gravity)
