class IntcodeComputer:
    def __init__(self, nums, loc=0, relbase=0):
        self.nums = nums
        self.loc = loc
        self.rel = relbase
        self.halted = False

    def nums_extend(self, address):
        if address >= len(self.nums):
            self.nums.extend([0] * (address + 1 - len(self.nums)))

    def get_val(self, loc, mode):
        if mode == 1:
            return self.nums[loc]
        elif mode == 0:
            address = self.nums[loc]
        elif mode == 2:
            address = self.rel + self.nums[loc]
        self.nums_extend(address)
        return self.nums[address]

    def write_address(self, loc, mode):
        if mode == 0:
            address = self.nums[loc]
        elif mode == 2:
            address = self.rel + self.nums[loc]
        self.nums_extend(address)
        return address

    def read_opcode(self, n):
        opcode = n % 100
        n //= 100
        c = n % 10
        n //= 10
        b = n % 10
        n //= 10
        a = n % 10
        return opcode, c, b, a

    def run_program(self, input_vals=None, input_cursor=0):
        if input_vals is None:
            input_vals = []

        while True:
            opcode, m1, m2, m3 = self.read_opcode(self.nums[self.loc])

            if opcode == 1:  # add
                addr = self.write_address(self.loc + 3, m3)
                self.nums[addr] = self.get_val(self.loc + 1, m1) \
                                + self.get_val(self.loc + 2, m2)
                self.loc += 4
            elif opcode == 2:  # multiply
                addr = self.write_address(self.loc + 3, m3)
                self.nums[addr] = self.get_val(self.loc + 1, m1) \
                                * self.get_val(self.loc + 2, m2)
                self.loc += 4
            elif opcode == 3:  # input
                addr = self.write_address(self.loc + 1, m1)
                if input_cursor >= len(input_vals):
                    raise IndexError("Too few inputs provided")
                self.nums[addr] = input_vals[input_cursor]
                input_cursor += 1
                self.loc += 2
            elif opcode == 4:  # output
                out = self.get_val(self.loc + 1, m1)
                self.loc += 2
                return out, input_cursor
            elif opcode == 5:  # jump-if-true
                gvl1 = self.get_val(self.loc + 1, m1)
                gvl2 = self.get_val(self.loc + 2, m2)
                self.loc = gvl2 if gvl1 != 0 else self.loc + 3
            elif opcode == 6:  # jump-if-false
                gvl1 = self.get_val(self.loc + 1, m1)
                gvl2 = self.get_val(self.loc + 2, m2)
                self.loc = gvl2 if gvl1 == 0 else self.loc + 3
            elif opcode == 7:  # less than
                addr = self.write_address(self.loc + 3, m3)
                gvl1 = self.get_val(self.loc + 1, m1)
                gvl2 = self.get_val(self.loc + 2, m2)
                self.nums[addr] = 1 if gvl1 < gvl2 else 0
                self.loc += 4
            elif opcode == 8:  # equals
                addr = self.write_address(self.loc + 3, m3)
                gvl1 = self.get_val(self.loc + 1, m1)
                gvl2 = self.get_val(self.loc + 2, m2)
                self.nums[addr] = 1 if gvl1 == gvl2 else 0
                self.loc += 4
            elif opcode == 9:  # adjust relative base
                self.rel += self.get_val(self.loc + 1, m1)
                self.loc += 2
            elif opcode == 99:  # halt
                self.halted = True
                return None, input_cursor
            else:
                raise RuntimeError("Unknown opcode {}".format(opcode))

    def run_program_till_inp(self, input_vals=None, input_cursor=0):
        if input_vals is None:
            input_vals = []

        while True:
            opcode, m1, m2, m3 = self.read_opcode(self.nums[self.loc])

            if opcode == 1:
                addr = self.write_address(self.loc + 3, m3)
                self.nums[addr] = self.get_val(self.loc + 1, m1) \
                                + self.get_val(self.loc + 2, m2)
                self.loc += 4

            elif opcode == 2:
                addr = self.write_address(self.loc + 3, m3)
                self.nums[addr] = self.get_val(self.loc + 1, m1) \
                                * self.get_val(self.loc + 2, m2)
                self.loc += 4

            elif opcode == 3:  # needs input
                if input_cursor >= len(input_vals):
                    return None, input_cursor, True
                addr = self.write_address(self.loc + 1, m1)
                self.nums[addr] = input_vals[input_cursor]
                input_cursor += 1
                self.loc += 2

            elif opcode == 4:
                out = self.get_val(self.loc + 1, m1)
                self.loc += 2
                return out, input_cursor, False

            elif opcode == 5:
                gvl1 = self.get_val(self.loc + 1, m1)
                gvl2 = self.get_val(self.loc + 2, m2)
                self.loc = gvl2 if gvl1 != 0 else self.loc + 3

            elif opcode == 6:
                gvl1 = self.get_val(self.loc + 1, m1)
                gvl2 = self.get_val(self.loc + 2, m2)
                self.loc = gvl2 if gvl1 == 0 else self.loc + 3

            elif opcode == 7:
                addr = self.write_address(self.loc + 3, m3)
                gvl1 = self.get_val(self.loc + 1, m1)
                gvl2 = self.get_val(self.loc + 2, m2)
                self.nums[addr] = 1 if gvl1 < gvl2 else 0
                self.loc += 4

            elif opcode == 8:
                addr = self.write_address(self.loc + 3, m3)
                gvl1 = self.get_val(self.loc + 1, m1)
                gvl2 = self.get_val(self.loc + 2, m2)
                self.nums[addr] = 1 if gvl1 == gvl2 else 0
                self.loc += 4

            elif opcode == 9:
                self.rel += self.get_val(self.loc + 1, m1)
                self.loc += 2

            elif opcode == 99:
                self.halted = True
                return None, input_cursor, False

            else:
                raise RuntimeError("Unknown opcode {}".format(opcode))

    def is_halted(self):
        return self.halted

