#!/usr/bin/env python

import argparse
import datetime
import getpass
import hashlib
import random
import struct
import re

import happybase


from flask import Flask, request, abort, jsonify

METRICS_DIR = '/home/mkoltsov/bigdata_course_assignments/hw01/statistics_data/'
METRICS_DIR_2 = '/home/mkoltsov/bigdata_course_assignments/hw02/statistics_data/'

app = Flask(__name__)
app.secret_key = "ml;jsdfhweoirtugsdnbhqwepogdksdf[pro#42123$5g"

# (u'http://bing.com/', 5784)
pair_str_re = re.compile("^\(u'(.*?)', (\d+)\)$")

HOSTS = ["hadoop2-%02d.yandex.ru" % i for i in xrange(11, 14)]



def iterate_between_dates(start_date, end_date):
    span = end_date - start_date
    for i in xrange(span.days + 1):
        yield start_date + datetime.timedelta(days=i)


@app.route("/")
def index():
    return "OK!"

def _get_metric(d, name, date, fnc):
    fl_name = '{}/{}/{}.txt'.format(d, name, date)
    try:
        with open(fl_name, 'r') as out:
            return fnc(out.read().strip())
    except:
        return None


def get_metric(name, date, fnc):
    return _get_metric(METRICS_DIR, name, date, fnc)

def get_metric_2(name, date, fnc):
    return _get_metric(METRICS_DIR_2, name, date, fnc)

def make_dict(s):
    lines = s.strip().split('\n')
    d = dict()

    for line in lines:
        a, b = line.strip().split('\t')
        d[a] = int(b)

    return d

def pairs_to_dict(s):
    d = dict()
    for line in s.strip().split('\n'):
        match = re.match(pair_str_re, line)

        if match is None:
            print(line, 'not matched')

        d[match.group(1)] = int(match.group(2))

    return d


@app.route("/api/hw1")
def api_hw1():
    start_date = request.args.get("start_date", None)
    end_date = request.args.get("end_date", None)
    if start_date is None or end_date is None:
        abort(400)
    start_date = datetime.datetime(*map(int, start_date.split("-")))
    end_date = datetime.datetime(*map(int, end_date.split("-")))

    result = {}
    for date in iterate_between_dates(start_date, end_date):
        dd = date.strftime("%Y-%m-%d")
        total_users = get_metric('first_group', dd, int)
        average_session_time = get_metric('second_group', dd, float)
        users_by_country = get_metric('third_group', dd, make_dict)
        new_users = get_metric('fourth_group', dd, int)
        profile_liked_three_days = get_metric_2('second_metric', dd, int)
        session_referers = get_metric_2('first_metric', dd, pairs_to_dict)
        result[dd] = dict()

        if total_users is not None:
            result[dd]["total_users"] = total_users
        if average_session_time is not None:
            result[dd]["average_session_time"] = average_session_time
        if users_by_country is not None:
            result[dd]["users_by_country"] = users_by_country
        if new_users is not None:
            result[dd]["new_users"] = new_users
        if profile_liked_three_days is not None:
            result[dd]["profile_liked_three_days"] = profile_liked_three_days
        if session_referers is not None:
            result[dd]["session_referers"] = session_referers

    return jsonify(result)


def bytes_to_number(b):
    if isinstance(b, int):
        return b

    ss = list(map(lambda x: ord(x), b))
    res = 0
    for x in ss:
        res = res * 10 + x

    return res

def get_numbers(table, dd, prof_id):
    try:
        result = table.row(bytes('{}_{}'.format(prof_id, dd)))
    except Exception as s:
        print(s)
        return None

    tt = []
    for i in range(24):
        key = 'h:{:02d}'.format(i)
        tt.append(bytes_to_number(result.get(key, 0)))

    return tt
    

def get_prof_hits(dd, prof_id):
    conn = happybase.Connection(random.choice(HOSTS))
    conn.open()
    table_metric_1 = happybase.Table('bigdatashad_mkoltsov_firstmetric', conn)
    return get_numbers(table_metric_1, dd, prof_id)


def get_prof_users(dd, prof_id):
    conn = happybase.Connection(random.choice(HOSTS))
    conn.open()
    table_metric_2 = happybase.Table('bigdatashad_mkoltsov_secondmetric', conn)
    return get_numbers(table_metric_2, dd, prof_id)


def get_prof_liked(dd, prof_id):
    conn = happybase.Connection(random.choice(HOSTS))
    conn.open()
    table_metric_4 = happybase.Table('bigdatashad_mkoltsov_fourthmetric', conn)
    try:
        result = table_metric_4.cells('{}_{}'.format(prof_id, dd), 
                't:1', 
                include_timestamp=True)
    except Exception as s:
        print(s)
        return None

    return list(map(lambda x: x[0], sorted(result, key=lambda x: -x[1])))


def get_user_vis_profiles(dd, user_ip):
    conn = happybase.Connection(random.choice(HOSTS))
    conn.open()
    table_metric_3 = happybase.Table('bigdatashad_mkoltsov_thirdmetric', conn)

    result = list(table_metric_3.scan(row_prefix='{}+{}'.format(dd, user_ip)))

    print(result)


    good_results = list(map(lambda x: ('id' + x[0][x[0].rindex('+') + 1:], 
        bytes_to_number(x[1]['c:1'])), result))

    return list(map(lambda x: x[0],
        sorted(good_results, key=lambda x: (-x[1], x[0]))))


@app.route("/api/hw3")
def api_hw3():
    start_date = request.args.get("start_date", None)
    end_date = request.args.get("end_date", None)
    if start_date is None or end_date is None:
        abort(400)
    profile_id = request.args.get("profile_id", None).strip('id')
    user_ip = request.args.get("user_ip", None)
    

    start_date = datetime.datetime(*map(int, start_date.split("-")))
    end_date = datetime.datetime(*map(int, end_date.split("-")))

    result = {}
    for date in iterate_between_dates(start_date, end_date):
        dd = date.strftime("%Y-%m-%d")
        result[dd] = dict()

        prof_hits = get_prof_hits(dd, profile_id)
        prof_users = get_prof_users(dd, profile_id)
        user_vis = get_user_vis_profiles(dd, user_ip)
        prof_liked = get_prof_liked(dd, profile_id)

        if prof_hits is not None:
            result[dd]['profile_hits'] = prof_hits
        if prof_users is not None:
            result[dd]['profile_users'] = prof_users
        if user_vis is not None:
            result[dd]['user_most_visited_profiles'] = user_vis
        if prof_liked is not None:
            result[dd]['profile_last_three_liked_users'] = prof_liked

    return jsonify(result)


def login_to_port(login):
    """
    We believe this method works as a perfect hash function
    for all course participants. :)
    """
    hasher = hashlib.new("sha1")
    hasher.update(login)
    values = struct.unpack("IIIII", hasher.digest())
    folder = lambda a, x: a ^ x + 0x9e3779b9 + (a << 6) + (a >> 2)
    return 10000 + reduce(folder, values) % 20000


def main():
    parser = argparse.ArgumentParser(description="Koltsov hw #1")
    parser.add_argument("--host", type=str, default="127.0.0.1")
    parser.add_argument("--port", type=int, default=login_to_port(getpass.getuser()))
    parser.add_argument("--debug", action="store_true", dest="debug")
    parser.add_argument("--no-debug", action="store_false", dest="debug")
    parser.set_defaults(debug=False)

    args = parser.parse_args()
    app.run(host=args.host, port=args.port, debug=args.debug)

if __name__ == "__main__":
    main()
