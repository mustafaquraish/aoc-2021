from copy import deepcopy
from rich import print
from parse import findall, parse
from more_itertools import chunked, ilen
from collections import Counter, defaultdict
from heapq import heappush, heappop
from tqdm import tqdm
import re
import statistics as stat
import sys

def infile(filename):
    if len(sys.argv) < 2:
        return 'input/'+filename
    return sys.argv[1]

def get_lines(file):
    return [line.strip() for line in file.readlines()]

def get_all_ints(text):
    return [int(x) for x in re.findall(r'-?\d+', text)]

def grid_to_dict(grid):
    return {(i, j): v for j, row in enumerate(grid) for i, v in enumerate(row)}

def neighbours(i, j, diag=False):
    yield (i-1, j)
    yield (i+1, j)
    yield (i, j-1)
    yield (i, j+1)
    if diag:
        yield (i-1, j-1)
        yield (i-1, j+1)
        yield (i+1, j-1)
        yield (i+1, j+1)

def drange(start, stop):
    if start > stop:
        return range(stop, start-1, -1)
    else:
        return range(start, stop+1)

def span_list(lst):
    return (min(lst), max(lst))

def range_span(lst):
    mn, mx = span_list(lst)
    return range(mn, mx+1)
