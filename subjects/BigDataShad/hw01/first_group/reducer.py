#!/usr/bin/env python
import sys


def work():
    unique_ips = 0
    last_ip = None

    for line in sys.stdin:
        ip, cnt = line.split('\t')

        if last_ip != ip:
            last_ip = ip
            unique_ips += 1

    print(unique_ips)



if __name__ == '__main__':
    work()
