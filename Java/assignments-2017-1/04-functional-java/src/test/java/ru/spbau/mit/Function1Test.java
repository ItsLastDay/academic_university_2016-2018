package ru.spbau.mit;

import org.junit.Test;

import static org.junit.Assert.assertEquals;


public class Function1Test {

    @Test
    public void testApply() {
        assertEquals(4, TestUtil.getFunNegate().apply(-4).intValue());
        assertEquals(-4, TestUtil.getFunNegate().apply(4).intValue());
    }

    @Test
    public void testCompose() {
        Function1<Integer, Integer> negateThenAddOne = TestUtil.getFunNegate().compose(TestUtil.getFunAddOne());
        assertEquals(-4, negateThenAddOne.apply(5).intValue());
        Function1<Integer, Integer> addOneThenNegate = TestUtil.getFunAddOne().compose(TestUtil.getFunNegate());
        assertEquals(-6, addOneThenNegate.apply(5).intValue());
    }
}
