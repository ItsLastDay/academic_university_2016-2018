package ru.spbau.mit;

import org.junit.Test;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.List;

import static org.junit.Assert.assertEquals;

public class CollectionsTest {
    @Test
    public void testMap() {
        ArrayList<Integer> lengths = new ArrayList<>();
        lengths.addAll(Collections.map(TestUtil.getFunToUppercaseLength(), TestUtil.getZiminStrings()));
        assertEquals(TestUtil.getZiminLengths(), lengths);
    }

    @Test
    public void testFilter() {
        Collection<String> filtered = Collections.filter(TestUtil.getPredLengthGreaterThree(),
                TestUtil.getZiminStrings());
        assertEquals(Arrays.asList("abacaba",
                "abacabadabacaba",
                "abacabadabacabaeabacabadabacaba"), filtered);
    }

    @Test
    public void testTakeWhile() {
        Collection<String> takenWhile = Collections.takeWhile(TestUtil.getPredLengthGreaterThree(),
                TestUtil.getZiminStrings());
        assertEquals(java.util.Collections.emptyList(), takenWhile);
    }

    @Test
    public void testTakeUnless() {
        Collection<String> takenUnless = Collections.takeUnless(TestUtil.getPredLengthGreaterThree(),
                TestUtil.getZiminStrings());
        assertEquals(Arrays.asList("a", "aba"), takenUnless);
    }

    @Test
    public void testFoldFactorial() {
        // factorial n = fold{r|l} (*) 1 [1..n]
        Collection<Integer> ints = Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

        int resL = Collections.foldl(TestUtil.getFunMultiply(), 1, ints);
        int resR = Collections.foldr(TestUtil.getFunMultiply(), 1, ints);

        assertEquals(resL, resR);
        assertEquals(3628800, resL);
    }

    @Test
    public void testFoldrFoldlUnequal() {
        Collection<Integer> ints = Arrays.asList(1, 2, 0, -4);

        // (((0 - 1) - 2) - 0) - (-4))
        int resFoldl = Collections.foldl(TestUtil.getFunSubtract(), 0, ints);
        // (1 - (2 - (0 - (-4 - 0))))
        int resFoldr = Collections.foldr(TestUtil.getFunSubtract(), 0, ints);

        assertEquals(1, resFoldl);
        assertEquals(3, resFoldr);
    }

    @Test
    public void testFoldCovariant() {
        List<Integer> ints = Arrays.asList(5, 11, -1, 23, 30);
        Function2<Object, Object, Integer> hashCodeSecondArg = new Function2<Object, Object, Integer>() {
            @Override
            public Integer apply(Object firstArg, Object secondArg) {
                return secondArg.hashCode();
            }
        };

        Integer lastHashCode = Collections.foldl(hashCodeSecondArg, 12345, ints);
        assertEquals(lastHashCode.intValue(), ints.get(ints.size() - 1).hashCode());
    }

    @Test
    public void testMapFuncWildcard() {
        Collection<Integer> ints = Arrays.asList(0, -1, -2, -3);
        Function1<Number, ArrayList<Number>> funSingleElem = new Function1<Number, ArrayList<Number>>() {
            @Override
            public ArrayList<Number> apply(Number obj) {
                return new ArrayList<>(java.util.Collections.singletonList(obj));
            }
        };

        Collection<List<Number>> mappedInts = Collections.<Number, List<Number>>map(funSingleElem, ints);
    }
}
