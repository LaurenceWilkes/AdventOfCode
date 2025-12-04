import itertools

def get_val(loc, nums, mode):
    return nums[loc] if mode == 1 else nums[nums[loc]]

def adding(loc, nums, modes):
    nums[nums[loc+3]] = get_val(loc+1, nums, modes[0]) \
                        + get_val(loc+2, nums, modes[1])

def multiplying(loc, nums, modes):
    nums[nums[loc+3]] = get_val(loc+1, nums, modes[0]) \
                        * get_val(loc+2, nums, modes[1])

def read_opcode(n):
    opcode = n % 100
    n //= 100
    c = n % 10
    n //= 10
    b = n % 10
    n //= 10
    a = n % 10
    return (opcode, c, b, a)

def run_program(nums, starting_loc, input_vals):
    loc = starting_loc
    input_ind = 0
    while True:
        (opcode, c, b, a) = read_opcode(nums[loc])
        modes = (c, b, a)
        if opcode == 1:
            adding(loc, nums, modes)
            loc += 4
        elif opcode == 2:
            multiplying(loc, nums, modes)
            loc += 4
        elif opcode == 3:
            nums[nums[loc+1]] = input_vals[input_ind]
            input_ind += 1
            loc += 2
        elif opcode == 4: ## output
            out = get_val(loc+1, nums, modes[0])
#            print("Output:", out)
            loc += 2
            return (out, loc)
        elif opcode == 5:
            if get_val(loc+1, nums, modes[0]) != 0:
                loc = get_val(loc+2, nums, modes[1])
            else:
                loc += 3
        elif opcode == 6:
            if get_val(loc+1, nums, modes[0]) == 0:
                loc = get_val(loc+2, nums, modes[1])
            else:
                loc += 3
        elif opcode == 7:
            first_param = get_val(loc+1, nums, modes[0])
            second_param = get_val(loc+2, nums, modes[1])
            if first_param < second_param:
                nums[nums[loc+3]] = 1
            else:
                nums[nums[loc+3]] = 0
            loc += 4
        elif opcode == 8:
            first_param = get_val(loc+1, nums, modes[0])
            second_param = get_val(loc+2, nums, modes[1])
            if first_param == second_param:
                nums[nums[loc+3]] = 1
            else:
                nums[nums[loc+3]] = 0
            loc += 4
        elif opcode == 99:
#            break
            return (0, -1)
        else:
            print("Error")

def test_sequence(nums, sequence):
    curval = 0
    for s in sequence:
        amp_prog = nums.copy()
        (curval, _) = run_program(amp_prog, 0, [s, curval])
    return curval

def test_sequence_loop(nums, sequence):
    curval = 0
    Eval = 0
    progs = [nums.copy() for i in range(5)]
    locs = [0, 0, 0, 0, 0]
    breakcount = 5
    for i in range(5):
        (curval, locs[i]) = run_program(progs[i], \
                                         locs[i], \
                                         [sequence[i], curval])
    while True:
        for i in range(5):
            (tempval, locs[i]) = run_program(progs[i], locs[i], [curval])
            if locs[i] == -1:
                return curval
            curval = tempval
#            if i == 5:
#                Eval = curval


with open("input.txt") as f:
    numbers = [int(x) for x in f.readline().split(",")]

# Part 1
nums = numbers.copy()
max_val = 0
for sequence in itertools.permutations([0, 1, 2, 3, 4]):
    val = test_sequence(nums, sequence)
    if val > max_val:
        max_val = val

print("Part 1 max val is", max_val)

# Part 2 (test)
nums = numbers.copy()
max_val = 0
for sequence in itertools.permutations([5, 6, 7, 8, 9]):
    val = test_sequence_loop(nums, sequence)
    if val > max_val:
        max_val = val
print()
print("Part 2 max val is", max_val)

