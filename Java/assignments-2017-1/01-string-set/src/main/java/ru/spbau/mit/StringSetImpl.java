package ru.spbau.mit;


public class StringSetImpl implements StringSet {
    private TrieNode trieRoot = new TrieNode();

    private static class TrieNode {
        private static final int ENGLISH_ALPHABET_SIZE = 26;

        private boolean isTerminal = false;
        private int subtreeCount = 0;

        // Only letters 'a'-'z' 'A'-'Z' are allowed, so jump table has the size twice of the english alphabet.
        private TrieNode[] nextState = new TrieNode[ENGLISH_ALPHABET_SIZE * 2];

        private static int convertCharToIndex(char character) {
            if ('a' <= character && character <= 'z') {
                return character - 'a';
            }
            return character - 'A' + ENGLISH_ALPHABET_SIZE;
        }

        boolean isTerminal() {
            return isTerminal;
        }

        void setTerminal(boolean terminal) {
            isTerminal = terminal;
        }

        int getSubtreeCount() {
            return subtreeCount;
        }

        void addSubtreeCount(int addend) {
            subtreeCount += addend;
        }

        void removeNextState(char nextCharacter) {
            nextState[convertCharToIndex(nextCharacter)] = null;
        }

        void createNextState(char nextCharacter) {
            nextState[convertCharToIndex(nextCharacter)] = new TrieNode();
        }

        TrieNode getNextState(char nextCharacter) {
            return nextState[convertCharToIndex(nextCharacter)];
        }

        boolean canJumpTo(char nextCharacter) {
            return nextState[convertCharToIndex(nextCharacter)] != null;
        }
    }

    /**
     * Expected complexity: O(|element|)
     *
     * @return <tt>true</tt> if this set did not already contain the specified
     * element
     */
    @Override
    public boolean add(String element) {
        TrieNode state = getNodeOrNull(element, false);

        // `state.isTerminal` will not result in NPE, because `getNodeOrNull` with `false` as the second
        // argument will always return a valid node.
        if (state.isTerminal()) {
            return false;
        }

        addPrefixVal(element, 1);
        state.setTerminal(true);

        return true;
    }

    /**
     * Expected complexity: O(|element|)
     *
     */
    @Override
    public boolean contains(String element) {
        TrieNode state = getNodeOrNull(element, true);

        return (state != null && state.isTerminal());
    }

    /**
     * Expected complexity: O(|element|)
     *
     * @return <tt>true</tt> if this set contained the specified element
     */
    @Override
    public boolean remove(String element) {
        TrieNode state = getNodeOrNull(element, true);

        if (state == null) {
            return false;
        }

        addPrefixVal(element, -1);
        state.setTerminal(false);

        return true;
    }

    /**
     * Expected complexity: O(1)
     */
    @Override
    public int size() {
        return trieRoot.getSubtreeCount();
    }

    /**
     * Expected complexity: O(|prefix|)
     *
     */
    @Override
    public int howManyStartsWithPrefix(String prefix) {
        TrieNode state = getNodeOrNull(prefix, true);

        return (state == null) ? 0 : state.getSubtreeCount();
    }

    private void addPrefixVal(String element, int addend) {
        TrieNode curState = trieRoot;

        curState.addSubtreeCount(addend);
        for (int i = 0; i < element.length(); i++) {
            char offset = element.charAt(i);
            TrieNode nextState = curState.getNextState(offset);
            nextState.addSubtreeCount(addend);

            if (nextState.getSubtreeCount() == 0) {
                curState.removeNextState(offset);
            }

            curState = nextState;
        }
    }

    private TrieNode getNodeOrNull(String element, boolean failOnNoJump) {
        TrieNode curState = trieRoot;

        for (int i = 0; i < element.length(); i++) {
            char offset = element.charAt(i);
            if (!curState.canJumpTo(offset)) {
                if (failOnNoJump) {
                    return null;
                }

                curState.createNextState(offset);
            }
            curState = curState.getNextState(offset);
        }

        return curState;
    }
}
