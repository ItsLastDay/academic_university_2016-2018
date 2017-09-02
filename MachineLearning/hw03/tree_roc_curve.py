import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.metrics import accuracy_score

import sys
import os.path
import itertools
import logging.config
import random

from decision_rule import *
from decision_tree import *
from auc import vis_roc_frontier


RESULT_DIR = './exp_result'


def experiment_tree_roc_curve(train, test):
    criterion = 'gini'
    tree = train_tree(train, train.columns, 10, criterion)

    list_test = []
    for i in range(test.shape[0]):
        list_test.append(test.iloc[i])

    plt.figure()
    plt.title('ROC curve for probabilities by tree depth, criterion {}'.format(criterion))
    plt.xlabel('false positive rate')
    plt.ylabel('true positive rate')
    plt.ylim([0.0, 1.0])
    plt.xlim([0.0, 1.0])
    for max_depth in range(2, 10 + 1):
        probs = tree.predict_proba(list_test, max_depth)

        new_test = test.assign(prob=probs)

        all_rules = []
        for prob in set(probs):
            all_rules.append(DecisionRule(prob, None, 1, 'prob'))

        results = calc_results_for_rules_scanline(all_rules, new_test,
                lambda x: x['prob'])

        vis_roc_frontier(results, str(max_depth))

    plt.legend(list(map(str, range(2, 10 + 1))), bbox_to_anchor=(0.9, 0.5))
    plt.show()


def read_train_data():
    return pd.read_csv('spambase_old.csv')

def read_test_data():
    return pd.read_csv('./spambase_new.csv')


def main():
    train = read_train_data()
    test = read_test_data()

    experiment_tree_roc_curve(train, test)

if __name__ == '__main__':
    main()
