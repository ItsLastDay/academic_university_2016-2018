#! /usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np
from scipy.spatial import ConvexHull


# 1
def gen_points(n, x, y, scale):
    '''
    >>> gen_points(1, 0, 0, 1).shape
    (1, 2)
    >>> ps = gen_points(5, 0, 0, 1)
    >>> ps.shape
    (5, 2)
    >>> ps.dtype
    dtype('float64')
    '''
    xs = np.random.normal(0, scale, (n, 1)) + x
    ys = np.random.normal(0, scale, (n, 1)) + y
    return np.hstack([xs, ys])


# 2
def build_convex_hull(points):
    '''
    >>> ch = build_convex_hull(gen_points(3, 0, 0, 1))
    >>> ch.shape
    (3, 2)
    >>> np.cross(ch[1] - ch[0], ch[2] - ch[0]) > 0
    True
    '''

    assert len(points) >= 3
    hull = ConvexHull(points)
    return hull.points[hull.vertices]


# 3
def calc_square(polygon):
    '''
    >>> calc_square(np.array([[0., 0], [1, 0], [0, 1]]))
    0.5
    >>> calc_square(np.array([[0., 0], [1, 0], [1, 1], [0, 1]]))
    1.0
    '''

    assert len(polygon) >= 3
    xs = polygon[:, 0]
    ys = polygon[:, 1]
    xs_r = np.roll(xs, -1)
    ys_r = np.roll(ys, -1)

    s = ((xs - xs_r) * (ys + ys_r)).sum() / 2
    return s


# 4
def plot(polygon):
    xs = polygon[:, 0]
    ys = polygon[:, 1]

    # Draw border.
    plt.fill(xs, ys, 'r', ls='solid', edgecolor='black',
             lw=2, alpha=1, fc='none')
    # Draw polygon.
    plt.fill(xs, ys, fc='r', alpha=0.1)

    # Draw axes.
    plt.axhline(color='k')
    plt.axvline(color='k')

    # Draw dashed lines from vertices to X-axis.
    plt.vlines(xs, ymin=0, ymax=ys, linestyles='dashed')

    plt.show()

if __name__ == '__main__':
    import doctest
    doctest.testmod()

    plot(build_convex_hull(gen_points(50, 0, 0, 10)))
