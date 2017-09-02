import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import colors as plt_colors

from sklearn.cluster import AffinityPropagation as AffinityPropagation_SK
from sklearn.metrics.pairwise import euclidean_distances
from scipy.spatial.distance import euclidean

import sys
import os.path
import random
from subprocess import getoutput, getstatusoutput
from collections import defaultdict


RESULT_DIR = './exp_result'


class AffinityPropagation:

    def __init__(self, max_iter=1, max_cons_iter=5, init_self_val=None, damping=0.9):
        self.max_iter = max_iter
        self.max_cons_iter = max_cons_iter
        self.init_self_val = init_self_val
        # See http://www.psi.toronto.edu/affinitypropagation/faq.html for details.
        self.damping = damping

    
    def _calc_clusters(self):
        point_to_cluster = defaultdict(list)

        for i in range(self.n):
            vals = [self.a[i][k] + self.r[i][k] for k in range(self.n)]
            mx_idx = np.argmax(vals)
#            print(i, vals[mx_idx], vals)

            point_to_cluster[mx_idx].append(i)

        clusters = [-1 for i in range(self.n)]
        for idx, best_guy in enumerate(point_to_cluster.keys()):
            for clustered_with_guy in point_to_cluster[best_guy]:
                clusters[clustered_with_guy] = idx

        return clusters, list(point_to_cluster.keys())



    def _update_r(self):
        for i in range(self.n):
            vals = [self.a[i][m] + self.affinity[i][m] for m in range(self.n)]
            ind = np.argpartition(vals, -2)[-2:]

            # ind[0] should be max index
            if vals[ind[0]] < vals[ind[1]]:
                ind[0], ind[1] = ind[1], ind[0]

            assert(ind[0] == np.argmax(vals))
            assert(ind[0] != ind[1])

            for k in range(self.n):
                sub = vals[ind[1]] if k == ind[0] else vals[ind[0]]
                old_val = self.r[i][k]
                new_val = self.affinity[i][k] - sub

                self.r[i][k] = self.damping * old_val + (1 - self.damping) * new_val

        #print('r updated')


    def _update_a(self):
        for k in range(self.n):
            vals = [max(0, self.r[j][k]) for j in range(self.n)]
            full_sum = sum(vals)

            for i in range(self.n):
                old_val = self.a[i][k]
                new_val = None
                if i == k:
                    new_val = full_sum - vals[i]
                else:
                    new_val = min(0, self.r[k][k] + full_sum - vals[i] - vals[k])

                self.a[i][k] = self.damping * old_val + (1 - self.damping) * new_val

        #print(np.matrix(self.a))
        #print('a updated')


    def cluster(self, objects):
        self.n = objects.shape[0]
        self.affinity = [[0 for i in range(self.n)] for j in range(self.n)]

        self.r = [[0 for i in range(self.n)] for j in range(self.n)]
        self.a = [[0 for i in range(self.n)] for j in range(self.n)]


        self.affinity = -euclidean_distances(objects, squared=True)
        if self.init_self_val is None:
            self.init_self_val = np.median(self.affinity)

        for i in range(self.n):
            self.affinity[i][i] = self.init_self_val
        print('Main diagonal affinity is {}'.format(self.init_self_val))

        clusters, centers_idx = self._calc_clusters()

        it = 0
        unchanged = 0
        while it < self.max_iter and unchanged < self.max_cons_iter:
            it += 1
            print('ITERATION {}'.format(it))

            self._update_r()
            self._update_a()

            new_clusters, centers_idx = self._calc_clusters()
            #print(new_clusters)

            if new_clusters == clusters:
                unchanged += 1
            else:
                unchanged = 1

            print('Clusters did not change for {} iterations'.format(unchanged))
            clusters = new_clusters

        return clusters, centers_idx


def read_data():
    return pd.read_csv('blobs.csv')


def main():
    random.seed(42)

    objects = read_data()


    if True:
        clusterer = AffinityPropagation(max_iter=100, max_cons_iter=15, 
                damping=0.7, init_self_val=-300)
        obj_to_cluster, centers_idx = clusterer.cluster(objects)
    else:
        clusterer = AffinityPropagation_SK()
        obj_to_cluster = clusterer.fit_predict(objects)
        centers_idx = clusterer.cluster_centers_indices_

    x_coord = objects['X']
    y_coord = objects['Y']
    color_names = ['red', 'green', 'blue', 'yellow'] + list(sorted(plt_colors.cnames.keys()))[4::8]
    if len(color_names) < len(set(obj_to_cluster)):
        colors = np.array(obj_to_cluster)
    else:
        colors = [color_names[x] for x in obj_to_cluster]

    print('Number of clusters: {}'.format(len(set(obj_to_cluster))))
    plt.figure()
    plt.scatter(x_coord, y_coord, c=colors)
    for center_id in centers_idx:
        plt.scatter([objects.iloc[center_id][0]], [objects.iloc[center_id][1]], marker='<', 
                c=colors[center_id], 
                s=100)
    plt.savefig(os.path.join(RESULT_DIR, 'affprop.png'))


if __name__ == '__main__':
    main()
