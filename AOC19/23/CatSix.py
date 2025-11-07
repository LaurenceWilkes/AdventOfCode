from intcode import IntcodeComputer

with open("input.txt") as f:
    program = [int(x) for x in f.readline().strip().split(",")]

def sim_network(program):
    computers = [IntcodeComputer(program.copy()) for _ in range(50)]
    cursors = [0 for _ in range(50)]
    inputs = [[i] for i in range(50)]
    pending = [[] for _ in range(50)]

    while True:
        for i in range(50):
            if computers[i].is_halted():
                continue

            out, cursor, need_inp = computers[i].run_program_till_inp(
                                                    inputs[i], cursors[i]
                                                )
            if need_inp:
                inputs[i].append(-1)
            else:
                cursors[i] = cursor
                if cursors[i] >= 2:
                    inputs[i] = inputs[i][cursors[i]:]
                    cursors[i] = 0
                if out is not None:
                    pending[i].append(out)

            if len(pending[i]) == 3:
                addr, x, y = pending[i]
                if addr == 255:
                    print("Part 1 output is x:", x, " y:", y)
                    return
                pending[i] = []
                inputs[addr].append(x)
                inputs[addr].append(y)

def sim_network_NAT(program):
    computers = [IntcodeComputer(program.copy()) for _ in range(50)]
    cursors = [0 for _ in range(50)]
    inputs = [[i] for i in range(50)]
    pending = [[] for _ in range(50)]

    NAT_x, NAT_y = None, None
    yvals = set()

    while True:
        progress = 0
        for i in range(50):
            if computers[i].is_halted():
                continue

            out, cursor, need_inp = computers[i].run_program_till_inp(
                                                    inputs[i], cursors[i]
                                                )
            if need_inp:
                inputs[i].append(-1)
            else:
                progress += 1
                cursors[i] = cursor
                if out is not None:
                    pending[i].append(out)

            if len(pending[i]) == 3:
                addr, x, y = pending[i]
                pending[i] = []
                if addr == 255:
                    NAT_x = x
                    NAT_y = y
                    continue
                inputs[addr].append(x)
                inputs[addr].append(y)
        if progress == 0 and NAT_x != None:
            inputs[0].append(NAT_x)
            inputs[0].append(NAT_y)
            if NAT_y in yvals:
                print("Part 2 Multiply sent y value is", NAT_y)
                return
            yvals.add(NAT_y)

# Part 1
sim_network(program)
# Part 2
print("\nPart 2 runs in ~30s -- Lots of things could be improved...")
sim_network_NAT(program)

