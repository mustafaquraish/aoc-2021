from common import *
from math import sqrt

with open(infile('15.txt'), 'r') as f:
    g, SX, SY = parse_block_grid(f.read())
    grid1 = grid_to_dict(g)

def dijkstras(grid, start, end):
    heap, seen, mins = [(0,start)], set(), {start: 0}
    while heap:
        cost, cur = heappop(heap)
        seen.add(cur)
        if cur == end: 
            return cost

        for n in neighbours(*cur):
            if n not in grid or n in seen: 
                continue
            new_cost = cost + grid[n]
            if n not in mins or new_cost < mins[n]:
                mins[n] = new_cost
                heappush(heap, (new_cost, n))
    assert False, "No path found"

grid2 = {}
for (x, y), val in grid1.items():
    for i in range(5):
        for j in range(5):
            grid2[(SX*i+x, SY*j+y)] = (val+i+j-1)%9+1

print("Part 1", dijkstras(grid1, (0,0), (SX-1, SY-1)))
print("Part 2", dijkstras(grid2, (0,0), (SX*5-1, SY*5-1)))
