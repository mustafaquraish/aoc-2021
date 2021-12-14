from common import *

with open(infile('10.txt')) as f:
    data = list(map(str.strip, f.readlines()))

OTHER = dict(zip(")]}>([{<","([{<)]}>"))
SCORE_1 = {')': 3, ']': 57, '}': 1197, '>': 25137}
SCORE_2 = {')': 1, ']': 2, '}': 3, '>': 4}

def part1():
    count = 0
    for line in data:
        stack = []
        bad = None
        for c in line:
            if c in "({[<":
                stack.append(c)
            elif c in ")}]>":
                if not stack or OTHER[c] != stack[-1]:
                    bad = c
                    break
                stack.pop()
        if bad is not None:
            count += SCORE_1[bad]
    print("Part 1:", count)


def part2():
    counts = []
    for line in data:
        stack = []
        for c in line:
            if c in "({[<":
                stack.append(c)
            elif c in ")}]>":
                if not stack or OTHER[c] != stack[-1]:
                    stack = []
                    break
                stack.pop()
        if stack != []:
            sc = 0
            for c in stack[::-1]:
                sc *= 5
                sc += SCORE_2[OTHER[c]]
            counts.append(sc)
    print("Part 2:", stat.median(counts))

part1()
part2()