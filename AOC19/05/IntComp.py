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

def run_program(nums, input_vals):
    loc = 0
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
        elif opcode == 4:
            out = get_val(loc+1, nums, modes[0])
            print("Output:", out)
            loc += 2
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
            break
        else:
            print("Error")

with open("input.txt") as f:
    numbers = [int(x) for x in f.readline().split(",")]

# Part 1
nums = numbers.copy()
print()
print("Part 1:")
run_program(nums, [1])

# Part 2
nums = numbers.copy()
print()
print("Part 2:")
run_program(nums, [5])
