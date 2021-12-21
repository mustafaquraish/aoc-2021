contents = open("input/21.txt").read().splitlines()

import functools

pos = list(map(int, [l.split(": ")[1] for l in contents]))
pos = [p-1 for p in pos]
score1 = 0
score2 = 0
mults = {
    3: 1, 
    4: 3, 
    5: 6, 
    6: 7, 
    7: 6, 
    8: 3, 
    9: 1
}

def cache(fn):
    cache = {}
    def wrapper(*args):
        if args in cache:
            return cache[args]
        result = fn(*args)
        cache[args] = result
        return result
    return wrapper

global p1, p2
p1 = 0
p2 = 0

@functools.lru_cache(maxsize=None)
def calculate(universes, p1pos, p1score, p2pos, p2score, turn, p1, p2):
    #global p1, p2
    if turn % 2 == 1:
        for final in range(3, 10):
            if turn == 1: print(final)
            temppos = (p1pos + final) % 10
            tempscore = p1score + temppos + 1
            copy = int(universes * mults[final])
            if tempscore >= 21:
                
                p1 += copy
            else:
                p1c, p2c = calculate(int(copy), p1pos, p1score, temppos, tempscore, turn+1, p1, p2)
                p1 += p1c
                p2 += p2c

    else:
        for final in range(3, 10):
            temppos = (p2pos + final) % 10
            tempscore = p2score + temppos + 1
            copy = int(universes * mults[final])
            if tempscore >= 21:
                p2 += copy
            else:
                p1c, p2c = calculate(int(copy), p1pos, p1score, temppos, tempscore, turn+1, p1, p2)
                p1 += p1c
                p2 += p2c
    return p1, p2


calculate(1, pos[0], 0, pos[1], 0, 1, 1, 1)

#print(max([sum(p1), sum(p2)]))
print(p1, p2) 