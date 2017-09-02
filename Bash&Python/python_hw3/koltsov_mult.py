#! /usr/bin/env python3

import timeit
import math

import matplotlib.pyplot as plt
import numpy as np


def mult_shtrassen(mat1, mat2):
    n = mat1.shape[0] // 2
    if n <= 2 ** 10:
        return mat1 @ mat2

    a11 = mat1[:n, :n]
    a12 = mat1[:n, n:]
    a21 = mat1[n:, :n]
    a22 = mat1[n:, n:]

    b11 = mat2[:n, :n]
    b12 = mat2[:n, n:]
    b21 = mat2[n:, :n]
    b22 = mat2[n:, n:]

    s1 = a21 + a22
    s2 = s1 - a11
    s3 = a11 - a21
    s4 = a12 - s2
    s5 = b12 - b11
    s6 = b22 - s5
    s7 = b22 - b12
    s8 = s6 - b21

    p1 = mult_shtrassen(s2, s6)
    p2 = mult_shtrassen(a11, b11)
    p3 = mult_shtrassen(a12, b21)
    p4 = mult_shtrassen(s3, s7)
    p5 = mult_shtrassen(s1, s5)
    p6 = mult_shtrassen(s4, b22)
    p7 = mult_shtrassen(a22, s8)

    t1 = p1 + p2
    t2 = t1 + p4

    c11 = p2 + p3
    c12 = t1 + p5 + p6
    c21 = t2 - p7
    c22 = t2 + p5

    return np.bmat([[c11, c12], [c21, c22]])


def mult(mat1, mat2):
    n, m = mat1.shape
    m, k = mat2.shape

    # Pad matrices to be 2^r x 2^r
    mx_n = max(n, m, k)
    r = math.ceil(math.log(mx_n, 2))

    desired_shape = (2 ** r, 2 ** r)
    padded_mat1 = np.zeros(desired_shape)
    padded_mat2 = np.zeros(desired_shape)

    padded_mat1[:n, :m] = mat1
    padded_mat2[:m, :k] = mat2

    mult_result = mult_shtrassen(padded_mat1, padded_mat2)

    return mult_result[:n, :k]


def check_mult(n, m, k):
    mat1 = np.random.uniform(0, 10.0, size=(n, m))
    mat2 = np.random.uniform(0, 10.0, size=(m, k))
    assert np.linalg.norm(mult(mat1, mat2) - mat1.dot(mat2)) <= 1e-3


def test_mult():
    sizes = [1, 2, 16, 128, 512, 640]
    for n in sizes:
        for m in sizes:
            for k in sizes:
                yield check_mult, n, m, k


def measure_time_and_show_plots():
    sizes = [2 ** r for r in range(12 + 1)]
    sizes = np.linspace(1, 4096, num=20)

    times_my_mult = []
    times_numpy = []
    # Measure speed for n x n matrices.
    for n in sizes:
        n = int(n)
        print('size=', n)
        mat1 = np.random.uniform(0, 10.0, size=(n, n))
        mat2 = np.random.uniform(0, 10.0, size=(n, n))

        tm_my = timeit.Timer(lambda: mult(mat1, mat2))
        tm_numpy = timeit.Timer(lambda: mat1.dot(mat2))

        times_my_mult.append(tm_my.timeit(3))
        times_numpy.append(tm_numpy.timeit(3))

    plt.title('Measurements for matrix multiplication.')
    plt.plot(sizes, times_my_mult)
    plt.plot(sizes, times_numpy)
    plt.xlabel('n = size of square matrix')
    plt.ylabel('time (seconds)')
    plt.legend(['my multiplication', 'numpy multiplication'], loc=0)
    plt.show()

if __name__ == '__main__':
    measure_time_and_show_plots()
    pass
