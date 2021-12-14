from common import *

with open(infile('04.txt')) as file:
    numbers = [int(i) for i in file.readline().strip().split(',')]
    orig_boards = [
        [[(int(i), False) for i in row.split()] for row in board.split('\n')]
        for board in file.read().strip().split('\n\n')
    ]

def did_win(board):
    return any(all(done for _, done in row) for row in board + list(zip(*board)))

def select_number(boards, number):
    for i, board in enumerate(boards):
        boards[i] = [[(n, True if n == number else done) for n, done in row] for row in board]
        
def winning_score(board, number):
    return sum(sum(n for n, done in row if not done) for row in board) * number

def part1():
    boards = deepcopy(orig_boards)
    for num in numbers:
        select_number(boards, num)
        won = [did_win(board) for board in boards]
        if any(won):
            return winning_score(boards[won.index(True)], num)

def part2():
    boards = deepcopy(orig_boards)
    done = [False for _ in boards]
    for num in numbers:
        select_number(boards, num)
        won = [did_win(board) for board in boards]
        if all(won) and any(not x for x in done):
            return winning_score(boards[done.index(False)], num)
        done = won

print("Part 1:", part1())
print("Part 2:", part2())