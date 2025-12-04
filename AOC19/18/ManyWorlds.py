from collections import deque
import heapq

DIRS = [(1, 0), (-1, 0), (0, 1), (0, -1)]

def get_grid(filename):
    grid = {}
    keys = set()
    start = None
    with open(filename) as f:
        for y, line in enumerate(f):
            for x, ch in enumerate(line.rstrip()):
                grid[(x, y)] = ch
                if ch == '@':
                    start = (x, y)
                if ch.islower():
                    keys.add(ch)
    return grid, start, keys

def dist_from(grid, start):
    q = deque([(start, 0, frozenset())])
    seen = set([start])
    found = {}
    while q:
        (x, y), dist, req = q.popleft()
        cell = grid[(x, y)]
        if cell.islower() and dist > 0:
            found[cell] = (dist, req)
        if cell.isupper():
            req = req | {cell.lower()}
        for dx, dy in DIRS:
            np = (x+dx, y+dy)
            if np not in grid or np in seen:
                continue
            if grid[np] != '#':
                seen.add(np)
                q.append((np, dist+1, req))
    return found

def build_graph(grid, keys):
    adjacent = {}
    for pos, k in grid.items():
        if k.islower() or k == '@':
            adjacent[k] = dist_from(grid, pos)
    return adjacent

def shortest_path(adjacent, keys):
    all_keys = frozenset(keys)
    pq = [(0, '@', frozenset())]
    seen = set()

    while pq:
        dist, cur_k, keys_collected = heapq.heappop(pq)
        state = (cur_k, keys_collected)
        if state in seen:
            continue
        seen.add(state)

        if keys_collected == all_keys:
            return dist

        for nk, (d, req) in adjacent[cur_k].items():
            if nk in keys_collected:
                continue
            if not req <= keys_collected:
                continue
            heapq.heappush(pq, (dist + d, nk, keys_collected | {nk}))

    return None

def build_graph_multi(grid, keys):
    graph = {}
    robots = tuple(str(i) for i in range(4))
    for pos, k in grid.items():
        if k.islower() or k in robots:
            graph[k] = dist_from(grid, pos)
    return graph

def shortest_path_multi(graph, starts, keys):
    all_keys = frozenset(keys)
    robots = tuple(str(i) for i in range(4))
    pq = [(0, robots, frozenset())]
    heapq.heapify(pq)
    seen = set()

    while pq:
        dist, robots, keys_collected = heapq.heappop(pq)
        state = (robots, keys_collected)
        if state in seen:
            continue
        seen.add(state)

        if keys_collected == all_keys:
            return dist

        for r in range(4):
            here = robots[r]
            for nk, (d, req) in graph[here].items():
                if nk in keys_collected:
                    continue
                if not req <= keys_collected:
                    continue
                new_robots = list(robots)
                new_robots[r] = nk
                new_robots = tuple(new_robots)
                heapq.heappush(pq,
                    (dist + d, new_robots, keys_collected | {nk})
                )
    return None

grid, start, keys = get_grid("input.txt")

# Part 1
adjacent = build_graph(grid, keys)
print("Part 1 shortest path gives", shortest_path(adjacent, keys))

# Part 2
grid[(start[0] - 1, start[1])] = '#'
grid[(start[0] + 1, start[1])] = '#'
grid[(start[0], start[1] + 1)] = '#'
grid[(start[0], start[1] - 1)] = '#'
grid[start] = '#'

starts = [
    (start[0] - 1, start[1] + 1), \
    (start[0] + 1, start[1] + 1), \
    (start[0] - 1, start[1] - 1), \
    (start[0] + 1, start[1] - 1)  \
]
for i, s in enumerate(starts):
    grid[s] = str(i)

graph2 = build_graph_multi(grid, keys)
print("Part 2 shortest path gives", shortest_path_multi(graph2, starts, keys))


