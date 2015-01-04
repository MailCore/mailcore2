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
    
    private static String messageForErrorCode(int errorCode)
    {
        return "Error";
    }
    
    static final int ERROR_NONE = 0;
    static final int ERROR_CONNECTION = 1;
    static final int ERROR_TLS_NOT_AVAILABLE = 2;
    static final int ERROR_PARSE = 3;
    static final int ERROR_CERTIFICATE = 4;
    static final int ERROR_AUTHENTICATION = 5;
    static final int ERROR_GMAIL_IMAP_NOT_ENABLED = 6;
    static final int ERROR_GMAIL_EXCEED_BANDWIDTH_LIMIT = 7;
    static final int ERROR_GMAIL_TOO_MANY_SIMULTANEOUS_CONNECTIONS = 8;
    static final int ERROR_MOBILEME_MOVED = 9;
    static final int ERROR_YAHOO_UNAVAILABLE = 10;
    static final int ERROR_NON_EXISTANT_FOLDER = 11;
    static final int ERROR_RENAME = 12;
    static final int ERROR_DELETE = 13;
    static final int ERROR_CREATE = 14;
    static final int ERROR_SUBSCRIBED = 15;
    static final int ERROR_APPEND = 16;
    static final int ERROR_COPY = 17;
    static final int ERROR_EXPUNGE = 18;
    static final int ERROR_FETCH = 19;
    static final int ERROR_IDLE = 20;
    static final int ERROR_IDENTITY = 21;
    static final int ERROR_NAMESPACE = 22;
    static final int ERROR_STORE = 23;
    static final int ERROR_CAPABILITY = 24;
    static final int ERROR_STARTTLS_NOT_AVAILABLE = 25;
    static final int ERROR_SEND_MESSAGE_ILLEGAL_ATTACHMENT = 26;
    static final int ERROR_STORAGE_LIMIT = 27;
    static final int ERROR_SEND_MESSAGE_NOT_ALLOWED = 28;
    static final int ERROR_NEEDS_CONNECT_TO_WEBMAIL = 29;
    static final int ERROR_SEND_MESSAGE = 30;
    static final int ERROR_AUTHENTICATION_REQUIRED = 31;
    static final int ERROR_FETCH_MESSAGE_LIST = 32;
    static final int ERROR_DELETE_MESSAGE = 33;
    static final int ERROR_INVALID_ACCOUNT = 34;
    static final int ERROR_FILE = 35;
    static final int ERROR_COMPRESSION = 36;
    static final int ERROR_NO_SENDER = 37;
    static final int ERROR_NO_RECIPIENT = 38;
    static final int ERROR_NOOP = 39;
    static final int ERROR_GMAIL_APPLICATION_SPECIFIC_PASSWORD_REQUIRED = 40;
    static final int ERROR_SERVER_DATE = 41;
    static final int ERROR_COUNT = 42;
    
    private int errorCode;
}