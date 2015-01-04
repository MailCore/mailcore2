package com.libmailcore;

public interface IMAPOperationItemProgressListener {
    void itemProgress(long current, long maximum);
}
