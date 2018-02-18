from collections import defaultdict
import logging
import sys
from multiprocessing import Pool
import random

from decision_rule import *

# Only binary classification.

class DecisionTreeNode:

    def __init__(self, rule, data, is_leaf):
        self.is_leaf = is_leaf
        self.left = None
        self.right = None

        labels = list(data['label'])
        self.label_cnt = [labels.count(i) for i in range(2)]
        self.rule = rule

    def predict(self, obj):
        for i in range(2):
            if self.label_cnt[i] == max(self.label_cnt):
                return i
        assert False

    def predict_proba(self, obj):
        # Probability of class 1
        return self.label_cnt[1] / sum(self.label_cnt)

    def decide(self, obj):
        return self.rule.decide(obj)


def calc_one_feature(feature_name, self, data):
    if feature_name == 'label':
        return []
    current_gains_rules = []
    logging.info('feature {}'.format(feature_name))
    rules = self.rules_by_featurename[feature_name]
    getter = lambda obj: obj[feature_name]
    results_for_feature = calc_results_for_rules_scanline(rules, data, getter)

    for result, rule in zip(results_for_feature, rules):
        gain_for_this_rule = None
        total_obj = result.total_predicted.num_objects()

        if self.criterion == 'entropy':
            impurity_fun = lambda obj: obj.calc_entropy()
        elif self.criterion == 'gini':
            impurity_fun = lambda obj: obj.calc_gini()
        else:
            assert False, 'unknown criterion'

        total_gain = impurity_fun(result.total_predicted)
        for i in range(2):
            num_obj_i = result.predicted_by_label[i].num_objects()
            total_gain -= (num_obj_i * 
                    impurity_fun(result.predicted_by_label[i]) / 
                    total_obj)
        gain_for_this_rule = total_gain 

        current_gains_rules.append((gain_for_this_rule, rule))

    return current_gains_rules

class DecisionTree:

    def __init__(self, rules, max_depth, criterion='entropy'):
        if criterion not in ('gini', 'entropy'):
            raise ValueError("Criterion must be one of ['gini', 'entropy']")

        self.criterion = criterion
        self.root = None
        self.max_depth = max_depth

        key = lambda rule: rule.feature_name
        self.rules_by_featurename = defaultdict(list)
        for rule in rules:
            self.rules_by_featurename[key(rule)].append(rule)

    def fit(self, objects):
        self.root = self._build(objects, self.max_depth)


    def _build(self, data, depth):
        logging.info('Building tree {} {}'.format(len(data), depth))
        if depth == 0 or len(set(data['label'])) == 1:
            return DecisionTreeNode(None, data, True)
        else:
            gains_with_rules = []
            feature_names = list(self.rules_by_featurename.keys())

            args = [(fn, self, data) for fn in feature_names]
            
            if self.no_pool:
                for arg in args:
                    gains_with_rules.extend(calc_one_feature(*arg))
            else:
                with Pool() as pool:
                    results = pool.starmap(calc_one_feature, args)
                    for res in results:
                        gains_with_rules.extend(res)

            gains_with_rules.sort(key=lambda x: x[0], reverse=True)
            best_gain, best_rule = gains_with_rules[0]

            node = DecisionTreeNode(best_rule, data, False)
            left_split, right_split = split_naive(data, best_rule)
            if len(left_split) == 0 or len(right_split) == 0 or best_gain < EPS:
                return DecisionTreeNode(None, data, True)

            node.left = self._build(left_split, depth - 1)
            node.right = self._build(right_split, depth - 1)

            return node


    def predict(self, objects, max_depth=None):
        if max_depth is None:
            max_depth = self.max_depth
        # expect list of pd.Series
        return [self.predict_one(obj, max_depth) for obj in objects]

    def predict_proba(self, objects, max_depth=None):
        if max_depth is None:
            max_depth = self.max_depth
        # expect list of pd.Series
        return [self.predict_one_proba(obj, max_depth) for obj in objects]

    def predict_one(self, obj, max_depth):
        if self.root is None:
            raise ValueError("Trying to predict from an unfitted tree.")

        root = self.root
        for i in range(max_depth):
            if root.is_leaf:
                break

            if root.decide(obj) == 0:
                root = root.left
            else:
                root = root.right 

        return root.predict(obj)


    def predict_one_proba(self, obj, max_depth):
        if self.root is None:
            raise ValueError("Trying to predict from an unfitted tree.")

        root = self.root
        for i in range(max_depth):
            if root.is_leaf:
                break

            if root.decide(obj) == 0:
                root = root.left
            else:
                root = root.right 

        return root.predict_proba(obj)


def train_tree(train_set, features_set, depth, criterion, no_pool=False):
    # train_set : pd.DataFrame

    all_rules = []
    for feature_name in features_set:
        all_values = set(train_set[feature_name])
        getter = None # Capturing `feature_name` in lambda caputres `label` all the time.

        for value in all_values:
            all_rules.append(DecisionRule(value, getter, 1, feature_name))

    tree = DecisionTree(all_rules, depth, criterion)
    tree.no_pool = no_pool
    tree.fit(train_set)
    return tree


def sample_with_replacement(lst, sz):
    res = []
    for i in range(sz):
        res.append(random.choice(lst))

    return res

def build_one_tree(self, data, _, avail_features, objects_lst):
    logging.info('Training tree #{} in the forest'.format(_))

    cur_features = random.sample(avail_features, self.num_features)
    cur_objects = sample_with_replacement(objects_lst, data.shape[0])
    cur_objects = pd.DataFrame(cur_objects)

    tree = train_tree(cur_objects, cur_features, 
            self.max_depth, self.criterion, no_pool=True)
    return tree

class RandomForest:
    
    def __init__(self, num_trees, max_depth, num_features):
        self.num_trees = num_trees
        self.max_depth = max_depth
        self.num_features = num_features
        self.criterion = 'gini'

        self.trees = []

    def fit(self, data):
        logging.error('Fitting forest with {} trees, {} depth, {} features'.format(
            self.num_trees, self.max_depth, self.num_features))
        avail_features = list(data.columns)
        avail_features.remove('label')
            
        objects_lst = []
        for i in range(data.shape[0]):
            objects_lst.append(data.iloc[i])

        with Pool() as pool:
            args = [(self, data, i, avail_features, objects_lst) 
                    for i in range(self.num_trees)]
            self.trees = list(pool.starmap(build_one_tree, args))

    def predict(self, objects, max_depth=None):
        if max_depth is None:
            max_depth = self.max_depth
        # expect list of pd.Series
        return [self.predict_one(obj, max_depth) for obj in objects]

    def predict_one(self, obj, max_depth):
        votes = [0, 0]
        for tree in self.trees:
            cur_vote = tree.predict([obj], max_depth)[0]
            votes[cur_vote] += 1

        for i in range(2):
            if votes[i] == max(votes):
                return i

        assert False
