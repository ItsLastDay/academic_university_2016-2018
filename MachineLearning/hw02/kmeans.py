import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

from scipy.spatial.distance import euclidean

import os.path
import random


RESULT_DIR = './exp_result'


class KMeans:

    def __init__(self, k):
        self.k = k
        self.objects = None
        self.num_objects = None


    def _choose_centers(self):
        cluster_centers = []
        for i in range(self.k):
            idx = random.randint(0, self.objects.shape[0] - 1)
            cluster_centers.append(self.objects.iloc[idx])
            
        return cluster_centers

    def _calc_clusters(self, centers):
        clusters = []

        for i in range(self.num_objects):
            obj = self.objects.iloc[i]

            distances = [euclidean(obj, center) for center in centers]
            best_dist_idx = np.argmin(distances)

            clusters.append(best_dist_idx)
        
        return clusters


    def _calc_centers(self, clusters):
        clusters = np.array(clusters)
        centers = []

        for i in range(self.k):
            cur_group_mask = clusters == i
            cur_group = self.objects[cur_group_mask]

            x = sum(cur_group['X'])
            y = sum(cur_group['Y'])
            x /= len(cur_group)
            y /= len(cur_group)

            centers.append((x, y))

        return centers


    def cluster(self, objects):
        self.objects = objects
        self.num_objects = objects.shape[0]

        centers = self._choose_centers()
        clusters = self._calc_clusters(centers)

        clusters_did_not_change = False
        it = 0
        while not clusters_did_not_change:
            it += 1
            print('ITERATION {}'.format(it))
            new_centers = self._calc_centers(clusters)
            new_clusters = self._calc_clusters(new_centers)

            if new_clusters != clusters:
                clusters = new_clusters
                centers = new_centers
            else:
                clusters_did_not_change = True

            if it % 100 == 0:
                print(clusters)
                print(centers)


        return clusters, centers
        


def read_data():
    return pd.read_csv('blobs.csv')


def main():
    random.seed(42)

    objects = read_data()

    for k in [2, 3, 4]:
        clusterer = KMeans(k)
        obj_to_cluster, cluster_centers = clusterer.cluster(objects)

        x_coord = objects['X']
        y_coord = objects['Y']
        colors = np.array(obj_to_cluster)
        
        plt.figure()
        plt.scatter(x_coord, y_coord, c=colors)
        plt.savefig(os.path.join(RESULT_DIR, 'kmeans_{}.png'.format(k)))



if __name__ == '__main__':
    main()
