import sys
import math

inp = sys.stdin.read().strip().splitlines()

answer = [None for i in range(len(inp) // 2)]

for i in range(0, len(inp), 2):
    x = int(inp[i])
    y = int(inp[i + 1])
    answer[i // 2] = math.gcd(x, y)

print('\n'.join(map(str, answer)))
