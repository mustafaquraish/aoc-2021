from common import *

class Cube:
    def __init__(self, x1, x2, y1, y2, z1, z2):
        self.x1, self.x2 = min(x1, x2), max(x1, x2)
        self.y1, self.y2 = min(y1, y2), max(y1, y2)
        self.z1, self.z2 = min(z1, z2), max(z1, z2)
    
    def overlaps(self, other):
        return not (self.x2 < other.x1 or self.x1 > other.x2 or
                    self.y2 < other.y1 or self.y1 > other.y2 or
                    self.z2 < other.z1 or self.z1 > other.z2)
    
    def area(self):
        dx = self.x2 - self.x1 + 1
        dy = self.y2 - self.y1 + 1
        dz = self.z2 - self.z1 + 1
        return dx * dy * dz

    def __repr__(self):
        return f'({self.x1},{self.y1},{self.z1}) -> ({self.x2},{self.y2},{self.z2})'

    def subtract(self, other):
        if not self.overlaps(other):
            return [self]
        max_x1 = max(self.x1, other.x1)
        min_x2 = min(self.x2, other.x2)
        max_y1 = max(self.y1, other.y1)
        min_y2 = min(self.y2, other.y2)

        res = []
        if other.x1 > self.x1: res.append(Cube(self.x1, other.x1 - 1, self.y1, self.y2, self.z1, self.z2))
        if other.y1 > self.y1: res.append(Cube(max_x1, min_x2, self.y1, other.y1 - 1, self.z1, self.z2))
        if other.z1 > self.z1: res.append(Cube(max_x1, min_x2, max_y1, min_y2, self.z1, other.z1 - 1))

        if other.x2 < self.x2: res.append(Cube(other.x2 + 1, self.x2, self.y1, self.y2, self.z1, self.z2))
        if other.y2 < self.y2: res.append(Cube(max_x1, min_x2, other.y2 + 1, self.y2, self.z1, self.z2))
        if other.z2 < self.z2: res.append(Cube(max_x1, min_x2, max_y1, min_y2, other.z2 + 1, self.z2))
        return res

    def add(self, other):
        extra_regions = other.subtract(self)
        return [self] + extra_regions

points = []
with open(infile('.txt'), 'r') as f:
    for line in f:
        points.append((line[1] == 'n', Cube(*get_all_ints(line))))

PART1_BOUNDS = Cube(-50, 50, -50, 50, -50, 50)

def solve(inbounds):
    all_cubes = []
    for OP, cur_cube in points:
        if inbounds and not PART1_BOUNDS.overlaps(cur_cube):
            continue
        new_cubes = []
        for cube in all_cubes:
            new_cubes.extend(cube.subtract(cur_cube))
        if OP == True:
            new_cubes.append(cur_cube)
        all_cubes = new_cubes
    return sum(c.area() for c in all_cubes)

print("Part 1:", solve(True))
print("Part 2:", solve(False))