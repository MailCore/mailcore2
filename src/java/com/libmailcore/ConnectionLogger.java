package com.libmailcore;

public interface ConnectionLogger {
    void log(long connectionID, int type, byte[] data);
}
