#!/usr/bin/env python
import sys
import re
from time import strptime, mktime
from datetime import datetime, timedelta, tzinfo

access_log_pattern = re.compile('^(?P<ip>.*?) - - (?P<time>.*?) "(?P<page>.*?)" ' + \
        '(?P<code>.*?) .*? "(?P<referrer>.*?)" ".*?"$')

def process_request_time(request_time):
    # 24/Sep/2016:12:32:53 +0400
    date_time = strptime(request_time[:-6], "%d/%b/%Y:%H:%M:%S")
    timezone_secs = int(request_time[-4:-2]) * 3600 + \
            int(request_time[-2:]) * 60
    if request_time[-5] == '-':
        timezone_secs *= -1

    time_info = list(date_time[:6]) + [ 0, None ]
    
    dt = datetime(*time_info) 
    
    return dt - timedelta(seconds = timezone_secs)


def work():
    for line in sys.stdin:
        match = re.match(access_log_pattern, line)

        if not match:
            continue

        if match.group('code') != '200':
            continue

        time = int(mktime(process_request_time(match.group('time').strip('[]')).timetuple()))

        print('{}\t{}'.format(match.group('ip'), time))

if __name__ == '__main__':
    work()
