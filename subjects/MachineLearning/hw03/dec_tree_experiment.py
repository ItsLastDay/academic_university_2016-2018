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


RESULT_DIR = './exp_result'


def evaluate_fitted_tree(tree, test, max_depth=None):
    if max_depth is None:
        max_depth = tree.max_depth

    list_test = []
    for i in range(test.shape[0]):
        list_test.append(test.iloc[i])

    predicted_labels = tree.predict(list_test, max_depth)
    ground_truth = test['label']
    return accuracy_score(ground_truth, predicted_labels)


def experiment_decision_tree_top_features(train, test):
    features = """
    0.543511297926 word_freq_will
    0.570189576027 word_freq_all
    0.585970599846 word_freq_our
    0.61654273395 char_freq_$
    0.687580099548 word_freq_you
    0.722533615103 word_freq_your
    0.766583528792 capital_run_length_total
    0.76941439663 char_freq_!
    0.791709899909 capital_run_length_average
    0.804511584341 capital_run_length_longest""".strip().splitlines()
    features = list(map(lambda s: s.split()[1], features))
    print('Taking only top {} features (by AUC)'.format(len(features)))

    for criterion in ('gini', 'entropy'):
        print('\nCriterion: {}'.format(criterion))
        tree = train_tree(train, features, 10, criterion)

        for depth in range(2, 10 + 1):
            accuracy = evaluate_fitted_tree(tree, test, depth)
            print('depth={}: {}% accuracy'.format(depth, accuracy * 100))


def experiment_decision_tree(train, test):
    features = list(train.columns)
    print('Training on all {} features'.format(len(features)))

    for criterion in ('gini', 'entropy'):
        print('\nCriterion: {}'.format(criterion))
        tree = train_tree(train, features, 10, criterion)

        for depth in range(2, 10 + 1):
            accuracy = evaluate_fitted_tree(tree, test, depth)
            print('depth={}: {}% accuracy'.format(depth, accuracy * 100))


def experiment_random_forest(train, test):
    DEFAULT_NUM_TREES = 51
    DEFAULT_NUM_FEATURES = 10

    """
    res_by_depth = [0 for i in range(100)]
    print('Choosing best depth')
    for max_depth in range(2, 10 + 1): 
        tree = RandomForest(num_trees=DEFAULT_NUM_TREES, 
                max_depth=max_depth, num_features=DEFAULT_NUM_FEATURES)
        tree.fit(train)
        res_by_depth[max_depth] = evaluate_fitted_tree(tree, test)
        logging.error('depth={} => {}'.format(max_depth, res_by_depth[max_depth]))

    best_depth = 0
    for i in range(10):
        if res_by_depth[i] > res_by_depth[best_depth]:
            best_depth = i
    print('Best depth is {}'.format(best_depth))
    """

    best_depth = 8 # Already ran the above code, no need to do it again.

    print('Choosing best number of features')
    res_by_features = [0 for i in range(100)]
    for num_features in range(5, 15 + 1):
        tree = RandomForest(num_trees=DEFAULT_NUM_TREES,
                max_depth=best_depth, num_features=num_features)
        tree.fit(train)
        res_by_features[num_features] = evaluate_fitted_tree(tree, test)
        logging.error('features={} => {}'.format(num_features,
            res_by_features[num_features]))

    best_num_features = 0
    for i in range(100):
        if res_by_features[i] > res_by_features[best_num_features]:
            best_num_features = i
    print('Best number of features is {}'.format(best_num_features))

    print('Choosing best number of trees')
    res_by_num_trees = [0 for i in range(500)]
    for num_trees in range(51, 303, 10):
        tree = RandomForest(num_trees=num_trees, max_depth=best_depth, 
                num_features=best_num_features)
        tree.fit(train)
        res_by_num_trees[num_trees] = evaluate_fitted_tree(tree, test)
        logging.error('num_trees={} => {}'.format(num_trees, 
            res_by_num_trees[num_trees]))

    best_num_trees = 0
    for i in range(500):
        if res_by_num_trees[i] > res_by_num_trees[best_num_trees]:
            best_num_trees = i

    print('Best number of trees is {}'.format(best_num_trees))


def read_train_data():
    return pd.read_csv('spambase_old.csv')

def read_test_data():
    return pd.read_csv('./spambase_new.csv')


def setup_logging():
    log_config = {
            'version': 1,
            'disable_existing_loggers': True,
            'formatters': {
                'simple': {
                    'class': 'logging.Formatter',
                    'format': '%(process)-5s %(asctime)s %(message)s'
                }
            },
            'handlers': {
                'file_info': {
                    'class': 'logging.FileHandler',
                    'filename': 'dec_tree_exp.log',
                    'level': 'INFO',
                    'mode': 'w',
                    'formatter': 'simple',
                },
                'file_results': {
                    'class': 'logging.FileHandler',
                    'filename': 'dec_tree_results.log',
                    'level': 'ERROR',
                    'mode': 'w',
                    'formatter': 'simple',
                }
            },
            'root': {
                'level': 'DEBUG',
                'handlers': ['file_info', 'file_results']
            },
        }
    logging.config.dictConfig(log_config)


def main():
    random.seed(12345)
    setup_logging()

    train = read_train_data()
    test = read_test_data()

    experiment_decision_tree(train, test)
    #experiment_decision_tree_top_features(train, test)
    #experiment_random_forest(train, test)

if __name__ == '__main__':
    main()
