
def with_metaclass(meta, *bases):
    return meta('with_metaclass', bases, {})

class Meta(type):
    def __init__(cls, name, bases, dct, **kwargs):
        print('hello', name)
        super().__init__(name, bases, dct)

class Base(object):
    pass

class A(with_metaclass(Meta, Base)):
    pass

print(A.__bases__)
