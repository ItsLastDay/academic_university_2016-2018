import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import pyqtree

import sys
import os.path
import itertools

from decision_rule import *


RESULT_DIR = './exp_result'


def build_roc_frontier(results):
    spatial_index = pyqtree.Index(bbox=[0, 0, 1.0, 1.0])
    for result in results:
        point = result.to_roc_curve_point()
        bbox = [*point, *point]
        spatial_index.insert(item=result, bbox=bbox)

    need_to_take = [False for result in results]
    for i in range(len(results)):
        point = results[i].to_roc_curve_point()
        x, y = point
        interesting_box = [0.0, y, x, 1.0]
        points_inside = spatial_index.intersect(interesting_box)

        points_inside = [p.to_roc_curve_point() for p in points_inside]
        points_equal = spatial_index.intersect([*point, *point])

        is_valid = len(points_inside) == len(points_equal)

        need_to_take[i] = is_valid

    return [result for tk, result in zip(need_to_take, results) if tk]


def calc_auc(roc_points):
    roc_points = roc_points
    roc_points.sort()

    answer = 0

    for i in range(len(roc_points) - 1):
        x1, y1 = roc_points[i]
        x2, y2 = roc_points[i + 1]

        answer += (y1 + y2) / 2 * (x2 - x1)

    return answer

def calc_all_aucs(data):
    feature_names = list(data.columns)

    for feature_name in feature_names:
        all_values = data[feature_name]
        getter = lambda obj: obj[feature_name]

        rules = []
        for value in all_values:
            rules.append(DecisionRule(value, getter, 0))
            rules.append(DecisionRule(value, getter, 1))

        rules = rules[:100]
        CHECK_ITER = 0
        small_rules = [calc_result_for_rule_naive(rule, data) for rule in rules[:CHECK_ITER]]
        results = calc_results_for_rules_scanline(rules, data, getter)

        for i in range(CHECK_ITER):
            assert small_rules[i] == results[i]

        roc_res = build_roc_frontier(results)
        print(feature_name, calc_auc([r.to_roc_curve_point() for r in roc_res]))


def read_data():
    return pd.read_csv('spambase_old.csv')


def print_best_aucs():
    inp = open('./results/auc_calculated.txt', 'r').read().strip().splitlines()

    vals = []
    for line in inp:
        name, val = line.split()
        val = float(val)
        vals.append((val, name))

    vals.sort()
    print('\n'.join(map(lambda x: ' '.join(map(str, x)), vals[-11:-1])))

def main():
    data = read_data()

    #calc_all_aucs(data)
    print_best_aucs()

    return 0


if __name__ == '__main__':
    sys.exit(main())
