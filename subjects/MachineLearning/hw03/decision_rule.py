import pandas as pd

import math

EPS = 1e-9


class DecisionRule:
    """Decision rules of the kind `x >= a`"""

    def __init__(self, thrsh, getter, out_val, feature_name='bla'):
        self.feature_name = feature_name
        self.threshold = thrsh
        # obj => value
        self.getter = getter
        # 0 or 1
        self.out_val = out_val

    def decide(self, obj):
        if self.getter:
            val = self.getter(obj)
        else:
            val = obj[self.feature_name]

        if val >= self.threshold:
            return self.out_val
        return not self.out_val

    def __repr__(self):
        return 'DecisionRule({}, {}, {})'.format(self.feature_name, 
                self.threshold, self.out_val)


class SplitWrapper:
    """Represents one split by DecisionRule"""

    def __init__(self):
        self.num_labels_by_class = [0, 0]

    def update(self, label, add):
        self.num_labels_by_class[label] += add

    def num_objects(self):
        ret = sum(self.num_labels_by_class)
        return EPS if ret == 0 else ret

    def calc_entropy(self):
        res = 0
        for i in range(2):
            cur_objects = self.num_labels_by_class[i]
            val = cur_objects / self.num_objects()
            if val != 0:
                res += val * math.log(val, 2)

        return -res

    def calc_gini(self):
        num_objects = sum(self.num_labels_by_class)
        res = 0

        for i in range(2):
            prob = self.num_labels_by_class[i] / self.num_objects()
            res += prob * (1 - prob)

        return res

    def __repr__(self):
        return 'SplitWrapper({})'.format(', '.join(map(str, self.num_labels_by_class)))



class ClassificationResultWrapper:

    def __init__(self):
        self.true_positive = 0
        self.false_negative = 0
        self.false_positive = 0
        self.true_negative = 0

        self.predicted_by_label = [SplitWrapper(), SplitWrapper()]
        self.total_predicted = SplitWrapper()

    def calc_precision(self):
        return self.true_positive / (self.true_positive + self.false_positive + EPS)

    def calc_recall(self):
        return self.true_positive / (self.true_positive + self.false_negative + EPS)

    def calc_true_positive_rate(self):
        return self.calc_recall()

    def calc_false_positive_rate(self):
        return self.false_positive / (self.false_positive + self.true_negative + EPS)

    def update(self, ground_truth, predicted, add=1):
        self.predicted_by_label[predicted].update(ground_truth, add)
        self.total_predicted.update(ground_truth, add)

        if ground_truth == predicted:
            if predicted == 1:
                self.true_positive += add
            else:
                self.true_negative += add
        else:
            if ground_truth == 1:
                self.false_negative += add
            else:
                self.false_positive += add

    def to_pareto_point(self):
        return (self.calc_recall(), self.calc_precision())

    def to_roc_curve_point(self):
        return (self.calc_false_positive_rate(), self.calc_true_positive_rate())

    def __eq__(self, other):
        return (self.true_positive == other.true_positive and
            self.true_negative == other.true_negative and
            self.false_positive == other.false_positive and
            self.false_negative == other.false_negative)

    def __hash__(self):
        return id(self)



def calc_results_for_rules_scanline(rules, data, getter):
    labels = data['label']
    num_unseen_labels = [0, 0]
    num_unseen_labels[1] = sum(labels)
    num_unseen_labels[0] = len(labels) - num_unseen_labels[1]
    total_labels = num_unseen_labels[:]

    answers = [ClassificationResultWrapper() for rule in rules]

    events = []
    for i in range(data.shape[0]):
        obj = data.iloc[i]
        value = getter(obj)
        events.append((value, 1, int(obj['label'])))

    for i, rule in enumerate(rules):
        events.append((rule.threshold, 0, i))

    events.sort()

    for ev in events:
        tp = ev[1]

        if tp == 0:
            # Rule
            rule = rules[ev[2]]
            result = answers[ev[2]]
            result.update(0, rule.out_val, num_unseen_labels[0])
            result.update(1, rule.out_val, num_unseen_labels[1])

            result.update(0, not rule.out_val, total_labels[0] - num_unseen_labels[0])
            result.update(1, not rule.out_val, total_labels[1] - num_unseen_labels[1])
        else:
            num_unseen_labels[ev[2]] -= 1

    return answers


def calc_result_for_rule_naive(dec_rule, data):
    res = ClassificationResultWrapper()

    for i in range(data.shape[0]):
        obj = data.iloc[i]
        predicted = dec_rule.decide(obj)
        ground_truth = obj['label']

        res.update(ground_truth, predicted)

    return res

def split_naive(data, rule):
    res = [[], []]

    for i in range(data.shape[0]):
        obj = data.iloc[i]
        
        predicted = rule.decide(obj)
        res[predicted].append(obj)

    res[0] = pd.DataFrame(res[0])
    res[1] = pd.DataFrame(res[1])
    return tuple(res)
