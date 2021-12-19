from common import *

with open(infile('.txt'), 'r') as f:
    text = f.read().strip()
    lines = text.split('\n')

data = get_all_ints(text)

def part1():
    pass


def part2():
    pass
    

print("Part 1:", part1())
print("Part 2:", part2())
