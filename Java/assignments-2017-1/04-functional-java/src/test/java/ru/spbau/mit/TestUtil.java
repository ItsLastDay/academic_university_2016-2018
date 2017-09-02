package ru.spbau.mit;

import java.util.Arrays;
import java.util.Collection;
import java.util.List;

import static org.junit.Assert.fail;

final class TestUtil {
    private static Function1<Integer, Integer> funNegate = new Function1<Integer, Integer>() {
        @Override
        public Integer apply(Integer obj) {
            return -obj;
        }
    };
    private static Function1<Integer, Integer> funAddOne = new Function1<Integer, Integer>() {
        @Override
        public Integer apply(Integer obj) {
            return obj + 1;
        }
    };
    private static Function2<Integer, Integer, Integer> funSubtract = new Function2<Integer, Integer, Integer>() {
        @Override
        public Integer apply(Integer firstArg, Integer secondArg) {
            return firstArg - secondArg;
        }
    };
    private static Function2<Integer, Integer, Integer> funMultiply = new Function2<Integer, Integer, Integer>() {
        @Override
        public Integer apply(Integer firstArg, Integer secondArg) {
            return firstArg * secondArg;
        }
    };
    private static Predicate<Object> predThrower = new Predicate<Object>() {
        @Override
        public boolean apply(Object obj) {
            fail("Called predicated that should be unreachable");
            return false;
        }
    };
    private static Predicate<Integer> predOdd = new Predicate<Integer>() {
        @Override
        public boolean apply(Integer obj) {
            return obj % 2 == 1;
        }
    };
    private static Predicate<Number> predValGreaterTen = new Predicate<Number>() {
        @Override
        public boolean apply(Number obj) {
            return obj.intValue() > 10;
        }
    };
    private static Collection<String> ziminStrings = Arrays.asList(
            "a",
            "aba",
            "abacaba",
            "abacabadabacaba",
            "abacabadabacabaeabacabadabacaba");
    private static List<Integer> ziminLengths = Arrays.asList(1, 3, 7, 15, 31);
    private static Function1<String, Integer> funLength = new Function1<String, Integer>() {
        @Override
        public Integer apply(String obj) {
            return obj.length();
        }
    };
    private static Function1<String, String> funUppercase = new Function1<String, String>() {
        @Override
        public String apply(String obj) {
            return obj.toUpperCase();
        }
    };
    private static Function1<String, Integer> funToUppercaseLength = getFunUppercase().compose(getFunLength());
    private static Predicate<CharSequence> predLengthGreaterThree = new Predicate<CharSequence>() {
        @Override
        public boolean apply(CharSequence obj) {
            return obj.length() > 3;
        }
    };

    private TestUtil() {
    }

    static Function1<Integer, Integer> getFunNegate() {
        return funNegate;
    }

    static Function1<Integer, Integer> getFunAddOne() {
        return funAddOne;
    }

    static Function2<Integer, Integer, Integer> getFunSubtract() {
        return funSubtract;
    }

    static Function2<Integer, Integer, Integer> getFunMultiply() {
        return funMultiply;
    }

    static Predicate<Object> getPredThrower() {
        return predThrower;
    }

    static Predicate<Integer> getPredOdd() {
        return predOdd;
    }

    static Predicate<Number> getPredValGreaterTen() {
        return predValGreaterTen;
    }

    static Collection<String> getZiminStrings() {
        return ziminStrings;
    }

    static List<Integer> getZiminLengths() {
        return ziminLengths;
    }

    static Function1<String, Integer> getFunLength() {
        return funLength;
    }

    static Function1<String, String> getFunUppercase() {
        return funUppercase;
    }

    static Function1<String, Integer> getFunToUppercaseLength() {
        return funToUppercaseLength;
    }

    static Predicate<CharSequence> getPredLengthGreaterThree() {
        return predLengthGreaterThree;
    }
}
