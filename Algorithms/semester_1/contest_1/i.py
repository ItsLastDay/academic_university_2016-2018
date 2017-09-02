import sys

def search_root(n):
    le = 0
    rg = n + 1
    while rg - le > 1:
        mid = (le + rg) // 2
        if mid ** 2 <= n:
            le = mid
        else:
            rg = mid
    return le

tests = sys.stdin.read().strip().splitlines()

for t in tests:
    n = int(t)
    print(search_root(n))
