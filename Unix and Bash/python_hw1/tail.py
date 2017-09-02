#!/usr/bin/env python3

import sys
from collections import deque


def main():
    sys.stdin = open(sys.argv[1], 'r')
    last_lines = deque()
    for line in sys.stdin:
        last_lines.append(line)
        if len(last_lines) > 10:
            last_lines.popleft()

    print(''.join(last_lines), end='')

if __name__ == '__main__':
    main()
