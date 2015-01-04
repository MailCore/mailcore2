package com.libmailcore;

public interface OperationCallback {
    void succeeded();
    void failed(MailException exception);
}
