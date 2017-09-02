
from collections import OrderedDict

class order(type):
    @classmethod
    def __prepare__(mcls, name, bases, **kwargs):
        if kwargs.get('ordered', None):
            return OrderedDict()
        return dict()

    def __new__(cls, name, bases, dct, **kwargs):
        return super().__new__(cls, name, bases, dct)

    def __init__(cls, name, bases, dct, **kwargs):
        super().__init__(name, bases, dct)


class A(metaclass=order):
    pass


class B(metaclass=order, ordered=True):
    def x():
        pass

    def g():
        pass


print(A.__dict__)
print(B.__dict__)
print(getattr(B, '__dict__'))
