package com.libmailcore;

/** Callbacks interface for the connection logger. */
public interface ConnectionLogger {
    /**
        Called when a new connection logs is added.
        @param param has one of the value of ConnectionLogType.
        @see ConnectionLogType.
    */
    void log(long connectionID, int type, byte[] data);
}
