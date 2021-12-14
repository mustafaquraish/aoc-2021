from common import *

with open(infile('14.txt')) as f:
    line, _, *rest = get_lines(f)
    first = line[0]
    start = Counter(map("".join, zip(line, line[1:])))
    data = dict(l.split(' -> ') for l in rest)

def step(mp):
    n = Counter()
    for k, v in mp.items():
        n[k[0]+data[k]] += v
        n[data[k]+k[1]] += v
    return n

def solve(iters):
    data = start.copy()
    for _ in range(iters):
        data = step(data)

    counts = Counter()
    for k, v in data.items():
        counts[k[1]] += v
    counts[first] += 1
    counts = counts.most_common()

    return counts[0][1] - counts[-1][1]

print("Part 1:", solve(10))
print("Part 2:", solve(40))
