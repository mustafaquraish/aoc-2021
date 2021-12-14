from common import *

with open(infile('12.txt')) as f:
    lines = [line.strip().split('-') for line in f]
data = {}
for a, b in lines:
    data[a] = data.get(a, []) + [b]
    data[b] = data.get(b, []) + [a]

def all_paths(path, cond):
    if path[-1] == 'end':
        yield path
    for n in data[path[-1]]:
        if n.isupper() or cond(n, path):
            yield from all_paths((*path, n), cond)

def part1():
    cond = lambda n, path: n not in path
    print("Part 1:", ilen(all_paths(['start'], cond)))

def part2():
    def cond(n, path):
        if n not in path: return True
        if n in ('start', 'end'): return False
        return not any(path.count(c) == 2 for c in path if c.islower())
    print("Part 2:", ilen(all_paths(['start'], cond)))

part1()
part2()