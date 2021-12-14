from common import *

with open(infile('01.txt')) as f:
    lines = get_all_ints(f.read())

count = 0
for a, b in zip(lines, lines[1:]):
    if a < b:
        count += 1
print("Part 1:", count)

count = 0
for a, b, c, d in zip(lines, lines[1:], lines[2:], lines[3:]):
    if a+b+c < b+c+d:
        count += 1
print("Part 2:", count)
