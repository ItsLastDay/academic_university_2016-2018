from pyspark import SparkConf, SparkContext
import happybase

import random
import sys
import datetime as dt
import re


access_log_pattern = re.compile('^(?P<ip>.*?) - - (?P<time>.*?) "(?P<page>.*?)" ' + \
        '(?P<code>.*?) .*? "(?P<referrer>.*?)" ".*?"$')
liked_page = re.compile('^GET /id(?P<profile>\d+).*$')
HDFS_DIR = '/user/bigdatashad/logs/{}'

HOSTS = ["hadoop2-%02d.yandex.ru" % i for i in xrange(11, 14)]
TABLE = 'bigdatashad_mkoltsov_secondmetric'


def is_valid_req(s):
    match = re.match(access_log_pattern, s)
    if not match:
        return False

    return match.group('code') == '200' 

def get_good(s):
    match = re.match(access_log_pattern, s)
    req = match.group('page')
    
    liked = re.match(liked_page, req)

    if not liked:
        return []

    try:
        tm = match.group('time')
        tm = tm[tm.index(':')+1 : tm.index(':') + 3]
        return [
                (
                    (liked.group('profile'), match.group('ip')), 
                    (match.group('ip'), tm)
                )
               ]
    except:
        return []

def commit(table, row, col, cnt):
    table.counter_set(row, col, cnt)

def get_liked(iter):
    host = random.choice(HOSTS)
    conn = happybase.Connection(host)
    conn.open()
    table = happybase.Table(TABLE, conn)

    prev = None
    cnt = 0
    for s in iter:
        prof, _ = s[0]
        _, tm = s[1]

        row = bytes('{}_{}'.format(prof, date))
        col = 'h:{}'.format(tm)
        if s != prev:
            if prev is not None:
                commit(table, row, col, cnt)
            cnt = 1

        prev = s

    if prev is not None:
        commit(table, row, col, cnt)

    conn.close()


def get_liked_profiles(date):
    print('reading logs {}'.format(date))
    lines = sc.textFile(HDFS_DIR.format(date))
    valid_reqs = lines.filter(is_valid_req)\
        .flatMap(get_good)\
        .repartitionAndSortWithinPartitions(partitionFunc=lambda x: hash(x[0]))
    valid_reqs.foreachPartition(get_liked)


conf = SparkConf().setAppName("Profile users")
sc = SparkContext(conf=conf)


yy, mm, dd = sys.argv[1].split('-')
date = dt.date(day=int(dd), month=int(mm), year=int(yy))

get_liked_profiles(str(date))
sc.parallelize([]).saveAsTextFile('/user/mkoltsov/hw3/second_metric/{}'.format(date))
