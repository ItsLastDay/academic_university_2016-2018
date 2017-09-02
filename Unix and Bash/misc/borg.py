
class Borg(type):
    megadict = dict()

    def __call__(cls, *args, **kwargs):
        obj = super().__call__(*args, **kwargs)
        obj.__dict__ = Borg.megadict
        return obj


class A(metaclass=Borg):
    def __init__(self, **kwargs):
        for arg in kwargs:
            setattr(self, arg, kwargs[arg])


x = A()
print(x.__dict__)
x.d = 'd'

y = A(ax='avb',rty='dfg')
y.asdrwer='dfs'
print(x.__dict__)
print(y.__dict__)
print(x is y)

print(x.__dict__ is y.__dict__)


class Borg_for_classes(type, metaclass=Borg):
    pass


class B(metaclass=Borg_for_classes):
    pass

class C(metaclass=Borg_for_classes):
    def func(self, x):
        pass

x = B()
x.abc = 'xyz'
y = B()
print(B.__dict__)
print(C.__dict__)
print(B.__dict__ is C.__dict__)
