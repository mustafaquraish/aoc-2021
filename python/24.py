with open('input/24.txt') as f:
    blocks = [x.strip().split('\n') for x in f.read().strip().split('inp')[1:]]

digits = []
for block in blocks:
    up = block[4].endswith('1')
    sub = int(block[5].split(' ')[-1])
    val = int(block[-3].split()[-1])
    digits.append((up, sub, val))

def solve(vars, inc, res=0):
    if len(vars) == 0 and res == 0:
        return ''
    up, sub, val = vars[0]
    if up:
        for i in list(range(1,10))[::inc]:
            if (rest := solve(vars[1:], inc, val + i + 26*res)) is not None:
                return str(i) + rest
    else:
        if not 1 <= (digit := (res % 26) + sub) <= 9:
            return None
        if (rest := solve(vars[1:], inc, res // 26)) is not None:
            return str(digit) + rest
    return None 

print("Part 1:", solve(digits,-1))
print("Part 2:", solve(digits, 1))