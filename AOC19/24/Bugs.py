from collections import defaultdict

DIRS = [(1, 0), (-1, 0), (0, 1), (0, -1)]
SQUARES = [(i, j) for i in range(5) for j in range(5) if not (i == j == 2)]
INNER_SQUARES = ((1, 2), (3, 2), (2, 1), (2, 3))
NEIGHBOURS = defaultdict(list)
for i, j in SQUARES:
    for di, dj in DIRS:
        ni, nj = i + di, j + dj
        if ni == nj == 2:
            bi, bj = i - di, j - dj
            NEIGHBOURS[(i, j)].append((bi, bj, -1))
            NEIGHBOURS[(i, j)].append((bi - dj, bj + di, -1))
            NEIGHBOURS[(i, j)].append((bi + dj, bj - di, -1))
            NEIGHBOURS[(i, j)].append((bi - 2 * dj, bj + 2 * di, -1))
            NEIGHBOURS[(i, j)].append((bi + 2 * dj, bj - 2 * di, -1))
        elif not (0 <= ni <= 4 and 0 <= nj <= 4):
            NEIGHBOURS[(i, j)].append((2 + di, 2 + dj, 1))
        else:
            NEIGHBOURS[(i, j)].append((ni, nj, 0))

def update_layout(layout):
    new_layout = []
    for i in range(5):
        new_layout.append([])
        for j in range(5):
            cv = layout[i][j]
            adj = 0
            for di, dj in DIRS:
                ni, nj = i + di, j + dj
                if not (0 <= ni <= 4 and 0 <= nj <= 4):
                    continue
                if layout[ni][nj] == '#':
                    adj += 1
            if cv == '#' and adj != 1:
                new_layout[i].append('.')
            elif cv == '.' and 1 <= adj <= 2:
                new_layout[i].append('#')
            else:
                new_layout[i].append(cv)
        new_layout[i] = tuple(new_layout[i])
    new_layout = tuple(new_layout)
    return new_layout

def run_sim(layout):
    seen = set([layout])
    while True:
        layout = update_layout(layout)
        if layout in seen:
            return layout
        seen.add(layout)

def calculate_bio(layout):
    out = 0
    for i in range(5):
        for j in range(5):
            k = j + 5 * i
            if layout[i][j] == '#':
                out += pow(2, k)
    return out

def new_layers(tiles):
    layers = [l for i, j, l in tiles.keys()]
    outer_layer = max(layers)
    inner_layer = min(layers)
    add_outer = False
    add_inner = False
    for (i, j, l), val in tiles.items():
        if l == outer_layer and (i == 0 or j == 0 or i == 4 or j == 4):
            if val == '#':
                add_outer = True
        elif l == inner_layer and (i, j) in INNER_SQUARES:
            if val == '#':
                add_inner = True
    if add_outer:
        for ni, nj in SQUARES:
            tiles[(ni, nj, outer_layer + 1)] = '.'
    if add_inner:
        for ni, nj in SQUARES:
            tiles[(ni, nj, inner_layer - 1)] = '.'

def update_map(tiles):
    new_layers(tiles)
    deaths = []
    births = []
    for (i, j, l), cv in tiles.items():
        adj = 0
        for ni, nj, inc in NEIGHBOURS[(i, j)]:
            if tiles.get((ni, nj, l + inc), '.') == '#':
                adj += 1
        if cv == '#' and adj != 1:
            deaths.append((i, j, l))
        elif cv == '.' and 1 <= adj <= 2:
            births.append((i, j, l))

    for b in births:
        tiles[b] = '#'
    for d in deaths:
        tiles[d] = '.'

def count_bugs_after(layout, minutes):
    tiles = dict()
    for i, j in SQUARES:
        tiles[(i, j, 0)] = layout[i][j]

    for _ in range(minutes):
        update_map(tiles)

    bugs = 0
    for v in tiles.values():
        if v == '#':
            bugs += 1
    return bugs

layout = []
with open("input.txt") as f:
    for line in f:
        layout.append(tuple(line.strip()))
layout = tuple(layout)

# Part 1
tile = run_sim(layout)
print("Part 1 biodiversity is", calculate_bio(tile))

# Part 2
bugs = count_bugs_after(layout, 200)
print("Part 2 bugs after 200 mins:", bugs)

