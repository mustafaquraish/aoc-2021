from common import *
import os 

from builtins import print

with open(infile('.txt'), 'r') as f:
    grid, X, Y = parse_block_grid(f.read(), str)

def print_grid(g):
    print("\n".join("".join(row) for row in g[:len(g)//2]))

def step(cur):
    print(chr(27) + "[2J")
    changed = False
    updated = [[i for i in row] for row in cur]
    for d, di, dj in (('>', 0, 1), ('v', 1, 0)):
        for i in range(Y):
            for j in range(X):
                if cur[i][j] != d:
                    continue
                ni, nj = (i + di) % Y, (j + dj) % X
                if cur[ni][nj] == '.':
                    updated[ni][nj] = d
                    updated[i][j] = '.'
                    changed = True
        cur = [[i for i in row] for row in updated]
    print_grid(cur)
    return cur if changed else None

from time import sleep

# print_grid(grid)
count = 0
while grid:
    grid = step(grid)
    sleep(0.1)
    count += 1
print("Part 1:", count)
