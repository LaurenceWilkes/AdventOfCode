from itertools import combinations

def counter(strings):
    twos = 0
    threes = 0
    for s in strings:
        char_counts = {}
        for c in s:
            char_counts[c] = char_counts.get(c, 0) + 1
        char_vals = set(char_counts.values())
        if 2 in char_vals:
            twos += 1
        if 3 in char_vals:
            threes += 1
    return twos * threes

def diff(s1, s2):
    idx = -1
    for i, (a, b) in enumerate(zip(s1, s2)):
        if a != b:
            if idx != -1:
                return -1
            idx = i
    return idx

def diff_check(strings):
    for s1, s2 in combinations(strings, 2):
        idx = diff(s1, s2)
        if idx != -1:
            return s1[:idx] + s1[idx + 1:]

def main():
    with open('input.txt', 'r') as file:
        strings = [s.strip() for s in file]
    print("Part 1:", counter(strings))
    print("Part 2:", diff_check(strings))

main()
