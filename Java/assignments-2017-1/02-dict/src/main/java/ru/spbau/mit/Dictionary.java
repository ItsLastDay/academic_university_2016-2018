package ru.spbau.mit;

public interface Dictionary {
    /**
     * @return the number of keys
     */
    int size();

    /**
     * @param key
     * @return true iff the dictionary contains `key`
     */
    boolean contains(String key);

    /**
     * @param key
     * @return mapping from the given key to it's value, or null, if the dictionary
     * does not contain the key
     */
    String get(String key);

    /**
     * Put new key-value pair into the dictionary,
     * or update the value for the given key if it already exists
     * @param key
     * @param value
     * @return if the dictionary already contains the key, return an old value,
     * otherwise return null
     */
    String put(String key, String value);

    /**
     * Remove the key-value from the dictionary if it exists there
     * @param key
     * @return an associated value to the key or null if the dictionary
     * doesn't contain the key
     */
    String remove(String key);

    /**
     * Remove all key-value pairs from the dictionary
     */
    void clear();
}
