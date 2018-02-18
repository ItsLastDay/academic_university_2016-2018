#!/usr/bin/env python

import sys
from pyspark import SparkConf, SparkContext, StorageLevel
conf = SparkConf().setAppName("Graph Mini Test")
sc = SparkContext(conf=conf)


def parse_edge(s):
    u, f = s.split("\t")
    return (int(u), int(f))

def step(i):
    pv, pd, nv = i[0], i[1][0], i[1][1]
    return (nv, pd + 1)

def complete(i):
    v, od, nd = i[0], i[1][0], i[1][1]
    return (v, od if od is not None else nd)


n = 4  # number of partitions
graph_file = "/user/agorokhov/data/graph_mini"

graph_file = "hdfs:///data/twitter/twitter_rv.net"
n=400

edges = sc.textFile(graph_file).map(parse_edge).cache()
forward_edges = edges.map(lambda e: (e[1], e[0])).partitionBy(n).persist()

x = 3
d = 0

distances = sc.parallelize([(x, d)]).partitionBy(n)

while True:
    candidates = distances.join(forward_edges, n)\
                          .map(step)\
                          .reduceByKey(max)

    new_distances = distances.fullOuterJoin(candidates, n)\
                             .map(complete, True)\
                             .persist()

    count = new_distances.filter(lambda i: i[1] == d + 1)\
                         .count()

    if count > 0:
        d += 1
        distances = new_distances
        print ">>>>>>>>>> d = ", d, "count = ", count
        print "\n".join(["%s    %s" % x for x in distances.collect()])
    else:
        break

print ">>>>>>>>>> Distances:"
print "\n".join(["%s    %s" % x for x in distances.collect()])

