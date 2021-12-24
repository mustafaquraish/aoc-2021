import re

def intersect(a, b):
    return (max(a[0], b[0]), min(a[1], b[1]),
            max(a[2], b[2]), min(a[3], b[3]),
            max(a[4], b[4]), min(a[5], b[5]))

def overlaps(a, b):
    return not (a[1] < b[0] or a[0] > b[1] or
                a[3] < b[2] or a[2] > b[3] or
                a[5] < b[4] or a[4] > b[5])

def area(c):
    return (c[1] - c[0] + 1) * (c[3] - c[2] + 1) * (c[5] - c[4] + 1)

with open('input/22.txt') as f:
    data = [(line[1]=='n', [int(x) for x in re.findall('-?\d+', line)]) for line in f]

def solve(part1):
    cubes = []
    bounds = (-50,50,-50,50,-50,50)
    for OP, a in data:
        if part1 and not overlaps(bounds, a): continue
        new_cubes = []
        for b, w in cubes:
            if overlaps(a, b):
                new_cubes.append((intersect(a, b), -w))
        if OP:
            new_cubes.append((a, 1))
        cubes += new_cubes

    return sum(area(c)*w for c,w in cubes)

print("Part 1:", solve(True))
print("Part 2:", solve(False))

exit(0)
"""
Horrible, code golfed version:
"""
for L,(O,a)in enumerate([K:=list(open('input/22.txt'))][:0]+[(l[1]=='n',[int(x)for x in __import__('re').findall('-?\d+',l)])for l in K]+[C:=[]][:0]):(C:=C+[((max(a[0],b[0]),min(a[1],b[1]),max(a[2],b[2]),min(a[3],b[3]),max(a[4],b[4]),min(a[5],b[5])),-w) for b,w in C if (a[1]>=b[0]and a[0]<=b[1]and a[3]>=b[2]and a[2]<=b[3]and a[5]>=b[4]and a[4]<=b[5])]+[(a,1)]*O)and(L==len(K)-1)and(print(sum(((c[1]-c[0]+1)*(c[3]-c[2]+1)*(c[5]-c[4]+1))*w for c,w in C)))
