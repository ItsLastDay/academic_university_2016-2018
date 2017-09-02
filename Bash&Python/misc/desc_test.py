class Desc:
    def __init__(self, f):
        self.f = f

    def __get__(self, instance, owner):
        print('desc', instance, owner)
        return lambda *args, **kwargs: self.f(instance, *args, **kwatgs)
        pass


class A:
    @Desc
    def f(self, x, y):
        return x + y

x = A()
x.f(4, 6)

