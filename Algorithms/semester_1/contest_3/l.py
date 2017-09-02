import sys

inp = sys.stdin.read().strip().splitlines()

for i in range(len(inp)):
    for j in range(len(inp) - 1, i, -1):
        if inp[j] + inp[j - 1] > inp[j - 1] + inp[j]:
            inp[j], inp[j - 1] = inp[j - 1], inp[j]

print(''.join(inp))
