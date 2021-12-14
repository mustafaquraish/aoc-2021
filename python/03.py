from common import *

with open(infile('03.txt')) as f:
    lines = f.read().split()

def part1():
    transpose = [[row[i] for row in lines] for i in range(len(lines[0]))]    
    g = "".join(["01"[row.count('0') < row.count('1')] for row in transpose])
    e = "".join(["10"[row.count('0') < row.count('1')] for row in transpose])
    return int(g, 2)*int(e, 2)

def part2():
    L = lines[:]
    cur = 0
    ox = ''
    while len(L) > 1:
        col = [row[cur] for row in L]
        if col.count('0') <= col.count('1'):
            ox += '1'
        else:
            ox += '0'
        L = [row for row in L if row[cur] == ox[-1]]
        cur += 1
    ox = L[0]

    L = lines[:]
    cur = 0
    ca = ''
    while len(L) > 1:
        col = [row[cur] for row in L]
        if col.count('0') > col.count('1'):
            ca += '1'
        else:
            ca += '0'
        L = [row for row in L if row[cur] == ca[-1]]
        cur += 1
    ca = L[0]

    return int(ca, 2)*int(ox, 2)

print("Part 1:", part1())
print("Part 2:", part2())