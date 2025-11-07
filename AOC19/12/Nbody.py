import re
from math import lcm

def energy(coords, vels):
    total_sum = 0
    for i in range(4):
        pot = abs(coords[0][i]) + abs(coords[1][i]) + abs(coords[2][i])
        kin = abs(vels[0][i]) + abs(vels[1][i]) + abs(vels[2][i])
        total_sum += pot * kin
    return total_sum

def update_vel(coord, vel):
    for i in range(3):
        for j in range(i, 4):
            a = coord[i]
            b = coord[j]
            if a > b:
                vel[i] -= 1
                vel[j] += 1
            elif a < b:
                vel[i] += 1
                vel[j] -= 1

def apply_vel(coord, vel):
    for i in range(4):
        coord[i] += vel[i]

def simulate(coords, steps):
    vels = [[0, 0, 0, 0] for _ in range(3)]
    for s in range(steps):
        for i in range(3):
            update_vel(coords[i], vels[i])
        for i in range(3):
            apply_vel(coords[i], vels[i])
    return energy(coords, vels)

def find_pattern(coord):
    vel = [0, 0, 0, 0]
    posvel_set = set()
    posvel_list = []
    tcv = (tuple(coord), tuple(vel))
    while tcv not in posvel_set:
        posvel_set.add(tcv)
        posvel_list.append(tcv)
        update_vel(coord, vel)
        apply_vel(coord, vel)
        tcv = (tuple(coord), tuple(vel))
    k = 0
    while True:
        if tcv == posvel_list[k]:
            return (len(posvel_list), k)
        k += 1

# loop length = len(posvel_list) - k
# index of first loop element = k

def combine_patterns(loops, first_elements):
    L = lcm(loops[0], loops[1], loops[2])
    B = max(first_elements[0], first_elements[1], first_elements[2])
    return B + L

def find_repeat(coords):
    loops = []
    first_elements = []
    for coord in coords:
        fp = find_pattern(coord)
        loops.append(fp[0] - fp[1])
        first_elements.append(fp[1])
    return combine_patterns(loops, first_elements)

coords = [[], [], []]
with open("input.txt") as f:
    for line in f:
        nums = list(map(int, re.findall(r"-?\d+", line)))
        coords[0].append(nums[0])
        coords[1].append(nums[1])
        coords[2].append(nums[2])

# Part 1
cs = [c[:] for c in coords]
e = simulate(cs, 1000)
print("Part 1 energy after 1000 steps is", e)

# Part 2
cs = [c[:] for c in coords]
r = find_repeat(cs)
print("Part 2 repeat is at", r)
