#!/usr/bin/env bash

hadoop --config /home/agorokhov/conf.empty jar /opt/hadoop/hadoop-streaming.jar \
    -D mapred.output.key.comparator.class=org.apache.hadoop.mapred.lib.KeyFieldBasedComparator \
    -D mapred.text.key.comparator.options=-k1,2 \
    -D stream.num.map.output.key.fields=2 \
    -D mapred.text.key.partitioner.options=-k1,1 \
    -files ./mapper.py,./reducer.py \
    -mapper mapper.py \
    -reducer reducer.py \
    -input ../common/input \
    -output out 
