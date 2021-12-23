from common import *

with open(infile('23s.txt')) as f:
    lines = f.read().split('\n')

COLS = {'A': 2, 'B': 4, 'C': 6, 'D': 8}
COST = {'A': 1, 'B': 10, 'C': 100, 'D': 1000}

def distance(a, b):
    return abs(a[1] - b[1]) + abs(a[0] - b[0])

def in_room(pod):
    _, (row, _) = pod
    return row > 0

def in_correct_room(pod):
    char, (row, col) = pod
    return row > 0 and COLS[char] == col

pods = []
for i, line in enumerate(lines[1:-1]):
    for j, char in enumerate(line[1:-1]):
        if char in "ABCD":
            pods.append((char, (i, j)))

class Grid:
    def __init__(self, pods, N):
        self.pods = pods
        self.N = N

    def __hash__(self):
        return hash((tuple(self.pods), self.N))

    def __eq__(self, other):
        return self.pods == other.pods

    def __iter__(self):
        yield from self.pods

    def __repr__(pods, N=2):
        grid = [list("╭―――――――――――╮"),
                list("│...........│"),
                list("╰―╮.│.│.│.╭―╯"),
                list("  │.│.│.│.│"),
                list("  │.│.│.│.│"),
                list("  │.│.│.│.│"),
                list("  ╰―┴―┴―┴―╯")]

        for char, (row, col) in pods:
            grid[row+1][col+1] = char

        grid = grid[:N+2]+grid[-1:]
        return "\n".join("".join(row) for row in grid)
    
    def available_spots_for(self, start):
        positions = {pos: c for c, pos in self.pods}
        visible = set()
        can_move = []

        def check_hall(col, offset):
            i = col
            while 0 <= i < 11 and (0, i) not in positions:
                if i not in (2, 4, 6, 8):
                    can_move.append( (0, i) )
                visible.add( (0, i) )
                i += offset

        char, (row, col) = start
        if in_room(start):
            if in_correct_room(start):
                if row == self.N:  return []

                others = [c for c,(ro,co) in self.pods if co == col and ro > row]
                assert len(others) > 0

                # Everything below me is the same, I'm done.
                if all(c == char for c in others):
                    return []

            # Trapped if there's something above me
            others = [c for c,(ro,co) in self.pods if co == col and 0 < ro < row]
            if len(others) > 0: return []

            check_hall(col+1, 1)
            check_hall(col-1,-1)
            return can_move
        else:
            check_hall(col+1, 1)
            check_hall(col-1,-1)
            dest_col = COLS[char]

            # Can't access the column to the room, no moves.
            if (0, dest_col) not in visible: return []

            # Don't go back in if there's other chars
            others = [c for c,(ro,co) in self.pods if co == dest_col and ro > row]
            if not all(c == char for c in others):
                return []

            # Elements in room are same, add myself
            return [(self.N - len(others), dest_col)]

@lru_cache(maxsize=None)
def solve(grid):
    if all(in_correct_room(pod) for pod in grid.pods):
        return 0

    lowest = float('inf')
    for i, pod in enumerate(grid.pods):
        char, (row, col) = pod
        for (nrow, ncol) in grid.available_spots_for(pod):
            step_cost = (abs(nrow-row) + abs(ncol-col)) * COST[char]

            grid.pods[i] = (char, (nrow, ncol))
            rec_cost = solve(grid)
            grid.pods[i] = (char, (row, col))

            if rec_cost + step_cost < lowest:
                lowest = rec_cost + step_cost
    return lowest

def part1():
    cur_pods = deepcopy(pods)
    return solve(Grid(pods, 2))

def part2():
    cur_pods = deepcopy(pods)
    for i, pod in enumerate(cur_pods):
        char, (row, col) = pod
        if row == 2:
            cur_pods[i] = (char, (4, col))
    cur_pods += [
        ('D',(2,2)), ('C',(2,4)), ('B',(2,6)), ('A',(2,8)),
        ('D',(3,2)), ('B',(3,4)), ('A',(3,6)), ('C',(3,8)),
    ]
    return solve(Grid(cur_pods, 4))


print("Part 1:", part1())
print("Part 2:", part2())
