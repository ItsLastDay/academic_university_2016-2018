#!/usr/bin/env python
import sys

def work():
    previous_ip = None
    cnt = [0, 0]

    res = 0


    for line in sys.stdin:
        ip, mask = line.strip().split('\t')

        if ip != previous_ip:
            if previous_ip is not None:
                if cnt[1] > 0 and cnt[0] == 0:
                    res += 1

            cnt = [0, 0]

        previous_ip = ip
        cnt[int(mask)] += 1


    if previous_ip is not None:
        if cnt[1] > 0 and cnt[0] == 0:
            res += 1

    print(res)

if __name__ == '__main__':
    work()
