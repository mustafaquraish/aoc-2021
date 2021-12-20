from common import *

with open(infile('20.txt')) as f:
    mapping, rest = f.read().replace('#', '1').replace('.', '0').split('\n\n')
mapping = [int(c) for c in mapping]
image, _, _ = parse_block_grid(rest)
default_value = 0

def neighbourhood(i, j):
    return ((i-1, j-1), (i-1,  j ), (i-1, j+1),
            ( i , j-1), ( i ,  j ), ( i , j+1),
            (i+1, j-1), (i+1,  j ), (i+1, j+1))

def enhance(grid):
    max_i, max_j = len(grid), len(grid[0])

    def find_value(ci, cj):
        k = 0
        for i, j in neighbourhood(ci, cj):
            v = default_value
            if 0 <= i < max_i and 0 <= j < max_j:
                v = grid[i][j]
            k = k << 1 | v
        return mapping[k]

    new = [
        [ find_value(i, j) for j in range(-1, max_j+1) ]
        for i in range(-1, max_i+1)
    ]

    # Careful! in this case the whole infinite grid outside changes values
    # every iteration. Assumes the pair is always (0,1) or (1,0) only.
    global default_value
    if mapping[0] == 1 and mapping[-1] == 0:
        default_value = 1 - default_value

    return new

def solve(iters):
    a = image
    for _ in range(iters):
        a = enhance(a)
    return sum(sum(row) for row in a)

print("Part 1:", solve(2))
print("Part 2:", solve(50))