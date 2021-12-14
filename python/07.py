from common import *

with open(infile('07.txt')) as f:
    nums = get_all_ints(f.read())

p1 = min(sum(abs(i-x) for i in nums) for x in nums)
p2 = min(sum((d:=abs(i-x))*(d+1)//2 for i in nums) for x in range_span(nums))

print("Part 1:", p1)
print("Part 2:", p2)