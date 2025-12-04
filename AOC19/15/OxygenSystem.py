from intcode import run_program
from collections import deque

DIRS = {1: (0, -1), 2: (0, 1), 3: (-1, 0), 4: (1, 0)}
REV  = {1: 2, 2: 1, 3: 4, 4: 3}

def move(p, d):
    x, y = DIRS[d]
    return (p[0] + x, p[1] + y)

def explore(nums):
    grid = {(0, 0): 1}
    prog = nums.copy()
    loc = 0
    relbase = 0
    oxy = None
    def search(pos):
        nonlocal loc, relbase, oxy
        for d in (1, 2, 3, 4):
            np = move(pos, d)
            if np in grid:
                continue
            out, loc, relbase = run_program(prog, loc, relbase, [d])
            if out == 0:
                grid[np] = 0
                continue
            grid[np] = 2 if out == 2 else 1
            if out == 2:
                oxy = np
            search(np)
            out, loc, relbase = run_program(prog, loc, relbase, [REV[d]])
    search((0, 0))
    return grid, oxy

def bfs(grid, start):
    q = deque([(start, 0)])
    seen = {start}
    maxdist = 0
    oxydist = 0
    while q:
        pos, dist = q.popleft()
        maxdist = max(maxdist, dist)
        for d in (1, 2, 3, 4):
            dx, dy = DIRS[d]
            np = (pos[0] + dx, pos[1] + dy)
            if grid.get(np, 0) != 0 and np not in seen:
                if np == oxy:
                    oxydist = dist + 1
                seen.add(np)
                q.append((np, dist + 1))
    return maxdist, oxydist

with open("input.txt") as f:
    nums=[int(x) for x in f.read().strip().split(",")]

grid, oxy = explore(nums)
maxdist, oxydist = bfs(grid, (0, 0))
print("Part 1 oxygen distance is", oxydist)

maxdist, oxydist = bfs(grid, oxy)
print("Part 2 time to full is", maxdist)
