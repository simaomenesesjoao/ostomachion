import sys
import matplotlib.pyplot as plt
import numpy as np
import re


class Polygon:
    def __init__(self, points):
        self.points = points
        if len(points)>0:
            self.points.append(points[0])
            self.xs = [point[0] for point in self.points]
            self.ys = [point[1] for point in self.points]
        else:
            self.xs = []
            self.ys = []


input = " ".join(sys.argv[1:])

# print(input)

used_polygons = re.findall(r'\[([^\]]+)\]', input)
used_polygons_list = []
for used_polygon in used_polygons:
    
    points = re.findall(r'\(([^\)]+)\)', used_polygon)
    points_list = []
    for point in points:
        x, y = point.split(",")
        points_list.append([float(x),float(y)])

    # print(points_list)
    used_polygons_list.append(Polygon(points_list))


fig, ax = plt.subplots(figsize=(7,7))

ax.set_xlim([-0.5,12.5])
ax.set_ylim([-0.5,12.5])
for pp, poly in enumerate(used_polygons_list):

    ax.fill(poly.xs, poly.ys)

    # xs, ys = state.frame.xs, state.frame.ys

    # ax.plot(xs, ys, color='k')
    # for x, y, xn, yn in zip(xs[:-1], ys[:-1], xs[1:], ys[1:]):
    #     ax.arrow(x, y, (xn-x)/2, (yn-y)/2, head_width = 0.6, head_length = 0.6, fc='k', ec='k')
        
plt.show()