package com.libmailcore;

public class SMTPOperation extends Operation {
    /**
        Returns error once the operation finished running. It will return null if the operation
        successfully ran.
    */
    public MailException exception() {
        if (errorCode() == ErrorCode.ErrorNone) {
            return null;
        }
        return new MailException(errorCode());
    }

    private native int errorCode();
    
    protected void callCallback()
    {
        if (callback != null) {
            if (errorCode() == ErrorCode.ErrorNone) {
                callback.succeeded();
            }
            else {
                callback.failed(exception());
            }
        }
    }
}
