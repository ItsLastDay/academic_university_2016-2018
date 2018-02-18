import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

from sklearn.cluster import DBSCAN as DBSCAN_SK
from scipy.spatial.distance import euclidean

import os.path
import random


RESULT_DIR = './exp_result'


class DBSCAN:
    def __init__(self, eps=0.5, min_samples=5, metric=lambda x, y: np.linalg.norm(x - y)):
        self.eps = eps
        self.min_samples = min_samples
        self.metric = metric
        self.labels_ = np.ndarray(0, dtype=int)
    
    # returns indicies of neighbouring to center points
    def _get_neighbours(self, centre, samples):
        res = []
        for i in range(len(samples)):
            if 0 < self.metric(centre, samples[i]) <= self.eps:
                res.append(i)
        return res

    # mark all points density-connected to 'cluster' as belonging to cluster 'n'
    def _expand(self, samples, cluster, visited, n):
        i = 0
        while i < len(cluster):
            if visited[cluster[i]]:
                i += 1
                continue
            
            visited[cluster[i]] = True
            self.labels_[cluster[i]] = n 
            
            # add all the neighbourhood, only for core samples
            nbh = self._get_neighbours(samples[cluster[i]], samples)
            if len(nbh) >= self.min_samples:
                for j in nbh:
                    if visited[j]:
                        continue
                    cluster.append(j)
            i += 1

    def fit(self, samples):
        samples = samples.as_matrix()
        n = len(samples)
        self.labels_ = np.ndarray(n, dtype=int)
        cur_clusters = -1
        vis = [False for i in range(n)]

        for i in range(n):
            if vis[i]:
                continue
            neighbours = self._get_neighbours(samples[i], samples) 
            vis[i] = True

            if len(neighbours) < self.min_samples:
                # noise 
                self.labels_[i] = -1
                continue
            
            # found new cluster
            cur_clusters += 1
            self.labels_[i] = cur_clusters
            self._expand(samples, neighbours, vis, cur_clusters)

        return self

    def fit_predict(self, samples):
        self.fit(samples)
        return self.labels_
        

def read_data():
    return pd.read_csv('blobs.csv')


def main():
    random.seed(42)

    NOISE_LABEL = -1

    objects = read_data()

    clusterer = DBSCAN(eps=0.25, min_samples=10)
#    clusterer = DBSCAN_SK(eps=0.25, min_samples=10)

    obj_to_cluster = clusterer.fit_predict(objects)

    non_noise = np.array(obj_to_cluster) != NOISE_LABEL
    x_coord = objects['X'][non_noise]
    y_coord = objects['Y'][non_noise]
    colors = np.array(obj_to_cluster)[non_noise]

    
    plt.figure()
    plt.scatter(x_coord, y_coord, c=colors)

    noise = np.array(obj_to_cluster) == NOISE_LABEL
    x_coord = objects['X'][noise]
    y_coord = objects['Y'][noise]
    plt.scatter(x_coord, y_coord, c='white')

    plt.savefig(os.path.join(RESULT_DIR, 'dbscan.png'))


if __name__ == '__main__':
    main()
