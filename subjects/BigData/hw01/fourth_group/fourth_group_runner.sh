#!/usr/bin/env bash

DATE=$1
shift

HDFS_LOG_DIR=/user/bigdatashad/logs
HDFS_DATA_DIR=/user/mkoltsov/hw1
LOCAL_ROOT_DIR=/home/mkoltsov/bigdata_course_assignments/hw01
LOCAL_DATA_DIR=${LOCAL_ROOT_DIR}/statistics_data/fourth_group
SCRIPT_DIR=$(dirname $0)

COMMONS_DIR=${LOCAL_ROOT_DIR}/common

STAT_OUT_DIR=${HDFS_DATA_DIR}/fourth_group/${DATE}

HDFS_COMMAND="hdfs dfs"
HADOOP_STREAM_COMMAND="yarn jar /opt/hadoop/hadoop-streaming.jar"

# DATE is like 2017-10-04


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

        d1=$(date --date="$date -1 day" +%Y-%m-%d)
        d2=$(date --date="$date -2 day" +%Y-%m-%d)
        d3=$(date --date="$date -3 day" +%Y-%m-%d)
        d4=$(date --date="$date -4 day" +%Y-%m-%d)
        d5=$(date --date="$date -5 day" +%Y-%m-%d)
        d6=$(date --date="$date -6 day" +%Y-%m-%d)
        d7=$(date --date="$date -7 day" +%Y-%m-%d)
        d8=$(date --date="$date -8 day" +%Y-%m-%d)
        d9=$(date --date="$date -9 day" +%Y-%m-%d)
        d10=$(date --date="$date -10 day" +%Y-%m-%d)
        d11=$(date --date="$date -11 day" +%Y-%m-%d)
        d12=$(date --date="$date -12 day" +%Y-%m-%d)
        d13=$(date --date="$date -13 day" +%Y-%m-%d)

        echo "Fourth group ${date}"
        ${HADOOP_STREAM_COMMAND} \
            -files ${SCRIPT_DIR}/merger.py  \
            -mapper cat \
            -reducer merger.py \
            -input $HDFS_DATA_DIR/fourth_group/negative_now/$d1 \
            -input $HDFS_DATA_DIR/fourth_group/negative_now/$d2 \
            -input $HDFS_DATA_DIR/fourth_group/negative_now/$d3 \
            -input $HDFS_DATA_DIR/fourth_group/negative_now/$d4 \
            -input $HDFS_DATA_DIR/fourth_group/negative_now/$d5 \
            -input $HDFS_DATA_DIR/fourth_group/negative_now/$d6 \
            -input $HDFS_DATA_DIR/fourth_group/negative_now/$d7 \
            -input $HDFS_DATA_DIR/fourth_group/negative_now/$d8 \
            -input $HDFS_DATA_DIR/fourth_group/negative_now/$d9 \
            -input $HDFS_DATA_DIR/fourth_group/negative_now/$d10 \
            -input $HDFS_DATA_DIR/fourth_group/negative_now/$d11 \
            -input $HDFS_DATA_DIR/fourth_group/negative_now/$d12 \
            -input $HDFS_DATA_DIR/fourth_group/negative_now/$d13 \
            -input $HDFS_DATA_DIR/fourth_group/positive_now/${date} \
            -output ${STAT_OUT_DIR}/ || exit 1
    fi
    if ! ls ${LOCAL_DATA_DIR}/${date}.txt >/dev/null 2>&1; then
        echo "Fourth group stat ${date}"
        ${HDFS_COMMAND} -text ${STAT_OUT_DIR}/part* \
                | ${COMMONS_DIR}/int_sum_reducer.py \
                > ${LOCAL_DATA_DIR}/${date}.txt.tmp && \
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
count_fourth_group ${DATE}

rm $PIDFILE
