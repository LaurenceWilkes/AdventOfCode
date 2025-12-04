from intcode import run_program

def isbeam(x, y, nums):
    prog = nums.copy()
    out, _, _ = run_program(prog, 0, 0, [x, y])
    return out

def issquarein(x, y, s, nums):
    return isbeam(x, y + s - 1, nums) == 1 and isbeam(x + s - 1, y, nums) == 1

def find_closest(s, nums):
    x, y = s, 0
    while True:
        while isbeam(x, y, nums) == 0:
            y += 1
        tx = x - s + 1
        if tx >= 0 and issquarein(tx, y, s, nums):
            return tx, y
        x += 1

with open("input.txt") as f:
    numbers = [int(x) for x in f.readline().strip().split(",")]

# Part 1
count = 0
for i in range(50):
    line = []
    for j in range(50):
        out = isbeam(i, j, numbers)
        line.append("#" if out == 1 else ".")
        count += out
    print(''.join(line))
print("Part 1 count is", count)

# Part 2
xp, yp = find_closest(100, numbers)
print("x:", xp, "y:", yp)
print("Part 2 output is", xp * 10_000 + yp)
