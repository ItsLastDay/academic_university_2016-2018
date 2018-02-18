#!/usr/bin/env bash

DATE=$1
shift

HDFS_LOG_DIR=/user/bigdatashad/logs
HDFS_DATA_DIR=/user/mkoltsov/hw1
LOCAL_ROOT_DIR=/home/mkoltsov/bigdata_course_assignments/hw01
LOCAL_DATA_DIR=${LOCAL_ROOT_DIR}/statistics_data/first_group
SCRIPT_DIR=$(dirname $0)

COMMONS_DIR=${LOCAL_ROOT_DIR}/common

STAT_OUT_DIR=${HDFS_DATA_DIR}/first_group/${DATE}

HDFS_COMMAND="hdfs dfs"
HADOOP_STREAM_COMMAND="yarn jar /opt/hadoop/hadoop-streaming.jar"

# DATE is like 2017-10-04


function is_log_ready {
    local date=$1
    local date_next=`date -d "${date} +1 day" +%F`

    ${HDFS_COMMAND} -ls ${HDFS_LOG_DIR}/${date} >/dev/null 2>&1 && \
        ${HDFS_COMMAND} -ls ${HDFS_LOG_DIR}/${date_next} >/dev/null 2>&1 && echo "1"
}

function count_first_group {
    local date=$1
    if ! ${HDFS_COMMAND} -ls ${STAT_OUT_DIR}/_SUCCESS >/dev/null 2>&1; then

        # Delete directory if no _SUCCESS but dir exists. Otherwise, MR won't start.
        if ${HDFS_COMMAND} -ls ${STAT_OUT_DIR} >/dev/null 2>&1; then
             ${HDFS_COMMAND} -rm -r ${STAT_OUT_DIR}
        fi

        echo "First group ${date}"
        ${HADOOP_STREAM_COMMAND} \
            -files ${SCRIPT_DIR}/mapper.py,${SCRIPT_DIR}/combiner.py,${SCRIPT_DIR}/reducer.py \
            -mapper mapper.py \
            -reducer reducer.py \
            -combiner combiner.py \
            -input ${HDFS_LOG_DIR}/${date} \
            -output ${STAT_OUT_DIR}/ || exit 1
    fi
    if ! ls ${LOCAL_DATA_DIR}/${date}.txt >/dev/null 2>&1; then
        echo "First group stat ${date}"
        ${HDFS_COMMAND} -text ${STAT_OUT_DIR}/part* | \
                ${COMMONS_DIR}/int_sum_reducer.py > ${LOCAL_DATA_DIR}/${date}.txt.tmp && \
            mv ${LOCAL_DATA_DIR}/${date}.txt.tmp ${LOCAL_DATA_DIR}/${date}.txt
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
count_first_group ${DATE}

rm $PIDFILE
