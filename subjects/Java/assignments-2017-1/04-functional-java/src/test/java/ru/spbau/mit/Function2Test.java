package ru.spbau.mit;

import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class Function2Test {
    @Test
    public void testBind() {
        Function1<Integer, Integer> minusHundred = TestUtil.getFunSubtract().bind2(100);
        Function1<Integer, Integer> hundredMinus = TestUtil.getFunSubtract().bind1(100);

        assertEquals(5, TestUtil.getFunSubtract().apply(15, 10).intValue());
        assertEquals(430, minusHundred.apply(530).intValue());
        assertEquals(-430, hundredMinus.apply(530).intValue());
    }

    @Test
    public void testCurry() {
        Function1<Integer, Function1<Integer, Integer>> curried = TestUtil.getFunMultiply().curry();
        assertEquals(6, curried.apply(2).apply(3).intValue());
        assertEquals(-6, curried.apply(-2).apply(3).intValue());
    }

    @Test
    public void testApply() {
        assertEquals(132, TestUtil.getFunMultiply().apply(11, 12).intValue());
    }

    @Test
    public void testCompose() {
        Function2<Integer, Integer, Integer> negMult = TestUtil.getFunMultiply().compose(TestUtil.getFunNegate());

        assertEquals(-6, negMult.apply(2, 3).intValue());
        assertEquals(6, negMult.apply(-2, 3).intValue());
        assertEquals(6, negMult.apply(2, -3).intValue());
    }
}
