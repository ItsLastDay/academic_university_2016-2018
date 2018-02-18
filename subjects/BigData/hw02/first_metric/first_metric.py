from pyspark import SparkConf, SparkContext

import datetime
from operator import add
import sys
import re


access_log_pattern = re.compile('^(?P<ip>.*?) - - (?P<time>.*?) "(?P<page>.*?)" ' + \
        '(?P<code>.*?) .*? "(?P<referrer>.*?)" ".*?"$')
HDFS_DIR = '/user/bigdatashad/logs/{}'

def count_referers(events):
    events = events[1]
    start_timestamp = None
    last_timestamp = None
    result = []
    last_ref = ''

    for event in events:
        timestamp, ref = event

        if not start_timestamp or timestamp - last_timestamp > 30 * 60:
            if start_timestamp is not None:
                result.append((last_ref, 1))
            start_timestamp = timestamp
            last_ref = ref

        if timestamp == start_timestamp:
            last_ref = max(last_ref, ref)

        last_timestamp = timestamp

    if start_timestamp is not None:
        result.append((last_ref, 1))

    return result

def extract_fields(s):
    match = re.match(access_log_pattern, s)
    if not match or match.group('code') != '200':
        return []

    try:
        date = datetime.datetime.strptime(match.group('time')[1:-7], "%d/%b/%Y:%H:%M:%S")
    except:
        return []
    timestamp = int(date.strftime("%s"))

    return [((match.group('ip'), timestamp), 
            (timestamp, match.group('referrer')))]


date = sys.argv[1]

conf = SparkConf().setAppName("Session referrers")
sc = SparkContext(conf=conf)


log = sc.textFile("/user/bigdatashad/logs/{}".format(date))

fields = log.flatMap(extract_fields)

ips_sorted = fields.repartitionAndSortWithinPartitions(partitionFunc=lambda s: hash(s[0]))\
        .map(lambda x: (x[0][0], x[1]), True)
ips_grouped = ips_sorted.groupByKey()

ag_referers = ips_grouped.flatMap(count_referers)

#print(ag_referers.take(10))

total_result = ag_referers.foldByKey(0, add)
#print(total_result.take(10))

total_result.saveAsTextFile('/user/mkoltsov/hw2/first_metric/{}'.format(date))
