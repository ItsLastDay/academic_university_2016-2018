import sys

# Useful for merging several *disjoint* *small* files into one:
# just make 1 mapper of such kind
def work():
    for line in sys.stdin.readlines():
        print(line)

if __name__ == '__main__':
    work()
