
class DictShower(type):
    def __init__(cls, name, bases, dct):
        super().__init__(name, bases, dct)
        print(dct)


class A(metaclass=DictShower):
    def f(self):
        return 123

    def g():
        pass

    @staticmethod
    def st():
        return 5


