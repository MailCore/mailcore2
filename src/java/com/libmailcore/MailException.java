package com.libmailcore;

public class MailException extends Exception {
    public int errorCode()
    {
        return errorCode;
    }
    
    public MailException(String message, int anErrorCode)
    {
        super(message);
        errorCode = anErrorCode;
    }
    
    public MailException(int anErrorCode)
    {
        super(messageForErrorCode(anErrorCode));
        errorCode = anErrorCode;
    }
    
    private static native String messageForErrorCode(int errorCode);
    
    private int errorCode;
}