import re

def find_ore(mineral, quantity, store):
    if mineral == "ORE":
        return quantity
    if mineral not in store:
        store[mineral] = 0
        store_quantity = 0
    else:
        store_quantity = store[mineral]
    if store_quantity >= quantity:
        store[mineral] -= quantity
        return 0
    quantity_needed = quantity - store_quantity
    store[mineral] = 0

    min_quant, ingredients = instructions[mineral]
    m, r = quantity_needed // min_quant, quantity_needed % min_quant
    if r != 0:
        m += 1
        r = min_quant - r
    store[mineral] += r
    out_ore = 0
    for num, ing in ingredients:
        out_ore += find_ore(ing, m * num, store)
    return out_ore


instructions = {}
with open("input.txt") as f:
    for line in f:
        pattern = r"\d+ [a-zA-Z]+"
        lp = [p.group().split() for p in re.finditer(pattern, line)]
        list_pairs = [(int(num), word) for num, word in lp]
        num, key = list_pairs.pop()
        instructions[key] = (num, tuple(list_pairs))

# Part 1
store = {}
ore_required = find_ore("FUEL", 1, store)
print("Ore required for one fuel is", ore_required)

# Part 2
TOTAL_ORE = 1_000_000_000_000
start_fuel = TOTAL_ORE // ore_required # less than what could be made
fuel_no = start_fuel
index = 1
while ore_required < TOTAL_ORE:
    store = {}
    ore_required = find_ore("FUEL", fuel_no, store)
    fuel_no += pow(2, index)
    index += 1
end_fuel = fuel_no # more than what could be made with TOTAL_ORE

while end_fuel - start_fuel > 1:
    centre = (start_fuel + end_fuel) // 2
    store = {}
    ore_required = find_ore("FUEL", centre, store)
    if ore_required <= TOTAL_ORE:
        start_fuel = centre
    else:
        end_fuel = centre

print("Fuel to be made with", TOTAL_ORE, "ore is", start_fuel)


