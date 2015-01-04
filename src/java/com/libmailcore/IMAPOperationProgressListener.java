package com.libmailcore;

public interface IMAPOperationProgressListener {
    void bodyProgress(long current, long maximum);
}
