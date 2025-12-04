import numpy as np

def partsum(a, b, P):
    n = len(P) - 1
    if a >= n:
        return 0
    ap = max(a, 0)
    bp = min(b, n-1)
    return P[bp + 1] - P[ap]

def transform(nums):
    n = len(nums)
    P = np.concatenate(([0], np.cumsum(nums)))
    # P[b+1] - P[a] = nums[a] + ... + nums[b]
    out = np.zeros(n, dtype = int)
    for r in range(n):
        kmax = (n - r) // (4 * r + 4) + 1
        partial = 0
        for k in range(kmax):
            apos, bpos = (4*k + 1) * r + 4*k, (4*k + 2) * r + 4*k
            aneg, bneg = (4*k + 3) * r + 4*k + 2, (4*k + 4) * r + 4*k + 2
            partial += partsum(apos, bpos, P) - partsum(aneg, bneg, P)
        out[r] = abs(partial) % 10
    return out

def lazytransform(nums, offset):
    tail = nums[offset:].astype(int)
    for _ in range(100):
        total = 0
        for i in range(len(tail) - 1, -1, -1):
            total += tail[i]
            tail[i] = total % 10
    return tail[:8]

with open("input.txt") as f:
    numbers = [int(x) for x in f.readline().strip()]

# Part 1
nums = numbers.copy()
for i in range(100):
    nums = transform(nums)
print("Part 1 numbers are:", ''.join(map(str, nums[:8])))

# Part 2
offset = int(''.join(map(str, numbers[:7])))

nums = np.tile(numbers, 10_000)

if offset < len(nums) / 2:
    print("Offset is too small and so slow calculation is performed...")
    for i in range(100):
        nums = transform(nums)
        print(i + 1, "done")
    print("Part 2 answer:", ''.join(map(str, nums[offset:offset + 8])))
else:
    print("Lazy (faster) transform applies as the offset is large enough")
    out_list = lazytransform(nums, offset)
    output = ''.join(map(str, out_list))
    print("Part 2 answer:", output)



