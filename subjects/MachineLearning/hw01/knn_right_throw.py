import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

from scipy.spatial.distance import euclidean
from sklearn.neighbors import KNeighborsClassifier
from sklearn.neighbors import KDTree
from sklearn.model_selection import LeaveOneOut

import sys
import abc
import math
import time
import os.path
import random
from multiprocessing import Pool


RESULT_DIR = './exp_result'


class KNN(metaclass=abc.ABCMeta):

    @abc.abstractmethod
    def classify(self, obj, throw_closest=False):
        pass


class KNN_Euclidean(KNN):

    def __init__(self, objects, labels, k):
        self.objects = objects
        self.labels = labels
        self.n = objects.shape[0]
        self.k = k
    
    def classify(self, obj, throw_closest=False):
        obj_repeated = np.array([obj,] * self.n)
        obj_repeated = np.repeat(obj, self.n).values.reshape((len(obj), self.n)).T
        obj_repeated -= self.objects
        norms = np.linalg.norm(obj_repeated, axis=1)
        indices = np.argpartition(norms, self.k + throw_closest)

        norms_indices = [(norms[i], i) for i in indices[:self.k + throw_closest]]
        norms_indices.sort()
        match_indices = [p[1] for p in norms_indices[int(throw_closest):]]

        votes_for_first_class = sum(self.labels[match_indices])
        
        # The majority class is `0`. I guess that this is `no-spam` class.
        if votes_for_first_class * 2 > self.k:
            return 1
        return 0


class KNN_Euclidean_KD(KNN):

    def __init__(self, objects, labels, k):
        self.objects_tree = KDTree(objects)
        self.labels = labels
        self.k = k

    
    def classify(self, obj, throw_closest=False):
        neighbours_idx = self.objects_tree.query([obj], self.k, sort_results=True, 
                return_distance=False)[0][int(throw_closest):]
        
        votes = self.labels[neighbours_idx]
        votes_for_first_class = sum(votes)

        if votes_for_first_class * 2 > self.k:
            return 1
        return 0


class KNN_Radius(KNN):

    def __init__(self, objects, labels, radius):
        self.objects = objects
        self.labels = labels
        self.n = objects.shape[0]
        self.radius = radius

    def classify(self, obj, throw_closest=False):
        obj_repeated = np.array([obj,] * self.n)
        obj_repeated = np.repeat(obj, self.n).values.reshape((len(obj), self.n)).T
        obj_repeated -= self.objects
        norms = np.linalg.norm(obj_repeated, axis=1)

        mask = norms <= self.radius
        if throw_closest:
            mask &= norms > 0


        votes = self.labels[mask]
        votes_for_first_class = sum(votes)
        
        if votes_for_first_class * 2 > len(votes):
            return 1
        return 0



def classify_one_obj(idx, row, classifier, right_label):
    guess = classifier.classify(row, True)
    return guess == right_label


def leave_one_out_unparallel(classifier_cls, objects, labels, **kwargs):
    classifier = classifier_cls(objects, labels, **kwargs)
    num_objects = objects.shape[0]

    args = [(i, objects.iloc[i], classifier, labels.iloc[i]) for i in range(num_objects)]
    right_guesses = 0

    for i in range(num_objects):
        obj = objects.iloc[i]
        ground_truth = labels.iloc[i]
        predicted = classifier.classify(obj, True)

        if i % 100 == 0:
            print('iteration i=',i)
            sys.stdout.flush()

        right_guesses += predicted == ground_truth

    return right_guesses / float(num_objects)


def experiment_loo_unparallel():
    classifier_cls = KNN_Euclidean

    print('Non-normalized data:')
    objects, labels = read_data()
    for k in [1, 3]:
        print('k=', k)
        loo_val = leave_one_out_unparallel(classifier_cls, objects, labels, k=k)
        print(loo_val)

    print('Normalized data')
    objects, labels = read_normalized_data()
    for k in [1, 3]:
        print('k=', k)
        loo_val = leave_one_out_unparallel(classifier_cls, objects, labels, k=k)
        print(loo_val)


def leave_one_out(classifier_cls, objects, labels, **kwargs):
    classifier = classifier_cls(objects, labels, **kwargs)
    num_objects = objects.shape[0]

    args = [(i, objects.iloc[i], classifier, labels.iloc[i]) for i in range(num_objects)]
    right_guesses = 0
    with Pool() as p:
        right_guesses = sum(p.starmap(classify_one_obj, args))

    return right_guesses / float(num_objects)


def read_data():
    data = pd.read_csv('spambase.csv')
    objects, labels = data.iloc[:, :-1], data.iloc[:, -1]
    return objects, labels


def read_normalized_data():
    objects, labels = read_data()
    
    cols = objects.shape[1]
    rows = objects.shape[0]

    max_by_col = [0.001 for i in range(cols)]

    for i in range(rows):
        row = objects.iloc[i]
        for j in range(cols):
            val = row.iloc[j]

            max_by_col[j] = max(max_by_col[j], abs(val))

    objects /= np.array(max_by_col)

    return objects, labels


def simple_test(objects, labels):
    NUM = 3345
    row = objects.iloc[NUM]
    classifier = KNN_Euclidean(objects, labels, 3)
    print(classifier.classify(row, True))
    print(labels.iloc[NUM])
    print(leave_one_out(KNN_Euclidean, objects, labels, k=3))


def generic_euclidean_loo(classifier_cls, objects, labels, filename):
    k_values = list(range(1, 10 + 1))
    loo_values = []
    times = []
    for k in k_values:
        print('Loo: k = ', k)
        time_before = time.time()
        loo_val = leave_one_out(classifier_cls, objects, labels, k=k)
        print('loo value: ', loo_val)
        time_after = time.time()

        time_delta = time_after - time_before
        times.append(time_delta)

        loo_values.append(loo_val)

    dct = {'K': k_values, 'LOO': loo_values, 'Time': times}
    frame = pd.DataFrame(dct)
    frame.to_csv(os.path.join(RESULT_DIR, filename))


def generic_radius_loo(classifier_cls, objects, labels, filename, maximal_radius):
    number_of_computations = 0

    radiuses = []
    loo_vals = []
    times = []

    def f(radius, saved=dict()):
        nonlocal number_of_computations
        # Cache already computed results.
        key = radius * 10 ** 100
        if key not in saved:
            time_before = time.time()
            saved[key] = leave_one_out(classifier_cls, objects, labels, radius=radius)
            time_after = time.time()

            times.append(time_after - time_before)
            radiuses.append(radius)
            loo_vals.append(saved[key])
            number_of_computations += 1

        return saved[key]

    le = 0
    rg = maximal_radius
    # Assume that LOO has unique maximum.
    # Use golden ratio search http://www.essie.ufl.edu/~kgurl/Classes/Lect3421/NM6_optim_s02.pdf
    # for lesser number of LOO calculations (which are *painfully* slow).
    golden_ratio = (1 + math.sqrt(5)) / 2
    NUM_ITER = 10

    for it in range(NUM_ITER):
        print('Iteration {}/{}:'.format(it + 1, NUM_ITER), 'left', le, 'right', rg)

        len_seg = (rg - le)
        len_left = len_seg / (1 + golden_ratio)
        len_right = len_left * golden_ratio

        mid1 = le + len_left
        mid2 = le + len_right

        loo_mid1 = f(mid1)
        loo_mid2 = f(mid2)

        print('LOO values:', loo_mid1, loo_mid2)

        if loo_mid1 < loo_mid2:
            le = mid1
        else:
            rg = mid2

    # Total number of computations is 27 vs 40 in regular ternary search (if we always choose rg = mid2).
    print('Total number of LOO computations:', number_of_computations)

    dct = {'Radius': radiuses, 'LOO': loo_vals, 'Time': times}
    frame = pd.DataFrame(dct)
    frame.to_csv(os.path.join(RESULT_DIR, filename))


def experiment_loo_euclidean():
    objects, labels = read_data()
    generic_euclidean_loo(KNN_Euclidean, objects, labels, 'loo_euclidean.csv')


def experiment_loo_radius():
    objects, labels = read_data()
    generic_radius_loo(KNN_Radius, objects, labels, 'loo_radius.csv', 100.0)


def experiment_loo_euclidean_normalized():
    objects, labels = read_normalized_data()
    generic_euclidean_loo(KNN_Euclidean, objects, labels, 'loo_euclidean_normalized.csv')


def experiment_loo_radius_normalized():
    objects, labels = read_normalized_data()
    generic_radius_loo(KNN_Radius, objects, labels, 'loo_radius_normalized.csv', 1.0)


def classify_one_obj_sklearn (k, objects, labels, train_index, test_index):
    x_train, x_test = objects[train_index], objects[test_index]
    y_train, y_test = labels[train_index], labels[test_index]
    classifier = KNeighborsClassifier(k)
    classifier.fit(x_train, y_train)
    return classifier.predict(x_test)[0] == y_test[0]


def generic_test_sklearn(objects, labels, filename):
    # http://scikit-learn.org/stable/modules/generated/sklearn.model_selection.LeaveOneOut.html

    loo = LeaveOneOut()

    objects = np.array(objects)
    labels = np.array(labels)

    k_values = list(range(1, 10 + 1))
    loo_values = []
    times = []
    for k in k_values:
        print('Loo: k = ', k)
        classifier = KNeighborsClassifier(k, n_jobs=-1)
        right_guesses = 0

        time_before = time.time()

        args = []
        for train_index, test_index in loo.split(objects):
            args.append((k, objects, labels, train_index, test_index))

        print('Computed args')

        right_guesses = 0
        with Pool() as p:
            right_guesses = sum(p.starmap(classify_one_obj_sklearn, args))

        loo_val = right_guesses / float(labels.shape[0])
        print('Loo value: ', loo_val)

        time_after = time.time()
        times.append(time_after - time_before)
        loo_values.append(loo_val)

    dct = {'K': k_values, 'LOO': loo_values, 'Time': times}
    frame = pd.DataFrame(dct)
    frame.to_csv(os.path.join(RESULT_DIR, filename))
    

def experiment_loo_sklearn():
    objects, labels = read_data()
    generic_test_sklearn(objects, labels, 'loo_euclidean_sklearn.csv')

    objects, labels = read_normalized_data()
    generic_test_sklearn(objects, labels, 'loo_euclidean_sklearn_normalized.csv')


def experiment_time_kdtree():
    objects, labels = read_data()
    generic_euclidean_loo(KNN_Euclidean_KD, objects, labels, 'loo_euclidean_kdtree.csv')

    objects, labels = read_normalized_data()
    generic_euclidean_loo(KNN_Euclidean_KD, objects, labels, 'loo_euclidean_kdtree_normalized.csv')


def write_result_euclidean():
    fname_prefixes = ['loo_euclidean', 'loo_euclidean_kdtree', 'loo_euclidean_sklearn']

    for suffix in ['.csv', '_normalized.csv']:
        fnames = [os.path.join(RESULT_DIR, fname_prefix + suffix) for fname_prefix in fname_prefixes]
        csv_files = [pd.read_csv(fname) for fname in fnames]

        loos = [csv['LOO'] for csv in csv_files]
        loos = np.matrix(loos).T

        df = pd.DataFrame(loos, index=csv_files[0]['K'],
                columns=['my_knn', 'kdtree_knn', 'sklearn_knn'])

        df.plot()
        plt.title('KNN on {} data'.format('non-normalized' if suffix == '.csv' else 'normalized'))
        plt.xlabel('K')
        plt.ylabel('LOO')
        plt.ylim([0.5, 1.0])
        plt.savefig(os.path.join(RESULT_DIR, 'knn_euclidean{}.png'.format('' if suffix == '.csv' else '_normalized')))

    for suffix in ['.csv']:
        fnames = [os.path.join(RESULT_DIR, fname_prefix + suffix) for fname_prefix in fname_prefixes]
        csv_files = [pd.read_csv(fname) for fname in fnames]

        loos = [csv['Time'][:-1] for csv in csv_files]
        loos = np.matrix(loos).T

        df = pd.DataFrame(loos, index=csv_files[0]['K'][:-1],
                columns=['my_knn', 'kdtree_knn', 'sklearn_knn'])

        df.plot()
        plt.title('Time of one LOO iteration')
        plt.xlabel('K')
        plt.ylabel('Time (seconds)')
        plt.savefig(os.path.join(RESULT_DIR, 'knn_time.png'))


def write_result_radius():
    fname_prefixes = ['loo_radius']

    for suffix in ['.csv', '_normalized.csv']:
        fnames = [os.path.join(RESULT_DIR, fname_prefix + suffix) for fname_prefix in fname_prefixes]
        csv_files = [pd.read_csv(fname) for fname in fnames]

        frame = csv_files[0]
        frame.sort_values('Radius', inplace=True)

        loos = [frame['LOO']]
        loos = np.matrix(loos).T

        df = pd.DataFrame(loos, index=frame['Radius'], columns=['my_knn'])

        df.plot()
        plt.title('KNN on {} data'.format('non-normalized' if suffix == '.csv' else 'normalized'))
        plt.xlabel('Radius')
        plt.ylabel('LOO')
        plt.savefig(os.path.join(RESULT_DIR, 'knn_radius{}.png'.format('' if suffix == '.csv' else '_normalized')))


def compute_graphic_results():
    write_result_euclidean()
    write_result_radius()


def main():
    #simple_test(*read_data())
    experiment_loo_euclidean()
    #experiment_loo_radius()

    experiment_loo_euclidean_normalized()
    #experiment_loo_radius_normalized()

    #experiment_loo_sklearn()
    #experiment_time_kdtree()

    compute_graphic_results()


if __name__ == '__main__':
    main()
