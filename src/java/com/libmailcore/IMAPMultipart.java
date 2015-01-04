package com.libmailcore;

public class IMAPMultipart extends AbstractMultipart {
    public native void setPartID(String partID);
    public native String partID();
    private native void setupNative();
}
