import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


def exp_perceptron_highdim(data_old, num_add):
    data = data_old.as_matrix()
    labels = data[:, -1]
    labels = np.array([-1 if x == 0 else 1 for x in list(labels)])
    data = data[:, :-1]

    xs = data[:, 0]
    ys = data[:, 1]

    # (x, y) pairs
    degs = [(0, 0), (1, 0), (0, 1)]
    for deg_x in range(num_add + 1):
        for deg_y in range(num_add + 1):
            if deg_x + deg_y == num_add:
                res = (xs ** deg_x * ys ** deg_y).reshape((data.shape[0], 1))
                data = np.concatenate((data, res), axis=1)
                degs.append((deg_x, deg_y))

    ones = np.ones((data.shape[0], 1))
    data = np.concatenate((ones, data), axis=1)
    
    w = np.random.rand(data.shape[1])

    best_w = w
    best_cnt = 0

    for it in range(400):
        cr = np.dot(data, w)
        signs = np.sign(cr)
        
        gr = signs > 0
        true_gr = labels == 1
        invalid_gr = gr ^ true_gr

        le = signs <= 0
        true_le = labels == -1
        invalid_le = le ^ true_le

        invalid_total = invalid_le | invalid_gr
        cur_valid = data.shape[0] - sum(invalid_total)

        for i in range(data.shape[0]):
            if invalid_total[i]:
                w += labels[i] * data[i]

        if cur_valid > best_cnt:
            best_cnt = cur_valid
            best_w = w.copy()

    plt.figure()

    print(best_w)

    NUM_P = 500
    xx = np.linspace(-2, 5.5, NUM_P)
    yy = np.linspace(-2, 6.5, NUM_P)

    x, y = np.meshgrid(xx, yy, indexing='ij')
    for lbl in [-1, 1]:
        mask_lbl = labels == lbl 
        points = data[mask_lbl][:, 1:]
        plt.scatter(points[:, 0], points[:, 1], c='g' if lbl == 1 else 'r')

    z = np.zeros((NUM_P, NUM_P))
    for i in range(NUM_P):
        for j in range(NUM_P):
            x_p = x[i, j]
            y_p = y[i, j]
            new_point = []
            for deg_x, deg_y in degs:
                new_point.append(x_p ** deg_x * y_p ** deg_y)

            new_point = np.array(new_point)
            z[i,j] = np.dot(new_point, best_w) >= 0

        

    plt.contourf(x, y, z, alpha=0.2, color=('aqua', 'y'))
    print(best_cnt)

    plt.show()


def exp_perceptron(data_old):
    data = data_old.as_matrix()
    labels = data[:, -1]
    labels = np.array([-1 if x == 0 else 1 for x in list(labels)])
    data = data[:, :-1]

    ones = np.ones((data.shape[0], 1))
    data = np.concatenate((ones, data), axis=1)
    
    w = np.random.rand(data.shape[1])

    best_w = w
    best_cnt = 0

    for it in range(400):
        cr = np.dot(data, w)
        signs = np.sign(cr)
        
        gr = signs > 0
        true_gr = labels == 1
        invalid_gr = gr ^ true_gr

        le = signs <= 0
        true_le = labels == -1
        invalid_le = le ^ true_le

        invalid_total = invalid_le | invalid_gr
        cur_valid = data.shape[0] - sum(invalid_total)

        for i in range(data.shape[0]):
            if invalid_total[i]:
                w += labels[i] * data[i]

        if cur_valid > best_cnt:
            best_cnt = cur_valid
            best_w = w.copy()


    plt.figure()
    for lbl in [-1, 1]:
        mask_lbl = labels == lbl 
        points = data[mask_lbl][:, 1:]
        plt.scatter(points[:, 0], points[:, 1], c='g' if lbl == 1 else 'r')


    a = best_w[1]
    b = best_w[2]
    c = best_w[0]

    y_getter = lambda x: (-c - a * x) / b
    xx = np.linspace(-2, 8, 1000)
    yy = [y_getter(x) for x in xx]
    plt.plot(xx, yy) 

    plt.show()

    print('correct points:', best_cnt)


def read_data():
    return pd.read_csv('blobs2.csv')


def main():
    data = read_data()

    #exp_perceptron(data)
    exp_perceptron_highdim(data, 4)


if __name__ == '__main__':
    main()
