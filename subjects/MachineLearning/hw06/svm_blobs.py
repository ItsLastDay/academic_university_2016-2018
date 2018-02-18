import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

from sklearn.svm import SVC

def exp_svm_rbf(objects, labels):
    cls = SVC(kernel='rbf')
    cls.fit(objects, labels)
    print('finished fit')

    sup_vec_inner(objects, labels, cls, 'rbf')


def exp_svm_poly(objects, labels):
    for deg in range(2, 10 + 1):
        cls = SVC(kernel='poly', degree=deg)
        cls.fit(objects, labels)
        print('finished fit', deg)

        sup_vec_inner(objects, labels, cls, 'poly_{}'.format(deg))

def exp_svm_linear(objects, labels):
    cls = SVC(kernel='linear')
    cls.fit(objects, labels)
    print('finished fit')

    sup_vec_inner(objects, labels, cls, 'linear')


def sup_vec_inner(objects, labels, cls, filename):
    sup_vec = cls.support_vectors_

    plt.figure()

    for lbl in [0, 1]:
        mask_lbl = labels == lbl 
        points = objects[mask_lbl]
        plt.scatter(points['x'], points['y'], c='g' if lbl == 1 else 'b')
 

    plt.scatter(sup_vec[:, 0], sup_vec[:, 1], c='r')

    NUM_P = 100
    xx = np.linspace(-2, 5.5, NUM_P)
    yy = np.linspace(-2, 6.5, NUM_P)

    x, y = np.meshgrid(xx, yy, indexing='ij')

    z = np.zeros((NUM_P, NUM_P))
    for i in range(NUM_P):
        for j in range(NUM_P):
            x_p = x[i, j]
            y_p = y[i, j]

            z[i,j] = cls.predict([(x_p, y_p)]) == 1

    plt.contourf(x, y, z, alpha=0.2, color=('aqua', 'y'))

    plt.savefig('./results/{}.png'.format(filename))


def read_data():
    data = pd.read_csv('blobs2.csv')
    objects, labels = data.iloc[:, :-1], data.iloc[:, -1]
    return objects, labels 


def main():
    objects, label = read_data()

    exp_svm_linear(objects, label)
    exp_svm_rbf(objects, label)
    exp_svm_poly(objects, label)


if __name__ == '__main__':
    main()
