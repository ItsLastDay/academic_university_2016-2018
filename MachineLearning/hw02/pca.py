import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

from sklearn.decomposition import PCA

import random
from kmeans import KMeans


def read_data():
    raw_data = pd.read_csv('mnist.csv')
    labels = raw_data.iloc[:, 0]
    objects = raw_data.iloc[:, 1:]
    return objects, labels


def main():
    random.seed(42)

    objects, labels = read_data()
    
    if False:
        clusters = eval(open('clust_result.txt', 'r').read().strip())

        clusters = np.array(clusters)

        print('     ' + (' ' * 4).join(map(str, range(10))))
        for k in range(10):
            clustered_as_k = clusters == k
            print(k, end=' ')

            labels_with_k = labels[clustered_as_k]
            for lb in range(10):
                lbl_eq_lb = labels_with_k[labels_with_k == lb]
                print('{:4d}'.format(len(lbl_eq_lb)), end=' ')
            print('')
    else:
        pca = PCA(n_components=2)
        transformed_objects = pca.fit_transform(objects)
        transformed_objects = pd.DataFrame(transformed_objects, columns=['X', 'Y'])

        #clusterer = KMeans(k=10)
        #clusters, centers = clusterer.cluster(transformed_objects)

        clusters = eval(open('clust_result.txt', 'r').read().strip())

        clusters = np.array(clusters)

        x_coord = transformed_objects['X']
        y_coord = transformed_objects['Y']
        colors = np.array(clusters)
        
        plt.figure()
        plt.scatter(x_coord, y_coord, c=colors)
        plt.show()

if __name__ == '__main__':
    main()
