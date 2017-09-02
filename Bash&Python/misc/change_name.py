import types

def change_name(name):
    def decor(f):
        globals()[name] = f
        return f

    return decor


@change_name('myfunc')
def func(x):
    return x + 5

print(globals())
print(myfunc(123))
