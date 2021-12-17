from common import *
from textwrap import indent

with open(infile('16.txt'), 'r') as f:
    data = "".join(f"{int(x,16):04b}" for x in f.read().strip())

OFF = 0 # offset within the data

def get_int(num_digits):
    global OFF
    res = int(data[OFF:OFF+num_digits],2)
    OFF += num_digits
    return res

def parse():
    packet = Packet()
    packet.version = get_int(3)
    packet.typeid = get_int(3)
    packet.value = 0
    packet.subs = []
    
    if packet.typeid == 4:
        loop = True
        while loop:
            loop = bool(get_int(1))
            packet.value = packet.value << 4 | get_int(4)
        return packet
    
    if get_int(1) == 0:
        size = get_int(15)
        while size:
            old = OFF
            packet.subs.append(parse())
            size -= OFF - old
    else:
        packet.subs = [parse() for _ in range(get_int(11))]

    return packet

class Packet:
    def __str__(self):
        if self.typeid == 4:
            s = f'Packet({self.version}, {self.typeid}, {self.value})'
        else:
            s = f'Packet({self.version}, {self.typeid},\n'
            s += "\n".join(indent(str(p), '  ') for p in self.subs)
            s += '\n)'
        return s
    
    def eval(self):
        match (self.typeid):
            case 4: return self.value
            case 0: return sum(p.eval() for p in self.subs)
            case 1: return prod(p.eval() for p in self.subs)
            case 2: return min(p.eval() for p in self.subs)
            case 3: return max(p.eval() for p in self.subs)
            case 5: return int(self.subs[0].eval() > self.subs[1].eval())
            case 6: return int(self.subs[0].eval() < self.subs[1].eval())
            case 7: return int(self.subs[0].eval() == self.subs[1].eval())

    def version_sum(self):
        return self.version + sum(p.version_sum() for p in self.subs)

P = parse()
# print(P)
print("Part 1:", P.version_sum())
print("Part 2:", P.eval())