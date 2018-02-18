#!/usr/bin/env python3

import sys


def main():
    cur_lineno = 1
    for line in sys.stdin:
        print('{:6}\t{}'.format(cur_lineno, line), end='')
        cur_lineno += 1

if __name__ == '__main__':
    main()
