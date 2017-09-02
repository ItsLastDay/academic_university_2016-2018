package ru.spbau.mit;

import java.util.*;

/**
 * A collection that supports order-independent equality, like {@link Set}, but
 * may have duplicate elements
 */
public interface Multiset<E> extends Collection<E> {
    /**
     * Returns the number of occurrences of an element in this multiset
     * Expected complexity: Same as `contains`
     */
    int count(Object element);

    /**
     * Returns the set of distinct elements contained in this multiset.
     * Expected complexity: O(1)
     */
    Set<E> elementSet();

    /**
     * @return a read-only set of entries representing the data of this multiset
     * Expected complexity: O(1)
     */
    Set<? extends Entry<E>> entrySet();

    interface Entry<E> {
        E getElement();
        int getCount();
    }

    /**
     * Elements that occur multiple times in the multiset will appear multiple times in this iterator
     * Expected complexity: O(1)
     */
    @Override
    Iterator<E> iterator();
}
