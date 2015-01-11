package com.libmailcore;

public class IMAPPart extends AbstractPart {
    public native void setPartID(String partID);
    public native String partID();
    
    public native void setSize(long size);
    public native long size();
    
    public native long decodedSize();
    
    public native void setEncoding(int encoding);
    public native int encoding();

    private static final long serialVersionUID = 1L;
}
