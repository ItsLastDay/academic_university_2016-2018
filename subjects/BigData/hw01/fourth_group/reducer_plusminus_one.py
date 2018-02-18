#!/usr/bin/env python
import sys


def work(arg):
    to_print = arg
    last_ip = None

    for line in sys.stdin:
        ip, cnt = line.split('\t')

        if last_ip != ip:
            if last_ip is not None:
                print('{}\t{}'.format(last_ip, to_print))
        last_ip = ip

    if last_ip is not None:
        print('{}\t{}'.format(last_ip, to_print))


if __name__ == '__main__':
    work(sys.argv[1]) # 1 or -1
