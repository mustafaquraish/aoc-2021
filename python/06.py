from common import *

with open(infile('06.txt'), 'r') as f:
    data = [0 for i in range(11)]
    for v in get_all_ints(f.read()):
        data[v] += 1

def solve(iters):
    vals = data.copy()
    for i in range(iters):
        vals[9] += vals[0]
        vals[7] += vals[0]
        vals[:-1] = vals[1:]
    return sum(vals)

print("Part 1:", solve(80))
print("Part 1:", solve(256))