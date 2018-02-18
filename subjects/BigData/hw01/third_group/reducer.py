#!/usr/bin/env python
import sys

def work():
    last_country = None
    last_user = None
    cnt_users = 0

    for line in sys.stdin:
        line = line.strip()
        assert len(line.split('\t')) == 2, line.replace('\t', '<TAB>')
        country, ip = line.split('\t')

        if last_country != country:
            if last_country is not None:
                print('{}\t{}'.format(last_country, cnt_users))
            cnt_users = 1
        else:
            cnt_users += last_user != ip

        last_user = ip
        last_country = country

    if last_country is not None:
        print('{}\t{}'.format(last_country, cnt_users))

if __name__ == '__main__':
    work()
