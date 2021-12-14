from common import *

with open(infile('02.txt')) as f:
    lines = f.readlines()

def part1():
    x, y = 0, 0
    for line in lines:
        match list(parse('{} {:d}', line.strip())):
            case 'forward', val: x += val
            case    'down', val: y += val
            case      'up', val: y -= val
    return x * y

def part2():
    x, y, aim = 0, 0, 0
    for line in lines:
        match list(parse('{} {:d}', line.strip())):
            case 'forward', val: x += val; y += aim * val
            case    'down', val: aim += val
            case      'up', val: aim -= val
    return x * y

print("Part 1:", part1())
print("Part 2:", part2())