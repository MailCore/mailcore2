package com.libmailcore;

public class IMAPMessagePart extends AbstractMessagePart {
    public native void setPartID(String partID);
    public native String partID();

    private static final long serialVersionUID = 1L;
}
