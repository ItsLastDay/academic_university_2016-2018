import matplotlib.pyplot as plt
import numpy as np


data_points = open('./tsp.csv').read().strip().splitlines()

def plot_l1_metric(x1, y1, x2, y2):
    if x1 != x2:
        plt.arrow(x1, y1, x2 - x1, 0, color='r', shape='full',
                length_includes_head=True, head_width=15.)
        x1 = x2

    if y1 != y2:
        plt.arrow(x1, y1, 0, y2 - y1, color='r', shape='full',
                length_includes_head=True, head_width=15.)

def plot_gr(fname):
    inp = open(fname).read().strip().splitlines()[-1]

    idx_val = inp.index(' ')
    val = inp[:idx_val]
    rest = inp[idx_val + 1:]

    print('Value is {}'.format(val))
    numbers = list(map(int, rest.strip('()').strip().split(' ')))

    if 52 in numbers:
        for i in range(len(numbers)):
            numbers[i] -= 1

    xs = []
    ys = []
    for line in data_points:
        _, x, y = map(int, line.split(','))
        xs.append(x)
        ys.append(y)

    plt.figure()

    plt.scatter(xs, ys, c='aqua')

    for i in range(1, len(numbers)):
        fr_idx = numbers[i - 1]
        to_idx = numbers[i]

        plot_l1_metric(xs[fr_idx], ys[fr_idx],
                xs[to_idx], ys[to_idx])

    plt.title('Path of length {}'.format(val.strip()))
    plt.xlabel('x')
    plt.ylabel('y')

    plt.savefig('{}.png'.format(fname))
    plt.show()

plot_gr('./results/genetic_100_600_55.txt')
#plot_gr('./results/genetic_100_500_25.txt')
#plot_gr('./results/genetic_mutate_5_10_15')
#plot_gr('./results/genetic_crossover_twice')
#plot_gr('./results/hamiltonian_path__annealing_local_search')
#plot_gr('./results/hamiltonian_path__climbing_local_search')
