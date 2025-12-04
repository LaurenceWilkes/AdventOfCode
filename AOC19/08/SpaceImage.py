layersize = 150

with open("input.txt") as f:
    enc = [int(x) for x in f.read().strip()]

layers = [enc[i:i + layersize] for i in range(0, len(enc), layersize)]

# Part 1
min_layer = min(layers, key=lambda layer: layer.count(0))

answer = min_layer.count(1) * min_layer.count(2)

print("Part 1 output is", answer)

# Part 2
output = []

for i in range(layersize):
    for layer in layers:
        li = layer[i]
        if li != 2:
            output.append(li)
            break

for i in range(6):
    print(''.join(" " if x == 0 else "#" for x in output[25*i:25*(i+1)]))

