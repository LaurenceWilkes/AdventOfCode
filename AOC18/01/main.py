with open('input.txt', 'r') as file:
    change_list = [int(line.strip()) for line in file]

def part_one():
    freq = 0
    for c in change_list:
        freq += c
    print("Part 1:", freq)

def part_two():
    freq = 0
    length = len(change_list)
    i = 0
    seen = set()
    while freq not in seen:
        seen.add(freq)
        freq += change_list[i]
        i = (i + 1) % length
    print("Part 2:", freq)

part_one()
part_two()
