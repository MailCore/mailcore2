package com.libmailcore;

public class POPMessageInfo extends NativeObject {
    public POPMessageInfo()
    {
        setupNative();
    }
    
    public native void setIndex(int index);
    public native int index();
    
    public native void setSize(long size);
    public native long size();
    
    public native void setUid(String uid);
    public native String uid();
    
    private native void setupNative();
}
