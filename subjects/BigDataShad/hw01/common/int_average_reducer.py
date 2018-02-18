#!/usr/bin/env python
import sys

def work():
    num_lines = 0
    total = 0

    for line in sys.stdin.readlines():
        num_lines += 1
        total += int(line)

    print(total / float(num_lines))

if __name__ == '__main__':
    work()
