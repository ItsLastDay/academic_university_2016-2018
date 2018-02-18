#!/bin/bash

hadoop --config /home/agorokhov/conf.empty jar /opt/hadoop/hadoop-streaming.jar \
 	-files mapper.py,reducer.py,combiner.py \
	-input inp/ -output out/ \
	-mapper mapper.py  \
	-reducer reducer.py \
        -combiner combiner.py

