#!/usr/bin/env python
import sys

THIRTY_MINUTES_IN_SECS = 30 * 60

def work():
    previous_time = -1
    previous_ip = None
    session_length = 0

    # Output number <session_length>.
    # I will sum session lengths and divide by 
    # number of lines (= number of sessions) later.

    for line in sys.stdin:
        ip, timestamp = line.strip().split('\t')
        diff = int(timestamp) - int(previous_time)

        if ip != previous_ip or diff > THIRTY_MINUTES_IN_SECS:
            if previous_ip is not None:
                print(session_length)

            session_length = 0
        else:
            session_length += diff

        previous_ip = ip
        previous_time = timestamp

    if previous_ip is not None:
        print(session_length)


if __name__ == '__main__':
    work()
