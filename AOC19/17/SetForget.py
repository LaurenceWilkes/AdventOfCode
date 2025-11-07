from intcode import run_program, run_program_with_state
from solution import main_routine, A, B, C

C2D = {'<': (-1, 0), '>': (1, 0), 'v': (0, -1), '^': (0, 1)}

def get_grid(nums):
    grid = {}
    cur_pos = (0, 0)
    loc = 0
    relbase = 0
    out, loc, relbase = run_program(nums, loc, relbase, [])
    prlist = [] #
    while loc != -1:
        if out == 10:
            print(''.join(map(str, prlist))) #
            prlist = [] #
            cur_pos = (0, cur_pos[1] - 1)
            out, loc, relbase = run_program(nums, loc, relbase, [])
            continue
        grid[cur_pos] = chr(out)
        prlist.append(chr(out)) #
        out, loc, relbase = run_program(nums, loc, relbase, [])
        cur_pos = (cur_pos[0] + 1, cur_pos[1])
    return grid

def find_intersections(grid):
    out_sum = 0
    for k in grid.keys():
        if grid[k] != '#':
            continue
        k1 = (k[0]-1, k[1])
        k2 = (k[0]+1, k[1])
        k3 = (k[0], k[1]-1)
        k4 = (k[0], k[1]+1)
        if all(grid.get(ka) == '#' for ka in (k1, k2, k3, k4)):
            out_sum += k[0] * abs(k[1])
    return out_sum

def entire_path(grid):
    cur_pos = None
    for k in grid.keys():
        if grid[k] in ('<', '>', 'v', '^'):
            cur_pos = k
            break
    dx, dy = C2D[grid[cur_pos]] # hope first dir isn't pointing away from path
    out = []
    fcount = 0
    while True:
        fw = (cur_pos[0] + dx, cur_pos[1] + dy)
        if grid.get(fw) == '#':
            fcount += 1
            cur_pos = fw
            continue
        le = (cur_pos[0] - dy, cur_pos[1] + dx)
        ri = (cur_pos[0] + dy, cur_pos[1] - dx)
        if grid.get(le) == '#':
            out.append(fcount)
            fcount = 0
            out.append('L')
            dx, dy = -dy, dx
            continue
        elif grid.get(ri) == '#':
            out.append(fcount)
            fcount = 0
            out.append('R')
            dx, dy = dy, -dx
            continue
        else:
            out.append(fcount)
            break
    return out

def to_ascii(line):
    return [ord(c) for c in line] + [10]

def solve_and_print(nums, inputs):
    loc = 0
    relbase = 0
    input_cursor = 0
    prlist = []
    while True:
        out, loc, relbase, input_cursor = run_program_with_state(
            nums, loc, relbase, inputs, input_cursor
        )
        if loc == -1:
            if prlist:
                print(''.join(prlist))
            return
        if out > 255:
            print("Dust is", out)
            return
        if out == 10:
            print(''.join(prlist))
            prlist = []
        else:
            prlist.append(chr(out))

with open("input.txt") as f:
    numbers = [int(x) for x in f.read().strip().split(",")]

nums = numbers.copy()
grid = get_grid(nums)

# Part 1
out_sum = find_intersections(grid)
print("Part 1 alignment number is", out_sum)

# Part 2
path = entire_path(grid)

print("Part 2 path is", path)

# Lazy solution here as it was easy to see by eye what the subroutines were
## main_routine = # can be found in solution.py
## A =
## B =
## C =

inputs = to_ascii(main_routine) \
        + to_ascii(A) \
        + to_ascii(B) \
        + to_ascii(C) \
        + to_ascii("n")

nums = numbers.copy()
nums[0] = 2
solve_and_print(nums, inputs)




