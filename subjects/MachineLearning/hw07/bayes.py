import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

from sklearn.preprocessing import binarize
from sklearn.naive_bayes import GaussianNB as SKGaussian
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, roc_curve, auc, precision_score, recall_score
from sklearn.feature_extraction.text import CountVectorizer

import math


def get_data_smsspam():
    data = pd.read_csv('smsspam.csv')
    label, text = data.iloc[:, 0], data.iloc[:, 1:]
    return text, label


def get_data_spambase():
    data = pd.read_csv('spambase.csv')
    objects, labels = data.iloc[:, :-1], data.iloc[:, -1]
    return objects, labels


def get_binarized_data_spambase():
    data = pd.read_csv('spambase.csv')
    objects, labels = data.iloc[:, :-1], data.iloc[:, -1]
    objects = pd.DataFrame(binarize(objects))
    return objects, labels


class MultinomialClassifier:

    def __init__(self):
        self.labels = []
        self.alpha = 0.5
        self.percentage_by_label = dict()
        self.vectorizer_by_label = dict()
        self.transform_by_label = dict()
        self.num_words_by_label = dict()
        self.trans_words_by_label = dict()
        self.total_vocab_size = 0

    def _fit_label(self, label, texts):
        texts = [texts.iloc[i, 0] for i in range(texts.shape[0])]
        cnt = CountVectorizer()
        cnt.fit(texts)
        self.vectorizer_by_label[label] = cnt
        r = cnt.transform(texts)
        self.transform_by_label[label] = r
        self.num_words_by_label[label] = r.sum()
        self.trans_words_by_label[label] = r.sum(axis=0)

    def fit(self, objects, labels):
        unique_labels = set(labels)
        self.labels = sorted(list(unique_labels))

        cnt = CountVectorizer()
        cnt.fit([objects.iloc[i, 0] for i in range(objects.shape[0])])
        self.total_vocab_size = len(cnt.vocabulary_)

        for label in unique_labels:
            mask = labels.as_matrix() == label
            self.percentage_by_label[label] = sum(mask) / float(objects.shape[0])
            self._fit_label(label, objects[mask])

        self.label_with_max_percentage = self.labels[0]
        for label in self.labels:
            if self.percentage_by_label[label] > self.percentage_by_label[self.label_with_max_percentage]:
                self.label_with_max_percentage = label

    def predict_proba(self, obj):
        # Return <label, probability> pair.

        lbl_prob = [self._predict_label(obj, label) for label 
                in self.labels]

        sum_prob = sum(pr[1] for pr in lbl_prob) 
        if sum_prob == 0:
            return (self.label_with_max_percentage, 1.0)
        
        mx_idx = 0
        for i in range(len(lbl_prob)):
            if lbl_prob[i][1] > lbl_prob[mx_idx][1]:
                mx_idx = i

        return (lbl_prob[mx_idx][0], lbl_prob[mx_idx][1] / sum_prob)


    def _predict_label(self, obj, label):
        ret = 1.0

        obj = obj.iloc[0]
        vectorizer = self.vectorizer_by_label[label]
        tokens = vectorizer.build_analyzer()(obj)

        total_words_label = self.num_words_by_label[label]
        total_vocab = self.total_vocab_size

        for token in tokens:
            idx = vectorizer.vocabulary_.get(token, -1)

            occ_word = 0 if idx == -1 else self.trans_words_by_label[label][0, idx]

            prob = (occ_word + float(self.alpha)) / (total_words_label + 
                    self.alpha * total_vocab)

            ret *= prob

        ret *= self.percentage_by_label[label]
        return (label, ret)


class BernoulliClassifier:

    def __init__(self):
        self.labels = []
        self.percentage_by_label = dict()
        self.cnt_label = dict()
        self.perc_ones_by_label = dict()

    def _fit_label(self, label, objects):
        objects = objects.as_matrix()
        sum_ones = objects.sum(axis=0)
        sum_ones /= objects.shape[0]
        self.perc_ones_by_label[label] = sum_ones

    def fit(self, objects, labels):
        unique_labels = set(labels)
        self.labels = list(unique_labels)

        for label in unique_labels:
            mask = labels.as_matrix() == label
            self.percentage_by_label[label] = sum(mask) / float(objects.shape[0])
            self._fit_label(label, objects[mask])

    def predict_proba(self, obj):
        # Return <label, probability> pair.

        lbl_prob = [self._predict_label(obj, label) for label 
                in self.labels]

        sum_prob = sum(pr[1] for pr in lbl_prob) + 1e-300
        
        mx_idx = 0
        for i in range(len(lbl_prob)):
            if lbl_prob[i][1] > lbl_prob[mx_idx][1]:
                mx_idx = i

        return (lbl_prob[mx_idx][0], lbl_prob[mx_idx][1] / sum_prob)


    def _predict_label(self, obj, label):
        ret = 1.0
        for i in range(obj.shape[0]):
            x = obj.iloc[i]
            one = self.perc_ones_by_label[label][i] * x 
            zero = (1 - self.perc_ones_by_label[label][i]) * (1 - x)
            ret *= one + zero

        ret *= self.percentage_by_label[label]
        return (label, ret)

class GaussianClassifier:

    def __init__(self):
        self.labels = []
        self.percentage_by_label = dict()
        self.mean_by_label = dict()
        self.variance_by_label = dict()

    def _fit_label(self, label, objects):
        objects = objects.as_matrix()
        mean = objects.mean(axis=0)
        variance = objects.var(axis=0)
        self.mean_by_label[label] = mean
        self.variance_by_label[label] = variance

    def fit(self, objects, labels):
        unique_labels = set(labels)
        self.labels = list(unique_labels)

        for label in unique_labels:
            mask = labels.as_matrix() == label
            self.percentage_by_label[label] = sum(mask) / float(objects.shape[0])
            self._fit_label(label, objects[mask])

    def predict_proba(self, obj):
        # Return <label, probability> pair.

        lbl_prob = [self._predict_label(obj, label) for label 
                in self.labels]

        sum_prob = sum(pr[1] for pr in lbl_prob) + 1e-300
        
        mx_idx = 0
        for i in range(len(lbl_prob)):
            if lbl_prob[i][1] > lbl_prob[mx_idx][1]:
                mx_idx = i

        return (lbl_prob[mx_idx][0], lbl_prob[mx_idx][1] / sum_prob)


    def _predict_label(self, obj, label):
        ret = 1.0
        for i in range(obj.shape[0]):
            x = obj.iloc[i]
            prob = 1.0 / math.sqrt(max(1e-30, 2 * math.pi * self.variance_by_label[label][i]))
            prob *= math.exp(-1 * ((x - self.mean_by_label[label][i]) ** 2) 
                    / 2.0 / max(1e-30, self.variance_by_label[label][i]))
            ret *= prob

        ret *= self.percentage_by_label[label]
        return (label, ret)


def experiment_bayes_general(objects, labels, cls, pos_label=1):
    x_train, x_test, y_train, y_test = train_test_split(objects, labels, test_size=0.3,
            stratify=labels)
    cls.fit(x_train, y_train)

    predicted_labels = []
    predicted_probs_of_first_class = []
    for i in range(x_test.shape[0]):
        obj = x_test.iloc[i]
        pred_lbl, prob = cls.predict_proba(obj)
        predicted_labels.append(pred_lbl)

        if pred_lbl == pos_label:
            predicted_probs_of_first_class.append(prob)
        else:
            predicted_probs_of_first_class.append(1.0 - prob)

    acc = accuracy_score(y_test, predicted_labels)
    print('Accuracy: {}%'.format(acc * 100))
    print('Precision: {}%'.format(precision_score(y_test, predicted_labels,
        pos_label=pos_label) * 100))
    print('Recall: {}%'.format(recall_score(y_test, predicted_labels,
        pos_label=pos_label) * 100))
    
    fpr, tpr, _ = roc_curve(y_test, predicted_probs_of_first_class, pos_label=pos_label)

    plt.figure()
    lw = 2
    plt.plot(fpr, tpr, label='ROC curve (auc={})'.format(auc(fpr, tpr)), lw=lw)
    plt.plot([0, 1], [0, 1], color='navy', linestyle='--', lw=lw)
    plt.xlim([0.0, 1.0])
    plt.ylim([0.0, 1.05])
    plt.xlabel('False Positive Rate')
    plt.ylabel('True Positive Rate')
    plt.title('Receiver operating characteristic')
    plt.legend(loc="lower right")
    plt.show()

def experiment_gaussian():
    objects, labels = get_data_spambase()

    cls = GaussianClassifier()
    experiment_bayes_general(objects, labels, cls)

def experiment_bernoulli():
    objects, labels = get_binarized_data_spambase()
    cls = BernoulliClassifier()
    experiment_bayes_general(objects, labels, cls)


def experiment_multinomial():
    texts, labels = get_data_smsspam()

    cls = MultinomialClassifier()
    experiment_bayes_general(texts, labels, cls, pos_label='spam')


def main():
    #experiment_gaussian()
    experiment_multinomial()
    #experiment_bernoulli()


if __name__ == '__main__':
    main()
