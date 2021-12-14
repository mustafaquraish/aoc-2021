from common import *
from math import prod

with open(infile('09.txt')) as f:
    lines = f.read().splitlines()

grid = defaultdict(lambda: 9)
size_x, size_y = len(lines[0]), len(lines)
for j, line in enumerate(lines):
    for i, c in enumerate(line):
        grid[(i,j)] = int(c)

lowest_points = {}
for i in range(size_x):
    for j in range(size_y):
        vals = [grid[n] for n in neighbours(i, j)]
        if min(vals) > grid[(i,j)]:
            lowest_points[(i,j)] = grid[(i,j)]

p1 = sum(lowest_points.values()) + len(lowest_points)
print("Part 1:", p1)

def dfs(coords):
    stack = [coords]
    visited = set()
    while stack:
        cur = stack.pop()
        visited.add(cur)
        for neighb in neighbours(*cur):
            if grid[cur] < grid[neighb] < 9 and neighb not in visited:
                stack.append(neighb)
    return len(visited)

basins = sorted(map(dfs, lowest_points))
print("Part 2:", prod(basins[-3:]))