from common import *

with open(infile('04.txt')) as file:
    numbers = get_all_ints(file.readline())
    orig_boards = [
        [[(int(i), False) for i in row.split()] for row in board.split('\n')]
        for board in file.read().strip().split('\n\n')
    ]

def did_win(board):
    for row in board:
        if all(won for _, won in row):
            return True
    for col in zip(*board):
        if all(won for _, won in col):
            return True
    return False

def select_number(boards, number):
    for board in boards:
        for i, row in enumerate(board):
            for j, (num, _) in enumerate(row):
                if num == number:
                    board[i][j] = (num, True)
        
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
        if all(won) and not all(done):
            return winning_score(boards[done.index(False)], num)
        done = won

print("Part 1:", part1())
print("Part 2:", part2())