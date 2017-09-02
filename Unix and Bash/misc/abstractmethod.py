
class AMeta(type):
    def __init__(cls, name, bases, dct):
        super().__init__(name, bases, dct)

        uncovered_abstract_names = set()

        for base in cls.mro():
            for k in base.__dict__:
                attr = getattr(cls, k)
                if getattr(attr, 'is_abstract', False) is True:
                    uncovered_abstract_names.add(k)

        cls.uncovered_abstract_names = frozenset(uncovered_abstract_names)

    def __call__(cls, *args, **kwargs):
        if cls.uncovered_abstract_names:
            raise TypeError('Class {} cannot be instantiated, because method(s) "{}" are\
            not overriden.'.format(cls.__name__, ', '.join(cls.uncovered_abstract_names)))

        return super().__call__(*args, **kwargs)



def abstractmethod(f):
    f.is_abstract = True
    return f


class X(metaclass=AMeta):
    @abstractmethod
    def f(self, x):
        return x + 5

    @abstractmethod 
    def g(self, y):
        return y - 1

class T(X):
    def g(self, q, r, t):
        return q + r - t


class Y(T):
    pass

g = X()
t = Y()
