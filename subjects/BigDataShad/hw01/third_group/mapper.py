#!/usr/bin/env python
import sys
import csv
import re
import bisect


access_log_pattern = re.compile('^(?P<ip>.*?) - - (?P<time>.*?) "(?P<page>.*?)" ' + \
        '(?P<code>.*?) .*? "(?P<referrer>.*?)" ".*?"$')

def open_database():
    arr = []
    with open('IP2LOCATION-LITE-DB1.CSV', 'r') as csvfile:
        reader = csv.reader(csvfile)
        for row in reader:
            arr.append((int(row[0]), row[3]))

    return sorted(arr)

database = open_database()

def to_raw_ip(ip):
    numbers = list(map(int, ip.split('.')))
    res = 0
    for i in range(4):
        res = res * 256 + numbers[i]

    return res


def find_country(raw_ip):
    key = (raw_ip, chr(255))
    pos = bisect.bisect_right(database, key) - 1
    return database[pos][1]


def work():
    for line in sys.stdin:
        match = re.match(access_log_pattern, line)

        if not match:
            continue

        if match.group('code') != '200':
            continue

        cur_ip_raw = to_raw_ip(match.group('ip'))
        country = find_country(cur_ip_raw)

        print('{}\t{}'.format(country, match.group('ip')))


if __name__ == '__main__':
    work()
