import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import pyqtree

import sys
import os.path
import itertools

from decision_rule import *


RESULT_DIR = './exp_result'

EPS = 1e-9


def build_pareto_frontier(results):
    spatial_index = pyqtree.Index(bbox=[0, 0, 1.0, 1.0])
    for result in results:
        point = result.to_pareto_point()
        bbox = [*point, *point]
        spatial_index.insert(item=result, bbox=bbox)

    need_to_take = [False for result in results]
    for i in range(len(results)):
        point = results[i].to_pareto_point()
        interesting_box = [*point, 1.0, 1.0]
        points_inside = spatial_index.intersect(interesting_box)

        points_inside = [p.to_pareto_point() for p in points_inside]
        points_equal = spatial_index.intersect([*point, *point])

        is_valid = len(points_inside) == len(points_equal)
        
        need_to_take[i] = is_valid

    return [result for tk, result in zip(need_to_take, results) if tk]


colors = itertools.cycle(['r', 'g', 'b'])

def vis_pareto_frontier(results, feature_name):
    print(feature_name)
    print(len(results))
    print([r.to_pareto_point() for r in results])

    results = build_pareto_frontier(results)
    points = [r.to_pareto_point() for r in results]

    print(len(points))
    print(points)

    plt.scatter([p[0] for p in points], [p[1] for p in points], color=next(colors))


def experiment_pareto(data):
    feature_names = ['capital_run_length_average',
                     'capital_run_length_longest',
                     'capital_run_length_total']

    plt.figure()
    plt.title('Pareto frontier for last three features')
    plt.xlabel('recall')
    plt.ylabel('precision')
    plt.ylim([0.5, 1.0])
    plt.xlim([0.0, 1.0])

    for feature_name in feature_names:
        all_values = data[feature_name]
        getter = lambda obj: obj[feature_name]

        rules = []
        for value in all_values:
            rules.append(DecisionRule(value, getter, 0))
            rules.append(DecisionRule(value, getter, 1))

        CHECK_ITER = 0
        small_rules = [calc_result_for_rule_naive(rule, data) for rule in rules[:CHECK_ITER]]
        results = calc_results_for_rules_scanline(rules, data, getter)

        for i in range(CHECK_ITER):
            assert small_rules[i] == results[i]

        vis_pareto_frontier(results, feature_name)

    plt.legend(feature_names, bbox_to_anchor=(0.1,0.1))
    plt.show()


def read_data():
    return pd.read_csv('spambase_old.csv')


def main():
    data = read_data()

    experiment_pareto(data)

    return 0


if __name__ == '__main__':
    sys.exit(main())
