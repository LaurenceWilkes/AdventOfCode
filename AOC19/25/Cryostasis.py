from intcode import IntcodeComputer
from itertools import combinations

ITEMS = ["coin", "mutex", "boulder", "antenna", "pointer", "cake",
         "tambourine", "fuel cell"]

INITIAL_CMDS = [
        "west",
        "west",
        "west",
        "take coin",
        "east",
        "east",
        "east",
        "north",
        "north",
        "take mutex",
        "east",
        "take antenna",
        "west",
        "south",
        "east",
        "take cake",
        "east",
        "north",
        "take pointer",
        "south",
        "west",
        "west",
        "south",
        "east",
        "east",
        "take tambourine",
        "east",
        "take fuel cell",
        "east",
        "take boulder",
        "north"
    ]

def run_interactive(program):
    comp = IntcodeComputer(program[:])
    input_vals = []
    cursor = 0
    while True:
        out, cursor, need_inp = comp.run_program_till_inp(input_vals, cursor)
        if out is not None:
            print(chr(out), end="")
        if need_inp or comp.is_halted():
            break

    for cmd in INITIAL_CMDS:
        cmd += "\n"
        input_vals = [ord(c) for c in cmd]
        cursor = 0
        while True:
            out, cursor, need_inp = comp.run_program_till_inp(
                                                input_vals, cursor
                                            )
            if out is not None:
                print(chr(out), end="")
            if need_inp or comp.is_halted():
                break

    while not comp.is_halted():
        command = input("> ") + "\n"
        input_vals = [ord(c) for c in command]
        cursor = 0
        while True:
            out, cursor, need_inp = comp.run_program_till_inp(
                                                input_vals, cursor
                                            )
            if out is not None:
                print(chr(out), end="")
            if need_inp or comp.is_halted():
                break

def auto_run(program):
    comp = IntcodeComputer(program[:])
    input_vals = []
    cursor = 0

    while True:
        out, cursor, need_inp = comp.run_program_till_inp(input_vals, cursor)
        if out is not None:
            print(chr(out), end="")
        if need_inp or comp.is_halted():
            break

    for cmd in INITIAL_CMDS:
        cmd += "\n"
        input_vals = [ord(c) for c in cmd]
        cursor = 0
        while True:
            out, cursor, need_inp = comp.run_program_till_inp(
                                                input_vals, cursor
                                            )
            if out is not None:
                print(chr(out), end="")
            if need_inp or comp.is_halted():
                break

    while not comp.is_halted():
        print("~~ Trying all possibilities ~~")
        for r in range(len(ITEMS) + 1):
            for subs in combinations(ITEMS, r):
                for item in subs:
                    command = "drop " + item + "\n"
                    input_vals = [ord(c) for c in command]
                    cursor = 0

                    while True:
                        out, cursor, need_inp = comp.run_program_till_inp(
                                                            input_vals, cursor
                                                        )
                        if need_inp or comp.is_halted():
                            break
                command = "east" + "\n"
                input_vals = [ord(c) for c in command]
                cursor = 0
                out_list = []
                while True:
                    out, cursor, need_inp = comp.run_program_till_inp(
                                                        input_vals, cursor
                                                    )
                    if out is not None:
                        out_list.append(chr(out))
                    if need_inp or comp.is_halted():
                        break

                out_string = "".join(out_list)
                if "Alert" not in out_string:
                    print(out_string, end="")
                    return

                for item in subs:
                    command = "take " + item + "\n"
                    input_vals = [ord(c) for c in command]
                    cursor = 0
                    while True:
                        out, cursor, need_inp = comp.run_program_till_inp(
                                                            input_vals, cursor
                                                        )
                        if need_inp or comp.is_halted():
                            break

def main():
    with open("input.txt") as f:
        program = [int(x) for x in f.readline().strip().split(",")]
#    run_interactive(program)
    auto_run(program)

main()
