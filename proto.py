from math import cos, floor, sin

import numpy as np

level_size = 15
level = np.zeros((level_size, level_size))

r = 5
qlen = 2 * r - 3
x = 7
y = 8
level[x, y] = 9
xs = []
ys = []
pi = 3.141592653589793

slice_deg = 0.5 * pi / qlen

for i in range(qlen + 1):
    rads = i * slice_deg

    new_x = floor(x + cos(rads) * r)
    new_y = floor(y + sin(rads) * r)

    xs.append(new_x)
    ys.append(new_y)

    xs.append(new_x)
    ys.append(2 * y - new_y)

    xs.append(2 * x - new_x)
    ys.append(new_y)

    xs.append(2 * x - new_x)
    ys.append(2 * y - new_y)

level[xs, ys] = 2
print(level)
