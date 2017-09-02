package ru.spbau.mit;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

public final class Collections {
    private Collections() {
    }

    public static <A, R> Collection<R> map(Function1<? super A, ? extends R> func,
                                           Collection<? extends A> inpCol) {
        Collection<R> mappedCol = new ArrayList<>();
        for (A obj : inpCol) {
            mappedCol.add(func.apply(obj));
        }
        return mappedCol;
    }

    public static <T> Collection<T> filter(Predicate<? super T> pred, Collection<? extends T> inpCol) {
        Collection<T> filteredCol = new ArrayList<>();
        for (T obj : inpCol) {
            if (pred.apply(obj)) {
                filteredCol.add(obj);
            }
        }
        return filteredCol;
    }

    public static <T> Collection<T> takeWhile(Predicate<? super T> pred, Collection<? extends T> inpCol) {
        Collection<T> prefix = new ArrayList<>();
        for (T obj : inpCol) {
            if (!pred.apply(obj)) {
                break;
            }
            prefix.add(obj);
        }
        return prefix;
    }

    public static <T> Collection<T> takeUnless(Predicate<? super T> pred, Collection<? extends T> inpCol) {
        return takeWhile(pred.not(), inpCol);
    }

    public static <T, R> R foldr(Function2<? super T, ? super R, ? extends R> func,
                                 R init, Collection<? extends T> inpCol) {
        List<T> values = new ArrayList<>();
        values.addAll(inpCol);
        java.util.Collections.reverse(values);
        for (T obj : values) {
            init = func.apply(obj, init);
        }
        return init;
    }

    public static <T, R> R foldl(Function2<? super R, ? super T, ? extends R> func,
                                 R init, Collection<? extends T> inpCol) {
        for (T obj : inpCol) {
            init = func.apply(init, obj);
        }
        return init;
    }
}
