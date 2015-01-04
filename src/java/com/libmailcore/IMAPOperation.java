package com.libmailcore;

public class IMAPOperation extends Operation {
    public MailException exception() {
        if (errorCode() == Error.ErrorNone) {
            return null;
        }
        return new MailException(errorCode());
    }

    private native int errorCode();
    
    protected void callCallback()
    {
        if (callback != null) {
            if (errorCode() == Error.ErrorNone) {
                callback.succeeded();
            }
            else {
                callback.failed(exception());
            }
        }
    }
}
