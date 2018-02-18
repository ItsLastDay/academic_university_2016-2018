#!/usr/bin/env bash


HDFS_LOG_DIR=/user/bigdatashad/logs
HDFS_DATA_DIR=/user/bigdatashad/hw1_data
LOCAL_ROOT_DIR=/home/bigdatashad/hw1_data
LOCAL_DATA_DIR=${LOCAL_ROOT_DIR}/stat
CONFIG_DIR=${LOCAL_ROOT_DIR}/conf
SCRIPT_DIR=$(dirname $0)

HDFS_COMMAND="hdfs dfs"
HADOOP_STREAM_COMMAND="yarn jar /opt/hadoop/hadoop-streaming.jar"


function is_log_ready {
    local date=$1
    local date_next=`date -d "${date} +1 day" +%F`

    ${HDFS_COMMAND} -ls ${HDFS_LOG_DIR}/${date} >/dev/null 2>&1 && \
        ${HDFS_COMMAND} -ls ${HDFS_LOG_DIR}/${date_next} >/dev/null 2>&1 && echo "1"
}

function count_sessions {
    local date=$1
    if ! ${HDFS_COMMAND} -ls ${HDFS_DATA_DIR}/sessions/${date}/_SUCCESS >/dev/null 2>&1; then
        echo "Sessions ${date}"
        ${HADOOP_STREAM_COMMAND} \
            -files ${SCRIPT_DIR}/user_sessions.py \
            -D mapred.output.key.comparator.class=org.apache.hadoop.mapred.lib.KeyFieldBasedComparator \
            -D mapred.text.key.comparator.options=-k1,2 \
            -D stream.num.map.output.key.fields=2 \
            -D mapred.text.key.partitioner.options=-k1,1 \
            -D mapreduce.job.reduces=8 \
            -mapper cat \
            -reducer "./user_sessions.py" \
            -partitioner org.apache.hadoop.mapred.lib.KeyFieldBasedPartitioner \
            -input ${HDFS_DATA_DIR}/extended/${date} \
            -output ${HDFS_DATA_DIR}/sessions/${date}/ || exit 1
    fi
    if ! ls ${LOCAL_DATA_DIR}/sessions-${date}.txt >/dev/null 2>&1; then
        echo "Sessions stat ${date}"
        ${HDFS_COMMAND} -text ${HDFS_DATA_DIR}/sessions/${date}/part* | \
                ${SCRIPT_DIR}/user_sessions.py --sum > ${LOCAL_DATA_DIR}/sessions-${date}.txt.tmp && \
            mv ${LOCAL_DATA_DIR}/sessions-${date}.txt.tmp ${LOCAL_DATA_DIR}/sessions-${date}.txt
    fi
}

DATE=$1
shift

# locking with pidfile
PIDFILE=${LOCAL_DATA_DIR}/$(basename $0)".pid"
[ -e $PIDFILE ] && kill -0 `cat $PIDFILE` && echo "Already started, pid=`cat $PIDFILE`" && exit 0
echo $$ > $PIDFILE


if [ $(is_log_ready ${DATE}) ]; then
    echo "Starting ${DATE}"
else
    rm $PIDFILE
    exit 1
fi

mkdir -p ${LOCAL_DATA_DIR}
count_sessions ${DATE}

rm $PIDFILE
