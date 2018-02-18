package ru.spbau.mit;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public final class LoggingExample {
    private static final Logger LOG = LogManager.getLogger(LoggingExample.class);

    private LoggingExample() {
    }


    public static void main(String[] args) {
        LOG.error("This is a error - should be in console, but not in file.");
        LOG.fatal("This is a fatal - should be in file.");
        LOG.info("This is info - should be in file.");
        LOG.warn("This is warn - should be in file.");
        LOG.debug("This is debug - should *not* be visible.");
        LOG.trace("This is trace - should *not* be visible.");
    }
}
