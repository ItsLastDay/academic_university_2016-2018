from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import numpy as np

from math import *


def plot_montecarlo():
    fig = plt.figure()
    ax = fig.gca(projection='3d')

    #data = open('./graph_points').read().strip().splitlines()
    data = open('./results/func_points').read().strip().splitlines()
    x = []
    y = []
    z = []

    for line in data:
        xx, yy, zz = map(float, line.split())
        x.append(xx)
        y.append(yy)
        z.append(zz)

    print('Number of points: {}'.format(len(z)))
    print('Maximum of a function: {}'.format(max(z)))
    idx = z.index(max(z))
    print('({}, {})'.format(x[idx], y[idx]))

    surf = ax.scatter(xs=x, ys=y, zs=z)

    plt.show()

def func(x, y):
    return (20 * exp(-1.0 / 5 * (pow(x - 2.5, 2) + pow(y + 1.5, 2))) +
        3 * (1 + pow(sin(0.08 * pi * x * y), 2)) + 
        3 * log(2 + pow(10 * (x - 2.4), 4) + pow(10 * (y + 1.3), 4)) / (1 + pow((x * x + y * y) / 100.0, 0.1)))


class GaussianGenerator:

    def __init__(self, mean, stddev):
        self.mean = mean
        self.stddev = stddev

    def get_prob(self, x):
        t1 = 1.0 / sqrt(2 * pi) / self.stddev 
        t2 = exp(-0.5 * pow((x - self.mean) / float(self.stddev), 2))
        ret = t1 * t2
        return ret

def plot_crossentropy(fname, gen_x, gen_y, best_val):
    fig = plt.figure()
    ax = fig.gca(projection='3d')

    NUM_P = 200
    xx = np.linspace(-10, 10, NUM_P)
    yy = np.linspace(-10, 10, NUM_P)

    x, y = np.meshgrid(xx, yy, indexing='ij')

    z = np.zeros((NUM_P, NUM_P))
    for i in range(NUM_P):
        for j in range(NUM_P):
            x_p = x[i, j]
            y_p = y[i, j]

            z[i, j] = func(x_p, y_p)

    ax.plot_surface(x, y, z, cmap='coolwarm', linewidth=0)


    NUM_P = 1000
    xx = np.linspace(-10, 10, NUM_P)
    yy = np.linspace(-10, 10, NUM_P)
    x, y = np.meshgrid(xx, yy, indexing='ij')
    z_gaussian = np.zeros((NUM_P, NUM_P))
    for i in range(NUM_P):
        for j in range(NUM_P):
            x_p = x[i, j]
            y_p = y[i, j]

            z_gaussian[i, j] = gen_x.get_prob(x_p) * gen_y.get_prob(y_p)

    z_gaussian /= z_gaussian.max()
    z_gaussian += 20

    ax.plot_surface(x, y, z_gaussian, cmap='OrRd', linewidth=0)



    ax.set_title('{}: best val = {}'.format(fname, best_val))
    ax.set_zlabel('f(x, y)')
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlim3d(20, 40)

    ax.view_init(azim=38.29, elev=16.6)

    plt.savefig('./results/{}.png'.format(fname))


inp = open('./results/cross_entropy_steps', 'r').read().strip().splitlines()

for i in range(0, len(inp), 5):
    fname = inp[i]
    x_mean, x_stddev = map(float, inp[i + 1].split())
    y_mean, y_stddev = map(float, inp[i + 2].split())
    val = float(inp[i + 3][inp[i + 3].index(':') + 2:])
    plot_crossentropy(fname, GaussianGenerator(x_mean, x_stddev),
            GaussianGenerator(y_mean, y_stddev), val)
