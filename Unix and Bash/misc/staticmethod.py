
from functools import wraps

class staticmethod:
    def __init__(self, f):
        self.f = f

    def __get__(self, instance, owner):
        return self.f

class classmethod:
    def __init__(self, f):
        self.f = f

    def __get__(self, instance, owner):
        @wraps(self.f)
        def g(*args, **kwargs):
            return self.f(owner, *args, **kwargs)

        return g


class A:
    @staticmethod
    def func(x):
        return x + 5

    @classmethod
    def foo(cls, x):
        print(cls)
        return x * 5


x = A()
print(A.func(123))
print(x.func(123))
print(A.foo(123))
print(x.foo(123))
