from common import *

with open(infile('05.txt')) as f:
    nums = [get_all_ints(line) for line in f]

x_max = max(max(a, b) for a, _, b, _ in nums)
y_max = max(max(c, d) for _, c, _, d in nums)

def sign(x):
    return 0 if x == 0 else x // abs(x)

def dump(grid):
    for row in grid:
        for x in row:
            print(x if x > 0 else '.', end='')
        print()

grid1 = [[0] * (x_max + 1) for _ in range(y_max + 1)]
grid2 = [[0] * (x_max + 1) for _ in range(y_max + 1)]
for x1, y1, x2, y2 in nums:
    dx = x2 - x1
    dy = y2 - y1
    for i in range(max(abs(dx), abs(dy))+1):
        x = x1 + sign(dx) * i
        y = y1 + sign(dy) * i
        if (dx == 0 or dy == 0):
            grid1[y][x] += 1
        grid2[y][x] += 1

p1 = sum(sum(x > 1 for x in row) for row in grid1)
p2 = sum(sum(x > 1 for x in row) for row in grid2)
print("Part 1:", p1)
print("Part 2:", p2)
