package ru.spbau.mit;

public class Artist {
    private final String name;

    public Artist(String name) {
        this.name = name;
    }

    @Override
    public String toString() {
        return name;
    }
}
