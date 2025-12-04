def track_card(instructions, deck_len, num):
    pos = num
    for line in instructions:
        if line == "deal into new stack":
            pos = deck_len - 1 - pos
        if line[0:3] == "cut":
            at = int(line[4:])
            pos = (pos - at) % deck_len
        if line[0:19] == "deal with increment":
            mul = int(line[20:])
            pos = (pos * mul) % deck_len
    return pos

def track_equations(instructions, deck_len):
    m, c = 1, 0
    for line in instructions:
        if line == "deal into new stack":
            m *= -1
            c = -1 - c
            c %= deck_len
            m %= deck_len
        if line[0:3] == "cut":
            at = int(line[4:])
            c -= at
            c %= deck_len
            m %= deck_len
        if line[0:19] == "deal with increment":
            mul = int(line[20:])
            m *= mul
            c *= mul
            c %= deck_len
            m %= deck_len
    return m, c

def modinv(a, n):
    t, nt = 0, 1
    r, nr = n, a % n
    while nr != 0:
        q = r // nr
        t, nt = nt, t - q * nt
        r, nr = nr, r - q * nr
    return t % n

def inv_trans(y, m, c, n, times):
    mk = pow(m, times, n)
    mki = modinv(mk, n)
    out = (y - c * (mk - 1) * modinv(m - 1, n)) % n
    out *= mki
    return out % n

with open("input.txt") as f:
    instructions = [line.strip() for line in f]

# Part 1
deck_len = 10_007
card_num = 2019
m, c = track_equations(instructions, deck_len)
print("Part 1 m:", m, " c:", c)
pos = (m * card_num + c) % deck_len
print("Part 1 card ends up at", pos)

# Part 2
long_deck_len = 119_315_717_514_047
shuffle_no = 101_741_582_076_661
m, c = track_equations(instructions, long_deck_len)
print("Part 2 m:", m, " c:", c)
card = inv_trans(2020, m, c, long_deck_len, shuffle_no)
print("Part 2 card at 2020 is", card)

