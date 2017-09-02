package ru.spbau.mit;

public abstract class Function2<F, S, T> {
    public abstract T apply(F firstArg, S secondArg);

    public <R> Function2<F, S, R> compose(final Function1<? super T, ? extends R> func) {
        return new Function2<F, S, R>() {
            public R apply(F firstArg, S secondArg) {
                return func.apply(Function2.this.apply(firstArg, secondArg));
            }
        };
    }

    public Function1<S, T> bind1(final F firstArg) {
        return new Function1<S, T>() {
            @Override
            public T apply(S secondArg) {
                return Function2.this.apply(firstArg, secondArg);
            }
        };
    }

    public Function1<F, T> bind2(final S secondArg) {
        return new Function1<F, T>() {
            @Override
            public T apply(F firstArg) {
                return Function2.this.apply(firstArg, secondArg);
            }
        };
    }

    public Function1<F, Function1<S, T>> curry() {
        return new Function1<F, Function1<S, T>>() {
            @Override
            public Function1<S, T> apply(F firstArg) {
                return Function2.this.bind1(firstArg);
            }
        };
    }
}
