package ru.spbau.mit;

import java.util.Arrays;
import java.util.List;

public class Album {
    private String name;
    private final List<Track> tracks;
    private final Artist artist;

    public Album(Artist artist, String name, Track... tracks) {
        this.name = name;
        this.tracks = Arrays.asList(tracks);
        this.artist = artist;
    }

    public String getName() {
        return name;
    }

    public List<Track> getTracks() {
        return tracks;
    }

    public Artist getArtist() {
        return artist;
    }

    @Override
    public String toString() {
        return name;
    }
}
