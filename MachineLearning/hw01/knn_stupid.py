import pandas as pd
import numpy as np
from scipy.spatial.distance import euclidean


def read_data():
    data = pd.read_csv('spambase.csv')
    objects, labels = data.iloc[:, :-1], data.iloc[:, -1]
    return objects, labels


def knn_loo(objects, labels):
    num_obj = objects.shape[0]

    pairwise_dist = [[None for i in range(num_obj)] for j in range(num_obj)]
    for i in range(num_obj):
        obj = objects.iloc[i]
        obj_repeated = np.array([obj,] * num_obj)
        obj_repeated = np.repeat(obj, num_obj).values.reshape((len(obj), num_obj)).T
        obj_repeated -= objects

        print(i)
        pairwise_dist[i] = np.linalg.norm(obj_repeated, axis=1)

    for i in range(num_obj):
        pairwise_dist[i] = sorted([(pairwise_dist[i][j], j) for j in range(num_obj)])


    k_values = list(range(1, 10 + 1))
    loo_values = []
    for k in k_values:
        success_predicted = 0
        print(k)

        for i in range(num_obj):
            print(i)
            other_neighbour_idx = [x[1] for x in pairwise_dist[i][1:k + 1]]
            labels_neighbours = labels[other_neighbour_idx]

            votes_for_first_class = sum(labels_neighbours)
            predicted = 1 if votes_for_first_class * 2 > k else 0
            ground_truth = labels.iloc[i]
        
            success_predicted += ground_truth == predicted

        loo_values.append(success_predict / float(num_obj))

    print(loo_values)


def main():
    knn_loo(*read_data())



if __name__ == '__main__':
    main()
