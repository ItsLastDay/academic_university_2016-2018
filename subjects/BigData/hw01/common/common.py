import re
from time import strptime
from datetime import datetime, timedelta, tzinfo

access_log_pattern = re.compile('^(?P<ip>.*?) - - (?P<time>.*?) "(?P<page>.*?)" ' + \
        '(?P<code>.*?) .*? "(?P<referrer>.*?)" ".*?"$')

# Modified version of https://gist.github.com/rubayeet/2366796#file-apache_log_parser-py-L185
def process_request_time(request_time):
    date_time = strptime(request_time[:-6], "%d/%b/%Y:%H:%M:%S")
    timezone_secs = int(request_time[-4:-2]) * 3600 + 
            int(request_time[-2:]) * 60

    time_info = list(date_time[:6]) + [ 0, None ]
    
    dt = datetime(*time_info) 
    
    return dt -  timedelta(seconds = timezone_secs)


def to_raw_ip(ip):
    numbers = list(map(int, ip.split('.')))
    res = 0
    for i in range(4):
        res = res * 256 + numbers[i]

    return res
