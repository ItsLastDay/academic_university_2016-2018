package ru.spbau.mit;

import org.junit.Test;

import static org.junit.Assert.*;

public class DictionaryTest {
    private static final int NUM_LARGE_OPERATIONS = 100500;

    private static Dictionary instance() {
        try {
            return (Dictionary) Class.forName("ru.spbau.mit.DictionaryImpl").newInstance();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        throw new IllegalStateException("Error while class loading");
    }

    @Test
    public void testSimplePutGet() {
        Dictionary dict = instance();
        assertNull(dict.get("abc"));

        assertNull(dict.put("abc", "cde"));
        assertEquals("cde", dict.get("abc"));
    }

    @Test
    public void testOverwrite() {
        Dictionary dict = instance();

        assertNull(dict.put("1 2 3", "4 5"));
        assertEquals("4 5", dict.put("1 2 3", "qwe"));
        assertEquals("qwe", dict.get("1 2 3"));
    }

    @Test
    public void testContainsRemoved() {
        Dictionary dict = instance();

        assertFalse(dict.contains("key"));
        assertNull(dict.put("key", "value"));
        assertTrue(dict.contains("key"));
        assertEquals("value", dict.remove("key"));
        assertFalse(dict.contains(("key")));
    }

    @Test
    public void testClearSizeContains() {
        Dictionary dict = instance();

        int curSize = 0;
        assertNull(dict.put("a", "1"));
        curSize++;
        assertNull(dict.put("b", "1"));
        curSize++;
        assertNull(dict.put("c", "1"));
        curSize++;
        assertNull(dict.put("d", "1"));
        curSize++;
        assertEquals(curSize, dict.size());

        assertTrue(dict.contains("a"));
        assertTrue(dict.contains("b"));
        assertTrue(dict.contains("c"));
        assertTrue(dict.contains("d"));

        dict.clear();

        assertEquals(0, dict.size());

        assertFalse(dict.contains("a"));
        assertFalse(dict.contains("b"));
        assertFalse(dict.contains("c"));
        assertFalse(dict.contains("d"));
    }

    @Test
    public void testBigAdd() {
        Dictionary dict = instance();

        for (int i = 0; i < NUM_LARGE_OPERATIONS; i++) {
            String iString = Integer.toString(i);
            assertNull(dict.put(iString, iString));
        }

        assertEquals(NUM_LARGE_OPERATIONS, dict.size());

        for (int i = 0; i < NUM_LARGE_OPERATIONS; i++) {
            String iString = Integer.toString(i);
            assertEquals(iString, dict.get(iString));
        }

        for (int i = 0; i < NUM_LARGE_OPERATIONS; i++) {
            String iString = Integer.toString(i);
            assertEquals(iString, dict.remove(iString));
        }

        assertEquals(0, dict.size());
    }

    @Test
    public void testGetUnseen() {
        Dictionary dict = instance();

        for (int i = 0; i < NUM_LARGE_OPERATIONS; i++) {
            String iString = Integer.toString(i);
            assertNull(dict.put(iString, iString));
        }

        for (int i = NUM_LARGE_OPERATIONS; i < NUM_LARGE_OPERATIONS + NUM_LARGE_OPERATIONS; i++) {
            String iString = Integer.toString(i);
            assertNull(dict.get(iString));
        }

        dict.clear();

        for (int i = 0; i < NUM_LARGE_OPERATIONS; i++) {
            String iString = Integer.toString(i);
            assertNull(dict.get(iString));
        }
    }
}
