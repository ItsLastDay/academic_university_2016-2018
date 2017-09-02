package ru.spbau.mit.testClasses;

public class ClassWithOneInterfaceDependency {
    public final Interface dependency;

    public ClassWithOneInterfaceDependency(Interface dependency) {
        this.dependency = dependency;
    }
}
