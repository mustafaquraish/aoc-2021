from common import *

with open(infile('12.txt')) as f:
    lines = [line.strip().split('-') for line in f]
data = {}
for a, b in lines:
    data[a] = data.get(a, []) + [b]
    data[b] = data.get(b, []) + [a]

@cache
def count_paths(cur="start", twice=0, visited=frozenset({'start'})):
    if cur == "end":
        return 1-twice, 1
    
    c1, c2 = 0, 0
    for n in data[cur]:
        rc1 = rc2 = 0
        if not n in visited:
            if n.islower():
                rc1, rc2 = count_paths(n, twice, visited | {n})
            else:
                rc1, rc2 = count_paths(n, twice, visited)
        elif twice==0 and n != "start":
            rc1, rc2 = count_paths(n, 1, visited)
        c1 += rc1
        c2 += rc2
    return c1, c2

c1, c2 = count_paths()
print("Part 1:", c1)
print("Part 2:", c2)