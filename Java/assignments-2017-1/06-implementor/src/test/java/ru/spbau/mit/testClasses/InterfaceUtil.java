package ru.spbau.mit.testClasses;

public interface InterfaceUtil extends Comparable<Object> {
    void bar();

    String meth(String x, String y);

    InterfaceUtil smth(InterfaceUtil x);
}
