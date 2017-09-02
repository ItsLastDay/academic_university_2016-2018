import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

from sklearn.svm import SVC
from sklearn.model_selection import StratifiedShuffleSplit, ParameterGrid
from sklearn.metrics import accuracy_score


fl_train_acc = open('./results/train_acc.txt', 'a')
fl_test_acc = open('./results/test_acc.txt', 'a')
fl_diff_acc = open('./results/diff_acc.txt', 'a')


def exp_linear(objects, labels):
    print('linear')
    objects = objects.as_matrix()
    splitter = StratifiedShuffleSplit(n_splits=5, test_size=0.1)
    for train_index, test_index in splitter.split(objects, labels):
        x_train, x_test = objects[train_index], objects[test_index]
        y_train, y_test = labels[train_index], labels[test_index]

        params_linear = dict()
        params_linear['C'] = [0.01, 0.1, 1, 10, 100, 1000]
        params_linear['shrinking'] = [True, False]
        params_linear['tol'] = [1e-3, 1e-2]

        for param in ParameterGrid(params_linear):
            cls = SVC(kernel='linear', **param)
            cls.fit(x_train, y_train)

            n_vec = len(cls.support_vectors_)

            train_acc = accuracy_score(y_train, cls.predict(x_train))
            test_acc = accuracy_score(y_test, cls.predict(x_test))
            difference = train_acc - test_acc

            fl_train_acc.write('{} {}\n'.format(n_vec, train_acc))
            fl_test_acc.write('{} {}\n'.format(n_vec, test_acc))
            fl_diff_acc.write('{} {}\n'.format(n_vec, difference))


def exp_rbf(objects, labels):
    print('rbf')
    objects = objects.as_matrix()
    splitter = StratifiedShuffleSplit(n_splits=5, test_size=0.1)
    for train_index, test_index in splitter.split(objects, labels):
        x_train, x_test = objects[train_index], objects[test_index]
        y_train, y_test = labels[train_index], labels[test_index]
        params_rbf = dict()
        params_rbf['C'] = [0.01, 0.1, 1, 10, 100, 1000]
        params_rbf['shrinking'] = [True, False]
        params_rbf['coef0'] = [0, 1, 10, 42]
        params_rbf['gamma'] = ['auto', 1e-4, 1e-3, 1e-2]
        params_rbf['tol'] = [1e-5, 1e-4, 1e-3, 1e-2]

        for param in ParameterGrid(params_rbf):
            cls = SVC(kernel='rbf', **param)
            cls.fit(x_train, y_train)

            n_vec = len(cls.support_vectors_)

            train_acc = accuracy_score(y_train, cls.predict(x_train))
            test_acc = accuracy_score(y_test, cls.predict(x_test))
            difference = train_acc - test_acc

            fl_train_acc.write('{} {}\n'.format(n_vec, train_acc))
            fl_test_acc.write('{} {}\n'.format(n_vec, test_acc))
            fl_diff_acc.write('{} {}\n'.format(n_vec, difference))


def exp_poly(objects, labels):
    print('poly')
    objects = objects.as_matrix()
    splitter = StratifiedShuffleSplit(n_splits=15, test_size=0.1 * 0.3, train_size=0.9 * 0.3)
    for train_index, test_index in splitter.split(objects, labels):
        x_train, x_test = objects[train_index], objects[test_index]
        y_train, y_test = labels[train_index], labels[test_index]
        params_poly = dict()
        params_poly['C'] = [0.01, 0.1, 1, 10, 100, 1000]
        params_poly['shrinking'] = [True, False]
        params_poly['coef0'] = [0, 1, 10, 42]
        params_poly['gamma'] = ['auto', 1e-4, 1e-3, 1e-2]
        params_poly['degree'] = list(range(2, 7 + 1))

        for param in ParameterGrid(params_poly):
            cls = SVC(kernel='poly', **param)
            print(param)
            cls.fit(x_train, y_train)

            n_vec = len(cls.support_vectors_)

            train_acc = accuracy_score(y_train, cls.predict(x_train))
            test_acc = accuracy_score(y_test, cls.predict(x_test))
            difference = train_acc - test_acc

            fl_train_acc.write('{} {}\n'.format(n_vec, train_acc))
            fl_train_acc.flush()
            fl_test_acc.write('{} {}\n'.format(n_vec, test_acc))
            fl_test_acc.flush()
            fl_diff_acc.write('{} {}\n'.format(n_vec, difference))
            fl_diff_acc.flush()


def print_results():
    for fl in ('./results/train_acc.txt',
            './results/diff_acc.txt',
            './results/test_acc.txt'):
        with open(fl, 'r') as inp:
            lines = inp.read().strip().splitlines()
            lines = lines[:4000] + lines[16000:]

            plt.figure()

            plt.title(fl.replace('acc', 'err'))
            plt.scatter([int(line.split()[0]) for line in lines],
                    [(1 if 'diff' not in fl else 0) - float(line.split()[1]) for line in lines])

            plt.savefig(fl.replace('acc', 'err') + '.png')


def read_data():
    data = pd.read_csv('cancer.csv')
    objects, labels = data.iloc[:, 1:], data.iloc[:, 0]
    return objects, labels 


def main():
    objects, labels = read_data()

    #exp_linear(objects, labels)
    #exp_rbf(objects, labels)
    #exp_poly(objects, labels)

    print_results()

if __name__ == '__main__':
    main()
