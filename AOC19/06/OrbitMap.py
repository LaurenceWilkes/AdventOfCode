parent_of = dict()
planets = set()
value = {"COM" : 0}
with open("input.txt") as f:
    for line in f:
        (parent, child) = line.strip().split(")")
        parent_of[child] = parent
        planets.add(parent)
        planets.add(child)

def value_of(planet):
    if planet in value:
        return value[planet]
    else:
        value[planet] = value_of(parent_of[planet]) + 1
        return value[planet]

# Part 1
print("Part 1:")
total = 0
for p in planets:
    total += value_of(p)
print("Total is", total)
print()

# Part 2
YOU_list = []
cur = "YOU"
while cur != "COM":
    cur = parent_of[cur]
    YOU_list.append(cur)

SAN_list = []
cur = "SAN"
while cur != "COM":
    cur = parent_of[cur]
    SAN_list.append(cur)

i = len(YOU_list) - 1
j = len(SAN_list) - 1

print("Part 2:")
while True:
    if YOU_list[i] != SAN_list[j]:
        print(i + j + 2)
        break
    if i == 0 or j == 0:
        print(i + j)
        break
    i -= 1
    j -= 1



