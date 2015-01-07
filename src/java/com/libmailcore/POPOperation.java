package com.libmailcore;

public class POPOperation extends Operation {
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
