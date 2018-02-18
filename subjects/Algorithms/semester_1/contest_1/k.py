import sys

def can(a, sz):
    return sum(x // sz for x in a)

inp = sys.stdin.read().strip().splitlines()

n, k = map(int, inp[0].split())

a = []
for i in range(1, len(inp)):
    a.append(int(inp[i]))



le = 0
rg = 10 ** 10
while rg - le > 1:
    mid = (le + rg) // 2
    if can(a, mid) >= k:
        le = mid
    else:
        rg = mid

print(le)

