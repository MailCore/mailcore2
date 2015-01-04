package com.libmailcore;

public interface OperationQueueListener {
    void queueStartRunning();
    void queueStoppedRunning();
}

