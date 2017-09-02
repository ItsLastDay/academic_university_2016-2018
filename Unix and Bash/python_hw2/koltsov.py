#! /usr/bin/env python3


# 1
def empty():
    '''
    >>> list(empty())
    []
    '''
    # http://stackoverflow.com/a/13243870/5338270
    return
    yield


# `dummy_top` is returned whenever `next` is called on an empty heap.
dummy_top = (None, 0, 0)


def reconstruct(head_tuple, heap_iter):
    """
    Given a returned tuple from a heap and an iterable of heap's children,
    construct a generator that behaves like tuple extraction did not happen.

    Special case when subject heap was empty: then
    `head_tuple` == `dummy_top`.
    """
    if head_tuple != dummy_top:
        yield head_tuple
        yield from heap_iter


# 2
def heapify(value, heap1, heap2):
    '''
    >>> list(heapify(1, empty(), empty()))[0]
    (1, 1, 1)

    >>> list(heapify(2, heapify(1, empty(), empty()), empty()))[0]
    (1, 2, 2)

    >>> heap1 = heapify(3, empty(), empty())
    >>> heap2 = heapify(1, empty(), empty())
    >>> list(list(heapify(2, heap1, heap2))[2])[0]
    (2, 1, 1)
    '''
    head_left = next(heap1, dummy_top)
    head_right = next(heap2, dummy_top)

    total_size = head_left[1] + head_right[1] + 1
    total_depth = max(head_left[2], head_right[2]) + 1

    # result[0][1] is None just for uniformity.
    result = [((value, total_size, total_depth), None),
              (head_left, heap1), (head_right, heap2)]

    idx_min = 0
    for i in range(1, 3):
        cur_val = result[i][0][0]
        if cur_val is None:
            continue
        if cur_val < result[idx_min][0][0]:
            idx_min = i

    for i in range(1, 3):
        if idx_min == i:
            minimal_value = result[i][0][0]

            # This heap is not empty, because it has an element at the top.
            left_grandson = next(result[i][1])
            right_grandson = next(result[i][1])

            result[i] = heapify(value, left_grandson, right_grandson)
            result[0] = ((minimal_value, result[0][0][1], result[0][0][2]),
                         None)
        else:
            result[i] = reconstruct(*result[i])

    result[0] = result[0][0]

    yield from result


# 3
def print_heap(heap, prefix=''):
    '''
    >>> print_heap(empty())

    >>> print_heap(heapify(1, empty(), empty()))
    [1, 1]: 1

    >>> print_heap(heapify(2, heapify(1, empty(), empty()), empty()))
    [2, 2]: 1
        [1, 1]: 2
    '''

    head_tuple = next(heap, None)
    if head_tuple is not None:
        print('{shift}[{size}, {depth}]: {value}'.format(shift=prefix,
                                                         size=head_tuple[1],
                                                         depth=head_tuple[2],
                                                         value=head_tuple[0]))
        left_son = next(heap)
        print_heap(left_son, prefix + ' ' * 4)
        right_son = next(heap)
        print_heap(right_son, prefix + ' ' * 4)


# 4
def build_heap(seq):
    '''
    >>> print_heap(build_heap([]))

    >>> print_heap(build_heap([1]))
    [1, 1]: 1

    >>> print_heap(build_heap([2, 1]))
    [2, 2]: 1
        [1, 1]: 2

    >>> print_heap(build_heap([3, 1, 2]))
    [3, 2]: 1
        [1, 1]: 3
        [1, 1]: 2

    >>> print_heap(build_heap(range(1, 4)))
    [3, 2]: 1
        [1, 1]: 2
        [1, 1]: 3

    >>> print_heap(build_heap([2, 4, 1, 3]))
    [4, 3]: 1
        [2, 2]: 3
            [1, 1]: 4
        [1, 1]: 2

    >>> print_heap(build_heap([2, 4, 1, 3, 5]))
    [5, 3]: 1
        [3, 2]: 3
            [1, 1]: 4
            [1, 1]: 5
        [1, 1]: 2
    '''

    seq = list(seq)
    size = len(seq)

    if not size:
        return empty()

    for i in range(size - 1, -1, -1):
        left_child_idx = 2 * i + 1
        right_child_idx = 2 * i + 2

        left_child = seq[left_child_idx] if left_child_idx < size else empty()
        right_child = (seq[right_child_idx] if right_child_idx < size
                       else empty())

        seq[i] = heapify(seq[i], left_child, right_child)

    return seq[0]


# 5
def extract_last(heap):
    '''
    >>> last, heap = extract_last(build_heap([]))
    >>> last
    >>> print_heap(heap)

    >>> last, heap = extract_last(build_heap([1]))
    >>> last
    1
    >>> print_heap(heap)

    >>> last, heap = extract_last(build_heap([2, 1]))
    >>> last
    2
    >>> print_heap(heap)
    [1, 1]: 1

    >>> last, heap = extract_last(build_heap([3, 1, 2]))
    >>> last
    2
    >>> print_heap(heap)
    [2, 2]: 1
        [1, 1]: 3

    >>> last, heap = extract_last(build_heap([2, 4, 1, 3, 5]))
    >>> last
    5
    >>> print_heap(heap)
    [4, 3]: 1
        [2, 2]: 3
            [1, 1]: 4
        [1, 1]: 2
    '''

    elem = next(heap, dummy_top)
    if elem[1] <= 1:
        yield elem[0]
        yield empty()
        return

    left_child = next(heap)
    right_child = next(heap)

    left_top = next(left_child, dummy_top)
    right_top = next(right_child, dummy_top)

    if left_top[2] > right_top[2]:
        last_val, converted_left = extract_last(reconstruct(left_top,
                                                            left_child))
        left_top = next(converted_left, dummy_top)
        left_child = converted_left
    else:
        last_val, converted_right = extract_last(reconstruct(right_top,
                                                             right_child))
        right_top = next(converted_right, dummy_top)
        right_child = converted_right

    left_child = reconstruct(left_top, left_child)
    right_child = reconstruct(right_top, right_child)
    cur_top = (elem[0], left_top[1] + right_top[1] + 1,
               max(left_top[2], right_top[2]) + 1)
    yield last_val
    yield reconstruct(cur_top, [left_child, right_child])


# 6
def extract_min(heap):
    '''
    >>> min_value, heap = extract_min(build_heap([]))
    >>> min_value
    >>> print_heap(heap)

    >>> min_value, heap = extract_min(build_heap([1]))
    >>> min_value
    1
    >>> print_heap(heap)

    >>> min_value, heap = extract_min(build_heap([2, 1]))
    >>> min_value
    1
    >>> print_heap(heap)
    [1, 1]: 2

    >>> min_value, heap = extract_min(build_heap([3, 1, 2]))
    >>> min_value
    1
    >>> print_heap(heap)
    [2, 2]: 2
        [1, 1]: 3

    >>> min_value, heap = extract_min(build_heap([2, 4, 1, 3, 5]))
    >>> min_value
    1
    >>> print_heap(heap)
    [4, 3]: 2
        [2, 2]: 3
            [1, 1]: 4
        [1, 1]: 5
    '''

    last_elem, converted_heap = extract_last(heap)
    top_elem = next(converted_heap, dummy_top)
    # Size is zero or one.
    if last_elem is None or top_elem == dummy_top:
        yield last_elem
        yield empty()
        return

    yield top_elem[0]
    left_child = next(converted_heap)
    right_child = next(converted_heap)
    yield heapify(last_elem, left_child, right_child)


# 7
def heap_sort(seq):
    '''
    >>> list(heap_sort([]))
    []

    >>> list(heap_sort(iter([1])))
    [1]

    >>> list(heap_sort([2, 1]))
    [1, 2]

    >>> list(heap_sort(iter([2, 1, 3])))
    [1, 2, 3]

    >>> list(heap_sort(range(7, 0, -1)))
    [1, 2, 3, 4, 5, 6, 7]
    '''

    heap = build_heap(seq)
    min_value, converted_heap = extract_min(heap)
    while min_value is not None:
        yield min_value
        min_value, converted_heap = extract_min(converted_heap)


def test_speed():
    import timeit
    setup_string = 'from __main__ import heap_sort; lst = \
            list(range(2 * 10 ** 3))'
    print('Sorting a sorted list')
    print(timeit.timeit('heap_sort(lst)', setup_string, number=5))
    print(timeit.timeit('sorted(lst)', setup_string, number=5))

    print('Sorting a reversed sorted list')
    setup_string = 'from __main__ import heap_sort; lst = \
            list(reversed(range(10 ** 3)))'
    print(timeit.timeit('heap_sort(lst)', setup_string, number=5))
    print(timeit.timeit('sorted(lst)', setup_string, number=5))


if __name__ == '__main__':
#    import doctest
#    doctest.testmod()
    print(list(heap_sort(range(10 ** 3))))
    #test_speed()
