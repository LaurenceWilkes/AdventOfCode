with open("input.txt", "r") as f:
    lines = f.readlines()

# Part 1
total = 0
for line in lines:
    total += (int(line) // 3) - 2

print("Part 1 total is", total)

# Part 2
total = 0
for line in lines:
    fuel = (int(line) // 3) - 2
    while fuel > 0:
        total += fuel
        fuel = (fuel // 3) - 2

print("Part 2 total is", total)

