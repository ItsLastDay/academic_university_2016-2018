from collections import defaultdict
from functools import wraps


class SameField(type):
    func_dict = defaultdict(list)

    @classmethod
    def __prepare__(mcls, name, bases):
        d = dict()
        d['func_dict'] = SameField.func_dict
        return d


class apply(metaclass = SameField):
    def __init__(self, func, test_func):
        self.test_func = test_func
        self.func = func

    def __call__(self, f):
        self.func_dict[self.func.__name__].append((self.test_func, f))

        @wraps(self.func)
        def g(*args, **kwargs):
            for test_func, func in self.func_dict[self.func.__name__]:
                if test_func(*args, **kwargs):
                    return func(*args, **kwargs)

            return self.func(*args, **kwargs)

        # Function `self.func` should be already in global
        # namespace, because `apply` has it as a parameter.
        globals()[self.func.__name__] = g
        return f 
        
        
def test1(num):
	return ((num > 0) and (num % 2 == 1))
def tmp(num):
	return (num >= 3)


def foo(num):
	print('Original')


@apply (foo, tmp)
def bbb(num):
	print('Magic')


@apply (foo, test1)
def asd(num):
	print('Modified')
	
	
foo(-1)
foo(1)
foo(2)
foo(3)
