from common import *

with open(infile('11.txt')) as f:
    data = [[int(x) for x in line.strip()] for line in f]
    data = grid_to_dict(data)

def dump(grid):
    for j in range(10):
        for i in range(10):
            print(grid[(i, j)], end="")
        print()

def takeStep(grid):
    for k in grid:
        grid[k] += 1

    flashed = 0
    changed = True
    while changed:
        changed = False
        for k, val in grid.items():
            if val > 9:
                grid[k] = 0
                flashed += 1
                changed = True
                for n in neighbours(*k, diag=True):
                    if n in grid and grid[n] > 0:
                        grid[n] += 1
    return flashed


def part1():
    grid = data.copy()
    count = 0
    for _ in range(100):
        count += takeStep(grid)
    print("Part 1:", count)

def part2():
    grid = data.copy()
    count = 1
    res = takeStep(grid)
    while res != 100:
        res = takeStep(grid)
        count += 1
    print("Part 2:", count)


def animate():
    from PIL import Image
    from random import randint

    grid = {}
    SIZE = 150
    for j in range(SIZE):
        for i in range(SIZE):
            grid[(i, j)] = randint(1, 9) 

    FRAMES = 120
    for i in tqdm(range(FRAMES)):
        im = Image.new('L', (SIZE, SIZE))
        im.putgrid([grid[(i, j)]*25 for j in range(SIZE) for i in range(SIZE)])
        im = im.resize((500, 500), Image.NEAREST)
        im.save("frames/frame%03d.png" % i)
        takeStep(grid)

part1()
part2()
# animate()