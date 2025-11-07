from math import gcd, atan2, pi

with open("input.txt") as f:
    lines = [line.strip() for line in f if line.strip()]

height = len(lines)
width = len(lines[0])

asteroids = {
    (x, y)
    for y, row in enumerate(lines)
    for x, cell in enumerate(row)
    if cell == '#'
}

def visible(a1, a2, asteroids):
    a = a2[0] - a1[0]
    b = a2[1] - a1[1]
    c = gcd(a, b)
    a = a // c
    b = b // c
    for i in range(1, c):
        ast = (a1[0] + a * i, a1[1] + b * i)
        if ast in asteroids:
            return False
    return True

def direction(station, ast):
    a = ast[0] - station[0]
    b = ast[1] - station[1]
    c = gcd(a, b)
    a //= c
    b //= c
    return (a, b)

def angle(x):
    ang = atan2(x[0], -x[1])
    if ang < 0:
        ang += 2 * pi
    return ang

# Part 1 (lazy)
max_count = 0
station = None
for ast1 in asteroids:
    count = 0
    for ast2 in asteroids:
        if ast2 != ast1 and visible(ast1, ast2, asteroids):
            count += 1
    if count > max_count:
        max_count = count
        station = ast1

print("Part 1:", station, "can see", max_count, "asteroids")

# Part 2 (also sort of lazy)
asteroids.remove(station)

dir_map = {}
for ast in asteroids:
    d = direction(station, ast)
    dir_map.setdefault(d, []).append(ast)

for d in dir_map:
    dir_map[d].sort(key = lambda ast : abs(direction(station, ast)[0]), \
                    reverse = True)

directions = sorted(dir_map.keys(), key = lambda x : angle(x))

i = 0
breakflag = True
while breakflag:
    for d in directions:
        i += 1
        cur = dir_map[d].pop()
        if i == 200:
            print("Part 2: 200th asteroid is", cur, \
                  "with the output", cur[0]*100 + cur[1])
            breakflag = False
            break
