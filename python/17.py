from common import *

with open(infile('17.txt'), 'r') as f:
    text = f.read().strip()

x1, x2, y1, y2 = parse('target area: x={:d}..{:d}, y={:d}..{:d}', text)

def check_vel(vx, vy):
    x, y = 0, 0
    while x <= x2 and y >= y1:
        x, y = x + vx, y + vy
        if vx > 0: vx -= 1
        if vx < 0: vx += 1
        vy -= 1
        if x1 <= x <= x2 and y1 <= y <= y2:
            return True
    return False


max_vy = abs(y1)-1
min_vy = -max_vy-1

min_vx = int(sqrt(x1*2))
max_vx = x2+1

count = sum(
    check_vel(vx, vy) 
    for vx in range(min_vx, max_vx+1) 
    for vy in range(min_vy, max_vy+1)
)
print("Part 1:", max_vy*(max_vy+1)//2)
print("Part 2:", count)
