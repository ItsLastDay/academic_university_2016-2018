#!/usr/bin/env python
import sys

def work():
    s = 0
    for line in sys.stdin.readlines():
        s += int(line)

    print(s)

if __name__ == '__main__':
    work()
