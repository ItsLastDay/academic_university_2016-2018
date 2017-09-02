package ru.spbau.mit.testClasses;

import java.math.BigInteger;

abstract public class AbstractBaseUtil {
    static public void emptyMain() {

    }

    abstract public void foo(BigInteger x);

    final protected int finalMethod() {
        return 42;
    }

    private String privMeth(BigInteger t) {
        return t.toString();
    }
}
