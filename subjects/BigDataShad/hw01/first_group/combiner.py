#!/usr/bin/env python
import sys

def work():
    last_ip = None
    last_cnt = 0 

    for line in sys.stdin:
        ip, cnt = line.split('\t')

        if ip != last_ip:
            if last_ip is not None:
                print('{}\t{}'.format(last_ip, last_cnt))
            last_cnt = 1
            last_ip = ip
        else:
            last_cnt += int(cnt)

    print('{}\t{}'.format(last_ip, last_cnt))

if __name__ == '__main__':
    work()
