def adding(loc, nums):
    nums[nums[loc+3]] = nums[nums[loc+1]] + nums[nums[loc+2]]

def multiplying(loc, nums):
    nums[nums[loc+3]] = nums[nums[loc+1]] * nums[nums[loc+2]]

def run_program(nums):
    loc = 0
    while True:
        if nums[loc] == 1:
            adding(loc, nums)
        elif nums[loc] == 2:
            multiplying(loc, nums)
        elif nums[loc] == 99:
            break
        else:
            print("Error")
        loc += 4

with open("input.txt") as f:
    numbers = [int(x) for x in f.readline().split(",")]

# Part 1
nums = numbers.copy()
nums[1] = 12 
nums[2] = 2
run_program(nums)
print("Part 1 value at 0 is", nums[0])

# Part 2
for noun in range(100):
    for verb in range(100):
        nums = numbers.copy()
        nums[1] = noun
        nums[2] = verb
        run_program(nums)
        if nums[0] == 19690720:
            print("Part 2 noun:", noun, 
                  "verb:", verb, 
                  "result:", 100 * noun + verb)
