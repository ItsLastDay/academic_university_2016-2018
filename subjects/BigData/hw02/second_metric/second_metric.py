from pyspark import SparkConf, SparkContext
from pyspark.sql import Row, SparkSession

import sys
import datetime as dt
import re


access_log_pattern = re.compile('^(?P<ip>.*?) - - (?P<time>.*?) "(?P<page>.*?)" ' + \
        '(?P<code>.*?) .*? "(?P<referrer>.*?)" ".*?"$')
liked_page = re.compile('^GET /id(?P<profile>\d+)\?like=1.*$')
HDFS_DIR = '/user/bigdatashad/logs/{}'

def is_valid_req(s):
    match = re.match(access_log_pattern, s)
    if not match:
        return False

    return match.group('code') == '200' 

def get_liked(s):
    match = re.match(access_log_pattern, s)
    if not match:
        return []
    req = match.group('page')
    
    liked = re.match(liked_page, req)
    if not liked:
        return []

    return [Row(id=int(liked.group('profile')))]


def get_liked_profiles(date, name):
    print('reading logs {}'.format(date))
    lines = sc.textFile(HDFS_DIR.format(date))
    valid_reqs = lines.filter(is_valid_req)
    res = valid_reqs.flatMap(get_liked)
#    print('result****', res.collect())
#    res = sc.parallelize([Row(id=1), Row(id=2)])
    schema = spark.createDataFrame(res)
    schema.createOrReplaceTempView(name)


conf = SparkConf().setAppName("Profile liked three days")
sc = SparkContext(conf=conf)
spark = spark = SparkSession.builder \
        .getOrCreate()


yy, mm, dd = sys.argv[1].split('-')
date = dt.date(day=int(dd), month=int(mm), year=int(yy))

get_liked_profiles(str(date), 's1')
get_liked_profiles(str(date - dt.timedelta(days=1)), 's2')
get_liked_profiles(str(date - dt.timedelta(days=2)), 's3')

result = spark.sql("SELECT * FROM s1 INTERSECT SELECT * FROM s2 INTERSECT\
        SELECT * FROM s3").count()

sc.parallelize([result]).saveAsTextFile('/user/mkoltsov/hw2/second_metric/{}'.format(date))

