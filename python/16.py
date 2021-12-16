from common import *
from textwrap import indent

with open(infile('16.txt'), 'r') as f:
    data = "".join(f"{int(x,16):04b}" for x in f.read().strip())

class Packet:
    @staticmethod
    def parse_from_text(text):
        rest = text
        version, rest = int(rest[:3], 2), rest[3:]
        typeid, rest = int(rest[:3], 2), rest[3:]
        packet = Packet(version, typeid)
        if typeid == 4:
            while True:
                part, rest = rest[:5], rest[5:]
                chunk = int("".join(part[1:]), 2)
                packet.value = packet.value << 4 | chunk
                if part[0] == '0':
                    return packet, rest
    
        mode, rest = rest[0], rest[1:]
        if mode == '0':
            length, rest = int(rest[:15], 2), rest[15:]
            while length > 0:
                subp, remaining = Packet.parse_from_text(rest)
                packet.subs.append(subp)
                length -= len(rest) - len(remaining)
                rest = remaining
        else:   
            length, rest = int(rest[:11], 2), rest[11:]
            for i in range(length):
                subp, rest = Packet.parse_from_text(rest)
                packet.subs.append(subp)

        return packet, rest

    def __init__(self, version, typeid):
        self.version = version
        self.typeid = typeid
        self.value = 0 
        self.subs = []

    def __str__(self):
        if self.typeid == 4:
            s = f'Packet({self.version}, {self.typeid}, {self.value})'
        else:
            s = f'Packet({self.version}, {self.typeid},\n'
            for p in self.subs:
                s += indent(str(p), '  ') + '\n'
            s += ')'
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

P, _ = Packet.parse_from_text(data)
print("Part 1:", P.version_sum())
print("Part 2:", P.eval())