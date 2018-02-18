from pyspark import SparkConf, SparkContext
import happybase

import random
import sys
import datetime as dt
import re


access_log_pattern = re.compile('^(?P<ip>.*?) - - (?P<time>.*?) "(?P<page>.*?)" ' + \
        '(?P<code>.*?) .*? "(?P<referrer>.*?)" ".*?"$')
liked_page = re.compile('^GET /id(?P<profile>\d+)\?like=1.*$')
HDFS_DIR = '/user/bigdatashad/logs/{}'

HOSTS = ["hadoop2-%02d.yandex.ru" % i for i in xrange(11, 14)]
TABLE = 'bigdatashad_mkoltsov_fourthmetric'


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
        date = dt.datetime.strptime(match.group('time')[1:-7], "%d/%b/%Y:%H:%M:%S")
        tm = int(date.strftime("%s"))
        return [
                (
                    (liked.group('profile'), match.group('ip')), 
                    (match.group('ip'), tm)
                )
               ]
    except:
        return []

def get_liked(iter):
    host = random.choice(HOSTS)
    conn = happybase.Connection(host)
    conn.open()
    table = happybase.Table(TABLE, conn)

    for s in iter:
        prof, ip = s[0]
        _, tm = s[1]

        table.put(bytes('{}_{}'.format(prof, date)), {'t:1': ip}, tm)

    conn.close()


def get_liked_profiles(date):
    print('reading logs {}'.format(date))
    lines = sc.textFile(HDFS_DIR.format(date))
    valid_reqs = lines.filter(is_valid_req)\
        .flatMap(get_good).repartitionAndSortWithinPartitions()
    valid_reqs.foreachPartition(get_liked)


conf = SparkConf().setAppName("Profile users")
sc = SparkContext(conf=conf)


yy, mm, dd = sys.argv[1].split('-')
date = dt.date(day=int(dd), month=int(mm), year=int(yy))

get_liked_profiles(str(date))
get_liked_profiles(str(date - dt.timedelta(days=1)))
get_liked_profiles(str(date - dt.timedelta(days=2)))
get_liked_profiles(str(date - dt.timedelta(days=3)))
get_liked_profiles(str(date - dt.timedelta(days=4)))
sc.parallelize([]).saveAsTextFile('/user/mkoltsov/hw3/fourth_metric/{}'.format(date))
