package com.libmailcore;

import java.util.Date;

public class IMAPAppendMessageOperation extends IMAPOperation {
    public native void setDate(Date date);
    public native Date date();
    
    public native long createdUID();
}
