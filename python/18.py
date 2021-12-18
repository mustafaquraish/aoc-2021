from common import *

with open(infile('18.txt'), 'r') as f:
    text = f.read().strip()
    lines = text.split('\n')

data = get_all_ints(text)

class Node:
    @classmethod
    def from_list(cls, arr):
        if isinstance(arr, int):
            return cls(arr)
        return Node(Node.from_list(arr[0]), Node.from_list(arr[1]))

    def __init__(self, left, right=None):
        if right is None:
            self.leaf = True
            self.value = left
        else:
            self.leaf = False
            self.l = left
            self.r = right
    
    def __repr__(self):
        return f'{self.value}' if self.leaf else f'[{self.l},{self.r}]'

    def should_explode(self, depth):
        return depth >= 4 and self.l.leaf and self.r.leaf

    def do_split(self, depth=0):
        if not self.leaf:
            if self.l.do_split(depth+1): return True
            if self.r.do_split(depth+1): return True
            return False
        if self.value < 10: return False

        a, b = self.value//2, self.value-self.value//2
        self.leaf = False
        self.l = Node(a)
        self.r = Node(b)
        return True

    def do_explode(self, pair, depth=0):
        if self.leaf: 
            return False

        if self.should_explode(depth):
            pair[:] = [self.l.value, self.r.value]
            return True

        if result := self.l.do_explode(pair, depth+1):
            if pair[1] is None: 
                return True
            if pair[0] is not None: 
                self.l = Node(0)
            cur = self.r
            while not cur.leaf:
                cur = cur.l
            cur.value += pair[1]
            pair[1] = None
            return True
        
        if result := self.r.do_explode(pair, depth+1):
            if pair[0] is None: 
                return True
            if pair[1] is not None: 
                self.r = Node(0)
            cur = self.l
            while not cur.leaf:
                cur = cur.r
            cur.value += pair[0]
            pair[0] = None
            return True

        return False

    def simplify(self):
        while True:
            if self.do_explode([None, None]): continue
            if self.do_split(): continue
            break

    def __add__(self, other):
        new_root = Node(self, other)
        return new_root

    def mag(self):
        return self.value if self.leaf else self.l.mag()*3 + self.r.mag()*2

def part1():
    bst = None
    for line in lines:
        lst = eval(line)
        if bst is None:
            bst = Node.from_list(lst)
        else:
            bst = bst + Node.from_list(lst)
        bst.simplify()
    res = bst.mag()
    return res

def part2():
    mx = 0
    for a in lines:
        for b in lines:
            if a == b: continue
            x, y = eval(a), eval(b)
            for arr in ([x,y], [y,x]):
                bst = Node.from_list(arr)
                bst.simplify()
                mx = max(mx, bst.mag())
    return mx
    

print("Part 1:", part1())
print("Part 2:", part2())