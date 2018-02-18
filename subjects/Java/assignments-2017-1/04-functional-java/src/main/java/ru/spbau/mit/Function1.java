package ru.spbau.mit;

public abstract class Function1<F, T> {
    public abstract T apply(F obj);

    public <R> Function1<F, R> compose(final Function1<? super T, R> func) {
        return new Function1<F, R>() {
            public R apply(F obj) {
                return func.apply(Function1.this.apply(obj));
            }
        };
    }
}
