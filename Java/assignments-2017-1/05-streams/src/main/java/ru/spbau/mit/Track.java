package ru.spbau.mit;

public class Track {
    private final int rating;
    private final String name;

    Track(String name, int rating) {
        this.rating = rating;
        this.name = name;
    }

    public int getRating() {
        return rating;
    }

    public String getName() {
        return name;
    }
}
