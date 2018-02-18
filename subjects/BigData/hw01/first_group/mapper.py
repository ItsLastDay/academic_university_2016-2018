#!/usr/bin/env python
import re
import sys

access_log_pattern = re.compile('^(?P<ip>.*?) - - (?P<time>.*?) "(?P<page>.*?)" ' + \
        '(?P<code>.*?) .*? "(?P<referrer>.*?)" ".*?"$')

def work():
    for line in sys.stdin:
        match = re.match(access_log_pattern, line)

        if not match:
            continue

        if match.group('code') != '200':
            continue
        
        print('{}\t1'.format(match.group('ip')))


if __name__ == '__main__':
    work()