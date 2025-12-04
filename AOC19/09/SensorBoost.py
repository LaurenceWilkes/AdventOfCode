def nums_extend(nums, address):
    if address >= len(nums):
        nums.extend([0] * (address + 1 - len(nums)))

def get_val(loc, nums, relbase, mode):
    if mode == 0:
        address = nums[loc]
    elif mode == 1:
        return nums[loc]
    elif mode == 2:
        address = relbase + nums[loc]
    nums_extend(nums, address)
    return nums[address]

def write_address(loc, nums, relbase, mode):
    address = -1
    if mode == 0:
        address = nums[loc]
    elif mode == 2:
        address = relbase + nums[loc]
    else:
        print("error")
    nums_extend(nums, address)
    return address

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
    relbase = 0
    while True:
        (opcode, c, b, a) = read_opcode(nums[loc])
        modes = (c, b, a)
        if opcode == 1:
            address = write_address(loc+3, nums, relbase, modes[2])
            nums[address] = get_val(loc+1, nums, relbase, modes[0]) \
                                + get_val(loc+2, nums, relbase, modes[1])
            loc += 4
        elif opcode == 2:
            address = write_address(loc+3, nums, relbase, modes[2])
            nums[address] = get_val(loc+1, nums, relbase, modes[0]) \
                                * get_val(loc+2, nums, relbase, modes[1])
            loc += 4
        elif opcode == 3:
            address = write_address(loc+1, nums, relbase, modes[0])
            nums[address] = input_vals[input_ind]
            input_ind += 1
            loc += 2
        elif opcode == 4: ## output
            out = get_val(loc+1, nums, relbase, modes[0])
            print("Output:", out)
            loc += 2
#            return (out, loc)
        elif opcode == 5:
            if get_val(loc+1, nums, relbase, modes[0]) != 0:
                loc = get_val(loc+2, nums, relbase, modes[1])
            else:
                loc += 3
        elif opcode == 6:
            if get_val(loc+1, nums, relbase, modes[0]) == 0:
                loc = get_val(loc+2, nums, relbase, modes[1])
            else:
                loc += 3
        elif opcode == 7:
            first_param = get_val(loc+1, nums, relbase, modes[0])
            second_param = get_val(loc+2, nums, relbase, modes[1])
            if first_param < second_param:
                address = write_address(loc+3, nums, relbase, modes[2])
                nums[address] = 1
            else:
                address = write_address(loc+3, nums, relbase, modes[2])
                nums[address] = 0
            loc += 4
        elif opcode == 8:
            first_param = get_val(loc+1, nums, relbase, modes[0])
            second_param = get_val(loc+2, nums, relbase, modes[1])
            if first_param == second_param:
                address = write_address(loc+3, nums, relbase, modes[2])
                nums[address] = 1
            else:
                address = write_address(loc+3, nums, relbase, modes[2])
                nums[address] = 0
            loc += 4
        elif opcode == 9:
            relbase += get_val(loc+1, nums, relbase, modes[0])
            loc += 2
        elif opcode == 99:
#            break
            return (0, -1)
        else:
            print("Error")


with open("input.txt") as f:
    numbers = [int(x) for x in f.readline().split(",")]

# Part 1
nums = numbers.copy()
run_program(nums, 0, [1])

# Part 1
nums = numbers.copy()
run_program(nums, 0, [2])

