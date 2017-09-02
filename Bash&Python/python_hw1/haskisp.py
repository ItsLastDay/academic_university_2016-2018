#! /usr/bin/env python3

from collections import namedtuple


Nil = namedtuple('Nil', [])
Cons = namedtuple('Cons', ['car', 'cdr'])


def null(a):
    '''
    >>> null(Nil())
    True
    >>> null(Cons(0, Nil()))
    False
    '''
    return a == Nil()


def fromseq(seq):
    '''
    >>> fromseq([])
    Nil()
    >>> fromseq(tuple())
    Nil()
    >>> fromseq([1, 2, 3])
    Cons(car=1, cdr=Cons(car=2, cdr=Cons(car=3, cdr=Nil())))
    '''
    if not seq:
        return Nil()
    return Cons(car=seq[0], cdr=fromseq(seq[1:]))


def head(a):
    '''
    >>> head(fromseq([1, 2, 3]))
    1
    >>> head(Nil())
    Traceback (most recent call last):
    ...
    AttributeError: 'Nil' object has no attribute 'car'
    '''
    return a.car


def tail(a):
    '''
    >>> tail(fromseq([1, 2, 3]))
    Cons(car=2, cdr=Cons(car=3, cdr=Nil()))
    >>> tail(fromseq([]))
    Traceback (most recent call last):
    ...
    AttributeError: 'Nil' object has no attribute 'cdr'
    '''
    return a.cdr


def foldr(f, i, a):
    '''
    >>> foldr(lambda x, y: x + y, 0, Nil())
    0
    >>> foldr(lambda x, y: x + y, 2, fromseq([1, 2, 3]))
    8
    >>> foldr(lambda x, y: x - y, 1, fromseq([3, 2, 1]))
    1
    '''
    if null(a):
        return i
    return f(head(a), foldr(f, i, tail(a)))


def foldl(f, i, a):
    '''
    >>> foldl(lambda x, y: x + y, 0, Nil())
    0
    >>> foldl(lambda x, y: x + y, 2, fromseq([1, 2, 3]))
    8
    >>> foldl(lambda x, y: x - y, 1, fromseq([3, 2, 1]))
    -5
    '''
    if null(a):
        return i
    return foldl(f, f(i, head(a)), tail(a))


def length(a):
    '''
    >>> length(Nil())
    0
    >>> length(fromseq((1, 2)))
    2
    '''
    if null(a):
        return 0
    return 1 + length(tail(a))


def tolist(a):
    '''
    >>> tolist(Nil())
    []
    >>> tolist(Cons(1, Nil()))
    [1]
    >>> tolist(fromseq([1, 2, 3]))
    [1, 2, 3]
    '''
    if null(a):
        return []
    return [a.car] + tolist(a.cdr)


def map_(f, a):
    '''
    >>> tolist(map_(lambda x: x, Nil()))
    []
    >>> tolist(map_(lambda x: x, fromseq([1, 2, 3])))
    [1, 2, 3]
    >>> tolist(map_(lambda x: str(x) + '0', fromseq([1, 2, 3])))
    ['10', '20', '30']
    '''
    if null(a):
        return a
    return Cons(car=f(head(a)), cdr=map_(f, tail(a)))


def append(a, b):
    '''
    >>> append(Nil(), fromseq([]))
    Nil()
    >>> append(Nil(), Cons(0, Cons(1, Nil())))
    Cons(car=0, cdr=Cons(car=1, cdr=Nil()))
    >>> append(fromseq([1]), Nil())
    Cons(car=1, cdr=Nil())
    >>> append(fromseq([1, 2]), fromseq([3]))
    Cons(car=1, cdr=Cons(car=2, cdr=Cons(car=3, cdr=Nil())))
    '''
    if null(a):
        return b
    return Cons(car=head(a), cdr=append(tail(a), b))


def filter_(p, a):
    '''
    >>> filter_(lambda x: True, Nil())
    Nil()
    >>> tolist(filter_(lambda x: True, fromseq([1, 2])))
    [1, 2]
    >>> tolist(filter_(lambda x: False, fromseq([1, 2])))
    []
    >>> tolist(filter_(lambda x: x % 2 == 0, fromseq(range(5))))
    [0, 2, 4]
    '''
    if null(a):
        return a
    if not p(head(a)):
        return filter_(p, tail(a))
    return Cons(car=head(a), cdr=filter_(p, tail(a)))


def reverse(a):
    '''
    >>> reverse(Nil())
    Nil()
    >>> tolist(reverse(fromseq(range(2))))
    [1, 0]
    >>> tolist(reverse(fromseq(range(3))))
    [2, 1, 0]
    '''
    if null(a):
        return a
    return append(reverse(tail(a)), Cons(head(a), Nil()))


def elem(e, a):
    '''
    >>> elem(10, Nil())
    False
    >>> elem(5, fromseq(range(5)))
    False
    >>> elem(5, fromseq(range(10)))
    True
    '''
    if null(a):
        return False
    return e == head(a) or elem(e, tail(a))


if __name__ == '__main__':
    import doctest
    doctest.testmod()
