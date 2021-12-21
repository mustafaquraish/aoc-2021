from common import *

with open(infile('20.txt'), 'r') as f:
    text = f.read().strip()

_, a, _, b = get_all_ints(text)

def part1(a, b):
    sa, sb = 0, 0
    count = 0
    while max(sa, sb) < 1000:
        dice = count % 100 + (count + 1) % 100 + (count + 2) % 100
        count += 3
        a = (a + dice + 3) % 10
        sa += a + 1
        a, sa, b, sb = b, sb, a, sa
    return min(sa, sb) * count

@cache
def part2(a, b, sa=0, sb=0):
    if max(sa, sb) >= 21:
        return (sa>sb,sb>sa)
    wa, wb = 0, 0
    for dice in product([1,2,3],repeat=3):
        ta = (a + sum(dice)) % 10
        rb, ra = part2(b, ta, sb, sa + ta + 1)
        wa, wb = wa + ra, wb + rb
    return (wa, wb)

print("Part 1:", part1(a-1, b-1))
print("Part 2:", max(part2(a-1, b-1)))


