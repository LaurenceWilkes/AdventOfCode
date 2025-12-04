with open("input.txt") as f:
    v = f.readline().split("-")
    first_num = int(v[0])
    second_num = int(v[1])

def check_number(n):
    double_check = False
    prevc = 11
    while n > 0:
        c = n % 10
        if c == prevc:
            double_check = True
        elif c > prevc:
            return False
        prevc = c
        n = n // 10
    return double_check

def convert(n):
    v = []
    while n > 0:
        v.append(n % 10)
        n = n // 10
    return v

def lazy_double(v):
    if (v[0] == v[1] != v[2]) \
            or (v[0] != v[1] == v[2] != v[3]) \
            or (v[1] != v[2] == v[3] != v[4]) \
            or (v[2] != v[3] == v[4] != v[5]) \
            or (v[3] != v[4] == v[5]):
        return True
    else:
        return False

def check_number_pairs(n):
    vn = convert(n)
    prevdigit = 11
    for digit in vn:
        if digit > prevdigit:
            return False
        prevdigit = digit
    return lazy_double(vn)

# Part 1
total = 0
for n in range(first_num, second_num + 1):
    if check_number(n):
        total += 1

print("Part 1 total is", total)

# Part 2
total = 0
for n in range(first_num, second_num + 1):
    if check_number_pairs(n):
        total += 1

print("Part 2 total is", total)

