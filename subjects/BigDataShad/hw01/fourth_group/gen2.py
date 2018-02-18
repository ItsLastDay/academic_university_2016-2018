for i in range(1, 13 + 1):
    print('d{}=$(date --date="$date -{} day" +%Y-%m-%d)'.format(i, i))



for i in range(1, 13 + 1):
    print('-input $HDFS_DATA_DIR/fourth_group/negative_now/$d{} \\'.format(i))

