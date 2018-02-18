package ru.spbau.mit;

import java.io.InputStream;
import java.io.OutputStream;

public interface StreamSerializable {
    /**
     * @throws SerializationException in case of IOException during serialization
     */
    void serialize(OutputStream out);

    /**
     * Replace current state with data from input stream containing serialized data
     * @throws SerializationException in case of IOException during deserialization
     */
    void deserialize(InputStream in);
}
