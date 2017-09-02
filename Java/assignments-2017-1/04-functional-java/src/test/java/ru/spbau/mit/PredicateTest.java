package ru.spbau.mit;

import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class PredicateTest {
    @Test
    public void testApply() {
        assertEquals(true, TestUtil.getPredOdd().apply(3));
        assertEquals(false, TestUtil.getPredOdd().apply(2));
    }

    @Test
    public void testNot() {
        Predicate<Integer> predEven = TestUtil.getPredOdd().not();
        assertEquals(false, predEven.apply(3));
        assertEquals(true, predEven.apply(2));
    }

    @Test
    public void testAnd() {
        Predicate<Integer> predOddAndGreaterTen = TestUtil.getPredOdd().and(TestUtil.getPredValGreaterTen());
        assertEquals(true, predOddAndGreaterTen.apply(13));
        assertEquals(false, predOddAndGreaterTen.apply(9));
        assertEquals(false, predOddAndGreaterTen.apply(666));
        assertEquals(false, predOddAndGreaterTen.apply(8));
    }

    @Test
    public void testOr() {
        Predicate<Integer> predOddOrGreaterTen = TestUtil.getPredOdd().or(TestUtil.getPredValGreaterTen());
        assertEquals(true, predOddOrGreaterTen.apply(15));
        assertEquals(true, predOddOrGreaterTen.apply(666));
        assertEquals(true, predOddOrGreaterTen.apply(3));
        assertEquals(false, predOddOrGreaterTen.apply(2));
    }

    @Test
    public void testPredicateLaziness() {
        Predicate<Object> lazyTrue = Predicate.ALWAYS_TRUE.or(TestUtil.getPredThrower());
        assertEquals(true, lazyTrue.apply(1));
        Predicate<Object> lazyFalse = Predicate.ALWAYS_FALSE.and(TestUtil.getPredThrower());
        assertEquals(false, lazyFalse.apply(1));
    }
}
