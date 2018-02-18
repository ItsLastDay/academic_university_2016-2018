#!/usr/bin/env bash

DATE=`date -d "yesterday 13:00" '+%Y-%m-%d'`
echo ${DATE}

#/home/mkoltsov/bigdata_course_assignments/hw03/first_metric/first_metric_runner.sh ${DATE} 2>&1
#/home/mkoltsov/bigdata_course_assignments/hw03/second_metric/second_metric_runner.sh ${DATE} 2>&1
/home/mkoltsov/bigdata_course_assignments/hw03/third_metric/third_metric_runner.sh ${DATE} 2>&1
/home/mkoltsov/bigdata_course_assignments/hw03/fourth_metric/fourth_metric_runner.sh ${DATE} 2>&1


#/home/mkoltsov/bigdata_course_assignments/hw02/first_metric/first_metric_runner.sh ${DATE} 2>&1
#/home/mkoltsov/bigdata_course_assignments/hw02/second_metric/second_metric_runner.sh ${DATE} 2>&1



#/home/mkoltsov/bigdata_course_assignments/hw01/first_group/first_group_runner.sh ${DATE} 2>&1
#/home/mkoltsov/bigdata_course_assignments/hw01/second_group/second_group_runner.sh ${DATE} 2>&1
#/home/mkoltsov/bigdata_course_assignments/hw01/third_group/third_group_runner.sh ${DATE} 2>&1

#/home/mkoltsov/bigdata_course_assignments/hw01/fourth_group/pos_runner.sh ${DATE} 2>&1
#/home/mkoltsov/bigdata_course_assignments/hw01/fourth_group/neg_runner.sh ${DATE} 2>&1
#/home/mkoltsov/bigdata_course_assignments/hw01/fourth_group/fourth_group_runner.sh ${DATE} 2>&1

