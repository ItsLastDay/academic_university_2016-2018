import time
from functools import wraps


def spy(f):
    times_and_params = []

    @wraps(f)
    def g(*args, **kwargs):
        tm = time.time()
        called_args = tuple(args) + tuple(kwargs.items())
        times_and_params.append((tm, called_args))
        return f(*args, **kwargs)

    g.times_and_params = times_and_params
    return g


def bond(func):
    if not hasattr(func, 'times_and_params'):
        raise ValueError('Function must me decorated with @spy.')

    for tm, attrs in func.times_and_params:
        yield tm, attrs


@spy
def foo(num, tt=5, arg=5):
    print(num)

def g(x):
    return x + 5;

foo(30)
foo('hello')
time.sleep(1)
foo(5, arg=345, tt=234)

print(list(bond(foo)))
for (tm, params) in bond(foo):
    print(tm)
    print(params)
