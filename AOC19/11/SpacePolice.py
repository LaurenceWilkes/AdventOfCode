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

def run_program(nums, starting_loc, relbase, input_vals):
    loc = starting_loc
    input_ind = 0
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
#            print("Output:", out)
            loc += 2
            return (out, loc, relbase)
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
            return (0, -1, relbase)
        else:
            print("Error")

## End of computer

def turn_left(cur_pos, cur_dir):
    new_dir = (-cur_dir[1], cur_dir[0])
    new_pos = (cur_pos[0] + new_dir[0], cur_pos[1] + new_dir[1])
    return (new_pos, new_dir)

def turn_right(cur_pos, cur_dir):
    new_dir = (cur_dir[1], -cur_dir[0])
    new_pos = (cur_pos[0] + new_dir[0], cur_pos[1] + new_dir[1])
    return (new_pos, new_dir)

def display_painted(painted):
    xs = [x for x, y in painted]
    ys = [y for x, y in painted]
    min_x, max_x = min(xs), max(xs)
    min_y, max_y = min(ys), max(ys)
    for y in range(max_y, min_y - 1, -1):
        row = ""
        for x in range(min_x, max_x + 1):
            row += "█" if (x, y) in painted else " "
        print(row)

# Part 1
with open("input.txt") as f:
    numbers = [int(x) for x in f.readline().split(",")]

def run_robot(nums, starting_white):
    cur_pos = (0, 0)
    cur_dir = (0, 1)
    painted_white = set(starting_white)
    visited = set()
    loc = 0
    relbase = 0
    while True:
        visited.add(cur_pos)
        colour = 0
        if cur_pos in painted_white:
            colour = 1
        (out_colour, loc, relbase) = run_program(nums, loc, relbase, [colour])
        if loc == -1:
            break
        (out_dir, loc, relbase) = run_program(nums, loc, relbase, [])
        if out_colour == 0 and colour == 1:
            painted_white.remove(cur_pos)
        elif out_colour == 1 and colour == 0:
            painted_white.add(cur_pos)
        if out_dir == 0:
            (cur_pos, cur_dir) = turn_left(cur_pos, cur_dir)
        else:
            (cur_pos, cur_dir) = turn_right(cur_pos, cur_dir)
    return visited, painted_white

# Part 1
nums = numbers.copy()
visited, painted_white = run_robot(nums, [])
print("Tiles visited:", len(visited))
print()

# Part 2
nums = numbers.copy()
visited, painted_white = run_robot(nums, [(0, 0)])
display_painted(painted_white)

