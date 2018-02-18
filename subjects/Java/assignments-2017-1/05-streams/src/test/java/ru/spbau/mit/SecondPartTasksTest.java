package ru.spbau.mit;

import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;

import static ru.spbau.mit.SecondPartTasks.*;

public class SecondPartTasksTest {
    private static Path fileFirst = Paths.get("testFindQuotesFirst");
    private static Path fileSecond = Paths.get("testFindQuotesSecond");
    private static Path fileThird = Paths.get("testFindQuotesThird");

    @BeforeClass
    public static void setUp() throws IOException {
        Files.createFile(fileFirst);
        Files.createFile(fileSecond);
        Files.createFile(fileThird);
    }

    @AfterClass
    public static void tearDown() throws IOException {
        Files.delete(fileFirst);
        Files.delete(fileSecond);
        Files.delete(fileThird);
    }

    @Test
    public void testFindQuotes() throws IOException {
        FileWriter outFirst = new FileWriter(fileFirst.toFile());
        outFirst.write("Hello this mad mad mad\n");
        outFirst.write("World without md!");
        outFirst.close();

        FileWriter outSecond = new FileWriter(fileSecond.toFile());
        outSecond.write("me mate\n");
        outSecond.write("what's up?");
        outSecond.close();

        FileWriter outThird = new FileWriter(fileThird.toFile());
        outThird.write("mad again\n");
        outThird.write("and again\n");
        outThird.write("and mad again");
        outThird.close();

        List<String> resultLines = findQuotes(Arrays.asList(fileFirst.toString(),
                fileSecond.toString(), fileThird.toString()), "mad");
        Assert.assertEquals(Arrays.asList("Hello this mad mad mad",
                "mad again", "and mad again"),
                resultLines);
    }

    @Test
    public void testPiDividedBy4() {
        Assert.assertEquals(Math.PI / 4, piDividedBy4(), 1e-3);
    }

    @Test
    public void testFindPrinter() {
        Map<String, List<String>> authorToBook = new HashMap<>();
        authorToBook.put("Pushkin", Arrays.asList("Moy dyada", "samix chestnix pravil",
                ", kogda ne v shutku" +
                        "zanemog", "on uvazhat sebya zastavil", "i luchshe vidumat ne mog"));
        authorToBook.put("Tolstoy", Arrays.asList("THIS IS VOINA I MIR",
                "THIS IS VOINA I MIR", "THIS IS VOINA I MIR", "THIS IS VOINA I MIR",
                "THIS IS VOINA I MIR", "THIS IS VOINA I MIR", "THIS IS VOINA I MIR",
                "THIS IS VOINA I MIR", "THIS IS VOINA I MIR", "THIS IS VOINA I MIR",
                "very long"));
        authorToBook.put("Sokolov-prbizhelsky vtoroy", Collections.emptyList());
        Assert.assertEquals("Tolstoy", findPrinter(authorToBook));
    }

    @Test
    public void testCalculateGlobalOrder() {
        Map<String, Integer> firstShop = new HashMap<>();
        firstShop.put("moloko", 4);
        firstShop.put("butter", 6);

        Map<String, Integer> secondShop = new HashMap<>();
        secondShop.put("moloko", 5);
        secondShop.put("bread", 1);

        Map<String, Integer> answerMerged = new HashMap<>();
        answerMerged.put("moloko", 9);
        answerMerged.put("butter", 6);
        answerMerged.put("bread", 1);

        Map<String, Integer> mergedMap = calculateGlobalOrder(Arrays.asList(firstShop, secondShop));
        Assert.assertEquals(answerMerged, mergedMap);
    }
}
