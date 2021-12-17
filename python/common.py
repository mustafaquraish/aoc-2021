from copy import deepcopy
from math import sqrt
from collections import Counter, defaultdict
from heapq import heappush, heappop
from time import perf_counter
from contextlib import contextmanager
import re
import statistics as stat
import sys
from queue import PriorityQueue

# These are not available on pypy, so we ignore them.
try:
    # External, can be installed
    from parse import findall, parse
    from tqdm import tqdm
    from more_itertools import chunked, ilen
    from rich import print
    # Just no available AFAIK
    from math import prod
    from functools import cache, lru_cache
except ImportError:
    print('Warning, not all libraries available, some functionality may be limited')


def infile(filename):
    if len(sys.argv) < 2:
        return 'input/'+filename
    return sys.argv[1]

def parse_block_grid(text, apply_fn=int):
    grid = [[apply_fn(x) for x in row.strip()] for row in text.splitlines()]
    return grid, len(grid[0]), len(grid)

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

@contextmanager
def timer(s) -> float:
    start = perf_counter()
    yield
    end = perf_counter()
    print(f"{s}: {end - start:.5f}s")