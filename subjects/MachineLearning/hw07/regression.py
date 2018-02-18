import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

from sklearn.preprocessing import PolynomialFeatures
from sklearn.linear_model import Lasso, ElasticNet


class BaseRegression:
    def __init__(self, num_poly_features=1):
        self.num_poly_features = num_poly_features
        self.transformer = PolynomialFeatures(degree=self.num_poly_features)

    def predict(self, obj):
        obj = self.transformer.transform(obj)[0]
        return np.dot(self.w, obj)


class LinearRegression(BaseRegression):
    
    def fit(self, objects, values):
        poly_features = self.transformer.fit_transform(objects)

        pinv = np.dot(np.transpose(poly_features), poly_features)
        pinv = np.linalg.inv(pinv)
        pinv = np.dot(pinv, np.transpose(poly_features))

        self.w = np.dot(pinv, values)


class RegularizedRegression(BaseRegression):

    def __init__(self, num_poly_features, alpha):
        super().__init__(num_poly_features)
        self.alpha = alpha


class RidgeRegression(RegularizedRegression):

    def fit(self, objects, values):
        poly_features = self.transformer.fit_transform(objects)

        pinv = np.dot(np.transpose(poly_features), poly_features)
        reg_addend = self.alpha * np.eye(pinv.shape[0])
        pinv = np.linalg.inv(pinv + reg_addend)
        pinv = np.dot(pinv, np.transpose(poly_features))

        self.w = np.dot(pinv, values)


class ElasticNetProxy(RegularizedRegression):

    def fit(self, objects, values):
        poly_features = self.transformer.fit_transform(objects)
        self.predictor = ElasticNet(self.alpha, max_iter=10**5)
        self.predictor.fit(poly_features, values)

    def predict(self, obj):
        obj = self.transformer.transform(obj)
        return self.predictor.predict(obj)[0]


class LassoProxy(RegularizedRegression):

    def fit(self, objects, values):
        poly_features = self.transformer.fit_transform(objects)
        self.predictor = Lasso(self.alpha, normalize=True, max_iter=10**5)
        self.predictor.fit(poly_features, values)

    def predict(self, obj):
        obj = self.transformer.transform(obj)
        return self.predictor.predict(obj)[0]


def get_data_sin():
    data = pd.read_csv('./noisysine.csv')
    objects = data.iloc[:, 0]
    objects = objects.as_matrix()
    objects = objects.reshape(-1, 1)
    values = data.iloc[:, 1]
    return objects, values


def get_data_hydro():
    data = pd.read_csv('./hydrodynamics.csv')
    objects = data.iloc[:, :-1].as_matrix()
    values = data.iloc[:, -1]
    return objects, values



def predict_on_objects(regressor, objects):
    values = []
    for obj in objects:
        obj = obj.reshape(1, -1)
        values.append(regressor.predict(obj))
    return np.array(values)


def calc_r2score(true_values, predicted_values):
    mean = true_values.mean()
    err_baseline = 0
    err_algo = 0

    for val, true_val in zip(predicted_values, true_values):
        err_baseline += pow(true_val - mean, 2)
        err_algo += pow(val - true_val, 2)

    return 1 - err_algo / err_baseline


def general_sin(reg_cls, **kwargs):
    plt.figure(figsize=(12, 8))
    objects, train_values = get_data_sin()
    
    colors = ['coral', 'red', 'burlywood', 'orange', 'crimson',
            'dimgrey', 'c', 'darkgreen', 'yellow', 'black']
    for degree in range(1, 10 + 1):
        regressor = reg_cls(degree, **kwargs)
        regressor.fit(objects, train_values)
        xs = np.linspace(-1, 40, 1000)

        values = [regressor.predict(np.array([x]).reshape(-1, 1)) for x in xs]
        print('degree = {}'.format(degree))
        print('R2-score: {}'.format(calc_r2score(train_values, 
            predict_on_objects(regressor, objects))))
        plt.plot(xs, values, c=colors[degree - 1], label='degree {}'.format(degree))


    plt.scatter(list(range(0, 40)), train_values, c='aqua', label='train points')

    plt.legend(loc='lower left')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.title('{}'.format(str(reg_cls.__name__)))

    plt.savefig('./results/{}_noisysin.png'.format(str(reg_cls.__name__)), dpi=400)
#    plt.show()


def general_hydro(reg_cls, need_print=True, **kwargs):
    objects, train_values = get_data_hydro()

    ret_score = 0
    for degree in range(1, 2 + 1):
        regressor = reg_cls(degree, **kwargs)
        regressor.fit(objects, train_values)

        cur_score = calc_r2score(train_values,
            predict_on_objects(regressor, objects))

        if need_print:
            print('Degree = {}'.format(degree))
            print('R2-score: {}'.format(cur_score))
        ret_score = cur_score

    return ret_score


def experiment_linear_regression_sin():
    general_sin(LinearRegression)

def experiment_linear_regression_hydro():
    general_hydro(LinearRegression)

def experiment_ridge_regression_sin():
    general_sin(RidgeRegression, alpha=1e-4)

def experiment_ridge_regression_hydro():
    general_hydro(RidgeRegression, alpha=0)
    '''
    alpha_search = np.logspace(-14, -10, num=100, base=10)

    all_values = []
    for alpha in alpha_search:
        print(alpha)
        val = general_hydro(RidgeRegression, need_print=False, alpha=alpha)
        print(val)
        all_values.append(val)

    all_values = np.array(all_values)
    print(all_values)
    print(all_values.max())
    '''

def experiment_lasso_net_sin():
    #general_sin(LassoProxy, alpha=1e-8)
    general_sin(ElasticNetProxy, alpha=1e-7)

def experiment_lasso_net_hydro():
    general_hydro(LassoProxy, alpha=5e-5)
    general_hydro(ElasticNetProxy, alpha=1e-6)


def main():
    #experiment_linear_regression_sin()
    #experiment_linear_regression_hydro()
    #experiment_ridge_regression_sin()
    #experiment_ridge_regression_hydro()

    experiment_lasso_net_sin()
    #experiment_lasso_net_hydro()


if __name__ == '__main__':
    main()

