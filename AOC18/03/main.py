import re

def intersect(c1, c2):
    x1, y1, w1, h1 = c1
    x2, y2, w2, h2 = c2
    x = max(x1, x2)
    y = max(y1, y2)
    w = min(x1 + w1, x2 + w2) - x
    h = min(y1 + h1, y2 + h2) - y
    return (x, y, w, h) if w > 0 and h > 0 else None

def count_points(claims):
    fabric = {}
    for x, y, w, h in claims:
        for i in range(x, x + w):
            for j in range(y, y + h):
                fabric[(i, j)] = fabric.get((i, j), 0) + 1
    return sum(1 for v in fabric.values() if v >= 2)

# Suboptimal because there are so many claims...
# i.e. claims^2 > claims * avg_claim_size
def single_intersect(claims):
    for i, c in enumerate(claims):
        for cp in claims:
            if cp == c:
                continue
            int = intersect(c, cp)
            if int is not None:
                break
        else:
            return i + 1
    return None

def main():
    with open('input.txt', 'r') as file:
        substrings = [re.findall(r'\d+', line)[1:] for line in file]
        claims = [tuple(int(n) for n in nums) for nums in substrings]
    print('Part 1:', count_points(claims))
    print('Part 2:', single_intersect(claims))

main()
