from intcode import run_program_with_state

with open("input.txt") as f:
    numbers = [int(x) for x in f.readline().strip().split(",")]

NOT  = [ord('N'), ord('O'), ord('T')]
AND  = [ord('A'), ord('N'), ord('D')]
OR   = [ord('O'), ord('R')]
WALK = [ord('W'), ord('A'), ord('L'), ord('K'), 10]
RUN  = [ord('R'), ord('U'), ord('N'), 10]

def write_command(cmd, first, second):
    return cmd + [ord(' ')] + [ord(first)] + [ord(' ')] + [ord(second), 10]

def simple_spring(nums):
    loc = 0
    relbase = 0
    cursor = 0
    line = []
    inputs = write_command(NOT, 'J', 'T') \
            + write_command(AND, 'A', 'T') \
            + write_command(AND, 'B', 'T') \
            + write_command(AND, 'C', 'T') \
            + write_command(NOT, 'T', 'J') \
            + write_command(AND, 'D', 'J') \
            + WALK
    while True:
        out, loc, relbase, cursor = run_program_with_state(
                nums, loc, relbase, inputs, cursor
            )
        if out == 10:
            print(''.join(line))
            line = []
        if out > 255:
            print("Value produced is", out)
            break
        line.append(chr(out))

def nine_spring(nums):
    loc = 0
    relbase = 0
    cursor = 0
    line = []
    inputs = write_command(NOT, 'J', 'T') \
            + write_command(AND, 'A', 'T') \
            + write_command(AND, 'B', 'T') \
            + write_command(AND, 'C', 'T') \
            + write_command(NOT, 'T', 'J') \
            + write_command(AND, 'D', 'J') \
            + write_command(OR, 'E', 'T') \
            + write_command(OR, 'H', 'T') \
            + write_command(AND, 'T', 'J') \
            + RUN
    while True:
        out, loc, relbase, cursor = run_program_with_state(
                nums, loc, relbase, inputs, cursor
            )
        if out == 10:
            print(''.join(line))
            line = []
        if out > 255:
            print("Value produced is", out)
            break
        line.append(chr(out))

# Part 1
nums = numbers.copy()
simple_spring(nums)

# Part 2
nums = numbers.copy()
nine_spring(nums)

