package ru.spbau.mit;


import com.google.common.collect.ImmutableMap;
import org.junit.Test;

import java.util.Arrays;
import java.util.Collections;
import java.util.Optional;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;

import static junitx.framework.Assert.assertEquals;
import static ru.spbau.mit.FirstPartTasks.*;

public class FirstPartTasksTest {
    @Test
    public void testAllNames() {
        assertEquals(
                Arrays.asList("Sun Structures", "Keep In The Dark"),
                allNames(Stream.of(ALBUM_0, ALBUM_1)));

        assertEquals(
                Collections.emptyList(),
                allNames(Stream.of()));
    }

    @Test
    public void testAllNamesSorted() {
        assertEquals(
                Arrays.asList("Keep In The Dark", "Sun Structures"),
                allNamesSorted(Stream.of(ALBUM_0, ALBUM_1)));

        assertEquals(
                Collections.emptyList(),
                allNamesSorted(Stream.of()));
    }

    @Test
    public void testAllTracksSorted() {
        assertEquals(
                Arrays.asList(
                        "A Question Isn't Answered", "Colours to Life", "Fragment's Light",
                        "Jewel of Mine Eye", "Keep in the Dark", "Keep in the Dark", "Mesmerise",
                        "Move With The Season", "Sand Dance", "Shelter Song", "Sun Structures",
                        "Test of Time", "The Golden Throne", "The Guesser"),
                allTracksSorted(Stream.of(ALBUM_0, ALBUM_1)));

        assertEquals(
                Collections.emptyList(),
                allTracksSorted(Stream.of()));
    }

    @Test
    public void testSortedFavorites() {
        assertEquals(
                Arrays.asList(ALBUM_13, ALBUM_18, ALBUM_10, ALBUM_5, ALBUM_12, ALBUM_4),
                sortedFavorites(Stream.of(ALL_ALBUMS)));

        assertEquals(
                Collections.emptyList(),
                sortedFavorites(Stream.of()));
    }

    @Test
    public void testGroupByArtist() {
        assertEquals(
                ImmutableMap.of(
                        ARTIST_0, Arrays.asList(ALBUM_2, ALBUM_3),
                        ARTIST_3, Arrays.asList(ALBUM_9, ALBUM_10),
                        ARTIST_7, Arrays.asList(ALBUM_17, ALBUM_19)),
                groupByArtist(Stream.of(ALBUM_2, ALBUM_9, ALBUM_3, ALBUM_10, ALBUM_17, ALBUM_19)));

        assertEquals(
                Collections.emptyMap(),
                groupByArtist(Stream.of()));
    }

    @Test
    public void testGroupByArtistMapName() {
        assertEquals(
                ImmutableMap.of(
                        ARTIST_0, Arrays.asList(ALBUM_2.getName(), ALBUM_3.getName()),
                        ARTIST_3, Arrays.asList(ALBUM_9.getName(), ALBUM_10.getName()),
                        ARTIST_7, Arrays.asList(ALBUM_17.getName(), ALBUM_19.getName())),
                groupByArtistMapName(Stream.of(ALBUM_2, ALBUM_9, ALBUM_3, ALBUM_10, ALBUM_17, ALBUM_19)));

        assertEquals(
                Collections.emptyMap(),
                groupByArtistMapName(Stream.of()));
    }

    @Test
    public void testCountAlbumDuplicates() {
        assertEquals(
                0,
                countAlbumDuplicates(Stream.of(ALBUM_0, ALBUM_1, ALBUM_4)));

        assertEquals(
                0,
                countAlbumDuplicates(Stream.of()));

        assertEquals(
                3,
                countAlbumDuplicates(Stream.of(ALBUM_0, ALBUM_1, ALBUM_2, ALBUM_3, ALBUM_4, ALBUM_5, ALBUM_6,
                        ALBUM_7, ALBUM_8, ALBUM_9, ALBUM_10, ALBUM_11, ALBUM_12, ALBUM_13, ALBUM_14, ALBUM_15,
                        ALBUM_16, ALBUM_17, ALBUM_4, ALBUM_1, ALBUM_3, ALBUM_18, ALBUM_19)));
    }

    @Test
    public void testMinMaxRating() {
        assertEquals(
                Optional.of(ALBUM_9),
                minMaxRating(Stream.of(ALBUM_2, ALBUM_9, ALBUM_3, ALBUM_10, ALBUM_17, ALBUM_19)));

        assertEquals(
                Optional.empty(),
                minMaxRating(Stream.of()));
    }

    @Test
    public void testSortByAverageRating() {
        assertEquals(
                Arrays.asList(ALBUM_10, ALBUM_3, ALBUM_2, ALBUM_9),
                sortByAverageRating(Stream.of(ALBUM_2, ALBUM_9, ALBUM_3, ALBUM_10)));

        assertEquals(
                Collections.emptyList(),
                sortByAverageRating(Stream.of()));
    }

    @Test
    public void testModuloProduction() {
        assertEquals(
                2,
                moduloProduction(IntStream.of(1, 3, 5, 7, 2), 4));

        assertEquals(
                720,
                moduloProduction(IntStream.of(1, 2, 3, 4, 5, 6), 10000));
    }

    @Test
    public void testJoinTo() {
        assertEquals(
                "<abc, cde>",
                joinTo("abc", "cde"));

        assertEquals(
                "<abc>",
                joinTo("abc"));

        assertEquals(
                "<>",
                joinTo());
    }

    @Test
    public void testFilterIsInstance() {
        assertEquals(
                Arrays.asList("", "cde"),
                filterIsInstance(Stream.of("", 1, 2.0, "cde"), CharSequence.class).collect(Collectors.toList()));

        assertEquals(
                Collections.emptyList(),
                filterIsInstance(Stream.of("", 1, 2.0, "cde"), Void.class).collect(Collectors.toList()));

        assertEquals(
                Arrays.asList("", 1, 2.0, "cde"),
                filterIsInstance(Stream.of("", 1, 2.0, "cde"), Object.class).collect(Collectors.toList()));
    }

    // CHECKSTYLE:OFF
    private static final Artist ARTIST_0 = new Artist("Morcheeba");
    private static final Artist ARTIST_1 = new Artist("Temples");
    private static final Artist ARTIST_2 = new Artist("God Help the Girl");
    private static final Artist ARTIST_3 = new Artist("All India Radio");
    private static final Artist ARTIST_4 = new Artist("UNKLE");
    private static final Artist ARTIST_5 = new Artist("Bonobo");
    private static final Artist ARTIST_6 = new Artist("Grimes");
    private static final Artist ARTIST_7 = new Artist("Massive Attack");
    private static final Album ALBUM_0 = new Album(ARTIST_0, "Sun Structures", new Track("Shelter Song", 85), new Track("Sun Structures", 53), new Track("The Golden Throne", 72), new Track("Keep in the Dark", 49), new Track("Mesmerise", 85), new Track("Move With The Season", 16), new Track("Colours to Life", 37), new Track("A Question Isn't Answered", 50), new Track("The Guesser", 53), new Track("Test of Time", 92), new Track("Sand Dance", 21), new Track("Fragment's Light", 61));
    private static final Album ALBUM_1 = new Album(ARTIST_0, "Keep In The Dark", new Track("Keep in the Dark", 89), new Track("Jewel of Mine Eye", 83));
    private static final Album ALBUM_2 = new Album(ARTIST_0, "Big Calm", new Track("The Sea", 72), new Track("Shoulder Holster", 26), new Track("Part of the Process", 31), new Track("Blindfold", 84), new Track("Let Me See", 55), new Track("Bullet Proof", 24), new Track("Over and Over", 1), new Track("Friction", 56), new Track("diggin' in a watery grave", 53), new Track("Fear and Love", 5), new Track("Big Calm", 37));
    private static final Album ALBUM_3 = new Album(ARTIST_0, "Charango", new Track("Slow Down", 0), new Track("Otherwise", 36), new Track("Aqualung", 82), new Track("Sao Paulo", 44), new Track("Charango (Feat: Pace Won)", 20), new Track("What New York Couples Fight About (Feat: Kurt Wagner)", 50), new Track("Undress Me Now", 26), new Track("Way Beyond", 4), new Track("Women Lose Weight (Feat: Slick Rick)", 76), new Track("Get Along (Feat: Pace Won)", 88), new Track("Public Displays of Affection", 15), new Track("The Great London Traffic Warden Massacre", 43), new Track("Slow Down (Instrumental)", 94), new Track("Otherwise (Instrumental)", 52), new Track("Aqualung (Instrumental)", 19), new Track("Sao Paulo (Instrumental)", 21), new Track("Charango (Feat: Pace Won) (Instrumental)", 32), new Track("What New York Couples Fight About (Instrumental)", 47), new Track("Undress Me Now (Instrumental)", 55), new Track("Way Beyond (Instrumental)", 81), new Track("Women Lose Weight (Feat: Slick Rick) (Instrumental)", 77), new Track("Get Along (Feat: Pace Won) (Instrumental)", 76), new Track("Public Displays Of Affection (Instrumental)", 35), new Track("The Great London Traffic Warden Massacre (Instrumental)", 58));
    private static final Album ALBUM_4 = new Album(ARTIST_1, "Shelter Song", new Track("Shelter Song", 99), new Track("Prisms", 87));
    private static final Album ALBUM_5 = new Album(ARTIST_2, "God Help The Girl", new Track("I Suppose That Was A Prayer", 30), new Track("Act of the Apostle", 33), new Track("I Dumped You First", 52), new Track("Pretty When The Wind Blows", 75), new Track("I Know I Have To Eat", 53), new Track("God Help the Girl", 34), new Track("The Psychiatrist Is In", 35), new Track("The God Of Music", 86), new Track("If You Could Speak", 48), new Track("The Catwalk Of The Dukes", 42), new Track("Perfection as a Hipster", 43), new Track("Fuck This Shit", 50), new Track("Pretty Eve in the Tub", 14), new Track("A loving Kind Of Boy", 36), new Track("What Do You Want This Band To Sound Like", 43), new Track("Come Monday Night", 50), new Track("Collective Idiocy", 68), new Track("I'm Not Rich", 90), new Track("I'll Have to Dance With Cassie", 6), new Track("Stalinist Russia", 14), new Track("Baby’s Just Waiting", 50), new Track("Partick Whistle", 54), new Track("Musician, Please Take Heed", 33), new Track("I Just Want Your Jeans", 21), new Track("Invisible", 72), new Track("The World's Last Cassette", 98), new Track("A Down and Dusky Blonde", 51), new Track("Dress Up In You", 17));
    private static final Album ALBUM_6 = new Album(ARTIST_2, "God Help The Girl sd (Original Motion Picture Soundtrack)");
    private static final Album ALBUM_7 = new Album(ARTIST_2, "Stills EP");
    private static final Album ALBUM_8 = new Album(ARTIST_3, "Echo Other", new Track("Tropic of Unicorn", 18), new Track("Four Three", 86), new Track("The Time", 36), new Track("Sunshine Briefly", 33), new Track("Elizabethland", 41), new Track("The Quiet Ambient", 21), new Track("Song of the See", 95), new Track("Mexicola", 36), new Track("Ghost Dirt", 49), new Track("Whistle", 2), new Track("Echo Other", 13), new Track("Endless Highway", 89));
    private static final Album ALBUM_9 = new Album(ARTIST_3, "Permanent Evolutions", new Track("Open Sky Experiment (St-244 Remix)", 11), new Track("Permanent Revolutions (Don Meers remix)", 14), new Track("Little Mexico", 2), new Track("How Many, For How Long (Morphodic Bliss Mix)", 59), new Track("Dark Ambient (am mix)", 53), new Track("Life and How to Do It", 19), new Track("For Angel (All India Radio vs. Don Meers Mix)", 24), new Track("Lo Fi Groovy", 67), new Track("Walking On A.I.R.", 47), new Track("Delhi Dub", 64), new Track("Pray To The TV Funk (Left Brain Mix)", 69), new Track("A Moment (TV Version)", 1), new Track("Old India", 4), new Track("The Long Goodbye", 79));
    private static final Album ALBUM_10 = new Album(ARTIST_3, "Film Musik", new Track("The Quiet Ambient", 7), new Track("Last Port Of Call", 17), new Track("Permanent Revolutions", 84), new Track("Sunshine Briefly", 76), new Track("Mexicola", 86), new Track("Evening Star", 97), new Track("Horse Groove", 30), new Track("Waukaringa", 95), new Track("Deady Boy", 67), new Track("The Time", 57), new Track("Doing Stuff", 70), new Track("Far Away (Instrumental)", 37), new Track("Harmonium Across The Sea", 34), new Track("Dry (film mix)", 22), new Track("Gunslinger 3", 29), new Track("Its Happy Time", 35));
    private static final Album ALBUM_11 = new Album(ARTIST_4, "Psyence Fiction", new Track("Guns Blazing (Drums Of Death Pt. 1)", 17), new Track("U.N.K.L.E. Main Title Theme", 80), new Track("Lonely Soul", 51), new Track("Getting Ahead In The Lucrative Field Of Artist Management", 49), new Track("Nursery Rhyme/ Breather", 9), new Track("Celestial Annihilation", 36), new Track("The Knock (Drums Of Death Pt. 2)", 13), new Track("Chaos", 51), new Track("Rabbit in Your Headlights", 49), new Track("Outro (Mandatory)", 21));
    private static final Album ALBUM_12 = new Album(ARTIST_4, "Never, Never, Land", new Track("Back And Forth", 79), new Track("Eye for an Eye", 59), new Track("In A State", 47), new Track("Safe In Mind (Please Get This Gun From Out My Face)", 99), new Track("I Need Something Stronger", 39), new Track("What Are You To Me?", 14), new Track("Panic Attack", 88), new Track("Invasion", 49), new Track("Reign", 91), new Track("Glow", 87), new Track("Inside", 76));
    private static final Album ALBUM_13 = new Album(ARTIST_5, "Black Sands", new Track("Prelude", 43), new Track("Kiara", 29), new Track("Kong", 0), new Track("Eyesdown", 97), new Track("El Toro", 98), new Track("We Could Forever", 18), new Track("1009", 27), new Track("All in Forms", 20), new Track("The Keeper", 90), new Track("Stay the Same", 88), new Track("Animals", 49), new Track("Black Sands", 14));
    private static final Album ALBUM_14 = new Album(ARTIST_5, "Days to Come", new Track("Intro", 68), new Track("Days To Come", 41), new Track("Between The Lines", 79), new Track("The Fever", 81), new Track("Ketto", 6), new Track("Nightlite feat. Bajka", 32), new Track("Transmission 94 (Parts 1 & 2)", 19), new Track("On Your Marks", 3), new Track("If You Stayed Over", 7), new Track("Walk In The Sky", 74), new Track("Recurring", 32));
    private static final Album ALBUM_15 = new Album(ARTIST_6, "Visions", new Track("Infinite ❤ Without Fulfillment", 37), new Track("Genesis", 39), new Track("Oblivion", 26), new Track("Eight", 65), new Track("Circumambient", 37), new Track("Vowels = Space and Time", 20), new Track("Visiting Statue", 54), new Track("Be a Body (侘寂)", 22), new Track("Colour Of Moonlight (Antiochus) (Ft. Doldrums)", 36), new Track("Symphonia IX (My Wait Is U)", 78), new Track("Nightmusic (Ft. Majical Cloudz)", 8), new Track("Skin", 44), new Track("Know the Way", 72));
    private static final Album ALBUM_16 = new Album(ARTIST_6, "Art Angels");
    private static final Album ALBUM_17 = new Album(ARTIST_7, "Mezzanine", new Track("Angel", 75), new Track("Risingson", 26), new Track("Teardrop", 93), new Track("Inertia Creeps", 65), new Track("Exchange", 76), new Track("Dissolved Girl", 87), new Track("Man Next Door", 50), new Track("Black Milk", 45), new Track("Mezzanine", 5), new Track("Group Four", 95), new Track("(Exchange)", 89));
    private static final Album ALBUM_18 = new Album(ARTIST_7, "Blue Lines", new Track("Safe From Harm", 99), new Track("One Love", 71), new Track("Blue Lines", 89), new Track("Be Thankful for What You've Got", 20), new Track("Five Man Army", 10), new Track("Unfinished Sympathy", 58), new Track("Daydreaming", 60), new Track("Lately", 27), new Track("Hymn Of The Big Wheel", 27));
    private static final Album ALBUM_19 = new Album(ARTIST_7, "Heligoland", new Track("Pray for Rain", 61), new Track("Babel", 26), new Track("Splitting the Atom", 37), new Track("Girl I Love You", 2), new Track("Psyche", 37), new Track("Flat of the Blade", 74), new Track("Paradise Circus", 11), new Track("Rush Minute", 53), new Track("Saturday Come Slow", 91), new Track("Atlas Air (Instrumental)", 41), new Track("Paradise Circus (Gui Boratto Remix)", 69), new Track("Fatalism (Ryuichi Sakamoto & Yukihiro Takahashi Remix)", 24), new Track("Girl I Love You (She is Danger Remix)", 7), new Track("Paradise Circus (Breakage's Tight Rope Remix)", 92));
    private static final Album[] ALL_ALBUMS = new Album[]{ALBUM_0, ALBUM_1, ALBUM_2, ALBUM_3, ALBUM_4, ALBUM_5, ALBUM_6,
            ALBUM_7, ALBUM_8, ALBUM_9, ALBUM_10, ALBUM_11, ALBUM_12, ALBUM_13, ALBUM_14, ALBUM_15, ALBUM_16, ALBUM_17,
            ALBUM_18, ALBUM_19
    };
    // CHECKSTYLE:ON
}
