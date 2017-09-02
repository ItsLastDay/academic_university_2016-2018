from functools import wraps

def singleton(cls):
    @wraps(cls.__init__)
    def myinit(self, *args, **kwargs):
        if not hasattr(cls, 'initialized_singleton_value'):
            cls.saved_init_function(self, *args, **kwargs)
            cls.initialized_singleton_value = True

    @wraps(cls.__new__)
    def mynew(self, *args, **kwargs):
        if not hasattr(cls, 'saved_singleton_instance'):
            cls.saved_singleton_instance = cls.saved_new_function(self, *args, **kwargs)

        return cls.saved_singleton_instance

    cls.saved_init_function = cls.__init__
    cls.__init__ = myinit
    cls.saved_new_function = cls.__new__
    cls.__new__ = mynew
    return cls


@singleton
class A:
    def __init__(self):
        self.x = 5

x = A()
y = A()
print(dir(A))
print(x is y)
print(x is A.saved_singleton_instance)
print(x.x)
