#!/usr/bin/env bash

DATE=$1
shift

HADOOP_STREAM_COMMAND="yarn jar /opt/hadoop/hadoop-streaming.jar"
HDFS_COMMAND="hdfs dfs"
HDFS_DATA_DIR=/user/mkoltsov/hw1
STAT_OUT_DIR=${HDFS_DATA_DIR}/fourth_group/positive_now/${DATE}
HDFS_LOG_DIR=/user/bigdatashad/logs

function is_log_ready {
    local date=$1
    local date_next=`date -d "${date} +1 day" +%F`

    ${HDFS_COMMAND} -ls ${HDFS_LOG_DIR}/${date} >/dev/null 2>&1 && \
        ${HDFS_COMMAND} -ls ${HDFS_LOG_DIR}/${date_next} >/dev/null 2>&1 && echo "1"
}


function count_fourth_group {
    local date=$1
    if ! ${HDFS_COMMAND} -ls ${STAT_OUT_DIR}/_SUCCESS >/dev/null 2>&1; then

        # Delete directory if no _SUCCESS but dir exists. Otherwise, MR won't start.
        if ${HDFS_COMMAND} -ls ${STAT_OUT_DIR} >/dev/null 2>&1; then
             ${HDFS_COMMAND} -rm -r ${STAT_OUT_DIR}
        fi

    ${HADOOP_STREAM_COMMAND} \
        -files /home/mkoltsov/bigdata_course_assignments/hw01/fourth_group/../first_group/mapper.py,/home/mkoltsov/bigdata_course_assignments/hw01/fourth_group/../first_group/combiner.py,/home/mkoltsov/bigdata_course_assignments/hw01/fourth_group/reducer_plusminus_one.py \
        -mapper mapper.py \
        -combiner combiner.py \
        -reducer 'reducer_plusminus_one.py 1' \
        -input ${HDFS_LOG_DIR}/${DATE} \
        -output ${STAT_OUT_DIR}
    fi
}


# locking with pidfile
PIDFILE=/home/mkoltsov/bigdata_course_assignments/hw01/$(basename $0)".pid"
[ -e $PIDFILE ] && kill -0 `cat $PIDFILE` && echo "Already started, pid=`cat $PIDFILE`" && exit 0
echo $$ > $PIDFILE


if [ $(is_log_ready ${DATE}) ]; then
    echo "Starting ${DATE}"
else
    echo "Did nothing"
    rm $PIDFILE
    exit 1
fi

count_fourth_group ${DATE}

rm $PIDFILE
