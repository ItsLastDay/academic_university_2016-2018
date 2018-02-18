package ru.spbau.mit;

import java.util.*;

public class HashMultiset<E> extends AbstractSet<E> implements Multiset<E> {
    private MyHashSet<EntryImpl<E>> addedElements = new MyHashSet<>();
    private LinkedHashSet<E> liveElements = new LinkedHashSet<>();
    private LinkedHashMap<E, EntryImpl<E>> counter = new LinkedHashMap<>();
    private int sz = 0;

    @Override
    public boolean add(E e) {
        EntryImpl<E> cnt = counter.get(e);
        if (cnt == null) {
            EntryImpl<E> x = new EntryImpl<>();
            x.element = e;
            x.count = 1;
            liveElements.add(e);
            addedElements.add(x);
            counter.put(e, x);
        } else {
            cnt.addCount(1);
        }

        sz++;
        return true;
    }

    @Override
    public int count(Object element) {
        EntryImpl<E> val = counter.get(element);
        if (val == null) {
            return 0;
        }
        return val.getCount();
    }

    @Override
    public Set<E> elementSet() {
        return liveElements;
    }

    @Override
    public Set<? extends Entry<E>> entrySet() {
        return addedElements;
    }

    @Override
    public Iterator<E> iterator() {
        return new MultisetIterator();
    }

    @Override
    public int size() {
        return sz;
    }

    private class MyHashSet<E> extends LinkedHashSet<E> {
        @Override
        public Iterator<E> iterator() {
            return (Iterator<E>) new EntrySetIterator();
        }

        public Iterator<E> defaultIterator() {
            return super.iterator();
        }
    }

    private class EntrySetIterator implements Iterator<EntryImpl<E>> {
        private Iterator<EntryImpl<E>> it = addedElements.defaultIterator();
        private EntryImpl<E> curElem = null;

        @Override
        public boolean hasNext() {
            return it.hasNext();
        }

        @Override
        public EntryImpl<E> next() {
            curElem = it.next();
            return curElem;
        }

        @Override
        public void remove() {
            sz -= curElem.getCount();
            curElem.addCount(-curElem.getCount());
            liveElements.remove(curElem.getElement());
            it.remove();
        }
    }

    private class EntryImpl<E> implements Entry<E> {
        private E element;
        private int count;

        void addCount(int val) {
            count += val;
        }

        @Override
        public E getElement() {
            return element;
        }

        @Override
        public int getCount() {
            return count;
        }

        public boolean equals(Object other) {
            return other instanceof EntryImpl && getElement() == ((EntryImpl) other).getElement();
        }

        public int hashCode() {
            if (element == null) {
                return 0;
            }
            return element.hashCode();
        }
    }

    private class MultisetIterator implements Iterator<E> {
        private int curCount = 0;
        private Iterator<EntryImpl<E>> it = addedElements.defaultIterator();
        private EntryImpl<E> curElem = null;

        private void upd() {
            curElem = it.next();
            curCount = 0;
        }

        @Override
        public boolean hasNext() {
            if (curElem == null) {
                if (!it.hasNext()) {
                    return false;
                }
                upd();
            }
            if (curCount == curElem.getCount()) {
                return it.hasNext();
            }

            return true;
        }

        @Override
        public E next() {
            if (curCount == curElem.getCount()) {
                upd();
            }
            E res = curElem.getElement();
            curCount++;
            return res;
        }

        @Override
        public void remove() {
            curElem.addCount(-1);
            sz--;
            if (curElem.count == 0) {
                liveElements.remove(curElem.getElement());
                it.remove();
            }
            curCount--;
        }
    }
}
