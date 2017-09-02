package ru.spbau.mit;

public abstract class Predicate<T> {
    public static final Predicate<Object> ALWAYS_TRUE = new Predicate<Object>() {
        @Override
        public boolean apply(Object obj) {
            return true;
        }
    };
    public static final Predicate<Object> ALWAYS_FALSE = new Predicate<Object>() {
        @Override
        public boolean apply(Object obj) {
            return false;
        }
    };

    public abstract boolean apply(T obj);

    public Predicate<T> not() {
        return new Predicate<T>() {
            @Override
            public boolean apply(T obj) {
                return !Predicate.this.apply(obj);
            }
        };
    }

    public Predicate<T> or(final Predicate<? super T> other) {
        return new Predicate<T>() {
            @Override
            public boolean apply(T obj) {
                boolean res = Predicate.this.apply(obj);
                return res || other.apply(obj);
            }
        };
    }

    public Predicate<T> and(final Predicate<? super T> other) {
        return new Predicate<T>() {
            @Override
            public boolean apply(T obj) {
                boolean res = Predicate.this.apply(obj);
                return res && other.apply(obj);
            }
        };
    }
}
