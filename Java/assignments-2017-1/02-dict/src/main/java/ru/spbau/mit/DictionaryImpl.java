package ru.spbau.mit;


public class DictionaryImpl implements Dictionary {
    private static final int INITIAL_SIZE = 5;
    private static final double MAX_LOAD_FACTOR = 0.75;
    private static final double MIN_LOAD_FACTOR = 0.25;
    private static final int GROW_MAGNITUDE = 2;

    private int numberOfBuckets;
    private StringListNode[] buckets;
    private int numberOfKeys;

    DictionaryImpl() {
        // Ensure that newly created dictionary is the same as a cleared one.
        clear();
    }

    private static int findBucketIndex(String key, int numBuckets) {
        return Math.abs(key.hashCode()) % numBuckets;
    }

    /**
     * @return the number of keys
     */
    @Override
    public int size() {
        return numberOfKeys;
    }

    /**
     * @param key
     * @return true iff the dictionary contains `key`
     */
    @Override
    public boolean contains(String key) {
        StringListNode containedNode = getNodeByKeyOrNull(key);
        return containedNode != null;
    }

    /**
     * @param key
     * @return mapping from the given key to it's value, or null, if the dictionary
     * does not contain the key
     */
    @Override
    public String get(String key) {
        StringListNode containedNode = getNodeByKeyOrNull(key);
        return (containedNode == null ? null : containedNode.getValue());
    }

    /**
     * Put new key-value pair into the dictionary,
     * or update the value for the given key if it already exists
     *
     * @param key
     * @param value
     * @return if the dictionary already contains the key, return an old value,
     * otherwise return null
     */
    @Override
    public String put(String key, String value) {
        StringListNode containedNode = getNodeByKeyOrNull(key);

        if (containedNode != null) {
            String oldValue = containedNode.getValue();
            containedNode.setValue(value);
            return oldValue;
        }

        int bucketIndex = findBucketIndex(key);
        StringListNode newNode = new StringListNode(key, value);
        buckets[bucketIndex] = StringListNode.mergeListWithNode(buckets[bucketIndex], newNode);
        numberOfKeys++;
        if (calcLoadFactor() > MAX_LOAD_FACTOR) {
            growBuckets();
        }

        return null;
    }

    private void growBuckets() {
        rehash(numberOfBuckets * GROW_MAGNITUDE);
    }

    /**
     * Remove the key-value from the dictionary if it exists there
     *
     * @param key
     * @return an associated value to the key or null if the dictionary
     * doesn't contain the key
     */
    @Override
    public String remove(String key) {
        int bucketIndex = findBucketIndex(key);
        StringListNode listHead = buckets[bucketIndex];
        String containedValue = get(key);

        if (containedValue != null) {
            buckets[bucketIndex] = StringListNode.removeByKey(listHead, key);
            numberOfKeys--;

            if (calcLoadFactor() < MIN_LOAD_FACTOR) {
                shrinkBuckets();
            }
        }

        return containedValue;
    }

    private void shrinkBuckets() {
        int nextNumberOfBuckets = numberOfBuckets / GROW_MAGNITUDE;
        nextNumberOfBuckets = Math.max(INITIAL_SIZE, nextNumberOfBuckets);
        rehash(nextNumberOfBuckets);
    }

    /**
     * Remove all key-value pairs from the dictionary
     */
    @Override
    public void clear() {
        numberOfBuckets = INITIAL_SIZE;
        buckets = new StringListNode[numberOfBuckets];
        numberOfKeys = 0;
    }

    private double calcLoadFactor() {
        return numberOfKeys / (double) numberOfBuckets;
    }

    private StringListNode getNodeByKeyOrNull(String key) {
        StringListNode listHead = buckets[findBucketIndex(key)];
        return StringListNode.getNodeByKeyOrNull(listHead, key);
    }

    private int findBucketIndex(String key) {
        return Math.abs(key.hashCode()) % numberOfBuckets;
    }

    private void rehash(int nextNumberOfBuckets) {
        StringListNode[] newBuckets = new StringListNode[nextNumberOfBuckets];

        for (int i = 0; i < numberOfBuckets; i++) {
            while (buckets[i] != null) {
                StringListNode nextNode = buckets[i].getNextNode();
                buckets[i].setNextNode(null);

                String key = buckets[i].getKey();
                int newBucketIdx = findBucketIndex(key, nextNumberOfBuckets);
                newBuckets[newBucketIdx] = StringListNode.mergeListWithNode(newBuckets[newBucketIdx], buckets[i]);

                buckets[i] = nextNode;
            }
        }

        numberOfBuckets = nextNumberOfBuckets;
        buckets = newBuckets;
    }

    private static final class StringListNode {
        private String key;
        private String value;
        private StringListNode nextNode;

        private StringListNode(String key, String value) {
            this.key = key;
            this.value = value;
            nextNode = null;
        }

        /**
         * Remove an element with key equal to @key from list pointed by @head.
         * Invariant: each key is presented no more than once.
         *
         * @param head The head of the list.
         * @param key  The key to be searched for removing.
         * @return A head of the new list with removed element.
         */
        private static StringListNode removeByKey(StringListNode head, String key) {
            if (head == null) {
                return null;
            }

            if (head.getKey().equals(key)) {
                return head.getNextNode();
            }

            StringListNode prevNode = head;
            StringListNode curNode = head.getNextNode();

            while (curNode != null) {
                if (curNode.getKey().equals(key)) {
                    prevNode.setNextNode(curNode.getNextNode());
                    break;
                }

                prevNode = curNode;
                curNode = curNode.getNextNode();
            }

            return head;
        }

        private static StringListNode mergeListWithNode(StringListNode oldHead, StringListNode node) {
            node.setNextNode(oldHead);
            return node;
        }

        private static StringListNode getNodeByKeyOrNull(StringListNode head, String key) {
            while (head != null) {
                if (head.getKey().equals(key)) {
                    return head;
                }
                head = head.getNextNode();
            }

            return null;
        }

        private String getKey() {
            return key;
        }

        private String getValue() {
            return value;
        }

        private void setValue(String value) {
            this.value = value;
        }

        private StringListNode getNextNode() {
            return nextNode;
        }

        private void setNextNode(StringListNode nextNode) {
            this.nextNode = nextNode;
        }
    }
}
