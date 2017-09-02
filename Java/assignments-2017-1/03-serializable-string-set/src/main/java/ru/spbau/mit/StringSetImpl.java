package ru.spbau.mit;


import java.io.*;

public class StringSetImpl implements StringSet, StreamSerializable {
    private TrieNode trieRoot = new TrieNode();

    /**
     * @param out
     * @throws SerializationException in case of IOException during serialization
     */
    @Override
    public void serialize(OutputStream out) {
        trieRoot.serialize(out);
    }

    /**
     * Replace current state with data from input stream containing serialized data
     *
     * @param in
     * @throws SerializationException in case of IOException during deserialization
     */
    @Override
    public void deserialize(InputStream in) {
        trieRoot.deserialize(in);
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

    private static class TrieNode implements StreamSerializable {
        private static final int ENGLISH_ALPHABET_SIZE = 26;
        private static final int JUMP_TABLE_SIZE = 2 * ENGLISH_ALPHABET_SIZE;

        private boolean isTerminal = false;
        private int subtreeCount = 0;

        // Only letters 'a'-'z' 'A'-'Z' are allowed, so jump table has the size twice of the english alphabet.
        private TrieNode[] nextState = new TrieNode[JUMP_TABLE_SIZE];

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

        private void doSerialize(OutputStream out) throws IOException {
            DataOutputStream serialWriter = new DataOutputStream(out);

            serialWriter.writeBoolean(isTerminal);

            for (int i = 0; i < JUMP_TABLE_SIZE; i++) {
                if (nextState[i] == null) {
                    continue;
                }
                serialWriter.writeByte(i);
                nextState[i].serialize(out);
            }
            serialWriter.writeByte(JUMP_TABLE_SIZE + 1);
        }

        /**
         * @param out
         * @throws SerializationException in case of IOException during serialization
         */
        @Override
        public void serialize(OutputStream out) {
            try {
                doSerialize(out);
            } catch (IOException e) {
                SerializationException exc = new SerializationException();
                exc.initCause(e);
                throw exc;
            }
        }

        private void doDeserialize(InputStream in) throws IOException {
            DataInputStream serialReader = new DataInputStream(in);

            isTerminal = serialReader.readBoolean();
            subtreeCount = isTerminal ? 1 : 0;

            int nextStateIdx = serialReader.readByte();
            while (nextStateIdx != JUMP_TABLE_SIZE + 1) {
                nextState[nextStateIdx] = new TrieNode();
                nextState[nextStateIdx].doDeserialize(in);
                subtreeCount += nextState[nextStateIdx].subtreeCount;

                nextStateIdx = serialReader.readByte();
            }
        }

        /**
         * Replace current state with data from input stream containing serialized data
         *
         * @param in
         * @throws SerializationException in case of IOException during deserialization
         */
        @Override
        public void deserialize(InputStream in) {
            try {
                doDeserialize(in);
            } catch (IOException e) {
                SerializationException exc = new SerializationException();
                exc.initCause(e);
                throw exc;
            }
        }
    }
}
