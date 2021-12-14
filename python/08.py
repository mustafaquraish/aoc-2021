from common import *

with open(infile('08.txt')) as fh:
    data = []
    for line in fh:
        l, r = line.strip().split(' | ')
        data.append((l.split(), r.split()))

def get_output_values(train, test):
    D = {}
    train = [set(x) for x in sorted(train, key=len)]    
    get = lambda cond: next(i for i in train if cond(i))

    D[1] = train[0]
    D[7] = train[1]
    D[4] = train[2]
    D[8] = train[-1]
    D[3] = get(lambda i:  len(D[7] & i) == 3 and len(i) == 5  )
    D[9] = get(lambda i:  len(D[3] & i) == 5 and len(i) == 6  )
    D[6] = get(lambda i:  len(D[1] & i) == 1 and len(i) == 6  )
    D[5] = get(lambda i:  len(D[6] & i) == 5 and len(i) == 5  )
    D[0] = get(lambda i:  i != D[6] and i != D[9] and len(i) == 6  )
    D[2] = get(lambda i:  i != D[3] and i != D[5] and len(i) == 5  )

    return [next(i for i, d in D.items() if set(item) == d) for item in test]


p1, p2 = 0, 0
for train, test in data:
    for item in test:
        if len(item) in (2, 3, 4, 7):
            p1 += 1
    vals = get_output_values(train, test)
    p2 += int("".join(map(str, vals)))

print("Part 1:", p1)
print("Part 2:", p2)

