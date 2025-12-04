from Intcode import run_program

def main():
    with open("input.txt") as f:
        numbers = [int(x) for x in f.readline().split(",")]

    # Part 1
    nums = numbers.copy()
    loc = 0
    relbase = 0
    block_count = 0
    while True:
        (x, loc, relbase) = run_program(nums, loc, relbase, [])
        if loc == -1:
            break
        (y, loc, relbase) = run_program(nums, loc, relbase, [])
        (tile_id, loc, relbase) = run_program(nums, loc, relbase, [])
        if tile_id == 2:
            block_count += 1
    print("Part 1 block count:", block_count)

    # Part 2
    nums = numbers.copy()
    nums[0] = 2

    loc = 0
    relbase = 0
    tiles = {}
    score = 0

    ball_x = 0
    paddle_x = 0

    # Draw
    prev_tile_count = 0
    while True:
        (x, loc, relbase) = run_program(nums, loc, relbase, [])
        if loc == -1:
            break
        (y, loc, relbase) = run_program(nums, loc, relbase, [])
        (tile_id, loc, relbase) = run_program(nums, loc, relbase, [])

        if tile_id == 4:
            ball_x = x
        elif tile_id == 3:
            paddle_x = x

        if x == -1 and y == 0:
            score = tile_id
        else:
            tiles[(x, y)] = tile_id

        tile_count = len(tiles)
        if tile_count == prev_tile_count:
            break
        prev_tile_count = tile_count

    # Play
    while True:
        input_vals = []
        if ball_x > paddle_x:
            input_vals = [1]
        elif ball_x < paddle_x:
            input_vals = [-1]
        else:
            input_vals = [0]
        (x, loc, relbase) = run_program(nums, loc, relbase, input_vals)
        if loc == -1:
            break
        (y, loc, relbase) = run_program(nums, loc, relbase, [])
        (tile_id, loc, relbase) = run_program(nums, loc, relbase, [])
        if tile_id == 4:
            ball_x = x
        if tile_id == 3:
            paddle_x = x
        if x == -1 and y == 0:
            score = tile_id
        else:
            tiles[(x, y)] = tile_id

    print("Part 2 final score:", score)

main()
