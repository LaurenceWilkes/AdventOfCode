from collections import defaultdict, deque

def read_grid(filename):
    grid = {}
    with open(filename) as f:
        lines = [line.rstrip('\n') for line in f]

    maxx, minx = 0, len(lines[0])
    maxy, miny = 0, len(lines)
    for y, row in enumerate(lines):
        for x, ch in enumerate(row):
            grid[(x, y)] = ch
            if ch == ".":
                minx = min(minx, x)
                maxx = max(maxx, x)
                miny = min(miny, y)
                maxy = max(maxy, y)
    return grid, (minx, maxx, miny, maxy)

def find_portals(grid, bounds):
    (minx, maxx, miny, maxy) = bounds
    portals = defaultdict(list)
    seen = set()
    for (x, y), ch in grid.items():
        if (x, y) in seen:
            continue

        if not ch.isupper():
            continue
        for dx, dy in [(1,0), (-1,0), (0,1), (0,-1)]:
            x2, y2 = x + dx, y + dy
            if not grid.get((x2, y2), " ").isupper():
                continue
            seen.add((x2, y2))

            if dx < 0 or dy < 0:
                label = grid[(x2, y2)] + ch
            else:
                label = ch + grid[(x2, y2)]

            px, py = x - dx, y - dy
            if grid.get((px, py), " ") == ".":
                portals[label].append((px, py))
            else:
                px2, py2 = x2 + dx, y2 + dy
                if grid.get((px2, py2), " ") == ".":
                    portals[label].append((px2, py2))
    goesin = {}
    goesout = {}
    start = None
    end = None
    for l in portals.keys():
        if l == "AA":
            start = portals[l][0]
            continue
        elif l == "ZZ":
            end = portals[l][0]
            continue
        a = portals[l][0]
        b = portals[l][1]
        if a[0] == minx or a[0] == maxx or a[1] == miny or a[1] == maxy:
            goesout[a] = b
            goesin[b] = a
        else:
            goesin[a] = b
            goesout[b] = a
    return goesin, goesout, start, end

def get_neighbours(grid, goesin, goesout):
    neigh = defaultdict(list)
    for pt, ch in grid.items():
        if ch != '.':
            continue
        for dx, dy in [(0, 1), (0, -1), (1, 0), (-1, 0)]:
            np = (pt[0] + dx, pt[1] + dy)
            if grid[np] == '.':
                neigh[pt].append(np)
            if grid[np].isupper() and (pt in goesin or pt in goesout):
                op = None
                if pt in goesin:
                    op = goesin[pt]
                else:
                    op = goesout[pt]
                neigh[pt].append(op)
    return neigh

def get_neighbours_rec(grid, goesin, goesout):
    neigh = defaultdict(list)
    for pt, ch in grid.items():
        if ch != '.':
            continue
        for dx, dy in [(0, 1), (0, -1), (1, 0), (-1, 0)]:
            np = (pt[0] + dx, pt[1] + dy)
            if grid[np] == '.':
                neigh[pt].append((np, 0))
            if grid[np].isupper() and (pt in goesin or pt in goesout):
                op = None
                if pt in goesin:
                    op = goesin[pt]
                    neigh[pt].append((op, 1))
                else:
                    op = goesout[pt]
                    neigh[pt].append((op, -1))
    return neigh

def flood(neigh, start, end):
    q = deque([(0, start)])
    seen = set([start])
    while q:
        d, pt = q.popleft()
        if pt == end:
            return d
        for np in neigh[pt]:
            if np not in seen:
                q.append((d + 1, np))
                seen.add(np)

def flood_rec(neigh, start, end):
    q = deque([(0, start, 0)]) #dist, pt, floor
    seen = set([(start, 0)]) #pt, floor
    while q:
        d, pt, floor = q.popleft()
        if pt == end and floor == 0:
            return d
        for np, mv in neigh[pt]:
            nfloor = floor + mv
            if nfloor >= 0 and (np, nfloor) not in seen:
                q.append((d + 1, np, nfloor))
                seen.add((np, nfloor))


grid, bounds = read_grid("input.txt")
goesin, goesout, start, end = find_portals(grid, bounds)

# Part 1
neigh = get_neighbours(grid, goesin, goesout)
d = flood(neigh, start, end)
print("Part 1 min dist is", d)

# Part 2
neigh_rec = get_neighbours_rec(grid, goesin, goesout)
d_rec = flood_rec(neigh_rec, start, end)
print("Part 2 min dist is", d_rec)

