#!/usr/bin/env bash

DATE=$1
shift

HDFS_LOG_DIR=/user/bigdatashad/logs
HDFS_DATA_DIR=/user/mkoltsov/hw3
LOCAL_ROOT_DIR=/home/mkoltsov/bigdata_course_assignments/hw03
LOCAL_DATA_DIR=${LOCAL_ROOT_DIR}
SCRIPT_DIR=$(dirname $0)

STAT_OUT_DIR=${HDFS_DATA_DIR}/first_metric/${DATE}

HDFS_COMMAND="hdfs dfs"
HADOOP_STREAM_COMMAND="yarn jar /opt/hadoop/hadoop-streaming.jar"

# DATE is like 2017-10-04


function is_log_ready {
    local date=$1
    local date_next=`date -d "${date} +1 day" +%F`

    ${HDFS_COMMAND} -ls ${HDFS_LOG_DIR}/${date} >/dev/null 2>&1 && \
        ${HDFS_COMMAND} -ls ${HDFS_LOG_DIR}/${date_next} >/dev/null 2>&1 && echo "1"
}

function count_first_metric {
    local date=$1
    if ! ${HDFS_COMMAND} -ls ${STAT_OUT_DIR}/_SUCCESS >/dev/null 2>&1; then

        # Delete directory if no _SUCCESS but dir exists. Otherwise, MR won't start.
        if ${HDFS_COMMAND} -ls ${STAT_OUT_DIR} >/dev/null 2>&1; then
             ${HDFS_COMMAND} -rm -r ${STAT_OUT_DIR}
        fi

        echo "first metric ${date}"
        spark2-submit --master yarn --num-executors 30 ${SCRIPT_DIR}/first_metric.py ${date}
    fi
}


# locking with pidfile
PIDFILE=${LOCAL_DATA_DIR}/$(basename $0)".pid"
[ -e $PIDFILE ] && kill -0 `cat $PIDFILE` && echo "Already started, pid=`cat $PIDFILE`" && exit 0
echo $$ > $PIDFILE


if [ $(is_log_ready ${DATE}) ]; then
    echo "Starting ${DATE}"
else
    echo "Did nothing"
    rm $PIDFILE
    exit 1
fi

mkdir -p ${LOCAL_DATA_DIR}
count_first_metric ${DATE}

rm $PIDFILE
