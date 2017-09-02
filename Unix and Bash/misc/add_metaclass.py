

def add_metaclass(metacls):
    def decor(cls):
        return metacls(cls.__name__, cls.__bases__, cls.__dict__.copy())

    return decor
    

class Meta(type):
    def __init__(cls, name, bases, dct, **kwargs):
        print('hello', name)
        super().__init__(name, bases, dct)


@add_metaclass(Meta)
class A(object):
    pass


print(type(A))
print(A.__bases__)
