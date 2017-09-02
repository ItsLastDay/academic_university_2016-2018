package ru.spbau.mit;

import java.io.IOException;
import java.nio.file.Paths;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import static java.nio.file.Files.readAllLines;

public final class SecondPartTasks {

    private SecondPartTasks() {}

    // Найти строки из переданных файлов, в которых встречается указанная подстрока.
    public static List<String> findQuotes(List<String> paths, CharSequence sequence) {
        return paths.stream().flatMap((String path) -> {
            try {
                return readAllLines(Paths.get(path)).stream();
            } catch (IOException e) {
                e.printStackTrace();
            }
            return Stream.empty();
        }).filter((String s) -> s.contains(sequence)).collect(Collectors.toList());
    }

    // В квадрат с длиной стороны 1 вписана мишень.
    // Стрелок атакует мишень и каждый раз попадает в произвольную точку квадрата.
    // Надо промоделировать этот процесс с помощью класса java.util.Random и посчитать,
    // какова вероятность попасть в мишень.
    public static double piDividedBy4() {
        Random rnd = new Random();
        // Point (x, y) lies within target if
        // (x - 0.5)^2 + (y - 0.5)^2 <= 0.25
        final int doublesLength = 10050000;
        final double targetRadius = 0.5;
        return rnd.doubles(doublesLength).map(x -> Math.pow(x - targetRadius, 2))
                .map(x -> Math.pow(rnd.nextDouble() - targetRadius, 2) + x)
                .filter(s -> s <= Math.pow(targetRadius, 2)).count() / (double) doublesLength;
    }

    // Дано отображение из имени автора в список с содержанием его произведений.
    // Надо вычислить, чья общая длина произведений наибольшая.
    public static String findPrinter(Map<String, List<String>> compositions) {
        return compositions.entrySet().stream().max(Comparator.comparing(entry ->
                entry.getValue().stream().collect(Collectors.summarizingInt(String::length)).getSum()))
                .orElseThrow(RuntimeException::new).getKey();
    }

    // Вы крупный поставщик продуктов. Каждая торговая сеть делает вам заказ в виде Map<Товар, Количество>.
    // Необходимо вычислить, какой товар и в каком количестве надо поставить.
    public static Map<String, Integer> calculateGlobalOrder(List<Map<String, Integer>> orders) {
        return orders.stream().flatMap(map -> map.entrySet().stream())
                .collect(Collectors.groupingBy(Map.Entry::getKey, Collectors.summingInt(Map.Entry::getValue)));
    }
}
