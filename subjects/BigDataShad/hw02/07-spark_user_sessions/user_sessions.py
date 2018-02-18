#!/usr/bin/env python

import os
import datetime
import re

from pyspark import SparkConf, SparkContext

def main():
    conf = SparkConf().setAppName("User sessions")
    sc = SparkContext(conf=conf)

    log = sc.textFile("/user/bigdatashad/logs/2017-09-01")

    fields = log.map(extract_fields).filter(lambda x: x is not None)
    print "**********\nLog rows count: %d\n**********" % fields.count()

    ips_sorted = fields.repartitionAndSortWithinPartitions(8, tuple_partitioner)\
        .map(lambda x: (x[0][0], x[1]))
    ips_grouped = ips_sorted.groupByKey()
    print ips_grouped.take(10)

    sessions = ips_grouped.mapValues(count_user_sessions)
    print "**********\nUsers count: %d\n**********" % ips_grouped.count()
    print sessions.take(10)

    # use sessions.fold(...) to sum for all keys


LOG_LINE_RE = re.compile('([\d\.:]+) - - \[(\S+) [^"]+\] "(\w+) ([^"]+) (HTTP/[\d\.]+)" (\d+) \d+ "([^"]+)" "([^"]+)"')

def extract_fields(line):
    match = LOG_LINE_RE.search(line.strip())
    if not match:
        return
    if match.group(6) != "200":
        return
    ip = match.group(1)
    date = datetime.datetime.strptime(match.group(2), "%d/%b/%Y:%H:%M:%S")
    url = match.group(4)
    timestamp = int(date.strftime("%s"))
    return ((ip, timestamp), (timestamp, 1))


def tuple_partitioner(pair):
    return hash(pair[0])


def count_user_sessions(events):
    sessions = 0
    sessions_length = 0
    start_timestamp = None
    last_timestamp = None
    hits = 0

    for event in events:
        timestamp, hit = event
        if not start_timestamp:
            start_timestamp = last_timestamp = timestamp

        if timestamp - last_timestamp >  30*60:
            sessions += 1
            sessions_length += last_timestamp - start_timestamp
            start_timestamp = timestamp
        last_timestamp = timestamp
        hits += hit # += 1

    sessions += 1
    sessions_length += last_timestamp - start_timestamp
    return (sessions, sessions_length, hits)


if __name__ == "__main__":
    main()

