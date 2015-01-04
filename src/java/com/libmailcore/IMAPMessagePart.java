package com.libmailcore;

public class IMAPMessagePart extends AbstractMessagePart {
    public native void setPartID(String partID);
    public native String partID();
}
