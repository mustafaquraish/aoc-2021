import sys, functools, itertools

with open(sys.argv[1]) as f:
    def handle(v, d=0):
        return [[v, d]] if isinstance(v, int) else handle(v[0], d+1) + handle(v[1], d+1)
    lines = [handle(eval(line)) for line in f.read().split('\n')]

def split(line):
    for i, (v, d) in enumerate(line):
        if v >= 10:
            line[i:i+1] = [[v//2,d+1], [(v+1)//2,d+1]]
            return True

def explode(line):
    for i, ((a, ad), (b, bd)) in enumerate(zip(line, line[1:])):
        if (ad == bd) and bd > 4:
            if i-1 >= 0: line[i-1][0] += a
            if i+2 < len(line): line[i+2][0] += b
            line[i:i+2] = [[0, ad-1]]
            return True

def add(a, b):
    cur = [[x,y+1] for x,y in a] + [[x,y+1] for x,y in b]
    while explode(cur) or split(cur): 
        pass
    return cur

def mag(line):
    while len(line) > 1:
        for i, ((a, ad), (b, bd)) in enumerate(zip(line, line[1:])):
            if ad == bd:
                line[i:i+2] = [[a*3 + b*2, ad-1]]
                break
    return line[0][0]

print("Part 1:", mag(functools.reduce(add, lines)))
print("Part 2:", max(mag(add(a, b)) for a,b in itertools.permutations(lines, 2)))
