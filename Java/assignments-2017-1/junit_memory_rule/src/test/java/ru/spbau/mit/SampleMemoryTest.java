package ru.spbau.mit;

import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;

public class SampleMemoryTest {
    @Rule
    public final MemoryLeakLimit memoryLimit = new MemoryLeakLimit();

    @Rule
    public ExpectedException thrown = ExpectedException.none();

    @Test
    public void testDoNothingNonzeroLimit() {
        memoryLimit.limit(4);
    }

    @Test
    public void testDoNothingZeroLimit() {
        memoryLimit.limit(0);
    }

    @Test
    public void testCreateGarbageViolateLimit() {
        memoryLimit.limit(1);
        MemoryConsumer consumer = new MemoryConsumer();
    }

    @Test
    public void testCreateGarbageNotViolateLimit() {
        memoryLimit.limit(1);
        MemoryConsumerSmall consumer = new MemoryConsumerSmall();
    }

    @Test
    public void testViolateLimitWithException() {
        // Expected result: test fails, because memory limit is violated.
        memoryLimit.limit(1);
        thrown.expect(RuntimeException.class);
        MemoryConsumer2 consumer = new MemoryConsumer2();
        throw new RuntimeException();
    }
}
