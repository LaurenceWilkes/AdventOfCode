with open("input.txt") as f:
    lineA = [(x[0], int(x[1:])) for x in f.readline().split(",")]
    lineB = [(x[0], int(x[1:])) for x in f.readline().split(",")]

def convert(line):
    loc = (0, 0)
    prevloc = (0, 0)
    directions = {
        "U": (0, 1),
        "R": (1, 0),
        "D": (0, -1),
        "L": (-1, 0)
    }
    segments = []
    for ins in line:
        loc = (loc[0] + ins[1] * directions[ins[0]][0],
               loc[1] + ins[1] * directions[ins[0]][1])
        segments.append(prevloc + loc)
        prevloc = loc
    return segments

def check_intersections(a, b):
    if a[0] == a[2] and b[1] == b[3]:  # a vertical, b horizontal
        if (min(b[0], b[2]) <= a[0] <= max(b[0], b[2]) and
            min(a[1], a[3]) <= b[1] <= max(a[1], a[3])):
            return (a[0], b[1])
    elif a[1] == a[3] and b[0] == b[2]:  # a horizontal, b vertical
        if (min(a[0], a[2]) <= b[0] <= max(a[0], a[2]) and
            min(b[1], b[3]) <= a[1] <= max(b[1], b[3])):
            return (b[0], a[1])
    return None ## are there parallel sections...?

def manhattan(c):
    return abs(c[0]) + abs(c[1])

def dist_to_reach(pt, segments):
    total_dist = 0
    for s in segments:
        if s[0] == s[2] == pt[0]:
            if (min(s[1], s[3]) <= pt[1] <= max(s[1], s[3])):
                total_dist += abs(pt[1] - s[1])
                break
        if s[1] == s[3] == pt[1]:
            if (min(s[0], s[2]) <= pt[0] <= max(s[0], s[2])):
                total_dist += abs(pt[0] - s[0])
                break
        total_dist += manhattan((s[2] - s[0], s[3] - s[1]))
    return total_dist

segmentsA = convert(lineA)
segmentsB = convert(lineB)

# Part 1
first_flag = True
mindist = -1
for a in segmentsA:
    for b in segmentsB:
        c = check_intersections(a, b)
        if c is not None and c != (0, 0):
            mc = manhattan(c)
            if mc < mindist or first_flag:
                mindist = mc
                first_flag = False

print("Part 1 minimum distance is", mindist)

# Part 2
first_flag = True
mindist = -1
for a in segmentsA:
    for b in segmentsB:
        c = check_intersections(a, b)
        if c is not None and c[0] != 0 and c[1] != 0:
            distA = dist_to_reach(c, segmentsA)
            distB = dist_to_reach(c, segmentsB)
            if distA + distB < mindist or first_flag:
                mindist = distA + distB
                first_flag = False

print("Part 2 minimum distance is", mindist)

