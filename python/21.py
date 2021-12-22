from common import *

with open(infile('20.txt'), 'r') as f:
    text = f.read().strip()

_, a, _, b = get_all_ints(text)

def part1(a, b):
    sa, sb = 0, 0
    dice = iter(cycle(range(1, 101)))
    count = 0
    while sb < 1000:
        roll = next(dice) + next(dice) + next(dice)
        count += 3
        a = (a + roll) % 10
        a, sa, b, sb = b, sb, a, sa + a + 1
    return sa * count

@cache
def part2(a, b, sa=0, sb=0):
    if sb >= 21:
        return (0, 1)
    wa, wb = 0, 0
    weights = (1, 3, 6, 7, 6, 3, 1)
    for i in range(7):
        ta = (a + i + 3) % 10
        rb, ra = part2(b, ta, sb, sa + ta + 1)
        wa, wb = wa + ra * weights[i], wb + rb * weights[i]
    return (wa, wb)

# Code golf'ed part 2, 132 chars
P=cache(lambda a,b,A=0,B=0:1j*(B>20)or sum((-1j*P(b,(a+x+3)%10,B,A+1+(a+x+3)%10)*(1,3,6,7,6,3,1)[x]).conjugate()for x in range(7)))

print("Part 1:", part1(a-1, b-1))
print("Part 2:", max(part2(a-1, b-1)))
