with open('input.txt', 'r') as file:
    changeList = [int(line.strip()) for line in file]

def partOne():
    freq = 0
    for c in changeList:
        freq += c
    print("Part 1:", freq)

def partTwo():
    freq = 0
    length = len(changeList)
    i = 0
    seen = set()
    while freq not in seen:
        seen.add(freq)
        freq += changeList[i]
        i = (i + 1) % length
    print("Part 2:", freq)

partOne()
partTwo()
